#include <algorithm>
#include <cstdint>
#include <iomanip>
#include <iostream>
#include <random>
#include <vector>
#include <x86intrin.h>

static inline std::uint64_t rdtscp()
{
    unsigned int aux;
    return __rdtscp(&aux);
}

std::uint64_t measure_latency(std::vector<std::uint32_t>& next,
                              std::uint64_t iterations)
{
    std::uint32_t index = 0;

    const auto start = rdtscp();

    for (std::uint64_t i = 0; i < iterations; ++i)
    {
        index = next[index];
    }

    const auto end = rdtscp();

    // Prevent compiler from removing the traversal.
    asm volatile("" : : "r,m"(index) : "memory");

    return end - start;
}

std::vector<std::uint32_t> create_random_chain(std::size_t elements)
{
    std::vector<std::uint32_t> indices(elements);

    for (std::uint32_t i = 0; i < elements; ++i)
    {
        indices[i] = i;
    }

    std::mt19937 rng(12345);

    std::shuffle(indices.begin(), indices.end(), rng);

    std::vector<std::uint32_t> next(elements);

    for (std::size_t i = 0; i < elements - 1; ++i)
    {
        next[indices[i]] = indices[i + 1];
    }

    next[indices.back()] = indices.front();

    return next;
}

int main()
{
    constexpr std::uint64_t iterations = 10'000'000;

    constexpr std::size_t l1_bytes = 16 * 1024;
    constexpr std::size_t l2_bytes = 256 * 1024;
    constexpr std::size_t l3_bytes = 4 * 1024 * 1024;
    constexpr std::size_t ram_bytes = 64 * 1024 * 1024;

    constexpr std::size_t element_size = sizeof(std::uint32_t);

    const std::size_t l1_elements = l1_bytes / element_size;
    const std::size_t l2_elements = l2_bytes / element_size;
    const std::size_t l3_elements = l3_bytes / element_size;
    const std::size_t ram_elements = ram_bytes / element_size;

    std::cout << std::fixed << std::setprecision(2);

    std::cout << "Pointer-chasing cache latency benchmark\n";
    std::cout << "========================================\n";
    std::cout << "Iterations: " << iterations << "\n\n";

    auto l1 = create_random_chain(l1_elements);
    auto l2 = create_random_chain(l2_elements);
    auto l3 = create_random_chain(l3_elements);
    auto ram = create_random_chain(ram_elements);

    const auto l1_cycles =
        measure_latency(l1, iterations);

    const auto l2_cycles =
        measure_latency(l2, iterations);

    const auto l3_cycles =
        measure_latency(l3, iterations);

    const auto ram_cycles =
        measure_latency(ram, iterations);

    std::cout << "16 KB working set:\n";
    std::cout << "  "
              << static_cast<double>(l1_cycles) / iterations
              << " cycles/access\n\n";

    std::cout << "256 KB working set:\n";
    std::cout << "  "
              << static_cast<double>(l2_cycles) / iterations
              << " cycles/access\n\n";

    std::cout << "4 MB working set:\n";
    std::cout << "  "
              << static_cast<double>(l3_cycles) / iterations
              << " cycles/access\n\n";

    std::cout << "64 MB working set:\n";
    std::cout << "  "
              << static_cast<double>(ram_cycles) / iterations
              << " cycles/access\n";

    return 0;
}
