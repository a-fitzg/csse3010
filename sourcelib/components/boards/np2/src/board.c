/**
  ******************************************************************************
  * @file    board.c
  * @author  MCD Application Team Modified by M. D'Souza
  * @version V1.1.0
  * @date    28-October-2011 - Modified 19-April-2015
  * @brief   This file provides set of firmware functions to manage UART, Leds 
  *          and push-button available on the NetduinoPlus2.
  ******************************************************************************
  * @attention
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2011 STMicroelectronics</center></h2>
  ******************************************************************************
  */ 
  
/* Includes ------------------------------------------------------------------*/
#include "board.h"
#include "stm32f4xx_hal_conf.h"
#include "stm32f4xx_hal.h"

#include <stdio.h>
#include <string.h>

#ifdef ENABLE_VCP
#include "usbd_cdc_vcp.h"

#include "usbd_conf.h"
#include "usbd_core.h"
#include "usbd_desc.h"
#include "usbd_cdc.h" 
//#include "usbd_hid.h"

/* Variables used for USB */

extern USBD_HandleTypeDef hUSBDDevice;
extern uint8_t VCP_Tx_Buffer[VCP_RX_BUFFER_SIZE];
extern uint8_t VCP_Rx_Buffer[VCP_RX_BUFFER_SIZE];

#endif

//#define ENABLE_ETHERNET	1	
//#define ENABLE_MMC		1

// HAL Tick Timer Initialisation using TIM5 
static TIM_HandleTypeDef TIM_HALTTickInit;

void tim5_irqhandler (void);

static uint32_t uwhaltick;

// Increment the HAL Tick 
void HAL_IncTick(void) {
  uwhaltick++;
}

// Get the HAL Tick count 
uint32_t HAL_GetTick(void) {

  return (uint32_t) __HAL_TIM_GET_COUNTER(&TIM_HALTTickInit)/1000;
}

// Get the HAL Tick count 
uint32_t HAL_GetTickus(void) {

  return (uint32_t) __HAL_TIM_GET_COUNTER(&TIM_HALTTickInit);
}

//Get HAL System Clock value
uint32_t HAL_GetSysClock() {

	unsigned short PrescalerValue;
	RCC_ClkInitTypeDef    sClokConfig;
  	uint32_t              uwTimclock, uwAPB1Prescaler = 0;
  	uint32_t              uwPrescalerValue = 0;
  	uint32_t              pFLatency;

	/* Timer 2 clock enable */
	__TIM5_CLK_ENABLE();
 

	/* Get APB1 prescaler */
  	uwAPB1Prescaler = sClokConfig.APB1CLKDivider;
  
  	/* Compute TIM6 clock */
  	if (uwAPB1Prescaler == 0) {
    	uwTimclock = HAL_RCC_GetPCLK1Freq();
  	} else {
    	uwTimclock = 2*HAL_RCC_GetPCLK1Freq();
  	}

	return uwTimclock;

}

