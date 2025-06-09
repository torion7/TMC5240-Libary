#include "TMC5240Driver.h"

TMC5240Driver::TMC5240Driver(SPIHelper& spi) : spi(spi) {}

void TMC5240Driver::init() {
  resetGSTAT();
  setRampMode(RampMode::POSITION);
  setVStart(10);
  setVStop(10);
  setVMax(50000);
  setAMax(1000);
  setDMax(1000);
  //setMotorCurrentIRun(1.2f);        // 1.2 A Fahrstrom
  //setMotorCurrentIHold(0.6f);       // 0.6 A Haltestrom
  //setDrvConfCurrentRange(1); // 36 A·kΩ → entspricht DRV_CONF[1:0] = 10
  writeDRVCONF();
  setIHoldDelay(6);
  enablePWM(PWM_Mode::ON);
  enableCoolStep(true);
  enableStallGuard(false);

  // SpreadCycle + Mikroschritt-Setup
  setChopperTOFF(3);
  setChopperHSTRT(4);
  setChopperHEND(3);
  setChopperInterpolation(true);
  setMicrosteps(256);
}

void TMC5240Driver::resetGSTAT() {
  spi.write(REG_GSTAT, 0x0F);
}

void TMC5240Driver::stop() {
  setVMax(0);
}

void TMC5240Driver::moveTo(int32_t position) {
  spi.write(REG_XTARGET, position);
}

void TMC5240Driver::printStatus() {
auto estCurrent = [&](uint8_t val) -> float {
  float KIFS_table[] = {11.75f, 24.0f, 36.0f, 36.0f}; // laut Datenblatt
  float KIFS = KIFS_table[drvConfCurrentRange];
  float IFS = KIFS / (irefResistor / 1000.0f); // RREF in kΩ
  float peakCurrent = (val / 31.0f) * IFS;
  return peakCurrent / 1.4142f; // RMS
};

  Serial.println("=== TMC5240 STATUS ===");

  int32_t xactual = (int32_t)spi.read(REG_XACTUAL);
  int32_t xtarget = (int32_t)spi.read(REG_XTARGET);
  int32_t vactual = (int32_t)spi.read(0x22);

  Serial.printf("Position IST     (XACTUAL):  %ld steps\n", xactual);
  Serial.printf("Position SOLL    (XTARGET):  %ld steps\n", xtarget);
  Serial.printf("Geschwindigkeit  (VACTUAL):  %ld × [1/256 steps/s]\n", vactual);
  Serial.printf("VSTART:      %lu\n", spi.read(REG_VSTART));
  Serial.printf("VSTOP:       %lu\n", spi.read(REG_VSTOP));
  Serial.printf("VMAX:        %lu\n", spi.read(REG_VMAX));
  Serial.printf("AMAX:        %lu\n", spi.read(REG_AMAX));
  Serial.printf("DMAX:        %lu\n", spi.read(REG_DMAX));

  Serial.printf("CHOPCONF:    0x%08X\n", spi.read(REG_CHOPCONF));
  Serial.printf("PWMCONF:     0x%08X\n", spi.read(REG_PWMCONF));
  Serial.printf("COOLCONF:    0x%08X\n", spi.read(REG_COOLCONF));
  Serial.printf("DCCTRL:      0x%08X\n", spi.read(REG_DCCTRL));
  Serial.printf("SGTHRS:      %lu\n", spi.read(REG_SGTHRS));
  Serial.printf("TCOOLTHRS:   %lu\n", spi.read(REG_TCOOLTHRS));
  Serial.printf("THIGH:       %lu\n", spi.read(REG_THIGH));

  uint32_t ihold_irun = spi.read(REG_IHOLD_IRUN);
  uint8_t irun = (ihold_irun >> 8) & 0x1F;
  uint8_t ihold = ihold_irun & 0x1F;

Serial.printf("Strom (IRUN):             %u / 31 = %.3f A RMS\n", irun, estCurrent(irun));
Serial.printf("Haltestrom (IHOLD):       %u / 31 = %.3f A RMS\n", ihold, estCurrent(ihold));
Serial.printf("DRV_CONF: 0x%08X\n", spi.read(REG_DRV_CONF));


  uint32_t gstat = spi.read(REG_GSTAT);
  Serial.printf("GSTAT:           0x%08X  – ", gstat);
  if (gstat == 0) Serial.println("Kein Fehler");
  if (gstat & 0x01) Serial.println("Reset durchgesetzt");
  if (gstat & 0x02) Serial.println("Driver Error erkannt");
  if (gstat & 0x04) Serial.println("Unterspannung erkannt");
  if (gstat & 0x08) Serial.println("Übertemperatur-Warnung");
  if (gstat & 0x10) Serial.println("Thermal Shutdown (TSD)");

  uint32_t ramp = spi.read(REG_RAMP_STAT);
  bool reached = ramp & (1 << 9);
  Serial.printf("RAMP_STAT:       0x%08X  → %s\n", ramp, reached ? "Ziel erreicht" : "Bewegt sich noch");
  Serial.printf("IFCNT:       %lu\n", spi.read(REG_IFCNT));
  Serial.printf("GSTAT:       0x%08X\n", spi.read(REG_GSTAT));

  float tempC = getChipTemperature();
  float vcc = getSupplyVoltage();
  Serial.printf("Temperatur:                %.1f °C\n", tempC);
  Serial.printf("Versorgungsspannung:       %.2f V\n", vcc);

  Serial.println("===========================");
}

