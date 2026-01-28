#include "Arduino.h"
#include "TEXHOUM_PAC.h"
#define IR1_pin A0
#define IR2_pin A1
#define IR3_pin A2
#define IR4_pin A3
#define IR5_pin A4
#define NUM_SENSORS 5  // количество сенсоров в массиве объектов
#define ERROR 2       // погрешность измерения +-n из-за которой серия не прерывается
#define MIN_SERIES 5   // минимальная длина, при которой набор близких чисел считается серией

// конструктор
MOTOR::MOTOR(byte dig, byte pwm) {
  init(dig, pwm);
}

// инициализация мотора
void MOTOR::init(byte dig, byte pwm) {
  _pin_DIG = dig;
  _pin_PWM = pwm;
  pinMode(_pin_DIG, OUTPUT);  // инициализация цифрового пина
  pinMode(_pin_PWM, OUTPUT);  // инициализация ШИМ пина
  digitalWrite(_pin_DIG, LOW);
  analogWrite(_pin_PWM, 0);
}

// Запуск мотора
void MOTOR::start(int speed) {
  _speed = constrain(speed, -100, 100);             // ограничиваем скорость speed допустимым диапазоном и записываем её как параметр мотора _speed
  int voltage = map(_speed, -100, 100, -255, 255);  // переводим скорость в вольтаж, пропорционально перенося значение между диапазонами
  if (voltage >= 0) {
    digitalWrite(_pin_DIG, LOW);
    analogWrite(_pin_PWM, voltage);
  }
  if (voltage < 0) {
    digitalWrite(_pin_DIG, HIGH);
    analogWrite(_pin_PWM, (255 + voltage));
  }
}

// Остановка мотора
void MOTOR::stop() {
  digitalWrite(_pin_DIG, LOW);
  analogWrite(_pin_PWM, LOW);
}

// Запуск мотора на заданное время
void MOTOR::move(int speed, int time) {
  _speed = constrain(speed, -100, 100);             // ограничиваем скорость speed допустимым диапазоном
  int voltage = map(_speed, -100, 100, -255, 255);  // переводим скорость в вольтаж, пропорционально перенося значение между диапазонами
  // запускаем моторы
  if (voltage >= 0) {
    digitalWrite(_pin_DIG, LOW);
    analogWrite(_pin_PWM, voltage);
  }
  if (voltage < 0) {
    digitalWrite(_pin_DIG, HIGH);
    analogWrite(_pin_PWM, (255 + voltage));
  }
  delay(time);  // ждём
  //останавливаем моторы
  digitalWrite(_pin_DIG, LOW);
  analogWrite(_pin_PWM, LOW);
}

// инициализация модуля
LINE::LINE(byte pin_INPUT) {
  _pin_INPUT = pin_INPUT;  // инициализация аналогового входа
}

// определение уровня освещённости
int16_t LINE::get() {
  return analogRead(_pin_INPUT);  // считываем уровень напряжения на входном пине
}

