#include <TM1638.h>

#define LOAD_PIN 7
#define SPEED 500

TM1638 dis(LOAD_PIN);

void setup() {
  dis.begin();
}

void loop() {
  // Use this if you dont want the transition animation
  // dis.print(random(-65535, 65535));
  dis.print(random(-65535, 65535), 5, 100);
  delay(SPEED);
}
