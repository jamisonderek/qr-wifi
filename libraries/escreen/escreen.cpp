// The spec for the 2.9inch e-paper v2 can be found on waveshare web site at:
// https://www.waveshare.com/w/upload/7/79/2.9inch-e-paper-v2-specification.pdf

// This code is based on the flowchart in 4.2 Reference Program Code & the section 5 Command table.
// Minor changes to the flowchart are as follows:
//  Slight delay after step 4 & step 5; otherwise screen update didn't happen.
//  Step 6 - we use 0x01 which is "(Y-,X+)" not "(Y+,X-)" as described in chart.
//  Step 8 - we don't do the SPI(0x1A, value)
//  Step 10 - we sent 296*22 = 6512 bytes not 5000.
//  Step 11 - added SPI(0x20) that is missing from the diagram (it just says "Master Activation:")
// https://www.waveshare.com/wiki/2.9inch_e-Paper_Module defines the wiring for ESP32 as...
//   Typical usage as... EScreen(14, 13, 15, 27, 26, 25)
//
// You can use ANY pins you want, we do GPIO bit-bang, so doesn't need to be SPI specific pins.

#include "EScreen.h"

/**
 * @brief Construct a new EScreen object for 2.9inch e-Paper module (128x296)
 * 
 * @param din The pin for data-in; labeled "din" on screen.
 * @param clk The pin for clock; labeled "clk" on screen.
 * @param cs The pin for chip-select; labeled "cs" on screen.
 * @param cmd The pin for selecting command/data; labeled "dc" on screen.
 * @param rst The pin for hardware-reset; labeled "rst" on screen.
 * @param busy The pin for determining if the display is busy; labeled "busy" on screen.
 * @param eScreenSizeW The width of the screen (number of pixels in the X axis)
 * @param eScreenSizeH The height of the screen (number of pixels in the Y axis)
 */
EScreen::EScreen(uint16_t din, uint16_t clk, uint16_t cs, uint16_t cmd, uint16_t rst, uint16_t busy, uint16_t eScreenSizeW, uint16_t eScreenSizeH) :
  pinHwReset(rst),
  pinBusy(busy)
{
  this->spi = new EScreenSpi(din, clk, cs, cmd);

  pinMode(this->pinHwReset, OUTPUT);
  pinMode(this->pinBusy, INPUT);
  
  this->image = new EScreenImage(eScreenSizeW, eScreenSizeH);
}

/**
 * @brief Deletes EScreen object, setting all pins to INPUT so they can
 * be safely connected to VCC or GND.
 *
 */
EScreen::~EScreen()
{
  delete this->image;
  this->image = NULL;

  pinMode(this->pinBusy, INPUT);
  pinMode(this->pinHwReset, INPUT);

  delete this->spi;
  this->spi = NULL;
}

/**
 * @brief Step 1 - Power on device
 * 
 */
void EScreen::step1_powerOn(void)
{
  // Nothing to do here.  (Apply power to VCC)
}

/**
 * @brief Step 2 - Initialize the state of the pins (to HIGH)
 * 
 */
void EScreen::step2_initAllPinsHigh(void)
{
  this->spi->initAllPinsHigh();
  digitalWrite(this->pinHwReset, HIGH); // NOT RESET
}

/**
 * @brief Step 3 - We are using 4-wire SPI configuration
 * 
 */
void EScreen::step3_4WireComm(void)
{
  // Nothing to do here. (We use 4-wire SPI)
}

/**
 * @brief Step 4 - Send hardware and software reset
 * 
 */
void EScreen::step4_panelReset(void)
{
  // HARDWARE RESET PULSE
  digitalWrite(this->pinHwReset, LOW);
  delay(10); // 10ms delay
  digitalWrite(this->pinHwReset, HIGH);
  delay(10); // 10ms delay

  // Software reset
  this->spi->send(0x12);
}

/**
 * @brief Step 4b - Testing showed we need to add some delay
 * 
 */
void EScreen::step4b_waitWhileBusy(void)
{
  do
  {
    delay(10); // How long is a good wait time -- 10ms?
  } while (digitalRead(this->pinBusy) == HIGH);
}

/**
 * @brief Step 5 - Send device initialization SPI command
 * 
 */
void EScreen::step5_initCode(void)
{
  // Driver Output Control
  this->spi->send(0x01, 0x27, 0x01, 0x00);
}

/**
 * @brief Step 5b - Testing showed we need to add some delay
 * 
 */
void EScreen::step5b_waitWhileBusy(void)
{
  do
  {
    delay(10); // How long is a good wait time -- 10ms?
  } while (digitalRead(this->pinBusy) == HIGH);
}

