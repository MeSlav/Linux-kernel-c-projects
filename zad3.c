#include <stdio.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/shm.h>


/*================ GLOBAL VARIABLES ================*/
int* favouredThread;
int* flags;

int* criticalSection;


/*================ DEKKERT's ALGORITHM ================*/
void enterCriticalSection(int i, int j){
  flags[i] = 1;

  while(flags[j]){ //while other thread is trying to enter critical section
    if(*favouredThread == j){ //if other thread is set as favoured
      flags[i] = 0; //let other thread execute
      while(*favouredThread == j); // wait for the other thread to finish
      flags[i] = 1; //execute current thread
    }
  }

  //MANIPULATE WITH CRITICAL SECTION HERE
  if(i && !j) (*criticalSection)++; //if first thread is entering
  else (*criticalSection)--; //if second thread is entering
}

void exitCriticalSection(int i, int j){
  *favouredThread = j; //set other thread to be favoured
  flags[i] = 0; //set current thread to stop trying to enter critical section
}

void executeThread(int i){
  int k, m;
  int isThread1 = i==1;
  int isThread2 = i==2;

  for(k = 1; k<=5; k++){
    enterCriticalSection(isThread1, isThread2);
    
    for(m = 1; m<=5; m++){
      printf("thread: %d is entering critical section for the %d. time (this is %d/5 print statements) - value of critical section = %d\n", i, k, m, *criticalSection);
    }

    exitCriticalSection(isThread1, isThread2);
  }
}

void detachAndDestroySharedMemory(int id1, int id2){
  //detach
  shmdt(favouredThread);
	shmdt(flags);
  //destroy
	shmctl(id1,IPC_RMID,NULL);
	shmctl(id2,IPC_RMID,NULL);
}

/*================ MAIN ================*/
int main(){
  int favouredThreadShmId = shmget(IPC_PRIVATE, sizeof(int), SHM_R|SHM_W);
  int flagsShmId = shmget(IPC_PRIVATE, sizeof(int)*2, SHM_R|SHM_W);
  int criticalSectionShmId = shmget(IPC_PRIVATE, sizeof(int), SHM_R|SHM_W);

  favouredThread = shmat(favouredThreadShmId, NULL, 0);
  flags = shmat(flagsShmId, NULL, 0);
  criticalSection = shmat(criticalSectionShmId, NULL, 0);

  if(fork() == 0){
    //child because return is 0
    executeThread(2);
  }else{
    //parent because return is non zero
    executeThread(1);
    wait(NULL);
    detachAndDestroySharedMemory(favouredThreadShmId, flagsShmId);
  }
    return 0;
}