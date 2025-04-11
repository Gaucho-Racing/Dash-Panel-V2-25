#include <stdint.h>

#ifndef DASH_H
    #define DASH_H

    #define BAD_TIME_Negative1 -1

    /* Must match ECU-25 EXACTLY */
    typedef enum {
        GLV_ON = (uint8_t)1,
        PRECHARGE_ENGAGED = (uint8_t)2,
        PRECHARGING = (uint8_t)3,
        PRECHARGE_COMPLETE = (uint8_t)4,
        DRIVE_STANDBY = (uint8_t)5,
        DRIVE_ACTIVE_IDLE = (uint8_t)6,
        DRIVE_ACTIVE_POWER = (uint8_t)7,
        DRIVE_ACTIVE_REGEN = (uint8_t)8,
        TS_DISCHARGE_OFF = (uint8_t)9,
        REFLASH_TUNE = (uint8_t)10,
        ERRORSTATE = (uint8_t)11,
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
        uint8_t glvStateOfCharge; // SOC for LV
        uint8_t motorTemperatures[4]; // Temperatures for all 4(?) motors X 
        uint8_t inverterTemperatures[4]; // Temepratures for all 4(?) inverters
        uint8_t inverterCurrents[4]; // Currents for all 4(?) motors; note: only the first two will be used if the DTI is used X 
        uint8_t brakeTemps[4]; // Temperatures for all 4 brakes
        ECUState ecuState; // Current state of the car
        uint16_t tsVoltage;
        char debugMessage[8];   // Will be empty if no message sent
    } DashInfo;

    extern volatile DashInfo globalStatus;

    void newDataPleaseRefresh(void);
#endif
