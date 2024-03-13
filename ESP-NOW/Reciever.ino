#include <esp_now.h>
#include <WiFi.h>

// тип структуры для входящего сообщения
typedef struct struct_message {
  int hall;
  float example;
  String text;
} struct_message;

// Структура для входящих сообщениий
struct_message incomingReadings;

// обработка входящего сообщения
void OnDataRecv(const uint8_t *mac, const uint8_t *incomingData, int len) {
  memcpy(&incomingReadings, incomingData, sizeof(incomingReadings));
  int incomingHall = incomingReadings.hall;
  Serial.println("Присланное значение датчика Холла: " + String(incomingHall));
  float incomingFloat = incomingReadings.example;
  Serial.println("Присланное случайное значение от 10 до 20: " + String(incomingFloat));
  String incomingText = incomingReadings.text;
  Serial.println("Присланный текст: " + incomingText);
}

void setup() {
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);

  if (esp_now_init() != ESP_OK) {
    Serial.println("Ошибка инициализации ESP-NOW");
    return;
  }

  esp_now_register_recv_cb(OnDataRecv);
}

void loop() {
}
