/**
  *****************************************************************************
  * @file   mylib/s4533087_os_lta1000g.c
  * @author Alexander FitzGerald - s4533087
  * @date   25042020
  * @brief  lta1000g os driver
  *****************************************************************************
  * EXTERNAL FUNCTIONS
  *****************************************************************************
  * s4533087_os_lta1000g_init()
  * s4533087_os_lta1000g_deinit()
  * s4533087_os_lta1000g_toggle_bit_9()
  * s4533087_os_lta1000g_write_x()
  * s4533087_os_lta1000g_write_y()
  *****************************************************************************
  */

/* Includes ---------------------------------------------------------------- */
#include "board.h"
#include "processor_hal.h"
#include "debug_printf.h"

#include "FreeRTOS.h"
#include "task.h"

#include "s4533087_hal_lta1000g.h"
#include "s4533087_os_lta1000g.h"

/* Private typedef --------------------------------------------------------- */
/* Private define ---------------------------------------------------------- */
/* Private macro ----------------------------------------------------------- */
/* Private variables ------------------------------------------------------- */
unsigned short timerValue;

/* Private function prototypes --------------------------------------------- */

/**
  * @brief  LTA1000G Initialisation Function
  * @param  None
  * @retval None
  */
void s4533087_os_lta1000g_init() {

    s4533087_hal_lta1000g_init();
}

/**
  * @brief  LTA1000G Deinitialisation Function
  * @param  None
  * @retval None
  */
void s4533087_os_lta1000g_deinit(void) {

    s4533087_hal_lta1000g_deinit();
}


/**
  * @brief  Toggles bit 9 of the LED bar (the highest bit)
  * @params None
  * @retval None
  */
void s4533087_os_lta1000g_toggle_bit_9(void) {

    portENTER_CRITICAL();
    unsigned short currentValue = s4533087_hal_lta1000g_read();
    unsigned short newValue = currentValue ^ (1 << 9);
    s4533087_hal_lta1000g_write(newValue);
    portEXIT_CRITICAL();
}

/**
  * @brief  Writes an x value to segments 3-5
  * @params value: X value to write
  * @retval None
  */
void s4533087_os_lta1000g_write_x(uint8_t value) {
    
    portENTER_CRITICAL();
    unsigned short currentValue = s4533087_hal_lta1000g_read();

    // Take out bits 3, 4, 5
    unsigned short mask = 0xFFFF & ~((1 << 3) | (1 << 4) | (1 << 5));

    // Write the new ones in
    unsigned short newValue = (currentValue & mask) | ((value & 0x07) << 3);
    s4533087_hal_lta1000g_write(newValue);
    //s4533087_hal_lta1000g_write_x(value);
    portEXIT_CRITICAL();
}

/**
  * @brief  Writes a y value to segments 0-2
  * @params value: Y value to write
  * @retval None
  */
void s4533087_os_lta1000g_write_y(uint8_t value) {

    portENTER_CRITICAL();
    unsigned short currentValue = s4533087_hal_lta1000g_read();

    // Take out bits 0, 1, 2
    unsigned short mask = 0xFFFF & ~((1 << 0) | (1 << 1) | (1 << 2));

    // Write the new ones in
    unsigned short newValue = (currentValue & mask) | (value & 0x07);
    s4533087_hal_lta1000g_write(newValue);

    //s4533087_hal_lta1000g_write_y(value);
    portEXIT_CRITICAL();
}
