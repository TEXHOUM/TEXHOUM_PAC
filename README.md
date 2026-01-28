# Библиотека для управления компонентами в составе программно-аппаратного комплекса TEXHOUM

Данная библиотека предоставляет единый интерфейс для управления компонентами программно-аппаратного комплекса TEXHOUM.

## Подключение библиотеки

```C++
#include <TEXHOUM_PAC.h>
```

---

## Управление моторами постоянного тока (DC Motor, L298N)

Библиотека позволяет управлять моторами постоянного тока с помощью драйверов L298N.

Для управления одним мотором используются два пина:

- цифровой пин - направление вращения

- ШИМ-пин - скорость вращения

### Пример использования

```C++
motor1.start(100);   // Запуск мотора с максимальной скоростью (100)
motor2.start(-100);  // Отрицательное значение меняет направление вращения

motor1.stop();       // Остановка мотора
```

### Добавление новых моторов

```C++
MOTOR motor5(40, 41); // Создание нового мотора (PWM, DIR)
```

---

## Датчик линии TEXHOUM (аналоговый)

Библиотека позволяет работать с аналоговым датчиком линии TEXHOUM, состоящим из 5 оптических сенсоров.

Поддерживается:

- получение текущих значений освещённости

- режим калибровки с выводом данных в монитор порта

- расчёт порога между чёрной и белой поверхностью

### Получение уровня освещённости

```C++
lum = IR1.get(); // Значение освещённости от 0 до 1023
```

### Функция мониторинга (калибровка)

```C++
monitoring(); // Выводит значения всех сенсоров и порог в монитор порта
```

⚠️ **Важно:**  
Функция `monitoring()` выполняется бесконечно.  
Любой код, расположенный после её вызова, **не будет выполнен**.

Для работы функции контроллер должен быть подключён к ПК, а монитор порта в Arduino IDE — открыт.

После завершения калибровки функцию необходимо удалить или закомментировать.

---

## Управление сервомоторами

Библиотека позволяет управлять сервомоторами с помощью одного цифрового пина на каждый мотор.

### Поворот сервомотора

```C++
servo1.rotate(0);    // Поворот в положение 0°
servo1.rotate(180);  // Поворот в положение 180°
servo1.rotate(170);  // Поворот в положение 170°
```

Последняя команда приведёт к повороту сервомотора на 10° относительно предыдущего положения.

### Удержание положения

```C++
servo2.hold(90, 10); // Удержание 90° в течение 10 секунд
```

### Добавление сервомоторов

```C++
SERVO servo5(40); // Добавление нового сервомотора
```

---

## Ультразвуковой датчик расстояния (HC-SR04)

Библиотека обеспечивает работу с ультразвуковым датчиком расстояния HC-SR04.

Алгоритм работы:

1. отправка стартового импульса

2. ожидание ответного сигнала

3. преобразование времени возвращения звуковой волны в расстояние

### Получение расстояния

```C++
distance = US1.get(); // Возвращает расстояние в сантиметрах
```

### Изменение максимальной дистанции

```C++
US1.limit(200); // Установка максимальной дистанции 200 см
```

Значение порога по умолчанию — **150 см**.

---

Библиотеку разработали и поддерживают:

*   [**Антон Пятов**](https://github.com/5anton)
*   **Максим Коркин**


---

# Library for controlling components of the TEXHOUM hardware-software complex

This library объединяет several previously independent libraries into a single interface for controlling components of the TEXHOUM hardware-software complex.

## Library connection

```C++
#include <TEXHOUM_PAC.h>
```

---

## DC Motor Control (L298N)

The library allows controlling DC motors using L298N drivers.

Each motor uses two pins:

- digital pin — rotation direction

- PWM pin — rotation speed

### Usage example

```C++
motor1.start(100);   // Start motor at maximum speed (100)
motor2.start(-100);  // Negative value reverses rotation direction

motor1.stop();       // Stop motor
```

### Adding new motors

```C++
MOTOR motor5(40, 41); // Create a new motor (PWM, DIR)
```

---

## TEXHOUM Line Sensor (Analog)

This library allows working with the TEXHOUM analog line sensor consisting of 5 optical sensors.

Supported features:

- reading current light levels

- calibration mode with serial monitor output

- threshold calculation between black and white surfaces

### Reading light level

```C++
lum = IR1.get(); // Light level value from 0 to 1023
```

### Monitoring function (calibration)

```C++
monitoring(); // Outputs all sensor values and threshold to the serial monitor
```

⚠️ **Important:**  
The `monitoring()` function runs infinitely.  
Any code placed after this function **will not be executed**.

For proper operation, the controller must be connected to a PC and the Arduino IDE serial monitor must be open.

After calibration, the function must be removed or commented out.

---

## Servo Motor Control

The library allows controlling servo motors using one digital pin per motor.

### Servo rotation

```C++
servo1.rotate(0);    // Rotate to 0°
servo1.rotate(180);  // Rotate to 180°
servo1.rotate(170);  // Rotate to 170°
```

The last command rotates the servo by 10° relative to the previous position.

### Holding position

```C++
servo2.hold(90, 10); // Hold 90° position for 10 seconds
```

### Adding servos

```C++
SERVO servo5(40); // Add a new servo
```

---

## Ultrasonic Distance Sensor (HC-SR04)

This library provides support for the HC-SR04 ultrasonic distance sensor.

Operation principle:

1. sending a trigger pulse

2. waiting for the echo signal

3. converting pulse duration into distance

### Reading distance

```C++
distance = US1.get(); // Returns distance in centimeters
```

### Changing maximum distance limit

```C++
US1.limit(200); // Set maximum distance to 200 cm
```

Default distance limit is **150 cm**.

---
The library is developed and maintained by:

* [**Anton Pyatov**](https://github.com/5anton)

* **Maxim Korkin**