// Initialise HAL Tick Timer 
HAL_StatusTypeDef HAL_InitTick(uint32_t TickPriority) {

	unsigned short PrescalerValue;

	// Timer 5 clock enable 
	HAL_TICKTIMER_CLK_ENABLE();

	//__TIM5_CLK_ENABLE();

	// Compute the prescaler value 
  	//PrescalerValue = (uint16_t) ((SystemCoreClock /2)/1000000) - 1;		//Set clock prescaler to 1kHz - SystemCoreClock is the system clock frequency. CANNOT GO BELOW 1.2kHZ
	PrescalerValue = (uint32_t) (( HAL_GetSysClock() / 1000000) - 1);

  	// Time base configuration 
	TIM_HALTTickInit.Instance = HAL_TICKTIMER;	//TIM5;				//Enable Timer 5
  	TIM_HALTTickInit.Init.Period = 0xFFFFFFFF;			//Set period to be 1s
  	TIM_HALTTickInit.Init.Prescaler = PrescalerValue;	//Set presale value
  	TIM_HALTTickInit.Init.ClockDivision = 0;			//Set clock division
	TIM_HALTTickInit.Init.RepetitionCounter = 0;	// Set Reload Value
  	TIM_HALTTickInit.Init.CounterMode = TIM_COUNTERMODE_UP;	//Set timer to count up.

	// Initialise Timer 
	HAL_TIM_Base_Init(&TIM_HALTTickInit);

	// Set priority of Timer 2 update Interrupt [0 (HIGH priority) to 15(LOW priority)] 
	//HAL_NVIC_SetPriority(TIM5_IRQn, TickPriority, 0);		//Set Main priority ot 10 and sub-priority ot 0.

	// Enable timer update interrupt and interrupt vector for Timer  
	//NVIC_SetVector(TIM5_IRQn, (uint32_t)&tim5_irqhandler);  
	//NVIC_EnableIRQ(TIM5_IRQn);

	// Start Timer 
	HAL_TIM_Base_Start(&TIM_HALTTickInit);

	//Configure the SysTick IRQ priority 
  //HAL_NVIC_SetPriority(SysTick_IRQn, TickPriority ,0);

  // Return function status 
  return HAL_OK;

}//*/

//Disable the Tick Interrupt
void HAL_SuspendTick(void) {
  // Disable SysTick Interrupt 
//  SysTick->CTRL &= ~SysTick_CTRL_TICKINT_Msk;
}

//Enable the Tick Interrupt
void HAL_ResumeTick(void) {
  // Enable SysTick Interrupt 
  //SysTick->CTRL  |= SysTick_CTRL_TICKINT_Msk;
}


//Delay for period (in ms)
void HAL_Delay(__IO uint32_t Delay) {

  uint32_t tickstart = 0;
  tickstart = HAL_GetTick();
  while((HAL_GetTick() - tickstart) < (Delay*1)) {
  }
}

//Delay for period (in us)
void HAL_Delayus(__IO uint32_t Delay) {

  uint32_t tickstart = 0;
  tickstart = HAL_GetTickus();
  while((HAL_GetTickus() - tickstart) < Delay) {
  }
}

/**
  * @brief  Delay Function.
  * @param  nCount:specifies the Delay time length.
  * @retval None
  */
void BRD_Delay(__IO unsigned long nCount) {
  	while(nCount--) {
  	}
}

//TIM5 Interrupt for HAL Delay
void tim5_irqhandler (void) {

	//Clear Update Flag
	__HAL_TIM_CLEAR_IT(&TIM_HALTTickInit, TIM_IT_UPDATE);
	HAL_IncTick();
}



UART_HandleTypeDef UART_debug;

/* Initialise Debug UART */
void BRD_debuguart_init()
{
	GPIO_InitTypeDef GPIO_serialtx;	
	
	//Enable DEBUG UART clock
	__BRD_DEBUG_UART_CLK();

	UART_debug.Instance = (USART_TypeDef *)(BRD_DEBUG_UART);
    UART_debug.Init.BaudRate   = BRD_DEBUG_UART_BAUDRATE;
    UART_debug.Init.WordLength = UART_WORDLENGTH_8B;
    UART_debug.Init.StopBits   = UART_STOPBITS_1;
    UART_debug.Init.Parity     = UART_PARITY_NONE;
    UART_debug.Init.HwFlowCtl  = UART_HWCONTROL_NONE;
	UART_debug.Init.Mode = UART_MODE_TX;

#ifdef ENABLE_DEBUG_UART
  	/* Configure the GPIO TX pin for DEBUG UART */ 
  	GPIO_serialtx.Pin = BRD_DEBUG_UART_TX_PIN;
  	GPIO_serialtx.Mode = GPIO_MODE_AF_PP;
  	GPIO_serialtx.Pull = GPIO_PULLUP;
  	GPIO_serialtx.Speed = GPIO_SPEED_FAST;
	GPIO_serialtx.Alternate = BRD_DEBUG_UART_AF;
  	HAL_GPIO_Init(BRD_DEBUG_UART_TX_GPIO_PORT, &GPIO_serialtx);
#endif

	HAL_UART_Init(&UART_debug);		//Initialise DEBUG UART
}

