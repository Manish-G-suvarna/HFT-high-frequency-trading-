#include "feed_handler.hpp"

#include <cstdint>
#include <iostream>

int main()
{
    constexpr std::uint32_t symbol = 1;

    hft::MarketEvent events[] =
    {
        {
            1,
            symbol,
            10000,
            100,
            hft::Side::BUY,
            hft::EventType::ADD
        },

        {
            2,
            symbol,
            10001,
            50,
            hft::Side::SELL,
            hft::EventType::ADD
        },

        {
            3,
            symbol,
            10000,
            25,
            hft::Side::BUY,
            hft::EventType::TRADE
        }
    };

    hft::FeedHandler feed_handler;

    for (const auto& event : events)
    {
        feed_handler.process(
            event,
            [](const hft::MarketEvent& normalized_event)
            {
                std::cout
                    << "timestamp=" << normalized_event.timestamp
                    << " symbol=" << normalized_event.symbol_id
                    << " price=" << normalized_event.price
                    << " quantity=" << normalized_event.quantity
                    << " side="
                    << (
                        normalized_event.side == hft::Side::BUY
                            ? "BUY"
                            : "SELL"
                    )
                    << " type=";

                switch (normalized_event.type)
                {
                    case hft::EventType::ADD:
                        std::cout << "ADD";
                        break;

                    case hft::EventType::CANCEL:
                        std::cout << "CANCEL";
                        break;

                    case hft::EventType::TRADE:
                        std::cout << "TRADE";
                        break;
                }

                std::cout << '\n';
            }
        );
    }

    return 0;
}
