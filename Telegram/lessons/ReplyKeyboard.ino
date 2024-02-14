#include <WiFi.h>  // библиотека для Wi-Fi
#include <WiFiClientSecure.h>
#include <UniversalTelegramBot.h>                                  // библиотека Телеграм
#define WIFI_SSID "XXXXX"                                          // логин Wi-Fi
#define WIFI_PASSWORD "XXXXXXXXXXXXX"                              // пароль Wi-Fi
#define BOT_TOKEN "917994990:XXXXXXXXXXXXXXXXXWpO7ooDvr-RPiM4PUc"  // токен
const unsigned long BOT_MTBS = 1000;                               // время обновления
WiFiClientSecure secured_client;                                   // клиент SSL (TLS)
UniversalTelegramBot bot(BOT_TOKEN, secured_client);               // экземпляр бота
unsigned long bot_lasttime;                                        // счетчик времени

#define LED 18  // 4

String keyboardJson1 = "[[\"/ledon\", \"/ledoff\", \"/hall\"],[\"/menu\"]]"; // панель для команды /options
String keyboardJson2 = "[[\"/start\", \"/info\"],[\"/options\"]]"; // панель для команды /menu 

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
    if (text == "/options") {
      // отображение панели
      bot.sendMessageWithReplyKeyboard(chat_id, "Выберите команду", "", keyboardJson1, true);
    }
    if (text == "/menu") {
      // отображение панели
      bot.sendMessageWithReplyKeyboard(chat_id, "Выберите команду", "", keyboardJson2, true);
    }
    if (text == "/info") {
      String sms = "Модель чипа: " + String(ESP.getChipModel()) + "\n";
      sms += "Номер ревизии чипа: " + String(ESP.getChipRevision()) + "\n";
      sms += "Версия SDK: " + String(ESP.getSdkVersion()) + "\n";
      sms += "Размер флеш-памяти: " + String(ESP.getFlashChipSize()) + " байт\n";
      sms += "Частота чипа: " + String(ESP.getFlashChipSpeed()) + " Гц\n";
      bot.sendMessage(chat_id, sms, "");
    }
    if (text == "/start") {
      String sms = "Привет, " + from_name + "!\n";
      sms += "Я контроллер Йотик 32. Я принимаю следующие команды:\n\n";
      sms += "/options - панель управления\n";
      bot.sendMessage(chat_id, sms, "");
    }
  }
}
