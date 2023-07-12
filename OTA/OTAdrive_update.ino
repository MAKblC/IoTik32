#include <otadrive_esp.h>                                     // Подключаем библиотеку для обновления по OTAdrive

#define API_Key "e6983e6a-205e-4221-972a-70a1b80bcdce"        // API ключ устройства
#define FW_Version "1.0.1"                                    // Текущая версия прошивки 
#define UpdReqTime 30                                         // Время через которое запрашивается обновление

#define ssid "MGBot"
#define password "Terminator812"

#define LED 18                                                // 4 для ЙоТик32А \ 18 для ЙоТик32В.
bool flagLED;

void onUpdateProgress(int progress, int totalt);              // Инициализируем метод отображения обновления

void setup() {
  Serial.begin(115200);

  OTADRIVE.setInfo(API_Key, FW_Version);                      // Сохраняем информацию о текущей версии прошивки

  pinMode(LED, OUTPUT);

  WiFi.begin(ssid, password);
  Serial.print("Wi-Fi - connected. IP: ");
  Serial.println(WiFi.localIP());
}

void loop() {

  if (WiFi.status() == WL_CONNECTED) {

    flagLED = !flagLED;                                        // Изменение флага каждый раз на противоположное значение
    digitalWrite(LED, flagLED);                                // Включение и выключение светодиода

    if (OTADRIVE.timeTick(UpdReqTime)) {                       // Каждые UpdReqTime секунд запрашиваем информацию         //появилась ли на сервере новая прошивка

      updateInfo inf = OTADRIVE.updateFirmwareInfo();          // Информация о текущей прошивке
      Serial.printf("\nfirmware info: %s ,%dBytes\n%s\n",
                    inf.version.c_str(), inf.size, inf.available ? "New version available" : "No newer version");

      if (inf.available)                                       // Если есть новая прошивка -> обновляемся
        OTADRIVE.updateFirmware();
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
    //пишем каждые 10%
    Serial.printf("%d", progressPercent);
  }
  last = progressPercent;
}
