# Order Matching Engine

A high-performance algorithmic Order Matching Engine implemented in C++. This project simulates a financial exchange by maintaining a Limit Order Book and matching incoming buy and sell orders based on **Price-Time Priority**.

## Features

- **Limit Order Book**: Maintains separate ordered data structures for Bids (Buy orders) and Asks (Sell orders).
- **Price-Time Priority Matching**: Ensures that orders are matched fairly based on the best price, and time of entry in case of a tie.
- **Order Management**: Supports adding orders (Limit orders with Buy/Sell sides) and canceling existing active orders.
- **Fast Execution & Lookup**: Uses `std::map`, `std::list`, and `std::unordered_map` strategically for optimal time complexity, including `O(1)` order lookups for fast cancellations.

## Directory Structure

- `include/`: Header files (`MatchingEngine.h`, `LimitOrderBook.h`, `IOrderBook.h`, `Order.h`)
- `src/`: Source code including the engine implementations and `main.cpp` for simulation.
- `CMakeLists.txt`: CMake configuration for building the project.

## Requirements

- C++17 Compiler
- CMake 3.10+

## Build and Run

1. **Clone the repository:**
   ```bash
   git clone https://github.com/SKaran872/OrderMatchingEngine.git
   cd OrderMatchingEngine
   ```

2. **Build the project using CMake:**
   ```bash
   mkdir build
   cd build
   cmake ..
   cmake --build .
   ```

3. **Run the simulation:**
   ```bash
   # On Windows
   ./Debug/MatchingEngine.exe
   
   # Or directly
   ./MatchingEngine.exe
   ```
