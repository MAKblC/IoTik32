#include <WiFi.h>
#include <HTTPClient.h>

const char* MY_SSID = "XXXXXXX"; //имя сети
const char* PASSWORD = "XXXXXXXXX"; //пароль сети

// настраиваем название сервера с событием из апплета (заменяем "Event" своим названием) и ключом из Documentation вместо XXX
const char* serv_name = "https://maker.ifttt.com/trigger/Event/with/key/XXXXXXXXXXXXXXXXXXX";

void setup() {
  Serial.begin(115200);
  Serial.setDebugOutput(true);
  Serial.println();

  WiFi.begin(MY_SSID, PASSWORD);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");

  Serial.print("Msg: 'http://");
  Serial.print(WiFi.localIP());
}

void loop() {

  delay(500);
  Serial.println("Go...");

  while (Serial.available() == 0) {}
  String testStr = Serial.readString();
  testStr.trim();
  {

    // проверка статуса сети
    if (WiFi.status() == WL_CONNECTED)
    {
      HTTPClient http;

      http.begin(serv_name);
      http.addHeader("Content-Type", "application/x-www-form-urlencoded");

      // определяем сообщение из Монитора порта, которое будет отправлено вместе с HTTP POST запросом
      String http_Req_Data = "value1=" + testStr;

      // отправка HTTP POST запроса
      int http_Resp_Code = http.POST(http_Req_Data);

      Serial.print("HTTP Code: ");
      Serial.println(http_Resp_Code);

    }
  }
}
