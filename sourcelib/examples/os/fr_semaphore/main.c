/**
  ******************************************************************************
  * @file    ex15_fr_semaphore/main.c
  * @author  MDS
  * @date    02022015
  * @brief   FreeRTOS LED Flashing program using semaphores.Creates a semaphore to
  *			 signal a task to toggle the onboard Blue LED. Creates another semaphore
  *			 to start/stop the LED Flashing, using the onboard pushbutton.
  *
  *			 Press the pushbutton to start and stop LED flashing (run kermusb).
  *
  *			 NOTE: THE IDLE TASK MUST BE DISABLED.
  *			 NOTE: debug_printf is NOT thread-safe and will fail if called
  *			 directly, from multiple tasks.
  ******************************************************************************
  *
  */

/* Includes ------------------------------------------------------------------*/
#include "board.h"
#include "processor_hal.h"
#include "debug_printf.h"

/* Scheduler includes. */
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"


/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
SemaphoreHandle_t LEDSemaphore;	/* Semaphore for LED flasher */
SemaphoreHandle_t PBSemaphore;	/* Semaphore for pushbutton interrupt */

/* Private function prototypes -----------------------------------------------*/
static void Hardware_init();
void ApplicationIdleHook( void ); /* The idle hook is just used to stream data to the USB port */
void Give_Task( void );
void Take_Task( void );

/* Task Priorities ------------------------------------------------------------*/
#define mainGIVETASK_PRIORITY					( tskIDLE_PRIORITY + 2 )
#define mainTAKETASK_PRIORITY					( tskIDLE_PRIORITY + 2 )

/* Task Stack Allocations -----------------------------------------------------*/
#define mainGIVETASK_STACK_SIZE		( configMINIMAL_STACK_SIZE * 2 )
#define mainTAKETASK_STACK_SIZE		( configMINIMAL_STACK_SIZE * 2 )


/**
  * @brief  Starts all the other tasks, then starts the scheduler.
  * @param  None
  * @retval None
  */
int main( void ) {

	BRD_init();
	Hardware_init();

	/* Start task one, two and three. */
    xTaskCreate( (void *) &Give_Task, (const signed char *) "GIVE", mainGIVETASK_STACK_SIZE, NULL, mainGIVETASK_PRIORITY, NULL );
	xTaskCreate( (void *) &Take_Task, (const signed char *) "TAKE", mainTAKETASK_STACK_SIZE, NULL, mainTAKETASK_PRIORITY, NULL );

	/* Create Semaphores */
	LEDSemaphore = xSemaphoreCreateBinary();
	PBSemaphore = xSemaphoreCreateBinary();

	/* Start the scheduler.

	NOTE : Tasks run in system mode and the scheduler runs in Supervisor mode.
	The processor MUST be in supervisor mode when vTaskStartScheduler is
	called.  The demo applications included in the FreeRTOS.org download switch
	to supervisor mode prior to main being called.  If you are not using one of
	these demo application projects then ensure Supervisor mode is used here. */

	vTaskStartScheduler();

	/* We should never get here as control is now taken by the scheduler. */
  	return 0;
}

/**
  * @brief  Give Task.Gives LED Semaphore every second.
  * @param  None
  * @retval None
  */
void Give_Task( void ) {

	for (;;) {


		if (LEDSemaphore != NULL) {	/* Check if semaphore exists */

			/* Give LED Semaphore */
			xSemaphoreGive(LEDSemaphore);

			portENTER_CRITICAL();
			debug_printf("Giving LED Semaphore\n\r");
			portEXIT_CRITICAL();
		}

		/* Wait for 1000ms */
		vTaskDelay(1000);

	}
}


/**
  * @brief  Take Task. Used to take semaphore events. Toggles LED.
  * @param  None
  * @retval None
  */
