#include <otadrive_esp.h>                                     // Подключаем библиотеку для обновления по OTAdrive
#define API_Key "----------"        // API ключ устройства
#define FW_Version "1.0.0"                                    // Текущая версия прошивки 
#define UpdReqTime 30                                         // Время через которое запрашивается обновление
#define ssid "-----"                                             // Логин
#define password "------"                                 // Пароль от вайфая
void update();
void onUpdateProgress(int progress, int totalt);              // Инициализируем метод отображения обновления
void setup() {
  Serial.begin(115200);
  OTADRIVE.setInfo(API_Key, FW_Version);                      // Сохраняем информацию о текущей версии прошивки
  WiFi.begin(ssid, password);
}
void loop() {
  if (WiFi.status() == WL_CONNECTED) {
    if (OTADRIVE.timeTick(UpdReqTime)) {                       // Каждые UpdReqTime секунд запрашиваем информацию
      // появилась ли на сервере новая прошивка
      OTADRIVE.updateFirmware();                                    // Обновляемся если есть новая версия
    }
  }
  delay(500);
}

void onUpdateProgress(int progress, int totalt)
{
  static int last = 0;
  int progressPercent = (100 * progress) / totalt;
  Serial.print("*");
  if (last != progressPercent && progressPercent % 10 == 0)
  {
    //print every 10%
    Serial.printf("%d", progressPercent);
  }
  last = progressPercent;
}
