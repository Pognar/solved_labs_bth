/***************************************************************************
 *
 * Sequential version of Matrix-Matrix multiplication
 *
 ***************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define SIZE 1024

static double a[SIZE][SIZE];
static double b[SIZE][SIZE];
static double c[SIZE][SIZE];


	        /* Simple initialization, which enables us to easy check
	         * the correct answer. Each element in c will have the same
	         * value as SIZE after the matmul operation.
	         */

void* init_matrix(void* params){
    unsigned long i, j;
    i = (unsigned long)params;

    for (j = 0; j < SIZE; j++) {
	        a[i][j] = 1.0;
	        b[i][j] = 1.0;
    }
}

void* matmul_seq(void* params){
    unsigned long i, j, k;
    i = (unsigned long)params;
   
    for (j = 0; j < SIZE; j++) {
        c[i][j] = 0.0;
        for (k = 0; k < SIZE; k++){
            c[i][j] = c[i][j] + a[i][k] * b[k][j];
        }
    }
}

static void
print_matrix(void)
{
    int i, j;

    for (i = 0; i < SIZE; i++) {
        for (j = 0; j < SIZE; j++)
	        printf(" %7.2f", c[i][j]);
	    printf("\n");
    }
}

int main(int argc, char **argv){

    pthread_t init_thr[SIZE];
    for(unsigned long i = 0; i < SIZE; ++i){

        pthread_create(&init_thr[i], NULL, init_matrix, (void*)i);

    }
    for(unsigned int i = 0; i < SIZE; ++i){
        pthread_join(init_thr[i], NULL);
    }

    pthread_t threads[SIZE];


    //init_matrix();
    for(unsigned long i = 0; i < SIZE; ++i){
        pthread_create(&threads[i], NULL, matmul_seq, (void*)i);
    }
    for(unsigned int i = 0; i < SIZE; ++i){
        pthread_join(threads[i], NULL);
    }
    //print_matrix();
}
