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
	int alpha, beta = 255, difficulty = NORMAL;

	
	while(beta > 0)
	{
		beta-=10;

		if (beta < 0)	beta = 0;

		alpha = floor(*alpha_ * beta/255);

		oslStartDrawing();
		oslDrawFillRect(0,0,WIDTH,HEIGHT,*bgstartColor);
		oslSetAlpha(OSL_FX_ALPHA, beta);
		oslDrawImage(bgstart);
		oslSetAlpha(OSL_FX_DEFAULT, 0);
		imFadeIn(start, alpha, 0);
		oslEndDrawing();
		oslEndFrame();
		oslSyncFrame();
	}



	play(difficulty);


	return 1;
}
