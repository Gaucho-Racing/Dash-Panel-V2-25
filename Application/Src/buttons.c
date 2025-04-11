#include <stdbool.h>

#include "buttons.h"
#include "stm32u5xx_hal.h"
#include "main.h"
#include "dash.h"

volatile ButtonState globalButtonState = {false, false};

void pollButtonState(void)
{
    bool result = false;

    // if (globalButtonState.rtd != HAL_GPIO_ReadPin()) {
    //     globalButtonState.rtd = HAL_GPIO_ReadPin();
    //     result = true;
    // }

    // if (globalButtonState.tsActive != HAL_GPIO_ReadPin()) {
    //     globalButtonState.tsActive = HAL_GPIO_ReadPin();
    //     result = true;
    // }

    if (result)
        recievedNewInformationPleaseRefresh();
}
