#include "TM1638.h"

#include <SPI.h>

unsigned char digits[10] = {0x3f, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x6F};

TM1638::TM1638(uint8_t loadPin, uint8_t din, uint8_t clk) {
  _display_number = 8;
  _cs = loadPin;
  _din = din;
  _clk = clk;
  _SPI = ( _din | _clk) == 0;
}

bool TM1638::begin() {
  pinMode(_cs, OUTPUT);
  
  if (_SPI) {
    SPI.setBitOrder(LSBFIRST);
    SPI.begin();
  } else {
    pinMode(_clk, OUTPUT);
    pinMode(_din, OUTPUT);
  }

  setBrightness(1);
  reset();
  // for randomness
  randomSeed(random(100));
}

/**
 * Transfers a byte of data.
 * 
 * @param value Value to store in the register
 */
void TM1638::transfer(uint8_t value) {
  digitalWrite(_cs, LOW);
  if (_SPI)
    SPI.transfer(value);
  else
    shiftOut(_din, _clk, LSBFIRST, value);
  digitalWrite(_cs, HIGH);
}

/**
 * Transfers data to a address.
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
    shiftOut(_din, _clk, LSBFIRST, address);
    shiftOut(_din, _clk, LSBFIRST, value);
  }
  digitalWrite(_cs, HIGH);
}

/**
 * Writes a byte of data to a MAX7219/MAX7221's register.
 * 
 * @param address The register to load data into
 * @param value Value to store in the register
 */
void TM1638::writeToAddr(uint8_t address, uint8_t value) {
  // 0x44 puts TM1638 into write mode.
  transfer(0x44);
  transfer(address, value);
}

void TM1638::setBrightness(uint8_t brightness) {
  if (brightness > 7 ) {
    transfer(0x8f);
  } else {
    transfer(0x88 + brightness);
  }
}

void TM1638::reset() {
  transfer(0x40); // auto increment mode
  digitalWrite(_cs, LOW);
  SPI.transfer(0xc0);
  for(uint8_t i = 0; i < 16; i++)
  {
    SPI.transfer(0x00);
  }
  digitalWrite(_cs, HIGH);
}

void TM1638::clearDisplay() {
  for(uint8_t i=0; i < _display_number; i++) {
    writeToAddr(0xc0 + (i << 1), 0);
  }
}

void TM1638::printDigit(uint8_t pos, uint8_t value) {
  writeToAddr(0xc0 + (pos << 1), digits[value]);
}

void TM1638::printInt(uint16_t value) {
  clearDisplay();
  uint32_t tmp = value;
  uint8_t count = _display_number - 1;
  do{
    printDigit(count, tmp % 10);
    tmp = tmp / 10;
    count --;
  }while(tmp>0 && count > -1);
}

void TM1638::print(uint16_t value) {
  for(uint8_t i=0; i < 5; i++) {
    printInt(random(0, 65535));
    delay(8);
  }
  printInt(value);
}

void TM1638::led(uint8_t position, uint8_t status) {
  writeToAddr(0xc1 + (position << 1), status);
}

uint8_t TM1638::readButton(uint8_t position) {
  uint8_t buttons = 0;
}