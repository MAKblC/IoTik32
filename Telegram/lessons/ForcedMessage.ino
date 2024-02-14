#include <WiFi.h>  // библиотека для Wi-Fi
#include <WiFiClientSecure.h>
#include <UniversalTelegramBot.h>                           // библиотека Телеграм
#define WIFI_SSID "XXXXXX"                                  // логин Wi-Fi
#define WIFI_PASSWORD "XXXXXXXXXXX"                         // пароль Wi-Fi
#define BOT_TOKEN "917994990:XXXXXXXXXXXXXXXXXXr-RPiM4PUc"  // токен
const unsigned long BOT_MTBS = 1000;                        // время обновления
const unsigned long hall_alert = 10000;                     // время отправки нового уведомления
WiFiClientSecure secured_client;                            // клиент SSL (TLS)
UniversalTelegramBot bot(BOT_TOKEN, secured_client);        // экземпляр бота
unsigned long bot_lasttime, hall_monitor;                   // счетчики времени

#define LED 18                    // 4
#define your_chat_ID "28XXXXX47"  // ID вашего чата с ботом

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
  bot.sendMessage(your_chat_ID, "Бот запущен", "");  // сообщение о запуске отправлено в Ваш чат
}

void loop() {
  Serial.println(hallRead());                                    // отслеживание датчика Холла
  if (hallRead() > 150) {                                        // если значение превышено
    if (millis() - hall_monitor > hall_alert) {                  // если уже прошло 10 секунд с последней отправки
      bot.sendMessage(your_chat_ID, "Превышено значение!", "");  // отправить уведомление
      hall_monitor = millis();                                   // сброс таймера
    }
  }
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
    String chat_id = bot.messages[i].chat_id;  // ID чата
    Serial.println(chat_id);
    String text = bot.messages[i].text;            // текст сообщения
    text.toLowerCase();                            // преобразовать текст в нижний регистр
    String from_name = bot.messages[i].from_name;  // имя пользователя
    if (from_name == "")
      from_name = "Guest";
    // обработка команд
    if (text == "/start") {
      bot.sendMessage(chat_id, "Привет, " + from_name + "!", "");
      bot.sendMessage(chat_id, "Я контроллер Йотик 32. Я присылаю сообщения о запуске бота, а также уведомлю, если датчик Холла покажет превышенное значение", "");
    }
  }
}
