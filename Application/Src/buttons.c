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
#include "gpio.h"

void colorPin(Color color, ButtonNames button)
{
    switch(color)
    {
        case COLOR_RED:
            if (button == 1)
            {
                HAL_GPIO_WritePin(LED1R_GPIO_Port, LED1R_Pin, GPIO_PIN_SET);
                HAL_GPIO_WritePin(LED1G_GPIO_Port, LED1G_Pin, GPIO_PIN_RESET);
                HAL_GPIO_WritePin(LED1B_GPIO_Port, LED1B_Pin, GPIO_PIN_RESET);
            }
            else if (button == 2)
            {
                HAL_GPIO_WritePin(LED2R_GPIO_Port, LED2R_Pin, GPIO_PIN_SET);
                HAL_GPIO_WritePin(LED2G_GPIO_Port, LED2G_Pin, GPIO_PIN_RESET);
                HAL_GPIO_WritePin(LED2B_GPIO_Port, LED2B_Pin, GPIO_PIN_RESET);
            }

            break;

        case COLOR_GREEN:
            if (button == 1)
            {
                HAL_GPIO_WritePin(LED1R_GPIO_Port, LED1R_Pin, GPIO_PIN_RESET);
                HAL_GPIO_WritePin(LED1G_GPIO_Port, LED1G_Pin, GPIO_PIN_SET);
                HAL_GPIO_WritePin(LED1B_GPIO_Port, LED1B_Pin, GPIO_PIN_RESET);
            }
            else if (button == 2)
            {
                HAL_GPIO_WritePin(LED2R_GPIO_Port, LED2R_Pin, GPIO_PIN_RESET);
                HAL_GPIO_WritePin(LED2G_GPIO_Port, LED2G_Pin, GPIO_PIN_SET);
                HAL_GPIO_WritePin(LED2B_GPIO_Port, LED2B_Pin, GPIO_PIN_RESET);
            }
            break;

        case COLOR_BLUE_PULSE:
            if (button == 1)
            {
                HAL_GPIO_WritePin(LED1R_GPIO_Port, LED1R_Pin, GPIO_PIN_RESET);
                HAL_GPIO_WritePin(LED1G_GPIO_Port, LED1G_Pin, GPIO_PIN_RESET);
                HAL_GPIO_WritePin(LED1B_GPIO_Port, LED1B_Pin, ((HAL_GPIO_ReadPin(LED1B_GPIO_Port, LED1B_Pin) == GPIO_PIN_SET) ? GPIO_PIN_RESET : GPIO_PIN_SET));
            }
            else if (button == 2)
            {
                HAL_GPIO_WritePin(LED2R_GPIO_Port, LED2R_Pin, GPIO_PIN_RESET);
                HAL_GPIO_WritePin(LED2G_GPIO_Port, LED2G_Pin, GPIO_PIN_RESET);
                HAL_GPIO_WritePin(LED2B_GPIO_Port, LED2B_Pin, ((HAL_GPIO_ReadPin(LED2B_GPIO_Port, LED2B_Pin) == GPIO_PIN_SET) ? GPIO_PIN_RESET : GPIO_PIN_SET));
            }

            break;
    }
}

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
