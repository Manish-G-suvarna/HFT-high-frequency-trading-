#pragma once

#include "market_event.hpp"

namespace hft
{

class FeedHandler
{
public:

    FeedHandler() = default;

    template <typename Handler>
    inline void process(const MarketEvent& event, Handler&& handler) const
    {
        handler(event);
    }
};

} // namespace hft
