/*

Copyright 2025 Marc Ketel
SPDX-License-Identifier: Apache-2.0

*/

#ifndef DISPLAY_MANAGER_H
#define DISPLAY_MANAGER_H

#include <MaxLedControl.h>

class DisplayManager {
public:
    DisplayManager(uint8_t dataPin, uint8_t clkPin, uint8_t csPin);
    void initialize();
    void displayNumber(int16_t n, bool open = true);
    void showPattern(uint8_t digit, uint8_t pattern);
    uint8_t getPattern(char character);
    void showText(const char *text);

private:
    LedControl lc;

    static const uint8_t PATTERN_UNDEFINED = 0b11001001;
    static const uint8_t patterns[96];
};

#endif