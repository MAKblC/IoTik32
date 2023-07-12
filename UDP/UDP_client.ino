// Подключаем библиотеки
#include "WiFi.h"
#include "AsyncUDP.h"
// Создаём переменную состояния кнопки
bool keyState = false;
// Определяем название и пароль точки доступа
const char *ssid = "esp32asAP";
const char *password = "password12345";
// Определяем порт
const uint16_t port = 49152;
// Создаём объект с IP-адресом точки доступа
IPAddress addr(192, 168, 4, 1);
// Создаём объект UDP соединения
AsyncUDP udp;
// Определяем callback функцию обработки пакета
void parsePacket(AsyncUDPPacket packet)
{
  // Выводи в последовательный порт все полученные данные
  Serial.write(packet.data(), packet.length());
  Serial.println();
}

void setup()
{
  // Инициируем последовательный порт
  Serial.begin(115200);
  // Устанавливаем режим работы в качестве клиента
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  // Ждём подключения WiFi
  while (WiFi.waitForConnectResult() != WL_CONNECTED) {
    Serial.print(".");
    delay(100);
  }
  // Если удалось подключится по UDP
  if (udp.connect(addr, port)) {
    Serial.println("UDP подключён");
    // вызываем callback функцию при получении пакета
    udp.onPacket(parsePacket);
  }
  // Если подключение не удалось
  else {
    Serial.println("UDP не подключён");
    // Входим в бесконечный цикл
    while (1) {
      delay(1000);
    }
  }
}

void loop()
{
  delay(50);
  // Если соединение WiFi прервано
  if (WiFi.status() != WL_CONNECTED) {
    // Вызываем функцию setup(), для повторного подключения
    setup();
  }
  // Записываем состояние кнопки в переменную в зависимости от введённых данных в монитор порта
  int kod = Serial.parseInt(); //Читает
  uint8_t key;
  if (kod == 1) key = 1;
  if (kod == 2) key = 0;
  // Если состояние кнопки изменилось и кнопка нажата
  if (keyState == false && key == true) {
    // Посылаем состояние кнопки
    udp.broadcastTo(&key, sizeof(key), port);
    // Меняем переменную состояния кнопки
    keyState = !keyState;
  }
  // Если состояние кнопки изменилось и кнопка не нажата
  else if (keyState == true && key == false) {
    // Посылаем состояние кнопки
    udp.broadcastTo(&key, sizeof(key), port);
    // Меняем переменную состояния кнопки
    keyState = !keyState;
  }
}
