/*

Copyright 2025 Marc Ketel
SPDX-License-Identifier: Apache-2.0

*/

#include "DisplayManager.h"

DisplayManager::DisplayManager(uint8_t dataPin, uint8_t clkPin, uint8_t csPin)
    : lc(dataPin, clkPin, csPin, 1) {
}

void DisplayManager::initialize() {
    lc.shutdown(0, false);
    lc.setIntensity(0, 2);
    for (uint8_t i = 0; i < 4; i++) {
        lc.setRow(0, i, 0xFF);
    }
}

void DisplayManager::displayNumber(int16_t n, bool open) {
    if (n < 0) {
        showPattern(0, 0);
        showPattern(1, MINUS_SIGN);
        showPattern(2, MINUS_SIGN);
        showPattern(3, 0);
        return;
    }

    if (n > 9999) {
        showPattern(0, 0);
        showPattern(1, DisplayManager::CHAR_H);
        showPattern(2, DisplayManager::CHAR_H);
        showPattern(3, 0);
        return;
    }

    if (n > 999) {
        lc.setChar(0, 0, n / 1000, false);
    } else {
        showPattern(0, CHAR_N_SMALL);
    }

    if (n > 99) {
        lc.setChar(0, 1, (n % 1000) / 100, false);
    } else {
        showPattern(1, CHAR_EQUALS);
    }

    if (n > 9) {
        lc.setChar(0, 2, (n % 100) / 10, false);
    } else {
        showPattern(2, 0);
    }

    lc.setChar(0, 3, n % 10, (open == false));
}

void DisplayManager::showPattern(uint8_t digit, uint8_t pattern) {
    lc.setRow(0, digit, pattern);
}

void DisplayManager::showText(const char *text) {
    // Display up to 4 characters
    for (uint8_t i = 0; i < 4 && text[i] != '\0'; i++) {
        switch (text[i]) {
            case ' ':
                showPattern(i, CHAR_BLANK);
                break;
            case '-':
                showPattern(i, MINUS_SIGN);
                break;
            case '=':
                showPattern(i, CHAR_EQUALS);
                break;
            case 'A':
                showPattern(i, CHAR_A);
                break;
            case 'F':
                showPattern(i, CHAR_F);
                break;
            case 'i':
                showPattern(i, CHAR_I);
                break;
            case 'M':
                showPattern(i, CHAR_N);
                break;
            case 'n':
                showPattern(i, CHAR_N_SMALL);
                break;
            case 'N':
                showPattern(i, CHAR_N);
                break;
            case 'P':
                showPattern(i, CHAR_P);
                break;
            case 'Q':
                showPattern(i, CHAR_Q);
                break;
            case 't':
                showPattern(i, CHAR_T);
                break;
            case 'v':
                showPattern(i, CHAR_V);
                break;
            case '_':
                showPattern(i, UNDERSCORE);
                break;
            default:
                showPattern(i, 0);
                break;
        }
    }
}