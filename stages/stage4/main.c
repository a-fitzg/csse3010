/**
  *****************************************************************************
  * @file    stage4/main.c
  * @author  Alexander FitzGerald - 45330874
  * @date    24042020
  * @brief   Stage 4 - FreeRTOS
  *****************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "board.h"
#include "processor_hal.h"
#include "debug_printf.h"

// FreeRTOS / Scheduler includes
#include "FreeRTOS.h"
#include "task.h"
#include "event_groups.h"
#include "queue.h"
#include "semphr.h"

// OS driver includes
#include "s4533087_os_lta1000g.h"
#include "s4533087_os_joystick.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define DEBOUNCE_PERIOD 10

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
// Bool for pausing / resuming timer tasks
uint8_t paused = 0;

// Control event group
EventGroupHandle_t s4533087_EventTimerGroup;

// Struct in which to send time information to display
typedef struct {
    char type;
    unsigned char timerValue;
} DualTimerMsg;

// Queue for timer events
QueueHandle_t s4533087_QueueTimerTasks;

// Task handles
TaskHandle_t taskTimerLeftHandle;
TaskHandle_t taskTimerRightHandle;
TaskHandle_t taskTimerDisplayHandle;
TaskHandle_t taskTimerSwitchInterruptHandle;

/* Private function prototypes -----------------------------------------------*/
static void Hardware_init();
void ApplicationIdleHook(void);
void s4533087_TaskTimerLeft(void);
void s4533087_TaskTimerRight(void);
void s4533087_TaskTimerDisplay(void);
void s4533087_TaskTimerSwitchInterrupt(void);

/* Task Priorities -----------------------------------------------------------*/
#define TIMER_TASK_PRIORITY                 (tskIDLE_PRIORITY + 3)
#define MASTER_TIMER_TASK_PRIORITY          (tskIDLE_PRIORITY + 2)
#define TIMER_SWITCH_INTERRUPT_PRIORITY     (tskIDLE_PRIORITY + 4)

/* Task Stack Allocations ----------------------------------------------------*/
#define TIMER_TASK_STACK_SIZE                (configMINIMAL_STACK_SIZE * 4)

/**
  * @brief  Starts all the other tasks, then starts the scheduler.
  * @param  None
  * @retval None
  */
int main( void ) {

	BRD_init();
	Hardware_init();

	// Start all required tasks
    xTaskCreate((void*)&s4533087_TaskTimerLeft, 
            (const signed char*)"TimerLeft", TIMER_TASK_STACK_SIZE, NULL, 
            TIMER_TASK_PRIORITY, &taskTimerLeftHandle);

    xTaskCreate((void*)&s4533087_TaskTimerRight, 
            (const signed char*)"TimerRight", TIMER_TASK_STACK_SIZE, NULL, 
            TIMER_TASK_PRIORITY, &taskTimerRightHandle);

    xTaskCreate((void*)&s4533087_TaskTimerDisplay, 
            (const signed char*)"TimerDisplay", TIMER_TASK_STACK_SIZE, NULL, 
            MASTER_TIMER_TASK_PRIORITY, &taskTimerDisplayHandle);

    xTaskCreate((void*)&s4533087_TaskTimerSwitchInterrupt, 
            (const signed char*)"SwitchInterrupt", TIMER_TASK_STACK_SIZE, NULL,
            TIMER_SWITCH_INTERRUPT_PRIORITY, &taskTimerSwitchInterruptHandle);

    // Initialise sempahore for joystick switch interrupt
    s4533087_SemaphoreJoystickSwitchRaw = xSemaphoreCreateBinary();

	// Create Event Group
	s4533087_EventTimerGroup = xEventGroupCreate();

	// Start the scheduler.
	vTaskStartScheduler();

	// We should never get here as control is now taken by the scheduler.
  	return 0;
}

/**
  * @brief  Timer task for left timer, ticks every 1 second
  * @param  None
  * @retval None
  */
void s4533087_TaskTimerLeft(void) {

    DualTimerMsg sendLeft;

    // Makes queue of 10 items
    s4533087_QueueTimerTasks = xQueueCreate(10, sizeof(sendLeft));

    // Stores a value from 0 - 31
    unsigned char timerValue = 0;

    // Put stuff in the struct
    sendLeft.type = 'l';
    sendLeft.timerValue = timerValue;

    for(;;) {
        
        // Check if queue exists (queue takes a bit of time to be created)
        if (s4533087_QueueTimerTasks != NULL) {

            // Wait no longer than 10 ticks to put the message onto the queue
            if (xQueueSendToBack(s4533087_QueueTimerTasks, (void*)&sendLeft, 
                    (portTickType)10) != pdPASS) {
                
                // Failed to put the item in the queue
                continue;
            }
            // Check if we need to reset counter
            if (timerValue < 31) {
                
                timerValue++;
            } else {
            
                // Timer value = 31, we need to reset to 0
                timerValue = 0;
            }
            sendLeft.timerValue = timerValue;
        }
    	// Wait for 1000ms
    	vTaskDelay(1000);
    }
}

