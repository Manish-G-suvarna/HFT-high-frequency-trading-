#include <atomic>
#include <chrono>
#include <cstdint>
#include <ctime>
#include <iomanip>
#include <iostream>
#include <x86intrin.h>

static inline std::uint64_t rdtscp()
{
    unsigned int aux;
    return __rdtscp(&aux);
}

static inline std::uint64_t monotonic_ns()
{
    timespec ts{};
    clock_gettime(CLOCK_MONOTONIC, &ts);

    return static_cast<std::uint64_t>(ts.tv_sec) * 1'000'000'000ULL
         + static_cast<std::uint64_t>(ts.tv_nsec);
}

int main()
{
    constexpr std::uint64_t iterations = 10'000'000;

    // ------------------------------------------------------------
    // 1. std::chrono::steady_clock
    // ------------------------------------------------------------

    auto start_chrono = std::chrono::steady_clock::now();

    for (std::uint64_t i = 0; i < iterations; ++i)
    {
        std::atomic_signal_fence(std::memory_order_seq_cst);
        std::chrono::steady_clock::now();
    }

    auto end_chrono = std::chrono::steady_clock::now();

    auto chrono_ns =
        std::chrono::duration_cast<std::chrono::nanoseconds>(
            end_chrono - start_chrono
        ).count();

    // ------------------------------------------------------------
    // 2. clock_gettime
    // ------------------------------------------------------------

    auto start_clock = monotonic_ns();

    for (std::uint64_t i = 0; i < iterations; ++i)
    {
        std::atomic_signal_fence(std::memory_order_seq_cst);
        monotonic_ns();
    }

    auto end_clock = monotonic_ns();

    auto clock_ns = end_clock - start_clock;

    // ------------------------------------------------------------
    // 3. RDTSCP
    // ------------------------------------------------------------

    auto start_tsc = rdtscp();

    for (std::uint64_t i = 0; i < iterations; ++i)
    {
        std::atomic_signal_fence(std::memory_order_seq_cst);
        rdtscp();
    }

    auto end_tsc = rdtscp();

    auto tsc_cycles = end_tsc - start_tsc;

    // ------------------------------------------------------------
    // Results
    // ------------------------------------------------------------

    std::cout << std::fixed << std::setprecision(3);

    std::cout << "\nTiming benchmark\n";
    std::cout << "==============================\n";

    std::cout << "Iterations: " << iterations << "\n\n";

    std::cout << "steady_clock:\n";
    std::cout << "  total : " << chrono_ns << " ns\n";
    std::cout << "  per   : "
              << static_cast<double>(chrono_ns) / iterations
              << " ns\n\n";

    std::cout << "clock_gettime:\n";
    std::cout << "  total : " << clock_ns << " ns\n";
    std::cout << "  per   : "
              << static_cast<double>(clock_ns) / iterations
              << " ns\n\n";

    std::cout << "RDTSCP:\n";
    std::cout << "  total : " << tsc_cycles << " cycles\n";
    std::cout << "  per   : "
              << static_cast<double>(tsc_cycles) / iterations
              << " cycles\n";

    return 0;
}
