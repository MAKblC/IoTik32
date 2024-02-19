#include <WiFi.h>  // библиотека для Wi-Fi
#include <WiFiClientSecure.h>
#include <UniversalTelegramBot.h>                                 // библиотека Телеграм
#define WIFI_SSID "XXXXXX"                                        // логин Wi-Fi
#define WIFI_PASSWORD "XXXXXXXXXXXX"                              // пароль Wi-Fi
#define BOT_TOKEN "917994990:XXXXXXXXXXXXXXXXXpO7ooDvr-RPiM4PUc"  // токен
const unsigned long BOT_MTBS = 1000;                              // время обновления
WiFiClientSecure secured_client;                                  // клиент SSL (TLS)
UniversalTelegramBot bot(BOT_TOKEN, secured_client);              // экземпляр бота
unsigned long bot_lasttime;                                       // счетчик времени

#define LED 18  // 4
 // переменные
String chat_id;
String text;
int message_id;
float val;

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
    // считывание ID сообщения   
    message_id = bot.messages[i].message_id;
    chat_id = bot.messages[i].chat_id;
    text = bot.messages[i].text;
    text.toLowerCase();                            // преобразовать текст в нижний регистр
    String from_name = bot.messages[i].from_name;  // имя пользователя
    if (from_name == "")
      from_name = "Guest";
    // обработка команд
    // если сообщение имеет тип обратного запроса
    if (bot.messages[i].type == "callback_query") {
      if (text == "/menu") {
        String sms = "[[{\"text\" : \"Инфо о контроллере\", \"callback_data\" : \"/info\"},{\"text\" : \"Датчик Холла\", \"callback_data\" : \"/hall\"}]]";
        String mail = "Выберите команду\n";
        bot.sendMessageWithInlineKeyboard(chat_id, mail, "Markdown", sms, message_id);
      } else {
        label();
      }
      // обычные сообщения
    } else {
      if (text == "/start") {
        String sms = "[[{\"text\" : \"Меню\", \"callback_data\" : \"/menu\" }]]";
        String mail = "Привет! 👋\n";
        mail += "Этот бот может показать значения датчика и информацию о контроллере\n\n";
        mail += "*Нажми на «Меню», чтобы выбрать раздел и узнать больше ↓↓↓* \n\n";
        bot.sendMessageWithInlineKeyboard(chat_id, mail, "Markdown", sms);
        // если команда незнакомая
      } else {
        String sms = "[[ { \"text\" : \"☰ В меню\", \"callback_data\" : \"/menu\" }]]";
        String mail = "Неизвестная команда. Вернуться в главное меню ↓↓↓\n";
        bot.sendMessageWithInlineKeyboard(chat_id, mail, "Markdown", sms);
      }
    }
  }
}
// функция конструктора команд
void label() {
  // текст сообщения и кнопки в панели
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
  } else if (text == "/hall") {
    val = hallRead();
    sms += "[[{ \"text\" : \"☰ В меню\", \"callback_data\" : \"/menu\" }],";
    sms += "[{ \"text\" : \"Показания: ";
    sms += String(val, 1);
    sms += "  \", \"callback_data\" : \"/hall\" }]]";
    mail = "*Датчик Холла: *";
  } else {
    sms = "[[ { \"text\" : \"☰ В меню\", \"callback_data\" : \"/menu\" }]]";
    mail = "Неизвестная команда. Вернуться в главное меню ↓↓↓\n";
  }
  // отправка сообщения с данными
  bot.sendMessageWithInlineKeyboard(chat_id, mail, "Markdown", sms, message_id);
}
