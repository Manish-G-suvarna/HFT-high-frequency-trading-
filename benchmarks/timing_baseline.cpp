#include <chrono>
#include <cstdint>
#include <iostream>

int main() {
    constexpr std::uint64_t iterations = 10'000'000;

    volatile std::uint64_t value = 0;

    const auto start = std::chrono::steady_clock::now();

    for (std::uint64_t i = 0; i < iterations; ++i) {
        value += i;
    }

    const auto end = std::chrono::steady_clock::now();

    const auto elapsed =
        std::chrono::duration_cast<std::chrono::nanoseconds>(
            end - start
        ).count();

    std::cout << "Iterations : " << iterations << '\n';
    std::cout << "Elapsed    : " << elapsed << " ns\n";
    std::cout << "Per op     : "
              << static_cast<double>(elapsed) / iterations
              << " ns\n";

    std::cout << "Result     : " << value << '\n';

    return 0;
}
