// библиотеки
#include <iocontrol.h>
#include <WiFi.h>
// Название панели на сайте iocontrol.ru
const char* myPanelName = "myPanel";
int status;
String myString;
// Название переменных как на сайте iocontrol.ru
const char* VarName_sensor = "Hall";
const char* VarName_display_text = "system_status";
const char* VarName_text_input = "Word";
// Wi-Fi
const char* ssid = "XXXXX";
const char* password = "XXXXXXXXXX";

// Создаём объект клиента
WiFiClient client;
// Создаём объект iocontrol, передавая название панели и клиента
iocontrol mypanel(myPanelName, client);

void setup() {

  Serial.begin(115200);
  WiFi.begin(ssid, password);
  // Ждём подключения
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  // Вызываем функцию первого запроса к сервису
  mypanel.begin();
}
void loop() {
  // ************************ ЧТЕНИЕ ************************
  // Чтение значений переменных из сервиса
  status = mypanel.readUpdate();
  // Если статус равен константе OK...
  if (status == OK) {
    // Выводим текст в последовательный порт
    Serial.println("------- Read OK -------");
    // Записываем считанный из сервиса значения в переменные
    String myString = mypanel.readString(VarName_text_input);
    // Выводим значение в монитор последовательного порта
    Serial.println(myString);
  }
  // ************************ ЗАПИСЬ ************************
  // Записываем  значение в переменную для отправки в сервис
  float hall = hallRead() + 0.3;
  Serial.println(hall);
  mypanel.write(VarName_sensor, hall);  // Записали значение
  if (hall > 100) {
    myString = "danger";
  } else {
    myString = "OK";
  }
  // записали фразу
  mypanel.write(VarName_display_text, myString);
  // Отправляем переменные из контроллера в сервис
  status = mypanel.writeUpdate();
  // Если статус равен константе OK...
  if (status == OK) {
    // Выводим текст в последовательный порт
    Serial.println("------- Write OK -------");
  }
}
