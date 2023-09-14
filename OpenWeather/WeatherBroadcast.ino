#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
// Вводим имя и пароль точки доступа
const char* ssid = "XXXXXXX";
const char* password = "XXXXXXXXXXXXX";
/*  Запрос на текущую погоду по координатам широты и долготы
    В примере: район Санкт-Петебрурга "Новая Голландия"
    59.9386 / 30.3141
*/
const String endpoint = "http://api.openweathermap.org/data/2.5/weather?lat=59.9386&lon=30.3141&APPID=";
// Введите API-ключ
const String key = "XXXXXXXXXXXXXXXXXXXXXXXXXXX";
// система мер - метрическая
const String units = "&units=metric";

void handleReceivedMessage(String message) {
  StaticJsonDocument<1500> doc;    // Выделен объем памяти для JSON-объекта
  DeserializationError error = deserializeJson(doc, message);
  // проверка
  if (error) {
    Serial.print(F("deserializeJson() failed: "));
    Serial.println(error.c_str());
    return;
  }

  Serial.println();
  Serial.println("----- Данные от OPENWEATHER ----");

  const char* name = doc["name"];
  Serial.print("Город: ");
  Serial.println(name);

  int humidity = doc["main"]["humidity"];
  Serial.print("Влажность: ");
  Serial.print(humidity);
  Serial.println("%");

  int temperature = doc["main"]["temp"];
  Serial.print("Температура: ");
  Serial.print(temperature);
  Serial.println("С");

  int feels_like = doc["main"]["feels_like"];
  Serial.print("Ощущается как: ");
  Serial.print(feels_like);
  Serial.println("С");

  int pressure = doc["main"]["pressure"];
  Serial.print("Давление: ");
  Serial.print(pressure);
  Serial.println("гПа");

  Serial.println("------------------------------");
}

void setup() {
  Serial.begin(115200);
  delay(5000);
  // подключаемся к Wi-Fi сети
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Соединяемся с Wi-Fi..");
  }
  Serial.println("Соединение с Wi-Fi установлено");
}

void loop() {
  // выполняем проверку подключения к беспроводной сети
  if ((WiFi.status() == WL_CONNECTED)) { 
    // создаем объект для работы с HTTP
    HTTPClient http;
    // подключаемся к веб-странице OpenWeatherMap с указанными параметрами
    http.begin(endpoint + key + units);
    int httpCode = http.GET();  // Делаем запрос
    // проверяем успешность запроса
    if (httpCode > 0) { 
      // выводим ответ сервера
      String payload = http.getString();
      Serial.println(httpCode);
      //Serial.println(payload);
      handleReceivedMessage(payload);
    }
    else {
      Serial.println("Ошибка HTTP-запроса");
    }

    http.end(); 
  }
  delay(30000); // 1 раз в 30 секунд
}
