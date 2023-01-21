#include<stdio.h>
#include<sys/ipc.h>
#include<sys/shm.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<unistd.h>
#include<string.h>
#include<semaphore.h>
#include<stdlib.h>
#include<signal.h>

#define SEM_WRITE 0
#define SEM_FULL 1
#define SEM_EMPTY 2

struct sharedMemory{
  sem_t semaphores[3];
  sem_t userInputSemaphore;
  char buffer[5];
  int in;
  int out;
};

struct sharedMemory *shm;
int shmid;


void writer(int n){
  // wait for first user input
  sem_wait(&shm->userInputSemaphore);
  if(n==1) sem_wait(&shm->semaphores[SEM_WRITE]);

  // take user input
  char input[256];
  printf("Please input some text for writer %d: ", n);
  scanf("%s", input);

  // force next user input
  sem_post(&shm->userInputSemaphore);
  if(n==2) sem_post(&shm->semaphores[SEM_WRITE]);

  // write after both user inputs are in
  for(int i=0; i<strlen(input)+1; i++){
    sem_wait(&shm->semaphores[SEM_WRITE]);
    sem_wait(&shm->semaphores[SEM_FULL]);

    shm->buffer[shm->in] = input[i];
    printf("writer %d storing --> %c\n", n, input[i]);
    shm->in = (shm->in+1)%5;

    sem_post(&shm->semaphores[SEM_WRITE]);
    sem_post(&shm->semaphores[SEM_EMPTY]);
  }
}

void reader(){
  char inputs[512];
  int endOfInputCount = 0;
  int count = 0;

  while(endOfInputCount<2){
    sem_wait(&shm->semaphores[SEM_EMPTY]);

    if(!shm->buffer[shm->out]) endOfInputCount++;
    else inputs[count++] = shm->buffer[shm->out];
    printf("reader is reading --> %c\n", shm->buffer[shm->out]);
    shm->out = (shm->out+1)%5;
    // sleep(1);

    sem_post(&shm->semaphores[SEM_FULL]);
  }
  inputs[count] = 0;

  printf("\n\nREAD FROM INPUTS --> %s\n", inputs);
}

int main()
{
	shmid=shmget(IPC_PRIVATE,sizeof(struct sharedMemory),SHM_R|SHM_W);
	shm=shmat(shmid,NULL,0);
	shm->in=0;
  shm->out=0;
	shm->buffer[0]='\0';

  sem_init(&(shm->semaphores[SEM_WRITE]), 1, 1);
  sem_init(&(shm->semaphores[SEM_FULL]), 1, 1);
  sem_init(&(shm->semaphores[SEM_EMPTY]), 1, 0);
  sem_init(&shm->userInputSemaphore, 1, 1);

  if(fork()==0)
	{
		writer(1);
	}
	else if(fork()==0)
	{
		writer(2);
	}
	else
	{
		reader();
	}

  wait(NULL);
  wait(NULL);

	return 0;
}