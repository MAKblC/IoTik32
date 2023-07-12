#include <WiFi.h>
#include <WiFiMulti.h>
#include <HTTPClient.h>
const char* ssid = "login"; // логин
const char* password = "password"; // пароль
const int ledPin_1 = 4; // (Йотик 32Б – 18, Йотик 32А – 4)
#include <Discord_WebHook.h>
Discord_Webhook discord; // Создание Discord_Webhook объекта
String DISCORD_WEBHOOK = "URL-адрес вебхука"; // ссылка на вебхук
void setup() {
    Serial.begin(115200);
    discord.begin(DISCORD_WEBHOOK); 
   discord.addWiFi(ssid,password); 
   discord.connectWiFi(); 
   discord.send("Подключение выполнено");
   pinMode(ledPin_1, OUTPUT);
}
void loop() {
    digitalWrite(ledPin_1, HIGH);
    discord.send("Светодиод включен");
    delay(10000);

    digitalWrite(ledPin_1, LOW);
    discord.send("Светодиод выключен");
    delay(10000);

    float val = hallRead();
    discord.send("Значение датчика Холла: " + String(val,true));
    delay(15000);
}
