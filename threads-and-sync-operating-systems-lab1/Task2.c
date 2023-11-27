#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#define SHMSIZE 128
#define SHM_R 0400
#define SHM_W 0200

int main(int argc, char **argv)
{
	struct shm_struct {
		int buffer[10];
		int size;
		int in;
		int out;
	};
	volatile struct shm_struct *shmp = NULL;
	char *addr = NULL;
	pid_t pid = -1;
	int var1 = 0, var2 = 0, shmid = -1;
	struct shmid_ds *shm_buf;

	/* allocate a chunk of shared memory */
	shmid = shmget(IPC_PRIVATE, SHMSIZE, IPC_CREAT | SHM_R | SHM_W);
	shmp = (struct shm_struct *) shmat(shmid, addr, 0);

	/* Initionalize shmp*/
	shmp->in = 1;

	pid = fork();
	if (pid != 0) {
		/* here's the parent, acting as producer */
		while (var1 < 100) {
			/* write to shmem */
			var1++;
			while (shmp->out == shmp->in);  /* check so that buffer is not overwritten*/
			shmp->buffer[shmp->in] = var1;
			if(shmp->in == 9){
				shmp->in = 0;
			} else {
				shmp->in++;
			}
			shmp->size++;
			printf("Send %d\n", var1); fflush(stdout);
		}
		shmdt(addr);
		shmctl(shmid, IPC_RMID, shm_buf);
	} else {
		/* here's the child, acting as consumer */
		while (var2 < 100) {
			/* read from shmem */
			while (shmp->size == 0); /* busy wait until there is something */
			var2 = shmp->buffer[shmp->out];
			shmp->size--; 
			if(shmp->out == 9){
				shmp->out = 0;
			} else {
				shmp->out++;
			}
			printf("Received %d\n", var2); fflush(stdout);
		}
		shmdt(addr);
		shmctl(shmid, IPC_RMID, shm_buf);
	}
}