// библиотеки
#include <iocontrol.h>
#include <WiFi.h>
// Wi-Fi
const char* ssid = "XXXXX";
const char* password = "XXXXXXXXXXXX";

// Название панели на сайте iocontrol.ru
const char* myPanelName = "myPanel";
int status;
// Название переменных как на сайте iocontrol.ru
const char* VarName_sensor = "Hallsensor";
const char* VarName_button = "LED";
const char* VarName_slider = "Brightness";
const char* VarName_input = "Number";

#define PWMA 18  // пин светодиода для ШИМ

// Создаём объект клиента
WiFiClient client;
// Создаём объект iocontrol, передавая название панели и клиента
iocontrol mypanel(myPanelName, client);

void setup() {
  ledcAttachPin(PWMA, 1);  // инициализация ШИМ
  ledcSetup(1, 50, 10);
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
    long io_LED = mypanel.readInt(VarName_button);
    // если позиция на ВКЛ
    if (io_LED == 1) {
      int mySlider = mypanel.readInt(VarName_slider);
      // установить ШИМ
      ledcWrite(1, mySlider * 10.23);
    } else {
      // выключить светодиод
      ledcWrite(1, 0);
    }
    // считывание вводимого числа
    int myNumber = mypanel.readInt(VarName_input);
    Serial.println(myNumber);
  }
  // ************************ ЗАПИСЬ ************************
  // Записываем  значение в переменную для отправки в сервис
  mypanel.write(VarName_sensor, hallRead());  // Записали значение
  // Отправляем переменные из контроллера в сервис
  status = mypanel.writeUpdate();
  // Если статус равен константе OK...
  if (status == OK) {
    // Выводим текст в последовательный порт
    Serial.println("------- Write OK -------");
  }
}
