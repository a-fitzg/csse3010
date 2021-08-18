/**
 ******************************************************************************
 * @file    usart/main.c
 * @author  MDS, NPG
 * @date    02022015, 07122017
 * @brief   USART polling example program - transmits '0' to '9' via USART 6
 * 			USART 6 is USART_A on the Nucleo-144.
 * 			USART_A_TX is D1 is PG14.
 * 			USART_A_RX is D0 is PG9.
 ******************************************************************************
 *
 */
#include "board.h"
#include "processor_hal.h"
#include "debug_printf.h"
/* Includes ------------------------------------------------------------------*/

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
UART_HandleTypeDef UART_test;
/* Private function prototypes -----------------------------------------------*/
void Hardware_init(void);
/* Private functions ---------------------------------------------------------*/

/**
 * @brief  Main program
 * @param  None
 * @retval None
 */
int main(void) {

	uint8_t tx_char;
	uint8_t rx_char;
	int tx_count = 0;
	int status;

	BRD_init(); // Initialise Board
	Hardware_init(); // Initialise hardware peripherals

	/* Infinite loop */
	while (1) {

		tx_char = '0' + tx_count; // Send characters '0' to '9' in ASCII

		// Transmit 1 character - do not delay
		status = HAL_UART_Transmit(&UART_test, &tx_char, 1, 0);
		if ((status != HAL_OK) && (status != HAL_TIMEOUT)) {
			debug_printf("Transmit ERROR %d\n\r", status);
		}

		tx_count = (tx_count + 1) % 10; // '0' to '9'

		// Check for 1 received character - do not delay
		status = HAL_UART_Receive(&UART_test, &rx_char, 1, 0);
		if (status == HAL_OK) {
			debug_printf("RX %c\n\r", rx_char);
		}

		BRD_LEDBlueToggle();		//Toggle blue LED on/off
		HAL_Delay(1000);		//Delay for 1s (1000ms)

	}
}

/**
 * @brief  Initialise Hardware
 * @param  None
 * @retval None
 */
void Hardware_init(void) {

	GPIO_InitTypeDef GPIO_InitStructure;

	BRD_LEDInit(); //Initialise LEDs

	/* Turn off LEDs */
	BRD_LEDRedOff();
	BRD_LEDGreenOff();
	BRD_LEDBlueOff();

	// Enable USART6 clock
	__USART6_CLK_ENABLE();

	// Enable D0 and D1 clocks
	__BRD_D0_GPIO_CLK();
	__BRD_D1_GPIO_CLK();

	// Configure settings for USART 6
	UART_test.Instance          = (USART_TypeDef *) USART6_BASE;// USART 6
	UART_test.Init.BaudRate     = 9600;                         // Baudrate
	UART_test.Init.WordLength   = UART_WORDLENGTH_8B;           // 8 bits data length
	UART_test.Init.StopBits     = UART_STOPBITS_1;              // 1 stop bit
	UART_test.Init.Parity       = UART_PARITY_NONE;             // No parity
	UART_test.Init.Mode         = UART_MODE_TX_RX;              // Set for transmit and receive mode
	UART_test.Init.HwFlowCtl    = UART_HWCONTROL_NONE;          // No hardware flow control

																// Configure D0 as RX pin for USART6
	GPIO_InitStructure.Pin      = BRD_D0_PIN;
	GPIO_InitStructure.Mode     = GPIO_MODE_AF_PP;              // Enable alternate mode
	GPIO_InitStructure.Pull     = GPIO_PULLDOWN;
	GPIO_InitStructure.Speed    = GPIO_SPEED_FREQ_LOW;
	GPIO_InitStructure.Alternate= GPIO_AF8_USART6;              // Set alternate function to USART6
	HAL_GPIO_Init(BRD_D0_GPIO_PORT, &GPIO_InitStructure);        //

																// Configure D1 as TX pin for USART6
	GPIO_InitStructure.Pin      = BRD_D1_PIN;
	GPIO_InitStructure.Mode     = GPIO_MODE_AF_PP;              // Enable alternate mode
	GPIO_InitStructure.Pull     = GPIO_PULLUP;
	GPIO_InitStructure.Speed    = GPIO_SPEED_FREQ_LOW;
	GPIO_InitStructure.Alternate= GPIO_AF8_USART6;              // Set alternate function to USART6
	HAL_GPIO_Init(BRD_D1_GPIO_PORT, &GPIO_InitStructure);        //

	// Initialise USART peripheral
	HAL_UART_Init(&UART_test);
}
