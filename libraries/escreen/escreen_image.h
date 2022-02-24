#ifndef ESCREEN_IMAGE_H
#define ESCREEN_IMAGE_H

#ifdef ARDUINO
#if (ARDUINO >= 100)
#include <Arduino.h>
#else
#include <WProgram.h>
#include <pins_arduino.h>
#endif
#endif

class EScreenImage
{
public:
  EScreenImage(const uint16_t eScreenSizeX, const uint16_t eScreenSizeY);
  ~EScreenImage();

  void clear(void);
  void setPixel(uint16_t x, uint16_t y, bool isDark);
  bool getPixel(uint16_t x, uint16_t y) const;
  uint8_t getByte(uint16_t xSeg, uint16_t y) const;

  uint16_t getSizeX(void) const { return eScreenSizeX; }
  uint16_t getSizeY(void) const { return eScreenSizeY; }

private:
  uint8_t *image;
  const uint16_t eScreenSizeX;
  const uint16_t eScreenSizeY;

  uint16_t getIndex(uint16_t y, uint16_t xSeg) const { return xSeg * eScreenSizeY + y; }
};

#endif
