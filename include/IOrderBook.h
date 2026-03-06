#pragma once

#include "Order.h"
#include <memory>
#include <string>

namespace MatchingEngine {

// Abstraction
class IOrderBook {
public:
    virtual ~IOrderBook() = default;
    virtual void addOrder(std::shared_ptr<Order> order) = 0;
    virtual void cancelOrder(const std::string& orderId) = 0;
    virtual void printBook() const = 0;
};

} // namespace MatchingEngine
