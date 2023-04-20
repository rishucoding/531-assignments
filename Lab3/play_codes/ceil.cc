#include <stdio.h>
#include <math.h>

int main(){
    int N = 100;
    int blocking_factor = 4;
    int rounds = ceil(N/(double)blocking_factor);
    printf("Rounds: %d\n", rounds);
}