//Transmit char through debug uart and USB, if enabled
void BRD_debuguart_putc(unsigned char c)
{
	__HAL_UART_FLUSH_DRREGISTER(&UART_debug) = (uint8_t) c;

#ifdef ENABLE_VCP	
	VCP_putc(c);		//Allows printing through usb
#endif

}

//Transmit string through debug uart and USB, if enabled
void BRD_debuguart_puts(unsigned char *c)
{
	int i;

	for (i = 0; i < (int) strlen(c); i++) {
	__HAL_UART_FLUSH_DRREGISTER(&UART_debug) = (uint8_t) (*(c + i));

#ifdef ENABLE_DEBUG_UART
	HAL_Delayus(200);
#endif

	}

#ifdef ENABLE_VCP	
	VCP_puts(c);		//Allows printing through usb
#endif

}

//Transmit message through debug uart and USB, if enabled
void BRD_debuguart_putm(unsigned char *c, int len)
{
	int i;
	for (i = 0; i < len; i++) {
	__HAL_UART_FLUSH_DRREGISTER(&UART_debug) = (uint8_t) (*(c + i));
	}

#ifdef ENABLE_VCP	
	VCP_putm(c, len);		//Allows printing through usb
	//VCP_txflush();
#endif

}


/* Debug UART getc */
unsigned char BRD_debuguart_getc() {

	uint8_t rx_char;

	//Non Block receive - 0 delay (set to HAL_MAX_DELAY for blocking)
	if (HAL_UART_Receive(&UART_debug, &rx_char, 1, 0) == HAL_OK) {
		return rx_char;
	} else {
		return 0;
	}

	//return (uint8_t)(__HAL_UART_FLUSH_DRREGISTER(&UART_debug) & (uint8_t)0x00FF);
}

unsigned char debug_getuc(void) {
	uint8_t c = '\0';

#ifdef ENABLE_VCP
	VCP_getc(&c);
#endif

	//Check if character is received is not NULL from VCP
	if (c == '\0') {
		c = BRD_debuguart_getc();
	}

	return (unsigned char) c; 
}

int debug_getm(unsigned char *buf) {

#ifdef ENABLE_VCP
	return VCP_getbuffer(buf);
#endif
}

/**
  * @brief  Initialise the NP2 board by turning on the power headers
  * @param  Led: Specifies the Led to be configured. 
  *   This parameter can be one of following parameters:
  * @retval None
  */
