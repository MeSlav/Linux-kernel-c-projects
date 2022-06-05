#include <stdio.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>
#include "menu.h"

#define ITEMS_COUNT 4 //number of menu items

char menuItems[ITEMS_COUNT][MAX_SIZE] = {
        "1.) Run a random process.\n",
        "2.) Print out all active processes.\n",
        "3.) Select a process to end.\n",
        "4.) Close all programs.\n"
    };

void startRandomProcess(){
    while(1){
        printf("The PID of the current process is --> %d. And the parent process is --> %d\n", getpid(), getppid());
	    sleep(3);
    }
}

void printActiveProcessdes(){
    //we can also use "exec" family of instructions to execute bash commands from path "/bin/*command_name*"
    // execl("/bin/ps","ps","-la", NULL);
    system("ps -l");
}

void killAllProcesses(){
    //kill all processes except users login shell, init, and kernel-specific processes.
    system("killall5 -9");
}

void selectProcessToKill(){
    int pid;
    printf("Please input a \"PID\" (Process ID) to kill: ");
    scanf("%d", &pid);
    kill(pid, SIGKILL);
}

void executeBasedOnUserSelection(int itemIndex){
    if(itemIndex!=-1) printf("\nyou selected:\n%s\n", menuItems[itemIndex]);

    switch (itemIndex){
        case 0:
            startRandomProcess();
            break;
        case 1:
            printActiveProcessdes();
            break;
        case 2:
            selectProcessToKill();
            break;
        case 3:
            killAllProcesses();
            break;
        default:
            printf("\n\nSomething went wrong while getting your menu item selection, please try again!\n");
            break;
    }
}

int main(){
    int menuItemIndex = getMenuItemSelection(menuItems, ITEMS_COUNT);
    executeBasedOnUserSelection(menuItemIndex);

    return 0;
}