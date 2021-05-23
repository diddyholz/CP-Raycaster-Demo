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

typedef struct 
{
  uint32_t length;
  uint16_t finalYPos;
  uint16_t finalXPos;
  uint8_t raySide;
  uint8_t hitType;
} RayData;

void gameSetup();
void gameLoop();
void characterManipulation();
void playerInput();
void castAllRays(RayData* rayData);
RayData castRay(uint16_t angle);
void draw2dField();
void draw3dField();
void drawRectangle(uint16_t x, uint16_t y, uint16_t width, uint16_t height, CASColor color);
void drawLine(int32_t x1, int32_t y1, int32_t x2, int32_t y2, CASColor color);

uint8_t map[MAP_HEIGHT][MAP_WIDTH] =
{
  1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
  1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
  1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
  1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
  1, 0, 0, 0, 0, 0, 2, 0, 0, 2, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
  1, 0, 0, 0, 0, 0, 2, 0, 0, 2, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
  1, 0, 0, 0, 0, 0, 2, 0, 0, 2, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
  1, 0, 0, 0, 2, 2, 2, 0, 0, 2, 2, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
  1, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
  1, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
  1, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
  1, 0, 0, 0, 2, 0, 0, 1, 1, 0, 0, 2, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
  1, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
  1, 0, 0, 0, 2, 2, 2, 2, 2, 2, 2, 2, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
  1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
  1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
  1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1,
  1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
  1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
  1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
  1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
  1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
  1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
  1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
  1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
  1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
  1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
  1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
  1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
  1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
  1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
  1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
};

uint16_t textureArray[TEXTURE_COUNT][TEXTURE_RES][TEXTURE_RES] = 
{
  0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0x0000, 0x0000, 0x0000, 0x0000,
  0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0x0000, 0x0000, 0x0000, 0x0000,
  0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0x0000, 0x0000, 0x0000, 0x0000,
  0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0x0000, 0x0000, 0x0000, 0x0000,
  0x0000, 0x0000, 0x0000, 0x0000, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF,
  0x0000, 0x0000, 0x0000, 0x0000, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF,
  0x0000, 0x0000, 0x0000, 0x0000, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF,
  0x0000, 0x0000, 0x0000, 0x0000, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF,

  0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF,
  0xFFFF, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0xFFFF,
  0xFFFF, 0x0000, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0x0000, 0xFFFF,
  0xFFFF, 0x0000, 0xFFFF, 0x0000, 0x0000, 0xFFFF, 0x0000, 0xFFFF,
  0xFFFF, 0x0000, 0xFFFF, 0x0000, 0x0000, 0xFFFF, 0x0000, 0xFFFF,
  0xFFFF, 0x0000, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0x0000, 0xFFFF,
  0xFFFF, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0xFFFF,
  0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF,
};

RayData rayData[DISPLAY_WIDTH - 20];

Player mainPlayer;

uint8_t engineRunning = 1;
uint8_t display2dField = 1;

void main()
{
  gameSetup();
  gameLoop();
}

void gameSetup()
{
  fillFrameBufferS(0x0000);
  refreshDisplay();

  mainPlayer.xPos = 40762;
  mainPlayer.yPos = 40760;
  mainPlayer.angle = 0;
}

void gameLoop()
{
  while (engineRunning)
  {
    playerInput();
    characterManipulation();
    castAllRays(rayData);

    if(display2dField)
      draw2dField();
    
    draw3dField();

    refreshDisplay();
  }
}

void characterManipulation()
{
  uint16_t oldXPos = mainPlayer.xPos;
  uint16_t oldYPos = mainPlayer.yPos;

  // check if can move to y direction
  if(map[(oldYPos + (sgn(sintable[mainPlayer.angle + 90]) * sgn(mainPlayer.velocity) * PLAYER_FRONT_OFFSET)) / (GRID_HEIGHT / MAP_HEIGHT)][oldXPos / (GRID_WIDTH / MAP_WIDTH)] == 0)
    mainPlayer.yPos += ((sintable[mainPlayer.angle + 90] * mainPlayer.velocity) / BIT_16);

  // check if can move to x direction
  if(map[oldYPos / (GRID_HEIGHT / MAP_HEIGHT)][(oldXPos + (sgn(sintable[mainPlayer.angle]) * sgn(mainPlayer.velocity) * PLAYER_FRONT_OFFSET)) / (GRID_WIDTH / MAP_WIDTH)] == 0)
    mainPlayer.xPos += ((sintable[mainPlayer.angle] * mainPlayer.velocity) / BIT_16);
 
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

  if(input.bufferOne & KEY_SHIFT_1)
    display2dField = !display2dField;

  if((input.bufferOne & KEY_SHIFT_1) && (input.bufferOne & KEY_ON_CLEAR_1))
    engineRunning = 0;
}

