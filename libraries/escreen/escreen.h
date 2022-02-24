#ifndef ESCREEN_H
#define ESCREEN_H

#ifdef ARDUINO
#if (ARDUINO >= 100)
#include <Arduino.h>
#else
#include <WProgram.h>
#include <pins_arduino.h>
#endif
#endif

#include "escreen_image.h"
#include "escreen_spi.h"

class EScreen
{
public:
  EScreen(uint16_t din = 14, uint16_t clk = 13, uint16_t cs = 15, uint16_t cmd = 27, uint16_t rst = 26, uint16_t busy = 25);

  void clear(void);
  void setPixel(uint16_t x, uint16_t y, bool isDark);
  bool getPixel(uint16_t x, uint16_t y);
  uint16_t getSizeX(void) { return image->getSizeX(); }
  uint16_t getSizeY(void) { return image->getSizeY(); }
  void drawDisplay(void);

private:
  void step1_powerOn(void);
  void step2_initAllPinsHigh(void);
  void step3_4WireComm(void);
  void step4_panelReset(void);
  void step4b_waitWhileBusy(void);
  void step5_initCode(void);
  void step5b_waitWhileBusy(void);
  void step6_dataEntry(void);
  void step7_setStartEndPos(void);
  void step8_displayUpdateSequnceSetting(void);
  void step9_waitWhileBusy(void);
  void step10_draw(void);
  void step11_displayUpdate(void);
  void step12_waitWhileBusy(void);
  void step13_deepSleep(void);

  uint16_t pinHwReset;
  uint16_t pinBusy;

  EScreenSpi *spi;
  EScreenImage *image;
};

#endif
