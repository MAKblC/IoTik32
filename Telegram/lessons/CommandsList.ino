#include <WiFi.h>  // библиотека для Wi-Fi
#include <WiFiClientSecure.h>
#include <UniversalTelegramBot.h>                        // библиотека Телеграм
#define WIFI_SSID "XXXXXX"                               // логин Wi-Fi
#define WIFI_PASSWORD "XXXXXXXX"                         // пароль Wi-Fi
#define BOT_TOKEN "917994990:XXXXXXXXXXXXoDvr-RPiM4PUc"  // токен
const unsigned long BOT_MTBS = 1000;                     // время обновления
WiFiClientSecure secured_client;                         // клиент SSL (TLS)
UniversalTelegramBot bot(BOT_TOKEN, secured_client);     // экземпляр бота
unsigned long bot_lasttime;                              // счетчик времени

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
  // настройка команд
  bot_setup();
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

// меню команд
void bot_setup() {
  const String commands = F("["
                            "{\"command\":\"help\",  \"description\":\"Помощь\"},"
                            "{\"command\":\"start\", \"description\":\"Стартовое сообщение\"},"
                            "{\"command\":\"status\",\"description\":\"Узнать статус\"}"
                            "]");
  bot.setMyCommands(commands);
}

void handleNewMessages(int numNewMessages) {
  for (int i = 0; i < numNewMessages; i++) {
    String chat_id = bot.messages[i].chat_id;
    String text = bot.messages[i].text;

    String from_name = bot.messages[i].from_name;
    if (from_name == "")
      from_name = "Guest";

    if (text == "/start") {
      String welcome = "Привет, " + from_name + ".\n";
      welcome += "В этом примере настроены команды программно";
      bot.sendMessage(chat_id, welcome, "Markdown");
    }
  }
}