void TMC5240Driver::setVStart(uint32_t val)     { spi.write(REG_VSTART, val); }
void TMC5240Driver::setVStop(uint32_t val)      { spi.write(REG_VSTOP, val); }
void TMC5240Driver::setVMax(uint32_t val)       { spi.write(REG_VMAX, val); }
void TMC5240Driver::setAMax(uint32_t val)       { spi.write(REG_AMAX, val); }
void TMC5240Driver::setDMax(uint32_t val)       { spi.write(REG_DMAX, val); }
void TMC5240Driver::setRampMode(RampMode mode)  { spi.write(REG_RAMPMODE, static_cast<uint8_t>(mode)); }

void TMC5240Driver::setIRun(uint8_t val) {
  uint32_t reg = spi.read(REG_IHOLD_IRUN);
  reg = (reg & 0xFFFF00FF) | (val << 8);
  spi.write(REG_IHOLD_IRUN, reg);
}

void TMC5240Driver::setIHold(uint8_t val) {
  uint32_t reg = spi.read(REG_IHOLD_IRUN);
  reg = (reg & 0xFFFFFF00) | val;
  spi.write(REG_IHOLD_IRUN, reg);
}

void TMC5240Driver::setIHoldDelay(uint8_t val) {
  uint32_t reg = spi.read(REG_IHOLD_IRUN);
  reg = (reg & 0xFF00FFFF) | (val << 16);
  spi.write(REG_IHOLD_IRUN, reg);
}

void TMC5240Driver::setIRunDelay(uint8_t val) {
  uint32_t reg = spi.read(REG_IHOLD_IRUN);
  reg = (reg & 0x00FFFFFF) | (val << 24);
  spi.write(REG_IHOLD_IRUN, reg);
}

void TMC5240Driver::enablePWM(PWM_Mode mode) {
  uint32_t gconf = spi.read(REG_GCONF);
  if (mode == PWM_Mode::ON)
    gconf |= (1 << 4);
  else
    gconf &= ~(1 << 4);
  spi.write(REG_GCONF, gconf);
}

void TMC5240Driver::setChopConf(uint32_t val) {
  spi.write(REG_CHOPCONF, val);
}

void TMC5240Driver::enableCoolStep(bool enable) {
  uint32_t reg = spi.read(REG_COOLCONF);
  if (enable)
    reg |= (1 << 16);
  else
    reg &= ~(1 << 16);
  spi.write(REG_COOLCONF, reg);
}

void TMC5240Driver::enableStallGuard(bool enable) {
  uint32_t sw = spi.read(REG_SWMODE);
  if (enable)
    sw |= (1 << 10);
  else
    sw &= ~(1 << 10);
  spi.write(REG_SWMODE, sw);
}

void TMC5240Driver::setSGThreshold(uint8_t threshold) {
  spi.write(REG_SGTHRS, threshold);
}

void TMC5240Driver::resetPosition() {
  spi.write(REG_XACTUAL, 0);
}

void TMC5240Driver::setMicrosteps(uint16_t microsteps) {
  uint8_t mres_bits;
  switch (microsteps) {
    case 1: mres_bits = 8; break;
    case 2: mres_bits = 7; break;
    case 4: mres_bits = 6; break;
    case 8: mres_bits = 5; break;
    case 16: mres_bits = 4; break;
    case 32: mres_bits = 3; break;
    case 64: mres_bits = 2; break;
    case 128: mres_bits = 1; break;
    case 256: mres_bits = 0; break;
    default:
      Serial.println("Ungültige Mikroschrittzahl.");
      return;
  }
  uint32_t reg = spi.read(REG_CHOPCONF);
  reg &= ~(0b11111UL << 24);
  reg |= ((uint32_t)mres_bits << 24);
  spi.write(REG_CHOPCONF, reg);
}

