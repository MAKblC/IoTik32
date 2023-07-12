// Подключаем библиотеки
#include "WiFi.h"
#include "AsyncUDP.h"
#include "ESPmDNS.h"
// Определяем количество плат ESP32 в сети
#define NBOARDS 3
// Определяем номер этой платы
const unsigned int NUM = 0;
// Определяем имя платы в сети
const char* master_host = "esp32master";
// Определяем структуру данных для обмена
/*
   У всех плат проекта должна быть одинаковая
   структура! Для удобства её можно записать в
   отдельный .h файл
*/
struct multidata {
  /* Номер платы (необходим для быстрого доступа по индексу
    в массиве структур) */
  uint8_t num;
  // Текущие millis платы
  unsigned long millis;
  /* В структуру можно добавлять элементы
    например, ip-адрес текущей платы:*/
  IPAddress boardIP;
  // или показания датчика:
  uint16_t sensor;
};
// Массив структур для обмена
multidata data[NBOARDS] {0};
// Определяем название и пароль точки доступа
const char* SSID = "ssid Вашей точки доступа WiFi";
const char* PASSWORD = "пароль Вашей точки доступа WiFi";
// Определяем порт
const uint16_t PORT = 49152;
// Создаём объект UDP соединения
AsyncUDP udp;
// Определяем callback функцию обработки пакета
void parsePacket(AsyncUDPPacket packet)
{
  // Преобразуем указатель на данные к указателю на структуру
  const multidata* tmp = (multidata*)packet.data();
  // Вычисляем размер данных (ожидаем получить размер в один элемент структур)
  const size_t len = packet.length() / sizeof(data[0]);
  // Если указатель на данные не равен нулю и размер данных больше нуля...
  if (tmp != nullptr && len > 0) {
    // Записываем порядковый номер платы
    data[tmp->num].num = tmp->num;
    // Записываем текущие millis платы
    data[tmp->num].millis = tmp->millis;
    // Записываем IP адрес
    data[tmp->num].boardIP = tmp->boardIP;
    // Записываем показания датчика
    data[tmp->num].sensor = tmp->sensor;
    // Отправляем данные всех плат побайтово
    packet.write((uint8_t*)data, sizeof(data));
  }
}

void setup()
{
  // Инициируем последовательный порт
  Serial.begin(115200);
  // Инициируем WiFi
  WiFi.begin(SSID, PASSWORD);
  // Ждём подключения WiFi
  Serial.print("Подключаем к WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(100);
  }
  Serial.println();
  // Записываем адрес текущей платы в элемент структуры
  data[NUM].boardIP = WiFi.localIP();
  if (!MDNS.begin(master_host)) {
    Serial.println(data[NUM].boardIP);
  }
  // Инициируем сервер
  if (udp.listen(PORT)) {
    // вызываем callback функцию при получении пакета
    udp.onPacket(parsePacket);
  }
  Serial.println("Сервер запущен.");
}

void loop()
{
  // Записываем текущие millis в элемент массива, соответствующий данной плате
  data[NUM].millis = millis();
  // Записываем показания датчика (для демонстрации это просто millis / 10)
  data[NUM].sensor = millis() / 10;
  // Выводим значения элементов в последовательный порт
  for (size_t i = 0; i < NBOARDS; i++) {
    Serial.print("IP адрес платы: ");
    Serial.print(data[i].boardIP);
    Serial.print(", порядковый номер: ");
    Serial.print(data[i].num);
    Serial.print(", текущие millis: ");
    Serial.print(data[i].millis);
    Serial.print(", значение датчика: ");
    Serial.print(data[i].sensor);
    Serial.print("; ");
    Serial.println();
  }
  Serial.println("----------------------------");
  delay(1000);
}
