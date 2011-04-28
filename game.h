/*
game.h
by Yosh alias Hitman_07

23/04/11

*/


# ifndef H_GAME
# define H_GAME

int play(OSL_COLOR *bgstartColor, OSL_IMAGE *bgstart, OSL_IMAGE *start, int *alpha, int mode);
Result gameLevel(int hitDmg, int level, bool sublevel, OSL_COLOR bgColor, OSL_IMAGE *player);

# endif
