#include "TM1638.h"

#include <SPI.h>

unsigned char digits[10] = {0x3f, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x6F};
unsigned char symbols[1] = {
  0x40 // '-'
};

/**
 * @brief Instantiates a new TM1638 class.
 * @param cs The CS / LOAD pin number.
 * @param dio The DIO pin number if SPI not used, else not required.
 * @param clk The CLK pin number if SPI not used, else not required.
 */
TM1638::TM1638(uint8_t cs, uint8_t dio, uint8_t clk) {
  _display_number = 8;
  _cs = cs;
  _dio = dio;
  _clk = clk;
  _SPI = ( _dio | _clk) == 0;
}

/**
 * @brief Setups the Pins, SPI and Display
 */
void TM1638::begin() {
  pinMode(_cs, OUTPUT);
  
  if (_SPI) {
    SPI.setBitOrder(LSBFIRST);
    SPI.begin();
  } else {
    pinMode(_clk, OUTPUT);
    pinMode(_dio, OUTPUT);
  }

  setBrightness(0x01);
  reset();
  // for randomness
  randomSeed(random(100));
}

/**
 * @brief Transfers a byte of data.
 * 
 * @param value Value to store in the register
 */
void TM1638::transfer(uint8_t value) {
  digitalWrite(_cs, LOW);
  if (_SPI)
    SPI.transfer(value);
  else
    shiftOut(_dio, _clk, LSBFIRST, value);
  digitalWrite(_cs, HIGH);
}

/**
 * @brief Transfers data to a address.
 * 
 * @param address The register to load data into
 * @param value Value to store in the register
 */
void TM1638::transfer(uint8_t address, uint8_t value) {
  digitalWrite(_cs, LOW);
  if (_SPI) {
    SPI.transfer(address);
    SPI.transfer(value);
  } else {
    shiftOut(_dio, _clk, LSBFIRST, address);
    shiftOut(_dio, _clk, LSBFIRST, value);
  }
  digitalWrite(_cs, HIGH);
}

/**
 * @brief Writes a byte of data to a TM1638's register.
 * 
 * @param address The register to load data into
 * @param value Value to store in the register
 */
void TM1638::writeToAddr(uint8_t address, uint8_t value) {
  // 0x44 puts TM1638 into write mode.
  transfer(0x44);
  transfer(address, value);
}

/**
 * @brief Set the display brightness.
 * @param brightness Brightness intensity from 0 - 7
 */ 
void TM1638::setBrightness(uint8_t brightness) {
  if (brightness > 7 ) {
    transfer(0x8f);
  } else {
    transfer(0x88 + brightness);
  }
}

/**
 * @brief Resets all 16 registers of TM1638
 */
void TM1638::reset() {
  transfer(0x40); // auto increment mode
  digitalWrite(_cs, LOW);
  if (_SPI) {
    SPI.transfer(0xc0);
    for(uint8_t i = 0; i < 16; i++)
    {
      SPI.transfer(0x00);
      shiftOut(_dio, _clk, LSBFIRST, 0x00);
    }
  } else {
    shiftOut(_dio, _clk, LSBFIRST, 0xc0);    
    for(uint8_t i = 0; i < 16; i++)
    {
      shiftOut(_dio, _clk, LSBFIRST, 0x00);
    }
  }
  digitalWrite(_cs, HIGH);
}

/**
 * @brief Clear all digits of 7Segment Display.
 */
void TM1638::clearDisplay() {
  for(uint8_t i=0; i < _display_number; i++) {
    writeToAddr(0xc0 + (i << 1), 0);
  }
}

/**
 * @brief Writes raw byte to display.
 * @param pos The display to print on.
 * @param value 8-bit Data.
 */
void TM1638::write(uint8_t pos, uint8_t value) {
  writeToAddr(0xc0 + (pos << 1), value);
};

/**
 * @brief Print a digit.
 * @param pos The display to print on.
 * @param value The digit to print.
 */
void TM1638::print(uint8_t pos, uint8_t value) {
  writeToAddr(0xc0 + (pos << 1), digits[value]);
}

// TODO: support signed integer
/**
 * @brief Prints a number to the display.
 * @param value An integer to.
 */
void TM1638::print(int value) {
  clearDisplay();
  bool neg = (value < 0);
  uint32_t tmp = abs(value);
  uint8_t count = _display_number - 1;
  do{
    print(count, tmp % 10);
    tmp = tmp / 10;
    count --;
  }while(tmp>0 && count > -1);
  // Display '-' sign
  if (neg) {
    count++;
    write(count, symbols[0]);
  }
}

// TODO: support signed integer
/**
 * @brief Prints a number with animation.
 * @param value An integer.
 * @param count How many random numbers to print.
 * @param ms How long to display random numbers.
 */
void TM1638::print(int value, uint8_t count, uint8_t ms) {
  uint8_t interval = ms / count;
  for(uint8_t i=0; i < count; i++) {
    print(random(0, 65535));
    delay(interval);
  }
  print(value);
}

/**
 * @brief LED Control
 * @param position The position of the LED.
 * @param status 0 for OFF, 1 for ON
 */
void TM1638::led(uint8_t position, uint8_t status) {
  writeToAddr(0xc1 + (position << 1), status);
}

// TODO: Read buttons
/**
 * @brief Read Button Values
 * @param position The button position (0 - 7)
 * @return The status of the button.
 */
uint8_t TM1638::readButton(uint8_t position) {
  uint8_t buttons = 0;
}