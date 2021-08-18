/**
  *****************************************************************************
  * @file	s4533087_hal_lta1000g.h
  * @author	Alexander FitzGerald - s4533087
  * @date	05032020
  * @brief	lta1000g HAL for LED bar I/O
  *****************************************************************************
  * EXTERNAL FUNCTIONS
  *****************************************************************************
  * s4533087_hal_lta1000g_init()
  * s4533087_hal_lta1000g_deinit()
  * s4533087_hal_lta1000g_write()
  * s4533087_hal_lta1000g_set_direction()
  * s4533087_hal_lta1000g_get_direction()
  * s4533087_hal_lta1000g_change_direction()
  *****************************************************************************
  */

#include "board.h"

#ifndef S4533087_HAL_LTA1000G_H
#define S4533087_HAL_LTA1000G_H

#define LED_PINS 10

/* Function prototypes ----------------------------------------------------- */
void s4533087_hal_lta1000g_init(void);
void s4533087_hal_lta1000g_deinit(void);
void s4533087_hal_lta1000g_write(unsigned short value);
unsigned short s4533087_hal_lta1000g_read(void);
void s4533087_hal_lta1000g_write_x(uint8_t);
void s4533087_hal_lta1000g_write_y(uint8_t);
void s4533087_hal_lta1000g_set_direction(signed char);
signed char s4533087_hal_lta1000g_get_direction(void);
void s4533087_hal_lta1000g_change_direction(void);
void advance_bar(void);

#endif
