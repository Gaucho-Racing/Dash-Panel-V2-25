#include <stdbool.h>

#ifndef BUTTONS_H
    #define BUTTONS_H

    typedef struct {
        bool rtd;
        bool tsActive;
    } ButtonState;

    extern volatile ButtonState globalButtonState;

    void pollButtonState(void);
#endif
