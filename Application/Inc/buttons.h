#include <stdbool.h>

#include "dash.h"

#ifndef BUTTONS_H
    #define BUTTONS_H

    #define POLL_BUTTON_STATE_DELAY 150
    #define UPDATE_BUTTON_COLORS_DELAY 200

    typedef enum {
        RTD_BUTTON = 1,
        TS_ACTIVE_BUTTON = 2,
    } ButtonNames;
    // Must be 1 or 2 for LED1 or LED2 respectively

    typedef enum {
        COLOR_RED,
        COLOR_GREEN,
        COLOR_BLUE_PULSE,
    } Color;

    void pollButtonState(void* args);
    void updateButtonColors(void* args);
#endif
