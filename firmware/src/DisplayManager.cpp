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
        DisplayManager::showText(" -- ");
        return;
    }

    if (n > 9999) {
        DisplayManager::showText(" HH ");
        return;
    }

    if (n > 999) {
        lc.setChar(0, 0, n / 1000, false);
    } else {
        showPattern(0, DisplayManager::getPattern('n'));
    }

    if (n > 99) {
        lc.setChar(0, 1, (n % 1000) / 100, false);
    } else {
        showPattern(1, DisplayManager::getPattern('='));
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
        showPattern(i, DisplayManager::getPattern(text[i]));
    }
}

uint8_t DisplayManager::getPattern(char character) {
    return DisplayManager::patterns[(int)character - 32];
}

/**
 * 7-segment led to bit mapping: dp, a, b, c, d, e, f, g.
 */
const uint8_t DisplayManager::patterns[96] = {
    0b00000000,        // space
    PATTERN_UNDEFINED, // !
    PATTERN_UNDEFINED, // "
    PATTERN_UNDEFINED, // #
    PATTERN_UNDEFINED, // $
    PATTERN_UNDEFINED, // %
    PATTERN_UNDEFINED, // &
    PATTERN_UNDEFINED, // '
    PATTERN_UNDEFINED, // (
    PATTERN_UNDEFINED, // )
    PATTERN_UNDEFINED, // *
    PATTERN_UNDEFINED, // +
    PATTERN_UNDEFINED, // ,
    0b00000001,        // -
    PATTERN_UNDEFINED, // .
    PATTERN_UNDEFINED, // /
    0b01111110,        // 0
    0b00110000,        // 1
    0b01101101,        // 2
    0b01111001,        // 3
    0b00110011,        // 4
    0b01011011,        // 5
    0b01011111,        // 6
    0b01110000,        // 7
    0b01111111,        // 8
    0b01111011,        // 9
    PATTERN_UNDEFINED, // :
    PATTERN_UNDEFINED, // ;
    PATTERN_UNDEFINED, // <
    0b00001001,        // =
    PATTERN_UNDEFINED, // >
    PATTERN_UNDEFINED, // ?
    PATTERN_UNDEFINED, // @
    0b01110111,        // A
    PATTERN_UNDEFINED, // B
    PATTERN_UNDEFINED, // C
    PATTERN_UNDEFINED, // D
    PATTERN_UNDEFINED, // E
    0b01000111,        // F
    PATTERN_UNDEFINED, // G
    0b00110111,        // H
    0b00010000,        // I
    PATTERN_UNDEFINED, // J
    PATTERN_UNDEFINED, // K
    PATTERN_UNDEFINED, // L
    0b01110110,        // N
    PATTERN_UNDEFINED, // M
    PATTERN_UNDEFINED, // O
    0b01100111,        // P
    0b01110011,        // Q
    PATTERN_UNDEFINED, // R
    0b00001111,        // T
    PATTERN_UNDEFINED, // S
    PATTERN_UNDEFINED, // U
    0b00011100,        // V
    0b00111111,        // W
    PATTERN_UNDEFINED, // X
    PATTERN_UNDEFINED, // Y
    PATTERN_UNDEFINED, // Z
    PATTERN_UNDEFINED, // [
    PATTERN_UNDEFINED, // backslash
    PATTERN_UNDEFINED, // ]
    PATTERN_UNDEFINED, // ^
    0b00001000,        // _
    0b00000010,        // `
    0b01110111,        // a
    PATTERN_UNDEFINED, // b
    PATTERN_UNDEFINED, // c
    PATTERN_UNDEFINED, // d
    PATTERN_UNDEFINED, // e
    PATTERN_UNDEFINED, // f
    PATTERN_UNDEFINED, // g
    PATTERN_UNDEFINED, // h
    PATTERN_UNDEFINED, // i
    PATTERN_UNDEFINED, // j
    PATTERN_UNDEFINED, // k
    PATTERN_UNDEFINED, // l
    PATTERN_UNDEFINED, // m
    0b00010101,        // n
    PATTERN_UNDEFINED, // o
    PATTERN_UNDEFINED, // p
    PATTERN_UNDEFINED, // q
    PATTERN_UNDEFINED, // r
    PATTERN_UNDEFINED, // s
    0b00001111,        // t
    PATTERN_UNDEFINED, // u
    PATTERN_UNDEFINED, // v
    0b00111111,        // w
    PATTERN_UNDEFINED, // x
    PATTERN_UNDEFINED, // y
    PATTERN_UNDEFINED, // z
    PATTERN_UNDEFINED, // {
    0b00000110,        // |
    PATTERN_UNDEFINED, // }
    0b01000000,        // ~
    0b00000000,        // (del)
};