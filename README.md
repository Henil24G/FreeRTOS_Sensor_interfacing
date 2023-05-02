# FreeRTOS_Sensor_interfacing

--> Multitasking with Semaphore

The code consists of three tasks: temperatureTask, microphoneTask, and printTask, which demonstrate reading temperature, simulating microphone input, and printing data, respectively. The tasks are synchronized using a semaphore.

--> Prerequisites

To run this code example, you will need the following:

Arduino board
OneWire library
DallasTemperature library
Arduino_FreeRTOS library

--> Hardware Setup

Connect the Dallas temperature sensor to the Arduino board. The data wire should be plugged into digital pin 2 (#define ONE_WIRE_BUS 2).

Connect a microphone to the Arduino board's analog input A0.