/* Structure représentant la position d'un objet dans le tableau */
typedef struct objet{
    int x;
    int y;
}objet;

typedef struct Vect_2di {
	int x;
	int y;
} Vect_2di_t;

/*    Fonctions permettant de faire fonctionner le jeu    */
/*****************************************************************************************************************/

void ncurses_initialiser();
/* Initialisation de ncurses pour les touches directionnelles */

void worldSize(char *myWorld, int* width, int* height);
/* width et height sont modifié en celle du fichier myWorld */

int** alloc2D(int width, int height);
/* retourne une matrice de taille width et height */

int* alloc1D(int size);
/* retourne un tableau de taille size */

objet* allocStruct1D(int size);
/* Retourne un tableau de structure objet de taille size */

void showWorld(int** tab, int width, int height);
/* Affiche le niveau */

void showWorldNumber(int **tab, int width, int height);
/* Affiche le niveau en chiffre */

int** loadWorld(char *myLevel, int* width, int* height);
/* retourne un tableau correspondant au niveau */

void posObjet(int **tab, int width, int height, int *i, int *j, int target);
/* modifie le i et j la position d'un objet */

int gagner(int** tab, int width, int height);
/* Retourne 1 si la partie est gagné et 0 sinon */

int regarder(char direction,int** tab, int width, int height, int x, int y, int range);
/* Retourne ce qu'il y a dans la case situé à range case de x et y et dans la direction que l'on souhaite */

void moveCaisse(char axes, char direction, int** tab, int i, int j, int devient);
/* Modifie la position d'un caisse situé à i et j dans la direction et sur l'axe que l'on veut */

void movePerso(char axes, char direction, int** tab, int* i, int* j, int devient);
/* Modifie la position du perso situé à i et j dans la direction et sur l'axe que l'on veut */

void mouvement(char axes, char direction, int** tab, int* i, int* j, int width, int height);
/* Cette fonction va s'occuper d'orchestrer le mouvement des caisses et du perso en fonction 
de ce qu'il y a autour d'elle et de la direction souhaité */

void newWorld(char* myFile);
/* Cette fonction utilise plusieurs fonctions parmi les précédentes afin d'afficher le niveau et utilise 
ncurses pour créer le mouvement à l'aide des touches directionnels et de la fonction mouvement */

/*****************************************************************************************************************/


/*    Fonctions permettant de réaliser le menu    */
/*****************************************************************************************************************/

WINDOW *create_newwin();/* Retoune une variable de type WINDOW afin de créer le cadre pour les menus du jeu */

void menuEasyView();/* La vue des niveaux easy */

void menuMediumView();/* La vue des niveaux medium */

void menuHardView();/* La vue des niveaux hard */

void organizeEasy();/* Lance newWorld en fonction du niveau easy choisit */

void organizeMedium();/* Lance newWorld en fonction du niveau medium choisit */

void organizeHard();/* Lance newWorld en fonction du niveau hard choisit */

void MENU();/* Affiche le menu principal qui permet de choisir entre les niveaux de difficulté */

void winScreen();/* La vue pour le succé d'un niveau */

/*****************************************************************************************************************/