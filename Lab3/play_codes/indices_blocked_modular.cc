//Purpose: Modular version of indices_blocked
//Why? because the code looks very verbose and redundant
//We can simplify the code by constructing functions for the pices of code which repeat.
//For example: the common thing is accessing a sub-matrix, applying FW (compare and update)


#include <cstdio>
#include <cstdlib>
#include <math.h>

//global variables -- READ ONLY purpose
int n = 6; //number of vertices
int blocking_factor = 2;
int rounds = ceil(n/(double)blocking_factor);

void access_submatrix(int row_offset, int colm_offset, int *arr){
    for(int b_i = 0; b_i < blocking_factor; b_i++){
        for(int b_j = 0; b_j < blocking_factor; b_j++){  
            int i_j_idx = (row_offset + b_i) * n + (colm_offset * blocking_factor) + b_j; //x_posn + y_posn
            printf("%d \t", arr[i_j_idx]);
        }
        printf("\n");
    }
}


int main(){
    
    int *d; //linearly storing the matrix
    d = (int *) malloc(sizeof(int *) * n * n);
    d[0] = 100;
    for (int i = 1; i < n * n; ++i) d[i] = d[i-1] + 1;

    //printing the matrix
    for(int i = 0; i < n; i++){
        for(int j = 0; j < n; j++){
            int idx = i*n + j;
            printf("%d \t", d[idx]);
        }
        printf("\n");
    }

    
    

    for(int rnd_id = 0; rnd_id < rounds; rnd_id++){
        //PHASE1: printing the pivot submatrices
        printf("\n >> Printing pivot block: \n");
        int cell_rowid = rnd_id * blocking_factor; // row_id
        int cell_colmid = rnd_id * blocking_factor; // row_id
        access_submatrix(cell_rowid, rnd_id, d);

        //PHASE2: printing the pivot-row pivot-column submatrices
        //two parts: goto row, goto column
        //pivot block is (rnd_id, rnd_id) ==> 

        printf("\n >> Printing pivot rows: \n");
        //par1: cover row
        for(int block = 0; block < rounds; block++){
           if(block != rnd_id){
            access_submatrix(cell_rowid, block, d);
           }
        }

        //par2: cover colm
        printf("\n >> Printing pivot columns: \n");
        for(int block = 0; block < rounds; block++){
           if(block != rnd_id){
            access_submatrix(block*blocking_factor, rnd_id, d);
           }
        }


        //PHASE3: print non-pivot elements
        //iterations: inter-block then intra-block
        printf("\n >> Printing non-pivot elements: \n");
        for(int rnd_i = 0; rnd_i < rounds; rnd_i++){
            if(rnd_i != rnd_id){
                for(int rnd_j = 0; rnd_j < rounds; rnd_j++){
                    if(rnd_j != rnd_id){
                        access_submatrix(rnd_i*blocking_factor, rnd_j, d);
                    }
                }
            }
        }
    }

    return 0;
}