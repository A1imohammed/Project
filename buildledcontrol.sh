#!/bin/bash 

echo "Starting build led_control build" 
config-pin p9_12 gpio
g++ led_control.cpp GPIO.cpp -o led_control -lpaho-mqtt3c -lpthread -I.
echo "Build finished"
