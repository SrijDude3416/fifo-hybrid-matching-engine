#ifndef MATCHING_STRATEGY_HPP
#define MATCHING_STRATEGY_HPP

#include <memory>
#include <list>
#include <map>

struct LimitOrder {
    bool is_buy;
    bool is_filled = false;
    double limit_price;
    double total_fill_price = 0;
    double avg_fill_price = 0;
    uint32_t order_id;
    uint32_t unfilled_quantity;
    uint32_t filled_quantity = 0;
};

using price_point_t = std::list<std::shared_ptr<LimitOrder>>;
using order_book_t = std::map<double, std::shared_ptr<price_point_t>>;

class MatchingStrategy {
public:
    virtual bool match(std::shared_ptr<LimitOrder> incoming,
                       std::shared_ptr<price_point_t> resting_orders,
                       double price) = 0;

    virtual ~MatchingStrategy() = default;
};

#endif
