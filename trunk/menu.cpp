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


int menu(OSL_COLOR *bgstartColor, OSL_IMAGE *bgstart, OSL_IMAGE *start, int *alpha_, int* difficulty)
{
	bool menu_on = true, launch = false;
	int alpha, beta = 255, i, strHeight, menuPosX = WIDTH/2, curpos = 0;
	char* str_menu[4]; str_menu[0] = "Play", str_menu[1] = "Settings", str_menu[2] = "Credits", str_menu[3] = "Quit";
	
	OSL_FONT *f = oslLoadFontFile("flash0:/font/ltn0.pgf");
	
	oslIntraFontSetStyle(f, 1.f,RGBA(255,255,255,255), RGBA(0,0,0,0),INTRAFONT_ALIGN_CENTER);
	oslSetFont(f);
	strHeight = osl_curFont->charHeight;
	
	
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

	while (menu_on) {
	
		oslReadKeys();
		
		
		if (osl_pad.pressed.up)
		{
			curpos--;
			if (curpos < 0)	curpos = 3;
		}

		if (osl_pad.pressed.down)
		{
			curpos++;
			if (curpos > 3)	curpos = 0;
		}
		
		
		oslStartDrawing();
		oslDrawFillRect(0,0,WIDTH,HEIGHT,*bgstartColor);
		
		
		for (i=0;i<NB_MENU_ELMTS;i++) {
			if (curpos == i) {
				oslIntraFontSetStyle(f, 1.f,RGBA(255/(i+2),255/(i+2),255/(i+2),255), RGBA(0,0,0,0),INTRAFONT_ALIGN_CENTER);
				oslDrawString(menuPosX, MENUPOSY, str_menu[i]);
				oslIntraFontSetStyle(f, 1.f,RGBA(255,255,255,255), RGBA(0,0,0,0),INTRAFONT_ALIGN_CENTER);
			}
			else {
				oslDrawString(menuPosX, MENUPOSY, str_menu[i]);
			}
		}

		oslEndDrawing();
		oslEndFrame();
		oslSyncFrame();
		
		if (curpos == PLAY && osl_pad.pressed.cross) {
			menu_on = false;
			launch = true;
		}
		else if (curpos == QUIT && osl_pad.pressed.cross) {
			menu_on = false;
		}
		
	}

	oslDeleteFont(f);
	

	return launch;
}
