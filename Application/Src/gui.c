#include "gui.h"
#include "lvgl/lvgl.h"
#include "stdio.h"
#include "dash.h"
#include "main.h"

// FIXME Please setup so that we can use `ENABLE_THREE_MOTORS`

static lv_style_t infoTextStyle;
static lv_style_t infoBoxStyle;

const size_t SCREEN_WIDTH_PX = 800;
const size_t SCREEN_HEIGHT_PX = 480;

lv_obj_t *voltage_l, *SoC_l, *power_l, *speed_l, *state_l, *cell_l, *motor_l, *inverter_l, *debugMsg_l;

lv_obj_t *chargeMeter, *powerMeter;

// extern DashInfo globalStatus;

void styleSetup(void) {
    lv_style_init(&infoTextStyle);
    lv_style_set_text_color(&infoTextStyle, lv_color_white());
    lv_style_set_text_font(&infoTextStyle, &lv_font_montserrat_20);

    lv_style_init(&infoBoxStyle);
    lv_style_set_bg_color(&infoBoxStyle, lv_color_black());
    lv_style_set_bg_opa(&infoBoxStyle, 100);
    lv_style_set_pad_all(&infoBoxStyle, 20);
    lv_style_set_border_width(&infoBoxStyle, 0);
}

void displaySetup(void) {
    styleSetup();

    #ifdef USE_NICE_BACKGROUND
    LV_IMAGE_DECLARE(dashbg);
    lv_obj_set_style_bg_image_src(lv_screen_active(), &dashbg, 0);
    #else
    lv_obj_set_style_bg_color(lv_screen_active(), lv_color_black(), 0);
    #endif

    topSetup(lv_screen_active());
    bottomSetup(lv_screen_active());
}


