#include <WiFi.h>  // библиотека для Wi-Fi
#include <WiFiClientSecure.h>
#include <UniversalTelegramBot.h>                           // библиотека Телеграм
#define WIFI_SSID "XXXXXX"                                  // логин Wi-Fi
#define WIFI_PASSWORD "XXXXXXXXX"                           // пароль Wi-Fi
#define BOT_TOKEN "917994990:XXXXXXXXXXXXXXooDvr-RPiM4PUc"  // токен
const unsigned long BOT_MTBS = 1000;                        // время обновления
WiFiClientSecure secured_client;                            // клиент SSL (TLS)
UniversalTelegramBot bot(BOT_TOKEN, secured_client);        // экземпляр бота
unsigned long bot_lasttime;                                 // счетчик времени

#define LED 18  // 4

String chat_id;
String text;
int message_id;
bool flag = false;  // для светодиода

void setup() {
  Serial.begin(115200);
  pinMode(LED, OUTPUT);
  digitalWrite(LED, LOW);  // выключение светодиода
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
    message_id = bot.messages[i].message_id;
    chat_id = bot.messages[i].chat_id;
    text = bot.messages[i].text;
    text.toLowerCase();                            // преобразовать текст в нижний регистр
    String from_name = bot.messages[i].from_name;  // имя пользователя
    if (from_name == "")
      from_name = "Guest";
    // обработка команд
    if (bot.messages[i].type == "callback_query") {
      if (text == "/menu") {
        String sms = "[[{\"text\" : \"Инфо о контроллере\", \"callback_data\" : \"/info\"},{\"text\" : \"Управление светодиодом\", \"callback_data\" : \"/ledcontrol\"}]]";
        String mail = "Выберите команду\n";
        bot.sendMessageWithInlineKeyboard(chat_id, mail, "Markdown", sms, message_id);
      } else {
        label();
      }
    } else {
      if (text == "/start") {
        String sms = "[[{\"text\" : \"Меню\", \"callback_data\" : \"/menu\" }]]";
        String mail = "Привет! 👋\n";
        mail += "Этот бот может показать значения датчика и информацию о контроллере\n\n";
        mail += "*Нажми на «Меню», чтобы выбрать раздел и узнать больше ↓↓↓* \n\n";
        bot.sendMessageWithInlineKeyboard(chat_id, mail, "Markdown", sms);
      } else {
        String sms = "[[ { \"text\" : \"☰ В меню\", \"callback_data\" : \"/menu\" }]]";
        String mail = "Неизвестная команда. Вернуться в главное меню ↓↓↓\n";
        bot.sendMessageWithInlineKeyboard(chat_id, mail, "Markdown", sms);
      }
    }
  }
}

void label() {
  String mail = "";
  String sms = "";
  if (text == "/info") {
    sms = "[[ { \"text\" : \"☰ В меню\", \"callback_data\" : \"/menu\" }]]";
    mail += "*Информация о контроллере:* \n";
    mail += "Модель чипа: " + String(ESP.getChipModel()) + "\n";
    mail += "Номер ревизии чипа: " + String(ESP.getChipRevision()) + "\n";
    mail += "Версия SDK: " + String(ESP.getSdkVersion()) + "\n";
    mail += "Размер флеш-памяти: " + String(ESP.getFlashChipSize()) + " байт\n";
    mail += "Частота чипа: " + String(ESP.getFlashChipSpeed()) + " Гц\n";
  } else if (text == "/ledcontrol") {
    sms += "[[{ \"text\" : \"☰ В меню\", \"callback_data\" : \"/menu\" }],";
    sms += "[{ \"text\" : \"Активировать\", \"callback_data\" : \"/led\" }]]";
    mail = "*Состояние светодиода: *";
  } else if (text == "/led") {
    sms += "[[{ \"text\" : \"☰ В меню\", \"callback_data\" : \"/menu\" }],";
    // проверка, какой текст вписать в плашку и дублировать это состояние на светодиоде
    if (flag) {
      sms += "[{ \"text\" : \"ВКЛ 🟣";
      digitalWrite(LED, HIGH);  // включение светодиода
    } else {
      sms += "[{ \"text\" : \"ВЫКЛ ⚫️";
      digitalWrite(LED, LOW);  // выключение светодиода
    }
    sms += "  \", \"callback_data\" : \"/led\" }]]";
    flag = !flag;  // смена состояния
    mail = "*Состояние светодиода: *";
  } else {
    sms = "[[ { \"text\" : \"☰ В меню\", \"callback_data\" : \"/menu\" }]]";
    mail = "Неизвестная команда. Вернуться в главное меню ↓↓↓\n";
  }
  bot.sendMessageWithInlineKeyboard(chat_id, mail, "Markdown", sms, message_id);
}
