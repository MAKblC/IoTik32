#include <WiFi.h>
#include <WiFiMulti.h>
#include <HTTPClient.h>
const char* ssid = "login"; // логин
const char* password = "password"; // пароль
#include <Discord_WebHook.h>
Discord_Webhook discord; // Создание Discord_Webhook объекта
String DISCORD_WEBHOOK = "URL-адрес вебхука"; // ссылка на вебхук
void setup() {
    Serial.begin(115200);
    discord.begin(DISCORD_WEBHOOK);
    discord.addWiFi(ssid, password);
    discord.connectWiFi();
    discord.send("Hello World!"); // Отправляем сообщение
}

void loop() {
}
