#include <stdlib.h>
#include <cas-sdk/display.h>
#include <cas-sdk/input/key-input.h>
#include <cas-sdk/input/touch-input.h>

#define MAP_HEIGHT  8
#define MAP_WIDTH   8
#define TILE_SIZE   40
#define PLAYER_SIZE 5
#define sgn(x) ((x<0)?-1:((x>0)?1:0)) /* macro to return the sign of a number */

typedef struct 
{
  float xPos;
  float yPos;
  float angle;
} Player;

void gameSetup();
void gameLoop();
void playerInput();
void draw2dField();
void drawRectangle(uint16_t x, uint16_t y, uint16_t width, uint16_t height, CASColor color);
void drawLine(int32_t x1, int32_t y1, int32_t x2, int32_t y2, CASColor color);

uint8_t map[MAP_HEIGHT][MAP_WIDTH] =
{
  1, 1, 1, 1, 1, 1, 1, 1,
  1, 0, 0, 0, 0, 0, 0, 1,
  1, 0, 0, 0, 0, 0, 0, 1,
  1, 0, 0, 0, 0, 0, 0, 1,
  1, 0, 0, 0, 0, 0, 0, 1,
  1, 0, 0, 0, 0, 0, 0, 1,
  1, 0, 0, 0, 0, 0, 0, 1,
  1, 1, 1, 1, 1, 1, 1, 1
};

Player mainPlayer;

// 0x8C20038E hangs CPU (sts fpul, r1)


void main()
{
  gameSetup();
  gameLoop();
}

void gameSetup()
{
  fillFrameBufferS(0x0000);
  refreshDisplay();

  mainPlayer.xPos = 4;
  mainPlayer.yPos = 4;
  mainPlayer.angle = 0;
}

void gameLoop()
{
  while (1)
  {
    playerInput();
    draw2dField();
  }
}

void playerInput()
{
  CASKeyboardInput input = getKeyInput();

  if(input.bufferTwo & KEY_DOWN_2) mainPlayer.yPos += 0.1;
  if(input.bufferTwo & KEY_UP_2) mainPlayer.yPos -= 0.1;
  if(input.bufferOne & KEY_RIGHT_1) mainPlayer.xPos += 0.1;
  if(input.bufferOne & KEY_LEFT_1) mainPlayer.xPos -= 0.1;
}

void draw2dField()
{
  CASColor color;

  for(uint8_t y = 0; y < MAP_HEIGHT; y++)
  {
    for(uint8_t x = 0; x < MAP_WIDTH; x++)
    {
      switch (map[y][x])
      {
      case 0:
        color.asShort = 0x0000;
        break;
      case 1:
        color.asShort = 0x0000;
        color.red = 0b11111;
        break;
      
      default:
        break;
      }
      // draw the tile
      drawRectangle(x * TILE_SIZE, y * TILE_SIZE, TILE_SIZE, TILE_SIZE, color);

      // draw borders
      color.asShort = 0xFFFF;

      drawRectangle(x * TILE_SIZE, y * TILE_SIZE, TILE_SIZE, 1, color);
      drawRectangle(x * TILE_SIZE, y * TILE_SIZE, 1, TILE_SIZE, color);
      drawRectangle(x * TILE_SIZE, (y * TILE_SIZE) + TILE_SIZE - 1, TILE_SIZE, 1, color);
      drawRectangle((x * TILE_SIZE) + TILE_SIZE - 1, y * TILE_SIZE, 1, TILE_SIZE, color);
    }
  }

  // draw player
  color.asShort = 0x0000;
  color.green = 0b111111;

  line_fast(299, 501, 5, 300, color);
  drawLine(5, 300, 299, 501, color);

  drawRectangle((uint16_t)(mainPlayer.xPos * (float)TILE_SIZE), (uint16_t)(mainPlayer.yPos * (float)TILE_SIZE), PLAYER_SIZE, PLAYER_SIZE, color);

  refreshDisplay();
}

void drawRectangle(uint16_t x, uint16_t y, uint16_t width, uint16_t height, CASColor color)
{
  for (uint16_t yOffset = 0; yOffset < height; yOffset++)
  {
    for (uint16_t xOffset = 0; xOffset < width; xOffset++)
    {
      frameBuffer[y + yOffset][x + xOffset] = color.asShort;
    }
  }
}

// void drawLine(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, CASColor color)
// {
//   uint32_t finePixelX;
//   uint32_t finePixelY;

//   uint32_t fineX2;
//   uint32_t fineY2;

//   uint8_t xLarger = 0;

//   if(x1 < x2)
//   {
//     finePixelX = x1 * 65536;
//     fineX2 = x2 * 65536;
//   }
//   else
//   {
//     finePixelX = x2 * 65536;
//     fineX2 = x1 * 65536;
//   }

//   if(y1 < y2)
//   {
//     finePixelY = y1 * 65536;
//     fineY2 = y2 * 65536;
//   }
//   else
//   {
//     finePixelY = y2 * 65536;
//     fineY2 = y1 * 65536;
//   }

//   int32_t slope;

//   if((fineX2 - finePixelX) > (fineY2 - finePixelY))
//   {
//     slope = (fineY2 - finePixelY) / ((fineX2 - finePixelX) / 65536);
//     xLarger = 1;
//   }
//   else
//   {
//     slope = (fineX2 - finePixelX) / ((fineY2 - finePixelY) / 65536);
//     xLarger = 0;
//   }

//   printHexWord(slope, 0, 0);

//   while (finePixelX != fineX2 || finePixelY != fineY2)
//   {
//     finePixelX += 65536;

//     if(fineY2 - finePixelY < 65536)
//       finePixelY = fineY2;
//     else
//       finePixelY += slope;

//     frameBuffer[finePixelY / 65536][finePixelX / 65536] = color.asShort;

//     printHexWord(finePixelY / 65536, 0, 1);
//     printHexWord(finePixelX / 65536, 0, 2);

//     refreshDisplay();
//   }
// }

void drawLine(int32_t x1, int32_t y1, int32_t x2, int32_t y2, CASColor color)
{
  int32_t i,dx,dy,sdx,sdy,dxabs,dyabs,x,y,px,py;

  dx=x2-x1;      /* the horizontal distance of the line */
  dy=y2-y1;      /* the vertical distance of the line */
  dxabs=abs(dx);
  dyabs=abs(dy);
  sdx=sgn(dx);
  sdy=sgn(dy);
  x=dyabs>>1;
  y=dxabs>>1;
  px=x1;
  py=y1;

  // VGA[(py<<8)+(py<<6)+px]=color;

  if (dxabs>=dyabs) /* the line is more horizontal than vertical */
  {
    for(i=0;i<dxabs;i++)
    {
      y+=dyabs;
      if (y>=dxabs)
      {
        y-=dxabs;
        py+=sdy;
      }
      px+=sdx;
      frameBuffer[py][px] = color.asShort;
    }
  }
  else /* the line is more vertical than horizontal */
  {
    for(i=0;i<dyabs;i++)
    {
      x+=dxabs;
      if (x>=dyabs)
      {
        x-=dyabs;
        px+=sdx;
      }
      py+=sdy;
      frameBuffer[py][px] = color.asShort;
    }
  }
}

void main() __attribute__((section(".text.main")));
