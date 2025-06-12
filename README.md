# TMC5240-Libary

This repository provides an ESP32 compatible library to control the TMC5240 stepper motor driver **via SPI**. The implementation is based on the official **TMC-API** and offers an Arduino style C++ wrapper.

## Features

- Simple initialization helpers for SPI communication
- Default register configuration out of the box
- Change motor current and microstep resolution
- High level position and velocity commands
- Access to all driver registers
- Status and error query helpers
- Example project for PlatformIO/Arduino

UART mode is currently not supported.

## Usage

1. Include the library sources in your ESP32 PlatformIO or Arduino project.
2. Instantiate `SPIHelper` and `TMC5240Driver` using the SPI pins and enable pin of your setup.
3. Call `begin()` once in `setup()` and use the movement commands in your application.

See `examples/src/main.cpp` for a minimal example.

The file `To Do` lists the original planning notes and API functions used.
