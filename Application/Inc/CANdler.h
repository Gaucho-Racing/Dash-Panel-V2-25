#include <stdint.h>

#ifndef CANDLER_H
#define CANDLER_H

void handleCANMessage(uint16_t msgID, uint8_t srcID, uint8_t *data, uint32_t length, uint32_t timestamp);

typedef union {
    struct {
        uint8_t ECUState;
        uint8_t StatusBits[3];
        uint8_t PowerLevelTorqueMap;
        uint8_t MaxCellTemp;
        uint8_t AccumulatorStateOfCharge;
        uint8_t GLVStateOfCharge;
        uint16_t TractiveSystemVoltage;
        uint16_t VehicleSpeed;
        uint16_t FRWheelRPM;
        uint16_t FLWheelRPM;
        uint16_t RRWheelRPM;
        uint16_t RLWheelRPM;
    };

    struct {
        uint8_t ECUStatusMsgOne[8];
        uint8_t ECUStatusMsgTwo[8];
        uint8_t ECUStatusMsgThree[4];
    };
} ECUStatusMsgs;

#endif
