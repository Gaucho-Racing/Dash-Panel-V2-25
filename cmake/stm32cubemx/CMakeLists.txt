cmake_minimum_required(VERSION 3.22)

project(stm32cubemx)
add_library(stm32cubemx INTERFACE)

# Enable CMake support for ASM and C languages
enable_language(C ASM)

target_compile_definitions(stm32cubemx INTERFACE 
	USE_HAL_DRIVER 
	STM32U5A9xx
    $<$<CONFIG:Debug>:DEBUG>
)

target_include_directories(stm32cubemx INTERFACE
    ../../Core/Inc
    ../../Drivers/STM32U5xx_HAL_Driver/Inc
    ../../Drivers/STM32U5xx_HAL_Driver/Inc/Legacy
    ../../Drivers/CMSIS/Device/ST/STM32U5xx/Include
    ../../Drivers/CMSIS/Include
)

target_sources(stm32cubemx INTERFACE
    ../../Core/Src/main.c
    ../../Core/Src/stm32u5xx_it.c
    ../../Core/Src/stm32u5xx_hal_msp.c
    ../../Drivers/STM32U5xx_HAL_Driver/Src/stm32u5xx_hal_adc.c
    ../../Drivers/STM32U5xx_HAL_Driver/Src/stm32u5xx_hal_adc_ex.c
    ../../Drivers/STM32U5xx_HAL_Driver/Src/stm32u5xx_hal_dma.c
    ../../Drivers/STM32U5xx_HAL_Driver/Src/stm32u5xx_hal_dma_ex.c
    ../../Drivers/STM32U5xx_HAL_Driver/Src/stm32u5xx_hal.c
    ../../Drivers/STM32U5xx_HAL_Driver/Src/stm32u5xx_hal_i2c.c
    ../../Drivers/STM32U5xx_HAL_Driver/Src/stm32u5xx_hal_i2c_ex.c
    ../../Drivers/STM32U5xx_HAL_Driver/Src/stm32u5xx_hal_rcc.c
    ../../Drivers/STM32U5xx_HAL_Driver/Src/stm32u5xx_hal_rcc_ex.c
    ../../Drivers/STM32U5xx_HAL_Driver/Src/stm32u5xx_hal_cortex.c
    ../../Drivers/STM32U5xx_HAL_Driver/Src/stm32u5xx_hal_flash.c
    ../../Drivers/STM32U5xx_HAL_Driver/Src/stm32u5xx_hal_flash_ex.c
    ../../Drivers/STM32U5xx_HAL_Driver/Src/stm32u5xx_hal_gpio.c
    ../../Drivers/STM32U5xx_HAL_Driver/Src/stm32u5xx_hal_exti.c
    ../../Drivers/STM32U5xx_HAL_Driver/Src/stm32u5xx_hal_pwr.c
    ../../Drivers/STM32U5xx_HAL_Driver/Src/stm32u5xx_hal_pwr_ex.c
    ../../Drivers/STM32U5xx_HAL_Driver/Src/stm32u5xx_hal_gtzc.c
    ../../Drivers/STM32U5xx_HAL_Driver/Src/stm32u5xx_hal_icache.c
    ../../Drivers/STM32U5xx_HAL_Driver/Src/stm32u5xx_hal_mdf.c
    ../../Drivers/STM32U5xx_HAL_Driver/Src/stm32u5xx_hal_cordic.c
    ../../Drivers/STM32U5xx_HAL_Driver/Src/stm32u5xx_hal_crc.c
    ../../Drivers/STM32U5xx_HAL_Driver/Src/stm32u5xx_hal_crc_ex.c
    ../../Drivers/STM32U5xx_HAL_Driver/Src/stm32u5xx_hal_dac.c
    ../../Drivers/STM32U5xx_HAL_Driver/Src/stm32u5xx_hal_dac_ex.c
    ../../Drivers/STM32U5xx_HAL_Driver/Src/stm32u5xx_hal_dcache.c
    ../../Drivers/STM32U5xx_HAL_Driver/Src/stm32u5xx_hal_dma2d.c
    ../../Drivers/STM32U5xx_HAL_Driver/Src/stm32u5xx_hal_fdcan.c
    ../../Drivers/STM32U5xx_HAL_Driver/Src/stm32u5xx_hal_gpu2d.c
    ../../Drivers/STM32U5xx_HAL_Driver/Src/stm32u5xx_hal_hash.c
    ../../Drivers/STM32U5xx_HAL_Driver/Src/stm32u5xx_hal_hash_ex.c
    ../../Drivers/STM32U5xx_HAL_Driver/Src/stm32u5xx_hal_lptim.c
    ../../Drivers/STM32U5xx_HAL_Driver/Src/stm32u5xx_hal_ltdc.c
    ../../Drivers/STM32U5xx_HAL_Driver/Src/stm32u5xx_hal_ltdc_ex.c
    ../../Drivers/STM32U5xx_HAL_Driver/Src/stm32u5xx_hal_dsi.c
    ../../Drivers/STM32U5xx_HAL_Driver/Src/stm32u5xx_hal_ospi.c
    ../../Drivers/STM32U5xx_HAL_Driver/Src/stm32u5xx_ll_dlyb.c
    ../../Drivers/STM32U5xx_HAL_Driver/Src/stm32u5xx_hal_rng.c
    ../../Drivers/STM32U5xx_HAL_Driver/Src/stm32u5xx_hal_rng_ex.c
    ../../Drivers/STM32U5xx_HAL_Driver/Src/stm32u5xx_hal_rtc.c
    ../../Drivers/STM32U5xx_HAL_Driver/Src/stm32u5xx_hal_rtc_ex.c
    ../../Drivers/STM32U5xx_HAL_Driver/Src/stm32u5xx_ll_sdmmc.c
    ../../Drivers/STM32U5xx_HAL_Driver/Src/stm32u5xx_hal_sd.c
    ../../Drivers/STM32U5xx_HAL_Driver/Src/stm32u5xx_hal_sd_ex.c
    ../../Drivers/STM32U5xx_HAL_Driver/Src/stm32u5xx_hal_mmc.c
    ../../Drivers/STM32U5xx_HAL_Driver/Src/stm32u5xx_hal_mmc_ex.c
    ../../Drivers/STM32U5xx_HAL_Driver/Src/stm32u5xx_hal_sdio.c
    ../../Drivers/STM32U5xx_HAL_Driver/Src/stm32u5xx_hal_spi.c
    ../../Drivers/STM32U5xx_HAL_Driver/Src/stm32u5xx_hal_spi_ex.c
    ../../Drivers/STM32U5xx_HAL_Driver/Src/stm32u5xx_hal_tim.c
    ../../Drivers/STM32U5xx_HAL_Driver/Src/stm32u5xx_hal_tim_ex.c
    ../../Drivers/STM32U5xx_HAL_Driver/Src/stm32u5xx_hal_uart.c
    ../../Drivers/STM32U5xx_HAL_Driver/Src/stm32u5xx_hal_uart_ex.c
    ../../Core/Src/system_stm32u5xx.c
    ../../Core/Src/sysmem.c
    ../../Core/Src/syscalls.c
    ../../startup_stm32u5a9xx.s
)

target_link_directories(stm32cubemx INTERFACE
)

target_link_libraries(stm32cubemx INTERFACE
)

# Validate that STM32CubeMX code is compatible with C standard
if(CMAKE_C_STANDARD LESS 11)
    message(ERROR "Generated code requires C11 or higher")
endif()


