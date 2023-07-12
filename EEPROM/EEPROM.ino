#include <EEPROM.h> // библиотека
#define EEPROM_SIZE 100 // указание размера используемой памяти

String ssid = "Hellow"; // данные
String pass = "World"; // данные

void setup() {
  Serial.begin(115200);
  // инициализировать EEPROM с предопределенным размером
  EEPROM.begin(EEPROM_SIZE);
  delay(250);
  Serial.println(sizeof(ssid)); // отобразить размер переменной
  EEPROM.put(1, ssid); // записать данные из переменной в адрес
  EEPROM.commit(); // подтвердить запись
}

void loop() {
  String s;
  EEPROM.get(1, s); // поместить данные из ячейки (№1) в переменную s
  Serial.println(s); // вывести в монитор
  delay(500);
}
