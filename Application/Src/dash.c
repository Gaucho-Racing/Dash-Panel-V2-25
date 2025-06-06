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

uint16_t speedData = 1;
ECUState stateData = GLV_ON;
uint16_t voltageData = 1;
uint16_t SoCData = 1;
uint16_t powerData = 1;

uint16_t cellData = 1;
uint16_t motorData[4] = {0};
uint16_t inverterData[4] = {0};
uint16_t brakeData[4] = {0};

uint16_t currentData = 1;
uint16_t torqueMappingData = 1;
uint16_t regenData = 1;

char speedBuffer[32] = "test";
char stateBuffer[32] = "tests";
char voltageBuffer[32] = "test";
char SoCBuffer[32] = "test";
char powerBuffer[32] = "test";

char cellBuffer[32] = "test\ntest";
char motorBuffer[64] = "test\ntest\ntest\ntest";
char inverterBuffer[64] = "test\ntest\ntest\ntest\ntest";
char brakeBuffer[64] = "test\ntest\ntest\ntest\ntest";

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

void updateWheelDisp() {
    lv_layer_t layer;
    lv_canvas_init_layer(wheelDispCanvas, &layer);

    lv_draw_arc_dsc_t dsc;
    lv_draw_arc_dsc_init(&dsc);
    dsc.width = 15;
    dsc.radius = 15;
    dsc.start_angle = 0;
    dsc.end_angle = 359;

    dsc.color = lv_color_hex(GR_PRPL); // TEMP TL
    dsc.center.x = 25;
    dsc.center.y = 25;
    lv_draw_arc(&layer, &dsc);
    dsc.center.x = 60;
    dsc.color = lv_color_hex(GR_PRPL); // VOLT TL
    lv_draw_arc(&layer, &dsc);

    dsc.color = lv_color_hex(GR_PRPL); // TEMP TR
    dsc.center.x = 180-60;
    dsc.center.y = 25;
    lv_draw_arc(&layer, &dsc);
    dsc.center.x = 180-25;
    dsc.color = lv_color_hex(GR_PRPL); // VOLT TR
    lv_draw_arc(&layer, &dsc);

    dsc.color = lv_color_hex(GR_PRPL); // TEMP BL
    dsc.center.x = 25;
    dsc.center.y = 180-25;
    lv_draw_arc(&layer, &dsc);
    dsc.center.x = 60;
    dsc.color = lv_color_hex(GR_PRPL); // VOLT BL
    lv_draw_arc(&layer, &dsc);

    dsc.color = lv_color_hex(GR_PRPL); // TEMP BR
    dsc.center.x = 180-60;
    dsc.center.y = 180-25;
    lv_draw_arc(&layer, &dsc);
    dsc.center.x = 180-25;
    dsc.color = lv_color_hex(GR_PRPL); // VOLT BR
    lv_draw_arc(&layer, &dsc);

    lv_canvas_finish_layer(wheelDispCanvas, &layer);

    lv_obj_invalidate(lv_screen_active());
}

void updateDataFromCAN() {
    speedData = globalStatus.vehicleSpeed % 256;
    stateData = globalStatus.ecuState % 256;
    voltageData = globalStatus.tsVoltage % 1000;
    SoCData = globalStatus.accumulatorStateOfCharge % 256;
    //powerData = globalStatus.; // tsVoltage * dcCurrent, converted into kW

    cellData = globalStatus.maxCellTemp;

    for (int i = 0; i < 4; i++) {
        motorData[i] = globalStatus.motorTemperatures[i] % 256;
        inverterData[i] = globalStatus.inverterTemperatures[i] % 256;
        brakeData[i]= globalStatus.brakeTemps[i] % 256;
    }
    

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

    snprintf(cellBuffer, sizeof(cellBuffer), "Cell:\n%d C", cellData);
    snprintf(motorBuffer, sizeof(motorBuffer), "Motor:\n%d C\n%d C\n%d C", motorData[0], motorData[1], motorData[2]);
    snprintf(inverterBuffer, sizeof(inverterBuffer), "Inverter:\n%d C\n%d C\n%d C\n%d C", inverterData[0], inverterData[1], inverterData[2], inverterData[3]);
    snprintf(brakeBuffer, sizeof(brakeBuffer), "Brake:\n%d C\n%d C\n%d C\n%d C", brakeData[0], brakeData[1], brakeData[2], brakeData[3]);

    snprintf(currentBuffer, sizeof(currentBuffer), "C: %d A", currentData);
    snprintf(torqueMappingBuffer, sizeof(torqueMappingBuffer), "TM: %d", torqueMappingData);
    snprintf(regenBuffer, sizeof(regenBuffer), "RN: %d", regenData);

    updateWheelDisp();

    // lv_label_set_text_static(speed, speedBuffer);
    // lv_label_set_text_static(state, stateBuffer);
    // lv_label_set_text_static(voltage, voltageBuffer);
    // lv_label_set_text_static(SoC, SoCBuffer);

    // lv_label_set_text_static(cell, cellBuffer);
    // lv_label_set_text_static(motor, motorBuffer);
    // lv_label_set_text_static(inverter, inverterBuffer);
    // lv_label_set_text_static(brake, brakeBuffer);
}

void recievedNewInformationPleaseRefresh()
{
    writeMessage(MSG_DASH_STATUS, GR_ECU, (uint8_t*)&globalStatus.dashStatusMsg, 3);

    // TODO: Implement functionality to call for a refresh the screen based off of the just-updated DashInfo from CANFD
    updateDataFromCAN();
}
