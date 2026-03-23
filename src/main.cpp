#include "MatchingEngine.h"
#include <iostream>
#include <memory>
#include <chrono>
#include <thread>

using namespace std;

using namespace MatchingEngine;

void runSimulations(Engine& engine, int threadId) {
    if (threadId == 1) {
        engine.addOrder(make_shared<Order>("B1_T1", "AAPL", Side::BUY, OrderType::LIMIT, 150.0, 100));
        engine.addOrder(make_shared<Order>("B2_T1", "AAPL", Side::BUY, OrderType::LIMIT, 150.0, 50));
        engine.addOrder(make_shared<Order>("B3_T1", "AAPL", Side::BUY, OrderType::LIMIT, 149.0, 200));
    } else if (threadId == 2) {
        engine.addOrder(make_shared<Order>("S1_T2", "AAPL", Side::SELL, OrderType::LIMIT, 151.0, 100));
        engine.addOrder(make_shared<Order>("S2_T2", "AAPL", Side::SELL, OrderType::LIMIT, 152.0, 300));
    } else if (threadId == 3) {
        this_thread::sleep_for(chrono::milliseconds(10));
        engine.addOrder(make_shared<Order>("S3_T3", "AAPL", Side::SELL, OrderType::LIMIT, 150.0, 120));
    }
}

int main() {
    cout << "Starting Concurrent Matching Engine Simulation...\n\n";

    Engine engine;

    cout << "[1] Launching threads for concurrent orders...\n";
    vector<thread> threads;
    for (int i = 1; i <= 3; ++i) {
        threads.emplace_back(runSimulations, ref(engine), i);
    }

    // Wait for all threads to finish
    for (auto& t : threads) {
        t.join();
    }

    cout << "\n[2] Final Book State after Concurrent Orders:\n";
    engine.printBook("AAPL");

    cout << "\n[3] Cancelling an order...\n";
    engine.cancelOrder("AAPL", "B3_T1");
    
    engine.printBook("AAPL");

    engine.printLatencyStats();

    cout << "\nSimulation Finished.\n";

    return 0;
}
