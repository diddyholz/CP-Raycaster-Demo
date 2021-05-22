#ifndef _CONSTANTS_H
#define _CONSTANTS_H

#include <cas-sdk/display.h>

#define BIT_16  0x10000

#define sgn(x) ((x<0)?-1:((x>0)?1:0)) /* macro to return the sign of a number */
#define inDisplay(x, y) ((x < DISPLAY_WIDTH && y < DISPLAY_HEIGHT)? 1, 0) /* macro to check if in display */

#define MAP_HEIGHT  32
#define MAP_WIDTH   32

#define GRID_HEIGHT  BIT_16
#define GRID_WIDTH   BIT_16

#define TILE_SIZE   10

#define PLAYER_SIZE 5
#define PLAYER_SENSITIVITY 10
#define PLAYER_FOV  60

#define TOP_OFFSET_3D (MAP_HEIGHT * TILE_SIZE)
#define HEIGHT_3D 208

#define WALL_HEIGHT_CONSTANT  800000
#define WALL_COLOR 0x47E0

#define CEILING_COLOR 0x6E7F
#define FLOOR_COLOR   0x6B6D


#endif