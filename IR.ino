#define LED 18 // пин встроенного светодиода ЙоТик 32В (для ЙоТика 32А - пин 4)
#include "IRremote.h" // библиотека для пультов ДУ
IRrecv irrecv(27); // пин 27 для ИК-приемника
decode_results results; // переменная для принимаемого сигнала
void setup() {
  Serial.begin(115200); // Инициализация последовательного порта
  irrecv.enableIRIn(); // запускаем прием
  pinMode(LED, OUTPUT); // пин на выход
}
void loop() {
  if ( irrecv.decode( &results )) { // если данные пришли
    Serial.println(results.value, HEX); // выводим в порт полученное значение
    digitalWrite(LED, HIGH); // моргание светодиода
    delay(100);
    digitalWrite(LED, LOW);
    irrecv.resume(); // принимаем следующую команду
  }
}
