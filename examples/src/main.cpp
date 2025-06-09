#include <Arduino.h>
#include "SPIHelper.h"
#include "TMC5240Driver.h"

// Pinbelegung für SPI
#define PIN_MOSI 12
#define PIN_MISO 21
#define PIN_SCK  13
#define PIN_CS   15
#define PIN_EN   54
#define PIN_BTN  0

SPIHelper spi(SPI2_HOST, PIN_MOSI, PIN_MISO, PIN_SCK, PIN_CS);
TMC5240Driver tmc(spi);

void setup() {
  Serial.begin(115200);
  delay(2000);
  pinMode(PIN_EN, OUTPUT);
  digitalWrite(PIN_EN, LOW);
  pinMode(PIN_BTN, INPUT_PULLUP);

  spi.begin();
  tmc.init();

  // === KONFIGURATION ===

  tmc.setMicrosteps(256);         // Mikroschritt-Auflösung (1–256)
  tmc.setStepsPerRev(200);        // 200 Steps = 1 Umdrehung bei 1.8° Stepper
  tmc.setRampMode(TMC5240Driver::RampMode::POSITION);  // Zielbasierte Bewegung

  tmc.setVStart(100);              // Startgeschwindigkeit [1/256 Steps/s]
  tmc.setVStop(10);               // Stop-Geschwindigkeit
  tmc.setVMax(50000);             // Maximale Geschwindigkeit
  tmc.setAMax(1000);              // Beschleunigung
  tmc.setDMax(1000);              // Verzögerung

  tmc.setMotorCurrentIRun(0.1f);        // 1.2 A Fahrstrom
  tmc.setMotorCurrentIHold(0.05f);       // 0.6 A Haltestrom
  tmc.setDrvConfCurrentRange(1); // 36 A·kΩ → entspricht DRV_CONF[1:0] = 10

  tmc.setIHoldDelay(6);           // Umschaltverzögerung
  tmc.setIRunDelay(6);            // Anlaufverzögerung

  tmc.enablePWM(TMC5240Driver::PWM_Mode::ON); // StealthChop2 aktivieren
  tmc.enableCoolStep(true);                   // Stromabsenkung aktivieren
  tmc.setSGThreshold(15);                     // StallGuard-Empfindlichkeit

  // === Bewegungsstart ===
  //tmc.resetGSTAT();

  tmc.resetPosition();

  tmc.moveByRevolutions(1.0);
  Serial.println("Drehe eine Umdrehung...");
  while (!tmc.isPositionReached()) delay(50);

  Serial.println("Drehe eine Umdrehung zurück...");
  tmc.moveByRevolutions(-1.0);
  while (!tmc.isPositionReached()) delay(50);

  // Ausgabe aller Statusdaten
  tmc.printStatus();
}

void loop() {
  // leer
}
