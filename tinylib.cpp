/*
tinylib.c
by Yosh alias Hitman_07

25/04/11

*/

#include <oslib/oslib.h>
#include "constants.h"
#include "tinylib.h"


int imFadeIn(OSL_IMAGE *image, int alpha, int decay) {
	oslSetAlpha(OSL_FX_ALPHA, alpha);
	oslDrawImage(image);
	oslSetAlpha(OSL_FX_DEFAULT, 0);
	int result = alpha + decay;
	if (result >255)	return 255;
	else if (result< 0)	return 0;
	else return	result;
}

bool splashScreen(char *location, int timeLimit, int decay, OSL_COLOR bgColor) {

	int alpha = 0, time = 0;

	OSL_IMAGE *splashscreen = NULL;
	splashscreen=oslLoadImageFile(location,OSL_IN_RAM,OSL_PF_8888);
	splashscreen->x=0;
	splashscreen->y=0;

	while ((time <timeLimit-255/decay) && !osl_quit) {
		time++;
		oslStartDrawing();
		oslDrawFillRect(0,0,480,272,bgColor);
		alpha = imFadeIn(splashscreen, alpha, decay);
		oslEndDrawing();
		oslEndFrame();
		oslSyncFrame();
	}

	decay*= -1;

	while ((alpha >0) && !osl_quit) {
		oslStartDrawing();
		oslDrawFillRect(0,0,480,272,bgColor);
		alpha = imFadeIn(splashscreen, alpha, decay);
		oslEndDrawing();
		oslEndFrame();
		oslSyncFrame();
	}

	oslDeleteImage(splashscreen);


	return true;
}

bool startScreen(OSL_FONT *f, int level, bool sublevel) {

	int stage = 1, alpha = 0, levelHeight;
	char levelDisp[8], stageDisp[8];
	Address levelPos, stagePos;
	if (sublevel)	stage = 2;

	sprintf(levelDisp, "Level %d", level);
	sprintf(stageDisp, "Stage %d", stage);

	oslIntraFontSetStyle(f, 1.f,RGBA(255,255,255,alpha), RGBA(0,0,0,alpha),INTRAFONT_ALIGN_CENTER);
	oslSetFont(f);
	levelPos.x = (WIDTH)/2;
	levelPos.y = (HEIGHT)/2;
	levelHeight = osl_curFont->charHeight;

	oslIntraFontSetStyle(f, 0.6f,RGBA(255,255,255,alpha), RGBA(0,0,0,alpha),INTRAFONT_ALIGN_CENTER);
	stagePos.x = (WIDTH)/2;
	stagePos.y = (HEIGHT)/2 + levelHeight;

	oslReadKeys();
	while (!(osl_pad.pressed.start || osl_pad.pressed.cross) && !osl_quit) {

		oslReadKeys();
		oslStartDrawing();
		oslDrawFillRect(0,0,480,272,RGBA(0,0,0,255));
		oslSetAlpha(OSL_FX_ALPHA, alpha);

		oslIntraFontSetStyle(f, 1.f,RGBA(255,255,255,alpha), RGBA(0,0,0,alpha),INTRAFONT_ALIGN_CENTER);
		oslSetFont(f);
		oslDrawString(levelPos.x, levelPos.y, levelDisp);

		oslIntraFontSetStyle(f, 0.6f,RGBA(255,255,255,alpha), RGBA(0,0,0,alpha),INTRAFONT_ALIGN_CENTER);
		oslSetFont(f);
		oslDrawString(stagePos.x, stagePos.y, stageDisp);
		oslSetAlpha(OSL_FX_ALPHA, 255-alpha);
		oslDrawFillRect(0,0,480,272,RGBA(0,0,0,255));
		oslSetAlpha(OSL_FX_DEFAULT, 0);

		oslEndDrawing();
		oslEndFrame();
		oslSyncFrame();
		alpha+= 10;
		if (alpha >255)	alpha = 255;
	}

	alpha = 66;

	while ((alpha <255) && !osl_quit) {

		oslStartDrawing();
		oslIntraFontSetStyle(f, 1.f,RGBA(255,255,255,alpha), RGBA(0,0,0,alpha),INTRAFONT_ALIGN_CENTER);
		oslSetFont(f);
		oslDrawString(levelPos.x, levelPos.y, levelDisp);

		oslIntraFontSetStyle(f, 0.6f,RGBA(255,255,255,alpha), RGBA(0,0,0,alpha),INTRAFONT_ALIGN_CENTER);
		oslSetFont(f);
		oslDrawString(stagePos.x, stagePos.y, stageDisp);
		oslSetAlpha(OSL_FX_ALPHA, alpha);
		oslDrawFillRect(0,0,480,272,RGBA(0,0,0,255));
		oslSetAlpha(OSL_FX_DEFAULT, 0);
		oslEndDrawing();
		oslEndFrame();
		oslSyncFrame();
		alpha+= 10;
	}

	return true;
}

