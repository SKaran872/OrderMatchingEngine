#include "MatchingEngine.h"
#include <iostream>
#include <chrono>
#include <numeric>
#include <algorithm>

namespace MatchingEngine {

void Engine::addOrder(std::shared_ptr<Order> order) {
    auto start_time = std::chrono::high_resolution_clock::now();

    const std::string& sym = order->getSymbol();
    LimitOrderBook* book = nullptr;

    {
        std::shared_lock<std::shared_mutex> readLock(engineMutex);
        auto it = orderBooks.find(sym);
        if (it != orderBooks.end()) {
            book = it->second.get();
        }
    }

    if (!book) {
        std::unique_lock<std::shared_mutex> writeLock(engineMutex);
        auto it = orderBooks.find(sym);
        if (it == orderBooks.end()) {
            orderBooks[sym] = std::make_unique<LimitOrderBook>(sym);
            book = orderBooks[sym].get();
        } else {
            book = it->second.get();
        }
    }

    book->addOrder(order);

    auto end_time = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time).count();

    std::lock_guard<std::mutex> lock(latencyMutex);
    latencies_us.push_back(duration);
}

void Engine::cancelOrder(const std::string& symbol, const std::string& orderId) {
    std::shared_lock<std::shared_mutex> readLock(engineMutex);
    auto it = orderBooks.find(symbol);
    if (it != orderBooks.end()) {
        it->second->cancelOrder(orderId);
    } else {
        std::cout << "Order book for " << symbol << " not found.\n";
    }
}

void Engine::printBook(const std::string& symbol) const {
    std::shared_lock<std::shared_mutex> readLock(engineMutex);
    auto it = orderBooks.find(symbol);
    if (it != orderBooks.end()) {
        it->second->printBook();
    } else {
        std::cout << "Order book for " << symbol << " not found.\n";
    }
}

void Engine::printLatencyStats() const {
    std::lock_guard<std::mutex> lock(latencyMutex);
    if (latencies_us.empty()) {
        std::cout << "No latency data available.\n";
        return;
    }
    long long sum = std::accumulate(latencies_us.begin(), latencies_us.end(), 0LL);
    double avg = static_cast<double>(sum) / latencies_us.size();
    
    auto min_it = std::min_element(latencies_us.begin(), latencies_us.end());
    auto max_it = std::max_element(latencies_us.begin(), latencies_us.end());

    std::cout << "\n=== Latency Statistics ===\n";
    std::cout << "Total Orders Processed : " << latencies_us.size() << "\n";
    std::cout << "Average Latency        : " << avg << " us\n";
    std::cout << "Min Latency            : " << *min_it << " us\n";
    std::cout << "Max Latency            : " << *max_it << " us\n";
    std::cout << "==========================\n";
}

} // namespace MatchingEngine
