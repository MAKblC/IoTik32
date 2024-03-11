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
uint8_t RemoteXY_CONF[] =  // 36 bytes
  { 255, 14, 0, 0, 0, 29, 0, 17, 0, 0, 0, 31, 1, 106, 200, 1, 1, 2, 0, 6,
    10, 9, 40, 40, 2, 26, 7, 61, 22, 40, 10, 36, 2, 26, 2, 11 };

// структура со всеми переменными для связи с элементами
struct {
  // входные
  uint8_t rgb_01_r;  // =0..255 значение Красного цвета
  uint8_t rgb_01_g;  // =0..255 значение Зеленого цвета
  uint8_t rgb_01_b;  // =0..255 значение Синего цвета
  char edit_01[11];  // string UTF8 end zero

  // другие
  uint8_t connect_flag;  // =1 if wire connected, else =0

} RemoteXY;
#pragma pack(pop)

#define LED 18  // пин светодиода для ШИМ

void setup() {
  Serial.begin(115200);
  pinMode(LED, OUTPUT);
  RemoteXY_Init();
}

void loop() {
  // обработчик событий
  RemoteXY_Handler();
  // включить светодиод, если бегунок находится в зоне W
  if (RemoteXY.rgb_01_r == 255 or RemoteXY.rgb_01_g == 255 or RemoteXY.rgb_01_b == 255) {
    digitalWrite(LED, HIGH);
  } else {
    digitalWrite(LED, LOW);
  }
  // отображать фразу из поля ввода, если бегунок находится в зоне B
  if (RemoteXY.rgb_01_r == 0 or RemoteXY.rgb_01_g == 0 or RemoteXY.rgb_01_b == 0) {
    Serial.println(RemoteXY.edit_01);
  }
}
