/**
  *********************************************************
  * @file       s4533087_hal_pb.c
  * @author     Alexander FitzGerald - s4533087
  * @date       06032020
  * @brief      Pushbutton HAL driver source
  *********************************************************
  * EXTERNAL FUNCTIONS
  *********************************************************
  * s4533087_hal_pb_on_init()
  * s4533087_hal_pb_on_isr()
  * s4533087_hal_pb_on_deinit()
  * s4533087_hal_pb_iss_init()
  * s4533087_hal_pb_iss_isr()
  * s4533087_hal_pb_iss_deinit()
  *********************************************************
  */

/* Includes -------------------------------------------- */
#include "board.h"
#include "processor_hal.h"
#include "debug_printf.h"
#include "s4533087_hal_pb.h"

#include "s4533087_hal_lta1000g.h"

/* Private typedef ------------------------------------- */
/* Private define -------------------------------------- */
/* Private macro --------------------------------------- */
/* Private variables ----------------------------------- */
/* Private function prototypes ------------------------- */

/**
  * @brief      Enable the on-board pushbutton
  * @param      None
  * @retval     None
  */
void s4533087_hal_pb_on_init(void) {
    /*
    GIO_InitTypeDef GPIO_InitStructure;
    
    BRD_USER_BUTTON_GPIO_CLK_ENABLE();
    
    GPIO_InitStructure.Mode = GPIO_MODE_IT_RISING;
    GPIO_InitStructure.Pull = GPIO_NOPULL;
    GPIO_InitStructure.Pin  = BRD_USER_BUTTON_PIN;
    HAL_GPIO_Init(BRD_USER_BUTTON_GPIO_PORT, &GPIO_InitStructure);
    
    HAL_NVIC_SetPriority(BRD_USER_BUTTON_EXTI_IRQn, 10, 0);
    HAL_NVIC_EnableIRQ(BRD_USER_BUTTON_EXTI_IRQn);
    */
}

/**
  * @brief      Pushbutton Interrupt Service Routine
  * @param      None
  * @retval     None
  */
void s4533087_hal_pb_on_isr(void) {
    debug_printf("CHANGING...\r\n");
    s4533087_hal_lta1000g_change_direction();
    debug_printf("changed\r\n");
}

/**
  * @brief      Disables ISS pin source
  * @param      None
  * @retval     None
  */
void s4533087_hal_pb_on_deinit(void) {

}

/**
  * @brief      Enable ISS pin source
  * @param      None
  * @retval     None
  */
void s4533087_hal_pb_iss_init(void) {

}

/**
  * @brief      ISS pin source ISR
  * @param      None
  * @retval     None
  */
void s4533087_hal_pb_iss_isr(void) {

}

/**
  * @brief      Disable ISS source
  * @param      None
  * @retval     None
  */
void s4533087_hal_pb_iss_deinit(void) {

}
