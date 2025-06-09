#pragma once

// Globale Konfigurations- und Statusregister
#define REG_GCONF           0x00  // Global Configuration Register: Hauptschalter, StallGuard-Modus, etc.
#define REG_GSTAT           0x01  // Global Status Register: Flags für Fehler, Reset, etc.
#define REG_IFCNT           0x02  // Interface Counter Register: Zähler für ausstehende SPI-Transaktionen
#define REG_SLAVECONF       0x03  // Slave Configuration Register: Konfiguration für UART/SPI
#define REG_IOIN            0x04  // Input Status Register: GPIO- und Schaltereingänge
#define REG_FACTORY_CONF    0x05  // Factory Configuration Register: Werksseitige Einstellungen (nur lesen)
#define REG_SHORT_CONF      0x06  // Short Protection Configuration Register
#define REG_DRV_STATUS      0x07  // Driver Status Register: Flags für Overcurrent, OpenLoad, UVLO, Temperature, etc.
#define REG_VERSION         0x08  // IC Version und Typ
#define REG_A1               0x24 // Beschleunigung erster Rampenabschnitt
#define REG_V1               0x25 // Geschwindigkeit am Ende des ersten Beschleunigungsabschnitts
#define REG_RAMPMODE        0x20  // RAMPMODE Register: Auswahl der Rampenform (Position, Velocity, etc.)
#define REG_XACTUAL         0x21  // XACTUAL Register: Aktuelle Position in Schritten
#define REG_VACTUAL         0x22  // VACTUAL Register: Aktuelle Geschwindigkeit in 1/256-Schritten/s
#define REG_VSTART          0x23  // VSTART Register: Startgeschwindigkeit für Rampenbeginn
#define REG_AMAX            0x26  // AMAX Register: Maximale Beschleunigung
#define REG_VMAX            0x27  // VMAX Register: Maximale Geschwindigkeit
#define REG_DMAX            0x28  // DMAX Register: Maximale Verzögerung
#define REG_TVMAX           0x29  // TVMAX Register: Mindestdauer der konstanten Höchstgeschwindigkeit
#define REG_D1              0x2A  // D1 Register: Verzögerung erster Abschnitt
#define REG_VSTOP           0x2B  // VSTOP Register: Endgeschwindigkeit beim Abbremsen
#define REG_TZEROWAIT       0x2C  // TZEROWAIT Register: Wartezeit nach Erreichen der Zielposition
#define REG_XTARGET         0x2D  // XTARGET Register: Zielposition in Schritten

// Software- und Rampenstatus
#define REG_SWMODE          0x34  // SWMODE Register: Software-Steuerbits (z.B. Position reset)
#define REG_RAMP_STAT       0x35  // RAMP_STAT Register: Status des Rampengenerators (Motion-Flags)

// Chopper- und Kühlkonfiguration
#define REG_CHOPCONF        0x6C  // CHOPCONF Register: Chopper-Einstellungen (Mikroschritt-PWM, Hysterese)
#define REG_COOLCONF        0x6D  // COOLCONF Register: CoolStep-Konfiguration (automatische Stromregelung)
#define REG_DCCTRL          0x6E  // DCCTRL Register: DC-Step Steuerung (Konstantstrom-Modus)
#define REG_PWMSCALE        0x70  // PWMCONF Register: PWM-Steuerungsparameter

// StallGuard und Sensoren
#define REG_SGTHRS          0x40  // SGTHRS Register: StallGuard-Schwellwert für Zuglast-Erkennung
#define REG_SG_RESULT       0x41  // SG_RESULT Register: StallGuard2 Ausgangswert
#define REG_COOL_VALUE      0x42  // TCOOLTHRS Register: Temperature-Threshold für CoolStep
#define REG_THIGH           0x43  // THIGH Register: Höchsttemperatur-Grenze für Schutz-Abschaltung

// ADC und Temperatur
#define REG_ADC_TEMPERATURE 0x50  // ADC_TEMPERATURE Register: Messwert der internen Chip-Temperatur
#define REG_ADC_VSUP        0x51  // ADC_VSUP Register: Messwert der Versorgungsspannung
