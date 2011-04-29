/*
game.h
by Yosh alias Hitman_07

23/04/11

*/


# ifndef H_TINYLIB
# define H_TINYLIB

int imFadeIn(OSL_IMAGE *image, int alpha, int decay);
bool splashScreen(char *location, int timeLimit, int decay, OSL_COLOR bgColor);
bool startScreen(OSL_FONT *f, int level, bool sublevel);
bool overScreen(OSL_FONT *f, bool over);
bool quitScreen(OSL_FONT *f);
void settings(int *bg_col_m, Color *Bg_col, OSL_FONT *f, OSL_COLOR *bgstartColor, int *difficulty);

# endif
