#define LED 18 // пин встроенного светодиода ЙоТик 32В (для ЙоТика 32А - пин 4)
#include "IRremote.h" // библиотека для пультов ДУ
IRrecv irrecv(27); // пин 27 для ИК-приемника
decode_results results; // переменная для принимаемого сигнала
int velocity = 500; // начальная скорость моргания
void setup() {
  Serial.begin(115200); // Инициализация последовательного порта
  irrecv.enableIRIn(); // запускаем прием
  pinMode(LED, OUTPUT); // пин на выход
}
void loop() {
  blinking(); // функция моргания светодиода
  if ( irrecv.decode( &results )) { // если данные пришли
    if (results.value == 0xFFA857) { // если это кнопка "медленее"
      velocity = velocity + 50;
    }
    if (results.value == 0xFFE01F) { // если это кнопка "быстрее"
      velocity = velocity - 50;
    }
    irrecv.resume(); // принимаем следующую команду
    velocity = constrain(velocity, 0, 1000); // ограничение скорости
  }
}
void blinking() { // функция моргания
  digitalWrite(LED, HIGH); // включение светодиода
  delay(velocity);
  digitalWrite(LED, LOW); // выключение светодиода
  delay(velocity);
}
