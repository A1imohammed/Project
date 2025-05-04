#!/bin/bash

echo "starting pwm build"
config-pin p9_22 pwm
g++ PWM.cpp GPIO.cpp util.cpp pwm.cpp -o sliderTask -lpaho-mqtt3c -pthread
echo "build finished"