bool overScreen(OSL_SOUND *congrats, OSL_FONT *f, bool over) {

	bool played = false;
	int alpha = 0, Height;

	oslIntraFontSetStyle(f, 0.6f,RGBA(255,255,255,alpha), RGBA(0,0,0,alpha),INTRAFONT_ALIGN_CENTER);
	oslSetFont(f);
	Height = osl_curFont->charHeight;

	oslReadKeys();
	while (!(osl_pad.pressed.start || osl_pad.pressed.cross) && !osl_quit) {

		oslReadKeys();
		oslStartDrawing();
		oslSetAlpha(OSL_FX_ALPHA, alpha);
		oslDrawFillRect(0,0,480,272,RGBA(0,0,0,255));
		oslIntraFontSetStyle(f, 0.6f,RGBA(255,255,255,alpha), RGBA(0,0,0,alpha),INTRAFONT_ALIGN_CENTER);
		if (over)	oslDrawString(WIDTH/2, (HEIGHT - Height/2)/2, "You won, congratulations !"/*"Congratulations, you just made it through the last level !"*/);
		else	oslDrawString(WIDTH/2, (HEIGHT - Height/2)/2, "You're not good enough to go to the next level.");
		oslSetAlpha(OSL_FX_DEFAULT, 0);
		oslEndDrawing();
		oslEndFrame();
		oslSyncFrame();
		alpha+= 10;
		if (alpha >255) {
			alpha = 255;
		}
		if (alpha >32) {
			if (over && !played)	oslPlaySound(congrats, 7), played = true;
		}
	}

	alpha = 66;

	while ((alpha <255) && !osl_quit) {

		oslStartDrawing();
		oslIntraFontSetStyle(f, 0.6f,RGBA(255,255,255,alpha), RGBA(0,0,0,alpha),INTRAFONT_ALIGN_CENTER);
		if (over)	oslDrawString(WIDTH/2, (HEIGHT - Height/2)/2, "You won, congratulations !"/*"Congratulations, you just made it through the last level !"*/);
		else	oslDrawString(WIDTH/2, (HEIGHT - Height/2)/2, "You're not good enough to go to the next level.");
		oslSetAlpha(OSL_FX_ALPHA, alpha);
		oslDrawFillRect(0,0,480,272,RGBA(0,0,0,255));
		oslSetAlpha(OSL_FX_DEFAULT, 0);
		oslEndDrawing();
		oslEndFrame();
		oslSyncFrame();
		alpha+= 10;
	}

	return true;
}

bool quitScreen(OSL_FONT *f) {

	bool quit = false, end = false;
	int frame = 0, height[2];
	char* str_quit[3];
	str_quit[0] = "Quit Game ?", str_quit[1] = "X : Yes.", str_quit[2] = "O : No.";

	oslIntraFontSetStyle(f, 0.8f,RGBA(255,255,255,64), RGBA(0,0,0,64),INTRAFONT_ALIGN_CENTER);
	oslSetFont(f);
	height[0] = osl_curFont->charHeight;
	oslIntraFontSetStyle(f, 0.6f,RGBA(255,255,255,64), RGBA(0,0,0,64),INTRAFONT_ALIGN_LEFT);
	oslSetFont(f);
	height[1] = osl_curFont->charHeight;


	oslReadKeys();
	while (!(end || osl_quit) && !osl_quit) {
		oslReadKeys();

		if (osl_pad.pressed.cross)	quit = true, end= true;
		else if (osl_pad.pressed.square || osl_pad.pressed.circle || osl_pad.pressed.start)	quit = false, end= true;

		oslStartDrawing();
		oslSetAlpha(OSL_FX_ALPHA, frame);
		oslDrawFillRect(0,0,480,272,RGBA(0,0,0,255));
		oslIntraFontSetStyle(f, 1.f,RGBA(255,255,255,frame), RGBA(0,0,0,frame),INTRAFONT_ALIGN_CENTER);
		oslSetFont(f);
		oslDrawString(WIDTH/2, HEIGHT/2 - 10, str_quit[0]);
		oslIntraFontSetStyle(f, 0.8f,RGBA(255,255,255,frame), RGBA(0,0,0,frame),INTRAFONT_ALIGN_CENTER);
		oslSetFont(f);
		oslDrawString(WIDTH/2, HEIGHT/2 + height[0], str_quit[1]);
		oslDrawString(WIDTH/2, HEIGHT/2 + height[0] + height[1], str_quit[2]);
		oslSetAlpha(OSL_FX_DEFAULT, 0);
		oslEndDrawing();
		oslEndFrame();
		oslSyncFrame();

		frame+=12;
		if (frame >255)	frame = 255;

	}

	return quit;
}

