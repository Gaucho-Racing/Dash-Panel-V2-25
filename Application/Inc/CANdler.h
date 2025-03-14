#include <stdint.h>

#ifndef CANDLER_H
#define CANDLER_H

extern volatile uint8_t numberOfBadMessages;

void handleCANMessage(uint16_t msgID, uint8_t srcID, uint8_t *data, uint32_t length, uint32_t timestamp);

typedef struct {
    uint8_t flags;
} DashWarningFlagsMsg;

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

typedef struct {
    uint16_t Accumulator_Voltage;
    uint16_t TS_Voltage;
    uint16_t Accumulator_Current;
    uint8_t Accumulator_SOC;
    uint8_t GLV_SOC;
} ACU_Status_MsgOne;


typedef struct {
    uint8_t V_20v_Voltage;
    uint8_t V_12v_Voltage;
    uint8_t SDC_Voltage;
    uint8_t Min_Cell_Voltage;
    uint8_t Max_Cell_Temp;
    uint8_t Error_Warning_Bits;
    uint8_t Precharge_Error_IR_State_Software_Latch_Bits;  // All in one!
} ACU_Status_MsgTwo;



#endif
