#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>
#include "MAX30105.h"  
#include "heartRate.h"
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <ArduinoJson.h>

Adafruit_MPU6050 gyroscope;
MAX30105 heartRate_sensor;

ESP8266WebServer server(80);  

int step_num = 0;
int prev_acc = 0;
int threshold = 8;

const char* ssid = "Caphone";
const char* password = "hihellohi";

const byte RATE_SIZE = 4; //Increase this for more averaging. 4 is good.
byte rates[RATE_SIZE]; //Array of heart rates
byte rateSpot = 0;
long lastBeat = 0; //Time at which the last beat occurred
float temperature;
float beatsPerMinute;
int beatAvg;
long irValue;

static unsigned long lastStepTime = 0;

void setup(void) {
  Serial.begin(115200);
  Serial.println("Initializing...");
  while (!Serial)
    delay(10); // will pause Zero, Leonardo, etc until serial console opens

  Serial.println("Adafruit MPU6050 test!");

  // Try to initialize!
  if (!gyroscope.begin()) {
    Serial.println("Failed to find MPU6050 chip");
    while (1) delay(10);
  }
  Serial.println("Gyroscope Init Successfully!");

  gyroscope.setAccelerometerRange(MPU6050_RANGE_8_G);
  gyroscope.setGyroRange(MPU6050_RANGE_500_DEG);
  gyroscope.setFilterBandwidth(MPU6050_BAND_21_HZ);

  // Initialize sensor
  if (heartRate_sensor.begin(Wire, I2C_SPEED_FAST) == false) //Use default I2C port, 400kHz speed
  {
    Serial.println("MAX30105 was not found. Please check wiring/power. ");
    while (1);
  }
  //The LEDs are very low power and won't affect the temp reading much but
  //you may want to turn off the LEDs to avoid any local heating
  heartRate_sensor.setup(); //Configure sensor. Use 25mA for LED drive
  heartRate_sensor.setPulseAmplitudeRed(0x0A); // Turn Red LED to low to indicate sensor is running
  heartRate_sensor.setPulseAmplitudeGreen(0); // Turn off Green LED

  heartRate_sensor.enableDIETEMPRDY(); //Enable the temp ready interrupt. This is required.

  // Initialize wifi 
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting Wi-Fi...");
  }
  Serial.println("Wi-Fi Successfully Connected");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  delay(500);
  server.on("/test",HTTP_GET,sendData);
  server.begin();

  delay(100);
}

void sendData(){
    StaticJsonDocument<300> JSONData;
    // Use the object just like a javascript object or a python dictionary  
    JSONData["step_count"] = step_num; 
    JSONData["heart_rate"] = beatsPerMinute;  
    JSONData["temperature"] = temperature; 
    // You can add more fields
    char data[300];
    // Converts the JSON object to String and stores it in data variable
    serializeJson(JSONData,data);
    // Set content type as application/json and send the data
    server.send(200,"application/json",data);
}

void loop() {
  server.handleClient();
  /* Get new sensor events with the readings */
  sensors_event_t a, g, temp;
  unsigned long currentTime = millis();
  int16_t ax, ay, az;
  int current_acc;
  bool isStop;
  if (currentTime - lastStepTime > 200) {
    gyroscope.getEvent(&a, &g, &temp);
    ax = a.acceleration.x;
    ay = a.acceleration.y;
    az = a.acceleration.z;
    current_acc = sqrt(ax * ax + ay * ay + az * az);
    isStop = abs(current_acc - prev_acc) < 2;
    if (!isStop && current_acc > threshold) 
      step_num++;

    prev_acc = current_acc;
  } 

  /* Print out the values */
  // Serial.print("Acceleration X: ");
  // Serial.print(ax);
  // Serial.print(", Y: ");
  // Serial.print(ay);
  // Serial.print(", Z: ");
  // Serial.print(a.acceleration.z);
  // Serial.println(" m/s^2");

  Serial.print("Step Number: ");
  Serial.println(step_num);

  // Serial.print("current_acc ");
  // Serial.println(current_acc);

  Serial.print("isStop: ");
  Serial.println(isStop);

  Serial.println("");

  // handle heart rate
  irValue = heartRate_sensor.getIR();   // infrared light intensity
  if (irValue < 4000) {
      beatsPerMinute = 0;
      beatAvg = 0;
  }
  if (checkForBeat(irValue) == true)
  {
    float prev_beatsPerMinute = beatsPerMinute;
    int prev_beatAvg = beatAvg;
    //We sensed a beat!
    long delta = millis() - lastBeat;
    lastBeat = millis();

    beatsPerMinute = 60 / (delta / 1000.0);

    if (beatsPerMinute < 255 && beatsPerMinute > 20)
    {
      rates[rateSpot++] = (byte)beatsPerMinute; // Store this reading in the array
      rateSpot %= RATE_SIZE; // Wrap variable

      // Take average of readings
      beatAvg = 0;
      for (byte x = 0 ; x < RATE_SIZE ; x++)
        beatAvg += rates[x];
      beatAvg /= RATE_SIZE;
    }
    // set a threshold to avoid fluctuation
    if (prev_beatsPerMinute > beatsPerMinute + 30) beatsPerMinute = prev_beatsPerMinute;
    if (prev_beatAvg > beatAvg + 30) beatAvg = prev_beatAvg;
  }

  Serial.print("IR=");
  Serial.print(irValue);
  Serial.print(", BPM=");
  Serial.print(beatsPerMinute);
  Serial.print(", Avg BPM=");
  Serial.print(beatAvg);

  if (irValue < 4000)
    Serial.print("Place the sensor closer to your wrist!!!");

  // handle temperature 
  temperature = heartRate_sensor.readTemperature();

  Serial.print(", temperatureC=");
  Serial.print(temperature, 4);

  Serial.println();
  // delay(300);
}