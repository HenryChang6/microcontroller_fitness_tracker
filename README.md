## About this project
Our project features a standalone, watch-like embedded system incorporating the MAX30102 sensor, the MPU-6050 gyroscope sensor, and the ESP8266 NodeMCU Wi-Fi module for wireless data transmission to a computer. The system is equipped with its own power supply, allowing users to wear and use it wirelessly, like a traditional watch. Data collected by the sensors is sent via Wi-Fi to a computer and then relayed to a PIC18F4520 microcontroller, which drives an SSD1306 OLED display. Due to the limited size of the OLED screen, users can toggle between different data views (e.g., heart rate, temperature) using a button.

## Project Struture
// TODO: Add struture image 
fitness_tracker directory is an arduino project folder.


## Hardware Info
- 0.96吋 4Pin IIC-I2C 白/藍/黃色 OLED 顯示器
- TP4056 1A 鋰電池充電板模組 TYPE-C
- ESP8266 WiFi NodeMCU 無線模組 
- GY-521 MPU-6050 模組（功能：三軸陀螺儀 + 三軸加速度計）
- MAX30102 血氧、心跳測量模組
