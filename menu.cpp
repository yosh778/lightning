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


int menu(OSL_SOUND *fx_up, OSL_SOUND *fx_dn, OSL_SOUND *cancel, OSL_SOUND *mstart, bool *returned, int *bg_col_m, Color *Bg_col, OSL_FONT *f, OSL_COLOR *bgstartColor, OSL_IMAGE *bgstart, OSL_IMAGE *start, int *alpha_, int* difficulty)
{
	bool menu_on = true, launch = false, go_up = false;
	int alpha, beta = 255, i, strHeight, menuPosX, curpos = 0;
	char* str_menu[NB_MENU_ELMTS];
	str_menu[0] = "Play", str_menu[1] = "Settings", str_menu[2] = "Credits", str_menu[3] = "Quit";
	OSL_IMAGE *menu_bg = NULL;
	menu_bg = oslLoadImageFilePNG("./res/menu.png",OSL_IN_RAM,OSL_PF_8888);
	menu_bg->x = ((3*WIDTH)/2 - menu_bg->sizeX)/2;
	menu_bg->y = (HEIGHT - menu_bg->sizeY) / 2;

	menuPosX = menu_bg->x/2;

	oslIntraFontSetStyle(f, 1.25f,RGBA(255,255,255,255), RGBA(0,0,0,255),INTRAFONT_ALIGN_CENTER);
	oslSetFont(f);
	strHeight = osl_curFont->charHeight;

	oslSetKeyAnalogToDPad(PAD_SENS);


	while(beta > 0 && !osl_quit)
	{
		strtsnd_if(cancel, mstart, returned);
		beta-=10;

		if (beta < 0)	beta = 0;

		alpha = floor(*alpha_ * beta/255);

		oslStartDrawing();
		//oslDrawFillRect(0,0,480,272,*bgstartColor);
		oslDrawGradientRect(0,0,WIDTH,HEIGHT,*bgstartColor,*bgstartColor,RGB((Bg_col->r)*(*bg_col_m),(Bg_col->g)*(*bg_col_m),(Bg_col->b)*(*bg_col_m)),*bgstartColor);
		oslSetAlpha(OSL_FX_ALPHA, beta);
		oslDrawImage(bgstart);
		oslSetAlpha(OSL_FX_DEFAULT, 0);
		imFadeIn(start, alpha, 0);
		oslEndDrawing();
		oslEndFrame();
		oslSyncFrame();
	}

	oslSetKeyAutorepeat(OSL_KEYMASK_UP | OSL_KEYMASK_DOWN | OSL_KEYMASK_LEFT | OSL_KEYMASK_RIGHT,40,6);

	alpha = 255;

	while (menu_on && !osl_quit) {

		oslReadKeys();

		
		strtsnd_if(cancel, mstart, returned);

		if (osl_pad.pressed.up)
		{
			if(oslGetSoundChannel(fx_up) != -1)	oslStopSound(fx_up);
			oslPlaySound(fx_up, 5);
			curpos--;
			if (curpos < 0)	curpos = 3;
		}

		if (osl_pad.pressed.down)
		{
			if(oslGetSoundChannel(fx_dn) != -1)	oslStopSound(fx_dn);
			oslPlaySound(fx_dn, 5);
			curpos++;
			if (curpos > 3)	curpos = 0;
		}


		oslStartDrawing();
		//oslDrawFillRect(0,0,480,272,*bgstartColor);
		oslDrawGradientRect(0,0,WIDTH,HEIGHT,*bgstartColor,*bgstartColor,RGB((Bg_col->r)*(*bg_col_m),(Bg_col->g)*(*bg_col_m),(Bg_col->b)*(*bg_col_m)),*bgstartColor);


		for (i=0; i<NB_MENU_ELMTS; i++) {
			if (curpos == i) {
				oslIntraFontSetStyle(f, 1.25f,RGBA(alpha,alpha,alpha,255), RGBA((255-alpha)/3,(255-alpha)/3,(255-alpha)/3,128),INTRAFONT_ALIGN_CENTER);
				oslDrawString(menuPosX, MENUPOSY, str_menu[i]);
				oslIntraFontSetStyle(f, 1.2f,RGBA(192,192,192,255), RGBA(0,0,0,128),INTRAFONT_ALIGN_CENTER);
			}
			else {
				oslDrawString(menuPosX, MENUPOSY, str_menu[i]);
			}
		}

		oslDrawImage(menu_bg);
		oslEndDrawing();
		oslEndFrame();
		oslSyncFrame();

		if (curpos == PLAY && osl_pad.pressed.cross) {
			menu_on = false;
			launch = true;
		}
		else if (curpos == SETTINGS && osl_pad.held.cross) {
			settings(cancel, mstart, returned, bg_col_m, Bg_col, f, bgstartColor, difficulty);
			curpos = 0;
		}
		else if (curpos == CREDITS && osl_pad.held.cross) {
		}
		else if (curpos == QUIT && osl_pad.pressed.cross) {
			menu_on = false;
		}

		if (go_up)	alpha+= 5;
		else	alpha-= 5;

		if (alpha <112)	go_up = true, alpha = 112;
		else if (alpha > 255)	go_up = false, alpha = 255;
		
	}

	oslSetKeyAutorepeat(NULL,0,0);
	oslDeleteImage(menu_bg);


	return launch;
}
