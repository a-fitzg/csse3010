/**
  ******************************************************************************
  * @file    ex21_fr_queueset/main.c
  * @author  MDS
  * @date    24032018
  * @brief   FreeRTOS queue set demonstration. Creates a queue to
  *			 to send an item from one task to another. Use semaphores to control
  *			 the LED Flashing with the push button. Queues and semaphores are
  *			 added to the queue set in the receiver task.
  *			 Open kermit to view output.

  *			 configUSE_QUEUE_SETS must be set to 1 in FreeRTOSConfig.h
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
#include <string.h>

/* Scheduler includes. */
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"


/* Private typedef -----------------------------------------------------------*/
struct Message {	/* Message consists of sequence number and payload string */
	int Sequence_Number;
	char Payload[10];
};

/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
QueueHandle_t MessageQueue;	/* Queue used */
SemaphoreHandle_t LEDSemaphore;	/* Semaphore for LED flasher */
SemaphoreHandle_t PBSemaphore;	/* Semaphore for pushbutton interrupt */

/* Private function prototypes -----------------------------------------------*/
static void Hardware_init();
void ApplicationIdleHook( void );
void Sender_Task( void );
void Receiver_Task( void );
void Give_Task( void );

/* Task Priorities ------------------------------------------------------------*/
#define mainSENDERTASK_PRIORITY					( tskIDLE_PRIORITY + 2 )
#define mainRECEIVERTASK_PRIORITY				( tskIDLE_PRIORITY + 2 )
#define mainGIVETASK_PRIORITY					( tskIDLE_PRIORITY + 2 )

/* Task Stack Allocations -----------------------------------------------------*/
#define mainSENDERTASK_STACK_SIZE		( configMINIMAL_STACK_SIZE * 2 )
#define mainRECEIVERTASK_STACK_SIZE		( configMINIMAL_STACK_SIZE * 2 )
#define mainGIVETASK_STACK_SIZE		( configMINIMAL_STACK_SIZE * 2 )

/**
  * @brief  Starts all the other tasks, then starts the scheduler.
  * @param  None
  * @retval None
  */
int main( void ) {

	BRD_init();
	Hardware_init();

	/* Start sender and receiver tasks */
    xTaskCreate( (void *) &Sender_Task, (const signed char *) "SEND", mainSENDERTASK_STACK_SIZE, NULL, mainSENDERTASK_PRIORITY, NULL );
	xTaskCreate( (void *) &Receiver_Task, (const signed char *) "RECV", mainRECEIVERTASK_STACK_SIZE, NULL, mainRECEIVERTASK_PRIORITY, NULL );
	xTaskCreate( (void *) &Give_Task, (const signed char *) "GIVE", mainGIVETASK_STACK_SIZE, NULL, mainGIVETASK_PRIORITY, NULL );

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
  * @brief  Sender Task.Send a message to the queue, every second.
  * @param  None
  * @retval None
  */
void Sender_Task( void ) {

	struct Message SendMessage;

	/*Initialise Message Item payload */
	strcpy(SendMessage.Payload, "Item");

	SendMessage.Sequence_Number = 0;

	for (;;) {


		if (MessageQueue != NULL) {	/* Check if queue exists */

			/*Send message to the front of the queue - wait atmost 10 ticks */
			if( xQueueSend(MessageQueue, ( void * ) &SendMessage, ( portTickType ) 200 ) != pdPASS ) {

				portENTER_CRITICAL();
				debug_printf("Failed to post the message, after 10 ticks.\n\r");
				portEXIT_CRITICAL();
			}
		}

		SendMessage.Sequence_Number++;		/* Increment Sequence Number */

		/* Wait for 1000ms */
		vTaskDelay(1000);

	}
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
		}

		/* Wait for 1000ms */
		vTaskDelay(1000);

	}
}


/**
  * @brief  Receiver Task. Used to receive messages.
  * @param  None
  * @retval None
  */
void Receiver_Task( void ) {

	struct Message RecvMessage;
	QueueSetHandle_t xQueueSet;
	QueueSetMemberHandle_t xActivatedMember;
	int mode = 0;
	int presscount = 0;

	/* Create QueueSet */
	xQueueSet = xQueueCreateSet(10 + 1 + 1);	//Size of Queueset = Size of Queue (10) + Size of Binary Semaphore (1) + Size of Binary Semaphore (1)

	/* Create elements - note elements must be created BEFORE being added */
	MessageQueue = xQueueCreate(10, sizeof(RecvMessage));		/* Create queue of length 10 Message items */

	LEDSemaphore = xSemaphoreCreateBinary();					/* Create LED Semaphore */
	PBSemaphore = xSemaphoreCreateBinary();						/* Create LED Semaphore */

	 /* Add the queues and semaphores to the set.  Reading from these queues and
    semaphore can only be performed after a call to xQueueSelectFromSet() has
    returned the queue or semaphore handle from this point on. */
    xQueueAddToSet(MessageQueue, xQueueSet);
    xQueueAddToSet(LEDSemaphore, xQueueSet);
    xQueueAddToSet(PBSemaphore, xQueueSet);

	BRD_LEDBlueOff();

	for (;;) {

		xActivatedMember = xQueueSelectFromSet(xQueueSet, 20);

		/* Which set member was selected?  Receives/takes can use a block time
        of zero as they are guaranteed to pass because xQueueSelectFromSet()
        would not have returned the handle unless something was available. */
        if (xActivatedMember == MessageQueue) {

			/* Receive item */
			xQueueReceive( MessageQueue, &RecvMessage, 0 );

			/* display received item */
			portENTER_CRITICAL();
			debug_printf("Received: %s - %d\n\r", RecvMessage.Payload, RecvMessage.Sequence_Number);
			portEXIT_CRITICAL();

		} else if (xActivatedMember == LEDSemaphore) {	/* Check if LED semaphore occurs */

			/* We were able to obtain the semaphore and can now access the shared resource. */
			xSemaphoreTake(LEDSemaphore, 0 );

			portENTER_CRITICAL();
			debug_printf("Taking LED Semaphore\n\r");
			portEXIT_CRITICAL();

            /* Toggle LED, if in correct mode */
			if (mode == 1) {
				BRD_LEDBlueToggle();
        	}

		} else if (xActivatedMember == PBSemaphore) {	/* Check if pb semaphore occurs */

			/* We were able to obtain the semaphore and can now access the shared resource. */
			xSemaphoreTake( PBSemaphore, 0 );

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

		/* Delay for 10ms */
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
	static TickType_t xLastTx = 0;

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

