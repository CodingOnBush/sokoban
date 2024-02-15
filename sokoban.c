#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>
#include "sokoban.h"

/*-----------------------------------------------*/

void ncurses_initialiser() {
    initscr();/* Démarre le mode ncurses */
    cbreak();/* Pour les saisies clavier (desac. mise en buffer) */
    noecho();/* Désactive l'affichage des caractères saisis */
    keypad(stdscr, TRUE);/* Active les touches spécifiques */
    refresh();/* Met a jour l'affichage */
    curs_set(FALSE);/* Masque le curseur */
    start_color();
}

void worldSize(char *myWorld, int* width, int* height){
    FILE* fic = fopen(myWorld, "r");
    int c=0;
    int temp=0;
    if(fic == NULL){
        printw("worldHeight | Erreur\n");
    }
    while( (c=fgetc(fic)) != EOF ){
        (*width) = (*width)+1;
        if( c == '\n'){
            (*height) = (*height)+1;
            if( *width > temp){
                temp = *width;
            }
            *width = 0;
        }
    }
    *width = temp-1;
    fclose(fic);
}

int** alloc2D(int width, int height){
    int** mat = NULL;
    mat = malloc(width*sizeof(int*));
    if(mat == NULL){
        printw("alloc2D | Erreur\n");
        exit(-1);
    }
    for(int i=0; i < width; i++)
    {
        mat[i]=(int*)malloc(height*sizeof(int));
        for(int j = 0 ; j < height ; j++){
            mat[i][j] = 1;//Initialisation à 1
        }
    }
    return mat;
}

int* alloc1D(int size){
    int* tab = NULL;
    tab = malloc(size*sizeof(int*));
    if(tab == NULL){
        printw("Erreur alloc1D\n");
        exit(-1);
    }
    for (int i=0; i<size; i++) {
        tab[i] = 0;//Initialisation à 0
    }
    return tab;
}

objet* allocStruct1D(int size){
    objet* tab = NULL;
    tab = malloc(size*sizeof(int*));
    if(tab == NULL){
        printw("Erreur alloc1D\n");
        exit(-1);
    }
    return tab;
}
void showWorld(int** tab, int width, int height){
    printw("\n");
    char conversion[9]={' ','#','$','*','.','@','+',' '};
    for (int j = 0 ; j < height ; j++){
        for (int i = 0 ; i < width ; i++){
            if(tab[i][j]>=0 && tab[i][j]<=8){
                printw("%c",conversion[tab[i][j]]);
            }
        }
        printw("\n");
    }
    printw("\n");
    printw("q : Exit | r : Restart\n");
    // printw("nombre de caisse : %d\n", caisseNumber(tab, width, height));
    // printw("nombre de deadLock : %d\n", deadlockNumber(tab, width, height));
}

void showWorldNumber(int **tab, int width, int height){
    printw("\n");
    for (int j = 0 ; j < height ; j++){
        for (int i = 0 ; i < width ; i++){
            printw("%d", tab[i][j]);
        }
        printw("\n");
    }
    printw("\n");
}

int** loadWorld(char *myLevel, int* width, int* height){
    int c = 0;
    int i = 0;
    int j = 0;
    FILE *level = NULL;
    worldSize(myLevel, width, height);
    int** map = alloc2D(*width, *height);
    if( (level= fopen(myLevel, "r")) == NULL){
        printw("Erreur d'ouverture du fichier dans loadLevel\n");
    }
    while( (c=fgetc(level)) != EOF ){
        if( c == '\n' ){
            map[i-1][j]=1;
            i=0;
            j++;
        }else{
            map[i][j] = c-48;
            i++;
        }
    }
    fclose(level);
    return map;
}

void posObjet(int **tab, int width, int height, int *i, int *j, int target){
    for (int h = 0 ; h < height ; h++){
        for (int w = 0 ; w < width ; w++){
            if( tab[w][h] == target ){
                *i = w;
                *j = h;
            }
        }
    }
}

// int checkWin(int** tab, int width, int height){
//     for (int i = 0; i < width; i++){
//         for (int j = 0; j < height; j++){
//             if(tab[i][j] == 2){return 0;/*pas gagner*/}
//         }
//     }
//     return 1;//gagner
// }

int gagner(int** tab, int width, int height){
    for (int i = 0; i < width; i++){
        for (int j = 0; j < height; j++){
            if(tab[i][j] == 2){return 0;/*pas gagner*/}
        }
    }
    return 1;//gagner
}

int regarder(char direction,int** tab, int width, int height, int x, int y, int range){
    switch (direction){
        case 'R':
            if( (x+range) < width ){
                return tab[x+range][y];
            }else{break;}
        case 'L':
            if( x >= range ){
                return tab[x-range][y];
            }else{break;}
        case 'U':
            if( y >= range ){
                return tab[x][y-range];
            }else{break;}
        case 'D':
            if( (y+range) < height ){
                return tab[x][y+range];
            }else{break;}
        default:
            return -1;
    }
    return -1;
}

