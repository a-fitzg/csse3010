/**
  *****************************************************************************
  * @file   mylib/s4533087_hal_lta1000g.c
  * @author Alexander FitzGerald - s4533087
  * @date   04032020
  * @brief  lta1000g hal driver
  *****************************************************************************
  * EXTERNAL FUNCTIONS
  *****************************************************************************
  * s4533087_hal_lta1000g_init()
  * s4533087_hal_lta1000g_deinit()
  * s4533087_hal_lta1000g_write()
  * s4533087_hal_lta1000g_read()
  * s4533087_hal_lta1000g_write_x()
  * s4533087_hal_lta1000g_write_y()
  * s4533087_hal_lta1000g_set_direction()
  * s4533087_hal_lta1000g_get_direction()
  * s4533087_hal_lta1000g_change_direction()
  *****************************************************************************
  */

/* Includes ---------------------------------------------------------------- */
#include "board.h"
#include "processor_hal.h"
#include "debug_printf.h"

#include "s4533087_hal_lta1000g.h"

/* Private typedef --------------------------------------------------------- */
/* Private define ---------------------------------------------------------- */
/* Private macro ----------------------------------------------------------- */
/* Private variables ------------------------------------------------------- */
volatile signed char direction;
unsigned short ledbarValue;
unsigned short position;

// Names of the pins used, in an array to be indexed
uint16_t ledbarPins[LED_PINS] = {
        BRD_D16_PIN,
        BRD_D17_PIN,
        BRD_D18_PIN,    
        BRD_D19_PIN,
        BRD_D20_PIN,
        BRD_D21_PIN,
        BRD_D22_PIN,
        BRD_D23_PIN,
        BRD_D24_PIN,
        BRD_D25_PIN
};

// Names of the GPIO ports used, to be indexed
GPIO_TypeDef* ledbarPorts[LED_PINS] = {
        BRD_D16_GPIO_PORT,
        BRD_D17_GPIO_PORT,
        BRD_D18_GPIO_PORT,
        BRD_D19_GPIO_PORT,
        BRD_D20_GPIO_PORT,
        BRD_D21_GPIO_PORT,
        BRD_D22_GPIO_PORT,
        BRD_D23_GPIO_PORT,
        BRD_D24_GPIO_PORT,
        BRD_D25_GPIO_PORT
};

/* Private function prototypes --------------------------------------------- */
void lta1000g_seg_set(int segment, unsigned char segment_value);

/**
  * @brief      LTA1000G Initialisation Function
  * @param      None
  * @retval     None
  */
void s4533087_hal_lta1000g_init() {

    GPIO_InitTypeDef GPIO_InitStructure;

    __BRD_D16_GPIO_CLK();
    __BRD_D17_GPIO_CLK();
    __BRD_D18_GPIO_CLK();
    __BRD_D19_GPIO_CLK();
    __BRD_D20_GPIO_CLK();
    __BRD_D21_GPIO_CLK();
    __BRD_D22_GPIO_CLK();
    __BRD_D23_GPIO_CLK();
    __BRD_D24_GPIO_CLK();
    __BRD_D25_GPIO_CLK();

    for (int i = 0; i < LED_PINS; i++) {
        GPIO_InitStructure.Pin      = ledbarPins[i];
        GPIO_InitStructure.Mode     = GPIO_MODE_OUTPUT_PP;
        GPIO_InitStructure.Pull     = GPIO_PULLDOWN;
        GPIO_InitStructure.Speed    = GPIO_SPEED_FAST;
        HAL_GPIO_Init(ledbarPorts[i], &GPIO_InitStructure);
    }
}

/**
  * @brief      LTA1000G Deinitialisation Function
  * @params     None
  * @retval     None
  */
void s4533087_hal_lta1000g_deinit(void) {

    for (uint8_t i = 0; i < LED_PINS; i++) {

        HAL_GPIO_DeInit(ledbarPorts[i], ledbarPins[i]);
    }
}


/**
  * @brief      Sets value (0 or 1) of the corresponding
  * @param      Segment number, segment value (0 or 1)
  * @retval     None
  */
void lta1000g_seg_set(int segment, unsigned char segment_value) {

    HAL_GPIO_WritePin(ledbarPorts[segment], ledbarPins[segment], 
            segment_value & 0x01);
}

/**
  * @brief      Write specified value (in binary) to the LED bar
  * @param      Value to display on LED bar
  * @retval     None
  */
void s4533087_hal_lta1000g_write(unsigned short value) {

    ledbarValue = value;
    unsigned short currentDigit = 0;
    unsigned short currentValue = 0;   
    for (int i = 0; i < LED_PINS; i++) {
        // Get the 'currentDigit'th value of 'value'
        currentValue = (value >> currentDigit) & 0x01;
        lta1000g_seg_set(currentDigit, currentValue);
        currentDigit++;
    }
}

/**
  * @brief      Read the current ledbar value
  * @params     None
  * @retval     unsigned short: current value of ledbar
  */
unsigned short s4533087_hal_lta1000g_read(void) {

    return ledbarValue;
}

/**
  * @brief      Write an x value to segments 3-5
  * @params     value: X value to write
  * @retval     None
  */
void s4533087_hal_lta1000g_write_x(uint8_t value) {

    lta1000g_seg_set(3, (unsigned char)(!!(value & 0x01)));
    lta1000g_seg_set(4, (unsigned char)(!!(value & 0x02)));
    lta1000g_seg_set(5, (unsigned char)(!!(value & 0x04)));
}

/**
  * @brief      Write a y value to segments 0-2
  * @params     value: Y value to write
  * @retval     None
  */
void s4533087_hal_lta1000g_write_y(uint8_t value) {

    lta1000g_seg_set(0, (unsigned char)(!!(value & 0x01)));
    lta1000g_seg_set(1, (unsigned char)(!!(value & 0x02)));
    lta1000g_seg_set(2, (unsigned char)(!!(value & 0x04)));
}


/**
  * @brief      Sets the direction of movement for the MPD
  * @param      dir: The new direction
  * @retval     None
  */
void s4533087_hal_lta1000g_set_direction(signed char dir) {

    direction = dir;
}

/**
  * @brief      Returns the direction of movement for the MPD
  * @param      None
  * @retval     signed char: The direction of movement for the MPD
  */
signed char s4533087_hal_lta1000g_get_direction(void) {

    return direction;
}

/**
  * @brief      Toggles the direction of movement for the MPD
  * @param      None
  * @retval     None
  */
void s4533087_hal_lta1000g_change_direction(void) {

    direction = -direction;
}
