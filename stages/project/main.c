/**
  *****************************************************************************
  * @file    project/main.c
  * @author  Alexander FitzGerald - 45330874
  * @date    08052020
  * @brief   CSSE3010 Project (Project 1)
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
#include "s4533087_os_cag_keypad.h"
#include "s4533087_os_cag_display.h"
#include "s4533087_os_cag_simulator.h"
#include "s4533087_os_cag_joystick.h"
#include "s4533087_os_cag_globals.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
// Task priority definitions
#define DEFAULT_TASK_PRIORITY   (tskIDLE_PRIORITY + 2)

// Task stack size definitions
#define DEFAULT_STACK_SIZE      (configMINIMAL_STACK_SIZE * 8)

/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/

/**
  * @brief  Starts all the other tasks, then starts the scheduler.
  * @param  None
  * @retval None
  */
int main(void) {

    // Initialise necessary hardware for basic functionality
    BRD_init();

	// Start all required tasks
    xTaskCreate((void*)&s4533087_os_keypad_master, 
            (const signed char*)"TaskKeypadMaster", DEFAULT_STACK_SIZE, 
            NULL, DEFAULT_TASK_PRIORITY, &s4533087_taskKeypadMasterHandle);

    xTaskCreate((void*)&s4533087_os_keypad_grid, 
            (const signed char*)"TaskKeypadGrid", DEFAULT_STACK_SIZE,
            NULL, DEFAULT_TASK_PRIORITY, &s4533087_taskKeypadGridHandle);

    xTaskCreate((void*)&s4533087_os_keypad_mnemonic,
            (const signed char*)"TaskKeypadMnemonic", DEFAULT_STACK_SIZE,
            NULL, DEFAULT_TASK_PRIORITY, &s4533087_taskKeypadMnemonicHandle);

    xTaskCreate((void*)&s4533087_os_cag_display, 
            (const signed char*)"TaskDisplay", DEFAULT_STACK_SIZE,
            NULL, DEFAULT_TASK_PRIORITY, &s4533087_taskDisplayHandle);

    xTaskCreate((void*)&s4533087_os_cag_simulator, 
            (const signed char*)"TaskSimulator", DEFAULT_STACK_SIZE,
            NULL, DEFAULT_TASK_PRIORITY, &s4533087_taskSimulatorHandle);

    xTaskCreate((void*)&s4533087_os_cag_display_colour_controller, 
            (const signed char*)"TaskDisplayColour", DEFAULT_STACK_SIZE,
            NULL, DEFAULT_TASK_PRIORITY, &s4533087_taskDisplayColourHandle);

    xTaskCreate((void*)&s4533087_os_cag_display_action_controller, 
            (const signed char*)"TaskDisplayAction", DEFAULT_STACK_SIZE, 
            NULL, DEFAULT_TASK_PRIORITY, &s4533087_taskDisplayActionHandle);

    xTaskCreate((void*)&s4533087_os_cag_joystick,
            (const signed char*)"TaskJoystick", DEFAULT_STACK_SIZE, 
            NULL, DEFAULT_TASK_PRIORITY, &s4533087_taskJoystickHandle);    

    xTaskCreate((void*)&s4533087_os_cag_simulator_alt_refresher,
            (const signed char*)"TaskSimulatorAltRefresher", DEFAULT_STACK_SIZE,
            NULL, DEFAULT_TASK_PRIORITY, &s4533087_taskSimulatorAltRefHandle);

	// Start the scheduler.
	vTaskStartScheduler();
   
	// We should never get here as control is now taken by the scheduler.
  	return 0;
}
