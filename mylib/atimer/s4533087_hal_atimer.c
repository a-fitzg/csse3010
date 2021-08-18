/**
  *****************************************************************************
  * @file       s4533087_hal_atimer.c
  * @author     Alexander FitzGerald - s4533087
  * @date       11032020
  * @brief      Alive timer HAL driver
  *****************************************************************************
  * EXTERNAL FUNCTIONS
  *****************************************************************************
  * s4533087_hal_atimer_init()
  * s4533087_hal_atimer_timer_getms()
  * s4533087_hal_atimer_timer_reset()
  * s4533087_hal_atimer_timer_pause()
  * s4533087_hal_atimer_timer_resume()
  * s4533087_hal_atimer_clkspeed_set()
  * s4533087_hal_atimer_period_set()
  * s4533087_hal_atimer_init_pin()
  *****************************************************************************
  */

/* Includes ---------------------------------------------------------------- */
#include "board.h"
#include "processor_hal.h"
#include "debug_printf.h"
#include "s4533087_hal_atimer.h"

/* Private typedef --------------------------------------------------------- */
/* Private define ---------------------------------------------------------- */
#define s4533087_hal_atimer_timer_read()    0

/* Private macro ----------------------------------------------------------- */
/* Private variables ------------------------------------------------------- */
uint32_t atimerClkspeedVal;
uint32_t atimerPeriodVal;
uint32_t atimerPrescalerVal;
uint32_t atimerCounterVal;
TIM_HandleTypeDef TIM_Init;

/* Private function prototypes --------------------------------------------- */
void atimer_isr_callback(void);
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef*);
void TIM3_IRQHandler(void);

/**
  * @brief      Initialise and start Alive Timer (starts from 0)
  * @params     None
  * @retval     None
  */
void s4533087_hal_atimer_init(void) {

    uint32_t prescalerValue;

    s4533087_hal_atimer_init_pin();
    
    // Enable HAL timer 3
    __TIM3_CLK_ENABLE();
    
    // Computes prescaler value for timer
    // Prescaler to be set to 50kHz, defined in HAL_ATIMER_CLKSPEED
    prescalerValue = (uint32_t)((double)SystemCoreClock / 
            (double)S4533087_HAL_ATIMER_CLKSPEED) - 1;
    TIM_Init.Instance = TIM3;
    // Calculates the period based on default clock speed and period
    TIM_Init.Init.Period = (uint32_t)((double)S4533087_HAL_ATIMER_CLKSPEED * 
            ((double)S4533087_HAL_ATIMER_PERIOD / 1000));
    TIM_Init.Init.Prescaler = prescalerValue;
    TIM_Init.Init.ClockDivision = 0;
    TIM_Init.Init.RepetitionCounter = 0;
    TIM_Init.Init.CounterMode = TIM_COUNTERMODE_UP;
    
    // Initialise timer
    HAL_TIM_Base_Init(&TIM_Init);
    
    // Set interrupt priority
    HAL_NVIC_SetPriority(TIM3_IRQn, 10, 0);
    
    // Enable timer interrupts
    HAL_NVIC_EnableIRQ(TIM3_IRQn);
    
    // Start timer base unit in interrupt mode
    HAL_TIM_Base_Start_IT(&TIM_Init);
    
}

/**
  * @brief      Returns the atimer counter value in ms
  * @params     None
  * @retval     atimer counter value
  */
uint32_t s4533087_hal_atimer_timer_getms(void) {

    return atimerCounterVal;
}

/**
  * @brief      Resets atimer counter value to 0
  * @params     None
  * @retval     None
  */
void s4533087_hal_atimer_timer_reset(void) {

    atimerCounterVal = 0;
    HAL_NVIC_EnableIRQ(TIM3_IRQn);
}

/**
  * @brief      Pause the atimer
  * @params     None
  * @retval     None
  */
void s4533087_hal_atimer_timer_pause(void) {

    HAL_NVIC_DisableIRQ(TIM3_IRQn);
}

/**
  * @brief      Resume the atimer from previously paused-at value
  * @params     None
  * @retval     None
  */
void s4533087_hal_atimer_timer_resume(void) {

    HAL_NVIC_EnableIRQ(TIM3_IRQn);
}

