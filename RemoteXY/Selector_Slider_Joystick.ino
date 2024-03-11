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
uint8_t RemoteXY_CONF[] =  // 44 bytes
  { 255, 4, 0, 0, 0, 37, 0, 17, 0, 0, 0, 31, 1, 106, 200, 1, 1, 3, 0, 3,
    8, 9, 44, 10, 133, 2, 26, 4, 63, 7, 36, 12, 128, 2, 26, 5, 39, 29, 32, 32,
    0, 2, 26, 31 };

// структура со всеми переменными для связи с элементами
struct {
  // входные
  uint8_t select_01;     // =0 если переключатель в положении A, =1 если в положении B, =2 если в положении C, ...
  int8_t slider_01;      // =0..100 положение слайдера
  int8_t joystick_01_x;  // oт -100 до 100}
  int8_t joystick_01_y;  // oт -100 до 100}

  // другие
  uint8_t connect_flag;  // =1 if wire connected, else =0

} RemoteXY;
#pragma pack(pop)

#define PWMA 18  // пин светодиода для ШИМ

void setup() {
  Serial.begin(115200);
  ledcAttachPin(PWMA, 1);  // инициализация ШИМ
  ledcSetup(1, 50, 10);
  RemoteXY_Init();
}

void loop() {
  // обработчик событий
  RemoteXY_Handler();
  // уровни яркости на переключателе
  ledcWrite(1, RemoteXY.select_01 * 1023 / 4);
  // вывод значения слайдера при определенных позициях джойстика
  if (RemoteXY.joystick_01_x == 100 or RemoteXY.joystick_01_y == -100) {
    Serial.println("Позиция слайдера: " + String(RemoteXY.slider_01));
  }
}