void Take_Task( void ) {

	uint8_t mode = 1;
	int presscount = 0;

	BRD_LEDBlueOff();

	for (;;) {

		if (LEDSemaphore != NULL) {	/* Check if semaphore exists */

			/* See if we can obtain the LED semaphore. If the semaphore is not available
          	 wait 10 ticks to see if it becomes free. */
			if ( xSemaphoreTake( LEDSemaphore, 10 ) == pdTRUE ) {

				portENTER_CRITICAL();
				debug_printf("Taking LED Semaphore\n\r");
				portEXIT_CRITICAL();

            	/* We were able to obtain the semaphore and can now access the shared resource. */

            	/* Toggle LED, if in correct mode */
				if (mode == 1) {
					BRD_LEDBlueToggle();
				}
        	}
		}


		if (PBSemaphore != NULL) {	/* Check if semaphore exists */

			/* See if we can obtain the PB semaphore. If the semaphore is not available
           	wait 10 ticks to see if it becomes free. */
			if( xSemaphoreTake( PBSemaphore, 10 ) == pdTRUE ) {

            	/* We were able to obtain the semaphore and can now access the shared resource. */

            	/* Invert mode to stop or start LED flashing */
				mode = ~mode & 0x01;
				presscount++;

				/* Print LED Flashing status */
				if (mode) {
					portENTER_CRITICAL();
					debug_printf("LED Flashing ON - ");
					portEXIT_CRITICAL();
				} else {
					portENTER_CRITICAL();
					debug_printf("LED Flashing OFF - ");
					portEXIT_CRITICAL();
				}

				portENTER_CRITICAL();
				debug_printf("Press Count %d\n\r", presscount);
				portEXIT_CRITICAL();
        	}
		}

		vTaskDelay(1);
	}
}

/**
  * @brief  Hardware Initialisation.
  * @param  None
  * @retval None
  */
static void Hardware_init( void ) {

	GPIO_InitTypeDef GPIO_InitStructure;

	portDISABLE_INTERRUPTS();	//Disable interrupts

	BRD_LEDInit();				//Initialise Blue LED
	BRD_LEDBlueOff();				//Turn off Blue LED

	BRD_USER_BUTTON_GPIO_CLK_ENABLE();

	GPIO_InitStructure.Mode = GPIO_MODE_IT_RISING;
	GPIO_InitStructure.Pull = GPIO_NOPULL;
	GPIO_InitStructure.Pin  = BRD_USER_BUTTON_PIN;
	HAL_GPIO_Init(BRD_USER_BUTTON_GPIO_PORT, &GPIO_InitStructure);

	HAL_NVIC_SetPriority(BRD_USER_BUTTON_EXTI_IRQn, 10, 0);
	HAL_NVIC_EnableIRQ(BRD_USER_BUTTON_EXTI_IRQn);

	portENABLE_INTERRUPTS();	//Enable interrupts
}

/**
 * @brief EXTI line detection callback
 * @param GPIO_Pin: Specifies the pins connected EXTI line
 * @retval None
 */
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
	BaseType_t xHigherPriorityTaskWoken;

	if (GPIO_Pin == BRD_USER_BUTTON_PIN)
	{
		/* Toggle all 3 board LEDs */
		BRD_LEDToggle(BRD_GREEN_LEDMASK|BRD_BLUE_LEDMASK|BRD_RED_LEDMASK);

		/* Is it time for another Task() to run? */
		xHigherPriorityTaskWoken = pdFALSE;

		if (PBSemaphore != NULL) {	/* Check if semaphore exists */
			xSemaphoreGiveFromISR( PBSemaphore, &xHigherPriorityTaskWoken );		/* Give PB Semaphore from ISR*/
		}

			/* Perform context switching, if required. */
			portYIELD_FROM_ISR( xHigherPriorityTaskWoken );
	}
}


//Override default mapping of this handler to Default_Handler
void EXTI15_10_IRQHandler(void)
{
	HAL_GPIO_EXTI_IRQHandler(BRD_USER_BUTTON_PIN);
}


/**
  * @brief  Application Tick Task.
  * @param  None
  * @retval None
  */
void vApplicationTickHook( void ) {

	BRD_LEDBlueOff();
}

/**
  * @brief  Idle Application Task (Disabled)
  * @param  None
  * @retval None
  */
void vApplicationIdleHook( void ) {
	static portTickType xLastTx = 0;

	BRD_LEDBlueOff();

	for (;;) {
		/* The idle hook simply prints the idle tick count */
		if ((xTaskGetTickCount() - xLastTx ) > (1000 / portTICK_RATE_MS)) {
			xLastTx = xTaskGetTickCount();
			BRD_LEDBlueToggle();
		}
	}
}

/**
  * @brief  vApplicationStackOverflowHook
  * @param  Task Handler and Task Name
  * @retval None
  */
void vApplicationStackOverflowHook( xTaskHandle pxTask, signed char *pcTaskName ) {
	/* This function will get called if a task overflows its stack.   If the
	parameters are corrupt then inspect pxCurrentTCB to find which was the
	offending task. */

	BRD_LEDBlueOff();
	( void ) pxTask;
	( void ) pcTaskName;

	for( ;; );
}

