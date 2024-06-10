// логин-пароль от роутера
#define AP_SSID "XXXXX"
#define AP_PASS "XXXXXXXXX"

#include <Arduino.h>
#include <GyverHub.h>
//GyverHub hub;
GyverHub hub("MyDevices", "IoTik32", "");  // можно настроить тут название и каталог проектов

// обработчик кнопки
void btn_cb() {
  static bool flag = 0;
  flag = !flag;
  digitalWrite(4, flag);
  digitalWrite(18, flag);
  Serial.println("click led");
}

// билдер
void build(gh::Builder& b) {
  b.Title("Тестовый скетч"); // заголовок страницы
  b.Button().label(F("Управление светодиодом")).attach(btn_cb); // к кнопке закрепляется название и чем управлять
  b.GaugeRound_("gagl").label("Датчик холла").size(2).range(-400, 400, 1); // к круговому индикатору прикрепляется переменная для считывания, название, размер, диапазон
}

void setup() {
  Serial.begin(115200);

#ifdef GH_ESP_BUILD
  // подключение к роутеру
  WiFi.mode(WIFI_STA);
  WiFi.begin(AP_SSID, AP_PASS);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println();
  Serial.println(WiFi.localIP());

  // режим точки доступа
  // WiFi.mode(WIFI_AP);
  // WiFi.softAP("My Hub");
  // Serial.println(WiFi.softAPIP());    // по умолч. 192.168.4.1
#endif

  // подключить билдер
  hub.onBuild(build);

  // запуск!
  hub.begin();

  pinMode(4, OUTPUT);
  pinMode(18, OUTPUT);
}

void loop() {
  // =================== ТИКЕР ===================
  // вызываем тикер в главном цикле программы
  // он обеспечивает работу связи, таймаутов и прочего
  hub.tick();

  // =========== ОБНОВЛЕНИЯ ПО ТАЙМЕРУ ===========
  // в библиотеке предусмотрен удобный класс асинхронного таймера
  static gh::Timer tmr(1000);  // период 1 секунда

  // каждую секунду будем обновлять значения Холла
  if (tmr) {
    hub.update("gagl").value(hallRead());
  }
}
