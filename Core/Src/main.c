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
#include "lvgl/lvgl.h"
#include "lvgl/demos/lv_demos.h"
#include "lvgl_port_display.h"
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
  MX_I2C4_Init();
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
  /* USER CODE BEGIN 2 */


  if (HAL_TIM_PWM_Start(&htim15, TIM_CHANNEL_1) != HAL_OK) {
    /* PWM Generation Error */
    Error_Handler();
  }

  /* reset display */
  HAL_GPIO_WritePin(LCD_DISP_RESET_GPIO_Port, LCD_DISP_RESET_Pin, GPIO_PIN_SET);

  /* initialize LVGL framework */
  lv_init();

  /* initialize display and touchscreen */
  lvgl_display_init();
  
  /* Change Active Screen's background color */
  lv_obj_set_style_bg_color(lv_screen_active(), lv_color_hex(0x003a57), LV_PART_MAIN);
  //lv_obj_set_style_text_color(lv_screen_active(), lv_color_hex(0xffffff), LV_PART_MAIN);

  // /* Create a spinner */
  // lv_obj_t * spinner = lv_spinner_create(lv_screen_active()/*, 1000, 60*/);
  // lv_obj_set_size(spinner, 64, 64);
  


  /*
  static lv_style_t style;
  lv_style_init(&style);
  lv_style_set_radius(&style, 5);

  lv_style_set_width(&style, 150);
  lv_style_set_height(&style, LV_SIZE_CONTENT);

  lv_style_set_pad_ver(&style, 20);
  lv_style_set_pad_left(&style, 5);

  lv_style_set_x(&style, lv_pct(50));
  lv_style_set_y(&style, 80);

  lv_obj_t * obj = lv_obj_create(lv_scr_act());
  lv_obj_add_style(obj, &style, 0);

  lv_obj_t * label = lv_label_create(obj);
  lv_label_set_text(label, "Hello");

  */

  /*Create a white label, set its text and align it to the center*/
  lv_obj_t * label = lv_label_create(lv_screen_active());
  lv_label_set_text(label, "Hello world");
  lv_obj_set_style_text_color(lv_screen_active(), lv_color_hex(0xffffff), LV_PART_MAIN);
  lv_obj_align(label, LV_ALIGN_CENTER, 0, 0);

  // Real code 

  /*
   * https://docs.lvgl.io/master/details/widgets/label.html - for changing label texts
   * https://forum.lvgl.io/t/backgroud-colour/2036 - for changing color
   * 
   * 
   * 
   * 
  */

  // Top Row

  // lv_obj_t * flexRow1Col1 = lv_obj_create(flexRow1);
  // lv_obj_set_layout(flexRow1Col1, LV_LAYOUT_FLEX);
  // lv_obj_set_flex_flow(flexRow1Col1, LV_FLEX_FLOW_COLUMN);
  // lv_obj_t * flexRow1Col2 = lv_obj_create(flexRow1);
  // lv_obj_set_layout(flexRow1Col2, LV_LAYOUT_FLEX);
  // lv_obj_set_flex_flow(flexRow1Col2, LV_FLEX_FLOW_COLUMN);
  // lv_obj_t * flexRow1Col3 = lv_obj_create(flexRow1);
  // lv_obj_set_layout(flexRow1Col3, LV_LAYOUT_FLEX);
  // lv_obj_set_flex_flow(flexRow1Col3, LV_FLEX_FLOW_COLUMN);

  // // Bottom Row

  // lv_obj_t * flexRow2Col1 = lv_obj_create(flexRow2);
  // lv_obj_set_layout(flexRow2Col1, LV_LAYOUT_FLEX);
  // lv_obj_set_flex_flow(flexRow2Col1, LV_FLEX_FLOW_COLUMN);
  // lv_obj_t * flexRow2Row1 = lv_obj_create(flexRow2);
  // lv_obj_set_layout(flexRow2Row1, LV_LAYOUT_FLEX);
  // lv_obj_set_flex_flow(flexRow2Row1, LV_FLEX_FLOW_ROW);

  // // Power - voltage, SOC, and total power

  // lv_obj_t * voltageWidget = lv_obj_create(flexRow1Col1);
  // lv_obj_t * socWidget = lv_obj_create(flexRow1Col1);
  // lv_obj_t * totalPowerWidget = lv_obj_create(flexRow1Col1);

  // // Main - speed, state, and warning

  // lv_obj_t * speedWidget = lv_obj_create(flexRow1Col2);
  // lv_obj_t * stateWidget = lv_obj_create(flexRow1Col2);
  // lv_obj_t * warningWidget = lv_obj_create(flexRow1Col2);

  // // Dials 

  // lv_obj_t * regenEncoder = lv_obj_create(flexRow1Col3);
  // lv_obj_t * currentEncoder = lv_obj_create(flexRow1Col3);
  // lv_obj_t * torqueMapEncoder = lv_obj_create(flexRow1Col3);


  // // Car diagram

  // lv_obj_t * flexRow2Col1Row1 = lv_obj_create(flexRow2Col1);
  // lv_obj_set_layout(flexRow2Col1Row1, LV_LAYOUT_FLEX);
  // lv_obj_set_flex_flow(flexRow2Col1Row1, LV_FLEX_FLOW_ROW);
  // lv_obj_t * flexRow2Col1Row2 = lv_obj_create(flexRow2Col1);
  // lv_obj_set_layout(flexRow2Col1Row2, LV_LAYOUT_FLEX);
  // lv_obj_set_flex_flow(flexRow2Col1Row2, LV_FLEX_FLOW_ROW);
  // lv_obj_t * flexRow2Col1Row3 = lv_obj_create(flexRow2Col1);
  // lv_obj_set_layout(flexRow2Col1Row3, LV_LAYOUT_FLEX);
  // lv_obj_set_flex_flow(flexRow2Col1Row3, LV_FLEX_FLOW_ROW);

  // lv_obj_t * wheelFLWidget = lv_obj_create(flexRow2Col1Row1);
  // lv_obj_t * wheelFRWidget = lv_obj_create(flexRow2Col1Row1);
  // lv_obj_t * wheelRLWidget = lv_obj_create(flexRow2Col1Row3);
  // lv_obj_t * wheelRRWidget = lv_obj_create(flexRow2Col1Row3);

  // lv_obj_t * carDirectionWidget = lv_obj_create(flexRow2Col1Row2);

  // // Temperature, battery, inverter, motor, water coolant, tire, brake

  // lv_obj_t * batteryTempWidget = lv_obj_create(flexRow2Row1);
  // lv_obj_t * inverterTempWidget = lv_obj_create(flexRow2Row1);
  // lv_obj_t * motorTempWidget = lv_obj_create(flexRow2Row1);
  // lv_obj_t * waterCoolantTempWidget = lv_obj_create(flexRow2Row1);
  // lv_obj_t * tireTempWidget = lv_obj_create(flexRow2Row1);
  // lv_obj_t * brakeTempWidget = lv_obj_create(flexRow2Row1);

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
