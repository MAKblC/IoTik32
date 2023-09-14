#include <WiFi.h>
#include <JSON_Decoder.h>
#include <OpenWeather.h>
// Для считывания времени
#include <Time.h>
#define TIME_OFFSET 1UL * 3600UL // UTC + 0 часов
// Данные Wi-FI
#define WIFI_SSID     "XXXXXXXXXXXX"
#define WIFI_PASSWORD "XXXXXXXXXXXXXX"

String api_key = "XXXXXXXXXXXXXXXXXXXXX"; // API ключ

String latitude =  "59.9386"; // широта
String longitude = "30.3141"; // долгота

String units = "metric";  // или, например, "imperial"
String language = "ru";   // язык

OW_Weather ow; // Создается экзампляр класса

void setup() {
  Serial.begin(115200); 
  Serial.printf("\n\nПодсоединяюсь к %s\n", WIFI_SSID);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println();
  Serial.print("Соединение установлено\n");
}

void loop() {
  printForecast();
  // 1000 запросов в день
  delay(20000); // Каждые 5 минут = 288 запросов в день
}

/***************************************************************************************
**                         Послать прогноз погоды в монтиор порта
***************************************************************************************/
void printForecast()
{
  // Создание структуры для хранения данных погоды
  OW_forecast  *forecast = new OW_forecast;
  Serial.print("\nЗапрос погоды от OpenWeather... ");
  ow.getForecast(forecast, api_key, latitude, longitude, units, language);
  
  Serial.println("Погода от сервиса OpenWeather\n");

  Serial.print("Населенный пункт/район: "); Serial.println(forecast->city_name);
  Serial.print("Время восхода         : "); Serial.println(strTime(forecast->sunrise));
  Serial.print("Время заката          : "); Serial.println(strTime(forecast->sunset));
  Serial.print("Широта                : "); Serial.println(ow.lat);
  Serial.print("Долгота               : "); Serial.println(ow.lon);
  Serial.println();

  if (forecast)
  {
    Serial.println("###############  Прогноз погоды на ближайшее время  ###############\n");
    // for (int i = 0; i < (MAX_DAYS * 8); i++) // на MAX-DAYS дней (8 прогнозов в день с периодичностью в 3 часа) можно менять размер цикла
    for (int i = 0; i < 1; i++) // на ближайшее время (однократный прогноз на ближайшие ~3 часа
    {
      Serial.print("Прогноз погоды №"); if (i < 10) Serial.print(" "); Serial.print(i);
      Serial.println();
      Serial.print("Время прогноза: "); Serial.print(strTime(forecast->dt[i]));
      Serial.println();
      Serial.print("Температура             : "); Serial.println(forecast->temp[i]);
      Serial.print("Минимальная температура : "); Serial.println(forecast->temp_min[i]);
      Serial.print("Максимальная температура: "); Serial.println(forecast->temp_max[i]);
      Serial.println();
      Serial.print("Давление               : "); Serial.println(forecast->pressure[i]);
      Serial.print("Давление на уровне моря: "); Serial.println(forecast->sea_level[i]);
      Serial.print("Давление на уровне суши: "); Serial.println(forecast->grnd_level[i]);
      Serial.print("Влажность              : "); Serial.println(forecast->humidity[i]);
      Serial.println();
      Serial.print("Облачность       : "); Serial.println(forecast->clouds_all[i]);
      Serial.print("Скорость ветра   : "); Serial.println(forecast->wind_speed[i]);
      Serial.print("Направление ветра:  "); Serial.println(forecast->wind_deg[i]);
      Serial.print("Порыв ветра      : "); Serial.println(forecast->wind_gust[i]);
      Serial.println();
      Serial.print("Видимость          : "); Serial.println(forecast->visibility[i]);
      Serial.print("Вероятность осадков: "); Serial.println(forecast->pop[i]);
      Serial.println();
      Serial.print("Время, когда сделан прогноз: "); Serial.println(forecast->dt_txt[i]);
      Serial.print("Общий прогноз              : "); Serial.println(forecast->main[i]);
      Serial.print("Описание                   : "); Serial.println(forecast->description[i]);
      Serial.println();
    }
  }
   delete forecast;
}

/***************************************************************************************
**                    Конвертация времени из UNIX в понятную строку
***************************************************************************************/
String strTime(time_t unixTime)
{
  unixTime += TIME_OFFSET;
  return ctime(&unixTime);
}
