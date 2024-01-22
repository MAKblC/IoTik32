#include <WiFi.h>
#include <HTTPClient.h>
#include <Wire.h>

/********************************************************************************/
//Логин/пароль WiFI, ID скрипта
const char * ssid = "XXXXXXXX";
const char * password = "XXXXXXXXXXXXXXX";
String GOOGLE_SCRIPT_ID = "AKfXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXdi";

const int sendInterval = 5000; // период запроса
/********************************************************************************/

WiFiClientSecure client;

void setup() {
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.println("Старт");
  Serial.print("Подсоединяюсь");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("Готово!");
}

void loop() {
  spreadsheet_comm();
  delay(sendInterval);
}

void spreadsheet_comm(void) {
  HTTPClient http;
  String url = "https://script.google.com/macros/s/" + GOOGLE_SCRIPT_ID + "/exec?read";
  //   Serial.print(url);
  Serial.print("Создаю запрос");
  http.begin(url.c_str());
  http.setFollowRedirects(HTTPC_STRICT_FOLLOW_REDIRECTS);
  int httpCode = http.GET();
  String payload;
  if (httpCode > 0) { // Если не ошибка в запросе
    payload = http.getString();
    Serial.println(httpCode);
    Serial.println(payload);
  }
  else {
    Serial.println("Ошибка при HTTP запросе");
  }
  http.end();
}
