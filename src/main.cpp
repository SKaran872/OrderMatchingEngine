#include "MatchingEngine.h"
#include <iostream>
#include <memory>
#include <chrono>
#include <thread>

using namespace MatchingEngine;

int main() {
    std::cout << "Starting Matching Engine Simulation...\n\n";

    Engine engine;

    // Simulate placing orders
    std::cout << "[1] Adding buy orders...\n";
    engine.addOrder(std::make_shared<Order>("B1", "AAPL", Side::BUY, OrderType::LIMIT, 150.0, 100));
    engine.addOrder(std::make_shared<Order>("B2", "AAPL", Side::BUY, OrderType::LIMIT, 150.0, 50));
    engine.addOrder(std::make_shared<Order>("B3", "AAPL", Side::BUY, OrderType::LIMIT, 149.0, 200));

    engine.printBook("AAPL");

    std::cout << "\n[2] Adding sell orders (no crossing)...\n";
    engine.addOrder(std::make_shared<Order>("S1", "AAPL", Side::SELL, OrderType::LIMIT, 151.0, 100));
    engine.addOrder(std::make_shared<Order>("S2", "AAPL", Side::SELL, OrderType::LIMIT, 152.0, 300));

    engine.printBook("AAPL");

    std::cout << "\n[3] Making a trade matching best bid...\n";
    // Crosses best bid fully
    engine.addOrder(std::make_shared<Order>("S3", "AAPL", Side::SELL, OrderType::LIMIT, 150.0, 120));
    
    engine.printBook("AAPL");

    std::cout << "\n[4] Cancelling an order...\n";
    engine.cancelOrder("AAPL", "B3");
    
    engine.printBook("AAPL");

    std::cout << "\nSimulation Finished.\n";

    return 0;
}
