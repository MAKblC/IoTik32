#include <WiFi.h>
#include <PubSubClient.h>
#define LED 4  // ЙоТик 32В - 18

////////////////////// Настройки //////////////////////
// Wi-Fi
const char* ssid = "XXXX";
const char* password = "XXXX";

const char* mqtt_server = "XXXXXXXX";  //адрес
int mqtt_port = XXXXXXX;               //порт
const char* mqtt_login = "XXXXXXX";    // пользователь
const char* mqtt_pass = "XXXXXXX";     //пароль

WiFiClient espClient;
PubSubClient client(espClient);
unsigned long lastMsg = 0;
#define MSG_BUFFER_SIZE (50)
char msg[MSG_BUFFER_SIZE];
int value = 0;

void setup_wifi() {

  delay(10);
  // Подключение к сети
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  randomSeed(micros());

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

// функция приёма и обработки пакета данных
void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();

  // Если принята единица то включить светодиод
  if ((char)payload[0] == '1') {
    digitalWrite(LED, 1);
  } else {
    digitalWrite(LED, 0);  // при нуле - выключить
  }
}

// Подключение к серверу
void reconnect() {
  // Ждем, пока не подсоединимся
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Создается случайный номер клиента
    String clientId = "IoTik32Client-";
    clientId += String(random(0xffff), HEX);
    // Попытка подключения
    if (client.connect(clientId.c_str(), mqtt_login, mqtt_pass)) {
      Serial.println("connected");
      // ... и подписки на топик
      client.subscribe("XXXXXXX");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // 5 секунд для повторного подключения
      delay(5000);
    }
  }
}

void setup() {
  pinMode(LED, OUTPUT);
  Serial.begin(115200);
  setup_wifi();
  client.setServer(mqtt_server, mqtt_port);
  client.setCallback(callback);
}

void loop() {

  if (!client.connected()) {
    reconnect();
  }
  client.loop();
}
