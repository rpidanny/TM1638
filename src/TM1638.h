#ifndef TM1638_H
#define TM1638_H

#include <SPI.h>

/*!
 * \class TM1638
 * 
 * \brief TM1638 Library.
 * 
 * Small library to be used with TM1638 based boards.
 * 
 * \version 0.0.1
 * 
 * \date 2018/06/16
 * 
 * */
class TM1638 {
  public:
    TM1638(uint8_t cs, uint8_t dio = 0, uint8_t clk = 0);
    void begin();
    void reset();
    void setBrightness(uint8_t brightness);
    
    // Seven Segment functions
    void clearDisplay();
    void write(uint8_t position, uint8_t value);
    void print(int value);
    void print(uint8_t position, uint8_t value);
    void print(int value, uint8_t count, uint8_t ms);
    
    // LED functions
    void setLed(uint8_t value);
    void setLed(uint8_t position, uint8_t value);

    // Button functions
    uint8_t readButton();
    uint8_t readButton(uint8_t position);

  private:
    void transfer(uint8_t value);
    void transfer(uint8_t address, uint8_t value);
    void writeToAddr(uint8_t address, uint8_t value);

    bool _SPI;
    uint8_t _display_number;
    uint8_t _cs;
    uint8_t _dio;
    uint8_t _clk;
};

#endif
