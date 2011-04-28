/*
constants.h
by Yosh alias Hitman_07

23/04/11

*/


# ifndef H_CONSTANTS
# define H_CONSTANTS

						// main syst
# define WIDTH 480
# define HEIGHT 272
# define PAD_SENS 80
# define HITUNIT 30
# define HITDMG 1
# define R_BG 16
# define G_BG 118
# define B_BG 167
# define NB_MENU_ELMTS 4
# define BORDER_MENU 60
# define LAG_MENU ((HEIGHT - 2*BORDER_MENU - NB_MENU_ELMTS*strHeight) / (NB_MENU_ELMTS - 1))
# define MENUPOSY (BORDER_MENU + (i)*(strHeight + LAG_MENU))


						//game engine
# define MOVE_STEP 12
# define HITUNIT 30
# define TIMEUNIT 10*60
# define TIMEDELAY hitDelay*3
# define MAXBOLTX 4
# define MAXBOLTY 2
# define BOLTLAG 58
# define TBOLTLAG 36

						
						//bolt disposition parameters
# define BORDER_LAGX1 20
# define BORDER_LAGX2 60
# define BORDER_LAGY1 60
# define BORDER_LAGY2 60
# define N_LAGX1 ((WIDTH - 2*BORDER_LAGX1 - MAXBOLTX*vbolt->sizeX) / (MAXBOLTX - 1))
# define N_LAGX2 ((WIDTH - 2*BORDER_LAGX2 - MAXBOLTX*vbolt->sizeX) / (MAXBOLTX - 1))
# define N_LAGY1 ((HEIGHT - 2*BORDER_LAGY1 - MAXBOLTY*hbolt->sizeY) / (MAXBOLTY - 1))
# define N_LAGY2 ((HEIGHT - 2*BORDER_LAGY2 - MAXBOLTY*hbolt->sizeY) / (MAXBOLTY - 1))
# define ADDRESS_X1 (BORDER_LAGX1 + (j)*(vbolt->sizeX + N_LAGX1))
# define ADDRESS_X2 (BORDER_LAGX2 + (j)*(vbolt->sizeX + N_LAGX2))
# define ADDRESS_Y1 (BORDER_LAGY1 + (j)*(hbolt->sizeY + N_LAGY1))
# define ADDRESS_Y2 (BORDER_LAGY2 + (j)*(hbolt->sizeY + N_LAGY2))


enum {LEFT, RIGHT, UP, DOWN};
enum {EASY, NORMAL, HARD, GOD};
enum {PLAY, SETTINGS, CREDITS, QUIT};

typedef struct {
	int x;
	int y;
} Address;
typedef struct {
	int life;
	bool quit;
} Result;


# endif
