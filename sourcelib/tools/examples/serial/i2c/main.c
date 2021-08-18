/**
 ******************************************************************************
 * @file    i2c/main.c
 * @author  MDS, NPG
 * @date    02022015, 07122017
 * @brief   I2C example with the MMA8462Q. Reads and displays the WHO_AM_I reg.
 * 			See the MMA8462 Datasheet (p15)
 * 			Uses D15 (I2C_A_SCL) & D14 (I2C_A_SDA)
 ******************************************************************************
 *
 */
#include "board.h"
#include "processor_hal.h"
#include "debug_printf.h"
/* Includes ------------------------------------------------------------------*/

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define MMA8452Q_ADDRESS	0x1D << 1
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
static I2C_HandleTypeDef I2CHandle;
/* Private function prototypes -----------------------------------------------*/
void Hardware_init(void);
/* Private functions ---------------------------------------------------------*/

/**
 * @brief  Main program
 * @param  None
 * @retval None
 */
int main(void) {

	uint8_t read_reg_val;

	BRD_init(); // Initialise Board
	Hardware_init(); // Initialise hardware peripherals

	/* Infinite loop */
	while (1) {

		__HAL_I2C_CLEAR_FLAG(&I2CHandle, I2C_FLAG_AF);	//Clear Flags

		I2CHandle.Instance->CR1 |= I2C_CR1_START;	// Generate the START condition

		/*  Wait the START condition has been correctly sent */
		while (__HAL_I2C_GET_FLAG(&I2CHandle, I2C_FLAG_SB) == RESET);

		/* Send Peripheral Device Write address */
		I2CHandle.Instance->DR = __HAL_I2C_7BIT_ADD_WRITE(MMA8452Q_ADDRESS);

		/* Wait for address to be acknowledged */
		while (__HAL_I2C_GET_FLAG(&I2CHandle, I2C_FLAG_ADDR) == RESET);
		__HAL_I2C_CLEAR_ADDRFLAG(&I2CHandle);		//Clear ADDR Flag

		/* Send Read Register Address - WHO_AM_I Register Address */
		I2CHandle.Instance->DR = 0x0D;

		/* Wait until register Address byte is transmitted */
		while ((__HAL_I2C_GET_FLAG(&I2CHandle, I2C_FLAG_TXE) == RESET) && (__HAL_I2C_GET_FLAG(&I2CHandle, I2C_FLAG_BTF) == RESET));

		/* Generate the START condition, again */
		I2CHandle.Instance->CR1 |= I2C_CR1_START;

		/* Wait the START condition has been correctly sent */
		while (__HAL_I2C_GET_FLAG(&I2CHandle, I2C_FLAG_SB) == RESET);

		/* Send Read Address */
		I2CHandle.Instance->DR = __HAL_I2C_7BIT_ADD_READ(MMA8452Q_ADDRESS);

		/* Wait address is acknowledged */
		while (__HAL_I2C_GET_FLAG(&I2CHandle, I2C_FLAG_ADDR) == RESET);
		__HAL_I2C_CLEAR_ADDRFLAG(&I2CHandle);		//Clear ADDR Flag

		/* Wait to read */
		while (__HAL_I2C_GET_FLAG(&I2CHandle, I2C_FLAG_RXNE) == RESET);

		/* Read received value */
		read_reg_val = I2CHandle.Instance->DR;

		/* Generate NACK */
		I2CHandle.Instance->CR1 &= ~I2C_CR1_ACK;

		/* Generate the STOP condition */
		I2CHandle.Instance->CR1 |= I2C_CR1_STOP;

		debug_printf("MMA8452Q WHO_AM_I Register value is %X (default is 0x2A)\n\r", read_reg_val);	//Display WHO_AM_I Register value (0x2A)

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

	BRD_LEDInit();	//Initialise LEDs

	/* Turn off LEDs */
	BRD_LEDRedOff();
	BRD_LEDGreenOff();
	BRD_LEDBlueOff();

	// Enable GPIO clocks
	__BRD_SCL_A_GPIO_CLK();
	__BRD_SDA_A_GPIO_CLK();

	// Enable I2C peripheral clock
	__BRD_I2C_A_CLK();

	/******************************************************/
	/* IMPORTANT NOTE: SCL Must be Initialised BEFORE SDA */
	/******************************************************/
	/* enable GPIO pins for I2C */
	GPIO_InitStructure.Pin        = GPIO_PIN_8;
	GPIO_InitStructure.Mode       = GPIO_MODE_AF_OD;
	GPIO_InitStructure.Speed      = GPIO_SPEED_FREQ_LOW;
	GPIO_InitStructure.Pull       = GPIO_PULLUP;
	GPIO_InitStructure.Alternate  = GPIO_AF4_I2C1;
	HAL_GPIO_Init(GPIOB, &GPIO_InitStructure);

	GPIO_InitStructure.Pin        = GPIO_PIN_9;
	GPIO_InitStructure.Mode       = GPIO_MODE_AF_OD;
	GPIO_InitStructure.Speed      = GPIO_SPEED_FREQ_LOW;
	GPIO_InitStructure.Pull       = GPIO_PULLUP;
	GPIO_InitStructure.Alternate  = GPIO_AF4_I2C1;
	HAL_GPIO_Init(GPIOB, &GPIO_InitStructure);

	//Configure the I2C peripheral
	I2CHandle.Instance            = I2C1;
	I2CHandle.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
	I2CHandle.Init.ClockSpeed     = 100000;
	I2CHandle.Init.DualAddressMode= I2C_DUALADDRESS_DISABLE;
	I2CHandle.Init.DutyCycle      = I2C_DUTYCYCLE_2;
	I2CHandle.Init.GeneralCallMode= I2C_GENERALCALL_DISABLE;
	I2CHandle.Init.NoStretchMode  = I2C_NOSTRETCH_DISABLE;
	I2CHandle.Init.OwnAddress1    = 0;
	I2CHandle.Init.OwnAddress2    = 0;

	// Initialise and start the I2C peripheral
	HAL_I2C_Init(&I2CHandle);

	/* -> Wait for the end of the transfer */
	/* Before starting a new communication transfer, you need to check the current
	 * state of the peripheral; if it�s busy you need to wait for the end of current
	 * transfer before starting a new one.
	 * For simplicity reasons, this example is just waiting till the end of the
	 * transfer, but application may perform other tasks while transfer operation
	 * is ongoing.
	 */
	while (HAL_I2C_GetState(&I2CHandle) != HAL_I2C_STATE_READY);
}
