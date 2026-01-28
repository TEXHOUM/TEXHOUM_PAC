#include <TEXHOUM_US.h>

void setup() {
  Serial.begin(9600);
  //US3.limit(200); //изменение максимальной дальности
}

void loop() {

  // вывод показаний каждого из трёх датчиков
  Serial.print("US1: ");
  Serial.print(US1.get()); 
  Serial.print(" cm ");

  Serial.print("US2: ");
  Serial.print(US2.get());
  Serial.print(" cm ");

  Serial.print("US3: ");
  Serial.print(US3.get());
  Serial.println(" cm ");

  delay(200);
}
