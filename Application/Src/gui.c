#include "gui.h"
#include "lvgl/lvgl.h"
#include "stdio.h"
#include "dash.h"

static lv_style_t screenStyle;
static lv_style_t flexRowStyle;
static lv_style_t flexColumnStyle;

const size_t SCREEN_WIDTH_PX = 800;
const size_t SCREEN_HEIGHT_PX = 480;

lv_obj_t * speed = NULL;
lv_obj_t * state = NULL;  
lv_obj_t * voltage = NULL;
lv_obj_t * SoC = NULL;
lv_obj_t * power = NULL;   

lv_obj_t * cell = NULL;
lv_obj_t * motor = NULL;
lv_obj_t * inverter = NULL;
lv_obj_t * brake = NULL;

lv_obj_t * current = NULL;
lv_obj_t * torqueMapping = NULL;
lv_obj_t * regen = NULL;

lv_obj_t * wheelDispCanvas = NULL;

LV_DRAW_BUF_DEFINE_STATIC(wheelDispBuf, GRID_WIDTH_PX, GRID_HEIGHT_PX, LV_COLOR_FORMAT_RGB565);

// volatile VolatileObjs volatileObjs = {};
lv_obj_t * gridCells[];
// extern DashInfo globalStatus;

void styleSetup(void) {
    lv_style_init(&screenStyle);
    lv_style_set_layout(&screenStyle, LV_LAYOUT_FLEX);
    lv_style_set_flex_flow(&screenStyle, LV_FLEX_FLOW_COLUMN);

    lv_style_init(&flexRowStyle);
    lv_style_set_width(&flexRowStyle, lv_pct(100)); // Make rows take full width
    lv_style_set_height(&flexRowStyle, LV_SIZE_CONTENT); // Row height based on content
    lv_style_set_flex_flow(&flexRowStyle, LV_FLEX_FLOW_ROW);
    lv_style_set_layout(&flexRowStyle, LV_LAYOUT_FLEX);
    lv_style_set_border_width(&flexRowStyle, 0);
    lv_style_set_flex_grow(&flexRowStyle, 1); // Specifically in context of columnn

    lv_style_init(&flexColumnStyle);
    lv_style_set_flex_flow(&flexColumnStyle, LV_FLEX_FLOW_COLUMN);
    lv_style_set_layout(&flexColumnStyle, LV_LAYOUT_FLEX);
}

void displaySetup(void) {
    styleSetup();

    lv_obj_t * screen = lv_screen_active();
    lv_obj_add_style(screen, &screenStyle, 0);
    lv_obj_set_style_bg_color(screen, lv_color_hex(GR_NAVY), LV_PART_MAIN);

    topSetup(screen);
    bottomSetup(screen);
}


