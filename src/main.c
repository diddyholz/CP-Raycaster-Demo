#include <cas-sdk/display.h>
#include <cas-sdk/input/key-input.h>
#include <cas-sdk/input/touch-input.h>

#define MAP_HEIGHT  8
#define MAP_WIDTH   8
#define TILE_SIZE   40
#define PLAYER_SIZE 5

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

  drawRectangle((uint16_t)(mainPlayer.xPos * (float)TILE_SIZE), (uint16_t)(mainPlayer.yPos * (float)TILE_SIZE), PLAYER_SIZE, PLAYER_SIZE, color);^

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

void main() __attribute__((section(".text.main")));
