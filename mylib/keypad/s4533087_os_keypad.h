/**
  *****************************************************************************
  * @file    s4533087_os_keypad.h
  * @author  Alexander FitzGerald - s4533087
  * @date    08052020
  * @brief   Alphanumeric keypad OS driver header
  *****************************************************************************
  * EXTERNAL FUNCTIONS
  *****************************************************************************
  * s4533087_os_keypad_init()
  * s4533087_os_keypad_deinit()
  * s4533087_os_keypad_fsmprocessing()
  * s4533087_os_keypad_read_status()
  * s4533087_os_keypad_read_key()
  * s4533087_os_keypad_read_ascii()
  *****************************************************************************
  */

#ifndef S4533087_OS_KEYPAD_H
#define S4533087_OS_KEYPAD_H

#include "s4533087_hal_keypad.h"

// FSM states for keypad mode
#define GRID_STATE      0
#define MNEMONIC_STATE  1

// Period for debouncing (in ms)
#define DEBOUNCE_PERIOD 40

// FSM state variable
uint8_t s4533087_keypadFsmState;

// Function prototypes
void s4533087_os_keypad_init(void);
void s4533087_os_keypad_deinit(void);
void s4533087_os_keypad_fsmprocessing(void);
int s4533087_os_keypad_read_status(void);
uint8_t s4533087_os_keypad_read_key(void);
unsigned char s4533087_os_keypad_read_ascii(void);

#endif
