#include <stdbool.h>
#include <stdint.h>

#include "CANdler.h"
#include "msgIDs.h"
#include "grIDs.h"

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

            break;

        case MSG_DASH_WARNING_FLAGS:

            break;

        case MSG_STEERING_STATUS:

            break;
        

    }

}