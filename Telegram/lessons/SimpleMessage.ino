#include <WiFi.h> // библиотекb для Wi-Fi
#include <WiFiClientSecure.h>
#include <UniversalTelegramBot.h> // библиотека Телеграм
#define WIFI_SSID "XXXXXX" // логин Wi-Fi
#define WIFI_PASSWORD "XXXXXXXXX" // пароль Wi-Fi
#define BOT_TOKEN "917994990:AXXXXXXXXXXXXXXXXXXXXXXoDvr-RPiM4PUc" // токен
const unsigned long BOT_MTBS = 1000; // время обновления
WiFiClientSecure secured_client; // клиент SSL (TLS)
UniversalTelegramBot bot(BOT_TOKEN, secured_client); // экземпляр бота
unsigned long bot_lasttime; // счетчик времени

#define LED 4 // 18

void setup()
{
  Serial.begin(115200);
  pinMode(LED, OUTPUT);
  delay(512);
  Serial.println();
  Serial.print("Connecting to Wifi SSID ");
  Serial.print(WIFI_SSID);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD); // подключение
  secured_client.setCACert(TELEGRAM_CERTIFICATE_ROOT); // сертификат
  while (WiFi.status() != WL_CONNECTED) // проверка подключения
  {
    Serial.print(".");
    delay(500);
  }
  Serial.print("\nWiFi connected. IP address: ");
  Serial.println(WiFi.localIP());
}

void loop()
{
  if (millis() - bot_lasttime > BOT_MTBS) // периодическая проверка
  {
    // постановка нового сообщения в очередь обработки
    int numNewMessages = bot.getUpdates(bot.last_message_received + 1);
    // если сообщение пришло
    while (numNewMessages)
    {
      Serial.println("got response");
      handleNewMessages(numNewMessages);
      numNewMessages = bot.getUpdates(bot.last_message_received + 1);
    }
    // отслеживание периодичности опроса
    bot_lasttime = millis();
  }
}

// функция обработки нового сообщения
void handleNewMessages(int numNewMessages)
{
  Serial.print("handleNewMessages ");
  Serial.println(numNewMessages);
  // обработка сообщений
  for (int i = 0; i < numNewMessages; i++)
  {
    String chat_id = bot.messages[i].chat_id; // ID чата
    String text = bot.messages[i].text; // текст сообщения
    text.toLowerCase(); // преобразовать текст в нижний регистр
    String from_name = bot.messages[i].from_name; // имя пользователя
    if (from_name == "")
      from_name = "Guest";
    // обработка команд
    if (text == "/ledon")
    {
      digitalWrite(LED, HIGH);
      bot.sendMessage(chat_id, "Светодиод включен", "");
    }
    if (text == "/ledoff")
    {
      digitalWrite(LED, LOW);
      bot.sendMessage(chat_id, "Светодиод выключен", "");
    }
    if (text == "/hall")
    {
      float val = hallRead();
      bot.sendMessage(chat_id, String(val, 1), "");
    }
    if (text == "/start")
    {
      bot.sendMessage(chat_id, "Привет, " + from_name + "!", "");
      bot.sendMessage(chat_id, "Я контроллер Йотик 32. Я принимаю следующие команды:", "");
      String sms = "/ledon или /ledoff - включить/выключить светодиод\n";
      sms += "/hall - считать значение с датчика Холла\n";
      bot.sendMessage(chat_id, sms, "");
    }
  }
}
