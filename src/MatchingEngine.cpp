#include "MatchingEngine.h"
#include <iostream>

namespace MatchingEngine {

void Engine::addOrder(std::shared_ptr<Order> order) {
    const std::string& sym = order->getSymbol();
    if (orderBooks.find(sym) == orderBooks.end()) {
        orderBooks[sym] = std::make_unique<LimitOrderBook>(sym);
    }
    orderBooks[sym]->addOrder(order);
}

void Engine::cancelOrder(const std::string& symbol, const std::string& orderId) {
    auto it = orderBooks.find(symbol);
    if (it != orderBooks.end()) {
        it->second->cancelOrder(orderId);
    } else {
        std::cout << "Order book for " << symbol << " not found.\n";
    }
}

void Engine::printBook(const std::string& symbol) const {
    auto it = orderBooks.find(symbol);
    if (it != orderBooks.end()) {
        it->second->printBook();
    } else {
        std::cout << "Order book for " << symbol << " not found.\n";
    }
}

} // namespace MatchingEngine