void BRD_init() {

	GPIO_InitTypeDef  GPIO_InitPowerHeader;

	/* Ensure all priority bits are assigned as preemption priority bits. */
	HAL_NVIC_SetPriorityGrouping(NVIC_PRIORITYGROUP_4);	

	HAL_Init();		//Initialise and start the HAL driver (i.e. enable interrupt priorities, etc)
	
	//Enable fault exceptions - Bus, Usage and MemManage faults
	//SCB->SHCSR |= (SCB_SHCSR_BUSFAULTENA_Msk | SCB_SHCSR_USGFAULTENA_Msk | SCB_SHCSR_MEMFAULTENA_Msk);

	
  	/* Enable the GPIO clock for NP2 power headers */
  	__GPIOA_CLK_ENABLE();
	__GPIOB_CLK_ENABLE();
	__GPIOC_CLK_ENABLE();
	__GPIOD_CLK_ENABLE();
	
#ifdef ENABLE_ETHERNET
	/* Enable Ethernet clock */
	GPIO_InitPowerHeader.Pin = BRD_ENC_CLK_PIN;
  	GPIO_InitPowerHeader.Mode = GPIO_MODE_AF_PP;
  	GPIO_InitPowerHeader.Pull = GPIO_NOPULL;
  	GPIO_InitPowerHeader.Speed = GPIO_SPEED_HIGH;
	GPIO_InitPowerHeader.Alternate = BRD_ENC_CLK_AF;
  	HAL_GPIO_Init(BRD_ENC_CLK_GPIO_PORT, &GPIO_InitPowerHeader);
#endif

  	/* Configure the GPIO for NP2 power, SD MMC Card and Ethernet power control headers */
  	GPIO_InitPowerHeader.Pin = BRD_PWR_CRTL_HEADERS_PIN | BRD_MMC_PWR_PIN;
  	GPIO_InitPowerHeader.Mode = GPIO_MODE_OUTPUT_PP;
  	GPIO_InitPowerHeader.Pull = GPIO_PULLDOWN;
  	GPIO_InitPowerHeader.Speed = GPIO_SPEED_FAST;
  	HAL_GPIO_Init(BRD_PWR_CRTL_HEADERS_GPIO_PORT, &GPIO_InitPowerHeader);

	GPIO_InitPowerHeader.Pin = BRD_ENC_PWR_PIN;
  	GPIO_InitPowerHeader.Mode = GPIO_MODE_OUTPUT_PP;
  	GPIO_InitPowerHeader.Pull = GPIO_PULLDOWN;
  	GPIO_InitPowerHeader.Speed = GPIO_SPEED_FAST;
  	HAL_GPIO_Init(BRD_ENC_PWR_GPIO_PORT, &GPIO_InitPowerHeader);

	/* Turn on NP2 Power headers */
	HAL_GPIO_WritePin(BRD_PWR_CRTL_HEADERS_GPIO_PORT, BRD_PWR_CRTL_HEADERS_PIN, GPIO_PIN_SET);

	/* Turn Ethernet Power on */	
#ifdef ENABLE_ETHERNET
	HAL_GPIO_WritePin(BRD_ENC_PWR_GPIO_PORT, BRD_ENC_PWR_PIN, GPIO_PIN_RESET);
#else
	HAL_GPIO_WritePin(BRD_ENC_PWR_GPIO_PORT, BRD_ENC_PWR_PIN, GPIO_PIN_SET);
#endif

	/* Turn SD MMC Card Power on */	
#ifdef ENABLE_MMC
	HAL_GPIO_WritePin(BRD_MMC_PWR_GPIO_PORT, BRD_MMC_PWR_PIN, GPIO_PIN_RESET);
#else
	HAL_GPIO_WritePin(BRD_MMC_PWR_GPIO_PORT, BRD_MMC_PWR_PIN, GPIO_PIN_SET);
#endif

	//enable debug uart
	BRD_debuguart_init();

#ifdef ENABLE_VCP
	/* Init Device Library */
  	USBD_Init(&hUSBDDevice, &VCP_Desc, 0);

  	/* Add Supported Class */
  	USBD_RegisterClass(&hUSBDDevice, USBD_CDC_CLASS);

	USBD_CDC_RegisterInterface(&hUSBDDevice, &USBD_CDC_VCP_fops);

  	/* Start Device Process */
 	USBD_Start(&hUSBDDevice);

	
//	USBD_Init(&USBD_Device, &VCP_Desc, 0);
  
  /* Add Supported Class */
  //USBD_RegisterClass(&USBD_Device, USBD_CDC_CLASS);
  
  /* Add CDC Interface Class */
  //USBD_CDC_RegisterInterface(&USBD_Device, &USBD_CDC_fops);
  
  /* Start Device Process */
  //USBD_Start(&USBD_Device);
	
#endif
}

/**
  * @brief  Configures LED GPIO.
  * @param  Led: Specifies the Led to be configured. 
  *   This parameter can be one of following parameters:

  * @retval None
  */
