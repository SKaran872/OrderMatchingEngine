#pragma once

#include "LimitOrderBook.h"
#include <unordered_map>
#include <string>
#include <memory>
#include <shared_mutex>
#include <vector>
#include <atomic>

using namespace std;

namespace MatchingEngine {

class Engine {
private:
    unordered_map<string, unique_ptr<LimitOrderBook>> orderBooks;
    mutable shared_mutex engineMutex;

    mutable mutex latencyMutex;
    vector<long long> latencies_us;

public:
    void addOrder(shared_ptr<Order> order);
    void cancelOrder(const string& symbol, const string& orderId);
    void printBook(const string& symbol) const;
    void printLatencyStats() const;
};

} // namespace MatchingEngine
