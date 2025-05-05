#!/bin/bash

echo "Starting build blinkusr3 build"
g++ blink_usr3.cpp derek_LED.cpp -o blink_usr3 -lpaho-mqtt3c -lpthread -I.
echo "Build finished"
