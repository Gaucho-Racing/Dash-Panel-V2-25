#include <stdbool.h>

#include "buttons.h"
#include "stm32u5xx_hal.h"
#include "main.h"
#include "dash.h"

void pollButtonState(void)
{
    bool result = false;

    uint8_t value = HAL_GPIO_ReadPin(RTD_GPIO_Port, RTD_Pin);

    if (globalStatus.dashStatusMsg.rtdButtonData != value)
    {
        globalStatus.dashStatusMsg.rtdButtonData = value;
        result = true;
    }

    value = HAL_GPIO_ReadPin(TS_ACTIVE_GPIO_Port, TS_ACTIVE_Pin);

    if (globalStatus.dashStatusMsg.tsButtonData != value)
    {
        globalStatus.dashStatusMsg.tsButtonData = value;
        result = true;
    }

    if (result)
    {
        // TODO: Send a Dash Status message
        recievedNewInformationPleaseRefresh();
    }
}
