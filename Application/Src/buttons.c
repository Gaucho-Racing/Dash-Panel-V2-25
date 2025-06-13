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

NeoPixelData globalNeoPixelData = {0};

void sendNeopixelSpi(Color color)
{
	uint8_t sendData[24];
	int indx = 0;

    for (int i = 23; i >= 0; i--)
	{
		if (((uint32_t)color >> i) & 0x1)
        {
            sendData[indx++] = 0x6;  // store 1
        }
        else
        {
            sendData[indx++] = 0x4;  // store 0
        }
	}

    //if you guys figure out how to get DMA working cause icbb.
    HAL_SPI_Transmit(&hspi1, sendData, 24, 1000);
}

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
}

void updateButtonColors(void* args)
{
    UNUSED(args);

    for (;;)
    {
        switch(globalStatus.ecuState)
        {
            case PRECHARGE_COMPLETE:
                globalNeoPixelData.RTD = COLOR_BLUE;
                break;

            case DRIVE_STANDBY:
            case DRIVE_ACTIVE_IDLE:
            case DRIVE_ACTIVE_POWER:
            case DRIVE_ACTIVE_REGEN:
                globalNeoPixelData.RTD = COLOR_GREEN;
                break;

            default:
                globalNeoPixelData.RTD = COLOR_RED;
                break;
        }

        switch(globalStatus.ecuState)
        {
            case TS_DISCHARGE_OFF:
            case ERRORSTATE:
                globalNeoPixelData.TS_Active = COLOR_RED;
                break;

            case GLV_ON:
                globalNeoPixelData.TS_Active = COLOR_BLUE;
                break;

            default:
                globalNeoPixelData.TS_Active = COLOR_GREEN;
        }

        sendNeopixelSpi(globalNeoPixelData.TS_Active);
        sendNeopixelSpi(globalNeoPixelData.RTD);

        osDelay(UPDATE_BUTTON_COLORS_DELAY);

        // Update LED colors (no good spot for em)
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

        if (globalStatus.dashStatusMsg.rtdButtonData != newValueRTD)
        {
            globalStatus.dashStatusMsg.rtdButtonData = newValueRTD;
            change = true;
        }

        bool newValueTSActive = !(bool)HAL_GPIO_ReadPin(NO_TS_ACTIVE_GPIO_Port, NO_TS_ACTIVE_Pin);  // Active low

        if (globalStatus.dashStatusMsg.tsButtonData != newValueTSActive)
        {
            globalStatus.dashStatusMsg.tsButtonData = newValueTSActive;
            change = true;
        }

        if (change)
        {
            writeMessage(MSG_DASH_STATUS, GR_ECU, (uint8_t*)&globalStatus.dashStatusMsg, 3);
        }

        osDelay(POLL_BUTTON_STATE_DELAY);
    }
}