RayData castRay(const uint16_t angle)
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
  // uint16_t playerPixelX = (mainPlayer.xPos * (MAP_WIDTH * TILE_SIZE)) / BIT_16;
  // uint16_t playerPixelY = (mainPlayer.yPos * (MAP_HEIGHT * TILE_SIZE)) / BIT_16;
  // uint16_t playerCenterPixelX = playerPixelX + (PLAYER_SIZE / 2);
  // uint16_t playerCenterPixelY = playerPixelY + (PLAYER_SIZE / 2);

  int16_t rayAngle = (((mainPlayer.angle - (PLAYER_FOV / 2)) * 10) + (angle * 2));

  if(rayAngle > 3599)
    rayAngle -= 3600;
  else if(rayAngle < 0)
    rayAngle += 3600;

  rayAngle = 3599 - rayAngle;

  if(rayAngle < 900)
  {
    slopeX = (fine_tantable[900 - rayAngle] / MAP_HEIGHT);
    slopeY = -(fine_tantable[rayAngle] / MAP_WIDTH);
    
    stepLengthX = ((GRID_WIDTH / MAP_WIDTH) * BIT_16) / fine_sintable[rayAngle];
    stepLengthY = ((GRID_HEIGHT / MAP_HEIGHT) * BIT_16) / fine_sintable[rayAngle + 900];

    xStep = -(GRID_WIDTH / MAP_WIDTH);
    yStep = (GRID_HEIGHT / MAP_HEIGHT);

    rayXPosX = ((mainPlayer.xPos / (GRID_WIDTH / MAP_WIDTH)) * (GRID_WIDTH / MAP_WIDTH));
    rayXPosY = (((mainPlayer.xPos - rayXPosX) * fine_tantable[90 - rayAngle]) / BIT_16) + mainPlayer.yPos;
    rayYPosY = (((mainPlayer.yPos / (GRID_HEIGHT / MAP_HEIGHT)) + 1) * (GRID_HEIGHT / MAP_HEIGHT));
    rayYPosX = (((rayYPosY - mainPlayer.yPos) * -(fine_tantable[rayAngle]) / BIT_16)) + mainPlayer.xPos;

    // calculate inital ray length to first map border
    rayLengthX = ((mainPlayer.xPos - rayXPosX) * BIT_16) / fine_sintable[rayAngle];
    rayLengthY = ((rayYPosY - mainPlayer.yPos) * BIT_16) / fine_sintable[rayAngle + 900];

    gridOffsetX = -1;
  }
  else if(rayAngle < 1800)
  {
    slopeX = -(fine_tantable[rayAngle - 900] / MAP_HEIGHT);
    slopeY = -(fine_tantable[900 - (rayAngle - 900)] / MAP_WIDTH);
    
    stepLengthX = ((GRID_WIDTH / MAP_WIDTH) * BIT_16) / fine_sintable[(rayAngle - 900) + 900];
    stepLengthY = ((GRID_HEIGHT / MAP_HEIGHT) * BIT_16) / fine_sintable[rayAngle - 900];

    xStep = -(GRID_WIDTH / MAP_WIDTH);
    yStep = -(GRID_HEIGHT / MAP_HEIGHT);

    rayXPosX = ((mainPlayer.xPos / (GRID_WIDTH / MAP_WIDTH)) * (GRID_WIDTH / MAP_WIDTH));
    rayXPosY = (((mainPlayer.xPos - rayXPosX) * -(fine_tantable[rayAngle - 900])) / BIT_16) + mainPlayer.yPos;
    rayYPosY = (((mainPlayer.yPos / (GRID_HEIGHT / MAP_HEIGHT))) * (GRID_HEIGHT / MAP_HEIGHT));
    rayYPosX = (((mainPlayer.yPos - rayYPosY) * -(fine_tantable[900 - (rayAngle - 900)]) / BIT_16)) + mainPlayer.xPos;

    // calculate inital ray length to first map border
    rayLengthX = ((mainPlayer.xPos - rayXPosX) * BIT_16) / fine_sintable[(rayAngle - 900) + 900];
    rayLengthY = ((mainPlayer.yPos - rayYPosY) * BIT_16) / fine_sintable[rayAngle - 900];

    gridOffsetX = -1;
    gridOffsetY = -1;
  }
  else if(rayAngle < 2700)
  {
    slopeX = -(fine_tantable[900 - (rayAngle - 1800)] / MAP_HEIGHT);
    slopeY = (fine_tantable[rayAngle - 1800] / MAP_WIDTH);
    
    stepLengthX = ((GRID_WIDTH / MAP_WIDTH) * BIT_16) / fine_sintable[rayAngle - 1800];
    stepLengthY = ((GRID_HEIGHT / MAP_HEIGHT) * BIT_16) / fine_sintable[(rayAngle - 1800) + 900];

    xStep = (GRID_WIDTH / MAP_WIDTH);
    yStep = -(GRID_HEIGHT / MAP_HEIGHT);

    rayXPosX = (((mainPlayer.xPos / (GRID_WIDTH / MAP_WIDTH)) + 1) * (GRID_WIDTH / MAP_WIDTH));
    rayXPosY = (((rayXPosX - mainPlayer.xPos) * -(fine_tantable[900 - (rayAngle - 1800)])) / BIT_16) + mainPlayer.yPos;
    rayYPosY = ((mainPlayer.yPos / (GRID_HEIGHT / MAP_HEIGHT)) * (GRID_HEIGHT / MAP_HEIGHT));
    rayYPosX = (((mainPlayer.yPos - rayYPosY) * (fine_tantable[rayAngle - 1800]) / BIT_16)) + mainPlayer.xPos;

    // calculate inital ray length to first map border
    rayLengthX = ((rayXPosX - mainPlayer.xPos) * BIT_16) / fine_sintable[rayAngle - 1800];
    rayLengthY = ((mainPlayer.yPos - rayYPosY) * BIT_16) / fine_sintable[(rayAngle - 1800) + 900];

    gridOffsetY = -1;
  }
  else
  {
    slopeX = (fine_tantable[rayAngle - 2700] / MAP_HEIGHT);
    slopeY = (fine_tantable[900 - (rayAngle - 2700)] / MAP_WIDTH);
    
    stepLengthX = ((GRID_WIDTH / MAP_WIDTH) * BIT_16) / fine_sintable[(rayAngle - 2700) + 900];
    stepLengthY = ((GRID_HEIGHT / MAP_HEIGHT) * BIT_16) / fine_sintable[rayAngle - 2700];

    xStep = (GRID_WIDTH / MAP_WIDTH);
    yStep = (GRID_HEIGHT / MAP_HEIGHT);

    rayXPosX = (((mainPlayer.xPos / (GRID_WIDTH / MAP_WIDTH)) + 1) * (GRID_WIDTH / MAP_WIDTH));
    rayXPosY = (((rayXPosX - mainPlayer.xPos) * (fine_tantable[rayAngle - 2700])) / BIT_16) + mainPlayer.yPos;
    rayYPosY = (((mainPlayer.yPos / (GRID_HEIGHT / MAP_HEIGHT)) + 1) * (GRID_HEIGHT / MAP_HEIGHT));
    rayYPosX = (((rayYPosY - mainPlayer.yPos) * (fine_tantable[900 - (rayAngle - 2700)]) / BIT_16)) + mainPlayer.xPos;

    // calculate inital ray length to first map border
    rayLengthX = ((rayXPosX - mainPlayer.xPos) * BIT_16) / fine_sintable[(rayAngle - 2700) + 900];
    rayLengthY = ((rayYPosY - mainPlayer.yPos) * BIT_16) / fine_sintable[rayAngle - 2700];
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
        // drawLine(playerCenterPixelX, playerCenterPixelY, (rayXPosX * (MAP_WIDTH * TILE_SIZE)) / BIT_16, (rayXPosY * (MAP_HEIGHT * TILE_SIZE)) / BIT_16, color);

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
        // CASColor color;
        // color.asShort = 0x0AA8;
        // drawLine(playerCenterPixelX, playerCenterPixelY, (rayXPosX * (MAP_WIDTH * TILE_SIZE)) / BIT_16, (rayXPosY * (MAP_HEIGHT * TILE_SIZE)) / BIT_16, color);

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

        RayData rayData;
        rayData.finalXPos = rayXPosX;
        rayData.finalYPos = rayXPosY;
        rayData.length = rayLengthX;
        rayData.hitType = mapInfo;
        rayData.raySide = 0;

        return rayData;
      }
    } 
    else
    {
        // CASColor color;
        // color.asShort = 0xA800;
        // drawLine(playerCenterPixelX, playerCenterPixelY, (rayYPosX * (MAP_WIDTH * TILE_SIZE)) / BIT_16, (rayYPosY * (MAP_HEIGHT * TILE_SIZE)) / BIT_16, color);

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
        // CASColor color;
        // color.asShort = 0xA800;
        // drawLine(playerCenterPixelX, playerCenterPixelY, (rayYPosX * (MAP_WIDTH * TILE_SIZE)) / BIT_16, (rayYPosY * (MAP_HEIGHT * TILE_SIZE)) / BIT_16, color);

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

        RayData rayData;
        rayData.finalXPos = rayYPosX;
        rayData.finalYPos = rayYPosY;
        rayData.length = rayLengthY;
        rayData.hitType = mapInfo;
        rayData.raySide = 1;

        return rayData;
      }
    } 
  } 
}

