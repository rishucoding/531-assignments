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

##  Design of Blocked APSP: 
* N vertices
* N x N "weight matrix", each element is >=0 stores the edge distance from each vertex i to other vertex j. 
* N x N "distance matrix", stores the shortest path distance from a vertex i to a vertex j. 
* Since it takes k steps to obtain the APSP distance matrix, we can have an intermediate distance matrix D(k). 

* Blocked APSP: (1) partition D into blocks (BxB submatrices), B is called as blocking factor. 
