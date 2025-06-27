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
    void displayNumber(int16_t n);
    void showPattern(uint8_t digit, uint8_t pattern);
    void showText(const char *text);

private:
    LedControl lc;

    // Display patterns
    static const uint8_t CHAR_A = 0b01110111;
    static const uint8_t CHAR_BLANK = 0b00000000;
    static const uint8_t CHAR_EQUALS = 0b00001001;
    static const uint8_t CHAR_F = 0b01000111;
    static const uint8_t CHAR_H = 0b00110111;
    static const uint8_t CHAR_I = 0b00010000;
    static const uint8_t CHAR_N = 0b01110110;
    static const uint8_t CHAR_N_SMALL = 0b00010101;
    static const uint8_t CHAR_P = 0b01100111;
    static const uint8_t CHAR_Q = 0b01110011;
    static const uint8_t CHAR_T = 0b00001111;
    static const uint8_t CHAR_V = 0b00011100;
    static const uint8_t MINUS_SIGN = 0b00000001;
};

#endif