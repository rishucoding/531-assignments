#include <cstdio>
#include <cstdlib>
#include <math.h>
#include <cuda.h>
#include <chrono>
#include <fstream>
#include <cuda_runtime.h>
#include <iostream>
using namespace std::chrono;

#define INF 200

//RJ: defining the variables for blocked implementation
int n;
int blocking_factor = 25;
int rounds = 0;

__global__ void gpu_submatrix_fw_krnl_p1(int row_offset, int colm_offset, int *arr, int rnd_id, int blocking_factor, int n){
    int w = 0;
    for(int b_k = 0; b_k < blocking_factor; b_k++){
            //accessing each element in the submatrix
            for(int b_i = 0; b_i < blocking_factor; b_i++){
                for(int b_j = 0; b_j < blocking_factor; b_j++){
                    //index for i,j
                    int i_j_idx = (row_offset + b_i) * n + (colm_offset * blocking_factor) + b_j; //x_posn + y_posn
                    int i_k_idx = (row_offset + b_i) * n + (colm_offset * blocking_factor) + b_k; //x_posn + y_posn
                    int k_j_idx = (row_offset + b_k) * n + (colm_offset * blocking_factor) + b_j; //x_posn + y_posn
                    if((w = arr[i_k_idx] + arr[k_j_idx]) < arr[i_j_idx]){
                        arr[i_j_idx] = w;
                    }
                }
            }
        }
}


__global__ void gpu_submatrix_fw_krnl_p2(int row_offset, int colm_offset, int *arr, int rnd_id, int blocking_factor, int n){
    int w = 0;
    colm_offset = threadIdx.x;
    for(int b_k = 0; b_k < blocking_factor; b_k++){
            //accessing each element in the submatrix
            for(int b_i = 0; b_i < blocking_factor; b_i++){
                for(int b_j = 0; b_j < blocking_factor; b_j++){
                    //index for i,j
                    int i_j_idx = (row_offset + b_i) * n + (colm_offset * blocking_factor) + b_j; //x_posn + y_posn
                    int i_k_idx = (rnd_id * blocking_factor + b_i) * n + (rnd_id * blocking_factor) + b_k; //x_posn + y_posn
                    int k_j_idx = (rnd_id * blocking_factor + b_k) * n + (colm_offset * blocking_factor) + b_j; //x_posn + y_posn
                    if((w = arr[i_k_idx] + arr[k_j_idx]) < arr[i_j_idx]){
                        arr[i_j_idx] = w;
                    }
                }
            }
        }
}

__global__ void gpu_submatrix_fw_krnl_p3(int row_offset, int colm_offset, int *arr, int rnd_id, int blocking_factor, int n){
    int w = 0;
    row_offset = threadIdx.x * blocking_factor;
    for(int b_k = 0; b_k < blocking_factor; b_k++){
            //accessing each element in the submatrix
            for(int b_i = 0; b_i < blocking_factor; b_i++){
                for(int b_j = 0; b_j < blocking_factor; b_j++){
                    //index for i,j
                    int i_j_idx = (row_offset + b_i) * n + (colm_offset * blocking_factor) + b_j; //x_posn + y_posn
                    int i_k_idx = (row_offset + b_i) * n + (rnd_id * blocking_factor) + b_k; //x_posn + y_posn
                    int k_j_idx = (rnd_id * blocking_factor + b_k) * n + (colm_offset * blocking_factor) + b_j; //x_posn + y_posn
                    if((w = arr[i_k_idx] + arr[k_j_idx]) < arr[i_j_idx]){
                        arr[i_j_idx] = w;
                    }
                }  
            }
        }
}

__global__ void gpu_submatrix_fw_krnl_p4(int row_offset, int colm_offset, int *arr, int rnd_id, int blocking_factor, int n){
    int w = 0;
    row_offset = threadIdx.x * blocking_factor;
    colm_offset = blockIdx.x;
    //printf("(x,y) is: (%d, %d)\n", threadIdx.x, blockIdx.x);
    for(int b_k = 0; b_k < blocking_factor; b_k++){
            //accessing each element in the submatrix
            for(int b_i = 0; b_i < blocking_factor; b_i++){
                for(int b_j = 0; b_j < blocking_factor; b_j++){
                    //index for i,j
                    int i_j_idx = (row_offset + b_i) * n + (colm_offset * blocking_factor) + b_j; //x_posn + y_posn
                    int i_k_idx = (row_offset + b_i) * n + (rnd_id * blocking_factor) + b_k; //x_posn + y_posn
                    int k_j_idx = (rnd_id * blocking_factor + b_k) * n + (colm_offset * blocking_factor) + b_j; //x_posn + y_posn
                    if((w = arr[i_k_idx] + arr[k_j_idx]) < arr[i_j_idx]){
                        arr[i_j_idx] = w;
                    }
                }
            }
        }
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

    
    
    auto start = high_resolution_clock::now();
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
        //do 3 phases in each round
        //Phase1 -- computing on the pivot block
        //(1) find which cell to work on. (2) apply blocked FW

        int cell_rowid = rnd_id * blocking_factor; // row_id
        int cell_colmid = rnd_id * blocking_factor; // row_id
        //GPU declaration and kernel launch:
        dim3 dimGrid = (1);
        dim3 dimBlock = 1;
        //start = high_resolution_clock::now();
        gpu_submatrix_fw_krnl_p1<<<dimGrid, dimBlock>>>(cell_rowid, rnd_id, gpu_d, rnd_id, blocking_factor, n);
        cudaDeviceSynchronize();


        //Phase2 -- apply fw on the pivot-row and pivot-colm
        // (1) cover row
        // (2) cover colm
        //pivot block is (rnd_id, rnd_id)
        //par1: cover row
        
        dimGrid = (1);
        dimBlock = rounds;

        //start = high_resolution_clock::now();
        gpu_submatrix_fw_krnl_p2<<<dimGrid, dimBlock>>>(cell_rowid, 0, gpu_d, rnd_id, blocking_factor, n);

        //par2: cover colm
        gpu_submatrix_fw_krnl_p3<<<dimGrid, dimBlock>>>(0*blocking_factor, rnd_id, gpu_d, rnd_id, blocking_factor, n);
        cudaDeviceSynchronize();
        

        //Phase3 -- apply fw on the non-pivot blocks
        dimGrid = (rounds);
        dimBlock = (rounds);
        gpu_submatrix_fw_krnl_p4<<<dimGrid, dimBlock>>>(0*blocking_factor, 0, gpu_d, rnd_id, blocking_factor, n);
        cudaDeviceSynchronize();        
    }
    
    //Obtain back the data from GPU
    cudaMemcpy(d, gpu_d, sizeof(int)* n * n, cudaMemcpyDeviceToHost);
    cudaDeviceSynchronize();
    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(stop - start);
    std::cout << ">>>> Time to compute on GPU: " << duration.count()/(1000000.0) << std::endl;
    
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