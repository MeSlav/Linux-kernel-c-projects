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
#define WAITING 1
#define EATING 2
//fork states
#define FORK_TAKEN 0
#define FORK_FREE 1

pthread_cond_t condition = PTHREAD_COND_INITIALIZER;
pthread_mutex_t monitor = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t printMutex = PTHREAD_MUTEX_INITIALIZER;

int forks[PHILOSOPHER_COUNT];
int philosopherStates[PHILOSOPHER_COUNT];
int philosopherIndexes[] = {0, 1, 2, 3, 4};

void initializeState(){
	//initialize philosopher states
	for(int i=0; i<PHILOSOPHER_COUNT; i++) {
		philosopherStates[i] = THINKING;
	}

	//initialize state for forks
	for(int i=0; i<PHILOSOPHER_COUNT; i++) {
		forks[i] = FORK_FREE;
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
			case WAITING:
				printf("Waiting for forks.\n");
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

void startEating(int philosopherIndex){
    //enter critical section
    pthread_mutex_lock(&monitor);

    philosopherStates[philosopherIndex] = WAITING;
    while(forks[philosopherIndex]==FORK_TAKEN || forks[(philosopherIndex+1)%PHILOSOPHER_COUNT]==FORK_TAKEN){
        pthread_cond_wait(&condition, &monitor);
    }

    forks[philosopherIndex] = FORK_TAKEN;
    forks[(philosopherIndex+1)%PHILOSOPHER_COUNT] = FORK_TAKEN;
    philosopherStates[philosopherIndex] = EATING;
    printState();

    //exit critical section
    pthread_mutex_unlock(&monitor);
}

void stopEating(int philosopherIndex){
    //enter critical section
    pthread_mutex_lock(&monitor);

    forks[philosopherIndex] = FORK_FREE;
    forks[(philosopherIndex+1)%PHILOSOPHER_COUNT] = FORK_FREE;
    philosopherStates[philosopherIndex] = THINKING;
	pthread_cond_broadcast(&condition);
    printState();

    //exit critical section
    pthread_mutex_unlock(&monitor);
}

void eat(int philosopherIndex){
    startEating(philosopherIndex);
	sleep(2);
    stopEating(philosopherIndex);
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