/**
  * @brief      Print the current value of the atimer (in ticks)
  * @params     None
  * @retval     None
  */
void s4533087_hal_atimer_timer_printticks(void) {
    
    debug_printf("%d\r\n", atimerCounterVal);
}

/**
  * @brief      Print the current value of the atimer (in ms)
  * @params     None
  * @retval     None
  */
void s4533087_hal_atimer_timer_printms(void) {

    double elapsed = (((float)atimerCounterVal) / ((float)atimerClkspeedVal));
    debug_printf("%f\r\n", elapsed);
}

/**
  * @brief      Add addend to the period
  * @params     addend: addend value to add to period
  * @retval     None
  */
void s4533087_hal_atimer_timer_addperiod(int addend) {

    if (atimerPeriodVal > 10) {
        atimerPeriodVal += addend;
        TIM_Init.Init.Period = (uint32_t)((double)atimerClkspeedVal * 
                ((double)atimerPeriodVal / 1000));
        HAL_TIM_Base_Init(&TIM_Init);
    }
}

/**
  * @brief      Add addend to frequency
  * @params     addend: addend value to add to frequency
  * @retval     None
  */
void s4533087_hal_atimer_timer_addfrequency(int addend) {

    if (atimerClkspeedVal > 1000) {
        atimerClkspeedVal += addend;
        TIM_Init.Init.Prescaler = (uint32_t)(((double)SystemCoreClock / 
                (double)atimerClkspeedVal) - 1);
        HAL_TIM_Base_Init(&TIM_Init);
    }
}


/**
  * @brief      Sets the atimer clock speed (in Hz)
  * @params     Frequency (Hz)
  * @retval     None
  */
void s4533087_hal_atimer_clkspeed_set(uint32_t frequency) {

    atimerClkspeedVal = frequency;
    TIM_Init.Init.Prescaler = (uint32_t)(((double)SystemCoreClock / 
            (double)atimerClkspeedVal) - 1);
    HAL_TIM_Base_Init(&TIM_Init);
}

/**
  * @brief      Sets the atimer period (in ms)
  * @params     Period (ms)
  * @retval     None
  */
void s4533087_hal_atimer_period_set(uint32_t period) {

    atimerPeriodVal = (uint32_t)period;
    TIM_Init.Init.Period = (uint32_t)((double)atimerClkspeedVal * 
            ((double)atimerPeriodVal / 1000));
    HAL_TIM_Base_Init(&TIM_Init);

}

/**
  * @brief      Initialises the atimer GPIO pin
  * @params     None
  * @retval     None
  */
void s4533087_hal_atimer_init_pin(void) {

    GPIO_InitTypeDef GPIO_InitStructure;

    // Enable relevant pin clock
    S4533087_HAL_ATIMER_PINCLK;

    GPIO_InitStructure.Pin      = S4533087_HAL_ATIMER_PIN;
    GPIO_InitStructure.Mode     = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStructure.Pull     = GPIO_PULLDOWN;
    GPIO_InitStructure.Speed    = GPIO_SPEED_FAST;
    HAL_GPIO_Init(S4533087_HAL_ATIMER_PINPORT, &GPIO_InitStructure);
}

/**
  * @brief      ISR callback
  * @params     None
  * @retval     None
  */
void atimer_isr_callback(void) {

    unsigned char val = HAL_GPIO_ReadPin(S4533087_HAL_ATIMER_PINPORT, 
            S4533087_HAL_ATIMER_PIN);
    HAL_GPIO_WritePin(S4533087_HAL_ATIMER_PINPORT, S4533087_HAL_ATIMER_PIN, 
            (1 - val) & 0x01);
    atimerCounterVal++;
}

/**
  * @brief      Period elapsed callback for timer 3. Calls atimer's ISR
  * @params     htim: pointer to struct containing information for TIM module
  * @retval     None
  */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef* htim) {

    if (htim->Instance == TIM3) {
        
        atimer_isr_callback();
    }
}

/**
  * @brief      Override default mapping of interrupt handler
  * @params     None
  * @retval     None
  */
void TIM3_IRQHandler(void) {

    HAL_TIM_IRQHandler(&TIM_Init);
}
