/**
  *****************************************************************************
  * @file    s4533087_hal_joystick.c
  * @author  Alexander FitzGerald - s4533087
  * @date    11032020
  * @brief   Joystick HAL driver
  *****************************************************************************
  * EXTERNAL FUNCTIONS
  *****************************************************************************
  * s4533087_hal_joystick_init()
  * s4533087_hal_joystick_deinit()
  *****************************************************************************
  */

/* Includes ---------------------------------------------------------------- */
#include "board.h"
#include "processor_hal.h"
#include "debug_printf.h"
#include "s4533087_hal_joystick.h"

/* Private typedef --------------------------------------------------------- */
/* Private define ---------------------------------------------------------- */
/* Private macro ----------------------------------------------------------- */
/* Private variables ------------------------------------------------------- */
/* Private function prototypes --------------------------------------------- */

/**
  * @brief      Initialise GPIO pins and ADC
  * @params     None
  * @retval     None
  */
void s4533087_hal_joystick_init(void) {

    GPIO_InitTypeDef GPIO_InitStructure;
    
    __BRD_A1_GPIO_CLK();
    __BRD_A2_GPIO_CLK();
    __BRD_A3_GPIO_CLK();

    // Initialise ADC1 for the joystick X axis
    GPIO_InitStructure.Pin  = BRD_A1_PIN;
    GPIO_InitStructure.Mode = GPIO_MODE_ANALOG;
    GPIO_InitStructure.Pull = GPIO_NOPULL;

    HAL_GPIO_Init(BRD_A1_GPIO_PORT, &GPIO_InitStructure);

    __ADC1_CLK_ENABLE();

    s4533087_AdcXHandle.Instance                      = 
            (ADC_TypeDef*)(ADC1_BASE);
    s4533087_AdcXHandle.Init.ClockPrescaler           = 
            ADC_CLOCKPRESCALER_PCLK_DIV2;
    s4533087_AdcXHandle.Init.Resolution               = ADC_RESOLUTION12b;
    s4533087_AdcXHandle.Init.ScanConvMode             = DISABLE;
    s4533087_AdcXHandle.Init.ContinuousConvMode       = DISABLE;
    s4533087_AdcXHandle.Init.DiscontinuousConvMode    = DISABLE;
    s4533087_AdcXHandle.Init.NbrOfDiscConversion      = 0;
    s4533087_AdcXHandle.Init.ExternalTrigConvEdge     = 
            ADC_EXTERNALTRIGCONVEDGE_NONE;
    s4533087_AdcXHandle.Init.ExternalTrigConv         = 
            ADC_EXTERNALTRIGCONV_T1_CC1;
    s4533087_AdcXHandle.Init.DataAlign                = ADC_DATAALIGN_RIGHT;
    s4533087_AdcXHandle.Init.NbrOfConversion          = 1;
    s4533087_AdcXHandle.Init.DMAContinuousRequests    = DISABLE;
    s4533087_AdcXHandle.Init.EOCSelection             = DISABLE;

    HAL_ADC_Init(&s4533087_AdcXHandle);

    AdcXChanConfig.Channel       = BRD_A1_ADC_CHAN;
    AdcXChanConfig.Rank          = 1;
    AdcXChanConfig.SamplingTime  = ADC_SAMPLETIME_3CYCLES;
    AdcXChanConfig.Offset        = 0;

    HAL_ADC_ConfigChannel(&s4533087_AdcXHandle, &AdcXChanConfig);

    // Initialise ADC2 for the joystick Y axis
    GPIO_InitStructure.Pin  = BRD_A2_PIN;
    GPIO_InitStructure.Mode = GPIO_MODE_ANALOG;
    GPIO_InitStructure.Pull = GPIO_NOPULL;

    HAL_GPIO_Init(BRD_A2_GPIO_PORT, &GPIO_InitStructure);

    __ADC2_CLK_ENABLE();

    s4533087_AdcYHandle.Instance                      = 
            (ADC_TypeDef*)(ADC2_BASE);
    s4533087_AdcYHandle.Init.ClockPrescaler           = 
            ADC_CLOCKPRESCALER_PCLK_DIV2;
    s4533087_AdcYHandle.Init.Resolution               = ADC_RESOLUTION12b;
    s4533087_AdcYHandle.Init.ScanConvMode             = DISABLE;
    s4533087_AdcYHandle.Init.ContinuousConvMode       = DISABLE;
    s4533087_AdcYHandle.Init.DiscontinuousConvMode    = DISABLE;
    s4533087_AdcYHandle.Init.NbrOfDiscConversion      = 0;
    s4533087_AdcYHandle.Init.ExternalTrigConvEdge     = 
            ADC_EXTERNALTRIGCONVEDGE_NONE;
    s4533087_AdcYHandle.Init.ExternalTrigConv         = 
            ADC_EXTERNALTRIGCONV_T1_CC1;
    s4533087_AdcYHandle.Init.DataAlign                = ADC_DATAALIGN_RIGHT;
    s4533087_AdcYHandle.Init.NbrOfConversion          = 1;
    s4533087_AdcYHandle.Init.DMAContinuousRequests    = DISABLE;
    s4533087_AdcYHandle.Init.EOCSelection             = DISABLE;

    HAL_ADC_Init(&s4533087_AdcYHandle);

    AdcYChanConfig.Channel       = BRD_A2_ADC_CHAN;
    AdcYChanConfig.Rank          = 1;
    AdcYChanConfig.SamplingTime  = ADC_SAMPLETIME_3CYCLES;
    AdcYChanConfig.Offset        = 0;

    HAL_ADC_ConfigChannel(&s4533087_AdcYHandle, &AdcYChanConfig);

    // Configure joystick switch
    // The joystick will be polled, and will not use interrupts for the switch
    GPIO_InitStructure.Pin = BRD_A3_PIN;
    GPIO_InitStructure.Mode = GPIO_MODE_IT_FALLING;
    GPIO_InitStructure.Pull = GPIO_PULLUP;
    GPIO_InitStructure.Speed = GPIO_SPEED_FAST;
    HAL_GPIO_Init(BRD_A3_GPIO_PORT, &GPIO_InitStructure);
}

/**
  * @brief  Deinitialise GPIO pins and ADC
  * @params None
  * @retval None
  */
void s4533087_hal_joystick_deinit(void) {

    // Deinit Joystick X
    HAL_GPIO_DeInit(BRD_A1_GPIO_PORT, BRD_A1_PIN);
    __ADC1_CLK_DISABLE();

    // Deinit Joystick Y
    HAL_GPIO_DeInit(BRD_A2_GPIO_PORT, BRD_A2_PIN);
    __ADC2_CLK_DISABLE();

    // Deinit Joystick Z
    HAL_GPIO_DeInit(BRD_A3_GPIO_PORT, BRD_A3_PIN);
}
