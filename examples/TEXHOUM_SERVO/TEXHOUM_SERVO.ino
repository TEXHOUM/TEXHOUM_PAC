#include "TEXHOUM_SERVO.h"


void setup() {
}

void loop() {
  servo1.rotate(0);
  delay(1000);
  servo1.rotate(180);
  delay(1000);
  servo1.hold(90, 10.1);
  delay(1000);
}
