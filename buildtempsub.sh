#!/bin/bash

echo "Starting temp_sub build"
g++ temp_sub.cpp derek_LED.cpp -o temp_sub -lpaho-mqtt3c -lpthread -I.
echo "Build finished"