void monitoring() {  // выводит в монитор порта текущие показания датчиков, значение на черном, значение на белом, и пороговое значение
  Serial.begin(250000);

  LINE IRSS[] = { IR1, IR2, IR3, IR4, IR5 };  // создаём массив копий объектов чтобы не прописывать одни команды для каждого

  for (int i = 0; i < NUM_SENSORS; ++i) {                    // NUM_SENSORS указано в define сверху
    IRSS[i].current_value = analogRead(IRSS[i]._pin_INPUT);  // записываем текущий уровень напряжения на входных пинах
    IRSS[i].previous_value = IRSS[i].current_value;          // приравниваем предыдущее к текущему
    IRSS[i].series_length = 1;                               // обновляем длину текущей серии значений
    // для каждого датчика устанавливаем минимальное значение на максимум и максимальное - на минимум
    IRSS[i].min_series_value = 1023;
    IRSS[i].max_series_value = 0;
  }

  while (true) {
    for (int i = 0; i < NUM_SENSORS; ++i) {                                                                   // NUM_SENSORS указано в define сверху
      IRSS[i].current_value = analogRead(IRSS[i]._pin_INPUT);                                                 // записываем текущий уровень напряжения на входных пинах
      if (abs(IRSS[i].previous_value - IRSS[i].current_value) <= ERROR) {                                     // проверка, что текущее значение не отличается от предыдущего в пределах погрешности
        IRSS[i].series_length++;                                                                              // увеличиваем длину текущей серии значений на 1
      } else {                                                                                                // если текущее значение отличается от предыдущего сильнее погрешности, то есть серия прервана
        if ((IRSS[i].previous_value > IRSS[i].max_series_value) and (IRSS[i].series_length >= MIN_SERIES)) {  // если число, с которого началась серия, больше значения серии устойчивых максимальных значений и длина текущей серии больше пороговой
          IRSS[i].max_series_value = IRSS[i].previous_value;                                                  // число, с которого началась серия, становится новым значением серии устойчивых максимальных значений
          // рассчитываем пороговое значение для датчика с учётом логарифмической зависимости
          if ((IRSS[i].min_series_value < 1023) and (IRSS[i].max_series_value > 0)) {  // если обе серии были обновлены
            uint32_t result1 = (uint32_t)IRSS[i].min_series_value * (uint32_t)IRSS[i].max_series_value;
            float result = sqrt(result1);
            IRSS[i].border = (int)round(result);  // округляем до целых и записываем порог
          }
        }
        if ((IRSS[i].previous_value < IRSS[i].min_series_value) and (IRSS[i].series_length >= MIN_SERIES)) {  // если число, с которого началась серия, меньше значения серии устойчивых минимальных значений и длина текущей серии больше пороговой
          IRSS[i].min_series_value = IRSS[i].previous_value;                                                  // число, с которого началась серия, становится новым значением серии устойчивых минимальных значений
          if ((IRSS[i].min_series_value < 1023) and (IRSS[i].max_series_value > 0)) {                         // если обе серии были обновлены
            // рассчитываем пороговое значение для датчика с учётом логарифмической зависимости
            uint32_t result1 = (uint32_t)IRSS[i].min_series_value * (uint32_t)IRSS[i].max_series_value;
            float result = sqrt(result1);
            IRSS[i].border = (int)round(result);  // округляем до целых и записываем порог
          }
        }
        IRSS[i].series_length = 1;                       // обновляем длину текущей серии значений
        IRSS[i].previous_value = IRSS[i].current_value;  // обновляем предыдущее значение
      }
    }
    /*Serial.println(F("+-----+--------------+------------------+-------------------+-----------+"));
    Serial.println(F("|  #  | Текущее      |   Минимальное    |   Максимальное    |   Порог   |"));
    Serial.println(F("|     | Current Val  |   Min (black)    |   Max (white)     |   Border  |"));
    Serial.println(F("+-----+--------------+------------------+-------------------+-----------+"));

    for (int i = 0; i < NUM_SENSORS; ++i) {
      Serial.print(F("| "));
      Serial.print("IR");
      Serial.print(i+1);
      Serial.print(F("  |"));

      Serial.print(IRSS[i].current_value);
      Serial.print(F("\t     | "));

      Serial.print(IRSS[i].min_series_value);
      Serial.print(F("\t        | \t"));

      Serial.print(IRSS[i].max_series_value);
      Serial.print(F("\t    | \t"));

      Serial.print(IRSS[i].border);
      Serial.println(F("\t|"));
    }

    Serial.println(F("+-----+--------------+------------------+-------------------+-----------+"));
    Serial.println();
    */
    
    for (int i = 0; i < NUM_SENSORS; ++i) {
      Serial.print(IRSS[i].current_value);
      Serial.print(" ");
      Serial.print(IRSS[i].min_series_value);
      Serial.print(" ");
      Serial.print(IRSS[i].max_series_value);
      Serial.print(" ");
      Serial.print(IRSS[i].border);
      Serial.println(" ");
    }
    Serial.println("---------------");
  }
}

