#ifndef _CONSTANTS_H
#define _CONSTANTS_H

#define POWER_16  0xFFFF

#define sgn(x) ((x<0)?-1:((x>0)?1:0)) /* macro to return the sign of a number */

#define MAP_HEIGHT  8
#define MAP_WIDTH   8

#define GRID_HEIGHT  POWER_16
#define GRID_WIDTH   POWER_16

#define TILE_SIZE   40

#define PLAYER_SIZE 5
#define PLAYER_SENSITIVITY 10

#endif