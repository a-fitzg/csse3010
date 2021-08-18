/**
  *****************************************************************************
  * @file    s4533087_os_cag_joystick.h
  * @author  Alexander FitzGerald - s4533087
  * @date    16052020
  * @brief   Joystick OS CAG driver header
  *****************************************************************************
  * EXTERNAL FUNCTIONS
  *****************************************************************************
  * s4533087_os_cag_joystick()
  *****************************************************************************
  */

#ifndef S4533087_OS_CAG_JOYSTICK_H
#define S4533087_OS_CAG_JOYSTICK_H

#include "FreeRTOS.h"
#include "semphr.h"
#include "event_groups.h"
#include "queue.h"

// Function prototypes
void s4533087_os_cag_joystick(void);

#endif
