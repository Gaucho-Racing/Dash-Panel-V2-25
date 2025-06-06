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
#define NUM_VARIABLES 8 // the number of variables we need to update and keep track of on the dash 

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

extern lv_obj_t * speed;
extern lv_obj_t * state; 
extern lv_obj_t * voltage;
extern lv_obj_t * SoC;
extern lv_obj_t * power;   

extern lv_obj_t * cell;
extern lv_obj_t * motor;
extern lv_obj_t * inverter;
extern lv_obj_t * brake;

extern lv_obj_t * current;
extern lv_obj_t * torqueMapping;
extern lv_obj_t * regen;

extern lv_obj_t * wheelDispCanvas; 

extern lv_obj_t * gridCells[GRID_ROWS * GRID_COLUMNS];

void styleSetup(void);
void displaySetup(void);
void topSetup(lv_obj_t * parent_obj);
void bottomSetup(lv_obj_t * parent_obj);
// static void ecu_update_timer_cb(lv_timer_t * timer);
void createGrid(lv_obj_t * parent_obj);

#endif

