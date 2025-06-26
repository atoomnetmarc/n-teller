/*

Copyright 2025 Marc Ketel
SPDX-License-Identifier: Apache-2.0

*/

#include <Arduino.h>

#include <MaxLedControl.h>

#define ESP8266_DRD_USE_RTC       true
#define DOUBLERESETDETECTOR_DEBUG true
#define DRD_TIMEOUT               2
#define DRD_ADDRESS               0
#include <ESP_DoubleResetDetector.h>
DoubleResetDetector *drd;

LedControl lc = LedControl(13, 14, 15);

void setup() {
    lc.shutdown(0, false);
    lc.setIntensity(0, 15);
    lc.clearDisplay(0);

    for (uint8_t i = 0; i < 8; i++) {
        lc.setDigit(0, i, 8, true);
    }

    for (uint8_t i = 15; i > 1; i--) {
        lc.setIntensity(0, i);
        delay(250);
    }
}

void displayN(int16_t n) {
    if (n < 0) {
        lc.setRow(0, 0, 0);
        lc.setRow(0, 1, 0b00000001);
        lc.setRow(0, 2, 0b00000001);
        lc.setRow(0, 3, 0);
        return;
    }

    if (n > 9999) {
        lc.setRow(0, 0, 0);
        lc.setRow(0, 1, 0b00110111); // H
        lc.setRow(0, 2, 0b00110111); // H
        lc.setRow(0, 3, 0);
        return;
    }

    if (n > 999) {
        lc.setChar(0, 0, n / 1000, false);
    } else {
        lc.setRow(0, 0, 0b00010101); // n
    }

    if (n > 99) {
        lc.setChar(0, 1, (n % 1000) / 100, false);
    } else {
        lc.setRow(0, 1, 0b00001001); // =
    }

    if (n > 9) {
        lc.setChar(0, 2, (n % 100) / 10, false);
    } else {
        lc.setRow(0, 2, 0);
    }

    lc.setChar(0, 3, n % 10, false);
}

void loop() {
    static int16_t n = -200;

    displayN(n);

    if (++n == 10100) {
        n = -200;
    }

    if (n > 999 && n < 9999) {
        delay(1);
    } else if (n < 0 || n > 99) {
        delay(10);
    } else if (n > 9) {
        delay(100);
    } else {
        delay(1000);
    }
}