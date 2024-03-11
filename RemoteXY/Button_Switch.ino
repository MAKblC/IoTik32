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
uint8_t RemoteXY_CONF[] =  // 62 bytes
  { 255, 3, 0, 0, 0, 55, 0, 17, 0, 0, 0, 31, 1, 106, 200, 1, 1, 3, 0, 1,
    16, 25, 24, 24, 0, 2, 31, 0, 10, 67, 25, 24, 24, 48, 4, 26, 31, 79, 78, 0,
    31, 79, 70, 70, 0, 2, 33, 63, 44, 22, 0, 2, 26, 31, 31, 79, 78, 0, 79, 70,
    70, 0 };

// структура со всеми переменными для связи с элементами
struct {
  // входные
  uint8_t button_01;      // =1 if button pressed, else =0
  uint8_t pushSwitch_01;  // =1 if state is ON, else =0
  uint8_t switch_01;      // =1 if switch ON and =0 if OFF

  // другие
  uint8_t connect_flag;  // =1 if wire connected, else =0

} RemoteXY;
#pragma pack(pop)

#define LED 18  // 4

void setup() {
  pinMode(LED, OUTPUT);
  // запуск
  RemoteXY_Init();
}

void loop() {
  // обработчик событий
  RemoteXY_Handler();

  if (RemoteXY.switch_01) {
    // если переключатель на ON
    if (RemoteXY.button_01) {
      // кнопка нажата
      digitalWrite(LED, HIGH);
    } else {
      // кнопка не нажата
      digitalWrite(LED, LOW);
    }
  } else {
    // если переключатель на OFF
    if (RemoteXY.pushSwitch_01) {
      // кнопка с фиксацией нажата
      digitalWrite(LED, HIGH);
    } else {
      // кнопка с фиксацией не нажата
      digitalWrite(LED, LOW);
    }
  }
}
