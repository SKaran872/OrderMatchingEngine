#pragma once

#include "IOrderBook.h"
#include <map>
#include <list>
#include <unordered_map>
#include <string>

namespace MatchingEngine {

// Inheritance and Encapsulation
class LimitOrderBook : public IOrderBook {
private:
    std::string symbol;

    // Bids (Buy orders): highest price first -> std::greater
    std::map<double, std::list<std::shared_ptr<Order>>, std::greater<double>> bids;

    // Asks (Sell orders): lowest price first -> std::less
    std::map<double, std::list<std::shared_ptr<Order>>, std::less<double>> asks;

    // Fast lookup for O(1) order cancellation or finding
    // Maps orderId -> iterator inside the list, plus a flag for side to know which map it's in.
    struct OrderLocation {
        std::shared_ptr<Order> order;
        std::list<std::shared_ptr<Order>>::iterator iterator;
    };
    std::unordered_map<std::string, OrderLocation> ordersMap;

    void match();

public:
    explicit LimitOrderBook(std::string sym);
    
    // Polymorphism: overriding base class methods
    void addOrder(std::shared_ptr<Order> order) override;
    void cancelOrder(const std::string& orderId) override;

    void printBook() const override;
};

} // namespace MatchingEngine
