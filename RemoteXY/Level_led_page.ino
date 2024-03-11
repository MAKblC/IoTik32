// выбор модуля и способа связи
#define REMOTEXY_MODE__ESP32CORE_BLE
// библиотека для BLE
#include <BLEDevice.h>
// Имя устройства
#define REMOTEXY_BLUETOOTH_NAME "IoTik32"
// библиотека RemoteXY
#include <RemoteXY.h>

// конфигурация графического интерфейса
#pragma pack(push, 1)
uint8_t RemoteXY_CONF[] =  // 104 bytes
  { 255, 1, 0, 2, 0, 97, 0, 17, 0, 0, 0, 31, 1, 106, 200, 3, 1, 0, 0, 3,
    0, 131, 10, 8, 40, 14, 2, 1, 2, 31, 208, 175, 209, 128, 208, 186, 208, 190, 209, 129,
    209, 130, 209, 140, 0, 37, 131, 57, 8, 40, 14, 2, 17, 2, 24, 208, 152, 208, 189, 208,
    180, 208, 184, 208, 186, 208, 176, 209, 130, 208, 190, 209, 128, 0, 25, 4, 12, 33, 82, 10,
    128, 2, 26, 1, 0, 66, 11, 61, 14, 32, 1, 2, 26, 1, 0, 70, 71, 68, 18, 18,
    17, 26, 234, 0 };

// структура со всеми переменными для связи с элементами
struct {
  // входные
  int8_t slider_01;  // =0..100 положение слайдера

  // выходные
  int8_t level_01;  // =0..100 положение уровня
  uint8_t led_01;   // состояние светодиода

  // другие
  uint8_t connect_flag;  // =1 if wire connected, else =0

} RemoteXY;
#pragma pack(pop)

#define PWMA 18  // пин светодиода для ШИМ

void setup() {
  ledcAttachPin(PWMA, 1);  // инициализация ШИМ
  ledcSetup(1, 50, 10);
  RemoteXY_Init();
}

void loop() {
  // обработчик событий
  RemoteXY_Handler();
  // установить яркость светодиода слайдером
  ledcWrite(1, RemoteXY.slider_01 * 1023 / 100);
  // отобразить значение слайдера на уровне
  RemoteXY.level_01 = RemoteXY.slider_01;
  // включить индикатор, если слайдер включен на полную
  if (RemoteXY.slider_01 == 100) {
    RemoteXY.led_01 = true;
  } else {
    RemoteXY.led_01 = false;
  }
}
