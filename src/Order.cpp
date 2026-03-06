#include "Order.h"
#include <chrono>

namespace MatchingEngine {

Order::Order(std::string id, std::string sym, Side s, OrderType t, double p, long long q)
    : orderId(std::move(id)), symbol(std::move(sym)), side(s), type(t), price(p), quantity(q), initialQuantity(q) {
    timestamp = std::chrono::steady_clock::now().time_since_epoch().count();
}

void Order::fill(long long qty) {
    if (qty > quantity) {
        quantity = 0;
    } else {
        quantity -= qty;
    }
}

} // namespace MatchingEngine
