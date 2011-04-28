#ifndef ppc
#define ppc

bool realCollide(OSL_IMAGE *one, OSL_IMAGE *two);
bool isCollide(OSL_IMAGE *one, OSL_IMAGE *two);
bool realCollideCopy(OSL_IMAGE *one, OSL_IMAGE *two, int x1, int y1, int cx1, int cy1, int x2, int y2, int cx2, int cy2);
bool isCollideCopy(OSL_IMAGE *one, OSL_IMAGE *two, int px1, int py1, int pcx1, int pcy1, int px2, int py2, int pcx2, int pcy2);

#endif
