// больше материалов: http://docs.thinger.io/arduino/
#define THINGER_SERIAL_DEBUG // подробное отслеживание процессов
#include <ThingerESP32.h>
/* введите логин и пароль от Wi-Fi, Имя пользователя,
  ID устройства и пароль устройства,
  пин светодиода (Йотик 32А - 4/ ЙоТик 32В - 18)
*/
#define ssid              "wifilogin"
#define PASSWORD          "wifipass"
#define USERNAME          "username"
#define DEVICE_ID         "deviceid"
#define DEVICE_CREDENTIAL "devicepass"
#define LED               4 
// добавление экземпляра с вышеуказанными настройками
ThingerESP32 thing(USERNAME, DEVICE_ID, DEVICE_CREDENTIAL); 

void setup() {
  Serial.begin(115200);
  pinMode(LED, OUTPUT); // режим выхода для светодиода
  thing.add_wifi(ssid, PASSWORD); // запуск
  // пример для управления исполнтельным устройством
  thing["LED"] << digitalPin(LED);
  // пример для считывания данных с датчика
  thing["Hall"] >> [](pson & out) {
    out = hallRead();
  };
}
void loop() {
  thing.handle();
}
