#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>
#include "menu.h"

int getExecutablePrograms(char programList[][MAX_SIZE]){
    int numberOfItems = 0;
    char buff[MAX_SIZE];
    FILE *fp = popen("find ./ -type f -perm -og+rx", "r"); //open pipe and read output from "find" command

    while (fgets(programList[numberOfItems++], MAX_SIZE, fp) != NULL);

    return (numberOfItems-1);
}

int main(){
    char programList[MAX_SIZE][MAX_SIZE];
    int numberOfExecutables = getExecutablePrograms(programList);

    printf("Please select a programm that you want to execute! %d\n", numberOfExecutables);
    int menuItemIndex = getMenuItemSelection(programList, numberOfExecutables);

    printf("\nthe programm that is about to execute after 5 seconds is: %s\n", programList[menuItemIndex]);
    sleep(5);
    char programName[strlen(programList[menuItemIndex])-1];
    char programPath[strlen(programList[menuItemIndex])];
    strncpy(programPath, programList[menuItemIndex], strlen(programList[menuItemIndex])-1);
    strcpy(programName, programPath+2);
    
    if(execl(programPath, programName, NULL)==-1){
        printf("somethiong went wrokg or couldn't find file %s!\n", programName);
    }
}