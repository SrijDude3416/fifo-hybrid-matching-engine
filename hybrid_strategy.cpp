#include "hybrid_strategy.hpp"
#include "utils.hpp"
#include <algorithm>
#include <vector>
#include <iostream>

bool HybridFifoProRataStrategy::match(std::shared_ptr<LimitOrder> incoming,
                                      std::shared_ptr<price_point_t> resting_orders,
                                      double price)
{
    if (resting_orders->empty() || incoming->unfilled_quantity == 0) return false;

    // Identify FIFO tie group at front
    auto first_id = resting_orders->front()->order_id;
    std::vector<std::shared_ptr<LimitOrder>> fifo_group;

    for (auto& o : *resting_orders) {
        if (o->order_id == first_id)
            fifo_group.push_back(o);
        else
            break;
    }

    // If only one order, fallback to FIFO
    if (fifo_group.size() == 1)
        return pure_fifo(incoming, resting_orders, price);

    // Pro-rata allocation among tied orders
    uint32_t total_qty = 0;
    for (auto& o : fifo_group) total_qty += o->unfilled_quantity;

    uint32_t inc_qty = incoming->unfilled_quantity;

    for (auto& o : fifo_group) {
        if (inc_qty == 0) break;

        double share = double(o->unfilled_quantity) / total_qty;
        uint32_t alloc = std::max(uint32_t(share * inc_qty), 1u);
        alloc = std::min({ alloc, o->unfilled_quantity, incoming->unfilled_quantity });

        std::cout << "[Hybrid Match] Incoming " << incoming->order_id
                  << " vs Resting " << o->order_id
                  << " | Allocated Qty: " << alloc
                  << " @ Price: " << price << std::endl;

        verbose_fill(o, price, alloc);
        verbose_fill(incoming, price, alloc);

        if (o->unfilled_quantity == 0)
            resting_orders->pop_front();
    }

    // Continue FIFO for remaining
    return pure_fifo(incoming, resting_orders, price);
}

// Pure FIFO for leftover
bool HybridFifoProRataStrategy::pure_fifo(std::shared_ptr<LimitOrder> incoming,
                                          std::shared_ptr<price_point_t> resting_orders,
                                          double price)
{
    while (!resting_orders->empty() && incoming->unfilled_quantity > 0) {
        auto o = resting_orders->front();
        uint32_t fill_qty = std::min(o->unfilled_quantity, incoming->unfilled_quantity);

        std::cout << "[Hybrid FIFO] Incoming " << incoming->order_id
                  << " vs Resting " << o->order_id
                  << " | Qty: " << fill_qty
                  << " @ Price: " << price << std::endl;

        if (verbose_fill(o, price, fill_qty))
            resting_orders->pop_front();

        if (verbose_fill(incoming, price, fill_qty))
            return true;
    }
    return false;
}
