#include "arduino_secrets.h"
#include "thingProperties.h"
#define LED 18 //пин встроенного в плату светодиода, ЙоТик 32 А – 4 пин
void setup() {
  // Инициируем монитор порта:
  Serial.begin(9600);
  delay(1500);
  pinMode(LED, OUTPUT); //указываем светодиод как выходной порт
  // Функция из thingProperties.h
  initProperties();

  // Соединение Arduino IoT Cloud
  ArduinoCloud.begin(ArduinoIoTPreferredConnection);
  setDebugMessageLevel(2);
  ArduinoCloud.printDebugInfo();
}

void loop() {
  ArduinoCloud.update();
}

// Функция вызывается каждый раз, когда меняется статус кнопки
void onBlinkingChange()  {
  if (blinking) {
    digitalWrite(LED, HIGH); //при переключении кнопки на on включается    светодиод
  }
  else {
    digitalWrite(LED, LOW); //на off отключается
  }
}
