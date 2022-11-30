
/* This is a options file which serves the purpose of 
changing common variables like screen resolution */

#pragma once

#include <stdio.h>

#define LOAD_FONT

#define OT_LEN			4096*4
#define PACKET_LEN		16384*4

#define SCREEN_XRES		        320
#define SCREEN_YRES_NTSC		240
#define SCREEN_YRES_PAL		    256

#define CENTERX			(SCREEN_XRES / 2)
#define CENTERY_NTSC		(SCREEN_YRES_NTSC / 2)
#define CENTERY_PAL		(SCREEN_YRES_PAL / 2)

typedef POLY_GT4 QUAD;
