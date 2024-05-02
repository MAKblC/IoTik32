#include <Wire.h>
#include "TOD.h"
template<class T> inline Print &operator <<(Print &obj, T arg) {
  obj.print(arg);
  return obj;
}
TOD RTC;
uint8_t lastminute = 0;
uint8_t lastsecond = 45;
char printbuffer[50];
bool TODvalid = false;
char ssid[] = "login";  //  логин
char password[] = "pass"; // пароль
int x = 0;
void setup()
{
  Serial.begin(115200);
  if (RTC.begin(ssid, password))TODvalid = true; // проверка подключения к серверу
  lastminute = RTC.minute();
}
void loop()
{
  if (RTC.second() != lastsecond && TODvalid) // выводим в порт время каждую секунду
  {
    lastsecond = RTC.second();
    sprintf(printbuffer, "   UTC Time:%02i:%02i:%02i.%03i\n", RTC.hour() + 3, RTC.minute(), RTC.second(), RTC.millisec());
    Serial << printbuffer;
  }
  if (RTC.hour() == 11 and RTC.minute() == 0 and x == 0) { // если по гринвичу 11-00 (14-00 МСК) то
    Serial.println("alarm!"); // включаем будильник  на 10 секунд
    delay(10000);
    Serial.println("alarm off!");
    x = 1; // устанавливаем флаг, чтобы он не включился повторно в ту же минуту
  }
  if (RTC.hour() == 11 and RTC.minute() == 1) { // в 14-01 сбрасываем флаг
    x = 0;
  }
  /*if(RTC.minute()==lastminute+3 && TODvalid) // перепроверка времени с сервером каждые 3 минуты
    {
     lastminute=RTC.minute();
     RTC.begin(ssid,password);
    }*/
}
