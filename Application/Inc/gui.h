// gui.h

#ifndef GUI_H
#define GUI_H

#include "lvgl/lvgl.h"

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

static lv_style_t screenStyle;
static lv_style_t flexRowStyle;
static lv_style_t flexColumnStyle;

// --- Global or static variables needed in the timer callback ---
lv_obj_t * speed;
lv_obj_t * state; 
lv_obj_t * voltage;
lv_obj_t * SoC;
lv_obj_t * power;   
lv_obj_t * current;
lv_obj_t * torqueMapping;
lv_obj_t * regen;

static uint16_t speedData = 1;
static char stateData[] = "Running"; 
static uint16_t voltageData = 1;
static uint16_t SoCData = 1;
static uint16_t powerData = 1;

static char speedBuffer[32];  
static char stateBuffer[32];
static char voltageBuffer[32];  
static char SoCBuffer[32];  
static char powerBuffer[32];  

lv_obj_t * gridCells[GRID_ROWS * GRID_COLUMNS];

void styleSetup(void);
void displaySetup(void);
static void ecu_update_timer_cb(lv_timer_t * timer);
void createGrid(lv_obj_t * parent_obj);

#endif

