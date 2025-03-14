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
    uint8_t MaxCellTemp;
    
} DashInfo;

extern volatile DashInfo globalStatus;

#endif
