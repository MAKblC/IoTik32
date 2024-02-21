#include <WiFi.h>  // библиотека для Wi-Fi
#include <WiFiClientSecure.h>
#include <UniversalTelegramBot.h>                     // библиотека Телеграм
#define WIFI_SSID "XXXXXX"                            // логин Wi-Fi
#define WIFI_PASSWORD "XXXXXXXX"                      // пароль Wi-Fi
#define BOT_TOKEN "917994990:XXXXXXXXXXr-RPiM4PUc"    // токен
const unsigned long BOT_MTBS = 1000;                  // время обновления
WiFiClientSecure secured_client;                      // клиент SSL (TLS)
UniversalTelegramBot bot(BOT_TOKEN, secured_client);  // экземпляр бота
unsigned long bot_lasttime;                           // счетчик времени

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
    int message_id = bot.messages[i].message_id;
    String chat_id = String(bot.messages[i].chat_id);
    String text = bot.messages[i].text;
    String query_id = String(bot.messages[i].query_id);
    Serial.println(message_id);
    Serial.println(text);
    Serial.println(query_id);
    text.toLowerCase();
    String from_name = bot.messages[i].from_name;
    if (from_name == "") from_name = "Guest";
    if (bot.messages[i].type == "callback_query") {
      if (text == "/check") {
        float val = hallRead();
        if (val < 100) {
          bot.answerCallbackQuery(query_id, "Показания с датчика Холла ниже 100!", true);
        } else {
          bot.answerCallbackQuery(query_id, "Показания с датчика Холла выше 100!", false);
        }
      }
    }
    if ((text == "/start") || (text == "start") || (text == "/help") || (text == "help")) {
      bot.sendMessage(chat_id, "Привет, " + from_name + "!", "");
      String keyboardJson = "[[{ \"text\" : \"Проверка датчика\", \"callback_data\" : \"/check\" }]]";
      bot.sendMessageWithInlineKeyboard(chat_id, "Проверка значений", "Markdown", keyboardJson);
    }
  }
}
