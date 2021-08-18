/**
  ******************************************************************************
  * @file    adc/adc.c
  * @author  MDS, NPG
  * @date    05122017
  * @brief   Enable the ADC1 on pin A0.
  *			 See Section 13 (ADC), P385 of the STM32F4xx Reference Manual.
  ******************************************************************************
  *
  */
/* Includes ------------------------------------------------------------------*/
#include <stdio.h>
#include <math.h>


#include "board.h"
#include "processor_hal.h"
#include "debug_printf.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
ADC_HandleTypeDef AdcHandle;
ADC_ChannelConfTypeDef AdcChanConfig;
/* Private function prototypes -----------------------------------------------*/
void Hardware_init(void);
/* Private functions ---------------------------------------------------------*/

/**
 * @brief  Main program
 * @param  None
 * @retval None
 */
int main(void)
{
	unsigned int adc_value;

    int mapped = 0;

	BRD_init();
	Hardware_init();

	/* Infinite loop */
	while (1)
	{

		HAL_ADC_Start(&AdcHandle); // Start ADC conversion

		// Wait for ADC conversion to finish
		while (HAL_ADC_PollForConversion(&AdcHandle, 10) != HAL_OK);

		adc_value = (uint16_t)(HAL_ADC_GetValue(&AdcHandle));

        // y = (1 / 2048^2) * (x - 2048)^3 + 2048
        //double newValue = ((1 / (2048 * 2048)) * ((double)adc_value - 2048) + 2048);
        double newValue = (double)adc_value;
        double mapped = (pow(2048, -2) * pow((double)adc_value - 2048, 3) + 2048);

        uint16_t scaled = (uint16_t)newValue / 55;

        debug_printf("%f -> %f\r\n", newValue, mapped);
        /*
		// Print ADC conversion values
		debug_printf("\r|");
        for (int i = 0; i < scaled; i++) {
            debug_printf("#");
        }
        for (int i = scaled; i < 75; i++) {
            debug_printf(" ");
        }
        debug_printf("|");
        */

		BRD_LEDBlueToggle();
		HAL_Delay(100);
	}
}

/**
 * @brief  Initialise Hardware
 * @param  None
 * @retval None
 */
void Hardware_init(void) {

	BRD_LEDInit();		//Initialise LEDS

	/* Turn off LEDs */
	BRD_LEDRedOff();
	BRD_LEDGreenOff();
	BRD_LEDBlueOff();

	GPIO_InitTypeDef GPIO_InitStructure;

	__BRD_A1_GPIO_CLK();

	GPIO_InitStructure.Pin = BRD_A1_PIN;
	GPIO_InitStructure.Mode = GPIO_MODE_ANALOG;
	GPIO_InitStructure.Pull = GPIO_NOPULL;

	HAL_GPIO_Init(BRD_A1_GPIO_PORT, &GPIO_InitStructure);

	__ADC1_CLK_ENABLE();

	AdcHandle.Instance = (ADC_TypeDef *)(ADC1_BASE);						//Use ADC1
	AdcHandle.Init.ClockPrescaler        = ADC_CLOCKPRESCALER_PCLK_DIV2;	//Set clock prescaler
	AdcHandle.Init.Resolution            = ADC_RESOLUTION12b;				//Set 12-bit data resolution
	AdcHandle.Init.ScanConvMode          = DISABLE;
	AdcHandle.Init.ContinuousConvMode    = DISABLE;
	AdcHandle.Init.DiscontinuousConvMode = DISABLE;
	AdcHandle.Init.NbrOfDiscConversion   = 0;
	AdcHandle.Init.ExternalTrigConvEdge  = ADC_EXTERNALTRIGCONVEDGE_NONE;	//No Trigger
	AdcHandle.Init.ExternalTrigConv      = ADC_EXTERNALTRIGCONV_T1_CC1;		//No Trigger
	AdcHandle.Init.DataAlign             = ADC_DATAALIGN_RIGHT;				//Right align data
	AdcHandle.Init.NbrOfConversion       = 1;
	AdcHandle.Init.DMAContinuousRequests = DISABLE;
	AdcHandle.Init.EOCSelection          = DISABLE;

	HAL_ADC_Init(&AdcHandle);		//Initialise ADC

	/* Configure ADC Channel */
	AdcChanConfig.Channel = BRD_A1_ADC_CHAN;							//Use AO pin
	AdcChanConfig.Rank         = 1;
	AdcChanConfig.SamplingTime = ADC_SAMPLETIME_3CYCLES;
	AdcChanConfig.Offset       = 0;

	HAL_ADC_ConfigChannel(&AdcHandle, &AdcChanConfig);		//Initialise ADC channel
}

