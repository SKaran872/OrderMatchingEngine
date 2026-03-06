#pragma once

#include "LimitOrderBook.h"
#include <unordered_map>
#include <string>
#include <memory>

namespace MatchingEngine {

class Engine {
private:
    std::unordered_map<std::string, std::unique_ptr<LimitOrderBook>> orderBooks;

public:
    void addOrder(std::shared_ptr<Order> order);
    void cancelOrder(const std::string& symbol, const std::string& orderId);
    void printBook(const std::string& symbol) const;
};

} // namespace MatchingEngine
