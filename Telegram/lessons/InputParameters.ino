#include <WiFi.h>  // библиотека для Wi-Fi
#include <WiFiClientSecure.h>
#include <UniversalTelegramBot.h>                                 // библиотека Телеграм
#define WIFI_SSID "XXXXX"                                         // логин Wi-Fi
#define WIFI_PASSWORD "XXXXXXXXX"                                 // пароль Wi-Fi
#define BOT_TOKEN "917994990:XXXXXXXXXXXXXbgaWpO7ooDvr-RPiM4PUc"  // токен
const unsigned long BOT_MTBS = 1000;                              // время обновления
WiFiClientSecure secured_client;                                  // клиент SSL (TLS)
UniversalTelegramBot bot(BOT_TOKEN, secured_client);              // экземпляр бота
unsigned long bot_lasttime;                                       // счетчик времени

String led_status;
#define LED 18
String hello = "Привет, ";

void setup() {
  pinMode(LED, OUTPUT);
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
  Serial.print("handleNewMessages ");
  Serial.println(numNewMessages);

  for (int i = 0; i < numNewMessages; i++) {
    String chat_id = bot.messages[i].chat_id;
    String text = bot.messages[i].text;
    String from_name = bot.messages[i].from_name;
    if (from_name == "")
      from_name = "Guest";
    if (text == "/hello") {
      bot.sendMessage(chat_id, "Введите новое приветствие", "");
      hello = text;
      // ждем, пока не придёт новое текстовое сообщение
      while (hello == text) {
        numNewMessages = bot.getUpdates(bot.last_message_received + 1);
        hello = bot.messages[i].text;
      }
      // пришедший текст теперь в приветствии
      bot.sendMessage(chat_id, "Новое приветствие: " + hello, "");
    }
    if (text == "/led") {
      bot.sendMessage(chat_id, "Введите состояние", "");
      led_status = text;
      // ждем, пока не придет новое значение
      while (led_status == text) {
        numNewMessages = bot.getUpdates(bot.last_message_received + 1);
        led_status = bot.messages[i].text;
      }
      // преобразовываем текст в int
      bool led = led_status.toInt();
      if (led) {
        bot.sendMessage(chat_id, "Светодиод включен!", "");
        digitalWrite(LED, HIGH);
      } else {
        bot.sendMessage(chat_id, "Светодиод выключен!", "");
        digitalWrite(LED, LOW);
      }
    }
    if (text == "/start") {
      String sms = hello + " " + from_name + "!\n";
      sms += "Я контроллер Йотик 32. Я принимаю следующие команды:\n";
      sms += "/led - управлять светодиодом\n";
      sms += "/hello - изменить приветствие\n";
      bot.sendMessage(chat_id, sms, "");
    }
  }
}
