#include "escreen_spi.h"

/**
 * @brief Construct a new EScreenSpi object
 * 
 * @param din Pin number for data-in
 * @param clk Pin number for clock
 * @param cs Pin number for chip-select
 * @param cmd Pin number for choosing command/data
 */
EScreenSpi::EScreenSpi(uint16_t din, uint16_t clk, uint16_t cs, uint16_t cmd) :
  pinData(din),
  pinClock(clk),
  pinChipSelect(cs),
  pinDataCommand(cmd)
{
  Serial.print("Data is pin ");
  Serial.println(this->pinData);

  Serial.print("Clock is pin ");
  Serial.println(this->pinClock);

  Serial.print("Chip Select is pin ");
  Serial.println(this->pinChipSelect);

  Serial.print("Data/Command is pin ");
  Serial.println(this->pinDataCommand);

  pinMode(this->pinData, OUTPUT);
  pinMode(this->pinClock, OUTPUT);
  pinMode(this->pinChipSelect, OUTPUT);
  pinMode(this->pinDataCommand, OUTPUT);
}

/**
 * @brief Deletes EScreenSpi object, setting all pins to INPUT so they can
 * be safely connected to VCC or GND.
 *
 */
EScreenSpi::~EScreenSpi()
{
  pinMode(this->pinData, INPUT);
  pinMode(this->pinClock, INPUT);
  pinMode(this->pinChipSelect, INPUT);
  pinMode(this->pinDataCommand, INPUT);
}

/**
 * @brief Sets the output pins to HIGH
 * 
 */
void EScreenSpi::initAllPinsHigh(void)
{
  digitalWrite(this->pinDataCommand, HIGH); // DATA
  digitalWrite(this->pinChipSelect, HIGH);  // CHIP NOT SELECTED
  digitalWrite(this->pinData, HIGH);
  digitalWrite(this->pinClock, HIGH);
}

/**
 * @brief Sends an SPI command
 * 
 * @param cmd The id of the command to send 
 */
void EScreenSpi::send(uint8_t cmd)
{
  digitalWrite(pinChipSelect, LOW);
  digitalWrite(pinDataCommand, LOW); // COMMAND
  spi_write(cmd);
  digitalWrite(pinChipSelect, HIGH);
}

/**
 * @brief Sends an SPI command and 1 byte of data
 * 
 * @param cmd The id of the command to send
 * @param data1 The byte of data to send
 */
void EScreenSpi::send(uint8_t cmd, uint8_t data1)
{
  digitalWrite(pinChipSelect, LOW);
  digitalWrite(pinDataCommand, LOW); // COMMAND
  spi_write(cmd);
  digitalWrite(pinDataCommand, HIGH); // DATA
  spi_write(data1);
  digitalWrite(pinChipSelect, HIGH);
}

/**
 * @brief Sends an SPI command and 2 bytes of data
 * 
 * @param cmd The id of the command to send
 * @param data1 The first byte of data to send
 * @param data2 The second byte of data to send
 */
void EScreenSpi::send(uint8_t cmd, uint8_t data1, uint8_t data2)
{
  digitalWrite(pinChipSelect, LOW);
  digitalWrite(pinDataCommand, LOW); // COMMAND
  spi_write(cmd);
  digitalWrite(pinDataCommand, HIGH); // DATA
  spi_write(data1);
  spi_write(data2);
  digitalWrite(pinChipSelect, HIGH);
}

/**
 * @brief Sends an SPI command and 3 bytes of data
 * 
 * @param cmd The id of the command to send
 * @param data1 The first byte of data to send
 * @param data2 The second byte of data to send
 * @param data3 The third byte of data to send
 */
void EScreenSpi::send(uint8_t cmd, uint8_t data1, uint8_t data2, uint8_t data3)
{
  digitalWrite(pinChipSelect, LOW);
  digitalWrite(pinDataCommand, LOW); // COMMAND
  spi_write(cmd);
  digitalWrite(pinDataCommand, HIGH); // DATA
  spi_write(data1);
  spi_write(data2);
  spi_write(data3);
  digitalWrite(pinChipSelect, HIGH);
}

/**
 * @brief Sends an SPI command and 4 bytes of data
 * 
 * @param cmd The id of the command to send
 * @param data1 The first byte of data to send
 * @param data2 The second byte of data to send
 * @param data3 The third byte of data to send
 * @param data4 The fourth byte of data of send
 */
void EScreenSpi::send(uint8_t cmd, uint8_t data1, uint8_t data2, uint8_t data3, uint8_t data4)
{
  digitalWrite(pinChipSelect, LOW);
  digitalWrite(pinDataCommand, LOW); // COMMAND
  spi_write(cmd);
  digitalWrite(pinDataCommand, HIGH); // DATA
  spi_write(data1);
  spi_write(data2);
  spi_write(data3);
  spi_write(data4);
  digitalWrite(pinChipSelect, HIGH);
}

/**
 * @brief Sends an SPI command and the data from an image
 * 
 * @param cmd The id of the command to send
 * @param image The image to send
 */
void EScreenSpi::send_image(uint8_t cmd, const EScreenImage* image)
{
  digitalWrite(pinChipSelect, LOW);
  digitalWrite(pinDataCommand, LOW); // COMMAND
  spi_write(cmd);
  digitalWrite(pinDataCommand, HIGH); // DATA

  uint16_t offsetX = 1;
  uint16_t virtualX = 22;
  uint16_t maxY = image->getSizeY();
  uint16_t maxX = image->getSizeX() + offsetX;

  for (uint16_t y = 0; y < maxY; y++)
  {
    for (uint16_t x = 0; x < virtualX; x++)
    {
      if (x < offsetX || x >= maxX)
      {
        spi_write_backwards(0);
      }
      else
      {
        spi_write_backwards(image->getByte(x - offsetX, y));
      }
    }
  }

  digitalWrite(pinChipSelect, HIGH);
}

/**
 * @brief Writes a byte of SPI data
 * 
 * @param d The byte to write
 */
void EScreenSpi::spi_write(uint8_t d)
{
  digitalWrite(pinChipSelect, LOW);
  for (int i = 0; i < 8; ++i)
  {
    digitalWrite(pinData, (d & 0x80) == 0 ? LOW : HIGH);
    d = d << 1;
    digitalWrite(pinClock, LOW);
    digitalWrite(pinClock, HIGH); // BIT READ ON RISING EDGE
  }
  digitalWrite(pinChipSelect, HIGH);
}

/**
 * @brief Writes a byte of image data (sends LSB & inverts bits)
 * 
 * @param d The byte to write
 */
void EScreenSpi::spi_write_backwards(uint8_t d)
{
  digitalWrite(pinChipSelect, LOW);
  // We write from LSB to MSB (so that lowest bit is near top of display)
  for (int i = 0; i < 8; ++i)
  {
    // NOTE: We invert the HIGH/LOW so that an "on bit" is black ink.
    digitalWrite(pinData, (d & 0x1) == 0 ? HIGH : LOW);
    d = d >> 1;
    digitalWrite(pinClock, LOW);
    digitalWrite(pinClock, HIGH); // BIT READ ON RISING EDGE
  }
  digitalWrite(pinChipSelect, HIGH);
}
