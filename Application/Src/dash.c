#include <stdbool.h>
#include <stdio.h>

#include "cmsis_os2.h"
#include "dash.h"
#include "msgIDs.h"
#include "grIDs.h"
#include "fdcan.h"
#include "gui.h"

/* 
GLOBAL VARIABLES (extern defined in gui.h), data/buffers are declared in dash.c
    - general flow is to have externs defined in a header file, then declare the actual variable in a .c file
    - when you need to refer to these global variabes, just include the header file in the .c file you're using them in
*/
lv_obj_t * speed = NULL;
lv_obj_t * state = NULL;  
lv_obj_t * voltage = NULL;
lv_obj_t * SoC = NULL;
lv_obj_t * power = NULL;   
lv_obj_t * current = NULL;
lv_obj_t * torqueMapping = NULL;
lv_obj_t * regen = NULL;

uint16_t speedData = 1;
ECUState stateData = GLV_ON;
uint16_t voltageData = 1;
uint16_t SoCData = 1;
uint16_t powerData = 1;
uint16_t currentData = 1;
uint16_t torqueMappingData = 1;
uint16_t regenData = 1;

char speedBuffer[32] = "test";
char stateBuffer[32] = "test";
char voltageBuffer[32] = "test";
char SoCBuffer[32] = "test";
char powerBuffer[32] = "test";
char currentBuffer[32] = "test";
char torqueMappingBuffer[32] = "test";
char regenBuffer[32] = "test";

// TODO Confirm all scaling is appropriate
volatile DashInfo globalStatus = {0};

int32_t prevRefresh = BAD_TIME_Negative1;
int32_t tempsRefreshRateMillis = 50;

// void initializeDash() {
//     for (int i=0; i < NUM_VARIABLES; i++) {
//         volatileObjects[i].data = 0;
//     }
// }

void updateDataFromCAN() {
    speedData = globalStatus.vehicleSpeed;
    stateData = globalStatus.ecuState;
    voltageData = globalStatus.tsVoltage;
    SoCData = globalStatus.accumulatorStateOfCharge;
    //powerData = globalStatus.;

    // This data is coming from the steering encoders
    currentData = globalStatus.steeringStatusMsg.currentEncoder;
    torqueMappingData = globalStatus.steeringStatusMsg.torqueMapEncoder;
    regenData = globalStatus.steeringStatusMsg.regenEncoder;

    //snprintf(speedBuffer, sizeof(speedBuffer), "Speed: %d mph", speedData);
    snprintf(speedBuffer, sizeof(speedBuffer), "Speed: %d mph", speedData);
    snprintf(stateBuffer, sizeof(stateBuffer), "State: %d", stateData);
    snprintf(voltageBuffer, sizeof(voltageBuffer), "Voltage: %d V", voltageData);
    snprintf(SoCBuffer, sizeof(SoCBuffer), "SoC: %d%%", SoCData);

    //snprintf(powerBuffer, sizeof(powerBuffer), "Power: %d V", powerData);

    snprintf(currentBuffer, sizeof(currentBuffer), "C: %d A", currentData);
    snprintf(torqueMappingBuffer, sizeof(torqueMappingBuffer), "TM: %d", torqueMappingData);
    snprintf(regenBuffer, sizeof(regenBuffer), "RN: %d", regenData);

    lv_label_set_text_static(speed, speedBuffer);
    lv_label_set_text_static(state, stateBuffer);
    lv_label_set_text_static(voltage, voltageBuffer);
    lv_label_set_text_static(SoC, SoCBuffer);
    lv_obj_invalidate(lv_screen_active());
}

void recievedNewInformationPleaseRefresh()
{
    writeMessage(MSG_DASH_STATUS, GR_ECU, (uint8_t*)&globalStatus.dashStatusMsg, 3);

    // TODO: Implement functionality to call for a refresh the screen based off of the just-updated DashInfo from CANFD
    updateDataFromCAN();
}
