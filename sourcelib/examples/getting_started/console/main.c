/**
 ******************************************************************************
 * @file    console/main.c
 * @author  MDS, NPG
 * @date    02022015, 07122017
 * @brief   Receive byte from serial console and print back to console using
 *		    debug printf or debug putc.
 *			Use kermusb to start the kermit console.
 *			Comment or uncomment PRINTF_REFLECT to choose either printf or putc.
 ******************************************************************************
 *
 */
#include "board.h"
#include "debug_printf.h"
#include "processor_hal.h"

/* Includes ------------------------------------------------------------------*/

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define PRINTF_REFLECT
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
void Hardware_init(void);
/* Private functions ---------------------------------------------------------*/
/**
 * @brief  Main program
 * @param  None
 * @retval None
 */
int main(void) {

	char RxChar;

	BRD_init(); // Initialise Board
	Hardware_init(); // Initialise hardware peripherals

	/* Infinite loop */
	while (1) {

		// Receive characters using getc
		RxChar = debug_getc();

		// Check if character is not null
		if (RxChar != '\0') {

#ifdef PRINTF_REFLECT
			debug_printf("%c", RxChar);// reflect byte using printf - must delay before calling printf again
#else
			debug_putc(RxChar);        // reflect byte using putc - puts character into buffer
#endif
		}

		BRD_LEDBlueToggle();            //Toggle blue LED on/off
		HAL_Delay(125);                //Delay for 125ms

	}
}

/**
 * @brief  Initialise Hardware
 * @param  None
 * @retval None
 */
void Hardware_init(void) {

	BRD_LEDInit(); //Initialise LEDs

	/* Turn off LEDs */
	BRD_LEDRedOff();
	BRD_LEDGreenOff();
	BRD_LEDBlueOff();

}
