#include <chrono>
#include <chrono_io>
#include <catch.hpp>

#include "core.h"

using namespace std;

TEST_CASE("Benchmark factorial", "[bench]") {
    auto numIterations = 10000;
    typedef std::chrono::high_resolution_clock Clock;
    
    auto start = Clock::now();
    
    double total = 0;
    for (int i = 0; i < numIterations; i++) {
        total += s7::Sqrt((float)i + 1);
    }
    
    auto end = Clock::now();
    
    WARN("Took " << (end-start) << "s to execute " << numIterations << " iterations.");
}
