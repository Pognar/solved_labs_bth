#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

#define N 5
#define LEFT (i + 4) % N
#define RIGHT (i + 1) % N
typedef enum {THINK, HUNGRY, EATING} prof_state;

unsigned long professor[N];
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;

void put_forks_down(unsigned long i) {
    if (professor[i] == EATING) {
        printf("Philosopher %ld putting fork %ld and %ld down\n",
           i, i, RIGHT);
    }else {
        printf("Philosopher %ld did not get to eat\n",i);
    }
    professor[i] = THINK;
}

void think(unsigned long i, int high, int low, char* string) {
    printf("Philosopher %ld is %s\n", i, string);
    int num = (rand() % (high-low)) + low;
    sleep(num);
}

void take_left(unsigned long i) {
    pthread_mutex_lock(&lock);
    if (professor[LEFT] != EATING && professor[LEFT] != HUNGRY) {
        professor[i] = HUNGRY;
        printf("Philosopher %ld is picking up the %ld (LEFT) chopstick\n", i, LEFT);
    }
    pthread_mutex_unlock(&lock);
}
void take_right(unsigned long i) {
    if (professor[i] == HUNGRY) {
        pthread_mutex_lock(&lock);
        if (professor[RIGHT] != HUNGRY && professor[RIGHT] != EATING) {
            professor[i] = EATING;
            printf("Philosopher %ld is picking up the %ld (RIGHT) chopstick\n", i, (RIGHT));
        }
        pthread_mutex_unlock(&lock);
    }
}

void eat(unsigned long i) {
    if(professor[i] == EATING) {
        think(i, 10, 20, "eating");
    }
}

void* professor_main(void* id) {
    unsigned long i = (unsigned long)id;
    while (1) {
        think(i,2, 10, "thinking");
        take_left(i);
        think(i, 1, 3, "thinking");
        take_right(i);
        eat(i);
        put_forks_down(i);
    }
}

int main() {
    pthread_mutex_init(&lock, NULL);
    pthread_t *thread_id;
    unsigned long i = 0;
    thread_id = malloc( N * sizeof(pthread_t) );
    for (i = 0; i < N; i++) {
        pthread_create(&(thread_id[i]), NULL, professor_main, (void*)i);
    }
    for (i = 0; i < N; i++) {
        pthread_join(thread_id[i], NULL);
    }
    free(thread_id);
    pthread_mutex_destroy(&lock);
    return 0;
}