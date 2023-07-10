#include <WiFi.h> // библиотека Wi-Fi
const char* ssid = "login"; // логин
const char* password =  "password"; // пароль
String translateEncryptionType(wifi_auth_mode_t encryptionType) {
  // функция определения спецификации шифрования точки доступа
  switch (encryptionType) {
    case (WIFI_AUTH_OPEN):
      return "Open";
    case (WIFI_AUTH_WEP):
      return "WEP";
    case (WIFI_AUTH_WPA_PSK):
      return "WPA_PSK";
    case (WIFI_AUTH_WPA2_PSK):
      return "WPA2_PSK";
    case (WIFI_AUTH_WPA_WPA2_PSK):
      return "WPA_WPA2_PSK";
    case (WIFI_AUTH_WPA2_ENTERPRISE):
      return "WPA2_ENTERPRISE";
  }
}
void scanNetworks() { // сканирование доступных Wi-Fi соединений
  int numberOfNetworks = WiFi.scanNetworks();
  Serial.print("Number of networks found: ");
  Serial.println(numberOfNetworks);
  for (int i = 0; i < numberOfNetworks; i++) {
    Serial.print("Network name: "); // имя сети
    Serial.println(WiFi.SSID(i));
    Serial.print("Signal strength: "); // уровень сигнала
    Serial.println(WiFi.RSSI(i));
    Serial.print("MAC address: "); // МAC-адрес
    Serial.println(WiFi.BSSIDstr(i));
    Serial.print("Encryption type: "); // тип шифрования
    String encryptionTypeDescription = translateEncryptionType(WiFi.encryptionType(i));
    Serial.println(encryptionTypeDescription);
    Serial.println("-----------------------");
  }
}
void connectToNetwork() {
  // функция подключения ESP32 к выбранной Wi-Fi точке
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Establishing connection to WiFi..");
  }
  Serial.println("Connected to network");
}
void setup() {
  Serial.begin(115200);
  scanNetworks(); // сканирование сетей
  connectToNetwork(); // подключение к выбранной Wi-Fi
  Serial.println(WiFi.macAddress()); // Определение MAC-адреса устройства
  Serial.println(WiFi.localIP()); // Определение локального IP-адреса
  WiFi.disconnect(true); // отключение от Wi-Fi
  Serial.println(WiFi.localIP()); // проверка, что действительно отключились
}
void loop() {}
