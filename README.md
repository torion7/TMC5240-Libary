# TMC5240-Libary

This repository aims to provide a dedicated ESP32 library for interfacing with the TMC5240 stepper motor driver **via SPI**. The implementation will rely on the official **TMC-API** and will expose Arduino style C++ classes for easy integration in ESP32 projects. Support for UART communication is currently out of scope.

## Goals
- Wrap the TMC-API to provide simple initialization and configuration helpers for the TMC5240 over SPI
- Allow reading and writing of all driver registers
- Provide high level movement commands (e.g. position and velocity control)
- Offer simple status and diagnostics functions
- Include usage examples targeting the ESP32 toolchain
- Support controlling multiple motors simultaneously using separate configuration files so the main application code stays minimal

At this stage the library focuses solely on SPI communication with the driver. UART mode is not supported yet.

The project currently contains only notes and collected documentation. See the `To Do` file for the planned feature list.

