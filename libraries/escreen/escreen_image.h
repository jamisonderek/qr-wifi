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

class WriteData
{
public:
  virtual void write(uint8_t data) = 0;
};

class EScreenImage
{
public:
  EScreenImage(uint16_t eScreenSizeH, uint16_t eScreenSizeW, uint16_t offsetY = 1, uint16_t virtualY = 22);
  ~EScreenImage();

  void clear(void);
  void setPixel(uint16_t x, uint16_t y, bool isDark);
  bool getPixel(uint16_t x, uint16_t y) const;

  uint16_t getScreenWidth(void) const;
  uint16_t getScreenHeight(void) const;

  void sendImage(WriteData* writer) const;

private:
  uint8_t *image;
  const uint16_t eScreenSizeH;
  const uint16_t eScreenSizeW;
  const uint16_t offsetY;
  const uint16_t virtualY;

  uint16_t getIndex(uint16_t x, uint16_t y) const { return x * ( eScreenSizeH >> 3) + (y >> 3); }
};

#endif

