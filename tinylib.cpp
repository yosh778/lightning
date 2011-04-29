/*
tinylib.c
by Yosh alias Hitman_07

25/04/11

*/

#include <oslib/oslib.h>
#include "constants.h"


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

	while (time <timeLimit-255/decay) {
		time++;
		oslStartDrawing();
		oslDrawFillRect(0,0,480,272,bgColor);
		alpha = imFadeIn(splashscreen, alpha, decay);
		oslEndDrawing();
		oslEndFrame();
		oslSyncFrame();
	}

	decay*= -1;

	while (alpha >0) {
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
	while (!(osl_pad.pressed.start || osl_pad.pressed.cross)) {

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

	while (alpha <255) {

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

bool overScreen(OSL_FONT *f, bool over) {

	int alpha = 0;

	oslIntraFontSetStyle(f, 0.6f,RGBA(255,255,255,alpha), RGBA(0,0,0,alpha),INTRAFONT_ALIGN_CENTER);
	oslSetFont(f);

	oslReadKeys();
	while (!(osl_pad.pressed.start || osl_pad.pressed.cross)) {

		oslReadKeys();
		oslStartDrawing();
		oslSetAlpha(OSL_FX_ALPHA, alpha);
		oslDrawFillRect(0,0,480,272,RGBA(0,0,0,255));
		oslIntraFontSetStyle(f, 0.6f,RGBA(255,255,255,alpha), RGBA(0,0,0,alpha),INTRAFONT_ALIGN_CENTER);
		if (over)	oslDrawString(WIDTH/2, HEIGHT/2, "Congratulations, you just made it through the last level !");
		else	oslDrawString(WIDTH/2, HEIGHT/2, "You're not good enough to go to the next level.");
		oslSetAlpha(OSL_FX_DEFAULT, 0);
		oslEndDrawing();
		oslEndFrame();
		oslSyncFrame();
		alpha+= 10;
		if (alpha >255)	alpha = 255;
	}

	alpha = 66;

	while (alpha <255) {

		oslStartDrawing();
		oslIntraFontSetStyle(f, 0.6f,RGBA(255,255,255,alpha), RGBA(0,0,0,alpha),INTRAFONT_ALIGN_CENTER);
		if (over)	oslDrawString(WIDTH/2, HEIGHT/2, "Congratulations, you just made it through the last level !");
		else	oslDrawString(WIDTH/2, HEIGHT/2, "You're not good enough to go to the next level.");
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
	int frame = 0, frames = 13, height[2];
	char* str_quit[3]; str_quit[0] = "Quit Game ?", str_quit[1] = "X: Yes.", str_quit[2] = "O: No.";
	
	oslIntraFontSetStyle(f, 0.8f,RGBA(255,255,255,64), RGBA(0,0,0,64),INTRAFONT_ALIGN_CENTER);
	oslSetFont(f);
	height[0] = osl_curFont->charHeight;
	oslIntraFontSetStyle(f, 0.6f,RGBA(255,255,255,64), RGBA(0,0,0,64),INTRAFONT_ALIGN_LEFT);
	oslSetFont(f);
	height[1] = osl_curFont->charHeight;

	oslReadKeys();

	while (frame <frames) {
		oslStartDrawing();
		oslSetAlpha(OSL_FX_ALPHA, 64);
		oslDrawFillRect(0,0,480,272,RGBA(0,0,0,255));
		oslIntraFontSetStyle(f, 0.8f,RGBA(255,255,255,255), RGBA(0,0,0,64),INTRAFONT_ALIGN_CENTER);
		oslSetFont(f);
		oslDrawString(WIDTH/2, HEIGHT/2, str_quit[0]);
		oslIntraFontSetStyle(f, 0.6f,RGBA(255,255,255,255), RGBA(0,0,0,64),INTRAFONT_ALIGN_LEFT);
		oslSetFont(f);
		oslDrawString(WIDTH/2, HEIGHT/2 + height[0], str_quit[1]);
		oslDrawString(WIDTH/2, HEIGHT/2 + height[0] + height[1], str_quit[2]);
		oslSetAlpha(OSL_FX_DEFAULT, 0);
		oslEndDrawing();
		oslEndFrame();
		oslSyncFrame();
		frame++;
	}

	oslReadKeys();
	while (!end) {

		oslReadKeys();

		if (osl_pad.pressed.cross)	quit = true, end= true;
		else if (osl_pad.pressed.square || osl_pad.pressed.circle)	quit = false, end= true;
		oslEndFrame();

	}

	return quit;
}

