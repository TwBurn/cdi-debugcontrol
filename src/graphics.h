#ifndef __GRAPHICS_H__
#define	__GRAPHICS_H__

#include "video.h"

extern int curIcfA, curIcfB;
extern unsigned char *paCursor, *pbBackground;

void setIcf(icfA, icfB);

#endif