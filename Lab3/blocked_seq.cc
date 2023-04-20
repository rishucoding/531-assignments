#include <cstdio>
#include <cstdlib>
#include <math.h>
#define INF 200

int main(int argc, char** argv) {
    int n, m, *d;
    // input
    FILE *infile = fopen(argv[1], "r");
    fscanf(infile, "%d %d", &n, &m);
    d = (int *) malloc(sizeof(int *) * n * n);
    for (int i = 0; i < n * n; ++i) d[i] = INF;
    int a, b, w;
    for (int i = 0; i < m; ++i) {
        fscanf(infile, "%d %d %d", &a, &b, &w);
        d[a * n + b] = d[b * n + a] = w;
    }
    fclose(infile);

    //RJ: defining the variables for blocked implementation
    int blocking_factor = 2;
    int rounds = ceil(n/(double)blocking_factor);
    int phases = 3;

    //RJ: notes
    // N/B rounds --> 3 phases --> B iterations
    // seems like phase2, phase 3 can be parallelized.
    for (int rnd_id = 0; rnd_id < rounds; rnd_id++){ // number of rounds
        //for(int iter = 0; iter < blocking_factor; iter++){ // B iterations per round
        //do 3 phases in each round
        //Phase1 -- computing the pivot block
        //(1) find which cell to work on. (2) apply blocked FW

        int cell_rowid = rnd_id * blocking_factor; // row_id
        int cell_colmid = rnd_id * blocking_factor; // row_id
        for(int b_k = 0; b_k < blocking_factor; b_k++){
            //accessing each element in the submatrix
            for(int b_i = 0; b_i < blocking_factor; b_i++){
                for(int b_j = 0; b_j < blocking_factor; b_j++){
                    //index for i,j
                    int i_j_idx = (cell_rowid + b_i) * n + (rnd_id * blocking_factor) + b_j; //x_posn + y_posn
                    int i_k_idx = (cell_rowid + b_i) * n + (rnd_id * blocking_factor) + b_k; //x_posn + y_posn
                    int k_j_idx = (cell_rowid + b_k) * n + (rnd_id * blocking_factor) + b_j; //x_posn + y_posn
                    if((w = d[i_k_idx] + d[k_j_idx]) < d[i_j_idx]){
                        d[i_j_idx] = w;
                    }
                }    
            }
        }

        //Phase2

        //Phase3
        //}
    }


    //RJ: main compute kernel
    // Floyd-Warshall
    for (int k = 0; k < n; ++k) {
        for (int i = 0; i < n; ++i) {
            for (int j = 0; j < n; ++j)
                if ((w = d[i * n + k] + d[k * n + j]) < d[i * n + j])
                    d[i * n + j] = w;
        }
    }


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