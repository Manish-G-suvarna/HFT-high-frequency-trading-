#include <cstdint>
#include <iomanip>
#include <iostream>
#include <x86intrin.h>

static inline std::uint64_t read_tsc()
{
    unsigned int aux;
    return __rdtscp(&aux);
}

int main()
{
    constexpr std::uint64_t iterations = 10'000'000;

    volatile std::uint64_t value = 0;

    std::uint64_t total_cycles = 0;

    for (std::uint64_t i = 0; i < iterations; ++i)
    {
        const auto start = read_tsc();

        value += i;

        const auto end = read_tsc();

        total_cycles += (end - start);
    }

    const double average =
        static_cast<double>(total_cycles) / iterations;

    std::cout << std::fixed << std::setprecision(3);

    std::cout << "RDTSCP micro-benchmark\n";
    std::cout << "=====================\n";
    std::cout << "Iterations : " << iterations << '\n';
    std::cout << "Total      : " << total_cycles << " cycles\n";
    std::cout << "Average    : " << average << " cycles\n";
    std::cout << "Result     : " << value << '\n';

    return 0;
}
