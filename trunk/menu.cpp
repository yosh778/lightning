/*
menu.c
by Yosh alias Hitman_07

28/04/11

*/

#include <oslib/oslib.h>
#include "constants.h"
#include "tinylib.h"
#include "game.h"
#include "menu.h"


int menu(OSL_COLOR *bgstartColor, OSL_IMAGE *bgstart, OSL_IMAGE *start, int *alpha_)
{
	int difficulty = NORMAL;
	
	
	play(bgstartColor, bgstart,start,alpha_, difficulty);
	
	return 1;
}