void topSetup(lv_obj_t * parent_obj) {
    lv_obj_t * boxTop1 = lv_obj_create(parent_obj);
    lv_obj_set_flex_flow(boxTop1, LV_FLEX_COLUMN);
    lv_obj_set_style_flex_cross_place(boxTop1, LV_FLEX_ALIGN_START, 0);
    lv_obj_set_style_flex_main_place(boxTop1, LV_FLEX_ALIGN_SPACE_EVENLY, 0);
    lv_obj_set_align(boxTop1, LV_ALIGN_TOP_LEFT);
    lv_obj_set_size(boxTop1, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
    lv_obj_add_style(boxTop1, &infoBoxStyle, 0);
    lv_obj_add_style(boxTop1, &infoTextStyle, 0);

        voltage_l = lv_label_create(boxTop1);
        SoC_l = lv_label_create(boxTop1);
        power_l = lv_label_create(boxTop1);

    lv_obj_t * boxTop2 = lv_obj_create(parent_obj);
    lv_obj_set_align(boxTop2, LV_ALIGN_TOP_MID);
    lv_obj_set_size(boxTop2, 420, 420);
    lv_obj_set_style_bg_color(boxTop2, lv_color_black(), 0);
    lv_obj_set_style_bg_opa(boxTop2, 100, 0);
    lv_obj_set_style_radius(boxTop2, LV_RADIUS_CIRCLE, 0);
    lv_obj_set_style_border_width(boxTop2, 0, 0);
    lv_obj_set_style_text_color(boxTop2, lv_color_white(), 0);

        LV_FONT_DECLARE(lekton_200);
        speed_l = lv_label_create(boxTop2);
        lv_obj_center(speed_l);
        lv_obj_set_style_text_font(speed_l, &lekton_200, 0);

        lv_obj_t * mph = lv_label_create(boxTop2);
        lv_obj_set_pos(mph, 270, 240);
        lv_obj_set_style_text_font(mph, &lv_font_montserrat_20, 0);
        lv_label_set_text(mph, "mph");
    
        state_l = lv_label_create(boxTop2);
        lv_obj_align(state_l, LV_ALIGN_CENTER, 0, 100);
        lv_obj_set_style_text_align(state_l, LV_TEXT_ALIGN_CENTER, 0);
        lv_obj_set_style_text_font(state_l, &lv_font_montserrat_20, 0);

    lv_obj_t * boxTop3 = lv_obj_create(parent_obj);
    lv_obj_set_align(boxTop3, LV_ALIGN_TOP_RIGHT);
    lv_obj_set_flex_flow(boxTop3, LV_FLEX_FLOW_COLUMN);
    lv_obj_set_style_flex_main_place(boxTop3, LV_FLEX_ALIGN_SPACE_EVENLY, 0);
    lv_obj_set_style_flex_cross_place(boxTop3, LV_FLEX_ALIGN_END, 0);
    lv_obj_set_size(boxTop3, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
    lv_obj_add_style(boxTop3, &infoBoxStyle, 0);
    lv_obj_add_style(boxTop3, &infoTextStyle, 0);

        cell_l = lv_label_create(boxTop3);
        //lv_label_set_text_static(cell_l, "Cell: 30 C");

        motor_l = lv_label_create(boxTop3);
        //lv_label_set_text_static(motor_l, "Motor: 30 C");

        inverter_l = lv_label_create(boxTop3);
        //lv_label_set_text_static(inverter_l, "Inverter: 30 C");
}

void bottomSetup(lv_obj_t * parent_obj) {
    lv_obj_t * chargeBox = lv_obj_create(parent_obj);
    lv_obj_align(chargeBox, LV_ALIGN_BOTTOM_LEFT, 0, 0);
    lv_obj_add_style(chargeBox, &infoBoxStyle, 0);
    lv_obj_set_size(chargeBox, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
    
        chargeMeter = lv_bar_create(chargeBox);
        lv_obj_set_width(chargeMeter, 50);
        lv_obj_set_height(chargeMeter, 300);
        lv_obj_set_style_outline_width(chargeMeter, 3, 0);
        lv_obj_set_style_outline_opa(chargeMeter, LV_OPA_COVER, 0);
        lv_obj_set_style_outline_color(chargeMeter, lv_color_hex(GR_GRAY), 0);
        lv_obj_set_style_bg_opa(chargeMeter, LV_OPA_COVER, LV_PART_INDICATOR);
        lv_obj_set_style_bg_opa(chargeMeter, 0, LV_PART_MAIN);
        lv_obj_set_style_radius(chargeMeter, 5, LV_PART_MAIN);
        lv_obj_set_style_radius(chargeMeter, 5, LV_PART_INDICATOR);
        lv_obj_set_style_bg_color(chargeMeter, lv_color_hex(GR_PINK), LV_PART_INDICATOR);

    lv_obj_t * debugPanel = lv_obj_create(parent_obj);
    lv_obj_set_size(debugPanel, 390, 55);
    lv_obj_set_align(debugPanel, LV_ALIGN_BOTTOM_MID);
    lv_obj_add_style(debugPanel, &infoBoxStyle, 0);
    lv_obj_add_style(debugPanel, &infoTextStyle, 0);
    lv_obj_set_scrollbar_mode(debugPanel, LV_SCROLLBAR_MODE_OFF);

        debugMsg_l = lv_label_create(debugPanel);
        lv_obj_center(debugMsg_l);
        //lv_label_set_text(debugMsg_l, "Allegedly");

    lv_obj_t * powerBox = lv_obj_create(parent_obj);
    lv_obj_align(powerBox, LV_ALIGN_BOTTOM_RIGHT, 0, 0);
    lv_obj_add_style(powerBox, &infoBoxStyle, 0);
    lv_obj_set_size(powerBox, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
    
        powerMeter = lv_bar_create(powerBox);
        lv_obj_set_width(powerMeter, 50);
        lv_obj_set_height(powerMeter, 300);
        lv_obj_set_style_outline_width(powerMeter, 3, 0);
        lv_obj_set_style_outline_opa(powerMeter, LV_OPA_COVER, 0);
        lv_obj_set_style_outline_color(powerMeter, lv_color_hex(GR_GRAY), 0);
        lv_obj_set_style_bg_opa(powerMeter, LV_OPA_COVER, LV_PART_INDICATOR);
        lv_obj_set_style_bg_opa(powerMeter, 0, LV_PART_MAIN);
        lv_obj_set_style_radius(powerMeter, 5, LV_PART_MAIN);
        lv_obj_set_style_radius(powerMeter, 5, LV_PART_INDICATOR);
        lv_obj_set_style_bg_color(powerMeter, lv_color_hex(GR_PRPL), LV_PART_INDICATOR);
        lv_bar_set_range(powerMeter, 0, 80);
}

void updateDisplay(int voltage, int SoC, int power, int speed, int cellTemp, int motorTemp, int inverterTemp, const char *state, volatile char *message) {
    lv_label_set_text_fmt(voltage_l, "%d V", voltage);
    lv_label_set_text_fmt(SoC_l, "SoC: %d%%", SoC);
    lv_label_set_text_fmt(power_l, "%d kW", power);
    lv_label_set_text_fmt(speed_l, "%02d ", speed);
    lv_label_set_text_fmt(cell_l, "Cell: %d C", cellTemp);
    lv_label_set_text_fmt(motor_l, "Motor: %d C", motorTemp);
    lv_label_set_text_fmt(inverter_l, "Inverter: %d C", inverterTemp);
    lv_label_set_text(state_l, state);
    lv_label_set_text_fmt(debugMsg_l, "%s", message);

    lv_bar_set_value(chargeMeter, SoC, LV_ANIM_OFF);
    lv_bar_set_value(powerMeter, power, LV_ANIM_OFF);

    lv_obj_invalidate(lv_screen_active());
}