#include <stdlib.h>
#include <cas-sdk/display.h>
#include <cas-sdk/input/key-input.h>
#include <cas-sdk/input/touch-input.h>
#include "math_tables.h"
#include "constants.h"

typedef struct 
{
  uint16_t xPos;
  uint16_t yPos;
  uint16_t angle;
  int16_t velocity;
} Player;

void gameSetup();
void gameLoop();
void characterManipulation();
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

void main()
{
  gameSetup();
  gameLoop();
}

void gameSetup()
{
  fillFrameBufferS(0x0000);
  refreshDisplay();

  mainPlayer.xPos = 32767;
  mainPlayer.yPos = 32767;
  mainPlayer.angle = 0;
}

void gameLoop()
{
  while (1)
  {
    playerInput();
    characterManipulation();
    draw2dField();
  }
}

void characterManipulation()
{
  mainPlayer.xPos += ((sintable[mainPlayer.angle] * mainPlayer.velocity) / POWER_16);
  mainPlayer.yPos += ((sintable[mainPlayer.angle + 90] * mainPlayer.velocity) / POWER_16);
  
  mainPlayer.velocity = 0;
}

void playerInput()
{
  CASKeyboardInput input = getKeyInput();

  if(input.bufferTwo & KEY_DOWN_2) 
  {
    mainPlayer.velocity = -500;
  }
  if(input.bufferTwo & KEY_UP_2)
  {
    mainPlayer.velocity = 500;
  }
  if(input.bufferOne & KEY_RIGHT_1)
  {
    if(mainPlayer.angle == 0)
      mainPlayer.angle = 360;

    mainPlayer.angle -= PLAYER_SENSITIVITY;
  }
  if(input.bufferOne & KEY_LEFT_1)
  {
    mainPlayer.angle += PLAYER_SENSITIVITY;
   
    if(mainPlayer.angle == 360)
      mainPlayer.angle = 0;
  }
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

  uint16_t playerPixelX = (mainPlayer.xPos * (MAP_WIDTH * TILE_SIZE)) / POWER_16;
  uint16_t playerPixelY = (mainPlayer.yPos * (MAP_HEIGHT * TILE_SIZE)) / POWER_16;
  uint16_t playerCenterPixelX = playerPixelX + (PLAYER_SIZE / 2);
  uint16_t playerCenterPixelY = playerPixelY + (PLAYER_SIZE / 2);

  drawRectangle(playerPixelX, playerPixelY, PLAYER_SIZE, PLAYER_SIZE, color);

  // draw player angle line
  uint8_t angleLineSize = 15;
  drawLine(playerCenterPixelX, playerCenterPixelY, playerCenterPixelX + ((sintable[mainPlayer.angle] * angleLineSize) / POWER_16), playerCenterPixelY + ((sintable[mainPlayer.angle + 90] * angleLineSize) / POWER_16), color);

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
