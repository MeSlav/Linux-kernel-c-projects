#include <stdio.h>
#include "myGetch.h"
#include <stdlib.h>

#define ESC_KEY 27
#define ENTER_KEY 10
#define TAB_KEY 9

#define MAX_SIZE 301 //max size of a string

void markIfSelected(int selectedIndex, int itemIndex){
    (selectedIndex == itemIndex)? printf("-->") : printf("   ");
}

int getMenuItemSelection(char menuItems[][MAX_SIZE], int numberOfItems){
    int itemIndex = 0;
    int inputChar = 0;

    while(inputChar!=ENTER_KEY){
        system("@cls||clear");

        printf("Please select one of the following:\n\n");
        for(int i = 0; i<numberOfItems; i++){
            markIfSelected(itemIndex, i);
            printf("%s", menuItems[i]);
        }
        printf("\nUse TAB key to navigate and press ENTER to select.\n");
        printf("If you want to terminate program, then press and hold CTRL + C keys.\n");
        inputChar = getch();

        if(inputChar == TAB_KEY) itemIndex = (itemIndex+1)%numberOfItems;
    }

    return (inputChar!=ESC_KEY)? itemIndex : -1;
}