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

bool startScreen(int level, bool sublevel) {
	
	int stage = 1, alpha = 0;
	char levelDisp[10], stageDisp[10];
	OSL_FONT *f = oslLoadFontFile("flash0:/font/ltn0.pgf");
	Address levelPos, stagePos;
	levelPos.x = (WIDTH - 92)/2;
	levelPos.y = (HEIGHT - 17)/2;
	stagePos.x = (WIDTH - 58)/2;
	stagePos.y = (HEIGHT - 13)/2 + 6 + 17;
	if (sublevel)	stage = 2;
	
    sprintf(levelDisp, "Level %d", level);
    sprintf(stageDisp, "Stage %d", stage);
	
	oslReadKeys();
	while (!(osl_pad.pressed.start || osl_pad.pressed.cross)) {
		
		oslReadKeys();
		oslStartDrawing();
		oslDrawFillRect(0,0,480,272,RGBA(0,0,0,255));
		oslSetAlpha(OSL_FX_ALPHA, alpha);
		
		oslIntraFontSetStyle(f, 1.f,RGBA(255,255,255,255), RGBA(0,0,0,0),NULL);
		oslSetFont(f);
        oslDrawString(levelPos.x, levelPos.y, levelDisp);
		
		oslIntraFontSetStyle(f, 0.6f,RGBA(255,255,255,255), RGBA(0,0,0,0),NULL);
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
		oslIntraFontSetStyle(f, 1.f,RGBA(255,255,255,255), RGBA(0,0,0,0),NULL);
		oslSetFont(f);
        oslDrawString(levelPos.x, levelPos.y, levelDisp);
		
		oslIntraFontSetStyle(f, 0.6f,RGBA(255,255,255,255), RGBA(0,0,0,0),NULL);
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
	
    oslDeleteFont(f);
	
	
	return true;
}

bool overScreen(bool over) {
	
	int alpha = 0;
	OSL_FONT *f = oslLoadFontFile("flash0:/font/ltn0.pgf");
	Address loosePos, winPos;
	loosePos.x = (WIDTH - 284)/2+2;
	loosePos.y = (HEIGHT - 11)/2;
	winPos.x = (WIDTH - 339)/2+7;
	winPos.y = (HEIGHT - 11)/2;
	
	oslIntraFontSetStyle(f, 0.6f,RGBA(255,255,255,255), RGBA(0,0,0,0),NULL);
	oslSetFont(f);
	
	oslReadKeys();
	while (!(osl_pad.pressed.start || osl_pad.pressed.cross)) {
		
		oslReadKeys();
		oslStartDrawing();
		oslSetAlpha(OSL_FX_ALPHA, alpha);
		oslDrawFillRect(0,0,480,272,RGBA(0,0,0,255));
        if (over)	oslDrawString(winPos.x, winPos.y, "Congratulations, you just made it through the last level !");
        else	oslDrawString(loosePos.x, loosePos.y, "You're not good enough to go to the next level.");
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
        if (over)	oslDrawString(winPos.x, winPos.y, "Congratulations, you just made it through the last level !");
        else	oslDrawString(loosePos.x, loosePos.y, "You're not good enough to go to the next level.");
		oslSetAlpha(OSL_FX_ALPHA, alpha);
		oslDrawFillRect(0,0,480,272,RGBA(0,0,0,255));
		oslSetAlpha(OSL_FX_DEFAULT, 0);
		oslEndDrawing();
		oslEndFrame();
		oslSyncFrame();
		alpha+= 10;
	}
	
    oslDeleteFont(f);
	
	
	return true;
}

bool quitScreen() {
	
	bool quit = false, end = false;
	int frame = 0, frames = 13;
	OSL_FONT *f = oslLoadFontFile("flash0:/font/ltn0.pgf");
	Address quitPos0, quitPos1, quitPos2;
	quitPos0.x = (WIDTH - 118)/2;
	quitPos0.y = (HEIGHT - 18)/2;
	quitPos1.x = (WIDTH - 56)/2;
	quitPos1.y = (HEIGHT - 11)/2 + 18 + 6;
	quitPos2.x = (WIDTH - 56)/2;
	quitPos2.y = (HEIGHT - 11)/2 + 18 + 6 + 11 + 10;
	
	
	oslReadKeys();
	
	while (frame <frames) {
		oslStartDrawing();
		oslSetAlpha(OSL_FX_ALPHA, 64);
		oslDrawFillRect(0,0,480,272,RGBA(0,0,0,255));
		oslIntraFontSetStyle(f, 0.8f,RGBA(255,255,255,255), RGBA(0,0,0,0),NULL);
		oslSetFont(f);
		oslDrawString(quitPos0.x, quitPos0.y, "Quit Game ?");
		oslIntraFontSetStyle(f, 0.6f,RGBA(255,255,255,255), RGBA(0,0,0,0),NULL);
		oslSetFont(f);
		oslDrawString(quitPos1.x, quitPos1.y, "X: Yes.");
		oslDrawString(quitPos2.x, quitPos2.y, "O: No.");
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
		oslSyncFrame();
		
	}
	
    oslDeleteFont(f);
	
	
	return quit;
}
