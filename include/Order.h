#pragma once

#include <string>

using namespace std;

namespace MatchingEngine {

enum class Side {
    BUY,
    SELL
};

enum class OrderType {
    LIMIT,
    MARKET
};

enum class OrderStatus {
    NEW,
    PARTIALLY_FILLED,
    FILLED,
    CANCELED
};

class Order {
private:
    string orderId;
    string symbol;
    Side side;
    OrderType type;
    double price;
    long long quantity;
    long long initialQuantity;
    long long timestamp;
    OrderStatus status;

public:
    Order(string id, string sym, Side s, OrderType t, double p, long long q);

    // Getters
    string getOrderId() const { return orderId; }
    string getSymbol() const { return symbol; }
    Side getSide() const { return side; }
    OrderType getType() const { return type; }
    double getPrice() const { return price; }
    long long getQuantity() const { return quantity; }
    long long getInitialQuantity() const { return initialQuantity; }
    long long getTimestamp() const { return timestamp; }
    OrderStatus getStatus() const { return status; }

    // Setters
    void setStatus(OrderStatus s) { status = s; }
    void fill(long long qty);
    bool isFilled() const { return quantity == 0; }
};

} // namespace MatchingEngine
