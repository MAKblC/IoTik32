int val = 0;
void setup() {
  Serial.begin(115200);
}

void loop() {
  val = hallRead(); // функция считывания значения с датчика Холла
  Serial.println(val);
  delay(500);
}
