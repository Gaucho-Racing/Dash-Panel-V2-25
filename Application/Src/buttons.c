#include <stdbool.h>

#include "buttons.h"
#include "stm32u5xx_hal.h"
#include "grIDs.h"
#include "fdcan.h"
#include "main.h"
#include "dash.h"
#include "msgIDs.h"
#include "dash.h"

void pollButtonState(void)
{
    bool change = false;

    uint8_t value = HAL_GPIO_ReadPin(RTD_GPIO_Port, RTD_Pin);

    if (globalStatus.dashStatusMsg.rtdButtonData != value)
    {
        globalStatus.dashStatusMsg.rtdButtonData = value;
        change = true;
    }

    value = HAL_GPIO_ReadPin(TS_ACTIVE_GPIO_Port, TS_ACTIVE_Pin);

    if (globalStatus.dashStatusMsg.tsButtonData != value)
    {
        globalStatus.dashStatusMsg.tsButtonData = value;
        change = true;
    }

    if (change)
    {
        writeMessage(MSG_DASH_STATUS, GR_ECU, (uint8_t*)&globalStatus.dashStatusMsg, 3);
        recievedNewInformationPleaseRefresh();
    }
}
