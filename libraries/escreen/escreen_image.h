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

const uint16_t ESCREENSIZEX = 128;
const uint16_t ESCREENSIZEY = 296;

class EScreenImage
{
public:
  EScreenImage(void) { }
  
  void clear(void);
  void setPixel(uint16_t x, uint16_t y, bool isDark);
  bool getPixel(uint16_t x, uint16_t y);
  uint8_t getByte(uint16_t xSeg, uint16_t y);

  uint16_t getSizeX(void) { return ESCREENSIZEX; }
  uint16_t getSizeY(void) { return ESCREENSIZEY; }

private:
  uint8_t image[ESCREENSIZEY][ESCREENSIZEX>>3];
};

#endif