static uint16_t steps_per_rev = 200;
static uint16_t microsteps = 256;

void TMC5240Driver::setStepsPerRev(uint16_t fullsteps) {
  steps_per_rev = fullsteps;
}

void TMC5240Driver::moveByRevolutions(float revs) {
  int32_t targetSteps = (int32_t)(revs * steps_per_rev * microsteps);
  int32_t current = (int32_t)spi.read(REG_XACTUAL);
  spi.write(REG_XTARGET, current + targetSteps);
}

bool TMC5240Driver::isPositionReached() {
  return spi.read(REG_RAMP_STAT) & (1 << 9);
}

float TMC5240Driver::getChipTemperature() {
  uint32_t raw = spi.read(REG_ADC_TEMP);
  uint16_t adc = raw & 0x1FFF;
  return (adc - 2038.0f) / 7.7f;
}

float TMC5240Driver::getSupplyVoltage() {
  uint32_t raw = spi.read(REG_ADC_VSUP);
  uint16_t adc = raw & 0x1FFF;
  return adc * 0.009732f;
}

void TMC5240Driver::setChopperTOFF(uint8_t val) {
  uint32_t reg = spi.read(REG_CHOPCONF);
  reg = (reg & ~0x0F) | (val & 0x0F);
  spi.write(REG_CHOPCONF, reg);
}

void TMC5240Driver::setChopperHSTRT(uint8_t val) {
  uint32_t reg = spi.read(REG_CHOPCONF);
  reg = (reg & ~(0x07 << 4)) | ((val & 0x07) << 4);
  spi.write(REG_CHOPCONF, reg);
}

void TMC5240Driver::setChopperHEND(uint8_t val) {
  uint32_t reg = spi.read(REG_CHOPCONF);
  reg = (reg & ~(0x0F << 7)) | ((val & 0x0F) << 7);
  spi.write(REG_CHOPCONF, reg);
}

void TMC5240Driver::setChopperInterpolation(bool enabled) {
  uint32_t reg = spi.read(REG_CHOPCONF);
  if (enabled)
    reg |= (1 << 11);
  else
    reg &= ~(1 << 11);
  spi.write(REG_CHOPCONF, reg);
}

void TMC5240Driver::setChopperDoubleEdge(bool enabled) {
  uint32_t reg = spi.read(REG_CHOPCONF);
  if (enabled)
    reg |= (1 << 12);
  else
    reg &= ~(1 << 12);
  spi.write(REG_CHOPCONF, reg);
}

void TMC5240Driver::setChopperDisableShortToGND(bool disable) {
  uint32_t reg = spi.read(REG_CHOPCONF);
  if (disable)
    reg |= (1 << 13);
  else
    reg &= ~(1 << 13);
  spi.write(REG_CHOPCONF, reg);
}

void TMC5240Driver::setChopperDisableShortToVS(bool disable) {
  uint32_t reg = spi.read(REG_CHOPCONF);
  if (disable)
    reg |= (1 << 14);
  else
    reg &= ~(1 << 14);
  spi.write(REG_CHOPCONF, reg);
}

void TMC5240Driver::setIRefResistor(float ohms) {
  irefResistor = ohms;
}

void TMC5240Driver::setFullScaleVoltage(float volts) {
  fullScaleVoltage = volts;
}

void TMC5240Driver::setGainFactor(float gain) {
  gainFactor = gain;
}

uint8_t calculateCurrentValue(float currentA, float Vfs, float R_iref, float gain) {
  // Formel gemäß Datenblatt (Abschnitt "Setting the Full-Scale Current Range")
  float result = (currentA * gain * R_iref) / Vfs;
  if (result > 31.0f) result = 31.0f;
  if (result < 0.0f) result = 0.0f;
  return static_cast<uint8_t>(roundf(result));
}

void TMC5240Driver::setMotorCurrentIRun(float currentA) {
  uint8_t val = calculateCurrentValue(currentA, fullScaleVoltage, irefResistor, gainFactor);
  setIRun(val);
}

void TMC5240Driver::setMotorCurrentIHold(float currentA) {
  uint8_t val = calculateCurrentValue(currentA, fullScaleVoltage, irefResistor, gainFactor);
  setIHold(val);
}

void TMC5240Driver::setDrvConfCurrentRange(uint8_t range) {
  drvConfCurrentRange = constrain(range, 0, 3); // Sicherheit
}

void TMC5240Driver::writeDRVCONF() {
  // Beispiel: Bits 0–1 = current range, rest = 0
  uint32_t reg = drvConfCurrentRange & 0x03;
  spi.write(REG_DRV_CONF, reg);
}

