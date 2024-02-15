#include <WiFi.h>  // библиотека для Wi-Fi
#include <WiFiClientSecure.h>
#include <UniversalTelegramBot.h>                           // библиотека Телеграм
#define WIFI_SSID "XXXXX"                                   // логин Wi-Fi
#define WIFI_PASSWORD "XXXXXXXXXXXX"                        // пароль Wi-Fi
#define BOT_TOKEN "917994990:XXXXXXXXXXXXXXXXXXr-RPiM4PUc"  // токен
const unsigned long BOT_MTBS = 1000;                        // время обновления
WiFiClientSecure secured_client;                            // клиент SSL (TLS)
UniversalTelegramBot bot(BOT_TOKEN, secured_client);        // экземпляр бота
unsigned long bot_lasttime;                                 // счетчик времени

#define LED 18  // 4

String keyboardJson1 = "[[{ \"text\" : \"Панель управления\", \"callback_data\" : \"/options\" }],[{ \"text\" : \"Ссылки\", \"callback_data\" : \"/site\" }]]";
String keyboardJson2 = "[[{ \"text\" : \"cайт MGBot\", \"url\" : \"https://mgbot.ru\" },{ \"text\" : \"сайт контроллера\", \"url\" : \"https://www.iotik.ru\" }],[{ \"text\" : \"☰ Меню\", \"callback_data\" : \"/menu\" }]]";
String keyboardJson3 = "[[{ \"text\" : \"Включить светодиод\", \"callback_data\" : \"/ledon\" },{ \"text\" : \"Выключить светодиод\", \"callback_data\" : \"/ledoff\" }],[{ \"text\" : \"Данные датчика Холла\", \"callback_data\" : \"/hall\" }],[{ \"text\" : \"☰ Меню\", \"callback_data\" : \"/menu\" }]]";

void setup() {
  Serial.begin(115200);
  pinMode(LED, OUTPUT);
  delay(512);
  Serial.println();
  Serial.print("Connecting to Wifi SSID ");
  Serial.print(WIFI_SSID);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);                 // подключение
  secured_client.setCACert(TELEGRAM_CERTIFICATE_ROOT);  // сертификат
  while (WiFi.status() != WL_CONNECTED)                 // проверка подключения
  {
    Serial.print(".");
    delay(500);
  }
  Serial.print("\nWiFi connected. IP address: ");
  Serial.println(WiFi.localIP());
}

void loop() {
  if (millis() - bot_lasttime > BOT_MTBS)  // периодическая проверка
  {
    // постановка нового сообщения в очередь обработки
    int numNewMessages = bot.getUpdates(bot.last_message_received + 1);
    // если сообщение пришло
    while (numNewMessages) {
      Serial.println("got response");
      handleNewMessages(numNewMessages);
      numNewMessages = bot.getUpdates(bot.last_message_received + 1);
    }
    // отслеживание периодичности опроса
    bot_lasttime = millis();
  }
}

// функция обработки нового сообщения
void handleNewMessages(int numNewMessages) {
  Serial.print("handleNewMessages ");
  Serial.println(numNewMessages);
  // обработка сообщений
  for (int i = 0; i < numNewMessages; i++) {
    String chat_id = bot.messages[i].chat_id;      // ID чата
    String text = bot.messages[i].text;            // текст сообщения
    text.toLowerCase();                            // преобразовать текст в нижний регистр
    String from_name = bot.messages[i].from_name;  // имя пользователя
    if (from_name == "")
      from_name = "Guest";
    // обработка команд
    if (text == "/ledon") {
      digitalWrite(LED, HIGH);
      bot.sendMessage(chat_id, "Светодиод включен", "");
    }
    if (text == "/ledoff") {
      digitalWrite(LED, LOW);
      bot.sendMessage(chat_id, "Светодиод выключен", "");
    }
    if (text == "/hall") {
      float val = hallRead();
      bot.sendMessage(chat_id, String(val, 1), "");
    }
    if (text == "/menu") {
      // отображение панели
      bot.sendMessageWithInlineKeyboard(chat_id, "Выберите раздел", "", keyboardJson1);
    }
    if (text == "/marking") {
      String sms = "*писать жирным шрифтом*\n";
      sms += "_писать курсивом_\n";
      sms += "`вставить элемент кода`\n";
      sms += "Ссылка на аккаунт @MGBotCommunity\n";
      sms += "Прямая ссылка mgbot.ru\n";
      sms += "[Гиперссылка с эмодзи 🌎📲](https://mgbot.ru)\n";
      bot.sendMessageWithInlineKeyboard(chat_id, sms, "Markdown", keyboardJson1);
    }
    if (text == "/markingv2") {
      String sms = "~зачеркнутый текст~\n";
      sms += "__подчеркнутый текст__\n";
      sms += "||спойлер||\n";
      bot.sendMessageWithInlineKeyboard(chat_id, sms, "MarkdownV2", keyboardJson1);
    }
    if (text == "/options") {
      // отображение панели
      bot.sendMessageWithInlineKeyboard(chat_id, "*Выберите команду*", "Markdown", keyboardJson3);
    }
    if (text == "/site") {
      // отображение панели
      bot.sendMessageWithInlineKeyboard(chat_id, "Выберите сайт", "", keyboardJson2);
    }
    if (text == "/start") {
      String sms = "Привет 👋, " + from_name + "!\n";
      sms += "Я контроллер Йотик 32. Я принимаю следующие команды:\n\n";
      sms += "/options - панель управления\n";
      sms += "/site - ссылки\n";
      sms += "/marking - варианты форматирования текста\n";
      sms += "/markingv2 - варианты форматирования текста\n";
      bot.sendMessage(chat_id, sms, "");
    }
  }
}
