#pragma once
#include <Arduino.h>
#include "../examples/lib/SPIHelper/SPIHelper.h"
#include "../TMC-API-master/tmc/ic/TMC5240/TMC5240.h"
#include "../TMC-API-master/tmc/ic/TMC5240/TMC5240_HW_Abstraction.h"
#include "../TMC-API-master/tmc/ramp/LinearRamp1.h"

class TMC5240Driver {
public:
    TMC5240Driver(SPIHelper &spi, uint16_t icID = DEFAULT_ICID, int enPin = -1);

    void begin();
    void configureDriver();
    void setMotorCurrent(uint16_t mA);
    void setMicrosteps(uint16_t steps);
    void enableDriver();
    void disableDriver();
    void moveTo(int32_t targetPosition);
    void moveBy(int32_t deltaSteps);
    void setVelocity(int32_t velocity);
    void stop();
    uint32_t readRegister(uint8_t address);
    void writeRegister(uint8_t address, uint32_t value);
    int32_t getPosition();
    uint32_t getStatus();
    bool checkErrors();

private:
    SPIHelper &_spi;
    uint16_t _icID;
    int _enPin;
    TMC_LinearRamp _ramp;
};
