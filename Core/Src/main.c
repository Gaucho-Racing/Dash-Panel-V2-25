/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "cmsis_os2.h"
#include "adc.h"
#include "mdf.h"
#include "cordic.h"
#include "crc.h"
#include "dcache.h"
#include "dma2d.h"
#include "fdcan.h"
#include "flash.h"
#include "gpu2d.h"
#include "gtzc.h"
#include "hash.h"
#include "i2c.h"
#include "icache.h"
#include "ltdc.h"
#include "memorymap.h"
#include "octospi.h"
#include "rng.h"
#include "rtc.h"
#include "spi.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <string.h>

#include "lvgl/lvgl.h"
#include "lvgl/demos/lv_demos.h"
#include "lvgl_port_display.h"
#include "msgIDs.h"
#include "grIDs.h"
#include "dash.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void SystemPower_Config(void);
void MX_FREERTOS_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
#define GR_NAVY 0x195297
#define GR_PINK 0xEF0DA1
#define GR_PRPL 0x7920FF
#define GR_GRAY 0x9AA3B0
#define TOP_HEIGHT 200
#define GRID_ROWS 2
#define GRID_COLUMNS 4
#define CELL_WIDTH_PX 20
#define CELL_HEIGHT_PX 20


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

// --- Timer Callback Function ---
static void ecu_update_timer_cb(lv_timer_t * timer) {
    speedData++;
    if (speedData > 100) {
        speedData = 1;
    }

    // Format the string
    snprintf(speedBuffer, sizeof(speedBuffer), "Speed: %d mph", speedData);
    if (speed) {
        lv_label_set_text(speed, speedBuffer);
        lv_obj_invalidate(lv_screen_active()); 
    }

    snprintf(stateBuffer, sizeof(stateBuffer), "State: %s", stateData);
    if (state) {
        lv_label_set_text(state, stateBuffer);
        lv_obj_invalidate(lv_screen_active()); 
    }

    snprintf(voltageBuffer, sizeof(voltageBuffer), "Voltage: %d V", voltageData);
    if (voltage) {
        lv_label_set_text(voltage, voltageBuffer);
        lv_obj_invalidate(lv_screen_active()); 
    }

    snprintf(SoCBuffer, sizeof(SoCBuffer), "SoC: %d%%", SoCData);
    if (SoC) {
        lv_label_set_text(SoC, SoCBuffer);
        lv_obj_invalidate(lv_screen_active()); 
    }

    snprintf(powerBuffer, sizeof(powerBuffer), "Power: %d V", powerData);
    if (power) {
        lv_label_set_text(power, powerBuffer);
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
/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{

  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the System Power */
  SystemPower_Config();

  /* Configure the system clock */
  SystemClock_Config();
  /* GTZC initialisation */
  MX_GTZC_Init();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_ADC1_Init();
  MX_ADF1_Init();
  MX_CRC_Init();
  MX_DCACHE1_Init();
  MX_DCACHE2_Init();
  MX_DMA2D_Init();
  MX_FDCAN1_Init();
  MX_GPU2D_Init();
  MX_HASH_Init();
  MX_I2C1_Init();
  MX_I2C2_Init();
  MX_ICACHE_Init();
  MX_LTDC_Init();
  MX_OCTOSPI1_Init();
  MX_RNG_Init();
  MX_RTC_Init();
  MX_ADC2_Init();
  MX_CORDIC_Init();
  MX_SPI2_Init();
  MX_TIM1_Init();
  MX_TIM3_Init();
  MX_TIM5_Init();
  MX_TIM6_Init();
  MX_TIM8_Init();
  MX_TIM15_Init();
  MX_USART3_UART_Init();
  MX_USART6_UART_Init();
  MX_FLASH_Init();
  MX_SPI1_Init();
  /* USER CODE BEGIN 2 */

  if (HAL_TIM_PWM_Start(&htim15, TIM_CHANNEL_1) != HAL_OK) {
    /* PWM Generation Error */
    Error_Handler();
  }

  /* reset display */
  HAL_GPIO_WritePin(LCD_DISP_RESET_GPIO_Port, LCD_DISP_RESET_Pin, GPIO_PIN_SET);

  /* Update default debug message on boot */
  strncpy((char*)globalStatus.debugMessage, "BootN/A", 8);

  /* initialize LVGL framework */
  lv_init();

  /* initialize display and touchscreen */
  lvgl_display_init();
  
  /* Change Active Screen's background color */

  lv_obj_t * screen = lv_screen_active();
  // lv_obj_add_style(screen, &screenStyle, 0);
  // lv_obj_set_style_bg_color(screen, lv_color_hex(GR_NAVY), LV_PART_MAIN);

  // // Code for top flex row 
  // lv_obj_t * flexRowTop = lv_obj_create(screen);
  // lv_obj_add_style(flexRowTop, &flexRowStyle, 0);
  // lv_obj_set_flex_align(flexRowTop, LV_FLEX_ALIGN_SPACE_AROUND, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_SPACE_EVENLY);
  // lv_obj_set_style_bg_color(flexRowTop, lv_color_hex(GR_NAVY), 0);

  //     lv_obj_t * boxTop1 = lv_obj_create(flexRowTop);
  //     lv_obj_set_flex_flow(boxTop1, LV_FLEX_COLUMN);
  //     lv_obj_set_flex_grow(boxTop1, 2);
  //     lv_obj_set_style_flex_cross_place(boxTop1, LV_FLEX_ALIGN_CENTER, 0);
  //     lv_obj_set_style_flex_main_place(boxTop1, LV_FLEX_ALIGN_SPACE_EVENLY, 0);
  //     // lv_obj_set_size(boxTop1, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
  //     lv_obj_set_height(boxTop1, TOP_HEIGHT);
  //     lv_obj_set_style_bg_color(boxTop1, lv_color_hex(GR_GRAY), 0);
  //     lv_obj_set_style_pad_all(boxTop1, 20, 0); 

  //         voltage = lv_label_create(boxTop1);
  //         lv_label_set_text(voltage, voltageBuffer);
  //         SoC = lv_label_create(boxTop1);
  //         lv_label_set_text(SoC, SoCBuffer);
  //         power = lv_label_create(boxTop1);
  //         lv_label_set_text(power, powerBuffer);


  //     lv_obj_t * boxTop2 = lv_obj_create(flexRowTop);
  //     lv_obj_set_flex_flow(boxTop2, LV_FLEX_COLUMN);
  //     lv_obj_set_flex_grow(boxTop2, 4); // TODO: edit later for real screen
  //     lv_obj_set_content_height(boxTop2, lv_pct(100));
  //     // lv_obj_set_size(boxTop2, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
  //     lv_obj_set_height(boxTop2, TOP_HEIGHT);
  //     lv_obj_set_style_bg_color(boxTop2, lv_color_hex(GR_GRAY), 0);
  //     lv_obj_set_style_pad_all(boxTop2, 20, 0); 
  //     lv_obj_set_style_flex_cross_place(boxTop2, LV_FLEX_ALIGN_CENTER, 0);
  //     lv_obj_set_style_flex_main_place(boxTop2, LV_FLEX_ALIGN_SPACE_EVENLY, 0);
      
  //         speed = lv_label_create(boxTop2);
  //         lv_label_set_text_static(speed, speedBuffer);
  //         state = lv_label_create(boxTop2);
  //         lv_label_set_text(state, stateBuffer);

  //     lv_obj_t * boxTop3 = lv_obj_create(flexRowTop);
  //     lv_obj_set_flex_flow(boxTop3, LV_FLEX_FLOW_COLUMN);
  //     lv_obj_set_flex_grow(boxTop3, 2);
  //     lv_obj_set_style_flex_main_place(boxTop3, LV_FLEX_ALIGN_SPACE_EVENLY, 0);
  //     lv_obj_set_style_flex_cross_place(boxTop3, LV_FLEX_ALIGN_CENTER, 0);
  //     // lv_obj_set_size(boxTop3, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
  //     lv_obj_set_height(boxTop3, TOP_HEIGHT);
  //     lv_obj_set_style_bg_color(boxTop3, lv_color_hex(GR_GRAY), 0);
  //     lv_obj_set_style_pad_all(boxTop3, 20, 0); // Add some padding inside the box

  //             current = lv_label_create(boxTop3);
  //             lv_label_set_text(current, "C: x A");
  //             torqueMapping = lv_label_create(boxTop3);
  //             lv_label_set_text(torqueMapping, "TM: x");
  //             regen = lv_label_create(boxTop3);
  //             lv_label_set_text(regen, "RN: x");

  // // Code for bottom flex row 
  // lv_obj_t * flexRowBottom = lv_obj_create(screen);
  // lv_obj_add_style(flexRowBottom, &flexRowStyle, 0);
  // lv_obj_set_flex_align(flexRowBottom, LV_FLEX_ALIGN_SPACE_AROUND, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_SPACE_AROUND);
  // lv_obj_set_style_bg_color(flexRowBottom, lv_color_hex(0x195297), 0);
  // // lv_obj_set_style_pad_all(flexRowBottom, 10, 0);

  //     lv_obj_t * boxBottom1 = lv_obj_create(flexRowBottom);
  //     // lv_obj_set_size(boxBottom1, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
  //     lv_obj_set_height(boxBottom1, TOP_HEIGHT);
  //     lv_obj_set_width(boxBottom1, 200);
  //     lv_obj_set_style_bg_color(boxBottom1, lv_color_hex(GR_GRAY), 0);
  //     lv_obj_set_style_pad_all(boxBottom1, 20, 0); // Add some padding inside the box

  //         lv_obj_update_layout(lv_screen_active());
  //         createGrid(boxBottom1);


  //     lv_obj_t * boxBottom2 = lv_obj_create(flexRowBottom);
  //     lv_obj_set_flex_flow(boxBottom2, LV_FLEX_FLOW_ROW); // --> no longer need flex column since we removed the "temperatures" label
  //     lv_obj_set_flex_grow(boxBottom2, 4);
  //     // lv_obj_set_size(boxBottom2, LV_PCT(100), LV_SIZE_CONTENT);
  //     lv_obj_set_height(boxBottom2, TOP_HEIGHT);
  //     lv_obj_set_style_bg_color(boxBottom2, lv_color_hex(GR_GRAY), 0);
  //     lv_obj_set_flex_align(boxBottom2, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
  //     lv_obj_set_style_flex_cross_place(boxBottom2, LV_FLEX_ALIGN_START, 0);
  //     //lv_obj_set_style_pad_all(boxBottom2, 20, 0); // Add some padding inside the box

  //         // most important temperatures to display will probably be maxCellTemp, motorTemp, inverterTemp, brakeTemp
          
  //         // honestly we can probably get rid of this label later down the line; driver can tell temp from celsius/fahrenheit
  //         // lv_obj_t * temperature_label = lv_label_create(boxBottom2);
  //         // lv_obj_set_flex_align(boxBottom2, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
  //         // lv_label_set_text(temperature_label, "Temperatures:");

  //                 lv_obj_t * cellTempBox = lv_obj_create(boxBottom2);
  //                 lv_obj_set_height(cellTempBox, 200);
  //                 lv_obj_set_flex_flow(cellTempBox, LV_FLEX_FLOW_COLUMN);
  //                 lv_obj_set_flex_align(cellTempBox, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
  //                 lv_obj_set_scrollbar_mode(cellTempBox, LV_SCROLLBAR_MODE_OFF);    // gets rid of scrollbars when content within a flexbox extends past box borders
  //                 lv_obj_set_size(cellTempBox, LV_SIZE_CONTENT, LV_SIZE_CONTENT);

  //                     lv_obj_t * cellTempLabel = lv_label_create(cellTempBox);
  //                     lv_label_set_text(cellTempLabel, "Cell: ");

  //                     lv_obj_t * cellTemp = lv_label_create(cellTempBox);
  //                     lv_label_set_text(cellTemp, "x");

  //                 lv_obj_t * motorTempBox = lv_obj_create(boxBottom2);
  //                 lv_obj_set_height(motorTempBox, 200);
  //                 lv_obj_set_flex_flow(motorTempBox, LV_FLEX_FLOW_COLUMN);
  //                 lv_obj_set_flex_align(motorTempBox, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
  //                 lv_obj_set_scrollbar_mode(motorTempBox, LV_SCROLLBAR_MODE_OFF);    // gets rid of scrollbars when content within a flexbox extends past box borders
  //                 lv_obj_set_size(motorTempBox, LV_SIZE_CONTENT, LV_SIZE_CONTENT);

  //                     lv_obj_t * motorTempLabel = lv_label_create(motorTempBox);
  //                     lv_label_set_text(motorTempLabel, "Motor: ");

  //                     lv_obj_t * motorTemp = lv_label_create(motorTempBox);
  //                     lv_label_set_text(motorTemp, "x");

  //                 lv_obj_t * inverterTemps = lv_obj_create(boxBottom2);
  //                 lv_obj_set_flex_flow(inverterTemps, LV_FLEX_FLOW_COLUMN);
  //                 lv_obj_set_flex_align(inverterTemps, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
  //                 lv_obj_set_scrollbar_mode(inverterTemps, LV_SCROLLBAR_MODE_OFF);    // gets rid of scrollbars when content within a flexbox extends past box borders
  //                 lv_obj_set_size(inverterTemps, LV_SIZE_CONTENT, LV_SIZE_CONTENT);

  //                     lv_obj_t * inverterLabel = lv_label_create(inverterTemps);
  //                     lv_label_set_text(inverterLabel, "Inverters");

  //                     lv_obj_t * inverter1 = lv_label_create(inverterTemps);
  //                     lv_label_set_text(inverter1, "1: ");

  //                     lv_obj_t * inverter2 = lv_label_create(inverterTemps);
  //                     lv_label_set_text(inverter2, "2: ");

  //                     lv_obj_t * inverter3 = lv_label_create(inverterTemps);
  //                     lv_label_set_text(inverter3, "3: ");
                  
  //                 lv_obj_t * brakeTemps = lv_obj_create(boxBottom2);
  //                 lv_obj_set_flex_flow(brakeTemps, LV_FLEX_FLOW_COLUMN);
  //                 lv_obj_set_flex_align(brakeTemps, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
  //                 lv_obj_set_scrollbar_mode(brakeTemps, LV_SCROLLBAR_MODE_OFF);
  //                 lv_obj_set_size(brakeTemps, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                  
  //                     lv_obj_t * brakeLabel = lv_label_create(brakeTemps);
  //                     lv_label_set_text(brakeLabel, "Brakes");
                      
  //                     lv_obj_t * brake1 = lv_label_create(brakeTemps);
  //                     lv_label_set_text(brake1, "1: ");

  //                     lv_obj_t * brake2 = lv_label_create(brakeTemps);
  //                     lv_label_set_text(brake2, "2: ");

  //                     lv_obj_t * brake3 = lv_label_create(brakeTemps);
  //                     lv_label_set_text(brake3, "3: ");

  //                     lv_obj_t * brake4 = lv_label_create(brakeTemps);
  //                     lv_label_set_text(brake4, "4: ");
  


  // writeMessage(MSG_DEBUG_2_0, GR_ECU, (uint8_t*)"Welcome!", 8);
  
  /* USER CODE END 2 */

  /* Init scheduler */
  osKernelInitialize();

  /* Call init function for freertos objects (in app_freertos.c) */
  MX_FREERTOS_Init();

  /* Start scheduler */
  osKernelStart();

  /* We should never get here as control is now taken by the scheduler */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  if (HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure LSE Drive Capability
  */
  HAL_PWR_EnableBkUpAccess();
  __HAL_RCC_LSEDRIVE_CONFIG(RCC_LSEDRIVE_LOW);

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI48|RCC_OSCILLATORTYPE_HSI
                              |RCC_OSCILLATORTYPE_HSE|RCC_OSCILLATORTYPE_LSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_BYPASS;
  RCC_OscInitStruct.LSEState = RCC_LSE_ON;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSI48State = RCC_HSI48_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMBOOST = RCC_PLLMBOOST_DIV1;
  RCC_OscInitStruct.PLL.PLLM = 4;
  RCC_OscInitStruct.PLL.PLLN = 80;
  RCC_OscInitStruct.PLL.PLLP = 8;
  RCC_OscInitStruct.PLL.PLLQ = 2;
  RCC_OscInitStruct.PLL.PLLR = 2;
  RCC_OscInitStruct.PLL.PLLRGE = RCC_PLLVCIRANGE_0;
  RCC_OscInitStruct.PLL.PLLFRACN = 0;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2
                              |RCC_CLOCKTYPE_PCLK3;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB3CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_4) != HAL_OK)
  {
    Error_Handler();
  }

  /** MCO configuration
  */
  HAL_RCC_MCOConfig(RCC_MCO1, RCC_MCO1SOURCE_SYSCLK, RCC_MCODIV_1);

  /** Enables the Clock Security System
  */
  HAL_RCCEx_EnableLSECSS();
}

/**
  * @brief Power Configuration
  * @retval None
  */
static void SystemPower_Config(void)
{
  HAL_PWREx_EnableVddIO2();

  PWR_PVDTypeDef sConfigPVD = {0};

  /*
   * PVD Configuration
   */
  sConfigPVD.PVDLevel = PWR_PVDLEVEL_0;
  sConfigPVD.Mode = PWR_PVD_MODE_NORMAL;
  HAL_PWR_ConfigPVD(&sConfigPVD);

  /*
   * Enable the PVD Output
   */
  HAL_PWR_EnablePVD();

  /*
   * Disable the internal Pull-Up in Dead Battery pins of UCPD peripheral
   */
  HAL_PWREx_DisableUCPDDeadBattery();

  /*
   * Switch to SMPS regulator instead of LDO
   */
  if (HAL_PWREx_ConfigSupply(PWR_SMPS_SUPPLY) != HAL_OK)
  {
    Error_Handler();
  }
/* USER CODE BEGIN PWR */
/* USER CODE END PWR */
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  Period elapsed callback in non blocking mode
  * @note   This function is called  when TIM2 interrupt took place, inside
  * HAL_TIM_IRQHandler(). It makes a direct call to HAL_IncTick() to increment
  * a global variable "uwTick" used as application time base.
  * @param  htim : TIM handle
  * @retval None
  */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  /* USER CODE BEGIN Callback 0 */

  /* USER CODE END Callback 0 */
  if (htim->Instance == TIM2) {
    HAL_IncTick();
  }
  /* USER CODE BEGIN Callback 1 */

  /* USER CODE END Callback 1 */
}

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
