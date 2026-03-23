#include "MatchingEngine.h"
#include <iostream>
#include <memory>
#include <chrono>
#include <thread>

using namespace MatchingEngine;

void runSimulations(Engine& engine, int threadId) {
    if (threadId == 1) {
        engine.addOrder(std::make_shared<Order>("B1_T1", "AAPL", Side::BUY, OrderType::LIMIT, 150.0, 100));
        engine.addOrder(std::make_shared<Order>("B2_T1", "AAPL", Side::BUY, OrderType::LIMIT, 150.0, 50));
        engine.addOrder(std::make_shared<Order>("B3_T1", "AAPL", Side::BUY, OrderType::LIMIT, 149.0, 200));
    } else if (threadId == 2) {
        engine.addOrder(std::make_shared<Order>("S1_T2", "AAPL", Side::SELL, OrderType::LIMIT, 151.0, 100));
        engine.addOrder(std::make_shared<Order>("S2_T2", "AAPL", Side::SELL, OrderType::LIMIT, 152.0, 300));
    } else if (threadId == 3) {
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
        engine.addOrder(std::make_shared<Order>("S3_T3", "AAPL", Side::SELL, OrderType::LIMIT, 150.0, 120));
    }
}

int main() {
    std::cout << "Starting Concurrent Matching Engine Simulation...\n\n";

    Engine engine;

    std::cout << "[1] Launching threads for concurrent orders...\n";
    std::vector<std::thread> threads;
    for (int i = 1; i <= 3; ++i) {
        threads.emplace_back(runSimulations, std::ref(engine), i);
    }

    // Wait for all threads to finish
    for (auto& t : threads) {
        t.join();
    }

    std::cout << "\n[2] Final Book State after Concurrent Orders:\n";
    engine.printBook("AAPL");

    std::cout << "\n[3] Cancelling an order...\n";
    engine.cancelOrder("AAPL", "B3_T1");
    
    engine.printBook("AAPL");

    engine.printLatencyStats();

    std::cout << "\nSimulation Finished.\n";

    return 0;
}