/**
  * @brief  Timer task for right timer, ticks every 0.1 seconds
  * @param  None
  * @retval None
  */
void s4533087_TaskTimerRight(void) {

    DualTimerMsg sendRight;
    s4533087_QueueTimerTasks = xQueueCreate(10, sizeof(sendRight));

    // Stores a value from 0 - 31
    unsigned char timerValue = 0;

    // Put things in the struct
    sendRight.type = 'r';
    sendRight.timerValue = timerValue;
    
    for(;;) {
        
        // Check if queue exists (queue takes a bit of time to be created)
        if (s4533087_QueueTimerTasks != NULL) {

            // Wait no longer than 10 ticks to put the message onto the queue
            if (xQueueSendToBack(s4533087_QueueTimerTasks, (void*)&sendRight, 
                    (portTickType)10) != pdPASS) {
                
                // Failed to put the item in the queue
                continue;
            }
            // Check if we need to reset counter
            if (timerValue < 31) {
                
                timerValue++;
            } else {
            
                // Timer value = 31, we need to reset to 0
                timerValue = 0;
            }
            sendRight.timerValue = timerValue;
        }
    	// Wait for 100ms
    	vTaskDelay(100);
    }
}

/**
  * @brief  Timer task for timer LED controller
  * @params None
  * @retval None
  */
void s4533087_TaskTimerDisplay(void) {

    DualTimerMsg message;

    for (;;) {
        if (s4533087_QueueTimerTasks != NULL) {

            if (xQueueReceive(s4533087_QueueTimerTasks, &message, 10)) {

                // Check if message received was for left or right timer
                if (message.type == 'l') {
                
                    // Left timer
                    portENTER_CRITICAL();
                    s4533087_os_lta1000g_write_value(message.timerValue, 'l');
                    portEXIT_CRITICAL();
                } else {
                
                    // Right timer
                    portENTER_CRITICAL();
                    s4533087_os_lta1000g_write_value(message.timerValue, 'r');
                    portEXIT_CRITICAL();
                }
            }
        }
        vTaskDelay(1);
    }
}

/**
  * @brief  Task to control start/stop from incoming interrupt form joystick
  * @params None
  * @retval None
  */
void s4533087_TaskTimerSwitchInterrupt(void) {

    for(;;) {
        if (s4533087_SemaphoreJoystickSwitchRaw != NULL) {
           
            // Try and get the semaphore, wait at most 10 ticks
            if (xSemaphoreTake(s4533087_SemaphoreJoystickSwitchRaw, 10) == 
                    pdTRUE) {

                // Try to get another semaphore in 10ms (DEBOUNCE_PERIOD=10ms), 
                // meaning there has been another button press. This will take
                // 10ms, an adequate debouncing period
                while (xSemaphoreTake(s4533087_SemaphoreJoystickSwitchRaw,
                        DEBOUNCE_PERIOD) == pdTRUE) {
                }
                // Now we've got the last semaphore for a while, check if the
                // switch is still pressed
                if (s4533087_os_joystick_read_switch_logic()) {

                    // Switch is still pressed, meaning we have a valid press
                    paused = !paused;
                    if (paused) {
                        
                        // Suspend tasks
                        vTaskSuspend(taskTimerLeftHandle);
                        vTaskSuspend(taskTimerRightHandle);
                    } else {
                        
                        // Resume tasks
                        vTaskResume(taskTimerLeftHandle);
                        vTaskResume(taskTimerRightHandle);
                    }
                }
            } else {

                // Didn't get semaphore, just do nothing
            }
        } else {

            // There's no semaphore
        }
        vTaskDelay(5);
    }
}

/**
  * @brief  Hardware Initialisation.
  * @param  None
  * @retval None
  */
static void Hardware_init( void ) {

	GPIO_InitTypeDef GPIO_InitStructure;

    // Disable interrupts
	portDISABLE_INTERRUPTS();	

    s4533087_os_lta1000g_init();
    s4533087_os_joystick_init();

    // Enable interrupts
	portENABLE_INTERRUPTS();

}

/**
  * @brief  Application Tick Task.
  * @param  None
  * @retval None
  */
void vApplicationTickHook( void ) {

}

/**
  * @brief  Idle Application Task (Disabled)
  * @param  None
  * @retval None
  */
void vApplicationIdleHook( void ) {
	static portTickType xLastTx = 0;

	for (;;) {
		if ((xTaskGetTickCount() - xLastTx ) > (1000 / portTICK_RATE_MS)) {
			xLastTx = xTaskGetTickCount();

		}
	}
}

/**
  * @brief  vApplicationStackOverflowHook
  * @param  Task Handler and Task Name
  * @retval None
  */
void vApplicationStackOverflowHook(xTaskHandle pxTask, 
        signed char *pcTaskName ) {

	/* This function will get called if a task overflows its stack.   If the
	parameters are corrupt then inspect pxCurrentTCB to find which was the
	offending task. */

	(void)pxTask;
	(void)pcTaskName;

	for( ;; );
}