void moveCaisse(char axes, char direction, int** tab, int i, int j, int devient){
    int c=1;
    if(axes == 'H')
    {
        if(direction == 'L'){
            c = (-1);
        }
        i=i+c;
        if(tab[i][j] == 2){
            tab[i][j] = 0;
            tab[i+c][j] = devient;
        }else if(tab[i][j] == 3){
            tab[i][j] = 4;
            tab[i+c][j] = devient;
        }
    }else
    {
        if(direction=='U'){
            c = (-1);
        }
        j=j+c;
        if(tab[i][j] == 2){
            tab[i][j] = 0;
            tab[i][j+c] = devient;
        }else if(tab[i][j] == 3){
            tab[i][j] = 4;
            tab[i][j+c] = devient;
        }
    }
}
void movePerso(char axes, char direction, int** tab, int* i, int* j, int devient){
    int c=1;
    if(axes == 'H'){
        if(direction == 'L'){
            c = -1;
        }
        if(tab[*i][*j] == 5){
            tab[*i][*j] = 0;
            tab[*i+c][*j] = devient;
            *i=*i+c;
        }else if(tab[*i][*j] == 6){
            tab[*i][*j] = 4;
            tab[*i+c][*j] = devient;
            *i=*i+c;
        }
    }else if(axes == 'V')
    {
        if(direction=='U'){
            c = -1;
        }
        if (tab[*i][*j] == 5) {
            tab[*i][*j] = 0;
            tab[*i][*j+c] = devient;
            *j=*j+c;
        }else if(tab[*i][*j] == 6){
            tab[*i][*j] = 4;
            tab[*i][*j+c] = devient;
            *j=*j+c;
        }
    }
}
void mouvement(char axes, char direction, int** tab, int* i, int* j, int width, int height){
    switch ( regarder(direction,tab,width,height,*i,*j,1) ){
        case 0:
            movePerso(axes, direction, tab, i, j, 5);
            break;
        case 4:
            movePerso(axes, direction, tab, i, j, 6);
            break;
        case 2:
            switch ( regarder(direction,tab,width,height,*i,*j,2) ) {
                case 0:
                    moveCaisse(axes, direction, tab, *i, *j, 2);
                    movePerso(axes, direction, tab, i, j, 5);
                    break;
                case 4:
                    moveCaisse(axes, direction, tab, *i, *j, 3);
                    movePerso(axes, direction, tab, i, j, 5);
                    break;
                case 7:
                    moveCaisse(axes, direction, tab, *i, *j, 2);
                    movePerso(axes, direction, tab, i, j, 5);
                    break;
            }
            break;
        case 3:
            switch ( regarder(direction,tab,width,height,*i,*j,2) ) {
                case 0:
                    moveCaisse(axes, direction, tab, *i, *j, 2);
                    movePerso(axes, direction, tab, i, j, 6);
                    break;
                case 4:
                    moveCaisse(axes, direction, tab, *i, *j, 3);
                    movePerso(axes, direction, tab, i, j, 6);
                    break;
                case 7:
                    moveCaisse(axes, direction, tab, *i, *j, 2);
                    movePerso(axes, direction, tab, i, j, 5);
                    break;
            }
            break;
        case 7:
            movePerso(axes, direction, tab, i, j, 5);
            break;
    }

}
void newWorld(char* myFile){
    int iPerso = 0;
    int jPerso = 0;
    int width = 0;
    int height = 0;
    int ch;
    int nbDeadLock = 0;
    int** map = NULL;

    map = loadWorld(myFile, &width, &height);
    posObjet(map, width, height, &iPerso, &jPerso, 5);

    // deadLock1(map, width, height);
    // int nb = deadlockNumber(map,width,height);
    // objet* deadTab = tabDeadlock(map, width, height, nb);
    showWorld(map, width, height);
    // printw("\nDans ce niveau il y a au moins %d deadLocks :\n",nb);
    // showStructTab(deadTab, nb);



    while( gagner(map, width, height) != 1 ){
        switch (ch = getch()) {
            case KEY_UP:
                clear();
                mouvement('V', 'U', map, &iPerso, &jPerso,width,height);
                showWorld(map, width, height);
                refresh();
                break;
            case KEY_DOWN:
                clear();
                mouvement('V', 'D', map, &iPerso, &jPerso,width,height);
                showWorld(map, width, height);
                refresh();
                break;
            case KEY_RIGHT:
                clear();
                mouvement('H', 'R', map, &iPerso, &jPerso,width,height);
                showWorld(map, width, height);
                refresh();
                break;
            case KEY_LEFT:
                clear();
                mouvement('H', 'L', map, &iPerso, &jPerso,width,height);
                showWorld(map, width, height);
                refresh();
                break;
            case 'q':
                clear();
                refresh();
                MENU();
                break;
            case 'r':
                clear();
                newWorld(myFile);
                refresh();
        }
    }
    winScreen();
}

/*-----------------------------------------------*/

