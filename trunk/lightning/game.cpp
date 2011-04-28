/*
game.c
by Yosh alias Hitman_07

23/04/11

*/

#include <oslib/oslib.h>
#include "ppc.h"
#include "constants.h"
#include "game.h"
#include "tinylib.h"


int play(OSL_COLOR *bgstartColor, OSL_IMAGE *bgstart, OSL_IMAGE *start, int *alpha_, int mode)
{
	int alpha, beta = 255, hitDmg, level = 1;
	bool sublevel = false;
	OSL_COLOR bgColor = NULL;
	bgColor = RGBA(R_BG,G_BG,B_BG,255);
    OSL_SOUND *mgame = NULL;
    mgame = oslLoadSoundFileBGM("./res/mgame.bgm",OSL_FMT_STREAM);
    OSL_IMAGE *player = NULL;
    player = oslLoadImageFilePNG("./res/player.png",OSL_IN_RAM,OSL_PF_8888);
	player->x = (WIDTH - player->sizeX) / 2;
	player->y = (HEIGHT - player->sizeY) / 2;
	Result Over;
	Over.quit = false;
	
    oslPlaySound(mgame, 0);
    oslSetSoundLoop(mgame, 1);
	
	
	oslSetKeyAnalogToDPad(PAD_SENS);
	
    while(beta > 0)
    {		
		beta-=10;
		
		if (beta < 0)	beta = 0;
		
		alpha = floor(*alpha_ * beta/255);
		
        oslStartDrawing();
		oslDrawFillRect(0,0,WIDTH,HEIGHT,RGBA(0,0,0,255));
        //oslDrawImage(player);
		oslSetAlpha(OSL_FX_ALPHA, beta);
		oslDrawFillRect(0,0,WIDTH,HEIGHT,*bgstartColor);
        oslDrawImage(bgstart);
		oslSetAlpha(OSL_FX_DEFAULT, 0);
        imFadeIn(start, alpha, 0);
        oslEndDrawing();
        oslEndFrame();
		oslSyncFrame();
    }
	
	if (mode == EASY)	hitDmg = HITDMG;
	else if (mode == NORMAL)	hitDmg = HITDMG*2;
	else if (mode == HARD)	hitDmg = HITDMG*4;
	else	hitDmg = HITDMG*8;
	
	
	while (level <5 && !Over.quit) {
		startScreen(level, sublevel);
		Over = gameLevel(hitDmg, level, sublevel, bgColor, player);
		
		if (Over.life >0) {
			if (sublevel)	level ++;
			sublevel = !sublevel;
		}
		else if (!Over.quit)	overScreen(false);
	}
	
	if (level >4)	overScreen(true);
	
	
	oslStopSound(mgame);
    oslDeleteSound(mgame);
    oslDeleteImage(player);
	
	return 1;
}


