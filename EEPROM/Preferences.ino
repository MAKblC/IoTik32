#include <Preferences.h>
Preferences preferences; // добавляем экземпляр класса

void setup() {
  Serial.begin(115200);
  Serial.println();

  // указываем собственное пространство имен (до 15 символов)
  // false - для режима чтения/записи
  preferences.begin("my-app", false);

  // удалить все настройки с пространством имен
  //preferences.clear();

  // удалить только определенный ключ
  //preferences.remove("counter");

  // получаем значение счетчика, если ключа нет, то возвращаем 0
  // Имя ключа до 15 символов
  unsigned int counter = preferences.getUInt("counter", 0);

  // увеличиваем счетчик на 1
  counter++;

  // выводим в монитор его значение
  Serial.printf("Current counter value: %u\n", counter);

  // записываем значение счетчика в preferences
  preferences.putUInt("counter", counter);

  // закрытие 
  preferences.end();

  // задержка в 10 секунд
  Serial.println("Restarting in 10 seconds...");
  delay(10000);

  // перезагрузка контроллера
  ESP.restart();
}

void loop() {}
