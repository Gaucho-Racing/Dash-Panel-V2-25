#include <stdbool.h>

#include "dash.h"

#ifndef BUTTONS_H
    #define BUTTONS_H

    #define POLL_BUTTON_STATE_DELAY 20
    #define UPDATE_BUTTON_COLORS_DELAY 100

    typedef enum {
        RTD_BUTTON = 1,
        TS_ACTIVE_BUTTON = 2,
    } ButtonNames;
    // Must be 1 or 2 for LED1 or LED2 respectively

    // Format is GRB
    typedef enum {
        COLOR_GREEN = (uint32_t)0xFF0000,
        COLOR_RED = (uint32_t)0x00FF00,
        COLOR_BLUE = (uint32_t)0x0000FF,
    } Color;

    typedef struct {
        Color TS_Active;
        Color RTD;
    } NeoPixelData;

    void pollButtonState(void* args);
    void updateButtonColors(void* args);
#endif
