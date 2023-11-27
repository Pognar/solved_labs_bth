#include <stdio.h>
#include <unistd.h>

int main(int argc, char **argv)
{
    pid_t pid[2];
    unsigned i;
    unsigned niterations = 10;
    //Fork to create child process A
    pid[0] = fork();
    if (pid[0] == 0) { // process A
        printf("Child A ID: %d, Parent ID: %d\n", getpid(), getppid());
        for (i = 0; i < niterations; ++i)
            printf("A = %d, ", i);
    } else {
        //Fork to create child process C
        pid[1] = fork();
        if(pid[1] == 0) { // process C
            printf("Child C ID: %d, Parent ID: %d\n", getpid(), getppid());
            for (i = 0; i < niterations; ++i)
                printf("C = %d, ", i);  
        } else { //Parent Process B
            printf("PARENT B ID:%d\n", getpid());
            //Print out both the children
            printf("Child 1:%d\nChild 2:%d\n", pid[0], pid[1]);
            for (i = 0; i < niterations; ++i)
                printf("B = %d, ", i);
        }
    }
    printf("\n");
}
