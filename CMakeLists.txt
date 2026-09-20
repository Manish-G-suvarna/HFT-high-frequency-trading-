cmake_minimum_required(VERSION 3.25)

project(HFT
    VERSION 0.1.0
    DESCRIPTION "Low-latency HFT engine"
    LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(hft_engine
    src/main.cpp
)

target_compile_options(hft_engine PRIVATE
    -Wall
    -Wextra
    -Wpedantic
)
