/**
  *****************************************************************************
  * @file   s4533087_hal_mylib_configure.h
  * @author Alexander FitzGerald - s4533087
  * @date   14032020
  * @brief  
  *****************************************************************************
  * EXTERNAL FUNCTIONS
  *****************************************************************************
  * 
  *****************************************************************************
  */

#ifndef S4533087_HAL_MYLIB_CONFIGURE_H
#define S4533087_HAL_MYLIB_CONFIGURE_H

// Macros from atimer
#define S4533087_HAL_ATIMER_PIN             BRD_D10_PIN
#define S4533087_HAL_ATIMER_PINPORT         BRD_D10_GPIO_PORT
#define S4533087_HAL_ATIMER_PINCLK          __BRD_D10_GPIO_CLK()
#define S4533087_HAL_ATIMER_PERIOD          1000
#define S4533087_HAL_ATIMER_CLKSPEED        50000

// Macros from keypad
#define S4533087_HAL_KEYPAD_COL1PIN         BRD_D5_PIN
#define S4533087_HAL_KEYPAD_COL1PINPORT     BRD_D5_GPIO_PORT
#define S4533087_HAL_KEYPAD_COL1PINCLK      __BRD_D5_GPIO_CLK()

#define S4533087_HAL_KEYPAD_COL2PIN         BRD_D4_PIN
#define S4533087_HAL_KEYPAD_COL2PINPORT     BRD_D4_GPIO_PORT
#define S4533087_HAL_KEYPAD_COL2PINCLK      __BRD_D4_GPIO_CLK()

#define S4533087_HAL_KEYPAD_COL3PIN         BRD_D3_PIN
#define S4533087_HAL_KEYPAD_COL3PINPORT     BRD_D3_GPIO_PORT
#define S4533087_HAL_KEYPAD_COL3PINCLK      __BRD_D3_GPIO_CLK()

#define S4533087_HAL_KEYPAD_COL4PIN         BRD_D2_PIN
#define S4533087_HAL_KEYPAD_COL4PINPORT     BRD_D2_GPIO_PORT
#define S4533087_HAL_KEYPAD_COL4PINCLK      __BRD_D2_GPIO_CLK()

#define S4533087_HAL_KEYPAD_ROW1PIN         BRD_A4_PIN
#define S4533087_HAL_KEYPAD_ROW1PINPORT     BRD_A4_GPIO_PORT
#define S4533087_HAL_KEYPAD_ROW1PINCLK      __BRD_A4_GPIO_CLK()

#define S4533087_HAL_KEYPAD_ROW2PIN         BRD_D9_PIN
#define S4533087_HAL_KEYPAD_ROW2PINPORT     BRD_D9_GPIO_PORT
#define S4533087_HAL_KEYPAD_ROW2PINCLK      __BRD_D9_GPIO_CLK()

#define S4533087_HAL_KEYPAD_ROW3PIN         BRD_D8_PIN
#define S4533087_HAL_KEYPAD_ROW3PINPORT     BRD_D8_GPIO_PORT
#define S4533087_HAL_KEYPAD_ROW3PINCLK      __BRD_D8_GPIO_CLK()

#define S4533087_HAL_KEYPAD_ROW4PIN         BRD_D7_PIN
#define S4533087_HAL_KEYPAD_ROW4PINPORT     BRD_D7_GPIO_PORT
#define S4533087_HAL_KEYPAD_ROW4PINCLK      __BRD_D7_GPIO_CLK()

// Function prototypes

#endif
