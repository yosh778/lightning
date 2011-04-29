/*
game.h
by Yosh alias Hitman_07

23/04/11

*/


# ifndef H_GAME
# define H_GAME

int play(Color *Bg_col, int *bg_col_m, OSL_SOUND *mgame, OSL_FONT *f, int mode);
Result gameLevel(OSL_SOUND *mgame, Color *Bg_col, int *bg_col_m, OSL_FONT *f, int hitDmg, int level, bool sublevel, OSL_COLOR bgColor, OSL_IMAGE *player);

# endif
