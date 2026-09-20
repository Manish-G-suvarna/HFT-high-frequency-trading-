#include "order_book.hpp"

#include <cassert>
#include <iostream>

int main()
{
    hft::OrderBook book;

    book.process({
        1,
        1,
        10000,
        100,
        hft::Side::BUY,
        hft::EventType::ADD
    });

    book.process({
        2,
        1,
        9999,
        50,
        hft::Side::BUY,
        hft::EventType::ADD
    });

    book.process({
        3,
        1,
        10001,
        75,
        hft::Side::SELL,
        hft::EventType::ADD
    });

    book.process({
        4,
        1,
        10002,
        125,
        hft::Side::SELL,
        hft::EventType::ADD
    });

    assert(book.best_bid() == 10000);
    assert(book.best_ask() == 10001);

    assert(book.bid_quantity() == 100);
    assert(book.ask_quantity() == 75);

    // Add more quantity at existing best bid.
    book.process({
        5,
        1,
        10000,
        25,
        hft::Side::BUY,
        hft::EventType::ADD
    });

    assert(book.best_bid() == 10000);
    assert(book.bid_quantity() == 125);

    std::cout << "OrderBook test: PASS\n";

    return 0;
}
