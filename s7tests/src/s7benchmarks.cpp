#include <chrono>
#include <chrono_io.hpp>
#include <catch.hpp>

#include "core.h"


void Bench(int numIterations, std::function<void(int)> fn)
{
    typedef std::chrono::high_resolution_clock Clock;
    
    auto start = Clock::now();
    
    double total = 0;
    for (int i = 0; i < numIterations; i++) {
        fn(i);
    }
    
    auto end = Clock::now();
    
    auto duration = std::chrono::duration_cast<std::chrono::duration<double>>(end - start);
    
    WARN("Took " << duration << " to execute " << numIterations << " iterations.");
}

TEST_CASE("Benchmark factorial", "[bench]") {
    //Bench(10000000, [](int iterIdx) { s7::Sqrt((float)iterIdx + 1); });
}
