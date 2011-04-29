/*
game.h
by Yosh alias Hitman_07

23/04/11

*/


# ifndef H_GAME
# define H_GAME

int play(OSL_SOUND *mgame, OSL_FONT *f, int mode);
Result gameLevel(OSL_FONT *f, int hitDmg, int level, bool sublevel, OSL_COLOR bgColor, OSL_IMAGE *player);

# endif
