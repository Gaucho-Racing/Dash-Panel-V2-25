// gui.h

#ifndef GUI_H
#define GUI_H

#include "lvgl/lvgl.h"
#include "dash.h"

// --- WIDGET STYLES ---
#define GR_NAVY 0x195297
#define GR_PINK 0xEF0DA1
#define GR_PRPL 0x7920FF
#define GR_GRAY 0x9AA3B0
#define TOP_HEIGHT 200
#define GRID_ROWS 2
#define GRID_COLUMNS 4
#define CELL_WIDTH_PX 20
#define CELL_HEIGHT_PX 20
#define GRID_WIDTH_PX 180
#define GRID_HEIGHT_PX 180


void styleSetup(void);
void displaySetup(void);
void topSetup(lv_obj_t * parent_obj);
void bottomSetup(lv_obj_t * parent_obj);
void updateDisplay(int voltage, int SoC, int power, int speed,
                   int cellTemp, int motorTemp, int inverterTemp,
                   const char *state, volatile char *message);
// static void ecu_update_timer_cb(lv_timer_t * timer);

#endif

