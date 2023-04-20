## Example steps to run the sequential code: 
* $ gcc seq.cc
* $ ./a.out dataset/100-4000.in myout
* $ diff myout dataset/100-4000.out


## Input Dataset: 
* first entry tells the total number of vertices, and total edges. 
* Eg: the first few lines in "100-4000.in" looks like: 
 100 4000 
 0 1 89 
 0 2 51 
 0 4 82 
 0 6 31 
 * This means that there are 100 vertices, and 4000 edges.
 * "0 1 89" means that the edge between vertex "0" and "1" is 89 distance apart. 

 ## Output dataset: 
 * Total entries/lines is equal to the total number of vertices.
 * Each line contains the SP distance for the N vertices. 

## Working of APSP via Floyd-Warshall algo: 
1. The main kernel is a 3 level nested loop with k, i, j with iteration bounds as N (total vertices)
2. We start with a distance matrix which contains the distance between two vertices. There is a distance matrix which is filled up using the input dataset. If the two vertices don't have a direct edge, then it is filled with INF (a large number).
3. The main kernel appends the distance matrix. The idea is: for any vertex pair (i,j) -- the shortest path should be tested with every intermediate vertex k, ie. (i,j) compared with (i,k) + (k,j)
* More details: https://en.wikipedia.org/wiki/Floyd%E2%80%93Warshall_algorithm

##  Design of Blocked APSP: 
* N vertices
* N x N "weight matrix", each element is >=0 stores the edge distance from each vertex i to other vertex j. 
* N x N "distance matrix", stores the shortest path distance from a vertex i to a vertex j. 
* Since it takes k steps to obtain the APSP distance matrix, we can have an intermediate distance matrix D(k). 

* Blocked APSP: (1) partition D into blocks (BxB submatrices), B is called as blocking factor. 

## Implementation discussion for blocking: 
* Deciding blocking factor -- have it parameterized. We can empirically check performance for {2,4,8,16,32,64}. Could this be bounded by the number of blocks to fit in the GPU? 
* Phase1: computing pivot block: each k can be parallelized! (intra-block)
eg: For a particular iteration k, d(1,1), d(1,2), d(2,1), d(2,2) can be parallelized because they are independent of each other, and only dependent on the previous iteration values.
* Phase2: computing pivot row/colm: each block can be parallelized! (inter-block)
* Phase3: computing other blocks: each block can be parallelized! (inter-block)


## Plan: 
* Let's first implement a sequential version of the blocked APSP. This would ensure that we have a correct working implementation with the blocked organization.
* Modity the sequential blocked to the parallel blocked -- what are the opportunities and how the semantics of the GPU could be used? 

## Sequential blocked APSP: 
* $ gcc blocked_seq.cc -lm
Note: -lm flag is needed as we have math library in the code.


