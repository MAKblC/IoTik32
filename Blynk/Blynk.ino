/********************************************************
  Это простой пример получения данных с датчика Холла,
  а также управления встроенным светодиодом через Blynk.Cloud
********************************************************/

// Здесь нужно вписать настройки вашего проекта:
// ID шаблона, имя устройства и токен (желательно на английском и без пробелов)
#define BLYNK_TEMPLATE_ID "XXXXXXXX"
#define BLYNK_DEVICE_NAME "IoTik32B"
#define BLYNK_AUTH_TOKEN "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXX"

#define LED 18 // пин встроенного светодиода (ЙоТик 32А - 4, ЙоТик 32В - 18)
float val; // переменная для датчика Холла

#define BLYNK_PRINT Serial // для вывода надписей по Blynk в монитор порта

// библиотеки для работы с Blynk и Wi-Fi
#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>

char auth[] = BLYNK_AUTH_TOKEN;
IPAddress blynk_ip(139, 59, 206, 133);

// Параметры вашего Wi-Fi соединения
char ssid[] = "MGBot";
char pass[] = "Terminator812";

// таймер обновления данных датчика Холла
BlynkTimer timer;

// Эта функция обработается по изменению состояния виртуального порта V0 (кнопка)
BLYNK_WRITE(V0)
{
  int value = param.asInt();
  if (value == 1) {
    digitalWrite(LED, HIGH);
  }
  else {
    digitalWrite(LED, LOW);
  }
}

// Эта функция срабатывает каждый раз по истечению таймера
void myTimerEvent()
{
  val = hallRead();

  Blynk.virtualWrite(V1, val); // отправить значение датчика Холла в вирт. порт V1 (Дисплей)
}

void setup()
{
  Serial.begin(115200);
  pinMode(LED, OUTPUT);
  Blynk.begin(auth, ssid, pass, "blynk.cloud", 80);
  // Можно также уточнить параметры подключения:
  //Blynk.begin(auth, ssid, pass, "blynk.cloud", 80);
  //Blynk.begin(auth, ssid, pass, IPAddress(192,168,1,100), 8080);

  timer.setInterval(1000L, myTimerEvent); // настройка таймера(период, функция для обновления)
}

// оставляем в главном цикле только проверку синхронизации с сервером Blynk и работы таймера
// желательно не перегружать данную функцию, т.к. Blynk может посчитать что вы отвалились от сервера
void loop()
{
  Blynk.run();
  timer.run();
}
