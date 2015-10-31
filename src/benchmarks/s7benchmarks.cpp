#include <cstdio>
#include <omp.h>

#include "core.h"

using namespace std;

void BenchSqrt()
{
	auto numIterations = 10000;
	auto start = omp_get_wtime();

	double total = 0;
	for (int i = 0; i < numIterations; i++) {
		total += s7::Sqrt((float)i + 1);
	}

	auto end = omp_get_wtime();

	printf("BenchSqrt: %fs for %d iterations.\n", end-start, numIterations);
}

int main()
{
	printf("Benchmarks\n");
	getchar();
	return 0;
}
