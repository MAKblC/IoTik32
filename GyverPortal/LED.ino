#define AP_SSID ""
#define AP_PASS ""
#include <GyverPortal.h>
GyverPortal portal;
#define LedBord 4 // 18, если ЙоТик 32В
// переменные
bool valCheck;
bool valSwitch;
bool valBtn;
String BtnName = "On/Off";
// конструктор страницы
void build() {
  BUILD_BEGIN();        //запускаем конструктор
  GP.THEME(GP_DARK);    //устанавливаем тёмную тему
  GP.AJAX_UPDATE("led");//создаём лист компонентов с динамическим изменением
  //Выстраиваем компонента объедения в блоки
  GP.TITLE("ЙоТик32");
  GP.HR();
  GP_MAKE_BLOCK_TAB(
    "Панель управления",
    GP_MAKE_BLOCK(
      GP.LABEL("LED state");
      GP.LED_RED("led", false);
    );
    GP.HR();
    GP_MAKE_BLOCK_THIN(
      GP_MAKE_BOX(GP.BUTTON_MINI("btn", BtnName); GP.CHECK("ch", valCheck); GP.SWITCH("sw", valSwitch););
    );
  );
  BUILD_END(); //Завершаем строительство страницы
}
void setup() {
  Serial.begin(115200);
  pinMode(LedBord, OUTPUT);
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
  // настраиваем лист автообновления
  // это позволит автоматически менять переменные привязанные к элементам управления
  // иначе пришлось бы прописывать их изменение в реакции на клик
  portal.list.init(2); //кол-во автообновляемых элементов
  portal.list.add(&valCheck, "ch", T_CHECK);
  portal.list.add(&valSwitch, "sw", T_CHECK);
}
void action() {
  if (portal.click()) { //реакция на нажатие
    // выводим новые значения с компонентов
    if (portal.click("ch")) {
      Serial.print("Check: ");
      Serial.println(valCheck);
    }
    if (portal.click("sw")) {
      Serial.print("Switch: ");
      Serial.println(valSwitch);
    }
    if (portal.click("btn")) {
      valBtn = !valBtn;
    }
  }
  if (portal.update()) { //при обновлении
    //Управление индикативным UI светодиодом
    if (portal.update("led")) portal.answer((valBtn || valSwitch || valCheck));
    //Управление светодиодом на плате
    digitalWrite(LedBord, (valBtn || valSwitch || valCheck));
    // структура (valBtn || valSwitch || valCheck)
    // позволяет реагировать на 3 элемента стразу
  }
}
void loop() {
  portal.tick();
}
