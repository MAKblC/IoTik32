#include <WiFi.h>
#include "Adafruit_MQTT.h"
#include "Adafruit_MQTT_Client.h"
#define MY_SSID "XXXXXXXXXXXX" //имя сети
#define MY_PASS "XXXXXXXXXXX" //пароль сети
#define ADA_SERVER "io.adafruit.com"
#define ADA_SERVERPORT 1883
#define ADA_USERNAME "XXXXXXXXXX" //юзернейм на сайте Adafruit
#define ADA_KEY "XXXXXXXXXXX" //ключ, который сохранили ранее

#define button1 LED_BUILTIN //feed светодиода
#define sensor1 Hall //feed датчика Холла

WiFiClient client;
Adafruit_MQTT_Client mqtt(&client, ADA_SERVER, ADA_SERVERPORT, ADA_USERNAME, ADA_KEY);
Adafruit_MQTT_Subscribe button1 = Adafruit_MQTT_Subscribe(&mqtt, ADA_USERNAME "/feeds/LED_BUILTIN"); //не задубьте вставить свой feed вместо "LED_BUILTIN"
Adafruit_MQTT_Publish sensor1 = Adafruit_MQTT_Publish(&mqtt, ADA_USERNAME "/feeds/Hall"); //не задубьте вставить свой feed вместо "Hall"

#define LED 4 // пин светодиода ЙоТик 32А, для 32В - 18

void MQTT_connect();

void setup() { //определяем светодиод
  Serial.begin(115200);
  delay(10);
  pinMode(LED, OUTPUT);

  //подключение к WiFi
  Serial.println(); Serial.println();
  Serial.print("Connecting to ");
  Serial.println(MY_SSID);
  WiFi.begin(MY_SSID, MY_PASS);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println();
  Serial.println("WiFi connected");
  Serial.println("IP address: "); Serial.println(WiFi.localIP());
  mqtt.subscribe(&button1); //меняем на название из Feeds
}

uint32_t x = 0;

void loop() { //проверка Adafruit
  MQTT_connect();

  Adafruit_MQTT_Subscribe *subscription;
  int i = hallRead();
  sensor1.publish(i);
  
  while ((subscription = mqtt.readSubscription(5000))) {
    if (subscription == &button1) { //”***” меняем на название из Feeds
      Serial.print(F("Got: "));
      Serial.println((char *)button1.lastread);
      if (!strcmp((char*) button1.lastread, "ON"))
      {
        digitalWrite(LED, HIGH); //включение светодиода
      }
      else {}

      if (!strcmp((char*) button1.lastread, "OFF")) //”***” меняем на название из Feeds
      {
        digitalWrite(LED, LOW); //отключение светодиода
      }
      else {}
    }
  }
}

void MQTT_connect() { //инициализация MQTT протокола

  int8_t ret;
  if (mqtt.connected()) {
    return;

  }

  Serial.print("Connecting to MQTT... ");

  uint8_t retries = 3;

  while ((ret = mqtt.connect()) != 0) {
    Serial.println(mqtt.connectErrorString(ret));
    Serial.println("Retrying MQTT connection in 10 seconds...");
    mqtt.disconnect();
    delay(10000);
    retries--;
  }
  Serial.println("MQTT Connected!");
}
