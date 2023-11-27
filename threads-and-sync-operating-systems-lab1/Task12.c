#include    <stdio.h>
#include    <stdlib.h>
#include    <unistd.h>
#include    <semaphore.h>
#include    <pthread.h>
#include    <time.h>

typedef enum {Tanenbaum, Bos, Lamport, Stallings, Silberschatz} professors;

void red () {
  printf("\033[1;31m");
}

void reset () {
  printf("\033[0m");
}


int chop_sticks[5] = {1,1,1,1,1};

struct hands{
    unsigned int id;
    unsigned int left_hand;
    unsigned int right_hand;
    unsigned int full;
    char* name;
};

void* kids_func(void* parameters){
    struct hands *traits = (struct hands*) parameters;

    //switch to assign names
    switch(traits->id){
        case Tanenbaum:
            traits->name = "Tanenbaum";
            traits->left_hand = 4;
            traits->right_hand = 0;
            break;
        case Bos:
            traits->name = "Bos";
            traits->left_hand = 0;
            traits->right_hand = 1;
            break;
        case Lamport:
            traits->name = "Lamport";
            traits->left_hand = 1;
            traits->right_hand = 2;
            break;
        case Stallings:
            traits->name = "Stallings";
            traits->left_hand = 2;
            traits->right_hand = 3;
            break;
        case Silberschatz:
            traits->name = "Silberschatz";
            traits->left_hand = 3;
            traits->right_hand = 4;
            break;
        default:
            break;
    }

    while(traits->full < 30){
        int think_time;
        think_time = (rand()% 10) + 2;
        printf("\n%s is thinking.. ", traits->name);
        sleep(think_time);
        printf("\n%s finished his thinking!", traits->name);


        printf("\n");
        for(unsigned i = 0; i < 5; ++i){
            printf("%d", chop_sticks[i]);
        }
        printf("\n");

        if(chop_sticks[traits->left_hand] == 1){

            chop_sticks[traits->left_hand] = 0;
            printf("\n%s picked up the left chopstick!", traits->name);

            think_time = (rand()% 3) + 1;
            printf("\n%s is thinking.. ", traits->name);
            sleep(think_time);

            if(chop_sticks[traits->right_hand] == 1){

                chop_sticks[traits->right_hand] = 0;
                red();
                printf("\n%s picked up the right chopstick!", traits->name);
                reset();
                think_time = (rand()% 20) + 10;
                red();
                printf("\n%s is eating.. ", traits->name);
                reset();
                sleep(think_time);
                traits->full += think_time;
                chop_sticks[traits->left_hand] = 1;
                chop_sticks[traits->right_hand] = 1;
                red();
                printf("\n%s FINISHED EATING!", traits->name);
                reset();


            } else {

                printf("\n%s tried to picked up the right chopstick!", traits->name);
                chop_sticks[traits->left_hand] = 1;

            }
        } else {
            printf("\n%s tried to pick up the left chopstick!", traits->name);
            
        }


    }
    red();
    printf("\n%s IS FULL!", traits->name);
    reset();


}


/*
The Dining problem..
 Five plates, five chopsticks
 in order to eat you need two chopsticks
 Which means they need to take turns eating.

 -There are always five plates. 

 If two people are eating there is one free chopstick
 - and four which are occupied.
 - At any given time only two people can eat at the same time.

 - Put the left chopstick down if the right one is taken.
*/




int main(int argc, char** argv){

    pthread_t *kids;
    struct hands* args;
    unsigned long id = 0;
    unsigned long n_threads = 5;

    kids = malloc (n_threads * sizeof(pthread_t));
    args = malloc (n_threads * sizeof(struct hands));
    
    for(unsigned int id = Tanenbaum; id <= Silberschatz; ++id){
        args[id].left_hand = 0;
        args[id].right_hand = 0;
        args[id].id = id;
        pthread_create(&(kids[id]), NULL, kids_func, (void*)&args[id]);

    }

    for (unsigned int id = 0; id < n_threads; id++) {

		pthread_join(kids[id], NULL);

	}


    free(args);
    free(kids);
    return 0;

}