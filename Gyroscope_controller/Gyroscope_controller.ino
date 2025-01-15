// Basic demo for accelerometer readings from Adafruit MPU6050

#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>

Adafruit_MPU6050 mpu;

int step_num = 0;
int prev_acc = 0;
int threshold = 8;

void setup(void) {
  Serial.begin(115200);
  while (!Serial)
    delay(10); // will pause Zero, Leonardo, etc until serial console opens

  Serial.println("Adafruit MPU6050 test!");

  // Try to initialize!
  if (!mpu.begin()) {
    Serial.println("Failed to find MPU6050 chip");
    while (1) delay(10);
  }
  Serial.println("Gyroscope Init Successfully!");

  mpu.setAccelerometerRange(MPU6050_RANGE_8_G);
  mpu.setGyroRange(MPU6050_RANGE_500_DEG);
  mpu.setFilterBandwidth(MPU6050_BAND_21_HZ);

  delay(100);
}

void loop() {

  /* Get new sensor events with the readings */
  sensors_event_t a, g, temp;
  mpu.getEvent(&a, &g, &temp);

  int16_t ax, ay, az;
  int current_acc;
  ax = a.acceleration.x;
  ay = a.acceleration.y;
  az = a.acceleration.z;
  current_acc = sqrt(ax * ax + ay * ay + az * az);

  // int16_t gx, gy, gz;
  // gx = g.gyro.x;
  // gy = g.gyro.y;
  // gz = g.gyro.z;
  
  bool isStop = abs(current_acc - prev_acc) < 2;
  if (!isStop && current_acc > threshold)
    step_num++;

  prev_acc = current_acc;

  /* Print out the values */
  Serial.print("Acceleration X: ");
  Serial.print(ax);
  Serial.print(", Y: ");
  Serial.print(ay);
  Serial.print(", Z: ");
  Serial.print(a.acceleration.z);
  Serial.println(" m/s^2");

  // Serial.print("Rotation X: ");
  // Serial.print(gx);
  // Serial.print(", Y: ");
  // Serial.print(gy);
  // Serial.print(", Z: ");
  // Serial.print(gz);
  // Serial.println(" rad/s");

  Serial.print("Step Number: ");
  Serial.println(step_num);

  Serial.print("current_acc ");
  Serial.println(current_acc);

  Serial.print("isStop: ");
  Serial.println(isStop);

  Serial.println("");
  delay(300);
}