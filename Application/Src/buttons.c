#include <stdbool.h>

#include "buttons.h"
#include "cmsis_os2.h"
#include "stm32u5xx.h"
#include "grIDs.h"
#include "fdcan.h"
#include "main.h"
#include "utils.h"
#include "dash.h"
#include "msgIDs.h"
#include "dash.h"
#include "spi.h"

// NeoPixel byte representing no brightness
const uint64_t NEOPIXEL_OFF = 0x8080808080808080;

// NeoPixel byte representing full brightness
const uint64_t NEOPIXEL_ON = 0xFCFCFCFCFCFCFCFC;

NeoPixelData globalNeoPixelData = {0};

uint64_t neopixelDataBuffer[2 * 3] = {0};
//                          ^   ^
//                          |   |
//                          |   3 bytes per NeoPixel
//                          2 NeoPixels

void colorPin(Color color, ButtonNames button)
{
    if ((button == TS_ACTIVE_BUTTON && color == globalNeoPixelData.TS_Active) || (button == RTD_BUTTON && color == globalNeoPixelData.RTD))
    {
        return;
    }

    switch(button)
    {
        case TS_ACTIVE_BUTTON:
            globalNeoPixelData.TS_Active = color;
            break;
        case RTD_BUTTON:
            globalNeoPixelData.RTD = color;
            break;
    }

    // Write new color code (GRB) see pattern at https://www.newinnovations.nl/post/controlling-ws2812-and-ws2812b-using-only-stm32-spi/#option-3-using-8-spi-bits--pulses
    switch (button)
    {
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

    // FIXME Figure out what to pull high/low/something such that we think we are always ready to transmit
    // HAL_SPI_Transmit_DMA(&hspi1, (uint8_t*)neopixelDataBuffer, 2 * 3 * 64);
    //                                                         ^   ^    ^
    //                                                         |   |    |
    //                                                         |   |    Single NeoPixel byte
    //                                                         |   3 bytes per NeoPixel
    //                                                         2 NeoPixels
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

        // Update LED colors (just cause no other real spot to put them)
        HAL_GPIO_WritePin(LED_AMS_GPIO_Port, LED_AMS_Pin, getBit(globalStatus.dashStatusMsg.ledBits, 0));
        HAL_GPIO_WritePin(LED_IMD_GPIO_Port, LED_IMD_Pin, getBit(globalStatus.dashStatusMsg.ledBits, 1));
        HAL_GPIO_WritePin(LED_BSPD_GPIO_Port, LED_BSPD_Pin, getBit(globalStatus.dashStatusMsg.ledBits, 2));
    }
}

void pollButtonState(void* args)
{
    UNUSED(args);

    for (;;)
    {
        bool change = false;

        bool newValueRTD = !(bool)HAL_GPIO_ReadPin(NO_RTD_GPIO_Port, NO_RTD_Pin);   // Active low
        globalStatus.debugMessage[0] = newValueRTD + 'a';
        globalStatus.debugMessage[1] = globalStatus.dashStatusMsg.rtdButtonData + 'a';

        if (globalStatus.dashStatusMsg.rtdButtonData != newValueRTD)
        {
            globalStatus.dashStatusMsg.rtdButtonData = newValueRTD;
            change = true;
        }

        bool newValueTSActive = !(bool)HAL_GPIO_ReadPin(NO_TS_ACTIVE_GPIO_Port, NO_TS_ACTIVE_Pin);  // Active low
        globalStatus.debugMessage[2] = newValueTSActive + 'a';
        globalStatus.debugMessage[3] = globalStatus.dashStatusMsg.tsButtonData + 'a';
        globalStatus.debugMessage[4] = '\0';

        if (globalStatus.dashStatusMsg.tsButtonData != newValueTSActive)
        {
            globalStatus.dashStatusMsg.tsButtonData = newValueTSActive;
            change = true;
        }

        #ifdef TEST_NO_CAN
        if (change)
        {
            recievedNewInformationPleaseRefresh();
        }
        #else
        UNUSED(change);
        #endif

        osDelay(POLL_BUTTON_STATE_DELAY);
    }
}
