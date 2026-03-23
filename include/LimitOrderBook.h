#pragma once

#include "IOrderBook.h"
#include <map>
#include <list>
#include <unordered_map>
#include <string>
#include <mutex>

using namespace std;

namespace MatchingEngine {

// Inheritance and Encapsulation
class LimitOrderBook : public IOrderBook {
private:
    string symbol;
    mutable mutex bookMutex;

    // Bids (Buy orders): highest price first -> greater
    map<double, list<shared_ptr<Order>>, greater<double>> bids;

    // Asks (Sell orders): lowest price first -> less
    map<double, list<shared_ptr<Order>>, less<double>> asks;

    // Fast lookup for O(1) order cancellation or finding
    // Maps orderId -> iterator inside the list, plus a flag for side to know which map it's in.
    struct OrderLocation {
        shared_ptr<Order> order;
        list<shared_ptr<Order>>::iterator iterator;
    };
    unordered_map<string, OrderLocation> ordersMap;

    void match();

public:
    explicit LimitOrderBook(string sym);
    
    // Polymorphism: overriding base class methods
    void addOrder(shared_ptr<Order> order) override;
    void cancelOrder(const string& orderId) override;

    void printBook() const override;
};

} // namespace MatchingEngine
