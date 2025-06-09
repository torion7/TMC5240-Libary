#pragma once
#include <Arduino.h>
#include "../SPIHelper/SPIHelper.h"
#include "TMC5240Constants.h"

/*
=========================================================
 TMC5240Driver – benutzerfreundliche API für TMC5240
---------------------------------------------------------
 Tooltips & Hinweise:
 - Alle Bewegungs- und Stromparameter sind logisch benannt
 - Werte sind in "Steps", "1/256 Steps/s", "Umdrehungen", etc.
 - Mikroschrittauflösung (z. B. 1/16) beeinflusst die effektive Auflösung
 - Verwende init() im setup(), danach können alle Parameter gesetzt werden
 - Die Einheit bei moveTo() oder moveByRevolutions() ist absolut, nicht relativ
 - isPositionReached() gibt true zurück, wenn der Zielwert erreicht wurde
 - Für komplexe Systeme: setMicrosteps(), setStepsPerRev(), moveByRevolutions()
=========================================================
*/

class TMC5240Driver {
public:
  
  

  TMC5240Driver(SPIHelper& spi);

  void setChopperTOFF(uint8_t val);
  void setChopperHSTRT(uint8_t val);
  void setChopperHEND(uint8_t val);
  void setChopperInterpolation(bool enabled);
  void setChopperDoubleEdge(bool enabled);
  void setChopperDisableShortToGND(bool disable);
  void setChopperDisableShortToVS(bool disable);

  void init();
  void resetGSTAT();
  void stop();
  void moveTo(int32_t position);
  bool isPositionReached();
  void resetPosition();
  void printStatus();

  void setMicrosteps(uint16_t microsteps);
  void setStepsPerRev(uint16_t fullsteps);
  void moveByRevolutions(float revolutions);

  void setVStart(uint32_t val);
  void setVStop(uint32_t val);
  void setVMax(uint32_t val);
  void setAMax(uint32_t val);
  void setDMax(uint32_t val);
  enum class RampMode { POSITION = 0, VELOCITY = 1 };
  void setRampMode(RampMode mode);

  void setIRun(uint8_t val);
  void setIHold(uint8_t val);
  void setIHoldDelay(uint8_t val);
  void setIRunDelay(uint8_t val);

  enum class PWM_Mode { OFF = 0, ON = 1 };
  void enablePWM(PWM_Mode mode);
  void setChopConf(uint32_t val);

  void enableCoolStep(bool enable);
  void enableStallGuard(bool enable);
  void setSGThreshold(uint8_t threshold);

  float getChipTemperature(); // Temperatur [°C]
  float getSupplyVoltage();   // Versorgungsspannung [V]

  void setIRefResistor(float ohms);
  void setFullScaleVoltage(float volts);
  void setGainFactor(float gain);
  void setMotorCurrentIRun(float currentA);
  void setMotorCurrentIHold(float currentA);

  void setDrvConfCurrentRange(uint8_t range); // 0–3
  void writeDRVCONF();

private:
  SPIHelper& spi;
  float irefResistor = 12000.0f;  // in Ohm, z. B. 15kΩ
 float fullScaleVoltage = 0.325f; // laut Datenblatt
 float gainFactor = 32.0f;        // aus Datenblatt/Modell

 uint8_t drvConfCurrentRange = 0; // 0=1A, 1=2A, 2=3A, 3=3A


};
