// Подключaем библиотеки
#include <AsyncUDP.h>
#include <WiFi.h>
#include <WiFiAP.h>
// Определяем вывод светодиода
const uint8_t LED_PIN = 18; // 18 для ЙоТик32 В / 4 для ЙоТик32 А
// Определяем название и пароль точки доступа
const char* ssid = "esp32asAP";
const char* password = "password12345";
// Создаём объект UDP cоединения
AsyncUDP udp;
// Определяем порт
const uint16_t port = 49152;
// Определяем callback функцию обработки пакета
void parsePacket(AsyncUDPPacket packet)
{
  // Записываем адрес начала данных в памяти
  const uint8_t* msg = packet.data();
  // Записываем размер данных
  const size_t len = packet.length();
  // Объект для хранения состояния светодиода в строковом формате
  String state;
  // Если адрес данных не равен нулю и размер данных больше нуля...
  if (msg != NULL && len > 0) {
    // Если первый байт данных содержит 0x1
    if (msg[0] == HIGH) {
      // записываем строку в объект String
      state = "включён";
    }
    // Если первый байт данных содержит 0x0
    else if (msg[0] == LOW) {
      // записываем строку в объект String
      state = "выключен";
    }
    // Меняем состояние встроенного в плату светодиода
    digitalWrite(LED_PIN, msg[0]);
    // Отправляем данные клиенту
    packet.printf("Светодиод %s", state.c_str());
    // Выводим состояние светодиода в последовательный порт
    Serial.println("Светодиод " + state);
  }
}

void setup()
{
  // Устанавливаем режим работы вывода светодиода
  pinMode(LED_PIN, OUTPUT);
  // Инициируем последовательный порт
  Serial.begin(115200);
  Serial.println();
  Serial.println("Инициируем точку доступа WiFi");
  // Инициируем точку доступа WiFi
  WiFi.softAP(ssid, password);
  IPAddress myIP = WiFi.softAPIP();
  // Выводим IP-адрес
  Serial.print("IP-адрес точки доступа: ");
  Serial.println(myIP);
  // Инициируем сервер
  if (udp.listen(port)) {
    // При получении пакета вызываем callback функцию
    udp.onPacket(parsePacket);
  }
  Serial.println("Сервер запущен.");
}

void loop()
{
  delay(10);
}
