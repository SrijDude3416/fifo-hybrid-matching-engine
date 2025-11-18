#ifndef FIFO_STRATEGY_HPP
#define FIFO_STRATEGY_HPP

#include "matching_strategy.hpp"

class FIFOStrategy : public MatchingStrategy {
public:
    bool match(std::shared_ptr<LimitOrder> incoming,
               std::shared_ptr<price_point_t> resting_orders,
               double price) override;

    };

#endif
