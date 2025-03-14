#include <stdbool.h>
#include <stdint.h>

#include "CANdler.h"
#include "msgIDs.h"
#include "grIDs.h"
#include "dash.h"
#include "customIDs.h"
#include "utils.h"

extern volatile globalStatus;
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

            respondToPing(srcID, *(uint32_t*)data);

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
            globalStatus.bseAppsViolation = (getBit(dashWarningFlagsMsg->flags, 0));
            /* May need to add more here if more warning flags are added */
            break;

        case MSG_STEERING_STATUS:
            if (length != 4) {
                numberOfBadMessages++;
                return;
            } else {
                numberOfBadMessages += (numberOfBadMessages > 0) ? -1 : 0;
            }

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
        
        // Prob don't actually need this
        // case MSG_ACU_STATUS_2:
        //     if (length != 7) {
        //         numberOfBadMessages++;
        //         return;
        //     } else {
        //         numberOfBadMessages += (numberOfBadMessages > 0) ? -1 : 0;
        //     }
        //     ACU_Status_MsgTwo* acuStatusMsgTwo = (ACU_STATUS_MSGTwo*) data;
        //     break;

        case MSG_DTI_DATA_2:
            if (length != 8) {
                numberOfBadMessages++;
                return;
            } else {
                numberOfBadMessages += (numberOfBadMessages > 0) ? -1 : 0;
            }

            break;

    }

}
