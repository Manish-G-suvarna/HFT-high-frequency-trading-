# Phase 0 — Environment and Benchmarking

## Objective

Establish a reproducible development and benchmarking environment
for the low-latency HFT engine.

## Hardware

- CPU: Intel Core i5-12500H
- Physical cores: 8
- Logical processors: 16
- L2 cache: 10 MB
- L3 cache: 18 MB
- RAM available to WSL: ~7.6 GB

## Software

- OS: Ubuntu 26.04 LTS
- Environment: WSL2
- GCC: 15.2
- Clang: 21.1.8
- CMake: 4.2.3
- Ninja: 1.13.2
- GDB: 17.1

## Important

WSL2 is being used for development.

Latency measurements obtained under WSL2 should not be
treated as authoritative hardware-level HFT measurements.

Native Linux benchmarking will be investigated later.

## Phase 0 goals

1. Establish C++ build system
2. Establish timing methodology
3. Measure timing overhead
4. Investigate CPU cycle timing
5. Measure memory access behavior
6. Establish baseline performance
