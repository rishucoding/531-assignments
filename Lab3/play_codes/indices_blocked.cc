//Purpose: playing with the blocked organization
//to be sure which indices are accessed

#include <cstdio>
#include <cstdlib>
#include <math.h>

int main(){
    int n = 6;
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

    
    int blocking_factor = 2;
    int rounds = ceil(n/(double)blocking_factor);

    for(int rnd_id = 0; rnd_id < rounds; rnd_id++){
        //PHASE1: printing the pivot submatrices
        printf("\n >> Printing pivot block: \n");
        int cell_rowid = rnd_id * blocking_factor; // row_id
        int cell_colmid = rnd_id * blocking_factor; // row_id
        for(int b_i = 0; b_i < blocking_factor; b_i++){
            for(int b_j = 0; b_j < blocking_factor; b_j++){  
                int i_j_idx = (cell_rowid + b_i) * n + (rnd_id * blocking_factor) + b_j; //x_posn + y_posn
                printf("%d \t", d[i_j_idx]);
            }
            printf("\n");
        }

        //PHASE2: printing the pivot-row pivot-column submatrices
        //two parts: goto row, goto column
        //pivot block is (rnd_id, rnd_id) ==> 

        printf("\n >> Printing pivot rows: \n");
        //par1: cover row
        for(int block = 0; block < rounds; block++){
           if(block != rnd_id){
            for(int b_i = 0; b_i < blocking_factor; b_i++){
                for(int b_j = 0; b_j < blocking_factor; b_j++){  
                    int i_j_idx = (cell_rowid + b_i) * n + (block * blocking_factor) + b_j; //x_posn + y_posn
                    printf("%d \t", d[i_j_idx]);
                }
                printf("\n");
            }
           }
        }

        //par2: cover colm
        printf("\n >> Printing pivot columns: \n");
        for(int block = 0; block < rounds; block++){
           if(block != rnd_id){
            for(int b_i = 0; b_i < blocking_factor; b_i++){
                for(int b_j = 0; b_j < blocking_factor; b_j++){  
                    int i_j_idx = (block*blocking_factor + b_i) * n + (rnd_id * blocking_factor) + b_j; //x_posn + y_posn
                    printf("%d \t", d[i_j_idx]);
                }
                printf("\n");
            }
           }
        }


        //PHASE3: print non-pivot elements
        //iterations: inter-block then intra-block
        printf("\n >> Printing non-pivot elements: \n");
        for(int rnd_i = 0; rnd_i < rounds; rnd_i++){
            if(rnd_i != rnd_id){
                for(int rnd_j = 0; rnd_j < rounds; rnd_j++){
                    if(rnd_j != rnd_id){
                        for(int b_i = 0; b_i < blocking_factor; b_i++){
                            for(int b_j = 0; b_j < blocking_factor; b_j++){  
                                int i_j_idx = (rnd_i*blocking_factor + b_i) * n + (rnd_j * blocking_factor) + b_j; //x_posn + y_posn
                                printf("%d \t", d[i_j_idx]);
                            }
                            printf("\n");
                        }
                    }
                }
            }
        }



    }

    return 0;
}