void settings(OSL_SOUND *won, OSL_SOUND *fx, OSL_SOUND *mstart, bool *returned, int *bg_col_m, Color *Bg_col, OSL_FONT *f, OSL_COLOR *bgstartColor, int *difficulty) {

	bool menu_on = true, go_up = false;
	int alpha, /*beta = 0, */i, strHeight, modeHeight, settPosX = WIDTH/2, set_mode = false, titleHeight, menuPosX, curpos = 1;
	char *str_menu[6], *str_mode[4], *mod_color = "<-Change Skin->";
	str_menu[0] = "Settings", str_menu[1] = "Difficulty", str_menu[2] = "Change Skin", str_menu[3] = "Load", str_menu[4] = "Save", str_menu[5] = "Help";
	str_mode[0] = "Easy", str_mode[1] = "Normal", str_mode[2] = "Hard", str_mode[3] = "God";

	menuPosX = WIDTH/2;

	oslIntraFontSetStyle(f, 1.25f,RGBA(255,255,255,255), RGBA(0,0,0,255),INTRAFONT_ALIGN_CENTER);
	oslSetFont(f);
	titleHeight = osl_curFont->charHeight;
	oslIntraFontSetStyle(f, 0.9f,RGBA(255,255,255,255), RGBA(0,0,0,255),INTRAFONT_ALIGN_CENTER);
	oslSetFont(f);
	strHeight = osl_curFont->charHeight;
	oslIntraFontSetStyle(f, 0.85f,RGBA(192,192,192,255), RGBA(0,0,0,128),INTRAFONT_ALIGN_CENTER);
	oslSetFont(f);
	modeHeight = osl_curFont->charHeight;



	/*
		while(beta < 255)
		{
			beta+=10;

			if (beta < 0)	beta = 0;

			oslStartDrawing();
			oslSetAlpha(OSL_FX_ALPHA, beta);
			//oslDrawFillRect(0,0,480,272,*bgstartColor);
			oslDrawGradientRect(0,0,WIDTH,HEIGHT,*bgstartColor,*bgstartColor,RGB((Bg_col->r)*(*bg_col_m),(Bg_col->g)*(*bg_col_m),(Bg_col->b)*(*bg_col_m)),*bgstartColor);
			oslSetAlpha(OSL_FX_DEFAULT, 0);
			oslEndDrawing();
			oslEndFrame();
			oslSyncFrame();
		}
	*/

	alpha = 255;

	while (menu_on && !osl_quit) {

		oslReadKeys();


		strtsnd_if(fx, mstart, returned);

		if (set_mode) {
			if (osl_pad.pressed.up)
			{
				if(oslGetSoundChannel(fx) != -1)	oslStopSound(fx);
				oslPlaySound(fx, 2);
				(*difficulty)--;
				if ((*difficulty) < 0)	(*difficulty) = 3;
			}

			if (osl_pad.pressed.down)
			{
				if(oslGetSoundChannel(fx) != -1)	oslStopSound(fx);
				oslPlaySound(fx, 2);
				(*difficulty)++;
				if ((*difficulty) > 3)	(*difficulty) = 0;
			}
		}

		else {
			if (osl_pad.pressed.up)
			{
				if(oslGetSoundChannel(fx) != -1)	oslStopSound(fx);
				oslPlaySound(fx, 2);
				curpos--;
				if (curpos < 1)	curpos = 5;
			}

			if (osl_pad.pressed.down)
			{
				if(oslGetSoundChannel(fx) != -1)	oslStopSound(fx);
				oslPlaySound(fx, 2);
				curpos++;
				if (curpos >5)	curpos = 1;
			}
		}




		oslStartDrawing();
		//oslDrawFillRect(0,0,480,272,*bgstartColor);
		oslDrawGradientRect(0,0,WIDTH,HEIGHT,*bgstartColor,*bgstartColor,RGB((Bg_col->r)*(*bg_col_m),(Bg_col->g)*(*bg_col_m),(Bg_col->b)*(*bg_col_m)),*bgstartColor);


		if (!set_mode) {
			i = 0;
			oslIntraFontSetStyle(f, 1.25f,RGBA(224,224,224,255), RGBA(0,0,0,160),INTRAFONT_ALIGN_CENTER);
			oslDrawString(settPosX, BORDER_SETT + 10, str_menu[0]);
			for (i=1; i<6; i++) {
				if ((curpos == i)&& (curpos != SET_COL+1)) {
					oslIntraFontSetStyle(f, 1.f,RGBA(alpha,alpha,alpha,255), RGBA((255-alpha)/3,(255-alpha)/3,(255-alpha)/3,128),INTRAFONT_ALIGN_CENTER);
					oslDrawString(settPosX, SETTPOSY, str_menu[i]);
				}
				else if ((i == SET_COL+1)&& (curpos == SET_COL+1)) {
					oslIntraFontSetStyle(f, 0.9f,RGBA(alpha,alpha,alpha,255), RGBA((255-alpha)/3,(255-alpha)/3,(255-alpha)/3,128),INTRAFONT_ALIGN_CENTER);
					oslDrawString(settPosX, SETTPOSY, mod_color);
				}
				else {
					oslIntraFontSetStyle(f, 0.85f,RGBA(192,192,192,255), RGBA(0,0,0,128),INTRAFONT_ALIGN_CENTER);
					oslDrawString(settPosX, SETTPOSY, str_menu[i]);
				}
			}
		}

		else {
			for (i=EASY; i<GOD+1; i++) {
				if (*difficulty == i) {
					oslIntraFontSetStyle(f, 0.95f,RGBA(alpha,alpha,alpha,255), RGBA((255-alpha)/3,(255-alpha)/3,(255-alpha)/3,128),INTRAFONT_ALIGN_CENTER);
					oslDrawString(menuPosX, MENUPOSY, str_mode[i]);
				}
				else {
					oslIntraFontSetStyle(f, 0.9f,RGBA(192,192,192,255), RGBA(0,0,0,128),INTRAFONT_ALIGN_CENTER);
					oslDrawString(menuPosX, MENUPOSY, str_mode[i]);
					//oslDrawString(SETTPOSX, settPosY, str_mode[i]);
				}
			}
		}

		oslEndDrawing();
		oslEndFrame();
		oslSyncFrame();


		if (!set_mode && osl_pad.pressed.circle) {
			menu_on = false;
		}
		if (!set_mode && curpos == SET_MODE+1 && osl_pad.pressed.cross) {
			set_mode = true;
		}
		else if (set_mode && (osl_pad.pressed.circle || osl_pad.pressed.cross)) {
			set_mode = false;
		}
		else if (curpos == SET_COL+1) {
			if (osl_pad.pressed.left)
			{
				(*bg_col_m)--;
				if (*bg_col_m <1)	(*bg_col_m) = 1;
				else {
					if(oslGetSoundChannel(fx) != -1)	oslStopSound(fx);
					oslPlaySound(fx, 2);
				}
				*bgstartColor = RGBA(6*(Bg_col->r)*(*bg_col_m),6*(Bg_col->g)*(*bg_col_m),6*(Bg_col->b)*(*bg_col_m),255);
			}
			else if (osl_pad.pressed.right)
			{
				(*bg_col_m)++;
				if (*bg_col_m >100)	(*bg_col_m) = 100;
				else {
					if(oslGetSoundChannel(fx) != -1)	oslStopSound(fx);
					oslPlaySound(fx, 2);
				}
			}
			*bgstartColor = RGBA(6*(Bg_col->r)*(*bg_col_m),6*(Bg_col->g)*(*bg_col_m),6*(Bg_col->b)*(*bg_col_m),255);
		}
		else if (curpos == SET_LOAD+1 && osl_pad.pressed.cross) {
			oslPlaySound(won, 6);
			if (load(bg_col_m, difficulty))	*bgstartColor = RGBA(6*(Bg_col->r)*(*bg_col_m),6*(Bg_col->g)*(*bg_col_m),6*(Bg_col->b)*(*bg_col_m),255), sv_info(f, bg_col_m, Bg_col, false, true, bgstartColor);
			else	sv_info(f, bg_col_m, Bg_col, false, false, bgstartColor);
		}
		else if (curpos == SET_SAVE+1 && osl_pad.pressed.cross) {
			oslPlaySound(won, 6);
			if (save(bg_col_m, difficulty)) sv_info(f, bg_col_m, Bg_col, true, true, bgstartColor);
			else	sv_info(f, bg_col_m, Bg_col, true, false, bgstartColor);
		}
		else if (curpos == SET_HELP+1 && osl_pad.pressed.cross) {
			help(f, bg_col_m, Bg_col, bgstartColor);
		}

		if (go_up)	alpha+= 5;
		else	alpha-= 5;

		if (alpha <112)	go_up = true, alpha = 112;
		else if (alpha > 255)	go_up = false, alpha = 255;

	}

}


