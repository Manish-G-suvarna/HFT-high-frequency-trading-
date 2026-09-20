#include "order_book.hpp"

namespace hft
{

OrderBook::OrderBook()
    : bid_count_(0),
      ask_count_(0)
{
    for (std::size_t i = 0; i < MAX_LEVELS; ++i)
    {
        bids_[i] = PriceLevel{0, 0};
        asks_[i] = PriceLevel{0, 0};
    }
}

void OrderBook::process(const MarketEvent& event)
{
    if (event.type != EventType::ADD)
    {
        return;
    }

    PriceLevel* levels;
    std::size_t* count;

    if (event.side == Side::BUY)
    {
        levels = bids_;
        count = &bid_count_;
    }
    else
    {
        levels = asks_;
        count = &ask_count_;
    }

    for (std::size_t i = 0; i < *count; ++i)
    {
        if (levels[i].price == event.price)
        {
            levels[i].quantity += event.quantity;
            return;
        }
    }

    if (*count < MAX_LEVELS)
    {
        levels[*count] = PriceLevel{
            event.price,
            event.quantity
        };

        ++(*count);
    }
}

std::int64_t OrderBook::best_bid() const
{
    if (bid_count_ == 0)
    {
        return 0;
    }

    std::int64_t best = bids_[0].price;

    for (std::size_t i = 1; i < bid_count_; ++i)
    {
        if (bids_[i].price > best)
        {
            best = bids_[i].price;
        }
    }

    return best;
}

std::int64_t OrderBook::best_ask() const
{
    if (ask_count_ == 0)
    {
        return 0;
    }

    std::int64_t best = asks_[0].price;

    for (std::size_t i = 1; i < ask_count_; ++i)
    {
        if (asks_[i].price < best)
        {
            best = asks_[i].price;
        }
    }

    return best;
}

std::uint32_t OrderBook::bid_quantity() const
{
    if (bid_count_ == 0)
    {
        return 0;
    }

    for (std::size_t i = 0; i < bid_count_; ++i)
    {
        if (bids_[i].price == best_bid())
        {
            return bids_[i].quantity;
        }
    }

    return 0;
}

std::uint32_t OrderBook::ask_quantity() const
{
    if (ask_count_ == 0)
    {
        return 0;
    }

    for (std::size_t i = 0; i < ask_count_; ++i)
    {
        if (asks_[i].price == best_ask())
        {
            return asks_[i].quantity;
        }
    }

    return 0;
}

} // namespace hft
