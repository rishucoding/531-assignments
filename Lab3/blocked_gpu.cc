#include <cstdio>
#include <cstdlib>
#include <math.h>
#include <cuda.h>
#include <chrono>
#include <fstream>
#include <cuda_runtime.h>

#define INF 200

//RJ: defining the variables for blocked implementation
int n;
int blocking_factor = 4;
int rounds = 0;

__global__ void gpu_submatrix_fw_krnl_p1((int row_offset, int colm_offset, int *arr)){

}

int main(int argc, char** argv) {
    int m, *d;
    // input
    FILE *infile = fopen(argv[1], "r");
    fscanf(infile, "%d %d", &n, &m);
    rounds = ceil(n/(double)blocking_factor);
    d = (int *) malloc(sizeof(int *) * n * n);
    for (int i = 0; i < n * n; ++i) d[i] = INF;
    int a, b, w;
    
    for (int i = 0; i < m; ++i) {
        fscanf(infile, "%d %d %d", &a, &b, &w);
        d[a * n + b] = d[b * n + a] = w;
    }
    fclose(infile);

    
    int phases = 3;
    int cnt = 0;

    //RJ: GPU init
    int *gpu_d;
    cudaMalloc((void **)&gpu_d, sizeof(int *) * n * n);
    cudaMemcpy(gpu_d, d, sizeof(int)* n * n, cudaMemcpyHostToDevice);
    dim3 block_dim(blocking_factor,blocking_factor);
    //RJ: notes
    // N/B rounds --> 3 phases --> B iterations
    // seems like phase2, phase 3 can be parallelized.
    printf("Rounds are: %d\n", rounds);
    for (int rnd_id = 0; rnd_id < rounds; rnd_id++){ // number of rounds
        //for(int iter = 0; iter < blocking_factor; iter++){ // B iterations per round
        //do 3 phases in each round
        //Phase1 -- computing on the pivot block
        //(1) find which cell to work on. (2) apply blocked FW

        int cell_rowid = rnd_id * blocking_factor; // row_id
        int cell_colmid = rnd_id * blocking_factor; // row_id
        int *gpu_rnd_id, *gpu_cell_rowid;
        // cudaMalloc((void **)&gpu_rnd_id, sizeof(int));
        // cudaMalloc((void **)&gpu_cell_rowid, sizeof(int));
        // cudaMemcpy(gpu_rnd_id, rnd_id, sizeof(int), cudaMemcpyHostToDevice);
        // cudaMemcpy(gpu_cell_rowid, cell_rowid, sizeof(int), cudaMemcpyHostToDevice);
        //kernel launch parameters: 
        dimGrid = (1,1);
        dimBlock = block_dim;
        gpu_submatrix_fw_krnl_p1<<<dimGrid, dimBlock, blocking_factor * blocking_factor * sizeof(int)>>>(cell_rowid, rnd_id, gpu_d);
    }
    
    //Obtain back the data from GPU
    cudaMemcpy(d, gpu_d, sizeof(int)* n * n, cudaMemcpyHostToDevice);
    
    
    // ouput
    FILE *outfile = fopen(argv[2], "w");
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            fprintf(outfile, "%d%s",
                (i == j ? 0 : d[i * n + j]),
                (j == n - 1 ? " \n" : " ")
            );
        }
    }
    free(d);
}