bool load(int *bg_col_m, int *difficulty)
{
	FILE* file = NULL;
	char fileline[6];

	file = fopen("res/lightning", "r");

	if (file == NULL)
		return false;

	if (fgets(fileline, 3, file) == NULL)
		return false;

	
	//*difficulty = fgetc(file);	// alternative
	//*bg_col_m = fgetc(file);
	*difficulty = fileline[0];
	*bg_col_m = fileline[1];
	
	
	fclose(file);
	return true;
}

bool save(int *bg_col_m, int *difficulty)
{
	FILE* file = NULL;

	file = fopen("res/lightning", "w+");
	if (file == NULL)
		return false;

	fputc(*difficulty,file);
	fputc(*bg_col_m,file);


	fclose(file);
	return true;
}


bool strtsnd_if(OSL_SOUND *to_wait, OSL_SOUND *to_play, bool *returned) {

	if (*returned)	if(oslGetSoundChannel(to_wait) == -1)	oslPlaySound(to_play, 0), oslSetSoundLoop(to_play, 1), *returned = false;;
	return returned;
}

void sv_info(OSL_FONT *f, int *bg_col_m, Color *Bg_col, bool save, bool success, OSL_COLOR *bgstartColor) {

	int Height;

	oslIntraFontSetStyle(f, 0.8f,RGBA(255,255,255,255), RGBA(0,0,0,255),INTRAFONT_ALIGN_CENTER);
	oslSetFont(f);
	Height = osl_curFont->charHeight;

	oslReadKeys();
	while (!(osl_pad.pressed.start || osl_pad.pressed.cross || osl_pad.pressed.circle) && !osl_quit) {

		oslReadKeys();
		oslStartDrawing();
		oslDrawGradientRect(0,0,WIDTH,HEIGHT,*bgstartColor,*bgstartColor,RGB((Bg_col->r)*(*bg_col_m),(Bg_col->g)*(*bg_col_m),(Bg_col->b)*(*bg_col_m)),*bgstartColor);
		oslIntraFontSetStyle(f, 0.8f,RGBA(255,255,255,255), RGBA(0,0,0,255),INTRAFONT_ALIGN_CENTER);
		if (save) {
			if (success)	oslDrawString(WIDTH/2, (HEIGHT - Height/2)/2, "Save completed");
			else	oslDrawString(WIDTH/2, (HEIGHT - Height/2)/2, "Save failed");
		}
		else {
			if (success)	oslDrawString(WIDTH/2, (HEIGHT - Height/2)/2, "Load completed");
			else	oslDrawString(WIDTH/2, (HEIGHT - Height/2)/2, "Load failed");
		}
		oslEndDrawing();
		oslEndFrame();
		oslSyncFrame();
	}
}

