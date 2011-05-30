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


int play(OSL_SOUND *congrats, OSL_SOUND *won, OSL_SOUND *appear, OSL_SOUND *fx, OSL_SOUND *critic, OSL_SOUND *hurt, Color *Bg_col, int *bg_col_m, OSL_SOUND *mgame, OSL_FONT *f, int mode)
{
	int hitDmg, level = 1;
	bool sublevel = false, launch = true;
	OSL_COLOR bgColor = NULL;
	bgColor = RGBA(R_BG,G_BG,B_BG,255);
	OSL_IMAGE *player = NULL;
	player = oslLoadImageFilePNG("./res/player.png",OSL_IN_RAM,OSL_PF_8888);
	player->x = (WIDTH - player->sizeX) / 2;
	player->y = (HEIGHT - player->sizeY) / 2;
	Result Over;
	Over.quit = false;


	oslPlaySound(mgame, 1);
	oslSetSoundLoop(mgame, 1);


	if (mode == EASY)	hitDmg = HITDMG;
	else if (mode == NORMAL)	hitDmg = HITDMG*2;
	else if (mode == HARD)	hitDmg = HITDMG*4;
	else	hitDmg = HITDMG*8;


	while (level <5 && !Over.quit && !osl_quit) {
		startScreen(f, level, sublevel);
		if (launch)	oslPlaySound(appear, 3), launch = false;
		Over = gameLevel(fx, critic, hurt, mgame, Bg_col, bg_col_m, f, hitDmg, level, sublevel, bgColor, player);

		if (!Over.quit) {
			if (Over.life >0) {
				if (!(level == 4 && sublevel))	oslPlaySound(won, 6);
				if (sublevel)	level ++;
				sublevel = !sublevel;
			}
			else	overScreen(congrats, f, false);
		}
	}

	oslStopSound(appear);
	oslStopSound(fx);
	oslStopSound(hurt);
	oslStopSound(critic);
	oslStopSound(won);
	oslStopSound(congrats);
	oslStopSound(mgame);
	
	if (level >4)	overScreen(congrats, f, true);
	


	oslDeleteImage(player);

	return 1;
}


