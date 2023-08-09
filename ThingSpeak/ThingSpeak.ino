#include <WiFi.h>
#include "ThingSpeak.h"
const char* ssid = "XXXX"; //имя сети
const char* pass = "XXXXXXXX"; //пароль сети
WiFiClient client;
unsigned long lastTime = 0;
unsigned long timerDelay = 10000;
unsigned long myChannelNumber = 1; //номер канала
const char* myWriteAPIKey = "XXXXXXXXXXXXX"; //Write API Key

void setup() {
  Serial.begin(115200);
  delay(1000);
  WiFi.mode(WIFI_STA);
  if (WiFi.status() != WL_CONNECTED) {
    Serial.print("Attempting to connect");
    while (WiFi.status() != WL_CONNECTED) {
      WiFi.begin(ssid, pass);
      delay(5000);
    }
    ThingSpeak.begin(client);
  }
 }


void loop() {
  int H_value = 0;
  H_value = hallRead();  //считывание датчика Холла
  Serial.print("H_value:");
  Serial.println(H_value);
  ThingSpeak.setField(1, H_value); //номер поля и значение 
  ThingSpeak.writeFields(my_Channel_Number, my_API_Key); //запись данных на сервер
}
