#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>
#include <BleMouse.h>

// Название нашей мышки, производителя и мнимый заряд аккумулятора
BleMouse bleMouse("IOTIK32_mouse", "MGBOT", 100);

// Wi-Fi
char ssid[] = "login";
char pass[] = "pass";

// Blynk
char auth[] = "token";
IPAddress blynk_ip(139, 59, 206, 133);

// Предыдущая позиция джойстика в Blynk
static volatile int old_dx = 639;
static volatile int old_dy = 511;

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
  bleMouse.begin();
}

void loop()
{
  Blynk.run();
}

BLYNK_WRITE(V0)
{
  int dx = param[0].asInt();
  int dy = param[1].asInt();
  Serial.println("X = " + String(dx) + " Y = " + String(dy));
  if (bleMouse.isConnected())
  {
    bleMouse.move(-(old_dx - dx), old_dy - dy);
  }
  old_dx = dx;
  old_dy = dy;
}

BLYNK_WRITE(V1)
{
  int lb = param.asInt();
  Serial.println("LEFT BUTTON = " + String(lb));
  if (lb)
    bleMouse.press(MOUSE_LEFT);
  else
    bleMouse.release(MOUSE_LEFT);
}

BLYNK_WRITE(V2)
{
  int rb = param.asInt();
  Serial.println("RIGHT BUTTON = " + String(rb));
  if (rb)
    bleMouse.press(MOUSE_RIGHT);
  else
    bleMouse.release(MOUSE_RIGHT);
}
