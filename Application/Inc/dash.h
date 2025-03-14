#include <stdint.h>

#ifndef DASH_H
#define DASH_H

typedef struct {
    struct {
        uint8_t ledBits;
        int8_t tsButtonData;
        int8_t rtdButtonData; 
    } DashStatusMsg;

    uint8_t bseAppsViolation;
    uint16_t vehicleSpeed;
    uint8_t maxCellTemp;
    uint16_t dtiAcCurrent;
    uint8_t accumulatorStateOfCharge;
    uint8_t motorTemperature[4];
    uint8_t ecuState;

} DashInfo;

extern volatile DashInfo globalStatus;

#endif
