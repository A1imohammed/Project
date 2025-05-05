#!/bin/bash

echo "starting window_sensor build"
config-pin p8_09 gpio
g++ window_sensor.cpp GPIO.cpp -o window_sensor -lpaho-mqtt3c -lpthread
echo "build finished"
