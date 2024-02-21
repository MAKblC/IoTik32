#include <WiFi.h>  // библиотека для Wi-Fi
#include <WiFiClientSecure.h>
#include <UniversalTelegramBot.h>                              // библиотека Телеграм
#define WIFI_SSID "XXXXXXXX"                                   // логин Wi-Fi
#define WIFI_PASSWORD "XXXXXXXXXXXXXX"                         // пароль Wi-Fi
#define BOT_TOKEN "917994990:XXXXXXXXXXXXXXXO7ooDvr-RPiM4PUc"  // токен
const unsigned long BOT_MTBS = 1000;                           // время обновления
WiFiClientSecure secured_client;                               // клиент SSL (TLS)
UniversalTelegramBot bot(BOT_TOKEN, secured_client);           // экземпляр бота
unsigned long bot_lasttime;                                    // счетчик времени

String user_id = "28XXXXXXX47";  // ваш chat_ID

// URL фотографии
String test_photo_url = "https://mgbot.ru/upload/logo-r.png";

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
  bot.sendMessage(user_id, "Бот запущен", "");
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
  Serial.print("handleNewMessages ");
  Serial.println(numNewMessages);
  for (int i = 0; i < numNewMessages; i++) {
    String chat_id = bot.messages[i].chat_id;
    Serial.println(chat_id);
    // проверка пользователя
    if (chat_id != user_id) {
      bot.sendMessage(chat_id, "У вас нет доступа к общению с данным ботом", "");
    } else {
      String text = bot.messages[i].text;
      String from_name = bot.messages[i].from_name;
      if (from_name == "")
        from_name = "Guest";

      if (text == "/start") {
        String sms = "Привет, " + from_name + "!\n";
        sms += "В этом примере показо общение с определенным пользователем\n";
        sms += "/photo - отправить фото по URL\n";
        bot.sendMessage(chat_id, sms, "");
      }
      if (text == "/photo") {  // пост фотографии
        bot.sendPhoto(chat_id, test_photo_url, "а вот и фотка!");
      }
    }
  }
}
