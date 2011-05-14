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
bool overScreen(OSL_SOUND *congrats, OSL_FONT *f, bool over);
bool quitScreen(OSL_FONT *f);
void settings(OSL_SOUND *cancel, OSL_SOUND *mstart, bool *returned, int *bg_col_m, Color *Bg_col, OSL_FONT *f, OSL_COLOR *bgstartColor, int *difficulty);
int load(Color *Bg_col, int *difficulty);
int save(Color *Bg_col, int *difficulty);
bool strtsnd_if(OSL_SOUND *cancel, OSL_SOUND *mstart, bool *returned);

# endif
