#ifndef HYBRID_STRATEGY_HPP
#define HYBRID_STRATEGY_HPP

#include "matching_strategy.hpp"
#include <vector>

class HybridFifoProRataStrategy : public MatchingStrategy {
public:
    bool match(std::shared_ptr<LimitOrder> incoming,
               std::shared_ptr<price_point_t> resting_orders,
               double price) override;

private:
    bool fill(std::shared_ptr<LimitOrder> o, double p, uint32_t q);
    bool pure_fifo(std::shared_ptr<LimitOrder> incoming,
                   std::shared_ptr<price_point_t> resting_orders,
                   double price);
};

#endif
