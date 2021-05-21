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
void castAllRays(const uint32_t* rayData);
uint32_t castRay(uint16_t angle);
void draw2dField();
void drawRectangle(uint16_t x, uint16_t y, uint16_t width, uint16_t height, CASColor color);
void drawLine(int32_t x1, int32_t y1, int32_t x2, int32_t y2, CASColor color);

uint8_t map[MAP_HEIGHT][MAP_WIDTH] =
{
  1, 1, 1, 1, 1, 1, 1, 1,
  1, 0, 0, 0, 0, 0, 0, 1,
  1, 0, 1, 1, 0, 1, 0, 1,
  1, 0, 0, 0, 0, 0, 0, 1,
  1, 0, 1, 0, 0, 1, 0, 1,
  1, 0, 0, 0, 0, 0, 0, 1,
  1, 0, 0, 0, 0, 0, 0, 1,
  1, 1, 1, 1, 1, 1, 1, 1
};

uint32_t rayData[PLAYER_FOV];

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

  mainPlayer.xPos = 32762;
  mainPlayer.yPos = 32760;
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

uint32_t castRay(uint16_t angle)
{
  int32_t slopeX; // slope in x direction (dy / dx)
  int32_t slopeY; // slope in y direction (dx / dy)

  int32_t xStep; // the length of one step in x direction
  int32_t yStep; // the length of one step in y direction

  uint16_t rayXPosX; // the location of the ray end in x steps
  uint16_t rayXPosY; // the location of the ray end in x steps
  uint16_t rayYPosX; // the location of the ray end in y steps
  uint16_t rayYPosY; // the location of the ray end in y steps

  uint32_t stepLengthX; // the length of the ray in one x step
  uint32_t stepLengthY; // the length of the ray in one y step

  uint32_t rayLengthX; // total length of ray in x steps
  uint32_t rayLengthY; // total length of ray in y steps

  int8_t gridOffsetX = 0;
  int8_t gridOffsetY = 0;

  // to draw line
  uint16_t playerPixelX = (mainPlayer.xPos * (MAP_WIDTH * TILE_SIZE)) / POWER_16;
  uint16_t playerPixelY = (mainPlayer.yPos * (MAP_HEIGHT * TILE_SIZE)) / POWER_16;
  uint16_t playerCenterPixelX = playerPixelX + (PLAYER_SIZE / 2);
  uint16_t playerCenterPixelY = playerPixelY + (PLAYER_SIZE / 2);

  int16_t rayAngle = (mainPlayer.angle - (PLAYER_FOV / 2) + angle);

  if(rayAngle > 359)
    rayAngle -= 360;
  else if(rayAngle < 0)
    rayAngle += 360;

  rayAngle = 359 - rayAngle;

  if(rayAngle < 90)
  {
    slopeX = (tantable[89 - rayAngle] / MAP_HEIGHT);
    slopeY = -(tantable[rayAngle] / MAP_WIDTH);
    
    stepLengthX = ((GRID_WIDTH / MAP_WIDTH) * POWER_16) / sintable[rayAngle];
    stepLengthY = ((GRID_HEIGHT / MAP_HEIGHT) * POWER_16) / sintable[rayAngle + 90];

    xStep = -(GRID_WIDTH / MAP_WIDTH);
    yStep = (GRID_HEIGHT / MAP_HEIGHT);

    rayXPosX = ((mainPlayer.xPos / (GRID_WIDTH / MAP_WIDTH)) * (GRID_WIDTH / MAP_WIDTH));
    rayXPosY = (((mainPlayer.xPos - rayXPosX) * tantable[89 - rayAngle]) / POWER_16) + mainPlayer.yPos;
    rayYPosY = (((mainPlayer.yPos / (GRID_HEIGHT / MAP_HEIGHT)) + 1) * (GRID_HEIGHT / MAP_HEIGHT));
    rayYPosX = (((rayYPosY - mainPlayer.yPos) * -(tantable[rayAngle]) / POWER_16)) + mainPlayer.xPos;

    // calculate inital ray length to first map border
    rayLengthX = ((mainPlayer.xPos - rayXPosX) * POWER_16) / sintable[rayAngle];
    rayLengthY = ((rayYPosY - mainPlayer.yPos) * POWER_16) / sintable[rayAngle + 90];

    gridOffsetX = -1;
  }
  else if(rayAngle < 180)
  {
    slopeX = -(tantable[rayAngle - 90] / MAP_HEIGHT);
    slopeY = -(tantable[89 - (rayAngle - 90)] / MAP_WIDTH);
    
    stepLengthX = ((GRID_WIDTH / MAP_WIDTH) * POWER_16) / sintable[(rayAngle - 90) + 90];
    stepLengthY = ((GRID_HEIGHT / MAP_HEIGHT) * POWER_16) / sintable[rayAngle - 90];

    xStep = -(GRID_WIDTH / MAP_WIDTH);
    yStep = -(GRID_HEIGHT / MAP_HEIGHT);

    rayXPosX = ((mainPlayer.xPos / (GRID_WIDTH / MAP_WIDTH)) * (GRID_WIDTH / MAP_WIDTH));
    rayXPosY = (((mainPlayer.xPos - rayXPosX) * -(tantable[rayAngle - 90])) / POWER_16) + mainPlayer.yPos;
    rayYPosY = (((mainPlayer.yPos / (GRID_HEIGHT / MAP_HEIGHT))) * (GRID_HEIGHT / MAP_HEIGHT));
    rayYPosX = (((mainPlayer.yPos - rayYPosY) * -(tantable[89 - (rayAngle - 90)]) / POWER_16)) + mainPlayer.xPos;

    // calculate inital ray length to first map border
    rayLengthX = ((mainPlayer.xPos - rayXPosX) * POWER_16) / sintable[(rayAngle - 90) + 90];
    rayLengthY = ((mainPlayer.yPos - rayYPosY) * POWER_16) / sintable[rayAngle - 90];

    gridOffsetX = -1;
    gridOffsetY = -1;
  }
  else if(rayAngle < 270)
  {
    slopeX = -(tantable[89 - (rayAngle - 180)] / MAP_HEIGHT);
    slopeY = (tantable[rayAngle - 180] / MAP_WIDTH);
    
    stepLengthX = ((GRID_WIDTH / MAP_WIDTH) * POWER_16) / sintable[rayAngle - 180];
    stepLengthY = ((GRID_HEIGHT / MAP_HEIGHT) * POWER_16) / sintable[(rayAngle - 180) + 90];

    xStep = (GRID_WIDTH / MAP_WIDTH);
    yStep = -(GRID_HEIGHT / MAP_HEIGHT);

    rayXPosX = (((mainPlayer.xPos / (GRID_WIDTH / MAP_WIDTH)) + 1) * (GRID_WIDTH / MAP_WIDTH));
    rayXPosY = (((rayXPosX - mainPlayer.xPos) * -(tantable[89 - (rayAngle - 180)])) / POWER_16) + mainPlayer.yPos;
    rayYPosY = ((mainPlayer.yPos / (GRID_HEIGHT / MAP_HEIGHT)) * (GRID_HEIGHT / MAP_HEIGHT));
    rayYPosX = (((mainPlayer.yPos - rayYPosY) * (tantable[rayAngle - 180]) / POWER_16)) + mainPlayer.xPos;

    // calculate inital ray length to first map border
    rayLengthX = ((rayXPosX - mainPlayer.xPos) * POWER_16) / sintable[rayAngle - 180];
    rayLengthY = ((mainPlayer.yPos - rayYPosY) * POWER_16) / sintable[(rayAngle - 180) + 90];

    gridOffsetY = -1;
  }
  else
  {
    slopeX = (tantable[rayAngle - 270] / MAP_HEIGHT);
    slopeY = (tantable[89 - (rayAngle - 270)] / MAP_WIDTH);
    
    stepLengthX = ((GRID_WIDTH / MAP_WIDTH) * POWER_16) / sintable[(rayAngle - 270) + 90];
    stepLengthY = ((GRID_HEIGHT / MAP_HEIGHT) * POWER_16) / sintable[rayAngle - 270];

    xStep = (GRID_WIDTH / MAP_WIDTH);
    yStep = (GRID_HEIGHT / MAP_HEIGHT);

    rayXPosX = (((mainPlayer.xPos / (GRID_WIDTH / MAP_WIDTH)) + 1) * (GRID_WIDTH / MAP_WIDTH));
    rayXPosY = (((rayXPosX - mainPlayer.xPos) * (tantable[rayAngle - 270])) / POWER_16) + mainPlayer.yPos;
    rayYPosY = (((mainPlayer.yPos / (GRID_HEIGHT / MAP_HEIGHT)) + 1) * (GRID_HEIGHT / MAP_HEIGHT));
    rayYPosX = (((rayYPosY - mainPlayer.yPos) * (tantable[89 - (rayAngle - 270)]) / POWER_16)) + mainPlayer.xPos;

    // calculate inital ray length to first map border
    rayLengthX = ((rayXPosX - mainPlayer.xPos) * POWER_16) / sintable[(rayAngle - 270) + 90];
    rayLengthY = ((rayYPosY - mainPlayer.yPos) * POWER_16) / sintable[rayAngle - 270];
  }

  // printHexWord(rayAngle, 0, 0);
  // printHexWord(slopeX >>16, 0, 1);
  // printHexWord(slopeX, 5, 1);
  // printHexWord(slopeY >>16, 0, 2);
  // printHexWord(slopeY, 5, 2);
  // printHexWord(rayXPosX, 0, 3);
  // printHexWord(rayYPosY, 0, 4);

  while (1)
  {
    // check which ray is shorter
    if(rayLengthX < rayLengthY)
    {
        // CASColor color;
        // color.asShort = 0x0AA8;
        // drawLine(playerCenterPixelX, playerCenterPixelY, (rayXPosX * (MAP_WIDTH * TILE_SIZE)) / POWER_16, (rayXPosY * (MAP_HEIGHT * TILE_SIZE)) / POWER_16, color);

        // refreshDisplay();
        // refreshDisplay();
        // refreshDisplay();
        // refreshDisplay();
        // refreshDisplay();
        // refreshDisplay();
        // refreshDisplay();
        // refreshDisplay();
        // refreshDisplay();
        // refreshDisplay();
        // refreshDisplay();
        // refreshDisplay();
        // refreshDisplay();
        // refreshDisplay();
        // refreshDisplay();

      // check if the ray hit a wall
      uint8_t mapInfo = map[rayXPosY / (GRID_HEIGHT / MAP_HEIGHT)][rayXPosX / (GRID_WIDTH / MAP_WIDTH) + gridOffsetX];

      if(abs(((rayXPosY / (GRID_HEIGHT / MAP_HEIGHT)) * (GRID_HEIGHT / MAP_HEIGHT)) - rayXPosY) < 1000 && abs(((rayXPosX / (GRID_HEIGHT / MAP_HEIGHT)) * (GRID_HEIGHT / MAP_HEIGHT)) - rayXPosX) < 1000)
        mapInfo = map[rayXPosY / (GRID_HEIGHT / MAP_HEIGHT)][rayXPosX / (GRID_WIDTH / MAP_WIDTH) + gridOffsetX];

      
      if(mapInfo == 0)
      {
        rayXPosX += xStep;
        rayXPosY += slopeX;
        rayLengthX += stepLengthX;
      }
      else
      {
        // hit
        // draw line
        CASColor color;
        color.asShort = 0x0AA8;
        drawLine(playerCenterPixelX, playerCenterPixelY, (rayXPosX * (MAP_WIDTH * TILE_SIZE)) / POWER_16, (rayXPosY * (MAP_HEIGHT * TILE_SIZE)) / POWER_16, color);

        // refreshDisplay();
        // refreshDisplay();
        // refreshDisplay();
        // refreshDisplay();
        // refreshDisplay();
        // refreshDisplay();
        // refreshDisplay();
        // refreshDisplay();
        // refreshDisplay();
        // refreshDisplay();
        // refreshDisplay();
        // refreshDisplay();
        // refreshDisplay();
        // refreshDisplay();
        // refreshDisplay();
        return rayLengthX;
      }
    } 
    else
    {
        // CASColor color;
        // color.asShort = 0xA800;
        // drawLine(playerCenterPixelX, playerCenterPixelY, (rayYPosX * (MAP_WIDTH * TILE_SIZE)) / POWER_16, (rayYPosY * (MAP_HEIGHT * TILE_SIZE)) / POWER_16, color);

        // refreshDisplay();
        // refreshDisplay();
        // refreshDisplay();
        // refreshDisplay();
        // refreshDisplay();
        // refreshDisplay();
        // refreshDisplay();
        // refreshDisplay();
        // refreshDisplay();
        // refreshDisplay();
        // refreshDisplay();
        // refreshDisplay();
        // refreshDisplay();
        // refreshDisplay();
        // refreshDisplay();
      // check if the ray hit a wall
      uint8_t mapInfo = map[rayYPosY / (GRID_HEIGHT / MAP_HEIGHT) + gridOffsetY][rayYPosX / (GRID_WIDTH / MAP_WIDTH)];
      
      if(mapInfo == 0)
      {
        rayYPosY += yStep;
        rayYPosX += slopeY;
        rayLengthY += stepLengthY;
      }
      else
      {
        // hit
        // draw line
        CASColor color;
        color.asShort = 0xA800;
        drawLine(playerCenterPixelX, playerCenterPixelY, (rayYPosX * (MAP_WIDTH * TILE_SIZE)) / POWER_16, (rayYPosY * (MAP_HEIGHT * TILE_SIZE)) / POWER_16, color);

        // refreshDisplay();
        // refreshDisplay();
        // refreshDisplay();
        // refreshDisplay();
        // refreshDisplay();
        // refreshDisplay();
        // refreshDisplay();
        // refreshDisplay();
        // refreshDisplay();
        // refreshDisplay();
        // refreshDisplay();
        // refreshDisplay();
        // refreshDisplay();
        // refreshDisplay();
        // refreshDisplay();
        return rayLengthY;
      }
    } 
  } 
}

void castAllRays(const uint32_t* rayData)
{
  for(uint8_t x = 0; x < PLAYER_FOV; x += 2)
  {
    rayData[x] = castRay(x);
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
  
  // refreshDisplay();
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
  
  // refreshDisplay();
    }
  }
}

void main() __attribute__((section(".text.main")));
