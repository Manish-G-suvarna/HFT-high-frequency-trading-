#pragma once

#include "market_event.hpp"

#include <cstdint>

namespace hft
{

struct PriceLevel
{
    std::int64_t price;
    std::uint32_t quantity;
};

class OrderBook
{
public:

    static constexpr std::size_t MAX_LEVELS = 1024;

    OrderBook();

    void process(const MarketEvent& event);

    std::int64_t best_bid() const;
    std::int64_t best_ask() const;

    std::uint32_t bid_quantity() const;
    std::uint32_t ask_quantity() const;

private:

    PriceLevel bids_[MAX_LEVELS];
    PriceLevel asks_[MAX_LEVELS];

    std::size_t bid_count_;
    std::size_t ask_count_;
};

} // namespace hft