void castAllRays(RayData* rayData)
{
  for(uint16_t x = 0; x < (DISPLAY_WIDTH - 20); x++)
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
      case 2:
        color.asShort = 0x0000;
        color.red = 0b11111;
        color.green = 0b111111;
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

  uint16_t playerPixelX = (mainPlayer.xPos * (MAP_WIDTH * TILE_SIZE)) / BIT_16;
  uint16_t playerPixelY = (mainPlayer.yPos * (MAP_HEIGHT * TILE_SIZE)) / BIT_16;
  uint16_t playerCenterPixelX = playerPixelX + (PLAYER_SIZE / 2);
  uint16_t playerCenterPixelY = playerPixelY + (PLAYER_SIZE / 2);

  // draw lines
  for(uint16_t x = 0; x < (DISPLAY_WIDTH - 20); x++)
  {
    CASColor color;
    color.asShort = 0x0AA8;
    drawLine(playerCenterPixelX, playerCenterPixelY, (rayData[x].finalXPos * (MAP_WIDTH * TILE_SIZE)) / BIT_16, (rayData[x].finalYPos * (MAP_HEIGHT * TILE_SIZE)) / BIT_16, color);
  }

  // draw player
  color.asShort = 0x0000;
  color.green = 0b111111;

  drawRectangle(playerPixelX, playerPixelY, PLAYER_SIZE, PLAYER_SIZE, color);

  // draw player angle line
  uint8_t angleLineSize = 15;
  drawLine(playerCenterPixelX, playerCenterPixelY, playerCenterPixelX + ((sintable[mainPlayer.angle] * angleLineSize) / BIT_16), playerCenterPixelY + ((sintable[mainPlayer.angle + 90] * angleLineSize) / BIT_16), color);
}

