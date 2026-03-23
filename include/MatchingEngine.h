#pragma once

#include "LimitOrderBook.h"
#include <unordered_map>
#include <string>
#include <memory>
#include <shared_mutex>
#include <vector>
#include <atomic>

namespace MatchingEngine {

class Engine {
private:
    std::unordered_map<std::string, std::unique_ptr<LimitOrderBook>> orderBooks;
    mutable std::shared_mutex engineMutex;

    mutable std::mutex latencyMutex;
    std::vector<long long> latencies_us;

public:
    void addOrder(std::shared_ptr<Order> order);
    void cancelOrder(const std::string& symbol, const std::string& orderId);
    void printBook(const std::string& symbol) const;
    void printLatencyStats() const;
};

} // namespace MatchingEngine
