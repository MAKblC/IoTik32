#include <esp_now.h>
#include <WiFi.h>

// Замените на MAC-адрес получателя
uint8_t broadcastAddress[] = { 0x01, 0x22, 0xc6, 0x83, 0x69, 0x1c };

// Переменная, которая хранит данные о состоянии связи
String success;

// Структура для отправки
// Должна совпадать со структурой приемника
typedef struct struct_message {
  int hall;
  float example;
  String text;
} struct_message;

// Структура для переменных
struct_message outgoingReadings;

// переменная для хранения параметров приёмника
esp_now_peer_info_t peerInfo;

// Функция при отправке
void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  if (status == 0) {
    success = "Успех отправки :)";
  } else {
    success = "Неудача при отправке :(";
  }
}


void setup() {
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  // запуск ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("Ошибка инициализации ESP-NOW");
    return;
  }

  // Как только протокол запущен, функция сообщает, была ли доставка успешной
  esp_now_register_send_cb(OnDataSent);

  // Указываем получателя
  memcpy(peerInfo.peer_addr, broadcastAddress, 6);
  peerInfo.channel = 0;
  peerInfo.encrypt = false;
  if (esp_now_add_peer(&peerInfo) != ESP_OK) {
    Serial.println("Не смог добавить получателя");
    return;
  }
}

void loop() {

  // Формирование данных в структуре
  outgoingReadings.example = random(10, 20);
  outgoingReadings.hall = hallRead();
  outgoingReadings.text = "I am #1";

  // Отправить сообщение
  esp_err_t result = esp_now_send(broadcastAddress, (uint8_t *)&outgoingReadings, sizeof(outgoingReadings));

  if (result == ESP_OK) {
    Serial.println("Успешно отправлено");
  } else {
    Serial.println("Ошибка при передаче");
  }
  delay(10000);
}
