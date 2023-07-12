#define BLYNK_PRINT Serial
#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h> // конфигурация блинка 
IPAddress blynk_ip(139, 59, 206, 133);            // конфигурация блинка 
#define UPDATE_TIMER 1000
BlynkTimer timer_update;      // настройка таймера для обновления данных с сервера BLynk 
#define LED 18 
int val = 0;
char ssid[] = "MGBot";       // Логин Wi-Fi  
char pass[] = "Terminator812";                    // Пароль от Wi-Fi 
char auth[] = "f6e62ba51f3d4ce2b2ab1d65fd45d1cc"; // Токен 
IPAddress blynk_ip(139, 59, 206, 133);            // Blynk server
#define UPDATE_TIMER 1000 // передавать данные каждую секунду
BlynkTimer timer_update; // определяем таймер
void setup()
{  pinMode(LED, OUTPUT); 
  Serial.begin(115200);
  delay(512);
  Blynk.begin(auth, ssid, pass, blynk_ip, 8442);// подключение к серверу Blynk 
  timer_update.setInterval(UPDATE_TIMER, readSendData); // включаем таймер обновления данных  
}
void readSendData() {
  val = hallRead(); // считывание датчика Холла
  Blynk.virtualWrite(V0, val); delay(25); // Отправка данных на сервер Blynk  Датчик Холла   
}
BLYNK_WRITE(V1)
{
  int buttonstate2 = param.asInt ();
  if (buttonstate2 == 1) {
digitalWrite(LED, HIGH); // включить, если нажата кнопка "Светодиод"
  }
  else    {
    digitalWrite(LED, LOW);
  }
}
