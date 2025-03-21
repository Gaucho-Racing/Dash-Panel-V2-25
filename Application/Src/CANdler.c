#include <stdbool.h>
#include <stdint.h>

#include "fdcan.h"
#include "CANdler.h"
#include "msgIDs.h"
#include "grIDs.h"
#include "dash.h"
#include "customIDs.h"
#include "utils.h"

volatile uint8_t numberOfBadMessages = 0;

void handleCANMessage(uint16_t msgID, uint8_t srcID, uint8_t *data, uint32_t length, uint32_t timestamp) {
    UNUSED(timestamp);

    switch (msgID) {
        case MSG_DEBUG_2_0:
            if (length > 8) {
                numberOfBadMessages++;
                return;
            } else {
                numberOfBadMessages += (numberOfBadMessages > 0) ? -1 : 0;
            }

            char* debug2String = (char*)data;
            UNUSED(debug2String);

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
            /* May need to add more here if more warning flags are added */
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
        
        case MSG_ECU_STATUS_1:
            if (length != 8) {
                numberOfBadMessages++;
                return;
            } else {
                numberOfBadMessages += (numberOfBadMessages > 0) ? -1 : 0;
            }

            ECUStatusMsgOne* ecuStatusMsgOne = (ECUStatusMsgOne*) data;
            globalStatus.ecuState = ecuStatusMsgOne->ECUState;
            globalStatus.maxCellTemp = ecuStatusMsgOne->MaxCellTemp;

            break;

        case MSG_ECU_STATUS_2:
            if (length != 8) {
                numberOfBadMessages++;
                return;
            } else {
                numberOfBadMessages += (numberOfBadMessages > 0) ? -1 : 0;
            }

            ECUStatusMsgTwo* ecuStatusMsgTwo = (ECUStatusMsgTwo*) data;
            globalStatus.vehicleSpeed = ecuStatusMsgTwo->VehicleSpeed;
            globalStatus.tsVoltage = ecuStatusMsgTwo->TractiveSystemVoltage;

            break;
        
        case MSG_ACU_STATUS_1:
            if (length != 8) {
                numberOfBadMessages++;
                return;
            } else {
                numberOfBadMessages += (numberOfBadMessages > 0) ? -1 : 0;
            }

            ACU_Status_MsgOne* acuStatusMsgOne = (ACU_Status_MsgOne*) data;
            globalStatus.accumulatorStateOfCharge = acuStatusMsgOne->Accumulator_SOC;

            break;

        case MSG_DTI_DATA_2:
            if (length != 8) {
                numberOfBadMessages++;
                return;
            } else {
                numberOfBadMessages += (numberOfBadMessages > 0) ? -1 : 0;
            }

            DTIDataTwoMsg* dtiDataTwoMsg = (DTIDataTwoMsg*) data;
            globalStatus.dtiAcCurrent = dtiDataTwoMsg->AC_Current;
            break;
        
        case MSG_INVERTER_STATUS_2:
            if (length != 6) {
                numberOfBadMessages++;
                return;
            } else {
                numberOfBadMessages += (numberOfBadMessages > 0) ? -1 : 0;
            }

            InverterStatusTwo* invTwoMsg = (InverterStatusTwo*)data;
            // SET A WAY TO FIGURE OUT WHAT THE INDEX IS
            globalStatus.motorTemperatures[0] = findTernaryMax(invTwoMsg->uTemp, invTwoMsg->vTemp, invTwoMsg->wTemp);

            break;
    }
}
