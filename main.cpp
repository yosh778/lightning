/*
main.c
by Yosh alias Hitman_07

23/04/11

*/

#include <oslib/oslib.h>
#include "constants.h"
#include "tinylib.h"
#include "menu.h"
#include "game.h"

PSP_MODULE_INFO("Lightning", 0, 1, 1);
PSP_MAIN_THREAD_ATTR(THREAD_ATTR_USER | THREAD_ATTR_VFPU);
PSP_HEAP_SIZE_MAX();

int main ()
{
	bool alphaBool = true;
	int alpha = 0, difficulty = NORMAL;
	OSL_COLOR bgstartColor = NULL;

	oslInit(0);

	oslInitGfx(OSL_PF_8888, 1);

	oslInitAudio();

	oslIntraFontInit(INTRAFONT_CACHE_MED);

	splashScreen("./res/genesis.jpg", 60*3, 6, RGBA(0,0,0,255));

	OSL_IMAGE *bgstart = NULL, *start = NULL;
	OSL_SOUND *mstart = NULL;

	mstart = oslLoadSoundFileBGM("./res/mstart.bgm",OSL_FMT_STREAM);
	oslPlaySound(mstart, 0);
	oslSetSoundLoop(mstart, 1);

	bgstartColor = RGBA(10,5,39,255);

	bgstart=oslLoadImageFilePNG("./res/bgstart.png",OSL_IN_RAM,OSL_PF_8888);
	bgstart->x=0;
	bgstart->y=0;

	start=oslLoadImageFilePNG("./res/start.png",OSL_IN_RAM,OSL_PF_8888);
	start->x= 70;
	start->y= 218 - start->sizeY/2;


	oslSetKeyAnalogToDPad(PAD_SENS);

	while(!osl_quit)
	{
		oslReadKeys();

		if(osl_pad.pressed.cross || osl_pad.pressed.start)
		{
			oslStopSound(mstart);
			if (menu(&bgstartColor, bgstart,start,&alpha, &difficulty))	play(difficulty);
			oslPlaySound(mstart, 0);
			oslSetSoundLoop(mstart, 1);
			alpha = 255;
			alphaBool = false;
		}


		if (alpha < 24)	alpha = 24, alphaBool = true;
		else if (alpha == 255)	alphaBool = false;

		oslStartDrawing();
		oslDrawFillRect(0,0,480,272,bgstartColor);
		oslDrawImage(bgstart);
		oslSetAlpha(OSL_FX_ALPHA, alpha);
		oslDrawImage(start);
		oslSetAlpha(OSL_FX_DEFAULT, 0);
		if (alphaBool)	alpha = imFadeIn(start, alpha, +6);
		else if (!alphaBool)	alpha = imFadeIn(start, alpha, -6);
		oslEndDrawing();
		oslEndFrame();
		oslSyncFrame();
	}

	oslDeleteSound(mstart);
	oslDeleteImage(bgstart);
	oslDeleteImage(start);
	oslIntraFontShutdown();
	oslDeinitAudio();
	oslEndGfx();
	oslQuit();

	return 1;
}

