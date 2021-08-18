/**
  *****************************************************************************
  * @file    s4533087_os_joystick.c
  * @author  Alexander FitzGerald - s4533087
  * @date    24042020
  * @brief   Joystick FreeRTOS driver
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

/* Includes ---------------------------------------------------------------- */
#include <math.h>

#include "board.h"
#include "processor_hal.h"
#include "debug_printf.h"

// Mylib includes
#include "s4533087_os_joystick.h"
#include "s4533087_hal_joystick.h"

// FreeRTOS libraries
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"

/* Private typedef --------------------------------------------------------- */
/* Private define ---------------------------------------------------------- */
/* Private macro ----------------------------------------------------------- */
/* Private variables ------------------------------------------------------- */
/* Private function prototypes --------------------------------------------- */
uint16_t map_value(uint16_t);

/**
  * @brief  Initialise GPIO pins and ADC
  * @params None
  * @retval None
  */
void s4533087_os_joystick_init(void) {

    s4533087_hal_joystick_init();
}

/**
  * @brief  Deinitialise GPIO pins and ADC
  * @params None
  * @retval None
  */
void s4533087_os_joystick_deinit(void) {

    s4533087_hal_joystick_deinit();
}


#ifdef JOYSTICK_INTERRUPT
/**
  * @brief  EXTI line detection callback.
  * @params GPIO_Pin: Specifies pins connected EXTI line
  * @retval None
  */
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin) {

    BaseType_t xHigherPriorityTaskWoken;
    
    if (GPIO_Pin == BRD_A3_PIN) {
        
        xHigherPriorityTaskWoken = pdFALSE;
        
        if (s4533087_SemaphoreJoystickSwitchRaw != NULL) {
            
            xSemaphoreGiveFromISR(s4533087_SemaphoreJoystickSwitchRaw, 
                    &xHigherPriorityTaskWoken);
        }

        // Perform context switching if required
        portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
    }
}

/**
  * @brief  Override default mapping of this hander to Default_Handler.
  * @params None
  * @retval None
  */
void EXTI3_IRQHandler(void) {

    HAL_GPIO_EXTI_IRQHandler(BRD_A3_PIN);

}
#endif


/**
  * @brief  Returns logical value of switch press
  * @params None
  * @retval uint8_t: value of switch (inverted from active low)
  */
uint8_t s4533087_os_joystick_read_switch_logic(void) {

    return !(HAL_GPIO_ReadPin(BRD_A3_GPIO_PORT, BRD_A3_PIN));
}


/**
  * @brief  Returns the X axis value of the joystick
  * @params None
  * @retval uint16_t: value of the joystick X axis
  */
uint16_t s4533087_os_joystick_read_x(void) {
    
    uint16_t xValue;
    HAL_ADC_Start(&s4533087_AdcXHandle);
    while (HAL_ADC_PollForConversion(&s4533087_AdcXHandle, 10) != HAL_OK) {}
    xValue = (uint16_t)(HAL_ADC_GetValue(&s4533087_AdcXHandle));

    return (xValue);
}


/**
  * @brief  Returns the Y axis value of the joystick
  * @params None
  * @retval uint16_t: value of the joystick Y axis
  */
uint16_t s4533087_os_joystick_read_y(void) {

    uint16_t yValue;
    HAL_ADC_Start(&s4533087_AdcYHandle);
    while (HAL_ADC_PollForConversion(&s4533087_AdcYHandle, 10) != HAL_OK) {}
    yValue = (uint16_t)(HAL_ADC_GetValue(&s4533087_AdcYHandle));
    
    return (yValue);
}

/**
  * @brief  Maps joystick range more linearly
  * @params value: Value before mapping
  * @retval uint16_t: Value after mapping
  */
uint16_t map_value(uint16_t value) {

    // The values you get from this are very non-linear, and are very difficult
    // to accurately choose values. To rectify this, I have devised a function
    // to try and map the joystick's 0-4096 travel more linearly

    // A function that does this well is the following polymonial:
    // y = (1 / 2048^2) * (x - 2048)^3 + 2048
    // This is a cubic function, with a point of inflection at (2048, 2048)
    // Here is a rough recreation of what it looks like
    // 4096
    // |                          /
    // |                         /
    // |                        /
    // |                      /
    // |             2048  /   
    // |         -----+----
    // |       / 
    // |     /  
    // |   /   
    // |  /    
    // |___________________________ 4096
    // 0

    double mapped = pow(2048, -2) * pow((double)value - 2048, 3) + 2048;

    return (uint16_t)mapped;
}
