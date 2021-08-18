/**
  *****************************************************************************
  * @file    s4533087_os_cag_keypad.h
  * @author  Alexander FitzGerald - s4533087
  * @date    08052020
  * @brief   Alphanumeric keypad OS CAG driver header
  *****************************************************************************
  * EXTERNAL FUNCTIONS
  *****************************************************************************
  * s4533087_os_keypad_master()
  * s4533087_os_keypad_grid()
  * s4533087_os_keypad_mnemonic()
  *****************************************************************************
  */

#ifndef S4533087_OS_CAG_KEYPAD_H
#define S4533087_OS_CAG_KEYPAD_H

#include "s4533087_os_keypad.h"
#include "s4533087_hal_keypad.h"

#include "FreeRTOS.h"
#include "semphr.h"

#define KEY_LOG_LENGTH  15
#define MUTEX_WAIT_TIME 1000

// Mutex for the 2 main keypad tasks, ensure only one running at a time
SemaphoreHandle_t s4533087_SemaphoreKeypadMutex;

// Function prototypes
void s4533087_os_keypad_master(void);
void s4533087_os_keypad_grid(void);
void s4533087_os_keypad_mnemonic(void);

#endif
