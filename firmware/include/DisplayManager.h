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
    enum displayMode {
        clock, nteller, thermometer, highT, lowT
    };
    void initialize();
    void displayNumber(int16_t n, bool open = true);
    void displayTime(int16_t hours, int16_t minutes);
    void toggleDisplay(displayMode mode);
    void showPattern(uint8_t digit, uint8_t pattern);
    uint8_t getPattern(char character);
    void showText(const char *text);

private:
    LedControl lc;
    int16_t n;
    int16_t hours;
    int16_t minutes;
    static const uint8_t PATTERN_UNDEFINED = 0b11001001;
    static const uint8_t patterns[96];
};

#endif