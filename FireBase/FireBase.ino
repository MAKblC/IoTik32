#include <Arduino.h>
#if defined(ESP32)
#include <WiFi.h>
#elif defined(ESP8266)
#include <ESP8266WiFi.h>
#endif
#include <Firebase_ESP_Client.h>

#include "addons/TokenHelper.h"
#include "addons/RTDBHelper.h"

#define WIFI_SSID "XXXXXXX" // логин WiFi
#define WIFI_PASSWORD "XXXXXXXXXX" // пароль WiFi
#define API_KEY "XXXXXXXXXXXXXXXXXXXXXXX" // API-ключ
#define DATABASE_URL "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX" // URL базы данных

#define LED 4 // пин светодиода, (для ЙоТика 32В - 18)

//объект БД
FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig config;

// переменные для алгоритма
unsigned long sendDataPrevMillis = 0;
int count = 0;
int intValue;
bool led_state;
float floatValue;
bool signupOK = false;


void setup() {
  pinMode(LED, OUTPUT);
  Serial.begin(115200);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to Wi-Fi");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(300);
  }
  Serial.println();
  Serial.print("Connected with IP: ");
  Serial.println(WiFi.localIP());
  Serial.println();

  //  подтягиваем ключ
  config.api_key = API_KEY;

  // подтягиваем ссылку на БД
  config.database_url = DATABASE_URL;

  // подключение к БД
  if (Firebase.signUp(&config, &auth, "", "")) {
    Serial.println("ok");
    signupOK = true;
  }
  else {
    Serial.printf("%s\n", config.signer.signupError.message.c_str());
  }

  // подключение функции вызова в случае долгой обработки команды
  config.token_status_callback = tokenStatusCallback; 

  Firebase.begin(&config, &auth);
  Firebase.reconnectWiFi(true);

}

void loop() {
  // смотрим на значения
  if (Firebase.ready() && signupOK && (millis() - sendDataPrevMillis > 15000 || sendDataPrevMillis == 0)) {
    sendDataPrevMillis = millis();
    
    // установка значения 5
    if (Firebase.RTDB.setInt(&fbdo, "test/int", 5)) {
      Serial.println("OKAY");
      Serial.println("FROM: " + fbdo.dataPath());
      Serial.println("TYPE: " + fbdo.dataType());
    }
    // считывание значения float-переменной
    if (Firebase.RTDB.getFloat(&fbdo, "/test/float")) {
      if (fbdo.dataType() == "float") {
        floatValue = fbdo.floatData();
        Serial.println(floatValue);
      }
    }
    else {
      Serial.println(fbdo.errorReason());
    }
  }
  
  // считывание значения для состояния светодиода
  if (Firebase.RTDB.getBool(&fbdo, "/test/bool")) {
    if (fbdo.dataType() == "boolean") {
      led_state = fbdo.boolData();
      digitalWrite(LED,  led_state);
    }
  }

}
