/**
  *****************************************************************************
  * @file   s4533087_os_joystick.h
  * @author Alexander FitzGerald - s4533087
  * @date   27042020
  * @brief  Joystick OS driver header
  *****************************************************************************
  * EXTERNAL FUNCTIONS
  *****************************************************************************
  * s4533087_os_joystick_init()
  * s4533087_os_joystick_deinit()
  * s4533087_os_joystick_read_x()
  * s4533087_os_joystick_read_y()
  * s4533087_os_joystick_read_switch_logic()
  *****************************************************************************
  */

#ifndef S4533087_OS_JOYSTICK_H
#define S4533087_OS_JOYSTICK_H

#include "FreeRTOS.h"
#include "semphr.h"

// Global variables
SemaphoreHandle_t s4533087_SemaphoreJoystickSwitchRaw;

// Function prototypes
void s4533087_os_joystick_init(void);
void s4533087_os_joystick_deinit(void);
uint16_t s4533087_os_joystick_read_x(void);
uint16_t s4533087_os_joystick_read_y(void);
uint8_t s4533087_os_joystick_read_switch_logic(void);

#endif
