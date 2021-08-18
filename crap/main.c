/**
  ******************************************************************************
  * @file    ex20_fr_events/main.c
  * @author  MDS
  * @date    08052016
  * @brief   FreeRTOS LED Flashing program using group events. Creates an event flag to
  *			 signal a task to toggle the onboard Blue LED.
  *
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
#include "event_groups.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define EVT_LED_ON			1 << 0		//LED Event Flag
#define EVT_LED_OFF			1 << 1		//Pushbutton Event Flag
#define LEDCTRL_EVENT		EVT_LED_ON | EVT_LED_OFF	//Control Event Group Mask

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
EventGroupHandle_t ledctrl_EventGroup;		//Control Event Group

/* Private function prototypes -----------------------------------------------*/
static void Hardware_init();
void ApplicationIdleHook( void ); // The idle hook is just used to stream data to the USB port.
void Give_Task( void );
void Take_Task( void );

/* Task Priorities ------------------------------------------------------------*/
#define mainGIVETASK_PRIORITY					( tskIDLE_PRIORITY + 4 )
#define mainTAKETASK_PRIORITY					( tskIDLE_PRIORITY + 2 )

/* Task Stack Allocations -----------------------------------------------------*/
#define mainGIVETASK_STACK_SIZE		( configMINIMAL_STACK_SIZE * 5 )
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

	/* Create Event Group */
	ledctrl_EventGroup = xEventGroupCreate();

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
  * @brief  Give Task. Set LED ON and OFF events every second.
  * @param  None
  * @retval None
  */
void Give_Task( void ) {

    portENTER_CRITICAL();
    debug_printf("Parent task created \r\n");
    portEXIT_CRITICAL();

	xTaskCreate( (void *) &Take_Task, (const signed char *) "TAKE", mainTAKETASK_STACK_SIZE, NULL, mainTAKETASK_PRIORITY, NULL );

	for (;;) {

        portENTER_CRITICAL();
        debug_printf("Parent task running\r\n");
        portEXIT_CRITICAL();
        
		vTaskDelay(1000);
	}
}


/**
  * @brief  Take Task. Used to check for LED ON and OFF events. Toggles LED.
  * @param  None
  * @retval None
  */
void Take_Task( void ) {

    portENTER_CRITICAL();
    debug_printf("Child task created\r\n");
    portEXIT_CRITICAL();

	for (;;) {

        portENTER_CRITICAL();
        debug_printf("Child running\r\n");
        portEXIT_CRITICAL();

		vTaskDelay(500);
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

	portENABLE_INTERRUPTS();	//Enable interrupts

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
			//debug_printf("IDLE Tick %d\n", xLastTx);
			//BRD_LEDBlueToggle();
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

	BRD_LEDBlueOn();
    BRD_LEDRedOn();
    BRD_LEDGreenOn();
	( void ) pxTask;
	( void ) pcTaskName;

	for( ;; );
}


