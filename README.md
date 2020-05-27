# Simple-IoT-application
This project was made for the Embedded Systems course. It is a simple IoT application that allows the user to control the LED status of a microcontroller or retrieve date/time from a RTC module through a web page. The project uses ESP8266 WiFi module, STM32L432KC microcontroller, and DS3231 RTC module

The WiFi module communicates with the microcontroller via asynchronous link UART.
The RTC module communicates with the microcontroller via I2C. 

Throuh the web page, the user can do the following:

1. Turn on or off the LED on the microcontroller

2. Retrieve date/time