void topSetup(lv_obj_t * parent_obj) {
    lv_obj_t * flexRowTop = lv_obj_create(parent_obj);
    lv_obj_add_style(flexRowTop, &flexRowStyle, 0);
    lv_obj_set_flex_align(flexRowTop, LV_FLEX_ALIGN_SPACE_AROUND, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_SPACE_EVENLY);
    lv_obj_set_style_bg_color(flexRowTop, lv_color_hex(GR_NAVY), 0);

        lv_obj_t * boxTop1 = lv_obj_create(flexRowTop);
        lv_obj_set_flex_flow(boxTop1, LV_FLEX_COLUMN);
        lv_obj_set_flex_grow(boxTop1, 2);
        lv_obj_set_style_flex_cross_place(boxTop1, LV_FLEX_ALIGN_CENTER, 0);
        lv_obj_set_style_flex_main_place(boxTop1, LV_FLEX_ALIGN_SPACE_EVENLY, 0);
        lv_obj_set_size(boxTop1, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
        lv_obj_set_height(boxTop1, TOP_HEIGHT);
        lv_obj_set_style_bg_color(boxTop1, lv_color_hex(GR_GRAY), 0);
        lv_obj_set_style_pad_all(boxTop1, 20, 0);

            voltage = lv_label_create(boxTop1);
            lv_label_set_text_static(voltage, voltageBuffer);
            SoC = lv_label_create(boxTop1);
            lv_label_set_text_static(SoC, SoCBuffer);
            power = lv_label_create(boxTop1);
            lv_label_set_text_static(power, powerBuffer);

        lv_obj_t * boxTop2 = lv_obj_create(flexRowTop);
        lv_obj_set_flex_flow(boxTop2, LV_FLEX_COLUMN);
        lv_obj_set_flex_grow(boxTop2, 4); // TODO: edit later for real screen
        lv_obj_set_content_height(boxTop2, lv_pct(100));
        lv_obj_set_size(boxTop2, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
        lv_obj_set_height(boxTop2, TOP_HEIGHT);
        lv_obj_set_style_bg_color(boxTop2, lv_color_hex(GR_GRAY), 0);
        lv_obj_set_style_pad_all(boxTop2, 20, 0);
        lv_obj_set_style_flex_cross_place(boxTop2, LV_FLEX_ALIGN_CENTER, 0);
        lv_obj_set_style_flex_main_place(boxTop2, LV_FLEX_ALIGN_SPACE_EVENLY, 0);

            speed = lv_label_create(boxTop2);
            lv_label_set_text_static(speed, speedBuffer);
            //lv_label_set_text(speed, speedBuffer);
            state = lv_label_create(boxTop2);
            lv_label_set_text_static(state, stateBuffer);

        lv_obj_t * boxTop3 = lv_obj_create(flexRowTop);
        lv_obj_set_flex_flow(boxTop3, LV_FLEX_FLOW_COLUMN);
        lv_obj_set_flex_grow(boxTop3, 2);
        lv_obj_set_style_flex_main_place(boxTop3, LV_FLEX_ALIGN_SPACE_EVENLY, 0);
        lv_obj_set_style_flex_cross_place(boxTop3, LV_FLEX_ALIGN_CENTER, 0);
        lv_obj_set_size(boxTop3, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
        lv_obj_set_height(boxTop3, TOP_HEIGHT);
        lv_obj_set_style_bg_color(boxTop3, lv_color_hex(GR_GRAY), 0);
        lv_obj_set_style_pad_all(boxTop3, 20, 0); // Add some padding inside the box

                current = lv_label_create(boxTop3);
                lv_obj_set_width(current, 100);
                lv_label_set_text_static(current, currentBuffer);
                torqueMapping = lv_label_create(boxTop3);
                lv_obj_set_width(torqueMapping, 100);
                lv_label_set_text_static(torqueMapping, torqueMappingBuffer);
                regen = lv_label_create(boxTop3);
                lv_obj_set_width(regen, 100);
                lv_label_set_text_static(regen, regenBuffer);
}

void bottomSetup(lv_obj_t * parent_obj) {

    // Code for bottom flex row
    lv_obj_t * flexRowBottom = lv_obj_create(parent_obj);
    lv_obj_add_style(flexRowBottom, &flexRowStyle, 0);
    lv_obj_set_flex_align(flexRowBottom, LV_FLEX_ALIGN_SPACE_AROUND, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_SPACE_AROUND);
    lv_obj_set_style_bg_color(flexRowBottom, lv_color_hex(0x195297), 0);
    lv_obj_set_style_pad_all(flexRowBottom, 10, 0);

        lv_obj_t * boxBottom1 = lv_obj_create(flexRowBottom);
        lv_obj_set_size(boxBottom1, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
        lv_obj_set_height(boxBottom1, TOP_HEIGHT);
        lv_obj_set_width(boxBottom1, 200);
        lv_obj_set_style_bg_color(boxBottom1, lv_color_hex(GR_GRAY), 0);
        lv_obj_set_style_pad_all(boxBottom1, 20, 0); // Add some padding inside the box

            // lv_obj_update_layout(lv_screen_active());
            createGrid(boxBottom1);

        lv_obj_t * boxBottom2 = lv_obj_create(flexRowBottom);
        lv_obj_set_flex_flow(boxBottom2, LV_FLEX_FLOW_ROW); // --> no longer need flex column since we removed the "temperatures" label
        lv_obj_set_flex_grow(boxBottom2, 4);
        lv_obj_set_size(boxBottom2, LV_PCT(100), LV_SIZE_CONTENT);
        lv_obj_set_height(boxBottom2, TOP_HEIGHT);
        lv_obj_set_style_bg_color(boxBottom2, lv_color_hex(GR_GRAY), 0);
        lv_obj_set_flex_align(boxBottom2, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
        lv_obj_set_style_flex_cross_place(boxBottom2, LV_FLEX_ALIGN_START, 0);
        lv_obj_set_style_pad_all(boxBottom2, 20, 0); // Add some padding inside the box

                    // lv_obj_t * cellTempBox = lv_obj_create(boxBottom2);
                    // lv_obj_set_height(cellTempBox, 200);
                    // lv_obj_set_flex_flow(cellTempBox, LV_FLEX_FLOW_COLUMN);
                    // lv_obj_set_flex_align(cellTempBox, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
                    // lv_obj_set_scrollbar_mode(cellTempBox, LV_SCROLLBAR_MODE_OFF);    // gets rid of scrollbars when content within a flexbox extends past box borders
                    // lv_obj_set_size(cellTempBox, LV_SIZE_CONTENT, LV_SIZE_CONTENT);

                        cell = lv_label_create(boxBottom2);
                        lv_label_set_text_static(cell, cellBuffer);

                        motor = lv_label_create(boxBottom2);
                        lv_label_set_text_static(motor, motorBuffer);

                        inverter = lv_label_create(boxBottom2);
                        lv_label_set_text_static(inverter, inverterBuffer);

                        brake = lv_label_create(boxBottom2);
                        lv_label_set_text_static(brake, brakeBuffer);

}

void createGrid(lv_obj_t * parent) {
    LV_DRAW_BUF_INIT_STATIC(wheelDispBuf);

    wheelDispCanvas = lv_canvas_create(parent);
    lv_obj_set_size(wheelDispCanvas, GRID_WIDTH_PX, GRID_HEIGHT_PX);
    lv_canvas_set_draw_buf(wheelDispCanvas, &wheelDispBuf);

    lv_canvas_fill_bg(wheelDispCanvas, lv_color_hex(GR_GRAY), LV_OPA_COVER);
    lv_obj_center(wheelDispCanvas);
}


// void updateVariableText(VolatileObj obj) {
//     lv_label_set_text(obj.data, obj.buffer); // TODO: change to lv_label_set_text_static if necessary
// }

/*
    * 1. Global Status -> map needed information into an array that aligns with volatileObjs
    * Define actual updateBuffer so that you take in data + VolatileObj so you update the specific buffer
    * And then updateVariableText should be called with ecu_update_timer_cb actually idk what the point of it is
    *
    * 2. Test update screen
    * 3. Implement GPIO pins check steering for implementation
    * 
    * Jason side note about general flow of data (following the flow from lvgltesting simulator):
    *   1. Update VolatileObj.data from globalStatus 
    *   2. use snprintf() to update each VolatileObj.buffer with a newly formatted string 
    *   3. Call updateVariableText() with the parameter as each VolatileObj
*/ 

// void updateBuffers() {
//     volatileObjs[SPEED].data = globalStatus.vehicleSpeed; 
//     volatileObjs[STATE].data = globalStatus.ECUState;
//     volatileObjs[VOLTAGE].data = globalStatus.tsVoltage; 
//     volatileObjs[SOC].data = globalStatus.glvStateOfCharge; 
//     volatileObjs[POWER].data = globalStatus.tsVoltage; // todo: what does POWER map to in globalStatus?
//     volatileObjs[CURRENT].data = globalStatus.tsVoltage; // todo: what does CURRENT map to in globalStatus?
//     volatileObjs[TORQUE_MAPPING].data = globalStatus.tsVoltage; // todo: what does TORQUE_MAPPING map to in globalStatus?
//     volatileObjs[REGEN].data = globalStatus.tsVoltage; // todo: what does REGEN map to in globalStatus?
//     /*
//     Note for bailey/others: I don't think we would be able assign each VolatileObj with data from globalStatus with a for-loop
//     because globalStatus has too many different variables - i think we would have to assign data manually like above?
//     lmk if you have a better solution 
//     for (int i=0; i < NUM_VARIABLES; i++) { 
//         volatileObjs[i].data = ?? 
//     }
//     */
// }

// --- Timer Callback Function ---
// static void ecu_update_timer_cb(lv_timer_t * timer) {
//     //enum VOLATILE_OBJECTS currVar = SPEED;

//     speedData++;
//     if (speedData > 100) {
//         speedData = 1;
//     }

//     // Format the string
//     snprintf(speedBuffer, sizeof(speedBuffer), "Speed: %d mph", speedData);
//     if (volatileObjs.arr[SPEED].data) {
//         //lv_label_set_text(volatileObjs.arr[SPEED].data, speedBuffer);
//         updateVariableText(volatileObjs.arr[SPEED]);
//         lv_obj_invalidate(lv_screen_active());
//     }

//     snprintf(stateBuffer, sizeof(stateBuffer), "State: %s", stateData);
//     if (volatileObjs.arr[STATE].data) {
//         lv_label_set_text(volatileObjs.arr[STATE].data, stateBuffer);
//         lv_obj_invalidate(lv_screen_active());
//     }

//     snprintf(voltageBuffer, sizeof(voltageBuffer), "Voltage: %d V", voltageData);
//     if (volatileObjs.arr[VOLTAGE].data) {
//         lv_label_set_text(volatileObjs.arr[VOLTAGE].data, voltageBuffer);
//         lv_obj_invalidate(lv_screen_active());
//     }

//     snprintf(SoCBuffer, sizeof(SoCBuffer), "SoC: %d%%", SoCData);
//     if (volatileObjs.arr[SOC].data) {
//         lv_label_set_text(volatileObjs.arr[SOC].data, SoCBuffer);
//         lv_obj_invalidate(lv_screen_active());
//     }

//     snprintf(powerBuffer, sizeof(powerBuffer), "Power: %d V", powerData);
//     if (volatileObjs.arr[POWER].data) {
//         lv_label_set_text(volatileObjs.arr[POWER].data, powerBuffer);
//         lv_obj_invalidate(lv_screen_active());
//     }
// }

// void createGrid(lv_obj_t * parent_obj) { // Renamed to avoid conflict if 'parent' is a global

//     // --- NEW DEBUG PRINTS ---
//     lv_coord_t actual_width = lv_obj_get_width(parent_obj);
//     lv_coord_t actual_height = lv_obj_get_height(parent_obj);
//     printf("createGrid - Actual Parent width: %d, Actual Parent height: %d\n", (int)actual_width, (int)actual_height);

//     lv_coord_t pad_top = lv_obj_get_style_pad_top(parent_obj, 0);
//     lv_coord_t pad_bottom = lv_obj_get_style_pad_bottom(parent_obj, 0);
//     lv_coord_t pad_left = lv_obj_get_style_pad_left(parent_obj, 0);
//     lv_coord_t pad_right = lv_obj_get_style_pad_right(parent_obj, 0);
//     printf("createGrid - Parent padding (T,B,L,R): %d, %d, %d, %d\n", (int)pad_top, (int)pad_bottom, (int)pad_left, (int)pad_right);
//     // --- END NEW DEBUG ---

//     lv_coord_t content_w = lv_obj_get_content_width(parent_obj);
//     lv_coord_t content_h = lv_obj_get_content_height(parent_obj);

//     printf("createGrid - Parent content_w: %d, content_h: %d\n", (int)content_w, (int)content_h);

//     // Calculate the width/height of each "slot" in the grid
//     lv_coord_t slot_width = content_w / (GRID_COLUMNS + 2);
//     lv_coord_t slot_height = content_h / GRID_ROWS;

//     printf("createGrid - Slot_width: %d, slot_height: %d\n", (int)slot_width, (int)slot_height);

//     for (int i = 0; i < GRID_ROWS * GRID_COLUMNS; i++) {
//         lv_obj_t * tmp = lv_obj_create(parent_obj);
//         if (tmp == NULL) {
//             fprintf(stderr, "Failed to create grid cell %d\n", i);
//             continue;
//         }
//         lv_obj_set_size(tmp, CELL_WIDTH_PX, CELL_HEIGHT_PX);

//         int col = i % GRID_COLUMNS;
//         int row = i / GRID_COLUMNS;

//         // Calculate the top-left X and Y for the current slot
//         lv_coord_t current_slot_x = col * slot_width;
//         lv_coord_t current_slot_y = row * slot_height;

//         // Position the cell. To center it within its slot:
//         lv_coord_t cell_pos_x = current_slot_x + (slot_width - CELL_WIDTH_PX) / 2;
//         if (col > 1) cell_pos_x += slot_width * 2;
//         lv_coord_t cell_pos_y = current_slot_y + (slot_height - CELL_HEIGHT_PX) / 2;

//         printf("createGrid - Cell %d: x-pos %d, y-pos: %d\n", i, cell_pos_x, cell_pos_y);

//         // If you want to ensure cells don't overlap if CELL_WIDTH_PX > slot_width,
//         // you might need to cap CELL_WIDTH_PX or use smaller fixed sizes.
//         // For now, this centers the defined CELL_WIDTH_PX/CELL_HEIGHT_PX within the dynamic slot.

//         lv_obj_set_pos(tmp, cell_pos_x, cell_pos_y);
//         lv_obj_set_style_bg_color(tmp, lv_color_hex(0x00FF00), 0);
//         lv_obj_clear_flag(tmp, LV_OBJ_FLAG_SCROLLABLE); // Objects are not usually scrollable by default

//         // If you want circles (as your description mentioned "all the circles")
//         if (CELL_WIDTH_PX == CELL_HEIGHT_PX) { // Only makes sense for squares
//             lv_obj_set_style_radius(tmp, LV_RADIUS_CIRCLE, 0);
//         }

//         gridCells[i] = tmp;
//     }
// }