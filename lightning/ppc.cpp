#include <iostream>
#include <oslib/oslib.h>
#include "ppc.h"

#define geta(c)      (u32)(c>>24)

#define PPCLEFT      0
#define PPCRIGHT     1
#define PPCTOP       2
#define PPCBOTTOM    3

bool realCollide(OSL_IMAGE *one, OSL_IMAGE *two)
{
    int ONE[4], TWO[4] = { (one->x - one->centerX), (one->y - one->centerY), (two->x - two->centerX), (two->y - two->centerY) }; //Assigning temporary data to TWO instead of creating more vars.
    ONE[PPCLEFT] = TWO[0];
    ONE[PPCRIGHT] = TWO[0] + one->sizeX;
    ONE[PPCTOP] = TWO[1];
    ONE[PPCBOTTOM] = TWO[1] + one->sizeY;
    TWO[PPCLEFT] = TWO[2];
    TWO[PPCRIGHT] = TWO[2] + two->sizeX;
    TWO[PPCTOP] = TWO[3];
    TWO[PPCBOTTOM] += two->sizeY;
    if ((ONE[PPCRIGHT] >= TWO[PPCLEFT]) && (ONE[PPCLEFT] <= TWO[PPCRIGHT]) && (ONE[PPCBOTTOM] >= TWO[PPCTOP]) && (ONE[PPCTOP] <= TWO[PPCBOTTOM])) return true;
    return false;
}
bool isCollide(OSL_IMAGE *one, OSL_IMAGE *two)
{
    if (!realCollide(one,two)) return false;
    bool i = false;
    int onex = one->x - one->centerX, twox = two->x - two->centerX, oney = one->y - one->centerY, twoy = two->y - two->centerY, xdif = onex - twox, ydif = oney - twoy, x1, y1, x2, y2, dx, dy;
    u32 pixel1, pixel2;
    int p1, p2;
	int minSizeX = one->sizeX, minSizeY = one->sizeY;
	if (one->sizeX > two->sizeX)	minSizeX = two->sizeX;
	if (one->sizeY > two->sizeY)	minSizeY = two->sizeY;
	
	if (xdif > 0)	dx = two->sizeX - xdif, x1 = 0, x2 = two->sizeX - dx;
	else	dx = one->sizeX - xdif*(-1), x1 = one->sizeX - dx, x2 = 0;
	if (ydif >0)	dy = two->sizeY - ydif, y1 = 0, y2 = two->sizeY - dy;
	else	dy = one->sizeY - ydif*(-1), y1 = one->sizeY - dy, y2 = 0;
	
	if (dx > minSizeX)	dx = minSizeX;		//avoids checking imaginary pixels
	if (dy > minSizeY)	dy = minSizeY;
	
    oslLockImage(one);
    oslLockImage(two);
    for (int y = 0;y < dy;y++)
    {
        for (int x = 0;x < dx;x++)
        {
            pixel1 = oslGetImagePixel(one,x1+x,y1+y);
            pixel2 = oslGetImagePixel(two,x2+x,y2+y);
            p1 = geta(pixel1);
            p2 = geta(pixel2);
            if ((p1 > 10) && (p2 > 10))
            {
                i = true;
                break;
            }
        }
        if (i) break;//must break both loops instead of returning so that we can unlock the images first.
    }
    oslUnlockImage(one);
    oslUnlockImage(two);
    return i;
}
bool realCollideCopy(OSL_IMAGE *one, OSL_IMAGE *two, int x1, int y1, int cx1, int cy1, int x2, int y2, int cx2, int cy2)
{
    int ONE[4], TWO[4];
    x1 -= cx1;
    y1 -= cy1;
    x2 -= cx2;
    y2 -= cy2;
    ONE[PPCLEFT] = x1;
    ONE[PPCRIGHT] = x1 + one->sizeX;
    ONE[PPCTOP] = y1;
    ONE[PPCBOTTOM] = y1 + one->sizeY;
    TWO[PPCLEFT] = x2;
    TWO[PPCRIGHT] = x2 + two->sizeX;
    TWO[PPCTOP] = y2;
    TWO[PPCBOTTOM] = y2 + two->sizeY;
    if ((ONE[PPCRIGHT] >= TWO[PPCLEFT]) && (ONE[PPCLEFT] <= TWO[PPCRIGHT]) && (ONE[PPCBOTTOM] >= TWO[PPCTOP]) && (ONE[PPCTOP] <= TWO[PPCBOTTOM])) return true;
    return false;
}
bool isCollideCopy(OSL_IMAGE *one, OSL_IMAGE *two, int px1, int py1, int pcx1, int pcy1, int px2, int py2, int pcx2, int pcy2)
{
    if (!realCollideCopy(one,two,px1,py1,pcx1,pcy1,px2,py2,pcx2,pcy2)) return false;
    bool i = false;
    int onex = px1 - pcx1, twox = px2 - pcx2, oney = py1 - pcy1, twoy = py2 - pcy2, xdif = onex - twox, ydif = oney - twoy, x1, y1, x2, y2, dx, dy;
    u32 pixel1, pixel2;
    int p1, p2;
	int minSizeX = one->sizeX, minSizeY = one->sizeY;
	if (one->sizeX > two->sizeX)	minSizeX = two->sizeX;
	if (one->sizeY > two->sizeY)	minSizeY = two->sizeY;
	
	if (xdif > 0)	dx = two->sizeX - xdif, x1 = 0, x2 = two->sizeX - dx;
	else	dx = one->sizeX - xdif*(-1), x1 = one->sizeX - dx, x2 = 0;
	if (ydif >0)	dy = two->sizeY - ydif, y1 = 0, y2 = two->sizeY - dy;
	else	dy = one->sizeY - ydif*(-1), y1 = one->sizeY - dy, y2 = 0;
	
	if (dx > minSizeX)	dx = minSizeX;		//avoids checking imaginary pixels
	if (dy > minSizeY)	dy = minSizeY;
	
    oslLockImage(one);
    oslLockImage(two);
    for (int y = 0;y < dy;y++)
    {
        for (int x = 0;x < dx;x++)
        {
            pixel1 = oslGetImagePixel(one,x1+x,y1+y);
            pixel2 = oslGetImagePixel(two,x2+x,y2+y);
            p1 = geta(pixel1);
            p2 = geta(pixel2);
            if ((p1 > 10) && (p2 > 10))
            {
                i = true;
                break;
            }
        }
        if (i) break;
    }
    oslUnlockImage(one);
    oslUnlockImage(two);
    return i;
}
