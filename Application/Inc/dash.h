#include <stdint.h>

#ifndef DASH_H
#define DASH_H

typedef enum {
    GLV_ON = 1,
    PRECHARGE_ENGAGED = 2,
    PRECHARGING = 3,
    PRECHARGE_COMPLETE = 4,
    DRIVE_STANDBY = 5,
    DRIVE_ACTIVE_IDLE = 6,
    DRIVE_ACTIVE_POWER = 7,
    DRIVE_ACTIVE_REGEN = 8,
    TS_DISCHARGE_OFF = 9,
    REFLASH_TUNE = 10,
    ERRORSTATE = 11
} ECUState;

typedef struct {
    struct {
        uint8_t ledBits;
        int8_t tsButtonData;
        int8_t rtdButtonData; 
    } DashStatusMsg;

    uint8_t bseAppsViolation;
    uint16_t vehicleSpeed; // From ECU Status
    uint8_t maxCellTemp; // Max cell temp from entire car
    uint16_t dtiAcCurrent; // Current of back motor
    uint8_t accumulatorStateOfCharge; // SOC aka battery life
    uint8_t motorTemperature[4]; // Temperatures for all 4(?) motors
    ECUState ecuState; // Current state of the car

} DashInfo;

extern volatile DashInfo globalStatus;

#endif
