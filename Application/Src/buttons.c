#include <stdbool.h>

#include "buttons.h"
#include "cmsis_os2.h"
#include "stm32u5xx.h"
#include "grIDs.h"
#include "fdcan.h"
#include "main.h"
#include "dash.h"
#include "msgIDs.h"
#include "dash.h"

void updateButtonColors(void* args)
{
    UNUSED(args);

    for (;;)
    {
        switch(globalStatus.ecuState)
        {
            case PRECHARGE_COMPLETE:
                colorPin(COLOR_BLUE_PULSE, RTD_BUTTON);
                break;

            case DRIVE_STANDBY:
            case DRIVE_ACTIVE_IDLE:
            case DRIVE_ACTIVE_POWER:
            case DRIVE_ACTIVE_REGEN:
                colorPin(COLOR_GREEN, RTD_BUTTON);
                break;

            default:
                colorPin(COLOR_RED, RTD_BUTTON);
                break;
        }

        switch(globalStatus.ecuState)
        {
            case TS_DISCHARGE_OFF:
            case ERRORSTATE:
                colorPin(COLOR_RED, TS_ACTIVE_BUTTON);
                break;

            case GLV_ON:
                colorPin(COLOR_BLUE_PULSE, TS_ACTIVE_BUTTON);
                break;

            default:
                colorPin(COLOR_GREEN, TS_ACTIVE_BUTTON);
        }

        osDelay(UPDATE_BUTTON_COLORS_DELAY);
    }
}

void pollButtonState(void* args)
{
    UNUSED(args);

    for (;;)
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
            recievedNewInformationPleaseRefresh();
        }

        osDelay(POLL_BUTTON_STATE_DELAY);
    }
}
