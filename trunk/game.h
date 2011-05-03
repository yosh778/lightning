/*
game.h
by Yosh alias Hitman_07

23/04/11

*/


# ifndef H_GAME
# define H_GAME

int play(OSL_SOUND *congrats, OSL_SOUND *lost, OSL_SOUND *won, OSL_SOUND *appear, OSL_SOUND *quit_open, OSL_SOUND *quit_close, OSL_SOUND *cancel, OSL_SOUND *critic, OSL_SOUND *hurt, Color *Bg_col, int *bg_col_m, OSL_SOUND *mgame, OSL_FONT *f, int mode);
Result gameLevel(OSL_SOUND *quit_open, OSL_SOUND *quit_close, OSL_SOUND *cancel, OSL_SOUND *critic, OSL_SOUND *hurt, OSL_SOUND *mgame, Color *Bg_col, int *bg_col_m, OSL_FONT *f, int hitDmg, int level, bool sublevel, OSL_COLOR bgColor, OSL_IMAGE *player);

# endif
