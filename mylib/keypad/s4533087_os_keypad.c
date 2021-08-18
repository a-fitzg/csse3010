/**
  *****************************************************************************
  * @file    s4533087_os_keypad.c
  * @author  Alexander FitzGerald - s4533087
  * @date    08052020
  * @brief   Alphanumeric keypad OS driver
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

/* Includes ---------------------------------------------------------------- */
#include "board.h"
#include "processor_hal.h"
#include "debug_printf.h"
#include "s4533087_os_keypad.h"

/* Private typedef --------------------------------------------------------- */
/* Private define ---------------------------------------------------------- */
/* Private macro ----------------------------------------------------------- */
/* Private variables ------------------------------------------------------- */
/* Private function prototypes --------------------------------------------- */

/**
  * @brief  Initialise keypad
  * @params None
  * @retval None
  */
void s4533087_os_keypad_init(void) {

    // We choose to start in grid mode
    s4533087_keypadFsmState = GRID_STATE;
    s4533087_hal_keypad_init();
}

/**
  * @brief  De-initialise keypad
  * @params None
  * @retval None
  */
void s4533087_os_keypad_deinit(void) {

    s4533087_hal_keypad_deinit();
}

/**
  * @brief  Keypad FSM processing loop. To be called repeatedly (in main loop)
  * @params None
  * @retval None
  */
void s4533087_os_keypad_fsmprocessing(void) {

    s4533087_hal_keypad_fsmprocessing();
}

/**
  * @brief  Read the KeypadStatus variable
  * @params None
  * @retval KeypadStatus: current status of keypad
  */
int s4533087_os_keypad_read_status(void) {

    return s4533087_hal_keypad_read_status();
}

/**
  * @brief  Returns current hex value of the keypad
  * @params None
  * @retval Hex value of keypad (mask)
  */
uint8_t s4533087_os_keypad_read_key(void) {

    return s4533087_hal_keypad_read_key();
}

/**
  * @brief  Returns ASCII value of keypad
  * @params None
  * @retval Current ASCII value of the keypad
  */
unsigned char s4533087_os_keypad_read_ascii(void) {

    return s4533087_hal_keypad_read_ascii();
}
