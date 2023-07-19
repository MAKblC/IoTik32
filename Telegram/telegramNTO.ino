#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <UniversalTelegramBot.h>
#define WIFI_SSID "Wokwi-GUEST"
#define WIFI_PASSWORD ""
#define BOT_TOKEN ""

const unsigned long BOT_MTBS = 1000; // время обновления
WiFiClientSecure secured_client;
UniversalTelegramBot bot(BOT_TOKEN, secured_client);
unsigned long bot_lasttime;
void setup()
{
  Serial.begin(115200);
  delay(512);
  Serial.println();
  Serial.print("Connecting to Wifi SSID ");
  Serial.print(WIFI_SSID);
  WiFi.begin("Wokwi-GUEST", "", 6);
  secured_client.setCACert(TELEGRAM_CERTIFICATE_ROOT); // Add root certificate for api.telegram.org
  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    delay(500);
  }
  Serial.print("\nWiFi connected. IP address: ");
  Serial.println(WiFi.localIP());
}

String translateEncryptionType(wifi_auth_mode_t encryptionType) {
  switch (encryptionType) {
    case (WIFI_AUTH_OPEN):
      return "Open";
    case (WIFI_AUTH_WEP):
      return "WEP";
    case (WIFI_AUTH_WPA_PSK):
      return "WPA_PSK";
    case (WIFI_AUTH_WPA2_PSK):
      return "WPA2_PSK";
    case (WIFI_AUTH_WPA_WPA2_PSK):
      return "WPA_WPA2_PSK";
    case (WIFI_AUTH_WPA2_ENTERPRISE):
      return "WPA2_ENTERPRISE";
  }
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
    }

    bot_lasttime = millis();
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
    
    if (text == "/sensors") 
    {
      String welcome = "Показания датчиков метеостанции:\n";
      welcome += "🌡 Температура воздуха: " + String(random(20,30)) + " °C\n";
      welcome += "💧 Влажность воздуха: " + String(random(20,40)) + " %\n";
      welcome += "☁ Атмосферное давление: " + String(random(740,770)) + " мм рт.ст.\n";
      welcome += "☀ Освещенность: " + String(random(100,3000)) + " Лк\n";
      welcome += "🅰 Ультрафиолет-А: " + String(random(0,100)) + " mkWt/cm2\n";
      welcome += "🅱 Ультрафиолет-В: " + String(random(0,100)) + " mkWt/cm2\n";
      welcome += "🔆 Индекс УФ: " + String(random(0,8)) + " \n";
      welcome += "🎏 Направление ветра: " + String(random(0,7)*45) + " °\n";
      welcome += "💨 Скорость ветра: " + String(random(0,20)) + " м/с\n";
      welcome += "☔️ Уровень осадков: " + String(random(0,20)) + " мм\n";
      bot.sendMessage(chat_id, welcome, "Markdown");
    }

    if (text == "/start")
    {
      String sms = "Привет, " + from_name + "!\n Команды:\n";
      sms += "/sensors - данные с датчиков\n";
      bot.sendMessage(chat_id, sms, "Markdown");
    }
    if (text == "/answers")
    {
      String sms = "Информация о метеостанции: \n";
      sms += "MAC-адрес: " + String(WiFi.macAddress())+ "\n";
      Serial.println(WiFi.macAddress());
      String hostname = WiFi.getHostname();
      sms += "IP-адрес подключения: " + hostname + "\n";
      Serial.println(WiFi.getHostname());
      String encryptionTypeDescription = translateEncryptionType(WiFi.encryptionType(i));
      sms += "Тип шифрования: " + encryptionTypeDescription + "\n";
      Serial.println(encryptionTypeDescription );
      bot.sendMessage(chat_id, sms, "Markdown");
    }
  }
}
