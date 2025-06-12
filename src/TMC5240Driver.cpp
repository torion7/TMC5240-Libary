#include "TMC5240Driver.h"

TMC5240Driver::TMC5240Driver(SPIHelper &spi, uint16_t icID, int enPin)
    : _spi(spi), _icID(icID), _enPin(enPin) {}

void TMC5240Driver::begin() {
    _spi.begin();
    if(_enPin >= 0) {
        pinMode(_enPin, OUTPUT);
        disableDriver();
    }
    tmc_ramp_linear_init(&_ramp);
    configureDriver();
}

void TMC5240Driver::configureDriver() {
    for(uint8_t addr = 0; addr < TMC5240_REGISTER_COUNT; ++addr) {
        if(tmc5240_registerAccess[addr] & TMC5240_ACCESS_DIRTY) {
            tmc5240_writeRegister(_icID, addr, tmc5240_sampleRegisterPreset[addr]);
        }
    }
}

void TMC5240Driver::setMotorCurrent(uint16_t mA) {
    uint8_t cs = (mA * 32) / 1000;
    cs = cs > 31 ? 31 : cs;
    uint32_t value = tmc5240_readRegister(_icID, TMC5240_IHOLD_IRUN);
    value = tmc5240_fieldUpdate(value, TMC5240_IRUN_FIELD, cs);
    tmc5240_writeRegister(_icID, TMC5240_IHOLD_IRUN, value);
}

void TMC5240Driver::setMicrosteps(uint16_t steps) {
    uint8_t mres = 0;
    if(steps >= 256) mres = 0;
    else if(steps >= 128) mres = 1;
    else if(steps >= 64) mres = 2;
    else if(steps >= 32) mres = 3;
    else if(steps >= 16) mres = 4;
    else if(steps >= 8)  mres = 5;
    else if(steps >= 4)  mres = 6;
    else if(steps >= 2)  mres = 7;
    else mres = 8;
    uint32_t val = tmc5240_readRegister(_icID, TMC5240_CHOPCONF);
    val = tmc5240_fieldUpdate(val, TMC5240_MRES_FIELD, mres);
    tmc5240_writeRegister(_icID, TMC5240_CHOPCONF, val);
}

void TMC5240Driver::enableDriver() {
    if(_enPin >= 0) digitalWrite(_enPin, LOW);
}

void TMC5240Driver::disableDriver() {
    if(_enPin >= 0) digitalWrite(_enPin, HIGH);
}

void TMC5240Driver::moveTo(int32_t targetPosition) {
    tmc_ramp_linear_set_targetPosition(&_ramp, targetPosition);
    tmc5240_fieldWrite(_icID, TMC5240_RAMPMODE_FIELD, TMC5240_MODE_POSITION);
    tmc5240_writeRegister(_icID, TMC5240_XTARGET, targetPosition);
}

void TMC5240Driver::moveBy(int32_t deltaSteps) {
    moveTo(tmc_ramp_linear_get_targetPosition(&_ramp) + deltaSteps);
}

void TMC5240Driver::setVelocity(int32_t velocity) {
    tmc_ramp_linear_set_targetVelocity(&_ramp, velocity);
    tmc5240_fieldWrite(_icID, TMC5240_RAMPMODE_FIELD, velocity >= 0 ? TMC5240_MODE_VELPOS : TMC5240_MODE_VELNEG);
    tmc5240_rotateMotor(_icID, 0, velocity);
}

void TMC5240Driver::stop() {
    setVelocity(0);
}

uint32_t TMC5240Driver::readRegister(uint8_t address) {
    return tmc5240_readRegister(_icID, address);
}

void TMC5240Driver::writeRegister(uint8_t address, uint32_t value) {
    tmc5240_writeRegister(_icID, address, value);
}

int32_t TMC5240Driver::getPosition() {
    return tmc5240_readRegister(_icID, TMC5240_XACTUAL);
}

uint32_t TMC5240Driver::getStatus() {
    return tmc5240_readRegister(_icID, TMC5240_DRVSTATUS);
}

bool TMC5240Driver::checkErrors() {
    uint32_t gstat = tmc5240_readRegister(_icID, TMC5240_GSTAT);
    return gstat & (TMC5240_DRV_ERR_MASK | TMC5240_UV_CP_MASK | TMC5240_VM_UVLO_MASK);
}
