// Библиотека для управления компонентами в составе программно-аппаратного комплекса TEXHOUM
// Library for managing components within the TEXHUM hardware-software platform

// Авторы библиотеки: Коркин Максим, Пятов Антон

#ifndef TEXHOUM_PAC_h
#define TEXHOUM_PAC_h
#include <Arduino.h>

class MOTOR {
public:
  MOTOR(byte dig, byte pwm);       // Инициализация (pin digital, pin pwm)
  void init(byte dig, byte pwm);
  void start(int speed);           // Запуск мотора (скорость от -100 до 100)
  void stop();                     // Остановка мотора
  void move(int speed, int time);  // Запуск мотора на заданное время (скорость от -100 до 100, время в миллисекундах)
private:
  byte _pin_DIG;  //	Номер цифрового пина
  byte _pin_PWM;  //	Номер ШИМ пина
  int _speed;     //  Текущая заданная скорость
};

class LINE {
public:
	LINE(byte pin_INPUT);      //	Инициализация (analog pin)
	byte _pin_INPUT;           //	Номер вывода, подключённого к оптопаре
	int16_t get();             //	Определение уровня освещённости
	int16_t min_series_value;  // Значение серии устойчивых минимальных значений
	int16_t max_series_value;  // Значение серии устойчивых максимальных значений
	int16_t current_value;     // Текущее значение
	int16_t previous_value;    // Предыдущее значение
	uint16_t series_length;    // Длина текущей серии значений
	int16_t border;            // Пороговое значение между чёрным и белым
private:
};

class	SERVO {
  public:
    SERVO(byte pin_DIG);	                //  инициализация (pin dig)
    void rotate(int degree);		        //  поворот сервомотора (угол от 0 до 180)
    void hold(int degree, int time);            //  угол поворота в градусах от 0 до 180 и время в секундах
  private:
    byte _pin_SERVO;		                //  номер пина
};

class	US{
	public:
	        US(byte trig, byte echo);	//	Инициализация (pin TRIG, pin ECHO)
		long get();		//	Определение расстояния
		void limit(uint16_t pulseinTimeout); // Порог дальности действия в см, по умолчанию 150
	private:
		long returnDelay();		//	Время ECHO
		byte _pin_TRIG;		//	Номер вывода подключённого к TRIG
		byte _pin_ECHO;		//	Номер вывода подключённого к ECHO
		uint16_t _pulseinTimeout;
};

void monitoring();  // выводит в монитор порта текущие показания датчиков, значение на черном, значение на белом, и пороговое значение

extern MOTOR motor1;
extern MOTOR motor2;
#if defined(ARDUINO_AVR_MEGA2560) || defined(__AVR_ATmega2560__)
extern MOTOR motor3;
extern MOTOR motor4;
#endif

extern LINE IR1;
extern LINE IR2;
extern LINE IR3;
extern LINE IR4;
extern LINE IR5;

extern SERVO servo1;
#if defined(ARDUINO_AVR_MEGA2560) || defined(__AVR_ATmega2560__)
extern SERVO servo2;
#endif

extern US US1;
#if defined(ARDUINO_AVR_MEGA2560) || defined(__AVR_ATmega2560__)
extern US US2;
extern US US3;
#endif

#endif
