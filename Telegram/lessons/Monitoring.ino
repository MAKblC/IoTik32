#include <WiFi.h>  // библиотека для Wi-Fi
#include <WiFiClientSecure.h>
#include <UniversalTelegramBot.h>                             // библиотека Телеграм
#define WIFI_SSID "XXXXX"                                     // логин Wi-Fi
#define WIFI_PASSWORD "XXXXXXXXX"                             // пароль Wi-Fi
#define BOT_TOKEN "917994990:XXXXXXXXXXXXXXX7ooDvr-RPiM4PUc"  // токен
const unsigned long BOT_MTBS = 1000;                          // время обновления
WiFiClientSecure secured_client;                              // клиент SSL (TLS)
UniversalTelegramBot bot(BOT_TOKEN, secured_client);          // экземпляр бота
unsigned long bot_lasttime;                                   // счетчик времени

String chat_id;
String text;
int message_id;
float val, prev_val;
bool flag;
#define your_chat_ID "28XXXXX7"  // ID вашего чата с ботом

void setup() {
  Serial.begin(115200);
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
  bot.sendMessage(your_chat_ID, "Бот запущен", "");  // сообщение о запуске отправлено в Ваш чат
}

void loop() {
  if (millis() - bot_lasttime > BOT_MTBS)  // периодическая проверка
  {
    // если определено сообщение для обновления
    if (flag) {
      label();
    }
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
    Serial.println(message_id);
    chat_id = bot.messages[i].chat_id;
    text = bot.messages[i].text;
    text.toLowerCase();                            // преобразовать текст в нижний регистр
    String from_name = bot.messages[i].from_name;  // имя пользователя
    if (from_name == "")
      from_name = "Guest";
    if (text == "/start") {
      flag = false;
      String sms = "[[{\"text\" : \"Панель\", \"callback_data\" : \"/hall\" }]]";
      String mail = "Привет! 👋\n";
      mail += "Этот бот может показать значения датчика и информацию о контроллере\n\n";
      mail += "*Нажми на «Меню», чтобы выбрать раздел и узнать больше ↓↓↓* \n\n";
      bot.sendMessageWithInlineKeyboard(chat_id, mail, "Markdown", sms);
    }
    if (text == "/hall") {
      flag = true;
      val = hallRead();
      String sms = "[[{ \"text\" : \"☰ В меню\", \"callback_data\" : \"/start\" }],";
      sms += "[{ \"text\" : \"Показания: ";
      sms += String(val, 1);
      sms += "  \", \"callback_data\" : \"/hall\" }]]";
      String mail = "*Датчик Холла: *";
      bot.sendMessageWithInlineKeyboard(chat_id, mail, "Markdown", sms);
    }
  }
}

void label() {
  String mail = "";
  String sms = "";
  val = hallRead();
  if (val != prev_val) {  // если значение отличается от предыдущего
    sms += "[[{ \"text\" : \"☰ В меню\", \"callback_data\" : \"/start\" }],";
    sms += "[{ \"text\" : \"Показания: ";
    sms += String(val, 1);
    sms += "  \", \"callback_data\" : \"/hall\" }]]";
    mail = "*Датчик Холла: *";
    Serial.println("отправляю значение " + String(message_id));
    // обновление панели
    bot.sendMessageWithInlineKeyboard(your_chat_ID, mail, "Markdown", sms, message_id + 1);
  }
  prev_val = val;  // перезаписать значение
}
