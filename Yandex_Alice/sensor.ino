#include <WiFi.h>
#include <PubSubClient.h> // библиотека для MQTT

/////////////////// Настройки /////////////////////////////

// Wi-Fi
const char* ssid = "XXXX";
const char* password = "XXXX";

// MQTT
const char* mqtt_server = "XXXX";     //ip или http адрес
int mqtt_port = XXXX;                 //порт
const char* mqtt_user = "XXXX";       //логин
const char* mqtt_password = "XXXXX";  //пароль

// SENSOR
const int sending_period = 5; // период отправки
const bool retain_flag = false;
const char* hall_topic = "hall"; // название топика
/////////////////////////////////////////////////////////

WiFiClient espClient;
PubSubClient client(espClient);
uint32_t tmr1;

void setup_wifi() {
  // подключение
  delay(10);
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void reconnect() {
  // подключение к серверу
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    String clientId = "IoTik32Client-" + WiFi.macAddress();
    if (client.connect(clientId.c_str(), mqtt_user, mqtt_password)) {
      Serial.println("connected");

    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      delay(5000);
    }
  }
}

// отправка данных в топик
void sendData() {
  float hall = hallRead();

  Serial.print("HallSensor = ");
  Serial.println(hall);

  client.publish(hall_topic, String(hall).c_str(), retain_flag);
}

void setup() {
  Serial.begin(115200);

  setup_wifi();
  client.setServer(mqtt_server, mqtt_port);
}

void loop() {
  // подключаемся к серверу
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  if (millis() - tmr1 >= (sending_period * 1000)) {
    tmr1 = millis();
    sendData();
  }
}
