#pragma once

#include <string>

namespace MatchingEngine {

enum class Side {
    BUY,
    SELL
};

enum class OrderType {
    LIMIT,
    MARKET
};

class Order {
private:
    std::string orderId;
    std::string symbol;
    Side side;
    OrderType type;
    double price;
    long long quantity;
    long long initialQuantity;
    long long timestamp;

public:
    Order(std::string id, std::string sym, Side s, OrderType t, double p, long long q);

    // Getters
    std::string getOrderId() const { return orderId; }
    std::string getSymbol() const { return symbol; }
    Side getSide() const { return side; }
    OrderType getType() const { return type; }
    double getPrice() const { return price; }
    long long getQuantity() const { return quantity; }
    long long getInitialQuantity() const { return initialQuantity; }
    long long getTimestamp() const { return timestamp; }

    // Setters
    void fill(long long qty);
    bool isFilled() const { return quantity == 0; }
};

} // namespace MatchingEngine
