// настройки сети
// --
const char* ssid = "wifi";
const char* password = "password";
// --

// настройки времени
const char* ntpServerName = "pool.ntp.org";
const int TIMEZONE=3;

// народный мониторинг
#define SRV   "185.245.187.136"
#define MAC  "macadress"
#define PASS  "pass"
#define USERNAME  "username"
#define TOPIC  "login/devicename/"

//MQTT
char server[] = SRV ;
char authMethod[] = USERNAME;
char token[] = PASS;
char clientId[] = MAC;
char conntopic[] = TOPIC "status";

// LED
const int LED_BUILTIN=4;  //светодиод на борту Йотика 32
