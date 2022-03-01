/**
 * @file BoundingBox.ino
 * @author Derek Jamison (derek@jamisoncreations.com)
 * @brief Draws a bounding box and a plus symbol on the screen.
 * @version 0.1
 * @date 2022-02-05
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#include "escreen.h"

#define PIN_BUILTIN_LED 2

EScreen screen(14, 13, 15, 27, 26, 25);

void blinkSlow()
{
  for (int i = 0; i < 3; i++)
  {
    digitalWrite(PIN_BUILTIN_LED, HIGH);
    delay(500);
    digitalWrite(PIN_BUILTIN_LED, LOW);
    delay(500);
  }
}

void fillImage()
{
  screen.clear();

  int maxX = screen.getPixelWidth();
  int maxY = screen.getPixelHeight();
  for (int x = 0; x < maxX; x++)
  {
    for (int y = 0; y < maxY; y++)
    {
      boolean value = false;
      
      // Bounding box
      if (y == 0) value = true;
      if (y == maxY-1) value = true;
      if (x == 0) value = true;
      if (x == maxX-1) value = true;

      // Plus symbol.
      if ((y >= 60) && (y < 65) && (x > 150) && (x < 155)) value = true;
      if ((y >= 64) && (y < 70) && (x > 140) && (x < 165)) value = true;
      if ((y >= 70) && (y < 75) && (x > 150) && (x < 155)) value = true;

      screen.setPixel(x, y, value);
    }
  }
}

void setup()
{
  Serial.begin(115200);

  pinMode(PIN_BUILTIN_LED, OUTPUT);

  blinkSlow();
  fillImage();
  screen.drawDisplay();
}

void loop()
{
  // put your main code here, to run repeatedly:
  digitalWrite(PIN_BUILTIN_LED, HIGH);
  delay(200);
  digitalWrite(PIN_BUILTIN_LED, LOW);
  delay(100);
}
