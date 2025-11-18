#pragma once
#include <memory>
#include <iostream>
#include "matching_strategy.hpp"

// Verbose fill for logging partial and completed fills
inline bool verbose_fill(std::shared_ptr<LimitOrder> order,
                         double fill_price_point,
                         uint32_t fill_quantity) 
{
    if (fill_quantity == 0) return false;

    double total_fill = fill_price_point * fill_quantity;

    // Update order
    order->total_fill_price += total_fill;
    order->filled_quantity += fill_quantity;
    order->unfilled_quantity -= fill_quantity;

    // Log partial fill
    std::cout << "Partially Filled " << order->order_id
              << " " << (order->is_buy ? "BUY " : "SELL ")
              << fill_quantity << " @ " << fill_price_point
              << " | Total Filled: " << order->filled_quantity
              << ", Remaining: " << order->unfilled_quantity
              << ", Avg Price so far: "
              << (order->filled_quantity ? order->total_fill_price / order->filled_quantity : 0)
              << std::endl;

    // Check completion
    if (order->unfilled_quantity == 0) {
        order->avg_fill_price = order->total_fill_price / order->filled_quantity;
        order->is_filled = true;

        // Log completion
        std::cout << "Completed " << order->order_id
                  << " " << (order->is_buy ? "BUY " : "SELL ")
                  << order->filled_quantity
                  << " @ " << fill_price_point
                  << " | Final Avg Price: " << order->avg_fill_price
                  << std::endl;
        return true;
    }

    return false;
}
