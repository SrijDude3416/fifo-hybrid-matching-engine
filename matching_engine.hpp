#ifndef MATCHING_ENGINE_HPP
#define MATCHING_ENGINE_HPP

#include "matching_strategy.hpp"
#include <memory>
#include <map>

enum class MatchAlgo {
    FIFO,
    HYBRID
};

class MatchingEngine {
public:
    MatchingEngine(MatchAlgo algo);

    bool create_order(bool is_buy, double price, uint32_t qty);
    void print_order_books();

private:
    bool match_price_points(std::shared_ptr<LimitOrder> order);

    uint32_t next_id = 0;
    order_book_t buy_orders;
    order_book_t sell_orders;
    std::unique_ptr<MatchingStrategy> strategy;
};

#endif
