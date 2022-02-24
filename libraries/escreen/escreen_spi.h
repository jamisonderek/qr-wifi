#ifndef ESCREEN_SPI_H
#define ESCREEN_SPI_H

#ifdef ARDUINO
#if (ARDUINO >= 100)
#include <Arduino.h>
#else
#include <WProgram.h>
#include <pins_arduino.h>
#endif
#endif

#include "escreen_image.h"

class EScreenSpi
{
public:
  EScreenSpi(uint16_t din, uint16_t clk, uint16_t cs, uint16_t cmd);

  void initAllPinsHigh(void);

  void send(uint8_t cmd);
  void send(uint8_t cmd, uint8_t data1);
  void send(uint8_t cmd, uint8_t data1, uint8_t data2);
  void send(uint8_t cmd, uint8_t data1, uint8_t data2, uint8_t data3);
  void send(uint8_t cmd, uint8_t data1, uint8_t data2, uint8_t data3, uint8_t data4);
  void send_image(uint8_t cmd, EScreenImage* image);

private:
  void spi_write(uint8_t d);
  void spi_write_backwards(uint8_t d);

  uint16_t pinData;
  uint16_t pinClock;
  uint16_t pinChipSelect;
  uint16_t pinDataCommand;
};

#endif