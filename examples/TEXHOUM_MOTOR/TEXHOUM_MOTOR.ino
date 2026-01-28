#include <TEXHOUM_MOTOR.h>


// Пример программы для езды вперёд в течение 3 секунд.
// An example program for driving forward for 3 seconds.

void setup() {
  start_forward(250);
  delay(3000);
  stopp();
}

void loop() {

}

// Объявление функций необязательно. Это упрощение кода.
// Declaring functions is optional. This is a simplification of the code.
void start_forward(int speed) {
  motor1.start(speed);
  motor2.start(speed);
  motor3.start(-speed);
  motor4.start(-speed);
}

void stopp() {
  motor1.stop();
  motor2.stop();
  motor3.stop();
  motor4.stop();
}

void move_forward(int speed, int time) {
  motor1.move(speed, time);
  motor2.move(speed, time);
  motor3.move(-speed, time);
  motor4.move(-speed, time);
}