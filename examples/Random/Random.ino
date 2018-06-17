#include <TM1638.h>

#define LOAD_PIN 7
#define DIO 11
#define CLK 13

#define SPEED 100

TM1638 disp(LOAD_PIN, DIO, CLK);

void setup() {
  disp.begin();
}

void loop() {
  // Use this if you dont want the transition animation
  // dis.print(random(-65535, 65535));
  disp.print(random(-65535, 65535), 5, 100);
  delay(SPEED);
}
