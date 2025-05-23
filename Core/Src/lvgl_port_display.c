/*********************
 *      INCLUDES
 *********************/

#include "lvgl_port_display.h"
#include "main.h"
#include "ltdc.h"
#include "dma2d.h"

/**********************
 *  STATIC PROTOTYPES
 **********************/

static void disp_flush(lv_display_t *, const lv_area_t *, uint8_t *color_p);

/**********************
 *  STATIC VARIABLES
 **********************/

lv_display_t *display;

volatile uint16_t buf_1[MY_DISP_HOR_RES * MY_DISP_VER_RES];

/**********************
 *   GLOBAL FUNCTIONS
 **********************/

void lvgl_display_init(void)
{
  /* display initialization */
  ; /* display is already initialized by cubemx-generated code */

  /* display buffer initialization */
  // lv_disp_draw_buf_init (&disp_buf,
  //                        (void*) buf_1,
  //                        NULL,
  //                        MY_DISP_HOR_RES * MY_DISP_VER_RES);

  /* register the display in LVGL */
  // lv_disp_drv_init(&disp_drv);
  display = lv_display_create(MY_DISP_HOR_RES, MY_DISP_VER_RES);
  lv_display_set_flush_cb(display, disp_flush);
  lv_display_set_buffers(display, (void*) &buf_1, NULL, sizeof(buf_1), LV_DISPLAY_RENDER_MODE_PARTIAL);

  HAL_LTDC_SetAddress(&hltdc, (uint32_t) &buf_1, LTDC_LAYER_1);

  /* set the resolution of the display */
  // disp_drv.hor_res = MY_DISP_HOR_RES;
  // disp_drv.ver_res = MY_DISP_VER_RES;

  /* set callback for display driver */
  // disp_drv.flush_cb = disp_flush;
  // disp_drv.full_refresh = 0;
  // disp_drv.direct_mode = 1;

  /* set a display buffer */
  // disp_drv.draw_buf = &disp_buf;

  /* finally register the driver */
  // lv_disp_drv_register(&disp_drv);
}

/**********************
 *   STATIC FUNCTIONS
 **********************/

static void disp_flush(lv_display_t *drv, const lv_area_t *area, uint8_t *color_p)
{
  UNUSED(drv);
  UNUSED(area);
  UNUSED(color_p);
  lv_display_flush_ready(display);
} 