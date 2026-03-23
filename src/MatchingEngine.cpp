#include "MatchingEngine.h"
#include <iostream>
#include <chrono>
#include <numeric>
#include <algorithm>

using namespace std;

namespace MatchingEngine {

void Engine::addOrder(shared_ptr<Order> order) {
    auto start_time = chrono::high_resolution_clock::now();

    const string& sym = order->getSymbol();
    LimitOrderBook* book = nullptr;

    {
        shared_lock<shared_mutex> readLock(engineMutex);
        auto it = orderBooks.find(sym);
        if (it != orderBooks.end()) {
            book = it->second.get();
        }
    }

    if (!book) {
        unique_lock<shared_mutex> writeLock(engineMutex);
        auto it = orderBooks.find(sym);
        if (it == orderBooks.end()) {
            orderBooks[sym] = make_unique<LimitOrderBook>(sym);
            book = orderBooks[sym].get();
        } else {
            book = it->second.get();
        }
    }

    book->addOrder(order);

    auto end_time = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::microseconds>(end_time - start_time).count();

    lock_guard<mutex> lock(latencyMutex);
    latencies_us.push_back(duration);
}

void Engine::cancelOrder(const string& symbol, const string& orderId) {
    shared_lock<shared_mutex> readLock(engineMutex);
    auto it = orderBooks.find(symbol);
    if (it != orderBooks.end()) {
        it->second->cancelOrder(orderId);
    } else {
        cout << "Order book for " << symbol << " not found.\n";
    }
}

void Engine::printBook(const string& symbol) const {
    shared_lock<shared_mutex> readLock(engineMutex);
    auto it = orderBooks.find(symbol);
    if (it != orderBooks.end()) {
        it->second->printBook();
    } else {
        cout << "Order book for " << symbol << " not found.\n";
    }
}

void Engine::printLatencyStats() const {
    lock_guard<mutex> lock(latencyMutex);
    if (latencies_us.empty()) {
        cout << "No latency data available.\n";
        return;
    }
    long long sum = accumulate(latencies_us.begin(), latencies_us.end(), 0LL);
    double avg = static_cast<double>(sum) / latencies_us.size();
    
    auto min_it = min_element(latencies_us.begin(), latencies_us.end());
    auto max_it = max_element(latencies_us.begin(), latencies_us.end());

    cout << "\n=== Latency Statistics ===\n";
    cout << "Total Orders Processed : " << latencies_us.size() << "\n";
    cout << "Average Latency        : " << avg << " us\n";
    cout << "Min Latency            : " << *min_it << " us\n";
    cout << "Max Latency            : " << *max_it << " us\n";
    cout << "==========================\n";
}

} // namespace MatchingEngine