Result gameLevel(OSL_SOUND *fx, OSL_SOUND *critic, OSL_SOUND *hurt, OSL_SOUND *mgame, Color *Bg_col, int *bg_col_m, OSL_FONT *f, int hitDmg, int level, bool sublevel, OSL_COLOR bgColor, OSL_IMAGE *player) {

	bool playing = true, tboltOn[4] = {false}, game_quit = false, lost_game = false, won_game = false, critic_on = false;
	int i, j, k, m, height[2], alpha = 255, alpha2 = alpha, delta[4] = {alpha}, redraw = 1, boltOn[4][4] = {{0}}, time = 0, oldTime = 0, timeLimit, timeLevel;
	int nbBolt[4], maxBoltX, maxBoltY, n, hitDelay = HITUNIT, Score = 0;
	char score[20] = {NULL}, health[20] = {NULL};
	OSL_IMAGE *hbolt = NULL, *vbolt = NULL, *tbolt1 = NULL, *tbolt2 = NULL;
	hbolt=oslLoadImageFilePNG("./res/hbolt.png",OSL_IN_RAM,OSL_PF_8888);
	vbolt=oslLoadImageFilePNG("./res/vbolt.png",OSL_IN_RAM,OSL_PF_8888);
	tbolt1=oslLoadImageFilePNG("./res/tbolt1.png",OSL_IN_RAM,OSL_PF_8888);
	tbolt2=oslLoadImageFilePNG("./res/tbolt2.png",OSL_IN_RAM,OSL_PF_8888);
	Address boltPos[4][4], tboltPos[4];
	Color Progress;
	Result Over;
	Over.life = 100;
	Over.quit = false;

	oslIntraFontSetStyle(f, 0.6f,RGBA(255,255,255,255), RGBA(0,0,0,224),INTRAFONT_ALIGN_LEFT);
	oslSetFont(f);
	height[0] = osl_curFont->charHeight;
	oslIntraFontSetStyle(f, 0.7f,RGBA(255,255,255,255), RGBA(0,0,0,224),INTRAFONT_ALIGN_CENTER);
	oslSetFont(f);
	height[1] = osl_curFont->charHeight;
	//width = oslGetStringWidth(text);
	
	
		// initial score calculation
	for (i=1;i<(level-1)*2;i++) {
		Score+= 10*(TIMEUNIT/60)*(i*2-1);
		Score+= 10*(TIMEUNIT/60)*(i*2);
	}
	if (sublevel) {
		Score+= 10*(TIMEUNIT/60)*(level*2-1);
	}
	
	
	
	// sets lightning bolts's locations
	for (i=LEFT; i<DOWN+1; i++)
	{
		for (j=0; j<4; j++)
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
		if (sublevel == false)	maxBoltX = MAXBOLTX/2, maxBoltY = MAXBOLTY/2, timeLimit = TIMEUNIT*(level*2-1), timeLevel = (TIMEUNIT)*(level*2-1) - (TIMEUNIT)*(level - 1)*2, bgColor = RGBA((R_BG*(10-level*2-1))/8,(G_BG*(10-level*2-1))/8,(B_BG*(10-level*2-1))/8,255);
		else	maxBoltX = MAXBOLTX, maxBoltY = MAXBOLTY, timeLimit = (TIMEUNIT)*(level)*2, timeLevel = (TIMEUNIT)*(level*2) - (TIMEUNIT)*(level*2 - 1), bgColor = RGBA((R_BG*(10-level*2-2))/8,(G_BG*(10-level*2-2))/8,(B_BG*(10-level*2-2))/8,255);
	}
	else if (level == 2)	{
		if (sublevel == false)	maxBoltX = MAXBOLTX/2, maxBoltY = MAXBOLTY/2, timeLimit = (TIMEUNIT)*(level*2-1), timeLevel = (TIMEUNIT)*(level*2-1) - (TIMEUNIT)*(level - 1)*2, bgColor = RGBA((R_BG*(10-level*2-1))/8,(G_BG*(10-level*2-1))/8,(B_BG*(10-level*2-1))/8,255);
		else	hitDelay = 35, maxBoltX = MAXBOLTX, maxBoltY = MAXBOLTY, timeLimit = (TIMEUNIT)*(level)*2, timeLevel = (TIMEUNIT)*(level*2) - (TIMEUNIT)*(level*2 - 1), bgColor = RGBA((R_BG*(10-level*2-2))/8,(G_BG*(10-level*2-2))/8,(B_BG*(10-level*2-2))/8,255);
	}
	else if (level == 3)	{
		if (sublevel == false)	hitDelay = 35, maxBoltX = MAXBOLTX/2, maxBoltY = MAXBOLTY/2, timeLimit = (TIMEUNIT)*(level*2-1), timeLevel = (TIMEUNIT)*(level*2-1) - (TIMEUNIT)*(level - 1)*2, bgColor = RGBA((R_BG*(10-level*2-1))/8,(G_BG*(10-level*2-1))/8,(B_BG*(10-level*2-1))/8,255);
		else	hitDelay = 40, maxBoltX = MAXBOLTX, maxBoltY = MAXBOLTY, timeLimit = (TIMEUNIT)*(level)*2, timeLevel = (TIMEUNIT)*(level*2) - (TIMEUNIT)*(level*2 - 1), bgColor = RGBA((R_BG*(10-level*2-2))/8,(G_BG*(10-level*2-2))/8,(B_BG*(10-level*2-2))/8,255);
	}
	else	{
		if (sublevel == false)	hitDelay = 40, maxBoltX = MAXBOLTX/2, maxBoltY = MAXBOLTY/2, timeLimit = (TIMEUNIT)*(level*2-1), timeLevel = (TIMEUNIT)*(level*2-1) - (TIMEUNIT)*(level - 1)*2, bgColor = RGBA(16,16,16,255);
		else	hitDelay = 45, maxBoltX = MAXBOLTX, maxBoltY = MAXBOLTY, timeLimit = (TIMEUNIT)*(level)*2, timeLevel = (TIMEUNIT)*(level*2) - (TIMEUNIT)*(level*2 - 1), bgColor = RGBA(0,0,0,255);
	}
	
	oslRgbaGet8888(bgColor,Progress.r,Progress.g,Progress.b,n);
	

	oslReadKeys();

	while (playing && !osl_quit) {

		// sets random number of bolts for each side, depending on difficulty
		n = 0;
		for (i=LEFT; i<DOWN+1; i++) {
			nbBolt[i] = 0;
		}
		for (i=LEFT; i<DOWN+1; i++) {
			j = rand()%4;
			if ((level == 1) && (n == 0)) {
				if (j <UP)	nbBolt[j] = maxBoltY;
				else	nbBolt[j] = maxBoltX;
				n++;
			}
			else if ((level == 2) && (n < 2)) {
				if (j <UP)	nbBolt[j] = maxBoltY;
				else	nbBolt[j] = maxBoltX;
				n++;
			}
			else if ((level == 3) && (n < 3)) {
				if (j <UP)	nbBolt[j] = maxBoltY;
				else	nbBolt[j] = maxBoltX;
				n++;
			}
			else if ((level == 4) && (n < 4)) {
				if (j <UP)	nbBolt[j] = maxBoltY;
				else	nbBolt[j] = maxBoltX;
				n++;
			}
		}

		// activates random bolts's appearance for each side, depending on maximum number of bolts set above for each side
		m = 0;
		for (i=LEFT; i<DOWN+1; i++) {
			n = 0;
			for (j=0; j<4; j++) {
				boltOn[i][j] = 0;
				if ((n <nbBolt[i]) && (i <UP) && (j<MAXBOLTY))	boltOn[i][j] = rand()%2;
				else if ((n <nbBolt[i]) && (i >RIGHT))	boltOn[i][j] = rand()%2;
				if (boltOn[i][j])	n++;
			}
			m+= n;
		}

		// erases one bolt if all of them got activated
		if (m >8) {
			i = rand()%4;

			if (i<UP)	j = rand()%2;
			else {
				j = rand()%4;
				if (j == 2)	j = 0;
				else if (j == 1)	j = 3;
			}

			if (i == LEFT)	{
				if (j == 0)	boltOn[i][j] = 0, boltOn[UP][0] = 0;
				else	boltOn[i][j] = 0, boltOn[DOWN][0] = 0;
			}
			else if (i == RIGHT)	{
				if (j == 0)	boltOn[i][j] = 0, boltOn[UP][3] = 0;
				else	boltOn[i][j] = 0, boltOn[DOWN][3] = 0;
			}
			else if (i == UP && (j == 0 || j == 3))	{
				if (j == 0)	boltOn[i][j] = 0, boltOn[LEFT][0] = 0;
				else	boltOn[i][j] = 0, boltOn[RIGHT][0] = 0;
			}
			else if (i == DOWN && (j == 0 || j == 3))	{
				if (j == 0)	boltOn[i][j] = 0, boltOn[LEFT][1] = 0;
				else	boltOn[i][j] = 0, boltOn[RIGHT][1] = 0;
			}
		}


			// erases some bolts if all up and 3 down were activated
		if (boltOn[UP][0] && boltOn[UP][1] && boltOn[UP][2] && boltOn[UP][3] && boltOn[DOWN][0] && boltOn[DOWN][1] && boltOn[DOWN][2]) {
			j = rand()%8;
			if (j <4) {
				boltOn[UP][j] = 0;
			}
			else {
				j = rand()%3;
				boltOn[DOWN][j] = 0, boltOn[DOWN][j+1] = 0;
			}
		}
		else if (boltOn[UP][0] && boltOn[UP][1] && boltOn[UP][2] && boltOn[UP][3] && boltOn[DOWN][0] && boltOn[DOWN][1] && boltOn[DOWN][3]) {
			j = rand()%8;
			if (j <4) {
				boltOn[UP][j] = 0;
			}
			else {
				j = rand()%3;
				boltOn[DOWN][j] = 0, boltOn[DOWN][j+1] = 0;
			}
		}
		else if (boltOn[UP][0] && boltOn[UP][1] && boltOn[UP][2] && boltOn[UP][3] && boltOn[DOWN][0] && boltOn[DOWN][2] && boltOn[DOWN][3]) {
			j = rand()%8;
			if (j <4) {
				boltOn[UP][j] = 0;
			}
			else {
				j = rand()%3;
				boltOn[DOWN][j] = 0, boltOn[DOWN][j+1] = 0;
			}
		}
		else if (boltOn[UP][0] && boltOn[UP][1] && boltOn[UP][2] && boltOn[UP][3] && boltOn[DOWN][1] && boltOn[DOWN][2] && boltOn[DOWN][3]) {
			j = rand()%8;
			if (j <4) {
				boltOn[UP][j] = 0;
			}
			else {
				j = rand()%3;
				boltOn[DOWN][j] = 0, boltOn[DOWN][j+1] = 0;
			}
		}

		
		for (i=0; i<4; i++) {
			tboltOn[i] = false;
		}

		if (boltOn[LEFT][0] && boltOn[UP][0])	boltOn[LEFT][0] = 0, boltOn[UP][0] = 0, tboltOn[0] = true;
		if (boltOn[RIGHT][0] && boltOn[UP][3])	boltOn[RIGHT][0] = 0, boltOn[UP][3] = 0, tboltOn[1] = true;
		if (boltOn[LEFT][1] && boltOn[DOWN][0])	boltOn[LEFT][1] = 0, boltOn[DOWN][0] = 0, tboltOn[2] = true;
		if (boltOn[RIGHT][1] && boltOn[DOWN][3])	boltOn[RIGHT][1] = 0, boltOn[DOWN][3] = 0, tboltOn[3] = true;

		
			// to prevent bolts from touching each other
		else if (boltOn[LEFT][0] && boltOn[UP][1]) {
			boltOn[LEFT][0] = 0, boltOn[UP][1] = 0, tboltOn[0] = true;
		}
		else if (boltOn[RIGHT][0] && boltOn[UP][2]) {
			boltOn[RIGHT][0] = 0, boltOn[UP][2] = 0, tboltOn[1] = true;
		}
		else if (boltOn[LEFT][1] && boltOn[DOWN][1]) {
			boltOn[LEFT][1] = 0, boltOn[DOWN][1] = 0, tboltOn[2] = true;
		}
		else if (boltOn[RIGHT][1] && boltOn[DOWN][2]) {
			boltOn[RIGHT][1] = 0, boltOn[DOWN][2] = 0, tboltOn[3] = true;
		}
		else if (boltOn[LEFT][1] && boltOn[UP][0]) {
			boltOn[UP][0] = 0;
		}
		else if (boltOn[RIGHT][1] && boltOn[UP][3]) {
			boltOn[UP][3] = 0;
		}
		else if (boltOn[LEFT][0] && boltOn[DOWN][0]) {
			boltOn[DOWN][0] = 0;
		}
		else if (boltOn[RIGHT][0] && boltOn[DOWN][3]) {
			boltOn[DOWN][3] = 0;
		}
		
		
		
		alpha = 10;
		alpha2 = alpha;
		for (i=0; i<4; i++) {
			delta[i] = alpha;
		}

		while (((time - oldTime) <TIMEDELAY) && playing && !osl_quit) {

			oslReadKeys();
			
			// moving statements
			if (osl_pad.held.left || osl_pad.held.right || osl_pad.held.up || osl_pad.held.down) {
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
			}
			
			else {
			
				if (osl_pad.analogX >PAD_SENS_) {
				player->x = player->x + (MOVE_STEP*(osl_pad.analogX-PAD_SENS_))/(128-PAD_SENS_);
				if (player->x < 0)	player->x = 0;
				else if (player->x > WIDTH-player->sizeX)	player->x = WIDTH-player->sizeX;
				}
				else if (-osl_pad.analogX >PAD_SENS_) {
				player->x = player->x + (MOVE_STEP*(osl_pad.analogX+PAD_SENS_))/(128-PAD_SENS_);
				if (player->x < 0)	player->x = 0;
				else if (player->x > WIDTH-player->sizeX)	player->x = WIDTH-player->sizeX;
				}
				
				if (osl_pad.analogY >PAD_SENS_) {
				player->y = player->y + (MOVE_STEP*(osl_pad.analogY-PAD_SENS_))/(128-PAD_SENS_);
				if (player->y < 0)	player->y = 0;
				else if (player->y > HEIGHT-player->sizeY)	player->y = HEIGHT-player->sizeY;
				}
				else if (-osl_pad.analogY >PAD_SENS_) {
				player->y = player->y + (MOVE_STEP*(osl_pad.analogY+PAD_SENS_))/(128-PAD_SENS_);
				if (player->y < 0)	player->y = 0;
				else if (player->y > HEIGHT-player->sizeY)	player->y = HEIGHT-player->sizeY;
				}
			}
			
			k = 0;
			while (k <redraw && !osl_quit) {
				if (!Over.quit) {
					oslStartDrawing();
					if (level == 4)	oslDrawGradientRect(0,0,WIDTH,HEIGHT,bgColor,RGB(0,0,0),RGB(160,160,160),bgColor);
					else	oslDrawGradientRect(0,0,WIDTH,HEIGHT,bgColor,RGB(2,3,40),RGB(255,255,255),bgColor);

					// draws images and checks collisions
					for (i=LEFT; i<DOWN+1; i++)
					{
						if (tboltOn[i]) {
							if (i == 0) {
								tbolt1->x = tboltPos[i].x;
								tbolt1->y = tboltPos[i].y;
								if (((time - oldTime) >hitDelay)&&(delta[i] == 255)) {
									if (isCollideCopy(player,tbolt1,player->x,player->y,0,0,tboltPos[i].x,tboltPos[i].y,0,0)) {
										if(oslGetSoundChannel(hurt) == -1)	oslPlaySound(hurt, 4);
										Over.life-=hitDmg;
										if (Over.life <0) Over.life = 0;
									}
								}
								imFadeIn(tbolt1, delta[i], 10);
							}
							else if (i == 1) {
								tbolt2->x = tboltPos[i].x;
								tbolt2->y = tboltPos[i].y;
								if (((time - oldTime) >hitDelay)&&(delta[i] == 255)) {
									if (isCollideCopy(player,tbolt2,player->x,player->y,0,0,tboltPos[i].x,tboltPos[i].y,0,0)) {
										if(oslGetSoundChannel(hurt) == -1)	oslPlaySound(hurt, 4);
										Over.life-=hitDmg;
										if (Over.life <0) Over.life = 0;
									}
								}
								imFadeIn(tbolt2, delta[i], 10);
							}
							else if (i == 2) {
								tbolt2->x = tboltPos[i].x;
								tbolt2->y = tboltPos[i].y;
								if (((time - oldTime) >hitDelay)&&(delta[i] == 255)) {
									if (isCollideCopy(player,tbolt2,player->x,player->y,0,0,tboltPos[i].x,tboltPos[i].y,0,0)) {
										if(oslGetSoundChannel(hurt) == -1)	oslPlaySound(hurt, 4);
										Over.life-=hitDmg;
										if (Over.life <0) Over.life = 0;
									}
								}
								imFadeIn(tbolt2, delta[i], 10);
							}
							else {
								tbolt1->x = tboltPos[i].x;
								tbolt1->y = tboltPos[i].y;
								if (((time - oldTime) >hitDelay)&&(delta[i] == 255)) {
									if (isCollideCopy(player,tbolt1,player->x,player->y,0,0,tboltPos[i].x,tboltPos[i].y,0,0)) {
										if(oslGetSoundChannel(hurt) == -1)	oslPlaySound(hurt, 4);
										Over.life-=hitDmg;
										if (Over.life <0) Over.life = 0;
									}
								}
								imFadeIn(tbolt1, delta[i], 10);
							}
						}

						for (j=0; j<4; j++)
						{
							if ((i >RIGHT) && boltOn[i][j]) {
								vbolt->x = boltPos[i][j].x;
								vbolt->y = boltPos[i][j].y;
								if (((time - oldTime) >hitDelay)&&(alpha == 255)) {
									if (isCollideCopy(player,vbolt,player->x,player->y,0,0,boltPos[i][j].x,boltPos[i][j].y,0,0)) {
										if(oslGetSoundChannel(hurt) == -1)	oslPlaySound(hurt, 4);
										Over.life-=hitDmg;
										if (Over.life <0) Over.life = 0;
									}
								}
								imFadeIn(vbolt, alpha, 10);
							}
							else if (boltOn[i][j]) {
								hbolt->x = boltPos[i][j].x;
								hbolt->y = boltPos[i][j].y;
								if (((time - oldTime) >hitDelay)&&(alpha2 == 255)) {
									if (isCollideCopy(player,hbolt,player->x,player->y,0,0,boltPos[i][j].x,boltPos[i][j].y,0,0)) {
										if(oslGetSoundChannel(hurt) == -1)	oslPlaySound(hurt, 4);
										Over.life-=hitDmg;
										if (Over.life <0) Over.life = 0;
									}
								}
								imFadeIn(hbolt, alpha2, 10);
							}
						}
					}


					oslSetAlpha(OSL_FX_TINT, RGBA(253+(((HURT_COL_R-253)*(100-Over.life))/100), 255+(((HURT_COL_G-255)*(100-Over.life))/100), 117+(((HURT_COL_B-117)*(100-Over.life))/100), 255));
					//oslSetAlpha(OSL_FX_TINT, RGBA(253+((2*(100-Over.life))/100), 255-((255*(100-Over.life))/100), 117-((117*(100-Over.life))/100), 255));
					oslDrawImage(player);
					oslSetAlpha(OSL_FX_DEFAULT, 0);
					
					sprintf(score, "Score : %d", Score+(time/60)*10);
					sprintf(health, "Health : %3d %%", Over.life);
					oslIntraFontSetStyle(f, 0.8f,RGBA(255,255,255,255), RGBA(0,0,0,224),INTRAFONT_ALIGN_LEFT);
					oslDrawString(20, 12, score);
					oslDrawString(20, 12+height[0]+0, health);
					
					oslSetAlpha(OSL_FX_ALPHA, 96);
					oslDrawFillRect(0,HEIGHT - (HEIGHT/16 + HEIGHT/32),WIDTH,HEIGHT,RGB(Progress.r,Progress.g,Progress.b));
					oslSetAlpha(OSL_FX_ALPHA, 48);
					oslDrawFillRect(0 + 2,HEIGHT - (HEIGHT/16 + HEIGHT/32)+ 2,2 + ((WIDTH - 4)*((100 - ((timeLimit - time)*100)/timeLimit)/PREVIEW))/(100/PREVIEW),HEIGHT - 2,RGB(8,8,8));
					sprintf(score, "%3d %%", ((100 - ((timeLimit - time)*100)/timeLimit)/PREVIEW)*PREVIEW);
					oslIntraFontSetStyle(f, 0.7f,RGBA(192,192,192,224), RGBA(0,0,0,224),INTRAFONT_ALIGN_CENTER);
					oslDrawString(WIDTH/2, (HEIGHT - (HEIGHT/16 + HEIGHT/32) + HEIGHT - height[1]/2)/2 - 1, score);
					oslSetAlpha(OSL_FX_DEFAULT, 0);
					
					oslEndDrawing();
					oslEndFrame();
					oslSyncFrame();
				}

				if (Over.life <30 && !critic_on)	oslPlaySound(critic, 5), critic_on = true;
				if (redraw == 2) {
					if (game_quit)	{
						oslPauseSound(mgame, 1);
						if (quitScreen(f))	playing = false, Over.quit = true;
						else	game_quit = false, oslPauseSound(mgame, 0);
					}
					else if (lost_game)	playing = false, Over.quit = false;
					else if (won_game)	playing = false;
				}

				else if (osl_pad.pressed.start || osl_pad.pressed.circle)	redraw = 2, game_quit = true;

				else if (time >timeLimit/* || osl_pad.held.L*/)	redraw = 2, won_game = true;

				else if (Over.life == 0)	redraw = 2, lost_game = true;


				k++;
			}
			
			time++;
			
			redraw = 1;
			
			if (delta[0] <245) delta[0]+= 10;
			else delta[0] = 255;
			if (delta[1] <245) delta[1]+= 10;
			else delta[1] = 255;
			if (delta[2] <245) delta[2]+= 10;
			else delta[2] = 255;
			if (delta[3] <245) delta[3]+= 10;
			else delta[3] = 255;
			if (alpha <245) alpha+= 10;
			else alpha = 255;
			if (alpha2 <245) alpha2+= 10;
			else alpha2 = 255;
		}

		oldTime = time;
	}

	oslDeleteImage(tbolt2);
	oslDeleteImage(tbolt1);
	oslDeleteImage(vbolt);
	oslDeleteImage(hbolt);
	return Over;
}
