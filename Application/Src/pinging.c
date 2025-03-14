#include <stdint.h>

#include "CANdler.h"
#include "pinging.h"
#include "msgIDs.h"
#include "fdcan.h"

void respondToPing(uint8_t destID, uint32_t timestamp)
{
    writeMessage(MSG_PING, destID, (uint8_t*)&timestamp, sizeof(uint32_t));
}
