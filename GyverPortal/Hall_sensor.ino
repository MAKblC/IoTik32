// тест графиков AJAX_PLOT, вывод случайных значений
#define AP_SSID ""
#define AP_PASS ""
#include <GyverPortal.h>
GyverPortal portal;
// конструктор
void build() {
  GP.BUILD_BEGIN(1900);    // ширина колонки (ширина мониторо – 100)
  GP.THEME(GP_DARK);
  GP.GRID_RESPONSIVE(700);         // располагать блоки по вертикали, если ширина меньше //указанной. int, в пикселях (например 700). Указывать ПОСЛЕ GP.THEME()!!!
  GP.AJAX_PLOT("plot1", 1, 20, 1000, 700, 0); 
 //GP.AJAX_PLOT(имя, к-во осей, к-во точек по Х, период update, высота графика,  //bool local = 0);
  GP.BUILD_END();
}
void setup() {
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  WiFi.begin(AP_SSID, AP_PASS);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println(WiFi.localIP());
  // подключаем конструктор и запускаем
  portal.attachBuild(build);
  portal.attach(action);
  portal.start();
}
void action() {
  if (portal.update("plot1")) portal.answer(hallRead()); 
//отправляем значения от датчика хола в график
}
void loop() {
  portal.tick();
}
