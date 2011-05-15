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
	bool alphaBool = true, returned = false;
	int alpha = 0, bg_col_m = 1, difficulty = NORMAL;
	OSL_COLOR bgstartColor = NULL;
	Color Bg_col;
	Bg_col.r = 182;
	Bg_col.g = 151;
	Bg_col.b = 191;
	
	if (load(&bg_col_m, &difficulty))	bgstartColor = RGBA(6*(Bg_col.r)*(bg_col_m),6*(Bg_col.g)*(bg_col_m),6*(Bg_col.b)*(bg_col_m),255);
	
	
	oslInit(0);

	oslInitGfx(OSL_PF_8888, 1);

	oslInitAudio();

	oslIntraFontInit(INTRAFONT_CACHE_MED);
	OSL_FONT *f = oslLoadFontFile("flash0:/font/ltn0.pgf");

	splashScreen("./res/genesis.jpg", 60*3, 6, RGBA(0,0,0,255));

	OSL_IMAGE *bgstart = NULL, *start = NULL;
	OSL_SOUND *mstart = NULL, *mgame = NULL, *congrats = NULL, *won = NULL, *appear = NULL, *critic = NULL, *hurt = NULL, *fx = NULL;

	mstart = oslLoadSoundFileBGM("./res/mstart.bgm",OSL_FMT_STREAM/*OSL_FMT_NONE*/);
	mgame = oslLoadSoundFileBGM("./res/mgame.bgm",OSL_FMT_STREAM);
	congrats = oslLoadSoundFileBGM("./res/congrats.bgm",OSL_FMT_NONE);
	won = oslLoadSoundFileBGM("./res/won.bgm",OSL_FMT_NONE);
	appear = oslLoadSoundFileBGM("./res/appear.bgm",OSL_FMT_NONE);
	critic = oslLoadSoundFileBGM("./res/critic.bgm",OSL_FMT_NONE);
	hurt = oslLoadSoundFileBGM("./res/hurt.bgm",OSL_FMT_NONE);
	fx = oslLoadSoundFileBGM("./res/fx.bgm",OSL_FMT_NONE);
	oslPlaySound(mstart, 0);
	oslSetSoundLoop(mstart, 1);

	//bgstartColor = RGBA(Bg_col.r,Bg_col.g,Bg_col.b,255);
	bgstartColor = RGBA(6*(Bg_col.r)*bg_col_m,6*(Bg_col.g)*bg_col_m,6*(Bg_col.b)*bg_col_m,255);	//RGB(68,126,125);

	bgstart=oslLoadImageFilePNG("./res/bgstart.png",OSL_IN_RAM,OSL_PF_8888);
	bgstart->x=0;
	bgstart->y=0;

	start=oslLoadImageFilePNG("./res/start.png",OSL_IN_RAM,OSL_PF_8888);
	start->x= 70;
	start->y= 218 - start->sizeY/2;


	while(!osl_quit)
	{
		oslReadKeys();

		if(osl_pad.pressed.cross || osl_pad.pressed.start)
		{
			if (menu(won, fx, mstart, &returned, &bg_col_m, &Bg_col, f, &bgstartColor, bgstart,start,&alpha, &difficulty)) {
				oslStopSound(fx);
				oslStopSound(mstart);
				play(congrats, won, appear, fx, critic, hurt, &Bg_col, &bg_col_m, mgame, f, difficulty);
				returned = true;
				//oslPlaySound(mstart, 0);
				//oslSetSoundLoop(mstart, 1);
			}
			
			alpha = 255;
			alphaBool = false;
		}
		
		if (returned)	if(oslGetSoundChannel(fx) == -1)	oslPlaySound(mstart, 0), oslSetSoundLoop(mstart, 1), returned = false;;
		

		if (alpha < 24)	alpha = 24, alphaBool = true;
		else if (alpha == 255)	alphaBool = false;

		oslStartDrawing();
		//oslDrawFillRect(0,0,480,272,bgstartColor);
		oslDrawGradientRect(0,0,WIDTH,HEIGHT,bgstartColor,bgstartColor,RGB((Bg_col.r)*bg_col_m,(Bg_col.g)*bg_col_m,(Bg_col.b)*bg_col_m),bgstartColor);
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

	oslDeleteSound(fx);
	oslDeleteSound(congrats);
	oslDeleteSound(won);
	oslDeleteSound(appear);
	oslDeleteSound(critic);
	oslDeleteSound(hurt);
	oslDeleteSound(mgame);
	oslDeleteSound(mstart);
	oslDeleteImage(bgstart);
	oslDeleteImage(start);
	oslDeleteFont(f);
	oslIntraFontShutdown();
	oslDeinitAudio();
	oslEndGfx();
	oslQuit();

	return 1;
}

