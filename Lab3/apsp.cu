#include <chrono>
#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <cuda_runtime.h>

#define check(err) __check(err, __LINE__)
void __check(cudaError err, int line) {
    if (err) {
        fprintf(stderr, "%d: %s\n", line, cudaGetErrorString(err));
    }
}

int main(int argc, char** argv) {
    // TODO: Read input from argv[1]
    // TODO: Blocked Floyd-Warshall
    int B = atoi(argv[3]);
    // int round = ceil(n / B);
    for (int r = 0; r < round; r++) {
        // phase 1
        // phase 2
        // phase 3
    }
    // TODO: Write output to argv[2]
}
