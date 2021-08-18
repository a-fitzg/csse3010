/**
  *********************************************************
  * @file       s4533087_os_pb.c
  * @author     Alexander FitzGerald - s4533087
  * @date       06032020
  * @brief      Pushbutton OS driver source
  *********************************************************
  * EXTERNAL FUNCTIONS
  *********************************************************
  * s4533087_os_pb_on_init()
  * s4533087_os_pb_on_deinit()
  * s4533087_os_pb_read_button()
  *********************************************************
  */

/* Includes -------------------------------------------- */
#include "board.h"
#include "processor_hal.h"
#include "debug_printf.h"

// Mylib includes
#include "s4533087_hal_pb.h"
#include "s4533087_os_pb.h"

// FreeRTOS includes
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"

/* Private typedef ------------------------------------- */
/* Private define -------------------------------------- */
/* Private macro --------------------------------------- */
/* Private variables ----------------------------------- */
/* Private function prototypes ------------------------- */
void EXTI15_10_IRQHandler(void);

/**
  * @brief      Enable the on-board pushbutton
  * @param      None
  * @retval     None
  */
void s4533087_os_pb_on_init(void) {
    
    s4533087_hal_pb_on_init();
}

/**
  * @brief      De-initialised on-board pushbutton GPIO
  * @param      None
  * @retval     None
  */
void s4533087_os_pb_on_deinit(void) {

    s4533087_hal_pb_on_deinit();
}

/**
  * @brief  EXTI line detection callback
  * @params GPIO_PIN: Specifies pins connected to the EXTI line
  * @retval None
  */
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin) {

    BaseType_t xHigherPriorityTaskWoken;
    
    if (GPIO_Pin == BRD_USER_BUTTON_PIN) {
        
        xHigherPriorityTaskWoken = pdFALSE;
        
        if (s4533087_SemaphoreButtonPress != NULL) {

            xSemaphoreGiveFromISR(s4533087_SemaphoreButtonPress, 
                    &xHigherPriorityTaskWoken);
        }
        
        // Perform context switching if needed
        portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
    }
}

/**
  * @brief  Override default mapping of this handler to Default_Handle
  * @params None
  * @retval None
  */
void EXTI15_10_IRQHandler(void) {

    HAL_GPIO_EXTI_IRQHandler(BRD_USER_BUTTON_PIN);
}

/**
  * @brief  Returns logical value of button press
  * @params None
  * @retval uint8_t: Current value of button
  */
uint8_t s4533087_os_pb_read_button(void) {

    return HAL_GPIO_ReadPin(BRD_USER_BUTTON_GPIO_PORT, BRD_USER_BUTTON_PIN);
}
