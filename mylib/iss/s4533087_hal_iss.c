/**
  *********************************************************
  * @file       s4533087_hal_iss.c
  * @author     Alexander FitzGerald - s4533087
  * @date       06032020
  * @brief      Input Signal Synchroniser HAL driver source
  *********************************************************
  * EXTERNAL FUNCTIONS
  *********************************************************
  * s4533087_hal_iss_init()
  * s4533087_hal_iss_synchroniser()
  * s4533087_hal_iss_eventcounter_read()
  * s4533087_hal_iss_lasttimer_read()
  * s4533087_hal_iss_eventcounter_reset()
  * s4533087_hal_iss_lasttimer_reset()
  * s4533087_hal_iss_delaytimer_ctrl()
  *********************************************************
  */

/* Includes -------------------------------------------- */
#include "board.h"
#include "processor_hal.h"
#include "debug_printf.h"
#include "s4533087_hal_iss.h"
#include "s4533087_hal_pb.h"

/* Private typedef ------------------------------------- */
/* Private define -------------------------------------- */
/* Private macro --------------------------------------- */
/* Private variables ----------------------------------- */

// Array to hold three significant event counter values
uint32_t iss_eventcounter_val[SIGNAL_SOURCES];

// Array to hold last timer value for each source
uint32_t iss_lasttimer_val[SIGNAL_SOURCES];

// Array to hold delay value for each source's synchroniser
uint32_t iss_delay_val[SIGNAL_SOURCES];

/* Private function prototypes ------------------------- */
void init_xenon(void);

/**
  * @brief      Initialise the ISS counters
  * @param      None
  * @retval     None
  */
void s4533087_hal_iss_init(void) {
    
    for(int i = 0; i < SIGNAL_SOURCES; i++) {
        iss_lasttimer_val[i] = 0;
    }
    for(int i = 0; i < SIGNAL_SOURCES; i++) {
        iss_delay_val[i] = 10;
    }

    init_xenon();

}

/**
  * @brief      Initialises the xenon interrupt
  * @param      None
  * @retval     None
  */
void init_xenon(void) {

    GPIO_InitTypeDef GPIO_InitStructure;

    //BRD_USER_BUTTON_GPIO_CLK_ENABLE();
    __BRD_A0_GPIO_CLK();

    GPIO_InitStructure.Mode = GPIO_MODE_IT_RISING;
    GPIO_InitStructure.Pull = GPIO_NOPULL;
    GPIO_InitStructure.Pin  = BRD_A0_PIN;
    HAL_GPIO_Init(BRD_A0_GPIO_PORT, &GPIO_InitStructure);
    
    HAL_NVIC_SetPriority(BRD_A0_EXTI_IRQ, 10, 0);
    HAL_NVIC_EnableIRQ(BRD_A0_EXTI_IRQ);
}

/**
  * @brief      Synchronise the input signal
  * @param      signal_source_index: source of the signal
  * @retval     None
  */
void s4533087_hal_iss_synchroniser(unsigned char signal_source_index) {
    
}

/**
  * @brief      Returns specified event counter value
  * @param      signal_source_index: source of the signal
  * @retval     Event counter value
  */
uint32_t s4533087_hal_iss_eventcounter_read(
        unsigned char signal_source_index) {

    return iss_eventcounter_val[signal_source_index];
}


/**
  * @brief      Returns specified last timer value
  * @param      signal_source_index: source of the signal
  * @retval     Last time value
  */
uint32_t s4533087_hal_iss_lasttimer_read(unsigned char signal_source_index) {

    return iss_lasttimer_val[signal_source_index];
}

/**
  * @brief      Resets specified event counter value
  * @param      signal_source_index: source of the signal
  r* @retval     None
  */
void s4533087_hal_iss_eventcounter_reset(unsigned char signal_source_index) {
    iss_eventcounter_val[signal_source_index] = (uint32_t)0;
}

/**
  * @brief      Resets last specified time value
  * @param      signal_source_index: source of the signal
  * @retval     None
  */
void s4533087_hal_iss_lasttimer_reset(unsigned char signal_source_index) {

    iss_lasttimer_val[signal_source_index] = (uint32_t)0;
}

/**
  * @brief      Sets the delay value for the specified source
  * @param      signal_source_index: source of the signal
  * @param      delay_value: the new delay value
  * @retval     None
  */
void s4533087_hal_iss_delaytimer_ctrl(unsigned char signal_source_index,
        int delay_value) {
    
    if(delay_value < 0) {
        return;
    }
    iss_delay_val[signal_source_index] = (uint32_t)delay_value;
}

// Interrupt service routine
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin) {
    
    if(GPIO_Pin == BRD_A0_PIN) {
        advance_bar();
        debug_printf("!!! ARRIVED !!!\r\n");
    }
}

// Hands off interrupt to above ISR
void EXTI3_IRQHandler(void) {
    
    HAL_GPIO_EXTI_IRQHandler(BRD_A0_PIN);
}

