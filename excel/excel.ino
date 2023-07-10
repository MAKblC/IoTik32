int row_excel = 0; // количество строк
int val = 0; // переменная для встроенного датчика Холла, которая будет передана excel
int test_2 = 456; // вторая тестовая переменная, которая будет передана excel
void setup() {
  Serial.begin(9600); // скорость передачи данных
  Serial.println("CLEARDATA"); // очистка листа excel
  Serial.println("LABEL,Time,Hall Sensor, Test 2, Num Rows"); // заголовки столбцов
}
void loop() {
  row_excel++; // номер строки + 1
  val = hallRead(); // считывание датчика Холла
  Serial.print("DATA,TIME,"); // запись в excel текущей даты и времени
  Serial.print(val);
  Serial.print(",");
  Serial.print(test_2);
  Serial.print(",");
  Serial.println(row_excel);
  if (row_excel > 50) { // если строк больше 50, то начинаем заполнять строки по новой
    row_excel = 0;
    Serial.println("ROW,SET,2");
  }
  delay(1000); // задержка
}