void draw3dField()
{
  CASColor wallColor;
  uint16_t wallHeight;
  uint32_t correctedDistance;

  // draw ceiling and floor
  wallColor.asShort = CEILING_COLOR;
  drawRectangle(10, TOP_OFFSET_3D, DISPLAY_WIDTH - 20, 104, wallColor);

  wallColor.asShort = FLOOR_COLOR;
  drawRectangle(10, TOP_OFFSET_3D + 104, DISPLAY_WIDTH - 20, 104, wallColor);

  for(uint16_t x = 0; x < (DISPLAY_WIDTH - 20); x++)
  {
    correctedDistance = (fine_sintable[((x * 2) - ((PLAYER_FOV / 2) * 10)) + 900] * (rayData[x].length / 2)) / (BIT_16 / 2);

    wallHeight = WALL_HEIGHT_CONSTANT / correctedDistance;

    int16_t rayAngle = (((mainPlayer.angle - (PLAYER_FOV / 2)) * 10) + (x * 2));
    uint32_t textureYStep = (TEXTURE_RES * BIT_16) / wallHeight;
    uint32_t textureYOffset = 0;
    uint8_t textureX = (rayData[x].raySide)? (rayData[x].finalXPos % (GRID_WIDTH / MAP_WIDTH)) / ((GRID_WIDTH / MAP_WIDTH) / TEXTURE_RES) : (rayData[x].finalYPos % (GRID_WIDTH / MAP_WIDTH)) / ((GRID_WIDTH / MAP_WIDTH) / TEXTURE_RES);

    if(wallHeight > HEIGHT_3D) 
    {
      textureYOffset = (((wallHeight - HEIGHT_3D) / 2) * textureYStep);
      wallHeight = HEIGHT_3D;
    }

    uint32_t currentTextureY = textureYOffset;

    if(rayAngle > 3599)
      rayAngle -= 3600;
    else if(rayAngle < 0)
      rayAngle += 3600;

    if(rayData[x].raySide && (rayAngle < 900 || rayAngle > 2700))
      textureX = ((TEXTURE_RES - 1) - textureX);
    else if((rayData[x].raySide == 0) && rayAngle > 1800)
      textureX = ((TEXTURE_RES - 1) - textureX);

    for(uint16_t pixelRow = 0; pixelRow < wallHeight; pixelRow++)
    {
      wallColor.asShort = textureArray[rayData[x].hitType - 1][(currentTextureY / BIT_16)][textureX];
  
      // wall side shading
      if(rayData[x].raySide)
      {
        wallColor.red = wallColor.red / 2;
        wallColor.green = wallColor.green / 2;
        wallColor.blue = wallColor.blue / 2;
      }

      // draw the wall
      drawRectangle(((DISPLAY_WIDTH - 21) - x) + 10, ((HEIGHT_3D - wallHeight) / 2) + TOP_OFFSET_3D + pixelRow, 1, 1, wallColor);    

      currentTextureY += textureYStep;
    }
  }
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
