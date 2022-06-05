#include<stdio.h>
#include<stdlib.h>
#include<semaphore.h>
#include<pthread.h>
#include<sys/ipc.h>
#include<sys/shm.h>
#include<unistd.h>

#define PHILOSOPHER_COUNT 5
//philosopher states
#define THINKING 0
#define EATING 2
//fork states
#define FORK_TAKEN 0
#define FORK_FREE 1

pthread_mutex_t printMutex = PTHREAD_MUTEX_INITIALIZER;

sem_t forks[PHILOSOPHER_COUNT];
sem_t waitingPhilosophers;
int philosopherStates[PHILOSOPHER_COUNT];
int philosopherIndexes[] = {0, 1, 2, 3, 4};

void initializeState(){
	//set waiting semaphore to max 4 philosophers
	sem_init(&waitingPhilosophers, 1, PHILOSOPHER_COUNT-1);

	//initialize philosopher states
	for(int i=0; i<PHILOSOPHER_COUNT; i++) {
		philosopherStates[i] = THINKING;
	}

	//initialize state for forks
	for(int i=0; i<PHILOSOPHER_COUNT; i++) {
		sem_init(&forks[i], 1, FORK_FREE);
	}
}

void printState(){
	pthread_mutex_lock(&printMutex);
	//clear screen every time new state is printed
	system("@cls||clear");

	//print state
	printf("Philosopher states:\n");
	for(int i=0; i<PHILOSOPHER_COUNT; i++){
		printf("Philosopher %d, state: ", i+1);

		switch (philosopherStates[i]) {
			case THINKING:
				printf("Thinking.\n");
				break;
			case EATING:
				printf("Eating.\n");
				break;
		}
	}
	pthread_mutex_unlock(&printMutex);
}

void think(int philosopherIndex){
	philosopherStates[philosopherIndex] = THINKING;
	printState();
	sleep(3);
}

void eat(int philosopherIndex){
	sem_wait(&waitingPhilosophers);
	sem_wait(&forks[philosopherIndex]);
	sem_wait(&forks[(philosopherIndex+1)%PHILOSOPHER_COUNT]);

	philosopherStates[philosopherIndex] = EATING;
	printState();
	sleep(2);

	sem_post(&forks[philosopherIndex]);
	sem_post(&forks[(philosopherIndex+1)%PHILOSOPHER_COUNT]);
	sem_post(&waitingPhilosophers);
}

void* philosopher(void* index){
	int* philosopherIndex = index;

	while(1){
		think(*philosopherIndex);
		eat(*philosopherIndex);
	}
}

int main()
{
	pthread_t philosopherThreads[PHILOSOPHER_COUNT];
	initializeState();
	printState();

	//create threads
	for(int i=0; i<PHILOSOPHER_COUNT; i++) {
		pthread_create(&philosopherThreads[i], NULL, philosopher, &philosopherIndexes[i]);
	}

	//join threads
	for(int i=0; i<PHILOSOPHER_COUNT; i++) {
		pthread_join(philosopherThreads[i], NULL);
	}

	return 0;
}