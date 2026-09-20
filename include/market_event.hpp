#pragma once

#include <cstdint>

namespace hft
{

enum class Side : std::uint8_t
{
    BUY = 0,
    SELL = 1
};

enum class EventType : std::uint8_t
{
    ADD = 0,
    CANCEL = 1,
    TRADE = 2
};

struct MarketEvent
{
    std::uint64_t timestamp;
    std::uint32_t symbol_id;

    std::int64_t price;
    std::uint32_t quantity;

    Side side;
    EventType type;
};

} // namespace hft
