#include <thread>
#include <chrono>
#include "matching_engine.hpp"
#include "fifo_strategy.hpp"
#include "hybrid_strategy.hpp"

int main() {
    // Choose matching algorithm
    MatchingEngine engine(MatchAlgo::HYBRID);

    engine.create_order(true, 5, 20);
    engine.create_order(true, 5, 15);
    engine.create_order(true, 6, 20);
    engine.create_order(true, 7, 10);
    engine.create_order(true, 6.7, 15);

    engine.print_order_books();
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));

    engine.create_order(false, 5, 90);
    engine.print_order_books();
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));

    engine.create_order(true, 7, 20);
    engine.print_order_books();
}
