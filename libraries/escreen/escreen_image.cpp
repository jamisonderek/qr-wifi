#include "escreen_image.h"

/**
 * @brief clears the display (all pixels to paperwhite)
 */
void EScreenImage::clear(void)
{
  uint16_t maxY = this->getSizeY();
  uint16_t maxX = this->getSizeX() >> 3;

  for (uint16_t y = 0; y < maxY; y++)
  {
    for (uint16_t x = 0; x < maxX; x++)
    {
      image[y][x] = 0;
    }
  }
}

/**
 * @brief Sets a pixel to dark or white
 * 
 * @param x the x coordinate
 * @param y the y coordinate
 * @param isDark use true for dark, false for white.
 */
void EScreenImage::setPixel(uint16_t x, uint16_t y, bool isDark)
{
  if (isDark)
  {
    image[y][x >> 3] |= 1 << (x & 7);
  }
  else
  {
    image[y][x >> 3] &= ~(1 << (x & 7));
  }
}

/**
 * @brief Gets the state of the pixel
 * 
 * @param x the x coordinate
 * @param y the y coordinate
 * @return true if pixel is dark
 * @return false is pixel is white
 */
bool EScreenImage::getPixel(uint16_t x, uint16_t y)
{
  return (image[y][x >> 3] & (1 << (x & 7))) != 0;
}

/**
 * @brief Gets a byte worth of display data 
 * 
 * @param xSeg the x segment (8 bits)
 * @param y the y coordinate
 * @return uint8_t 8 bits of display data
 */
uint8_t EScreenImage::getByte(uint16_t xSeg, uint16_t y)
{
  return image[y][xSeg];
}
