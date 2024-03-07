// библиотеки
#include <iocontrol.h>
#include <WiFi.h>
// Название панели на сайте iocontrol.ru
const char* myPanelName = "myPanel";
int status;
// Название переменных как на сайте iocontrol.ru
const char* VarName_sensor = "Hall";
const char* VarName_matrix = "matrix";
// Wi-Fi и ключ доступа
const char* ssid = "XXXXX";
const char* password = "XXXXXXXXX";
const char* key = "p1gXXXXXXXXXXXXxu";
// Создаём объект клиента
WiFiClient client;
// Создаём объект iocontrol, передавая название панели, ключа и клиента
iocontrol mypanel(myPanelName, key, client);
// иконки для матрицы из массива двоичных чисел
uint8_t smiley[] = {
  0b00000000,
  0b00100100,
  0b00000000,
  0b00000000,
  0b01000010,
  0b00111100,
  0b00000000,
  0b00000000
};
uint8_t sad[] = {
  0b00000000,
  0b00100100,
  0b00000000,
  0b00000000,
  0b00000000,
  0b00111100,
  0b01000010,
  0b00000000
};

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
  // ************************ ЗАПИСЬ ************************
  // Записываем  значение в переменную для отправки в сервис
  float hall = hallRead();
  mypanel.write(VarName_sensor, hall);  // Записали значение
  // запись нужной структуры в матрицу
  if (hall > 100) {
    mypanel.writeMatrix(VarName_matrix, sad);
  } else {
    mypanel.writeMatrix(VarName_matrix, smiley);
  }

  // Отправляем переменные из контроллера в сервис
  status = mypanel.writeUpdate();
  // Если статус равен константе OK...
  if (status == OK) {
    // Выводим текст в последовательный порт
    Serial.println("------- Write OK -------");
  }
}
