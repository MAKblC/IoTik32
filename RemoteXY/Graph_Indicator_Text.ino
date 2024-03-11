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
uint8_t RemoteXY_CONF[] =  // 120 bytes
  { 255, 0, 0, 20, 0, 113, 0, 17, 0, 0, 0, 31, 1, 106, 200, 1, 1, 5, 0, 72,
    15, 10, 31, 31, 12, 2, 26, 140, 38, 0, 0, 200, 194, 0, 0, 200, 66, 0, 0, 0,
    0, 71, 63, 8, 30, 30, 56, 0, 2, 24, 135, 0, 0, 200, 194, 0, 0, 200, 66, 0,
    0, 72, 66, 0, 0, 32, 65, 0, 0, 0, 64, 24, 0, 68, 8, 45, 91, 47, 1, 8,
    36, 67, 10, 101, 88, 14, 5, 2, 26, 11, 129, 12, 118, 84, 12, 17, 208, 148, 208, 176,
    209, 130, 209, 135, 208, 184, 208, 186, 32, 208, 165, 208, 190, 208, 187, 208, 187, 208, 176, 0 };

// структура со всеми переменными для связи с элементами
struct {
  // выходные
  int8_t circularbar_01;  // oт -100 до 100
  float instrument_01;    // oт -100 до 100
  float onlineGraph_01_var1;
  char text_01[11];  // =строка UTF8 оканчивающаяся нулем

  // другие
  uint8_t connect_flag;  // =1 if wire connected, else =0

} RemoteXY;
#pragma pack(pop)

void setup() {
  RemoteXY_Init();
}

void loop() {
  // обработчик событий
  RemoteXY_Handler();
  // считывание датчика Холла
  int val = hallRead();
  // вывод в стрелочный индикатор
  RemoteXY.instrument_01 = val;
  // вывод в круговой уровень
  RemoteXY.circularbar_01 = val;
  // вывод на график
  RemoteXY.onlineGraph_01_var1 = val;
  // перевод значения в char и вывод в строке
  itoa(val, RemoteXY.text_01, 10);
}
