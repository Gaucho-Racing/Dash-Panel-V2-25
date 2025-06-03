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
#define NUM_VARIABLES 8 // the number of variables we need to update and keep track of on the dash 

static lv_style_t screenStyle;
static lv_style_t flexRowStyle;
static lv_style_t flexColumnStyle;

enum VOLATILE_OBJECTS {
    SPEED = 0,
    STATE = 1,
    VOLTAGE = 2,
    SOC = 3,
    POWER = 4,
    CURRENT = 5,
    TORQUE_MAPPING = 6,
    REGEN = 7,
};

typedef struct {
    lv_obj_t * variable;    // this maps to the LVGL text variable we're updating/keeping track of  
    uint16_t data;   
    char buffer[32];
} VolatileObj;

/*
FOOD FOR THOUGHT: might need to split up VolatileObj into different structs? we're being too general
for example - can't use VolatileObj.data for something like globalStatus.inverterCurrents[4] because inverter currents is an array
*/ 


// Dark magic fuckery
typedef union {
    struct {
        VolatileObj speed;
        VolatileObj state; 
        VolatileObj voltage;
        VolatileObj SoC;
        VolatileObj power;   
        VolatileObj current;
        VolatileObj torqueMapping;
        VolatileObj regen;
    };
    VolatileObj arr[8];
} VolatileObjs;

// --- Global or static variables needed in the timer callback ---
extern volatile VolatileObj volatileObjects[8];

static uint16_t speedData = 1;
static ECUState stateData = GLV_ON;
static uint16_t voltageData = 1;
static uint16_t SoCData = 1;
static uint16_t powerData = 1;

static char speedBuffer[32];  
static char stateBuffer[32];
static char voltageBuffer[32];  
static char SoCBuffer[32];  
static char powerBuffer[32];  

extern lv_obj_t * gridCells[GRID_ROWS * GRID_COLUMNS];

void styleSetup(void);
void displaySetup(void);
void topSetup(lv_obj_t * parent_obj);
void bottomSetup(lv_obj_t * parent_obj);
static void ecu_update_timer_cb(lv_timer_t * timer);
void createGrid(lv_obj_t * parent_obj);

#endif

