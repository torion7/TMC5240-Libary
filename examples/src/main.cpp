#include <Arduino.h>
#include "SPIHelper.h"


// Pinbelegung für SPI
#define PIN_MOSI 12
#define PIN_MISO 21
#define PIN_SCK  13
#define PIN_CS   15
#define PIN_EN   54
#define PIN_BTN  0

SPIHelper spi(SPI2_HOST, PIN_MOSI, PIN_MISO, PIN_SCK, PIN_CS);
TMC5240Driver tmc(spi);


