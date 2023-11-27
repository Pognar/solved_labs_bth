#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <pthread.h>
#include <sys/wait.h>
#include <semaphore.h>
#include <fcntl.h> /* For O_* constants */

#define 		SHMSIZE		128
#define 		SHM_R		0400
#define 		SHM_W		0200

const char *semName1 = "my_sema1";
const char *semName2 = "my_sema2";

int main(int argc, char **argv)
{
	struct shm_struct {
		int buffer[10];
		int size;
	};

	volatile struct shm_struct *shmp = NULL;
	char *addr = NULL;
	int var1 = 0, var2 = 0, shmid = -1;
	struct shmid_ds *shm_buf;


	/* allocate a chunk of shared memory */
	shmid = shmget(IPC_PRIVATE, SHMSIZE, IPC_CREAT | SHM_R | SHM_W);
	shmp = (struct shm_struct *) shmat(shmid, addr, 0);

	/* Initionalize shmp*/

    pid_t pid;
	sem_t *sem_id1 = sem_open(semName1, O_CREAT, O_RDWR, 10);
	sem_t *sem_id2 = sem_open(semName2, O_CREAT, O_RDWR, 0);

	pid = fork();
	int status, i;
	i = 0;

	if(sem_id1 == SEM_FAILED || sem_id1 == SEM_FAILED){

		printf("%s", "SEM FAILED!");
		//sem_close(sem_id1);
		//sem_close(sem_id2);
		sem_unlink(semName1);
		sem_unlink(semName2);

	}

	if (pid) {
		/* here's the parent, acting as producer */
		while (var1 <= 100) {

			sem_wait(sem_id1);
			shmp->buffer[i] = var1;
			printf("Send %d\n", shmp->buffer[i]); fflush(stdout);
			++i;
			sem_post(sem_id2);

			if(i == 10){
				i = 0;
			}

			++var1;
    
		}

		sem_close(sem_id1);
		sem_close(sem_id2);
		wait(&status);
		sem_unlink(semName1);
		sem_unlink(semName2);
		shmdt(addr);
		shmctl(shmid, IPC_RMID, shm_buf);

	} else {
		/* here's the child, acting as consumer */

		while (var2 <= 100) {

			sem_wait(sem_id2);
			var2 = shmp->buffer[shmp->size];
			++shmp->size;
			printf("Received %d\n", var2); fflush(stdout);
			++var2;
			if(shmp->size > 9){
				shmp->size = 0;
			}
            sem_post(sem_id1);
		}
		
		sem_close(sem_id1);
		sem_close(sem_id2);
		shmdt(addr);
		shmctl(shmid, IPC_RMID, shm_buf);
	}
}
