# pip install requests pyserial

import requests
import time 
import serial 

url = 'http://172.20.10.2:80/test' 
ser = serial.Serial('COM6', 9600) 

while True:
    response = requests.get(url)
    data = response.json()  
    # example response: 
    # { 
    #   "step_count":285,
    #   "heart_rate":0,
    #   "temperature":27.4375
    # }
    step = data.get('step_count')
    print("step: " + str(step))
    heartRate = data.get('heart_rate')
    print("hearRate: " + str(heartRate))
    temp = data.get('temperature')
    print("temp: " + str(temp))

    # adjust according to real port num and baud rate
    ser.write(f'{step} {heartRate} {temp}\n')

    time.sleep(1)