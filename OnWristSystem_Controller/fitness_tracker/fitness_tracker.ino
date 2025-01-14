/*
  MAX3010 Breakout: Read the onboard temperature sensor
  By: Nathan Seidle @ SparkFun Electronics
  Date: October 20th, 2016
  https://github.com/sparkfun/MAX30105_Breakout

  This demo outputs the onboard temperature sensor. The temp sensor is accurate to +/-1 C but
  has an astonishing precision of 0.0625 C.

  Hardware Connections (Breakoutboard to Arduino):
  -5V = 5V (3.3V is allowed)
  -GND = GND
  -SDA = A4 (or SDA)
  -SCL = A5 (or SCL)
  -INT = Not connected
 
  The MAX30105 Breakout can handle 5V or 3.3V I2C logic. We recommend powering the board with 5V
  but it will also run at 3.3V.
*/

#include <Wire.h>
#include "MAX30105.h"  
#include "heartRate.h"
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

MAX30105 particleSensor;
ESP8266WebServer server(80);

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

void setup()
{
  //Serial.begin(9600);
  Serial.begin(115200);
  Serial.println("Initializing...");

  // Initialize wifi 
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting Wi-Fi...");
  }
  Serial.println("Wi-Fi Successfully Connected");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  // Init Server
  server.on("/", handleRoot);
  server.begin();
  Serial.println("HTTP server successfully init");

  // Initialize sensor
  if (particleSensor.begin(Wire, I2C_SPEED_FAST) == false) //Use default I2C port, 400kHz speed
  {
    Serial.println("MAX30105 was not found. Please check wiring/power. ");
    while (1);
  }

  //The LEDs are very low power and won't affect the temp reading much but
  //you may want to turn off the LEDs to avoid any local heating
  particleSensor.setup(); //Configure sensor. Use 25mA for LED drive
  particleSensor.setPulseAmplitudeRed(0x0A); // Turn Red LED to low to indicate sensor is running
  particleSensor.setPulseAmplitudeGreen(0); // Turn off Green LED

  particleSensor.enableDIETEMPRDY(); //Enable the temp ready interrupt. This is required.
}

void handleRoot()
{
  String html = "<html><body>";
  html += "<h1>Heart Rate Monitor</h1>";
  html += "<p>IR value: " + String(irValue) + "</p>";
  html += "<p>current heart rate (BPM): " + String(beatsPerMinute) + "</p>";
  html += "<p>average heart rate (BPM): " + String(beatAvg) + "</p>";
  html += "<p>temperature(C): " + String(temperature) + "</p>";
  html += "</body></html>";
  server.send(200, "text/html", html);
}

void loop()
{
  server.handleClient();
  // handle heart rate
  irValue = particleSensor.getIR();   // infrared light intensity
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
  temperature = particleSensor.readTemperature();

  Serial.print(", temperatureC=");
  Serial.print(temperature, 4);

  Serial.println();
}