// инициализация модуля
SERVO::SERVO(byte pin_DIG) {
  _pin_SERVO=pin_DIG; pinMode(_pin_SERVO, OUTPUT);   //  инициализация сигнального пина
  digitalWrite(_pin_SERVO, LOW);
}
// поворот сервомотора на заданный угол
void SERVO::rotate(int degree){
  for (int i=0; i<20; i++) {
    int pulse = map(degree, 0, 180, 500, 2500);       // перевод граудсов в длительность импульса
    digitalWrite(_pin_SERVO, HIGH);
    delayMicroseconds(pulse);
    digitalWrite(_pin_SERVO, LOW);
    delay(20);                                        // 50 Hz
  }
}

void SERVO::hold(int degree, int time) {             //  угол поворота в градусах от 0 до 180 и время в секундах
  int t = 0;
  while (t*50 < time*1000) {
    int pulse = map(degree, 0, 180, 500, 2500);      // перевод граудсов в длительность импульса
    digitalWrite(_pin_SERVO, HIGH);
    delayMicroseconds(pulse);
    digitalWrite(_pin_SERVO, LOW);
    delay(20);                                       // 50 Hz
    t+=1;
  }
}

// инициализация модуля
US::US(byte trig, byte echo) {
  _pin_TRIG=trig; pinMode(_pin_TRIG, OUTPUT);			// инициализация TRIG
  _pin_ECHO=echo; pinMode(_pin_ECHO, INPUT);			// инициализация ECHO
  _pulseinTimeout = 9445;                         // значение по умолчанию - 150 см ((150+12)*58.3)
  digitalWrite(_pin_TRIG, LOW);
}
// определение расстояния
long US::get(){
  long time = 0;
  byte k = 0;
  while ((time <= 0) and (k < 5)) { // пока не получено ненулевое значение и количество проходов меньше пяти
    k += 1;
    time=returnDelay(); // Считываем длительность импульса на выводе ECHO
    if (k >= 5) {       // Если за 5 проходов стабильно получаем 0, значит время равно таймауту pulsein
      time = 0;
    }    
  }					
  return (long)(time / 58.3f + 0.5f); // округление до ближайшего целого и вывод результата в см
}
// определение длительности
long US::returnDelay(){
  digitalWrite(_pin_TRIG, HIGH);					// Устанавливаем высокий уровень на TRIG
  delayMicroseconds(10);					// Ждём 10 мкс
  digitalWrite(_pin_TRIG, LOW);					// Устанавливаем низкий уровень на TRIG
  return pulseIn(_pin_ECHO, HIGH, _pulseinTimeout);			// Фиксируем вермя возвращения сигнала
}

void US::limit(uint16_t pulseinTimeout) { // Изменение максимального времени ожидания возврата сигнала для увеличения дальности действия
  _pulseinTimeout = (unsigned long)((pulseinTimeout + 12) * 58.3f + 0.5f); // 12 - компенсация дополнительной задержки
}

// моторы, согласно схеме
MOTOR motor1(4, 5);
MOTOR motor2(7, 6);

#if defined(ARDUINO_AVR_MEGA2560) || defined(__AVR_ATmega2560__)
MOTOR motor3(22, 44);
MOTOR motor4(14, 45);
#endif

// датчики, согласно схеме
LINE IR1(IR1_pin);
LINE IR2(IR2_pin);
LINE IR3(IR3_pin);
LINE IR4(IR4_pin);
LINE IR5(IR5_pin);

SERVO servo1(2);
#if defined(ARDUINO_AVR_MEGA2560) || defined(__AVR_ATmega2560__)
SERVO servo2(13);
#endif

// датчики, согласно схеме
US US1(9, 8);
#if defined(ARDUINO_AVR_MEGA2560) || defined(__AVR_ATmega2560__)
US US2(27, 29);
US US3(31, 33);
#endif
