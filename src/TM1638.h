#ifndef TM1638_H
#define TM1638_H

#include <SPI.h>

class TM1638 {
  public:
    TM1638(uint8_t loadPin, uint8_t din = 0, uint8_t clk = 0);
    bool begin();
    void reset();
    void setBrightness(uint8_t brightness);
    
    // Seven Segment functions
    void clearDisplay();
    void printDigit(uint8_t position, uint8_t val);
    void printInt(uint16_t value);
    void print(uint16_t value);
    
    // LED functions
    void led(uint8_t position, uint8_t status);

    // Button functions
    uint8_t readButton(uint8_t position);

  private:
    void transfer(uint8_t value);
    void transfer(uint8_t address, uint8_t value);
    void writeToAddr(uint8_t address, uint8_t value);

    bool _SPI;
    uint8_t _display_number;
    uint8_t _cs;
    uint8_t _din;
    uint8_t _clk;
};

#endif
