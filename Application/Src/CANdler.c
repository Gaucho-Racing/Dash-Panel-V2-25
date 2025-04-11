#include <stdbool.h>
#include <stdint.h>
#include <string.h>

#include "fdcan.h"
#include "CANdler.h"
#include "msgIDs.h"
#include "grIDs.h"
#include "dash.h"
#include "customIDs.h"
#include "utils.h"

volatile uint8_t numberOfBadMessages = 0;

void handleCANMessage(uint16_t msgID, uint8_t srcID, uint8_t *data, uint32_t length)
{
    switch (msgID) {
        case MSG_DEBUG_2_0:
            if (length > 8) {
                numberOfBadMessages++;
                return;
            } else {
                numberOfBadMessages += (numberOfBadMessages > 0) ? -1 : 0;
            }

            memcpy((char*)globalStatus.debugMessage, data, 8);

            break;
        
        case MSG_PING:
            if (length != 4) {
                numberOfBadMessages++;
                return;
            } else {
                numberOfBadMessages += (numberOfBadMessages > 0) ? -1 : 0;
            }

            writeMessage(MSG_PING, srcID, data, sizeof(uint32_t));  // Write back to ya buckaroo

            break;
        
        case MSG_ECU_STATUS_1:
            if (length != 8) {
                numberOfBadMessages++;
                return;
            } else {
                numberOfBadMessages += (numberOfBadMessages > 0) ? -1 : 0;
            }

            ECUStatusMsgOne* ecuStatusMsgOne = (ECUStatusMsgOne*) data;
            globalStatus.ecuState = ecuStatusMsgOne->ECUState;
            globalStatus.maxCellTemp = (uint8_t)(ecuStatusMsgOne->MaxCellTemp * 0.25);
            globalStatus.glvStateOfCharge = ecuStatusMsgOne->GLVStateOfCharge;

            break;

        case MSG_ECU_STATUS_2:
            if (length != 8) {
                numberOfBadMessages++;
                return;
            } else {
                numberOfBadMessages += (numberOfBadMessages > 0) ? -1 : 0;
            }

            ECUStatusMsgTwo* ecuStatusMsgTwo = (ECUStatusMsgTwo*) data;
            globalStatus.vehicleSpeed = (uint16_t)(ecuStatusMsgTwo->VehicleSpeed * 0.01);
            globalStatus.tsVoltage = (uint16_t)(ecuStatusMsgTwo->TractiveSystemVoltage * 0.01);

            break;

        case MSG_DASH_CONFIG:
            if (length != 7) {
                numberOfBadMessages++;
                return;
            } else {
                numberOfBadMessages += (numberOfBadMessages > 0) ? -1 : 0;
            }

            DashStatusMsg* dashStatusMsg = (DashStatusMsg*) data;
            globalStatus.dashStatusMsg.ledBits = dashStatusMsg->ledBits;
            globalStatus.dashStatusMsg.rtdButtonData = dashStatusMsg->rtdButtonData;
            globalStatus.dashStatusMsg.tsButtonData = dashStatusMsg->tsButtonData;

            break;

        case MSG_DASH_WARNING_FLAGS:
            if (length != 1) {
                numberOfBadMessages++;
                return;
            } else {
                numberOfBadMessages += (numberOfBadMessages > 0) ? -1 : 0;
            }

            DashWarningFlagsMsg* dashWarningFlagsMsg = (DashWarningFlagsMsg*) data;
            globalStatus.bseAppsViolation = getBit(dashWarningFlagsMsg->flags, 0);

            break;

        case MSG_STEERING_STATUS:
            if (length != 4) {
                numberOfBadMessages++;
                return;
            } else {
                numberOfBadMessages += (numberOfBadMessages > 0) ? -1 : 0;
            }

            SteeringStatusMsg* steeringStatusMsg = (SteeringStatusMsg*) data;
            globalStatus.steeringStatusMsg.currentEncoder = steeringStatusMsg->currentEncoder;
            globalStatus.steeringStatusMsg.torqueMapEncoder = steeringStatusMsg->torqueMapEncoder;
            globalStatus.steeringStatusMsg.regenEncoder = steeringStatusMsg->regenEncoder;

            break;

        case MSG_ACU_STATUS_1:
            if (length != 8) {
                numberOfBadMessages++;
                return;
            } else {
                numberOfBadMessages += (numberOfBadMessages > 0) ? -1 : 0;
            }

            ACU_Status_MsgOne* acuStatusMsgOne = (ACU_Status_MsgOne*) data;
            globalStatus.accumulatorStateOfCharge = (uint8_t)(acuStatusMsgOne->Accumulator_SOC * 20.0 / 51.0);

            break;

        case MSG_DTI_DATA_2:
            if (length != 8) {
                numberOfBadMessages++;
                return;
            } else {
                numberOfBadMessages += (numberOfBadMessages > 0) ? -1 : 0;
            }

            DTIDataTwoMsg* dtiDataTwoMsg = (DTIDataTwoMsg*) data;
            globalStatus.dtiAcCurrent = (uint16_t)(dtiDataTwoMsg->AC_Current * 0.01);

            break;
        
        case MSG_INVERTER_STATUS_1:
            if (length != 6) {
                numberOfBadMessages++;
                return;
            } else {
                numberOfBadMessages += (numberOfBadMessages > 0) ? -1 : 0;
            }

            InverterStatusOne* invOneMsg = (InverterStatusOne*)data;

            switch(srcID) {
                case GR_GR_INVERTER_1:
                    globalStatus.inverterCurrents[0] = invOneMsg->AC_Current;
                    break;
                case GR_GR_INVERTER_2:
                    globalStatus.inverterCurrents[1] = invOneMsg->AC_Current;
                    break;
                case GR_GR_INVERTER_3:
                    globalStatus.inverterCurrents[2] = invOneMsg->AC_Current;
                    break;
                case GR_GR_INVERTER_4:
                    globalStatus.inverterCurrents[3] = invOneMsg->AC_Current;
                    break;
            }

            break;
        
        case MSG_INVERTER_STATUS_2:
            if (length != 6) {
                numberOfBadMessages++;
                return;
            } else {
                numberOfBadMessages += (numberOfBadMessages > 0) ? -1 : 0;
            }

            InverterStatusTwo* invTwoMsg = (InverterStatusTwo*)data;

            switch (srcID) {
                case GR_GR_INVERTER_1:
                    globalStatus.inverterTemperatures[0] = findTernaryMax(invTwoMsg->uTemp, invTwoMsg->vTemp, invTwoMsg->wTemp);
                    break;
                case GR_GR_INVERTER_2:
                    globalStatus.inverterTemperatures[1] = findTernaryMax(invTwoMsg->uTemp, invTwoMsg->vTemp, invTwoMsg->wTemp);
                    break;
                case GR_GR_INVERTER_3:
                    globalStatus.inverterTemperatures[2] = findTernaryMax(invTwoMsg->uTemp, invTwoMsg->vTemp, invTwoMsg->wTemp);
                    break;
                case GR_GR_INVERTER_4:
                    globalStatus.inverterTemperatures[3] = findTernaryMax(invTwoMsg->uTemp, invTwoMsg->vTemp, invTwoMsg->wTemp);
                    break;
            }

            break;

        case MSG_INVERTER_STATUS_3:
            if (length != 2) {
                numberOfBadMessages++;
                return;
            } else {
                numberOfBadMessages += (numberOfBadMessages > 0) ? -1 : 0;
            }

            InverterStatusThree* invThreeMsg = (InverterStatusThree*)data;

            switch (srcID) {
                case GR_GR_INVERTER_1:
                    globalStatus.motorTemperatures[0] = invThreeMsg->Motor_Temp;
                    break;
                case GR_GR_INVERTER_2:
                    globalStatus.motorTemperatures[1] = invThreeMsg->Motor_Temp;
                    break;
                case GR_GR_INVERTER_3:
                    globalStatus.motorTemperatures[2] = invThreeMsg->Motor_Temp;
                    break;
                case GR_GR_INVERTER_4:
                    globalStatus.motorTemperatures[3] = invThreeMsg->Motor_Temp;
                    break;
            }
            break;
        
        case MSG_SPECIFIC_BRAKE_IR:
            if (length != 2) {
                numberOfBadMessages++;
                return;
            } else {
                numberOfBadMessages += (numberOfBadMessages > 0) ? -1 : 0;
            }

            SpecificBrakeIR* brakeMsg = (SpecificBrakeIR*)data;

            if (brakeMsg->Wheel_Identifier > 3)
                break;

            globalStatus.brakeTemps[brakeMsg->Wheel_Identifier] = brakeMsg->Brake_Temp;

            break;

        default:
            return;
    }

    recievedValidMessage();
}