void BRD_LEDInit() {
  GPIO_InitTypeDef  GPIO_InitStructure;
  
  /* Enable the GPIO_LED Clock */
  __BRD_LED_GPIO_CLK();

  /* Configure the GPIO_LED pin */
  GPIO_InitStructure.Pin = BRD_LED_PIN;
  GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStructure.Pull = GPIO_PULLUP;
  GPIO_InitStructure.Speed = GPIO_SPEED_FAST;
  HAL_GPIO_Init(BRD_LED_GPIO_PORT, &GPIO_InitStructure);
}

/**
  * @brief  Turns selected LED On.
  * @param  Led: Specifies the Led to be set on. 
  *   This parameter can be one of following parameters:  
  * @retval None
  */
void BRD_LEDOn() {
  BRD_LED_GPIO_PORT->BSRR |= BRD_LED_PIN;
}

/**
  * @brief  Turns selected LED Off.
  * @param  Led: Specifies the Led to be set off. 
  *   This parameter can be one of following parameters:
  * @retval None
  */
void BRD_LEDOff() {
  BRD_LED_GPIO_PORT->BSRR |= BRD_LED_PIN << 16;  
}

/**
  * @brief  Toggles the selected LED.
  * @param  Led: Specifies the Led to be toggled. 
  *   This parameter can be one of following parameters:  
  * @retval None
  */
void BRD_LEDToggle() {
  BRD_LED_GPIO_PORT->ODR ^= BRD_LED_PIN;
}

/**
  * @brief  Configures Button GPIO and EXTI Line.
  * @param  Button: Specifies the Button to be configured.
  *   This parameter should be: BUTTON_USER
  * @param  Button_Mode: Specifies Button mode.
  *   This parameter can be one of following parameters:   
  *     @arg BUTTON_MODE_GPIO: Button will be used as simple IO 
  *     @arg BUTTON_MODE_EXTI: Button will be connected to EXTI line with interrupt
  *                            generation capability  
  * @retval None
  */ /*
void BRD_PBInit(Button_TypeDef Button, ButtonMode_TypeDef Button_Mode)
{
  GPIO_InitTypeDef GPIO_InitStructure;
  EXTI_InitTypeDef EXTI_InitStructure;
  NVIC_InitTypeDef NVIC_InitStructure;

  // Enable the BUTTON Clock 
  RCC_AHB1PeriphClockCmd(BUTTON_CLK[Button], ENABLE);
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);

  // Configure Button pin as input 
  GPIO_InitStructure.Mode = Mode_IN;
  GPIO_InitStructure.Pull = GPIO_NOPULL;
  GPIO_InitStructure.Pin = BUTTON_PIN[Button];
  GPIO_Init(BUTTON_PORT[Button], &GPIO_InitStructure);

  if (Button_Mode == BUTTON_MODE_EXTI)
  {
    // Connect Button EXTI Line to Button GPIO Pin 
    SYSCFG_EXTILineConfig(BUTTON_PORT_SOURCE[Button], BUTTON_PIN_SOURCE[Button]);

    // Configure Button EXTI line 
    EXTI_InitStructure.EXTI_Line = BUTTON_EXTI_LINE[Button];
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;  
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_Init(&EXTI_InitStructure);

    // Enable and set Button EXTI Interrupt to the lowest priority 
    NVIC_InitStructure.NVIC_IRQChannel = BUTTON_IRQn[Button];
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x0F;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x0F;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;

    NVIC_Init(&NVIC_InitStructure); 
  }
}//*/

/**
  * @brief  Returns the selected Button state.
  * @param  Button: Specifies the Button to be checked.
  *   This parameter should be: BUTTON_USER  
  * @retval The Button GPIO pin value.
  *//*
uint32_t BRD_PBGetState(Button_TypeDef Button)
{
  return GPIO_ReadInputDataBit(BUTTON_PORT[Button], BUTTON_PIN[Button]);
}*/
    
