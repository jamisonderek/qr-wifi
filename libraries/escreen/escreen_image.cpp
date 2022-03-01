#include "escreen_image.h"

/**
 * @brief Construct a new EScreenImage object
 * 
 * @param eScreenSizeW The width of the display (number of pixels in X axis)
 * @param eScreenSizeH The height of the display (number of pixels in Y axis)
 * @param offsetY The number of offset bytes in Y axis before rendering on display 
 * @param virtualY The total number of bytes in the Y axis, including offscreen data
 */
EScreenImage::EScreenImage(uint16_t eScreenSizeW, uint16_t eScreenSizeH, uint16_t offsetY, uint16_t virtualY) :
  eScreenSizeW(eScreenSizeW),
  eScreenSizeH(eScreenSizeH),
  offsetY(offsetY),
  virtualY(virtualY)
{
  this->image = new uint8_t[eScreenSizeW * eScreenSizeH >> 3];
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
  memset(image, 0, (eScreenSizeH >> 3) * eScreenSizeW);
}

/**
 * @brief Sets a pixel to dark or white
 * 
 * @param x the x coordinate (amount right)
 * @param y the y coordinate (amount down)
 * @param isDark use true for dark, false for white.
 */
void EScreenImage::setPixel(uint16_t x, uint16_t y, bool isDark)
{
  uint8_t pixelMask = 1 << (y & 7);
  if (isDark)
  {
    image[getIndex(x, y)] |= pixelMask;
  }
  else
  {
    image[getIndex(x, y)] &= ~pixelMask;
  }
}

/**
 * @brief Gets the state of the pixel
 * 
 * @param x the x coordinate (amount right)
 * @param y the y coordinate (amount down)
 * @return true if pixel is dark
 * @return false is pixel is white
 */
bool EScreenImage::getPixel(uint16_t x, uint16_t y) const
{
  uint8_t pixelMask = 1 << (y & 7);
  return (image[getIndex(x, y)] & pixelMask) == pixelMask;
}

/**
 * @brief Gets the width of the display (number of pixels in X axis)
 * 
 * @return The width of the display (pixels in X axis)
 */
uint16_t EScreenImage::getScreenWidth(void) const
{
  return eScreenSizeW;
}

/**
 * @brief Gets the height of the display (number of pixels in Y axis)
 * 
 * @return The height of the display (pixels in Y axis)
 */
uint16_t EScreenImage::getScreenHeight(void) const
{
  return eScreenSizeH;
}

/**
 * @brief Sends an image in Y,X order including padding.
 */
void EScreenImage::sendImage(WriteData* writer) const
{
  uint16_t maxY = (eScreenSizeH >> 3) + offsetY;

  uint8_t* imageData = image;
  for (uint16_t x = 0; x < eScreenSizeW; x++)
  {
    for (uint16_t y = 0; y < virtualY; y++)
    {
      if (y < offsetY || y >= maxY)
      {
        // We can write any byte value since it is off-screen.
        writer->write(0);
      }
      else
      {
        writer->write(*imageData++);
      }
    }
  }
}