void credits(OSL_FONT *f, int *bg_col_m, Color *Bg_col, OSL_COLOR *bgstartColor) {

	int Height0, Height1, Height2;

	oslIntraFontSetStyle(f, 1.25f,RGBA(224,224,224,255), RGBA(0,0,0,160),INTRAFONT_ALIGN_CENTER);
	oslSetFont(f);
	Height0 = osl_curFont->charHeight;
	oslIntraFontSetStyle(f, 1.f,RGBA(192,192,192,255), RGBA(0,0,0,160),INTRAFONT_ALIGN_CENTER);
	oslSetFont(f);
	Height1 = osl_curFont->charHeight;
	oslIntraFontSetStyle(f, 0.8f,RGBA(192,192,192,255), RGBA(0,0,0,160),INTRAFONT_ALIGN_CENTER);
	oslSetFont(f);
	Height2 = osl_curFont->charHeight;

	oslReadKeys();
	while (!(osl_pad.pressed.start || osl_pad.pressed.cross || osl_pad.pressed.circle) && !osl_quit) {

		oslReadKeys();
		oslStartDrawing();
		oslDrawGradientRect(0,0,WIDTH,HEIGHT,*bgstartColor,*bgstartColor,RGB((Bg_col->r)*(*bg_col_m),(Bg_col->g)*(*bg_col_m),(Bg_col->b)*(*bg_col_m)),*bgstartColor);
		oslIntraFontSetStyle(f, 1.25f,RGBA(224,224,224,255), RGBA(0,0,0,160),INTRAFONT_ALIGN_CENTER);
		oslDrawString(WIDTH/2, BORDER_SETT + 10, "Credits");
		oslIntraFontSetStyle(f, 1.f,RGBA(208,208,208,255), RGBA(0,0,0,160),INTRAFONT_ALIGN_CENTER);
		oslDrawString(WIDTH/2, BORDER_SETT + 18 + Height0 + 16, "Special thanks to :");
		oslIntraFontSetStyle(f, 0.8f,RGBA(192,192,192,255), RGBA(0,0,0,160),INTRAFONT_ALIGN_CENTER);
		oslDrawString(WIDTH/2, BORDER_SETT + 18 + Height0 + 14 + 4 + Height1 + 8, "BenHur for the intraFont, Brunni for his OSlib");
		oslDrawString(WIDTH/2, BORDER_SETT + 18 + Height0 + 14 + 4 + Height1 + (Height2 + 9), "Jakex09 for the current theme,  ' Survival '");
		oslDrawString(WIDTH/2, BORDER_SETT + 18 + Height0 + 14 + 4 + Height1 + (Height2 + 9)*2 - 8, "Radman for the game theme,  ' Thunderstorm '");
		oslIntraFontSetStyle(f, 0.8f,RGBA(255,255,255,255), RGBA(0,0,0,255),INTRAFONT_ALIGN_CENTER);
		oslDrawString(WIDTH/2, HEIGHT-HEIGHT/5-2, "by Yosh, alias Hitman 07");
		oslEndDrawing();
		oslEndFrame();
		oslSyncFrame();
	}
}

