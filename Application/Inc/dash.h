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
    uint8_t currentEncoder;
    uint8_t torqueMapEncoder;
    uint8_t regenEncoder;
} SteeringStatusMsg;

typedef struct {
    uint8_t ledBits;
    int8_t tsButtonData;
    int8_t rtdButtonData;
} DashStatusMsg;

typedef struct {
    DashStatusMsg dashStatusMsg;
    SteeringStatusMsg steeringStatusMsg;

    uint8_t bseAppsViolation;  
    uint16_t vehicleSpeed; // From ECU Status 2
    uint8_t maxCellTemp; // Max cell temp from entire car
    uint16_t dtiAcCurrent; // Current of back motor X
    uint8_t accumulatorStateOfCharge; // SOC aka battery life
    uint8_t motorTemperatures[4]; // Temperatures for all 4(?) motors X 
    uint8_t inverterCurrents[4]; // Currents for all 4(?) motors; note: only the first two will be used if the DTI is used X 
    ECUState ecuState; // Current state of the car
    uint16_t tsVoltage;
} DashInfo;

extern volatile DashInfo globalStatus;

#endif
