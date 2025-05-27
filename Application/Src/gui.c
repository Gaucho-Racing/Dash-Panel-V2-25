#include "gui.h"
#include "lvgl/lvgl.h"
#include "stdio.h"

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

    lv_obj_t * voltage = volatileObjs.voltage;
    lv_obj_t * SoC = volatileObjs.SoC;
    lv_obj_t * power = volatileObjs.power;
    lv_obj_t * speed = volatileObjs.speed;
    lv_obj_t * state = volatileObjs.state;
    lv_obj_t * current = volatileObjs.current;
    lv_obj_t * torqueMapping = volatileObjs.torqueMapping;
    lv_obj_t * regen = volatileObjs.regen;


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
            lv_label_set_text(voltage, voltageBuffer);
            SoC = lv_label_create(boxTop1);
            lv_label_set_text(SoC, SoCBuffer);
            power = lv_label_create(boxTop1);
            lv_label_set_text(power, powerBuffer);


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
            state = lv_label_create(boxTop2);
            lv_label_set_text(state, stateBuffer);

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
                lv_label_set_text(current, "C: x A");
                torqueMapping = lv_label_create(boxTop3);
                lv_label_set_text(torqueMapping, "TM: x");
                regen = lv_label_create(boxTop3);
                lv_label_set_text(regen, "RN: x");


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

            lv_obj_update_layout(lv_screen_active());
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

            // most important temperatures to display will probably be maxCellTemp, motorTemp, inverterTemp, brakeTemp
            
            // honestly we can probably get rid of this label later down the line; driver can tell temp from celsius/fahrenheit
            // lv_obj_t * temperature_label = lv_label_create(boxBottom2);
            // lv_obj_set_flex_align(boxBottom2, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
            // lv_label_set_text(temperature_label, "Temperatures:");

                    lv_obj_t * cellTempBox = lv_obj_create(boxBottom2);
                    lv_obj_set_height(cellTempBox, 200);
                    lv_obj_set_flex_flow(cellTempBox, LV_FLEX_FLOW_COLUMN);
                    lv_obj_set_flex_align(cellTempBox, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
                    lv_obj_set_scrollbar_mode(cellTempBox, LV_SCROLLBAR_MODE_OFF);    // gets rid of scrollbars when content within a flexbox extends past box borders
                    lv_obj_set_size(cellTempBox, LV_SIZE_CONTENT, LV_SIZE_CONTENT);

                        lv_obj_t * cellTempLabel = lv_label_create(cellTempBox);
                        lv_label_set_text(cellTempLabel, "Cell: ");

                        lv_obj_t * cellTemp = lv_label_create(cellTempBox);
                        lv_label_set_text(cellTemp, "x");

                    lv_obj_t * motorTempBox = lv_obj_create(boxBottom2);
                    lv_obj_set_height(motorTempBox, 200);
                    lv_obj_set_flex_flow(motorTempBox, LV_FLEX_FLOW_COLUMN);
                    lv_obj_set_flex_align(motorTempBox, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
                    lv_obj_set_scrollbar_mode(motorTempBox, LV_SCROLLBAR_MODE_OFF);    // gets rid of scrollbars when content within a flexbox extends past box borders
                    lv_obj_set_size(motorTempBox, LV_SIZE_CONTENT, LV_SIZE_CONTENT);

                        lv_obj_t * motorTempLabel = lv_label_create(motorTempBox);
                        lv_label_set_text(motorTempLabel, "Motor: ");

                        lv_obj_t * motorTemp = lv_label_create(motorTempBox);
                        lv_label_set_text(motorTemp, "x");

                    lv_obj_t * inverterTemps = lv_obj_create(boxBottom2);
                    lv_obj_set_flex_flow(inverterTemps, LV_FLEX_FLOW_COLUMN);
                    lv_obj_set_flex_align(inverterTemps, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
                    lv_obj_set_scrollbar_mode(inverterTemps, LV_SCROLLBAR_MODE_OFF);    // gets rid of scrollbars when content within a flexbox extends past box borders
                    lv_obj_set_size(inverterTemps, LV_SIZE_CONTENT, LV_SIZE_CONTENT);

                        lv_obj_t * inverterLabel = lv_label_create(inverterTemps);
                        lv_label_set_text(inverterLabel, "Inverters");

                        lv_obj_t * inverter1 = lv_label_create(inverterTemps);
                        lv_label_set_text(inverter1, "1: ");

                        lv_obj_t * inverter2 = lv_label_create(inverterTemps);
                        lv_label_set_text(inverter2, "2: ");

                        lv_obj_t * inverter3 = lv_label_create(inverterTemps);
                        lv_label_set_text(inverter3, "3: ");
                    
                    lv_obj_t * brakeTemps = lv_obj_create(boxBottom2);
                    lv_obj_set_flex_flow(brakeTemps, LV_FLEX_FLOW_COLUMN);
                    lv_obj_set_flex_align(brakeTemps, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
                    lv_obj_set_scrollbar_mode(brakeTemps, LV_SCROLLBAR_MODE_OFF);
                    lv_obj_set_size(brakeTemps, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    
                        lv_obj_t * brakeLabel = lv_label_create(brakeTemps);
                        lv_label_set_text(brakeLabel, "Brakes");
                        
                        lv_obj_t * brake1 = lv_label_create(brakeTemps);
                        lv_label_set_text(brake1, "1: ");

                        lv_obj_t * brake2 = lv_label_create(brakeTemps);
                        lv_label_set_text(brake2, "2: ");

                        lv_obj_t * brake3 = lv_label_create(brakeTemps);
                        lv_label_set_text(brake3, "3: ");

                        lv_obj_t * brake4 = lv_label_create(brakeTemps);
                        lv_label_set_text(brake4, "4: ");

}

// --- Timer Callback Function ---
static void ecu_update_timer_cb(lv_timer_t * timer) {
    speedData++;
    if (speedData > 100) {
        speedData = 1;
    }

    // Format the string
    snprintf(speedBuffer, sizeof(speedBuffer), "Speed: %d mph", speedData);
    if (volatileObjs.speed) {
        lv_label_set_text(volatileObjs.speed, speedBuffer);
        lv_obj_invalidate(lv_screen_active()); 
    }

    snprintf(stateBuffer, sizeof(stateBuffer), "State: %s", stateData);
    if (volatileObjs.state) {
        lv_label_set_text(volatileObjs.state, stateBuffer);
        lv_obj_invalidate(lv_screen_active()); 
    }

    snprintf(voltageBuffer, sizeof(voltageBuffer), "Voltage: %d V", voltageData);
    if (volatileObjs.voltage) {
        lv_label_set_text(volatileObjs.voltage, voltageBuffer);
        lv_obj_invalidate(lv_screen_active()); 
    }

    snprintf(SoCBuffer, sizeof(SoCBuffer), "SoC: %d%%", SoCData);
    if (volatileObjs.SoC) {
        lv_label_set_text(volatileObjs.SoC, SoCBuffer);
        lv_obj_invalidate(lv_screen_active()); 
    }

    snprintf(powerBuffer, sizeof(powerBuffer), "Power: %d V", powerData);
    if (volatileObjs.power) {
        lv_label_set_text(volatileObjs.power, powerBuffer);
        lv_obj_invalidate(lv_screen_active()); 
    }
}

void createGrid(lv_obj_t * parent_obj) { // Renamed to avoid conflict if 'parent' is a global

    // --- NEW DEBUG PRINTS ---
    lv_coord_t actual_width = lv_obj_get_width(parent_obj);
    lv_coord_t actual_height = lv_obj_get_height(parent_obj);
    printf("createGrid - Actual Parent width: %d, Actual Parent height: %d\n", (int)actual_width, (int)actual_height);

    lv_coord_t pad_top = lv_obj_get_style_pad_top(parent_obj, 0);
    lv_coord_t pad_bottom = lv_obj_get_style_pad_bottom(parent_obj, 0);
    lv_coord_t pad_left = lv_obj_get_style_pad_left(parent_obj, 0);
    lv_coord_t pad_right = lv_obj_get_style_pad_right(parent_obj, 0);
    printf("createGrid - Parent padding (T,B,L,R): %d, %d, %d, %d\n", (int)pad_top, (int)pad_bottom, (int)pad_left, (int)pad_right);
    // --- END NEW DEBUG ---

    lv_coord_t content_w = lv_obj_get_content_width(parent_obj);
    lv_coord_t content_h = lv_obj_get_content_height(parent_obj);

    printf("createGrid - Parent content_w: %d, content_h: %d\n", (int)content_w, (int)content_h);

    // Calculate the width/height of each "slot" in the grid
    lv_coord_t slot_width = content_w / (GRID_COLUMNS + 2);
    lv_coord_t slot_height = content_h / GRID_ROWS;

    printf("createGrid - Slot_width: %d, slot_height: %d\n", (int)slot_width, (int)slot_height);

    for (int i = 0; i < GRID_ROWS * GRID_COLUMNS; i++) {
        lv_obj_t * tmp = lv_obj_create(parent_obj);
        if (tmp == NULL) {
            fprintf(stderr, "Failed to create grid cell %d\n", i);
            continue;
        }
        lv_obj_set_size(tmp, CELL_WIDTH_PX, CELL_HEIGHT_PX);

        int col = i % GRID_COLUMNS;
        int row = i / GRID_COLUMNS;

        // Calculate the top-left X and Y for the current slot
        lv_coord_t current_slot_x = col * slot_width;
        lv_coord_t current_slot_y = row * slot_height;

        // Position the cell. To center it within its slot:
        lv_coord_t cell_pos_x = current_slot_x + (slot_width - CELL_WIDTH_PX) / 2;
        if (col > 1) cell_pos_x += slot_width * 2;
        lv_coord_t cell_pos_y = current_slot_y + (slot_height - CELL_HEIGHT_PX) / 2;

        printf("createGrid - Cell %d: x-pos %d, y-pos: %d\n", i, cell_pos_x, cell_pos_y);

        // If you want to ensure cells don't overlap if CELL_WIDTH_PX > slot_width,
        // you might need to cap CELL_WIDTH_PX or use smaller fixed sizes.
        // For now, this centers the defined CELL_WIDTH_PX/CELL_HEIGHT_PX within the dynamic slot.

        lv_obj_set_pos(tmp, cell_pos_x, cell_pos_y);
        lv_obj_set_style_bg_color(tmp, lv_color_hex(0x00FF00), 0);
        lv_obj_clear_flag(tmp, LV_OBJ_FLAG_SCROLLABLE); // Objects are not usually scrollable by default

        // If you want circles (as your description mentioned "all the circles")
        if (CELL_WIDTH_PX == CELL_HEIGHT_PX) { // Only makes sense for squares
            lv_obj_set_style_radius(tmp, LV_RADIUS_CIRCLE, 0);
        }

        gridCells[i] = tmp;
    }
}