#include <stdint.h>

#ifndef UTILS_H
#define UTILS_H
    uint8_t getBit(uint8_t number, uint8_t indexFromLeft);
    uint8_t getBits(uint8_t number, uint8_t indexFromLeft, uint8_t length);
    uint8_t findTernaryMax(const uint8_t a, const uint8_t b, const uint8_t c);
#endif
