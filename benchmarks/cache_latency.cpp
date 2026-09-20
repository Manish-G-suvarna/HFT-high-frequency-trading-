#include <cstdint>
#include <iomanip>
#include <iostream>
#include <vector>
#include <x86intrin.h>

static inline std::uint64_t rdtscp()
{
    unsigned int aux;
    return __rdtscp(&aux);
}

static inline void prevent_optimization(std::uint64_t value)
{
    asm volatile("" : : "r,m"(value) : "memory");
}

double measure_access(std::vector<std::uint64_t>& data,
                      std::size_t index,
                      std::uint64_t iterations)
{
    volatile std::uint64_t value = 0;

    const auto start = rdtscp();

    for (std::uint64_t i = 0; i < iterations; ++i)
    {
        value += data[index];
    }

    const auto end = rdtscp();

    prevent_optimization(value);

    return static_cast<double>(end - start) / iterations;
}

int main()
{
    constexpr std::uint64_t iterations = 10'000'000;

    // Different working-set sizes.
    constexpr std::size_t l1_size = 32 * 1024 / sizeof(std::uint64_t);
    constexpr std::size_t l2_size = 512 * 1024 / sizeof(std::uint64_t);
    constexpr std::size_t l3_size = 8 * 1024 * 1024 / sizeof(std::uint64_t);
    constexpr std::size_t ram_size = 64 * 1024 * 1024 / sizeof(std::uint64_t);

    std::vector<std::uint64_t> l1(l1_size);
    std::vector<std::uint64_t> l2(l2_size);
    std::vector<std::uint64_t> l3(l3_size);
    std::vector<std::uint64_t> ram(ram_size);

    l1[0] = 1;
    l2[0] = 2;
    l3[0] = 3;
    ram[0] = 4;

    std::cout << std::fixed << std::setprecision(2);

    std::cout << "Cache / memory access benchmark\n";
    std::cout << "================================\n";
    std::cout << "Iterations: " << iterations << "\n\n";

    const double l1_cycles =
        measure_access(l1, 0, iterations);

    const double l2_cycles =
        measure_access(l2, 0, iterations);

    const double l3_cycles =
        measure_access(l3, 0, iterations);

    const double ram_cycles =
        measure_access(ram, 0, iterations);

    std::cout << "32 KB working set:\n";
    std::cout << "  " << l1_cycles << " cycles/access\n\n";

    std::cout << "512 KB working set:\n";
    std::cout << "  " << l2_cycles << " cycles/access\n\n";

    std::cout << "8 MB working set:\n";
    std::cout << "  " << l3_cycles << " cycles/access\n\n";

    std::cout << "64 MB working set:\n";
    std::cout << "  " << ram_cycles << " cycles/access\n";

    return 0;
}
