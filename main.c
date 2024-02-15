#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>
#include "sokoban.h"

int main(int argc, char* argv[]){
    ncurses_initialiser();

    MENU();
    
    refresh();
    endwin();
    
    return 0;
}
