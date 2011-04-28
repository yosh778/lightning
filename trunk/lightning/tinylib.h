/*
game.h
by Yosh alias Hitman_07

23/04/11

*/


# ifndef H_TINYLIB
# define H_TINYLIB

int imFadeIn(OSL_IMAGE *image, int alpha, int decay);
bool splashScreen(char *location, int timeLimit, int decay, OSL_COLOR bgColor);
bool startScreen(int level, bool sublevel);
bool overScreen(bool over);
bool quitScreen();

# endif
