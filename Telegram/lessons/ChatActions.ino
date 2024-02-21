#include <WiFi.h>  // библиотека для Wi-Fi
#include <WiFiClientSecure.h>
#include <UniversalTelegramBot.h>                          // библиотека Телеграм
#define WIFI_SSID "XXXXXX"                                 // логин Wi-Fi
#define WIFI_PASSWORD "XXXXXXXX"                           // пароль Wi-Fi
#define BOT_TOKEN "917994990:XXXXXXXXXWpO7ooDvr-RPiM4PUc"  // токен
const unsigned long BOT_MTBS = 1000;                       // время обновления
WiFiClientSecure secured_client;                           // клиент SSL (TLS)
UniversalTelegramBot bot(BOT_TOKEN, secured_client);       // экземпляр бота
unsigned long bot_lasttime;                                // счетчик времени

void setup() {
  Serial.begin(115200);
  delay(512);
  Serial.println();
  Serial.print("Connecting to Wifi SSID ");
  Serial.print(WIFI_SSID);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  secured_client.setCACert(TELEGRAM_CERTIFICATE_ROOT);  // Add root certificate for api.telegram.org
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.print("\nWiFi connected. IP address: ");
  Serial.println(WiFi.localIP());
}

void loop() {
  if (millis() - bot_lasttime > BOT_MTBS) {
    int numNewMessages = bot.getUpdates(bot.last_message_received + 1);

    while (numNewMessages) {
      Serial.println("got response");
      handleNewMessages(numNewMessages);
      numNewMessages = bot.getUpdates(bot.last_message_received + 1);
    }

    bot_lasttime = millis();
  }
}

void handleNewMessages(int numNewMessages) {
  Serial.println("handleNewMessages");
  Serial.println(String(numNewMessages));
  for (int i = 0; i < numNewMessages; i++) {
    String chat_id = String(bot.messages[i].chat_id);
    String text = bot.messages[i].text;
    String from_name = bot.messages[i].from_name;
    if (from_name == "") from_name = "Guest";
    if (text == "/send_test_action") {
      // Можно использовать только из списка:
      bot.sendChatAction(chat_id, "typing");
      delay(4000);
      bot.sendChatAction(chat_id, "upload_photo");
      delay(4000);
      bot.sendChatAction(chat_id, "record_video");
      delay(4000);
      bot.sendChatAction(chat_id, "record_audio");
      delay(4000);
      bot.sendChatAction(chat_id, "upload_document");
      delay(4000);
      bot.sendChatAction(chat_id, "choose_sticker");
      delay(4000);
      bot.sendMessage(chat_id, "Увидели все действия?");
    }
    if (text == "/start") {
      String welcome = "Привет, " + from_name + ".\n";
      welcome += "Рассмотрим действия бота.\n\n";
      welcome += "/send_test_action : посмотреть действия\n";
      bot.sendMessage(chat_id, welcome);
    }
  }
}
