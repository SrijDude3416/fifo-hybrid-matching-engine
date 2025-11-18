#include "matching_engine.hpp"
#include "fifo_strategy.hpp"
#include "hybrid_strategy.hpp"
#include <iostream>

// Constructor: select strategy based on enum
MatchingEngine::MatchingEngine(MatchAlgo algo) {
    if (algo == MatchAlgo::FIFO) {
        strategy = std::make_unique<FIFOStrategy>();
    } else { // HYBRID
        strategy = std::make_unique<HybridFifoProRataStrategy>();
    }
}

// Create order
bool MatchingEngine::create_order(bool is_buy, double price, uint32_t qty) {
    if (!price || !qty) throw std::runtime_error("Invalid order parameters");

    next_id++;

    auto order = std::make_shared<LimitOrder>();
    order->order_id = next_id;
    order->is_buy = is_buy;
    order->limit_price = price;
    order->unfilled_quantity = qty;
    order->filled_quantity = 0;

    if (match_price_points(order)) return true;

    order_book_t& book = is_buy ? buy_orders : sell_orders;
    if (!book.count(price)) book[price] = std::make_shared<price_point_t>();
    book[price]->push_back(order);
    return false;
}

// Print order books
void MatchingEngine::print_order_books() {
    std::cout << "=============\nSELL Orders\n";
    for (auto it = sell_orders.begin(); it != sell_orders.end(); ++it) {
        for (auto& o : *(it->second)) {
            std::cout << o->limit_price << ", " << o->unfilled_quantity << "\n";
        }
    }
    std::cout << "BUY Orders\n";
    for (auto it = buy_orders.rbegin(); it != buy_orders.rend(); ++it) {
        for (auto& o : *(it->second)) {
            std::cout << o->limit_price << ", " << o->unfilled_quantity << "\n";
        }
    }
    std::cout << "=============\n";
}

// Match price points
bool MatchingEngine::match_price_points(std::shared_ptr<LimitOrder> order) {
    order_book_t& opposing_book = order->is_buy ? sell_orders : buy_orders;

    auto it = order->is_buy ? opposing_book.begin() : opposing_book.end();
    auto end_it = order->is_buy ? opposing_book.end() : opposing_book.begin();

    while ((order->is_buy ? it != end_it : it != end_it)) {
        if (!order->is_buy) --it;

        if ((order->is_buy && it->first > order->limit_price) ||
            (!order->is_buy && it->first < order->limit_price))
            break;

        auto pp = it->second;
        if (strategy->match(order, pp, it->first)) return true;

        if (order->is_buy) ++it;
    }
    return false;
}
