#include <iocontrol.h>
#include <WiFi.h>
// Название панели на сайте iocontrol.ru
const char* myPanelName = "panelname";
int status;
// Название переменных как на сайте iocontrol.ru
const char* VarName_Hallsensor = "Hallsensor";
const char* VarName_LED = "LED";
const char* ssid = "wifilogin";
const char* password = "wifipass";
#define LED 4 // ЙоТик 32А – 4, ЙоТик 32В - 18
// Создаём объект клиента
WiFiClient client;
// Создаём объект iocontrol, передавая название панели и клиента
iocontrol mypanel(myPanelName, client);

void setup()
{
  pinMode(LED, OUTPUT);
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
void loop()
{
  // ************************ ЧТЕНИЕ ************************
  // Чтение значений переменных из сервиса
  status = mypanel.readUpdate();
  // Если статус равен константе OK...
  if (status == OK) {
    // Выводим текст в последовательный порт
    Serial.println("------- Read OK -------");
    // Записываем считанный из сервиса значения в переменные
    long io_LED = mypanel.readInt(VarName_LED);   
    Serial.println((String)"io_LED = " + io_LED);
    digitalWrite(LED, io_LED);
  }
  // ************************ ЗАПИСЬ ************************
  // Записываем  значение в переменную для отправки в сервис
  mypanel.write(VarName_Hallsensor, hallRead());    // Записали значение 
  // Отправляем переменные из контроллера в сервис
  status = mypanel.writeUpdate();
  // Если статус равен константе OK...
  if (status == OK) {
    // Выводим текст в последовательный порт
    Serial.println("------- Write OK -------");
  }
}