Result gameLevel(int hitDmg, int level, bool sublevel, OSL_COLOR bgColor, OSL_IMAGE *player) {
	
	bool playing = true, tboltOn[4] = {false};
	int i, j, alpha = 255, alpha2 = alpha, delta[4] = {alpha}, boltOn[4][4] = {{0}}, time = 0, oldTime = 0, timeLimit;
	int nbBolt[4], maxBoltX, maxBoltY, n, hitDelay = HITUNIT;
    OSL_IMAGE *hbolt = NULL, *vbolt = NULL, *tbolt1 = NULL, *tbolt2 = NULL;
    hbolt=oslLoadImageFilePNG("./res/hbolt.png",OSL_IN_RAM,OSL_PF_8888);
    vbolt=oslLoadImageFilePNG("./res/vbolt.png",OSL_IN_RAM,OSL_PF_8888);
    tbolt1=oslLoadImageFilePNG("./res/tbolt1.png",OSL_IN_RAM,OSL_PF_8888);
    tbolt2=oslLoadImageFilePNG("./res/tbolt2.png",OSL_IN_RAM,OSL_PF_8888);
	Address boltPos[4][4], tboltPos[4];
	Result Over;
	Over.life = 100;
	Over.quit = false;
	
	
										// sets lightning bolts's locations
	for (i=LEFT;i<DOWN+1;i++)
	{
		for (j=0;j<4;j++)
		{
			if (i == UP)	boltPos[i][j].x = ADDRESS_X1, boltPos[i][j].y = -BOLTLAG;
			else if (i == DOWN)	boltPos[i][j].x = ADDRESS_X2, boltPos[i][j].y = HEIGHT - vbolt->sizeY + BOLTLAG;
			else if (i == LEFT)	boltPos[i][j].x = -BOLTLAG, boltPos[i][j].y = ADDRESS_Y1;
			else	boltPos[i][j].x = WIDTH - hbolt->sizeX + BOLTLAG, boltPos[i][j].y = ADDRESS_Y2;
		}
	}
	
	tboltPos[0].x = -TBOLTLAG;
	tboltPos[0].y = -TBOLTLAG;
	tboltPos[1].x = WIDTH-tbolt2->sizeX + TBOLTLAG;
	tboltPos[1].y = -TBOLTLAG;
	tboltPos[2].x = -TBOLTLAG;
	tboltPos[2].y = HEIGHT - tbolt2->sizeY + TBOLTLAG;
	tboltPos[3].x = WIDTH-tbolt1->sizeX + TBOLTLAG;
	tboltPos[3].y = HEIGHT - tbolt1->sizeY + TBOLTLAG;
	
	
													// sets corresponding background (darker at each level/stage) and difficulty
	if (level == 1)	{
		if (sublevel == false)	maxBoltX = MAXBOLTX/2, maxBoltY = MAXBOLTY/2, timeLimit = TIMEUNIT*(level*2-1),
								bgColor = RGBA((R_BG*(10-level*2))/8,(G_BG*(10-level*2))/8,(B_BG*(10-level*2))/8,255);
		else	maxBoltX = MAXBOLTX, maxBoltY = MAXBOLTY, timeLimit = (TIMEUNIT)*(level)*2,
								bgColor = RGBA((R_BG*(10-level*2+1))/8,(G_BG*(10-level*2+1))/8,(B_BG*(10-level*2+1))/8,255);
	}
	else if (level == 2)	{
		if (sublevel == false)	maxBoltX = MAXBOLTX/2, maxBoltY = MAXBOLTY/2, timeLimit = (TIMEUNIT)*(level*2-1),
								bgColor = RGBA((R_BG*(10-level*2))/8,(G_BG*(10-level*2))/8,(B_BG*(10-level*2))/8,255);
		else	maxBoltX = MAXBOLTX, maxBoltY = MAXBOLTY, timeLimit = (TIMEUNIT)*(level)*2,
								bgColor = RGBA((R_BG*(10-level*2+1))/8,(G_BG*(10-level*2+1))/8,(B_BG*(10-level*2+1))/8,255);
	}
	else if (level == 3)	{
		bgColor = RGBA((R_BG*(5-level))/5,(G_BG*(5-level))/5,(B_BG*(5-level))/5,255);
		if (sublevel == false)	maxBoltX = MAXBOLTX/2, maxBoltY = MAXBOLTY/2, timeLimit = (TIMEUNIT)*(level*2-1),
								bgColor = RGBA((R_BG*(10-level*2))/8,(G_BG*(10-level*2))/8,(B_BG*(10-level*2))/8,255);
		else	maxBoltX = MAXBOLTX, maxBoltY = MAXBOLTY, timeLimit = (TIMEUNIT)*(level)*2,
								bgColor = RGBA((R_BG*(10-level*2+1))/8,(G_BG*(10-level*2+1))/8,(B_BG*(10-level*2+1))/8,255);
	}
	else	{
		bgColor = RGBA((R_BG*(5-level))/5,(G_BG*(5-level))/5,(B_BG*(5-level))/5,255);
		if (sublevel == false)	maxBoltX = MAXBOLTX/2, maxBoltY = MAXBOLTY/2, timeLimit = (TIMEUNIT)*(level*2-1),
								bgColor = RGBA((R_BG*(10-level*2))/8,(G_BG*(10-level*2))/8,(B_BG*(10-level*2))/8,255);
		else	maxBoltX = MAXBOLTX, maxBoltY = MAXBOLTY, timeLimit = (TIMEUNIT)*(level)*2,
								bgColor = RGBA((R_BG*(10-level*2+1))/8,(G_BG*(10-level*2+1))/8,(B_BG*(10-level*2+1))/8,255);
	}
	
	
	oslReadKeys();
	
	while (playing) {
	
										// sets random number of bolts for each side, depending on difficulty
		n = 0;
		for (i=LEFT;i<DOWN+1;i++){
			nbBolt[i] = 0;
		}
		for (i=LEFT;i<DOWN+1;i++){
			j = rand()%4;
			if ((level == 1) && (n == 0)) {
				if (j <UP)	nbBolt[j] = maxBoltY;
				else	nbBolt[j] = maxBoltX;
				n++; }
			else if ((level == 2) && (n < 2)) {
				if (j <UP)	nbBolt[j] = maxBoltY;
				else	nbBolt[j] = maxBoltX;
				n++; }
			else if ((level == 3) && (n < 3)) {
				if (j <UP)	nbBolt[j] = maxBoltY;
				else	nbBolt[j] = maxBoltX;
				n++; }
			else if ((level == 4) && (n < 4)) {
				if (j <UP)	nbBolt[j] = maxBoltY;
				else	nbBolt[j] = maxBoltX;
				n++; }
		}
		
										// activates random bolts's appearance for each side, depending on maximum number of bolts set above for each side
		for (i=LEFT;i<DOWN+1;i++){
			n = 0;
			for (j=0;j<4;j++) {
				boltOn[i][j] = 0;
				if ((n <nbBolt[i]) && (i <UP) && (j<MAXBOLTY))	boltOn[i][j] = rand()%2;
				else if ((n <nbBolt[i]) && (i >RIGHT))	boltOn[i][j] = rand()%2;
				if (boltOn[i][j])	n++;
			}
		}
		
		for (i=0;i<4;i++) {
			tboltOn[i] = false;
		}
		
		if (boltOn[LEFT][0] && boltOn[UP][0])	boltOn[LEFT][0] = 0, boltOn[UP][0] = 0, tboltOn[0] = true;
		if (boltOn[RIGHT][0] && boltOn[UP][3])	boltOn[RIGHT][0] = 0, boltOn[UP][3] = 0, tboltOn[1] = true;
		if (boltOn[LEFT][1] && boltOn[DOWN][0])	boltOn[LEFT][1] = 0, boltOn[DOWN][0] = 0, tboltOn[2] = true;
		if (boltOn[RIGHT][1] && boltOn[DOWN][3])	boltOn[RIGHT][1] = 0, boltOn[DOWN][3] = 0, tboltOn[3] = true;
		
		
		alpha = 10;
		alpha2 = alpha;
		for (i=0;i<4;i++) {
			delta[i] = alpha;
		}
		
		while (((time - oldTime) <TIMEDELAY) && playing) {
		
			oslReadKeys();
													// moving statements
			if (osl_pad.held.left)
			{
				player->x = player->x - MOVE_STEP;
				if (player->x < 0)	player->x = 0;
			}
			
			if (osl_pad.held.right)
			{
				player->x = player->x + MOVE_STEP;
				if (player->x > WIDTH-player->sizeX)	player->x = WIDTH-player->sizeX;
			}
			
			if (osl_pad.held.up)
			{
				player->y = player->y - MOVE_STEP;
				if (player->y < 0)	player->y = 0;
			}
			
			if (osl_pad.held.down)
			{
				player->y = player->y + MOVE_STEP;
				if (player->y > HEIGHT-player->sizeY)	player->y = HEIGHT-player->sizeY;
			}
			
			if (osl_pad.pressed.start || osl_pad.pressed.circle)
			{	
				if (quitScreen())	playing = false, Over.quit = true;
			}
			
			if (!Over.quit) {
				oslStartDrawing();
				oslDrawGradientRect(0,0,WIDTH,HEIGHT,bgColor,RGB(4,33,47),RGB(255,255,255),bgColor);
				
											// draws images and checks collisions
				for (i=LEFT;i<DOWN+1;i++)
				{
					if (tboltOn[i]) {
						if (i == 0) {
							tbolt1->x = tboltPos[i].x;
							tbolt1->y = tboltPos[i].y;
							if (((time - oldTime) >hitDelay)&&(delta[i] == 255)) {
								if (isCollideCopy(player,tbolt1,player->x,player->y,0,0,tboltPos[i].x,tboltPos[i].y,0,0)) {
									Over.life-=hitDmg;
									if (Over.life <0) Over.life = 0;
								}
							}
							delta[i] = imFadeIn(tbolt1, delta[i], 10);
						}
						else if (i == 1) {
							tbolt2->x = tboltPos[i].x;
							tbolt2->y = tboltPos[i].y;
							if (((time - oldTime) >hitDelay)&&(delta[i] == 255)) {
								if (isCollideCopy(player,tbolt2,player->x,player->y,0,0,tboltPos[i].x,tboltPos[i].y,0,0)) {
									Over.life-=hitDmg;
									if (Over.life <0) Over.life = 0;
								}
							}
							delta[i] = imFadeIn(tbolt2, delta[i], 10);
						}
						else if (i == 2) {
							tbolt2->x = tboltPos[i].x;
							tbolt2->y = tboltPos[i].y;
							if (((time - oldTime) >hitDelay)&&(delta[i] == 255)) {
								if (isCollideCopy(player,tbolt2,player->x,player->y,0,0,tboltPos[i].x,tboltPos[i].y,0,0)) {
									Over.life-=hitDmg;
									if (Over.life <0) Over.life = 0;
								}
							}
							delta[i] = imFadeIn(tbolt2, delta[i], 10);
						}
						else {
							tbolt1->x = tboltPos[i].x;
							tbolt1->y = tboltPos[i].y;
							if (((time - oldTime) >hitDelay)&&(delta[i] == 255)) {
								if (isCollideCopy(player,tbolt1,player->x,player->y,0,0,tboltPos[i].x,tboltPos[i].y,0,0)) {
									Over.life-=hitDmg;
									if (Over.life <0) Over.life = 0;
								}
							}
							delta[i] = imFadeIn(tbolt1, delta[i], 10);
						}
					}
					
					for (j=0;j<4;j++)
					{
						if ((i >RIGHT) && boltOn[i][j]) {
							vbolt->x = boltPos[i][j].x;
							vbolt->y = boltPos[i][j].y;
							if (((time - oldTime) >hitDelay)&&(alpha == 255)) {
								if (isCollideCopy(player,vbolt,player->x,player->y,0,0,boltPos[i][j].x,boltPos[i][j].y,0,0)) {
									Over.life-=hitDmg;
									if (Over.life <0) Over.life = 0;
								}
							}
							alpha = imFadeIn(vbolt, alpha, 10);
						}
						else if (boltOn[i][j]) {
							hbolt->x = boltPos[i][j].x;
							hbolt->y = boltPos[i][j].y;
							if (((time - oldTime) >hitDelay)&&(alpha2 == 255)) {
								if (isCollideCopy(player,hbolt,player->x,player->y,0,0,boltPos[i][j].x,boltPos[i][j].y,0,0)) {
									Over.life-=hitDmg;
									if (Over.life <0) Over.life = 0;
								}
							}
							alpha2 = imFadeIn(hbolt, alpha2, 10);
						}
					}
				}
				
				
				if (Over.life == 0)
				{
					playing = false;
					Over.quit = false;
				}
				/*if (osl_pad.pressed.L)
				{
					playing = false;
				}*/
				
				
				time++;
				
				if (time >timeLimit)	playing = false;
				
				oslSetAlpha(OSL_FX_TINT, RGBA(253+((2*(100-Over.life))/100), 255-((255*(100-Over.life))/100), 117-((117*(100-Over.life))/100), 255));
				oslDrawImage(player);
				oslSetAlpha(OSL_FX_DEFAULT, 0);
				oslEndDrawing();
				oslEndFrame();
				oslSyncFrame();
			}
		}
		
		oldTime = time;
	}
	
    oslDeleteImage(tbolt2);
    oslDeleteImage(tbolt1);
    oslDeleteImage(vbolt);
    oslDeleteImage(hbolt);
	return Over;
}