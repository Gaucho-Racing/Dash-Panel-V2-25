#include <stdbool.h>

#include "buttons.h"
#include "stm32u5xx_hal.h"
#include "main.h"
#include "dash.h"

volatile ButtonState globalButtonState = {false, false};

void pollButtonState(void)
{
    bool result = false;

    uint8_t value = HAL_GPIO_ReadPin(RTD_GPIO_Port, RTD_Pin);

    if (globalButtonState.rtd != value)
    {
        globalButtonState.rtd = value;
        result = true;
    }

    value = HAL_GPIO_ReadPin(TS_ACTIVE_GPIO_Port, TS_ACTIVE_Pin);

    if (globalButtonState.tsActive != value)
    {
        globalButtonState.tsActive = value;
        result = true;
    }

    if (result)
        recievedNewInformationPleaseRefresh();
}
