#include "escreen_image.h"

/**
 * @brief Construct a new EScreenImage object
 * 
 * @param eScreenSizeX number of pixels in the X axis
 * @param eScreenSizeY number of pixels in the Y axis
 */
EScreenImage::EScreenImage(const uint16_t eScreenSizeX, const uint16_t eScreenSizeY) :
  eScreenSizeX(eScreenSizeX), eScreenSizeY(eScreenSizeY)
{
  this->image = new uint8_t[eScreenSizeX * eScreenSizeY >> 3];
}

/**
 * @brief Deletes EScreenImage object, freeing image memory.
 * 
 */
EScreenImage::~EScreenImage() 
{
  delete [] this->image;
  this->image = NULL;
}

/**
 * @brief clears the display (all pixels to paperwhite)
 */
void EScreenImage::clear(void)
{
  uint16_t maxY = this->getSizeY();
  uint16_t maxXBytes = this->getSizeX() >> 3;

  memset(image, 0, maxY * maxXBytes);
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
    image[getIndex(y, x >> 3)] |= 1 << (x & 7);
  }
  else
  {
    image[getIndex(y, x >> 3)] &= ~(1 << (x & 7));
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
bool EScreenImage::getPixel(uint16_t x, uint16_t y) const
{
  return (image[getIndex(y, x >> 3)] & (1 << (x & 7))) != 0;
}

/**
 * @brief Gets a byte worth of display data 
 * 
 * @param xSeg the x segment (8 bits)
 * @param y the y coordinate
 * @return uint8_t 8 bits of display data
 */
uint8_t EScreenImage::getByte(uint16_t xSeg, uint16_t y) const
{
  return image[getIndex(y, xSeg)];
}
