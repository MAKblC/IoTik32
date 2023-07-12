// Подключаем библиотеки
#include "WiFi.h"
#include "AsyncUDP.h"
#include "ESPmDNS.h"
// Определяем количество плат
#define NBOARDS 3
// Определяем номер этой платы
const unsigned int NUM = 2;
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
/* Определяем имена для mDNS */
// для ведущей платы
const char* master_host = "esp32master";
// приставка имени ведомой платы
const char* slave_host = "esp32slave";
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
  const multidata* tmp = (multidata*)packet.data();
  // Вычисляем размер данных
  const size_t len = packet.length() / sizeof(data[0]);
  // Если адрес данных не равен нулю и размер данных больше нуля...
  if (tmp != nullptr && len > 0) {
    // Проходим по элементам массива
    for (size_t i = 0; i < len; i++) {
      // Если это не ESP на которой выполняется этот скетч
      if (i != NUM) {
        // Обновляем данные массива структур
        data[i].num = tmp[i].num;
        data[i].millis = tmp[i].millis;
        data[i].boardIP = tmp[i].boardIP;
        data[i].sensor = tmp[i].sensor;
      }
    }
  }
}
void setup()
{
  // Добавляем номер этой платы в массив структур
  data[NUM].num = NUM;
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
  // Инициируем mDNS с именем "esp32slave" + номер платы
  if (!MDNS.begin(String(slave_host + NUM).c_str())) {
    Serial.println("не получилось инициировать mDNS");
  }
  // Узнаём IP адрес платы с UDP сервером
  IPAddress server = MDNS.queryHost(master_host);
  // Если удалось подключиться по UDP
  if (udp.connect(server, PORT)) {
    Serial.println("UDP подключён");
    // вызываем callback функцию при получении пакета
    udp.onPacket(parsePacket);
  }
}

void loop()
{
  // Записываем текущие millis в элемент массива, соответствующий данной плате
  data[NUM].millis = millis();
  // Записываем показания датчика (для демонстрации это просто millis / 10)
  data[NUM].sensor = millis() / 10;
  // Отправляем данные этой платы побайтово
  udp.broadcastTo((uint8_t*)&data[NUM], sizeof(data[0]), PORT);
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
