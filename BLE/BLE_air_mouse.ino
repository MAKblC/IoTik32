#include <Wire.h>
#include <BleMouse.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_LSM9DS1.h>

// Название нашей мышки, производителя и мнимый заряд аккумулятора
BleMouse bleMouse("IOTIK32_mouse", "MGBOT", 100);

// Датчик LSM9DS1
Adafruit_LSM9DS1 imu = Adafruit_LSM9DS1();

void setup()
{
  // Инициализация UART для монитора порта
  Serial.begin(115200);
  delay(500);
  Serial.println();
  Serial.println();
  Serial.println();
  Serial.println("Starting BLE work!");

  // Инициализация датчика LSM9DS1
  if (!imu.begin())
  {
    Serial.println("Unable to initialize the LSM9DS1. Check your wiring!");
    while (1);
  }
  Serial.println("Found LSM9DS1 9DOF");
  imu.setupAccel(imu.LSM9DS1_ACCELRANGE_2G);
  imu.setupMag(imu.LSM9DS1_MAGGAIN_4GAUSS);
  imu.setupGyro(imu.LSM9DS1_GYROSCALE_245DPS);

  // Инициализация BLE
  bleMouse.begin();
}

void loop()
{
  imu.read();
  sensors_event_t a, m, g, temp;
  imu.getEvent(&a, &m, &g, &temp);
  int dx = g.gyro.x / 5.0;
  int dy = g.gyro.y / 5.0;
  int dz = g.gyro.z / 5.0;
  Serial.print("X: "); Serial.print(dx);
  Serial.print(" Y: "); Serial.print(dy);
  Serial.print(" Z: "); Serial.print(dz);
  Serial.println();
  if (bleMouse.isConnected())
  {
    bleMouse.move(dz, dy);
  }
  delay(10);
}
