/**
  *****************************************************************************
  * @file	s4533087_os_lta1000g.h
  * @author	Alexander FitzGerald - s4533087
  * @date	27042020
  * @brief	lta1000g OS for LED bar I/O
  *****************************************************************************
  * EXTERNAL FUNCTIONS
  *****************************************************************************
  * s4533087_os_lta1000g_init()
  * s4533087_os_lta1000g_toggle_bit_9()
  *****************************************************************************
  */

#include "board.h"

#ifndef S4533087_OS_LTA1000G_H
#define S4533087_OS_LTA1000G_H

/* Function prototypes ----------------------------------------------------- */
void s4533087_os_lta1000g_init(void);
void s4533087_os_lta1000g_deinit(void);
void s4533087_os_lta1000g_toggle_bit_9(void);
void s4533087_os_lta1000g_write_x(uint8_t);
void s4533087_os_lta1000g_write_y(uint8_t);

#endif
