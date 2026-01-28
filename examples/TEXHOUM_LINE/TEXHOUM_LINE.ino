#include <TEXHOUM_PAC.h>

void setup() {
  monitoring();
}

void loop() {
  Serial.print(IR1.get());
}
