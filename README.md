# Low-Latency HFT Engine

A C++23-based low-latency electronic trading engine focused on building and measuring a microsecond-scale trading technology stack.

The primary goal of this project is not to develop a sophisticated trading strategy. Instead, the project focuses on the engineering challenges behind low-latency trading systems:

- In-memory market-data processing
- Cache-friendly data structures
- Low-latency order-book processing
- Deterministic execution paths
- Risk checks
- Order management
- Exchange/matching-engine simulation
- Cycle-level latency measurement
- Performance profiling and optimization
- Market-data replay
- Streaming analytics and observability

The project is being developed incrementally, starting with a local market simulator and eventually supporting external market-data/paper-trading adapters.

> **Important:** This is an educational and engineering project. It is not intended to compete with institutional HFT infrastructure, which relies on specialized networking, colocated servers, FPGA/ASIC systems, kernel bypass, and exchange-specific infrastructure.

---

# Architecture

The system is designed around a **two-speed architecture**.

The latency-sensitive trading path remains entirely in memory and is implemented in C++.

A separate asynchronous path handles telemetry, replay, analytics, monitoring, and historical data.

```text
                         LOW-LATENCY HOT PATH
                     ───────────────────────────

                         Market Data
                              │
                              ▼
                    ┌──────────────────┐
                    │  Feed Handler    │
                    └────────┬─────────┘
                             │
                             ▼
                    ┌──────────────────┐
                    │   Order Book     │
                    │   In-Memory      │
                    └────────┬─────────┘
                             │
                             ▼
                    ┌──────────────────┐
                    │ Strategy Engine  │
                    └────────┬─────────┘
                             │
                             ▼
                    ┌──────────────────┐
                    │   Risk Engine    │
                    └────────┬─────────┘
                             │
                             ▼
                    ┌──────────────────┐
                    │ Order Management │
                    └────────┬─────────┘
                             │
                             ▼
                    ┌──────────────────┐
                    │ Execution /      │
                    │ Exchange Adapter │
                    └────────┬─────────┘
                             │
                             ▼
                    ┌──────────────────┐
                    │ Matching Engine  │
                    │ / Simulator      │
                    └──────────────────┘


                         ASYNC SIDE PATH
                     ─────────────────────

                         Trading Events
                              │
                              ▼
                           Kafka
                              │
                              ▼
                           Flink
                              │
                    ┌─────────┴─────────┐
                    ▼                   ▼
                Analytics          Event Replay

                              │
                              ▼
                    Prometheus / Grafana
