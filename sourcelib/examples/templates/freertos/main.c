/**
  ******************************************************************************
  * @file    ex_freertos/main.c
  * @author  MDS
  * @date    27112017
  * @brief   Nucleo429ZI FreeRTOS onboard LED flashing example.
  ******************************************************************************
  */

#include <stdio.h>

/* Scheduler includes. */
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "board.h"
#include "debug_printf.h"


/* Task Priorities ------------------------------------------------------------*/
#define mainREDLED_PRIORITY					( tskIDLE_PRIORITY + 2 )
#define mainBLUELED_PRIORITY				( tskIDLE_PRIORITY + 2 )

/* Task Stack Allocations -----------------------------------------------------*/
#define mainREDLED_TASK_STACK_SIZE		( configMINIMAL_STACK_SIZE * 2 )
#define mainBLUELED_TASK_STACK_SIZE		( configMINIMAL_STACK_SIZE * 2 )


void ApplicationIdleHook( void ); /* The idle hook is used to blink the Blue 'Alive LED' every second */
void blueled_task( void );
void redled_task( void );
void Hardware_init( void );

			
int main(void)
{

	BRD_init();
	Hardware_init();

	/* Start the task to flash the LED. */
	xTaskCreate( (void *) &redled_task, (const signed char *) "RLED", mainREDLED_TASK_STACK_SIZE, NULL, mainREDLED_PRIORITY, NULL );
	xTaskCreate( (void *) &blueled_task, (const signed char *) "BLED", mainBLUELED_TASK_STACK_SIZE, NULL, mainBLUELED_PRIORITY, NULL );

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
  * @brief  LED Flashing Task.
  * @param  None
  * @retval None
  */
void redled_task( void ) {

	BRD_LEDRedOff();

	for (;;) {

		/* Toggle LED */
		BRD_LEDRedToggle();

		portENTER_CRITICAL();
		debug_printf("Red LED Flashing Task %d\n\r", HAL_GetTick());
		portEXIT_CRITICAL();

		/* Delay the task for 1000ms */
		vTaskDelay(2000);

	}
}

void blueled_task( void ) {

	BRD_LEDBlueOff();

	for (;;) {

		/* Toggle LED */
		BRD_LEDBlueToggle();

		portENTER_CRITICAL();
		debug_printf("Blue LED Flashing Task %d\n\r", HAL_GetTick());
		portEXIT_CRITICAL();

		/* Delay the task for 1000ms */
		vTaskDelay(1000);

	}
}

/**
  * @brief  Hardware Initialisation.
  * @param  None
  * @retval None
  */
void Hardware_init( void ) {

	portDISABLE_INTERRUPTS();	//Disable interrupts

	BRD_LEDInit();				//Initialise Blue LED
	BRD_LEDRedOff();				//Turn off Red LED
	BRD_LEDGreenOff();				//Turn off Green LED
	BRD_LEDBlueOff();				//Turn off Blue LED

	portENABLE_INTERRUPTS();	//Enable interrupts

}

/**
  * @brief  Application Tick Task.
  * @param  None
  * @retval None
  */
void vApplicationTickHook( void ) {

	BRD_LEDGreenOff();
}

/**
  * @brief  Idle Application Task
  * @param  None
  * @retval None
  */
void vApplicationIdleHook( void ) {
	static portTickType xLastTx = 0;

	BRD_LEDGreenOff();

	for (;;) {

		/* The idle hook simply prints the idle tick count, every second */
		if ((xTaskGetTickCount() - xLastTx ) > (1000 / portTICK_RATE_MS)) {

			xLastTx = xTaskGetTickCount();

			/* Blink Alive LED */
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

	//BRD_LEDBlueOff();
	( void ) pxTask;
	( void ) pcTaskName;

	for( ;; );
}


