# Microcontroller Watch-like Fitness Tracker 

## About This Project

This project is the final project of the NCKU CSIE Microcontroller Course. It showcases a standalone, wearable, watch-like embedded system featuring advanced sensor integration, wireless data transmission, and user-friendly display capabilities.

### Key Features

1. **Hardware Components**:
   - **MAX30102**: Pulse oximeter and heart rate sensor for real-time biometric data.
   - **MPU-6050**: Gyroscope and accelerometer sensor for motion tracking.
   - **ESP8266 NodeMCU**: Wi-Fi module enabling wireless data transmission via a REST API.
   - **PIC18F4520 Microcontroller**: Responsible for processing sensor data and driving the display.
   - **SSD1306 OLED Display**: Compact screen for presenting user data.

2. **Power Supply**:
   - The system is self-powered, enabling complete wireless operation, akin to a traditional wristwatch.

3. **Data Communication**:
   - Sensor data is transmitted wirelessly over Wi-Fi using the ESP8266WebServer library, which provides a REST API service on port 80.
   - A Python script fetches sensor data every second and writes it to the serial port connected to the PIC18F4520 microcontroller.

4. **Display Features**:
   - The OLED display shows real-time data such as heart rate, temperature, and activity duration.
   - Users can toggle between different data views (e.g., heart rate, temperature, and sporting time) using a button interface.

5. **Sporting Time Measurement**:
   - A CCP (Capture/Compare/PWM) timer mode is implemented to calculate and display sporting time on the OLED screen.


## System Structure
![Diagram of the system structure](System_Structure.jpg)

## Project Struture
// TODO: Add struture image 
fitness_tracker directory is an arduino project folder.


## Hardware Info
- 0.96吋 4Pin IIC-I2C 白/藍/黃色 OLED 顯示器
- TP4056 1A 鋰電池充電板模組 TYPE-C
- ESP8266 WiFi NodeMCU 無線模組 
- GY-521 MPU-6050 模組（功能：三軸陀螺儀 + 三軸加速度計）
- MAX30102 血氧、心跳測量模組
