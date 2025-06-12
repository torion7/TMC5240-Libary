#include <Arduino.h>
#include "SPIHelper.h"
#include "TMC5240Driver.h"

// SPI pin configuration
#define PIN_MOSI 12
#define PIN_MISO 21
#define PIN_SCK  13
#define PIN_CS   15
#define PIN_EN   14

SPIHelper spi(SPI2_HOST, PIN_MOSI, PIN_MISO, PIN_SCK, PIN_CS);
TMC5240Driver driver(spi, DEFAULT_ICID, PIN_EN);

void setup() {
    Serial.begin(115200);
    driver.begin();
    driver.enableDriver();
    driver.setMotorCurrent(800); // 800mA
    driver.setMicrosteps(16);
    driver.setVelocity(1000);
}

void loop() {
    static int32_t pos = 0;
    driver.moveTo(pos);
    pos += 2000;
    delay(5000);
    driver.moveBy(-2000);
    delay(5000);
}
