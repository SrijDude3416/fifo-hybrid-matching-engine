#include "fifo_strategy.hpp"
#include "utils.hpp"
#include <algorithm>

bool FIFOStrategy::match(std::shared_ptr<LimitOrder> incoming,
                         std::shared_ptr<price_point_t> resting_orders,
                         double price)
{
    while (!resting_orders->empty() && incoming->unfilled_quantity > 0) {
        auto resting = resting_orders->front();

        uint32_t fill_qty = std::min(resting->unfilled_quantity,
                                     incoming->unfilled_quantity);

        std::cout << "[FIFO Match] Incoming " << incoming->order_id
                  << " vs Resting " << resting->order_id
                  << " | Qty: " << fill_qty
                  << " @ Price: " << price << std::endl;

        if (verbose_fill(resting, price, fill_qty))
            resting_orders->pop_front();

        if (verbose_fill(incoming, price, fill_qty))
            return true; // incoming fully filled
    }
    return false;
}