void help(OSL_FONT *f, int *bg_col_m, Color *Bg_col, OSL_COLOR *bgstartColor) {

	int Height0, Height1;

	oslIntraFontSetStyle(f, 1.25f,RGBA(224,224,224,255), RGBA(0,0,0,160),INTRAFONT_ALIGN_CENTER);
	oslSetFont(f);
	Height0 = osl_curFont->charHeight;
	oslIntraFontSetStyle(f, 0.8f,RGBA(192,192,192,255), RGBA(0,0,0,160),INTRAFONT_ALIGN_CENTER);
	oslSetFont(f);
	Height1 = osl_curFont->charHeight;

	oslReadKeys();
	while (!(osl_pad.pressed.start || osl_pad.pressed.cross || osl_pad.pressed.circle) && !osl_quit) {

		oslReadKeys();
		oslStartDrawing();
		oslDrawGradientRect(0,0,WIDTH,HEIGHT,*bgstartColor,*bgstartColor,RGB((Bg_col->r)*(*bg_col_m),(Bg_col->g)*(*bg_col_m),(Bg_col->b)*(*bg_col_m)),*bgstartColor);
		oslIntraFontSetStyle(f, 1.25f,RGBA(224,224,224,255), RGBA(0,0,0,160),INTRAFONT_ALIGN_CENTER);
		oslDrawString(WIDTH/2, BORDER_SETT + 10, "Help");
		oslIntraFontSetStyle(f, 0.8f,RGBA(192,192,192,255), RGBA(0,0,0,160),INTRAFONT_ALIGN_LEFT);
		oslDrawString(WIDTH/16, BORDER_SETT + 10 + 36 + Height0 + 3 , "You have to escape incoming thunderbolts about");
		oslDrawString(WIDTH/16, BORDER_SETT + 10 + 36 + Height0 + 3 + (Height1 + 0) , "a half second after their appearance or you'll get");
		oslDrawString(WIDTH/16, BORDER_SETT + 10 + 36 + Height0 + 3 + (Height1 + 0)*2 , "hurt, and eventually die.");
		oslDrawString(WIDTH/16, BORDER_SETT + 10 + 36 + Height0 + 3 + (Height1 + 0)*3 , "Difficulty determines how deathly the thunder'll be.");
		oslEndDrawing();
		oslEndFrame();
		oslSyncFrame();
	}
}
