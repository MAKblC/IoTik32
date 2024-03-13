#include <esp_now.h>
#include <WiFi.h>

// Замените на MAC-адрес получателя
uint8_t broadcastAddress[] = { 0x24, 0x0A, 0xc4, 0x9b, 0x04, 0x08 };

// Переменная, которая хранит данные о состоянии связи
String success;

// Структура для отправки
// Должна совпадать со структурой приемника
typedef struct struct_message {
  int hall;
  float example;
  String text;
} struct_message;

// Структура для входящих сообщениий
struct_message incomingReadings;

// переменная для хранения параметров приёмника
esp_now_peer_info_t peerInfo;

// Вызов, когда данные отправили
void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  if (status == 0) {
    success = "Успех отправки :)";
  } else {
    success = "Неудача при отправке :(";
  }
}

// Вызов, когда данные получены
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
  // запуск ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("Ошибка инициализации ESP-NOW");
    return;
  }

  // Как только протокол запущен, функция сообщает, была ли доставка успешной
  esp_now_register_send_cb(OnDataSent);

  peerInfo.channel = 0;
  peerInfo.encrypt = false;

  // Указываем получателя
  memcpy(peerInfo.peer_addr, broadcastAddress, 6);
  if (esp_now_add_peer(&peerInfo) != ESP_OK) {
    Serial.println("Не смог добавить контроллер 3");
    return;
  }

  // функция сообщает, был ли приём успешным
  esp_now_register_recv_cb(OnDataRecv);
}

void loop() {
  // Отправляем данные с отправителя (sender) приемнику (reciever)
  esp_err_t result = esp_now_send(broadcastAddress, (uint8_t *)&incomingReadings, sizeof(incomingReadings));

  Serial.println(String(result));
  if (result == ESP_OK) {
    Serial.println("Успешно отправлено контроллеру 3");
  } else {
    Serial.println("Ошибка при передаче");
  }
  delay(10000);
}
