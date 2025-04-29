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
#include "spi.h"

const uint64_t NEOPIXEL_OFF = 0x8080808080808080;
const uint64_t NEOPIXEL_ON = 0xFCFCFCFCFCFCFCFC;

NeoPixelData globalNeoPixelData = {0};
uint64_t neopixelDataBuffer[6] = {0};

void colorPin(Color color, ButtonNames button)
{
    if ((button == TS_ACTIVE_BUTTON && color == globalNeoPixelData.TS_Active) || (button == RTD_BUTTON && color == globalNeoPixelData.RTD))
    {
        return;
    }

    switch(button) {
        case TS_ACTIVE_BUTTON:
            globalNeoPixelData.TS_Active = color;
            break;
        case RTD_BUTTON:
            globalNeoPixelData.RTD = color;
            break;
    }

    // Write new color code (GRB) see https://www.newinnovations.nl/post/controlling-ws2812-and-ws2812b-using-only-stm32-spi/#option-3-using-8-spi-bits--pulses for pattern
    switch (button) {
        case TS_ACTIVE_BUTTON:
            neopixelDataBuffer[0] = (color == COLOR_GREEN) ? NEOPIXEL_ON : NEOPIXEL_OFF;
            neopixelDataBuffer[1] = (color == COLOR_RED) ? NEOPIXEL_ON : NEOPIXEL_OFF;
            neopixelDataBuffer[2] = (color == COLOR_BLUE) ? NEOPIXEL_ON : NEOPIXEL_OFF;
            break;

        case RTD_BUTTON:
            neopixelDataBuffer[3] = (color == COLOR_GREEN) ? NEOPIXEL_ON : NEOPIXEL_OFF;
            neopixelDataBuffer[4] = (color == COLOR_RED) ? NEOPIXEL_ON : NEOPIXEL_OFF;
            neopixelDataBuffer[5] = (color == COLOR_BLUE) ? NEOPIXEL_ON : NEOPIXEL_OFF;
            break;
    }

    HAL_SPI_Transmit_DMA(&hspi1, (uint8_t*)neopixelDataBuffer, 6 * 64);
}

void updateButtonColors(void* args)
{
    UNUSED(args);

    for (;;)
    {
        switch(globalStatus.ecuState)
        {
            case PRECHARGE_COMPLETE:
                colorPin(COLOR_BLUE, RTD_BUTTON);
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
                colorPin(COLOR_BLUE, TS_ACTIVE_BUTTON);
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
