#include "LimitOrderBook.h"
#include <iostream>
#include <algorithm>

using namespace std;

namespace MatchingEngine {

LimitOrderBook::LimitOrderBook(string sym) : symbol(move(sym)) {}

void LimitOrderBook::addOrder(shared_ptr<Order> order) {
    if (order->getQuantity() <= 0) return;

    lock_guard<mutex> lock(bookMutex);

    if (order->getSide() == Side::BUY) {
        bids[order->getPrice()].push_back(order);
        auto it = prev(bids[order->getPrice()].end());
        ordersMap[order->getOrderId()] = {order, it};
    } else {
        asks[order->getPrice()].push_back(order);
        auto it = prev(asks[order->getPrice()].end());
        ordersMap[order->getOrderId()] = {order, it};
    }

    match();
}

void LimitOrderBook::cancelOrder(const string& orderId) {
    lock_guard<mutex> lock(bookMutex);
    auto it = ordersMap.find(orderId);
    if (it != ordersMap.end()) {
        auto& loc = it->second;
        auto order = loc.order;
        if (order->getSide() == Side::BUY) {
            bids[order->getPrice()].erase(loc.iterator);
            if (bids[order->getPrice()].empty()) {
                bids.erase(order->getPrice());
            }
        } else {
            asks[order->getPrice()].erase(loc.iterator);
            if (asks[order->getPrice()].empty()) {
                asks.erase(order->getPrice());
            }
        }
        ordersMap.erase(it);
        order->setStatus(OrderStatus::CANCELED);
        cout << "Order " << orderId << " cancelled.\n";
    }
}

void LimitOrderBook::match() {
    while (!bids.empty() && !asks.empty()) {
        auto highestBidIt = bids.begin();
        auto lowestAskIt = asks.begin();

        double bestBidPrice = highestBidIt->first;
        double bestAskPrice = lowestAskIt->first;

        if (bestBidPrice >= bestAskPrice) {
            auto& bidQueue = highestBidIt->second;
            auto& askQueue = lowestAskIt->second;

            if (bidQueue.empty()) {
                bids.erase(highestBidIt);
                continue;
            }
            if (askQueue.empty()) {
                asks.erase(lowestAskIt);
                continue;
            }

            auto bidOrder = bidQueue.front();
            auto askOrder = askQueue.front();

            long long tradeQuantity = min(bidOrder->getQuantity(), askOrder->getQuantity());
            double tradePrice = bidOrder->getTimestamp() < askOrder->getTimestamp() ? bidOrder->getPrice() : askOrder->getPrice();

            cout << "TRADE EXECUTED: " << tradeQuantity << " " << symbol 
                      << " @ " << tradePrice << " (" << bidOrder->getOrderId() 
                      << " vs " << askOrder->getOrderId() << ")";

            if (bidOrder->getQuantity() > tradeQuantity || askOrder->getQuantity() > tradeQuantity) {
                cout << " [PARTIAL FILL]\n";
            } else {
                cout << " [FULL FILL]\n";
            }

            bidOrder->fill(tradeQuantity);
            askOrder->fill(tradeQuantity);

            if (bidOrder->isFilled()) {
                ordersMap.erase(bidOrder->getOrderId());
                bidQueue.pop_front();
            }
            if (askOrder->isFilled()) {
                ordersMap.erase(askOrder->getOrderId());
                askQueue.pop_front();
            }

            if (bidQueue.empty()) bids.erase(highestBidIt);
            if (askQueue.empty()) asks.erase(lowestAskIt);

        } else {
            break;
        }
    }
}

void LimitOrderBook::printBook() const {
    lock_guard<mutex> lock(bookMutex);
    cout << "=== LIMIT ORDER BOOK: " << symbol << " ===\n";
    cout << "--- ASKS ---\n";
    for (auto it = asks.rbegin(); it != asks.rend(); ++it) {
        long long totalVol = 0;
        for (const auto& o : it->second) totalVol += o->getQuantity();
        cout << it->first << " : " << totalVol << "\n";
    }
    cout << "--- BIDS ---\n";
    for (auto it = bids.begin(); it != bids.end(); ++it) {
        long long totalVol = 0;
        for (const auto& o : it->second) totalVol += o->getQuantity();
        cout << it->first << " : " << totalVol << "\n";
    }
    cout << "================================\n";
}

} // namespace MatchingEngine
