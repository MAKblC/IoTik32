#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <UniversalTelegramBot.h>
#define WIFI_SSID "login"
#define WIFI_PASSWORD "password"
#define BOT_TOKEN "token"
const unsigned long BOT_MTBS = 1000; // время обновления
WiFiClientSecure secured_client;
UniversalTelegramBot bot(BOT_TOKEN, secured_client);
unsigned long bot_lasttime;
#include <Preferences.h>
Preferences pref; 
int val; 

void setup()
{
  pref.begin("gpio-state", false); // запуск пространства имен
  Serial.begin(115200);
  delay(512);
  Serial.println();
  Serial.print("Connecting to Wifi SSID ");
  Serial.print(WIFI_SSID);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  secured_client.setCACert(TELEGRAM_CERTIFICATE_ROOT); 
  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    delay(500);
  }
  Serial.print("\nWiFi connected. IP address: ");
  Serial.println(WiFi.localIP());
}

void loop()
{
  if (millis() - bot_lasttime > BOT_MTBS)
  {
    int numNewMessages = bot.getUpdates(bot.last_message_received + 1);
    while (numNewMessages)
    {
      Serial.println("got response");
      handleNewMessages(numNewMessages);
      numNewMessages = bot.getUpdates(bot.last_message_received + 1);
    } bot_lasttime = millis();
  }
}

void handleNewMessages(int numNewMessages)
{
  Serial.print("handleNewMessages ");
  Serial.println(numNewMessages);
  for (int i = 0; i < numNewMessages; i++)
  {
    String chat_id = bot.messages[i].chat_id;
    String text = bot.messages[i].text;
    text.toLowerCase();
    String from_name = bot.messages[i].from_name;
    if (from_name == "")
      from_name = "Guest";
    if (text == "/getvalue") // команда для считывания значения из памяти
    {
      val = pref.getInt("state", false); // чтение данных, используя ключ
      bot.sendMessage(chat_id, "А вот и значение: " + String(val), "");
    }
    if (text == "/setvalue") // команда, которая записывает в память int случайную от 0-255
    {
      val = random(0, 255);
      bot.sendMessage(chat_id, "Значение для записи: " + String(val), "");
      pref.putInt("state", val);
    }
    if ((text == "/start") || (text == "start") || (text == "/help") || (text == "help"))
    {
      String welcome = "Привет, Я контроллер Йотик 32. Я принимаю следующие команды: \n";
      welcome += "/setvalue - записать значение в память\n";
      welcome += "/getvalue - считать значение из памяти";
      bot.sendMessage(chat_id, welcome, "");
    }
  }
}
