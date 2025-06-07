#include <AceSPI.h>     // HardSpiInterface
#include <AceSegment.h> // Max7219Module
#include <Arduino.h>
#include <SPI.h> // SPIClass, SPI

#define ESP8266_DRD_USE_RTC       true
#define DOUBLERESETDETECTOR_DEBUG true
#define DRD_TIMEOUT               2
#define DRD_ADDRESS               0
#include <ESP_DoubleResetDetector.h>
DoubleResetDetector *drd;

using ace_segment::kDigitRemapArray8Max7219;
using ace_segment::Max7219Module;
using ace_spi::HardSpiInterface;

const uint8_t LATCH_PIN = SS;

const uint8_t NUM_DIGITS = 4;

using SpiInterface = HardSpiInterface<SPIClass>;
SpiInterface spiInterface(SPI, LATCH_PIN);
Max7219Module<SpiInterface, NUM_DIGITS> ledModule(
    spiInterface, kDigitRemapArray8Max7219);

const uint8_t NUM_PATTERNS = 10;
const uint8_t PATTERNS[NUM_PATTERNS] = {
    0b00111111, // 0
    0b00000110, // 1
    0b01011011, // 2
    0b01001111, // 3
    0b01100110, // 4
    0b01101101, // 5
    0b01111101, // 6
    0b00000111, // 7
    0b01111111, // 8
    0b01101111, // 9
};

void setup() {
    delay(1000);

    SPI.begin();
    spiInterface.begin();
    ledModule.begin();

    ledModule.setPatternAt(0, PATTERNS[0]);
    ledModule.setPatternAt(1, PATTERNS[1]);
    ledModule.setPatternAt(2, PATTERNS[2]);
    ledModule.setPatternAt(3, PATTERNS[3]);

    ledModule.setBrightness(2);

    ledModule.flush();
}

void loop() {}