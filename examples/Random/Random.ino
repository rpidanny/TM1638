#include <TM1638.h>

#define LOAD_PIN 7
#define SPEED 500

TM1638 dis(LOAD_PIN);

void setup() {
  dis.begin();
}

void loop() {
  dis.print(random(-65535, 65535), 5, 100);
  delay(SPEED);
}
