/***************************************************************************
 *
 * Sequential version of Matrix-Matrix multiplication
 *
 ***************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define SIZE 1024
#define N_THREADS 4

static double a[SIZE][SIZE];
static double b[SIZE][SIZE];
static double c[SIZE][SIZE];


void* init_matrix(void* params)
{
    unsigned long i, j;
    i = (unsigned long)params;

    for (j = 0; j < SIZE; j++) {
        // Simple initialization, which enables us to easy check
        //    * the correct answer. Each element in c will have the same
        //    * value as SIZE after the matmul operation.
        a[i][j] = 1.0;
        b[i][j] = 1.0;
    }
}

void* matmul_seq(void* params) {
    unsigned long p, r, k, d;

    p = (unsigned long)params; //start
    
    for (r = p; r < p+256; r++) {
        for (d = 0; d < SIZE; d++) {
            c[r][d] = 0.0;
            for (k = 0; k < SIZE; k++)
                c[r][d] = c[r][d] + a[r][k] * b[k][d];
        }
    }
}

static void
print_matrix(void) {
    int i, j;

    for (i = 0; i < SIZE; i++) {
        for (j = 0; j < SIZE; j++)
	        printf(" %7.2f", c[i][j]);
	    printf("\n");
    }
}

int main(int argc, char **argv) {
    pthread_t threads[SIZE];

    for(unsigned long i = 0; i < SIZE; ++i){
        pthread_create(&threads[i], NULL, init_matrix, (void*)i);

    }
    for(unsigned long i = 0; i < SIZE; ++i){
        
        pthread_join(threads[i], NULL);

    }
    for(unsigned long i = 0; i < N_THREADS; ++i){
        unsigned long j = (i * 1024/4);
        pthread_create(&threads[i], NULL, matmul_seq, (void*)j);

    }
    for(unsigned long i = 0; i < N_THREADS; ++i){
        
        pthread_join(threads[i], NULL);

    }
    print_matrix();
}