/**
 * @brief Step 6 - Send data entry mode setting (Y-,X+)
 * 
 */
void EScreen::step6_dataEntry(void)
{
  // 0x01 = (Y dec, X inc) **
  // &0x4==0 = address incremented in X direction first.
  this->spi->send(0x11, 0x01);
}

/**
 * @brief Step 7 - Set RAMX and RAMY positions
 * 
 */
void EScreen::step7_setStartEndPos(void)
{
  // 2.9 inch screen display is 296 by 128 pixels.

  // Set X position [not sure why we use 0x18 (dec 24) here?]
  this->spi->send(0x44, 0x0, 0x18);

  // Set Y position [start 295, end 0]
  this->spi->send(0x45, 0x27, 0x01, 0x00, 0x00); // 0x127+1=296d
}

/**
 * @brief Step 8 - Send display update control 2 + master activation
 * 
 */
void EScreen::step8_displayUpdateSequnceSetting(void)
{
  // Load LUT from OTP? ("Load LUT with DISPLAY Mode 1")
  this->spi->send(0x22, 0xb1);

  // Master activation.
  this->spi->send(0x20);
}

/**
 * @brief Step 9 - Wait until the display no longer
 * 
 */
void EScreen::step9_waitWhileBusy(void)
{
  do
  {
    delay(50); // How long is a good wait time -- 50ms?
  } while (digitalRead(this->pinBusy) == HIGH);
}

/**
 * @brief Step 10 - Set RAM addresses & write image.
 * 
 */
void EScreen::step10_draw(void)
{
  // Set RAMX address
  this->spi->send(0x4e, 0x00);

  // Set RAMY address
  this->spi->send(0x4f, 0x27, 0x01);

  // Write BW RAM
  this->spi->send_image(0x24, this->image);

  // Set RAMX address
  this->spi->send(0x4e, 0x00);

  // Set RAMY address
  this->spi->send(0x4f, 0x27, 0x01);

  // Write RED RAM
  this->spi->send_image(0x26, this->image);
}

/**
 * @brief Step 11 - Send Display Update Control 2
 * 
 */
void EScreen::step11_displayUpdate(void)
{
  // Display with DISPLAY Mode 1
  this->spi->send(0x22, 0xc7);

  // Master Activation
  this->spi->send(0x20);
}

/**
 * @brief Step 12 - Wait until the display no longer
 * 
 */
void EScreen::step12_waitWhileBusy(void)
{
  do
  {
    delay(50); // How long is a good wait time -- 50ms?
  } while (digitalRead(this->pinBusy) == HIGH);
}

/**
 * @brief Step 13 - Put device in deep sleep mode
 * 
 */
void EScreen::step13_deepSleep(void)
{
  this->spi->send(0x10, 0x01);
}

/**
 * @brief Runs through all the steps to display full screen image
 * 
 */
void EScreen::drawDisplay(void)
{
  int stepDelay = 1;
  this->step1_powerOn();
  this->step2_initAllPinsHigh();
  this->step3_4WireComm();
  this->step4_panelReset();
  this->step4b_waitWhileBusy();
  this->step5_initCode();
  this->step5b_waitWhileBusy();
  this->step6_dataEntry();
  this->step7_setStartEndPos();
  this->step8_displayUpdateSequnceSetting();
  this->step9_waitWhileBusy();
  this->step10_draw();
  this->step11_displayUpdate();
  this->step12_waitWhileBusy();
  this->step13_deepSleep();
}

/**
 * @brief Clears the memory buffer, setting all pixels to paperwhite.
 * 
 */
void EScreen::clear(void)
{
  this->image->clear();
}

/**
 * @brief Sets a pixel to dark or paperwhite
 * 
 * @param x the x coordinate (amount right)
 * @param y the y coordinate (amount down)
 * @param isDark use true for dark, false for paperwhite.
 */
void EScreen::setPixel(uint16_t x, uint16_t y, bool isDark)
{
  this->image->setPixel(x, y, isDark);
}

/**
 * @brief Gets the state of the pixel
 * 
 * @param x the x coordinate (amount right)
 * @param y the y coordinate (amount down)
 * @return true if pixel is dark
 * @return false is pixel is white
 */
bool EScreen::getPixel(uint16_t x, uint16_t y) const
{
  return this->image->getPixel(x, y);
}

/**
 * @brief Gets the width of the display (number of pixels in X axis)
 */
uint16_t EScreen::getPixelWidth(void) const 
{
  return image->getScreenWidth();
}

/**
 * @brief Gets the height of the display (number of pixels in Y axis).
 */
uint16_t EScreen::getPixelHeight(void) const
{
  return image->getScreenHeight();
}
