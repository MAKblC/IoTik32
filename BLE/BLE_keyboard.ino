#define BLYNK_PRINT Serial

#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>
#include <BleKeyboard.h>

// Название нашей мышки, производителя и мнимый заряд аккумулятора
BleKeyboard bleKeyboard("IOTIK32_keyboard", "MGBOT", 100);

// Wi-Fi
char ssid[] = "login";
char pass[] = "pass";

// Blynk
char auth[] = "token";
IPAddress blynk_ip(139, 59, 206, 133);

void setup()
{
  // Инициализация UART для монитора порта
  Serial.begin(115200);
  delay(500);
  Serial.println();
  Serial.println();
  Serial.println();
  Serial.println("Starting BLE work!");

  // Подключение к Wi-Fi и Blynk
  Serial.print("Connecting to ");
  Serial.println(ssid);
  Blynk.begin(auth, ssid, pass, blynk_ip, 8442);
  delay(500);
  int wifi_status = WiFi.status();
  int blynk_status = Blynk.connected();
  if ((wifi_status == WL_CONNECTED) && (blynk_status))
  {
    Serial.println("");
    Serial.println("WiFi connected");
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());
    Serial.println();
  }

  // Инициализация BLE
  bleKeyboard.begin();
}

void loop()
{
  Blynk.run();
}

BLYNK_WRITE(V0)
{
  int my_key = param.asInt();
  if (my_key)
    Serial.println("Key pressed");
  if (bleKeyboard.isConnected())
  {
    if (my_key)
      bleKeyboard.write(KEY_MEDIA_CALCULATOR);
  }
}

BLYNK_WRITE(V1)
{
  int my_key = param.asInt();
  if (my_key)
    Serial.println("Key pressed");
  if (bleKeyboard.isConnected())
  {
    if (my_key)
      bleKeyboard.write(KEY_MEDIA_LOCAL_MACHINE_BROWSER);
  }
}

BLYNK_WRITE(V2)
{
  int my_key = param.asInt();
  if (my_key)
    Serial.println("Key pressed");
  if (bleKeyboard.isConnected())
  {
    if (my_key)
      bleKeyboard.write(KEY_MEDIA_VOLUME_DOWN);
  }
}

BLYNK_WRITE(V3)
{
  int my_key = param.asInt();
  if (my_key)
    Serial.println("Key pressed");
  if (bleKeyboard.isConnected())
  {
    if (my_key)
      bleKeyboard.write(KEY_MEDIA_VOLUME_UP);
  }
}

BLYNK_WRITE(V4)
{
  int my_key = param.asInt();
  if (my_key)
    Serial.println("Key pressed");
  if (bleKeyboard.isConnected())
  {
    if (my_key)
      bleKeyboard.write(KEY_MEDIA_MUTE);
  }
}
