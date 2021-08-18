/**
  *****************************************************************************
  * @file    s4533087_os_cag_joystick.c
  * @author  Alexander FitzGerald - s4533087
  * @date    16052020
  * @brief   Alphanumeric keypad OS CAG driver
  *****************************************************************************
  * EXTERNAL FUNCTIONS
  *****************************************************************************
  * s4533087_os_cag_joystick()
  *****************************************************************************
  */

/* Includes ---------------------------------------------------------------- */
#include "board.h"
#include "processor_hal.h"
#include "debug_printf.h"

// FreeRTOS includes
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"

// Mylib includes
#include "s4533087_os_joystick.h"
#include "s4533087_os_cag_joystick.h"
#include "s4533087_os_cag_globals.h"

/* Private typedef --------------------------------------------------------- */
/* Private define ---------------------------------------------------------- */
// Task priorities
#define JOYSTICK_DEFAULT_TASK_PRIORITY     (tskIDLE_PRIORITY + 2)

// Task stack allocations
#define JOYSTICK_DEFAULT_STACK_SIZE        (configMINIMAL_STACK_SIZE * 4)

/* Private macro ----------------------------------------------------------- */
/* Private variables ------------------------------------------------------- */
/* Private function prototypes --------------------------------------------- */

/**
  * @brief  Task for monitoring joystick and sending relevant signals
  * @params None
  * @retval None
  */
void s4533087_os_cag_joystick(void) {

    // Wait a bit for required inits
    vTaskDelay(1);
    
    s4533087_os_joystick_init();

    uint16_t xValue;
    uint16_t xValueBin;
    uint16_t yValue;
    uint16_t yValueBin;
    uint8_t  zValue;
    
    // Wait a bit more for things to properly get set up
    vTaskDelay(10);

    for (;;) {
    
        xValue = s4533087_os_joystick_read_x();
        yValue = s4533087_os_joystick_read_y();
        zValue = s4533087_os_joystick_read_switch_logic();

        // Calculate relevant "bin" for x and y values. 
        // X value translated from 0 - 4095 into bins 0 - 7
        // Y value translated from 0 - 4095 into bins 0 - 4
        xValueBin = (uint16_t)((((float)xValue) / ((float)512)));
        yValueBin = (uint16_t)((((float)yValue) / ((float)819.2)));

        xEventGroupClearBits(s4533087_EventSimulatorColour, 
                COLOUR_CONTROL_EVENT);

        // Switch on x value to set relevant colour
        switch (xValueBin) {

            case 0:
                
                // Set colour to black
                xEventGroupSetBits(s4533087_EventSimulatorColour, 
                        EVENT_COLOUR_BLACK);
                break;
            
            case 1:
                
                // Set colour to red
                xEventGroupSetBits(s4533087_EventSimulatorColour, 
                        EVENT_COLOUR_RED);
               break;
            
            case 2:
                
                // Set colour to green
                xEventGroupSetBits(s4533087_EventSimulatorColour, 
                        EVENT_COLOUR_GREEN);
              break;
            
            case 3:
                
                // Set colour to yellow
                xEventGroupSetBits(s4533087_EventSimulatorColour, 
                        EVENT_COLOUR_YELLOW);
               break;
            
            case 4:
                
                // Set colour to blue (median value)
                xEventGroupSetBits(s4533087_EventSimulatorColour, 
                        EVENT_COLOUR_BLUE);
               break;
            
            case 5:
                
                // Set colour to magenta
                xEventGroupSetBits(s4533087_EventSimulatorColour, 
                        EVENT_COLOUR_MAGENTA);
               break;
            
            case 6:
                
                // Set colour to cyan
                xEventGroupSetBits(s4533087_EventSimulatorColour, 
                        EVENT_COLOUR_CYAN);
               break;
            
            case 7:
                
                // Set colour to white
                xEventGroupSetBits(s4533087_EventSimulatorColour, 
                        EVENT_COLOUR_WHITE);
               break;
            
            default:
                
                // For default, we'll just choose blue (the median colour)
                xEventGroupSetBits(s4533087_EventSimulatorColour, 
                        EVENT_COLOUR_BLUE);
                break;
        }        

        switch (yValueBin) {

            case 0:
                
                // Set refresh time = 0.5s
                xEventGroupSetBits(s4533087_EventSimulatorAction,
                        EVENT_SIMULATOR_TIME_0_5);
                break;
            
            case 1:
                
                // Set refresh time = 1s
                xEventGroupSetBits(s4533087_EventSimulatorAction,
                        EVENT_SIMULATOR_TIME_1);
                break;
            
            case 2:
                
                // Set refresh time = 2s (median value)
                xEventGroupSetBits(s4533087_EventSimulatorAction,
                        EVENT_SIMULATOR_TIME_2);
                break;
            
            case 3:
                
                // Set refresh time = 5s
                xEventGroupSetBits(s4533087_EventSimulatorAction,
                        EVENT_SIMULATOR_TIME_5);
                break;
            
            case 4:
                
                // Set refresh time = 10s
                xEventGroupSetBits(s4533087_EventSimulatorAction,
                        EVENT_SIMULATOR_TIME_10);
                break;
            
            default:
                
                // For default, we'll just choose 2s (median value)
                xEventGroupSetBits(s4533087_EventSimulatorAction,
                        EVENT_SIMULATOR_TIME_2);
                break;
        }

        // If the switch is pressed, set "grid clear" event bit
        if (s4533087_os_joystick_read_switch_logic()) {
        
            xEventGroupSetBits(s4533087_EventSimulatorAction, 
                    EVENT_SIMULATOR_CLEAR_GRID);
        }

        vTaskDelay(10);
    }
}
