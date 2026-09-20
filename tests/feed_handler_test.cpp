#include "feed_handler.hpp"

#include <cassert>
#include <cstdint>
#include <iostream>

int main()
{
    hft::FeedHandler feed_handler;

    hft::MarketEvent event{
        100,
        1,
        10025,
        50,
        hft::Side::BUY,
        hft::EventType::ADD
    };

    bool received = false;

    feed_handler.process(
        event,
        [&](const hft::MarketEvent& received_event)
        {
            received = true;

            assert(received_event.timestamp == 100);
            assert(received_event.symbol_id == 1);
            assert(received_event.price == 10025);
            assert(received_event.quantity == 50);
            assert(received_event.side == hft::Side::BUY);
            assert(received_event.type == hft::EventType::ADD);
        }
    );

    assert(received);

    std::cout << "FeedHandler test: PASS\n";

    return 0;
}
