// Network settings
// --
const char* ssid = "wifi";
const char* password = "password";
// --

// Time settings
const char* ntpServerName = "pool.ntp.org";
const int TIMEZONE=3;

// narmon

#define SRV   "185.245.187.136"
#define MAC  "macadress"
#define PASS  "pass"
#define USERNAME  "username"
#define TOPIC  "login/devicename/"

//--

//mqtt4narmon
char server[] = SRV ;
char authMethod[] = USERNAME;
char token[] = PASS;
char clientId[] = MAC;
char conntopic[] = TOPIC "status";

// LED
const int LED_BUILTIN=2;  //Onboard LED Pin


// Application settings
