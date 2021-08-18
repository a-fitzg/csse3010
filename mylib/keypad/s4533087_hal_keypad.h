/**
  *****************************************************************************
  * @file   s4533087_hal_keypad.h
  * @author Alexander FitzGerald - s4533087
  * @date   07042020
  * @brief  Alphanumeric keypad HAL driver header
  *****************************************************************************
  * EXTERNAL FUNCTIONS
  *****************************************************************************
  * void s4533087_hal_keypad_init()
  * void s4533087_hal_keypad_deinit()
  * void s4533087_hal_keypad_fsmprocessing()
  * int s4533087_hal_keypad_read_status()
  * uint8_t s4533087_hal_keypad_read_key()
  * char s4533087_hal_keypad_read_ascii()
  *****************************************************************************
  */

#ifndef S4533087_HAL_KEYPAD_H
#define S4533087_HAL_KEYPAD_H

#ifdef S4533087_MYLIB_CONFIGURE_INC
#include "s4533087_mylib_configure.h"
#endif

// Pin definitions for keypad
#ifndef S4533087_HAL_KEYPAD_COL1PIN
#define S4533087_HAL_KEYPAD_COL1PIN             BRD_D0_PIN
#endif

#ifndef S4533087_HAL_KEYPAD_COL1PINPORT
#define S4533087_HAL_KEYPAD_COL1PINPORT         BRD_D0_GPIO_PORT
#endif

#ifndef S4533087_HAL_KEYPAD_COL1PINCLK
#define S4533087_HAL_KEYPAD_COL1PINCLK          __BRD_D0_GPIO_CLK()
#endif

#ifndef S4533087_HAL_KEYPAD_COL2PIN 
#define S4533087_HAL_KEYPAD_COL2PIN             BRD_D0_PIN
#endif

#ifndef S4533087_HAL_KEYPAD_COL2PINPORT
#define S4533087_HAL_KEYPAD_COL2PINPORT         BRD_D0_GPIO_PORT
#endif

#ifndef S4533087_HAL_KEYPAD_COL2PINCLK
#define S4533087_HAL_KEYPAD_COL2PINCLK          __BRD_D0_GPIO_CLK()
#endif

#ifndef S4533087_HAL_KEYPAD_COL3PIN
#define S4533087_HAL_KEYPAD_COL3PIN             BRD_D0_PIN
#endif

#ifndef S4533087_HAL_KEYPAD_COL3PINPORT
#define S4533087_HAL_KEYPAD_COL3PINPORT         BRD_D0_GPIO_PORT
#endif

#ifndef S4533087_HAL_KEYPAD_COL3PINCLK
#define S4533087_HAL_KEYPAD_COL3PINCLK          __BRD_D0_GPIO_CLK()
#endif

#ifndef S4533087_HAL_KEYPAD_COL4PIN
#define S4533087_HAL_KEYPAD_COL4PIN             BRD_D0_PIN
#endif

#ifndef S4533087_HAL_KEYPAD_COL4PINPORT
#define S4533087_HAL_KEYPAD_COL4PINPORT         BRD_D0_GPIO_PORT
#endif

#ifndef S4533087_HAL_KEYPAD_COL4PINCLK
#define S4533087_HAL_KEYPAD_COL4PINCLK          __BRD_D0_GPIO_CLK()
#endif

#ifndef S4533087_HAL_KEYPAD_ROW1PIN
#define S4533087_HAL_KEYPAD_ROW1PIN             BRD_D0_PIN
#endif

#ifndef S4533087_HAL_KEYPAD_ROW1PINPORT
#define S4533087_HAL_KEYPAD_ROW1PINPORT         BRD_D0_GPIO_PORT
#endif

#ifndef S4533087_HAL_KEYPAD_ROW1PINCLK
#define S4533087_HAL_KEYPAD_ROW1PINCLK          __BRD_D0_GPIO_CLK()
#endif

#ifndef S4533087_HAL_KEYPAD_ROW2PIN
#define S4533087_HAL_KEYPAD_ROW2PIN             BRD_D0_PIN
#endif

#ifndef S4533087_HAL_KEYPAD_ROW2PINPORT
#define S4533087_HAL_KEYPAD_ROW2PINPORT         BRD_D0_GPIO_PORT
#endif

#ifndef S4533087_HAL_KEYPAD_ROW2PINCLK
#define S4533087_HAL_KEYPAD_ROW2PINCLK          __BRD_D0_GPIO_CLK()
#endif

#ifndef S4533087_HAL_KEYPAD_ROW3PIN
#define S4533087_HAL_KEYPAD_ROW3PIN             BRD_D0_PIN
#endif

#ifndef S4533087_HAL_KEYPAD_ROW3PINPORT
#define S4533087_HAL_KEYPAD_ROW3PINPORT         BRD_D0_GPIO_PORT
#endif

#ifndef S4533087_HAL_KEYPAD_ROW3PINCLK
#define S4533087_HAL_KEYPAD_ROW3PINCLK          __BRD_D0_GPIO_CLK()
#endif

#ifndef S4533087_HAL_KEYPAD_ROW4PIN
#define S4533087_HAL_KEYPAD_ROW4PIN             BRD_D0_PIN
#endif

#ifndef S4533087_HAL_KEYPAD_ROW4PINPORT
#define S4533087_HAL_KEYPAD_ROW4PINPORT         BRD_D0_GPIO_PORT
#endif

#ifndef S4533087_HAL_KEYPAD_ROW4PINCLK
#define S4533087_HAL_KEYPAD_ROW4PINCLK          __BRD_D0_GPIO_CLK()
#endif

// Function prototypes
void s4533087_hal_keypad_init(void);
void s4533087_hal_keypad_deinit(void);
void s4533087_hal_keypad_fsmprocessing(void);
int s4533087_hal_keypad_read_status(void);
uint8_t s4533087_hal_keypad_read_key(void);
unsigned char s4533087_hal_keypad_read_ascii(void);

#endif
