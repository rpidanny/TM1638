#include <TM1638.h>

#define LOAD_PIN 7
#define DIO 11
#define CLK 13

TM1638 disp(LOAD_PIN, DIO, CLK);

void setup() {
  disp.begin();
  disp.setBrightness(5);
}

void loop() {
  disp.setLed(disp.readButton());
  delay(10);
}