WINDOW *create_newwin(){
    WINDOW *local_win;
	local_win = newwin(17, 16, 3, 5);
	box(local_win, 0 , 0);
	wrefresh(local_win);
	return local_win;
}
void menuEasyView(){
    WINDOW* my_win;
    my_win = create_newwin();
    mvwprintw(my_win,1,1,"Levels :");
    for (int i = 2; i <= 11; i++){
        mvwprintw(my_win,i,1,"%c.easy %d",(i+'a'-2),i-1);
    }
    mvwprintw(my_win,12,1,"r.Return");
    mvwprintw(my_win,13,1,"q.Exit");
    mvwprintw(my_win,15,1,"Write choice");
    wrefresh(my_win);
}
void menuMediumView(){
    WINDOW* my_win;
    my_win = create_newwin();
    mvwprintw(my_win,1,1,"Levels :");
    for (int i = 2; i < 5; i++){
        mvwprintw(my_win,i,1,"%c.medium %d",(i+'a'-2),i-1);
    }
    mvwprintw(my_win,5,1,"r.Return");
    mvwprintw(my_win,6,1,"q.Exit");
    mvwprintw(my_win,8,1,"Write choice");
    wrefresh(my_win);
}
void menuHardView(){
    WINDOW* my_win;
    my_win = create_newwin();
    mvwprintw(my_win,1,1,"Levels :");
    for (int i = 2; i < 5; i++){
        mvwprintw(my_win,i,1,"%c.hard %d",(i+'a'-2),i-1);
    }
    mvwprintw(my_win,5,1,"r.Return");
    mvwprintw(my_win,6,1,"q.Exit");
    mvwprintw(my_win,8,1,"Write choice");
    wrefresh(my_win);
}
void organizeEasy(){
    int ch;
    while( 1 ){
        switch (ch = getch()) {
            case 'a':
                clear();
                newWorld("easy_1.txt");
                break;
            case 'b':
                clear();
                newWorld("easy_2.txt");
                break;
            case 'c':
                clear();
                newWorld("easy_3.txt");
                break;
            case 'd':
                clear();
                newWorld("easy_4.txt");
                break;
            case 'e':
                clear();
                newWorld("easy_5.txt");
                break;
            case 'f':
                clear();
                newWorld("easy_6.txt");
                break;
            case 'g':
                clear();
                newWorld("easy_7.txt");
                break;
            case 'h':
                clear();
                newWorld("easy_8.txt");
                break;
            case 'i':
                clear();
                newWorld("easy_9.txt");
                break;
            case 'j':
                clear();
                newWorld("easy_10.txt");
                break;
            case 'r':
                clear();
                MENU();
                break;
            case 'q':
                clear();
                printw("Merci d'avoir joué à notre jeu :)");
                refresh();
                endwin();
                exit(-1);
            default:
                break;
        }
    }
}
void organizeMedium(){
    int ch;
    while(1){
        switch (ch = getch()) {
            case 'a':
                clear();
                newWorld("medium_1.txt");
                break;
            case 'b':
                clear();
                newWorld("medium_2.txt");
                break;
            case 'c':
                clear();
                newWorld("medium_3.txt");
                break;
            case 'r':
                clear();
                MENU();
                break;
            case 'q':
                clear();
                printw("Merci d'avoir joué à notre jeu :)");
                refresh();
                endwin();
                exit(-1);
            default:
                break;
        }
    }
}
void organizeHard(){
    int ch;
    while(1){
        switch (ch=getch()) {
            case 'a':
                clear();
                newWorld("hard_1.txt");
                break;
            case 'b':
                clear();
                newWorld("hard_2.txt");
                break;
            case 'c':
                clear();
                newWorld("hard_3.txt");
                break;
            case 'r':
                clear();
                MENU();
                break;
            case 'q':
                clear();
                printw("Merci d'avoir joué à notre jeu :)");
                refresh();
                endwin();
                exit(-1);
            default:
                break;
        }
    }
}
void MENU(){
    WINDOW* my_win;
    my_win = create_newwin();
    mvwprintw(my_win,1,1,"MENU :");
    mvwprintw(my_win,2,1,"a.Easy");
    mvwprintw(my_win,3,1,"b.Medium");
    mvwprintw(my_win,4,1,"c.Hard");
    mvwprintw(my_win,5,1,"q.Exit");
    mvwprintw(my_win,7,1,"Write choice");
    wrefresh(my_win);
    int ch;
    while(1){
        switch (ch=getch()) {
            case 'a':
                menuEasyView();
                organizeEasy();
                break;
            case 'b':
                menuMediumView();
                organizeMedium();
                break;
            case 'c':
                menuHardView();
                organizeHard();
                break;
            case 'q':
                clear();
                refresh();
                endwin();
                exit(-1);
            default:
                break;
        }
    }
    winScreen();
}
void winScreen(){
    clear();
    refresh();
    printw("Bravo vous avez gagner la partie maintenant appuyer sur une touche pour accéder au menu :)");
    getch();
    MENU();
    clear();
    refresh();
}
