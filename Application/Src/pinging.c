#include <stdint.h>

#include "CANdler.h"
#include "pinging.h"
#include "msgIDs.h"
#include "grIDs.h"

void respondToPing(uint8_t destID, uint32_t timestamp)
{
    uint32_t tick = HAL_GetTick();
    writeMessage(1, MSG_PING, destID, (uint8_t *)&tick, sizeof(uint32_t));
}
