/**
  *********************************************************
  * @file       s4533087_os_cag_display.c
  * @author     Alexander FitzGerald - s4533087
  * @date       13052020
  * @brief      Display CAG driver source
  *********************************************************
  * EXTERNAL FUNCTIONS
  *********************************************************
  * s4533087_os_cag_display()
  * s4533087_os_cag_display_colour_controller()
  * s4533087_os_cag_display_action_controller()
  *********************************************************
  */

/* Includes -------------------------------------------- */
#include <string.h>

// STM32 includes
#include "board.h"
#include "processor_hal.h"
#include "debug_printf.h"

// FreeRTOS includes
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
#include "event_groups.h"

// Mylib includes
#include "s4533087_os_cag_display.h"
#include "s4533087_os_cag_joystick.h"
#include "s4533087_os_lta1000g.h"
#include "s4533087_os_cag_globals.h"

/* Private typedef ------------------------------------- */
/* Private define -------------------------------------- */
#define COLOUR_BUFFER_SIZE  15

/* Private macro --------------------------------------- */
/* Private variables ----------------------------------- */
// String for printing cells of a colour
char* aliveColour;
char* deadColour;

// Temporary cell colours for alive and dead cells (for use in display task)
char* tempAliveColour;
char* tempDeadColour;

/* Private function prototypes ------------------------- */
void clearGrid(uint8_t**);
void uart_print(char*);

/**
  * @brief  Main task for the display
  * @params None
  * @retval None
  */
void s4533087_os_cag_display(void) {

    s4533087_os_lta1000g_init();

    // Allocate space for colour strings
    aliveColour     = pvPortMalloc(COLOUR_BUFFER_SIZE * sizeof(char));
    deadColour      = pvPortMalloc(COLOUR_BUFFER_SIZE * sizeof(char));
    tempAliveColour = pvPortMalloc(COLOUR_BUFFER_SIZE * sizeof(char));
    tempDeadColour  = pvPortMalloc(COLOUR_BUFFER_SIZE * sizeof(char));
   
    // Set default colours to black background, blue foreground
    taskENTER_CRITICAL();
    strcpy(aliveColour, CELL_BLUE);
    strcpy(deadColour, CELL_BLACK);
    taskEXIT_CRITICAL();

    // Initialise game board, allocate memory
    s4533087_gameGrid = pvPortMalloc(GRID_Y_SIZE * sizeof(uint8_t*));
    for (uint8_t i = 0; i < GRID_Y_SIZE; i++) {
        s4533087_gameGrid[i] = pvPortMalloc(GRID_X_SIZE * sizeof(uint8_t));
    }

    // Start the grid with 0s everywhere (start wiht every cell dead)
    for (uint8_t row = 0; row < GRID_Y_SIZE; row++) {
        
        for (uint8_t column = 0; column < GRID_X_SIZE; column++) {       

            s4533087_gameGrid[row][column] = 0;
        }
    }
    
    for (;;) {
        
        if (s4533087_SemaphoreDisplayRefresh != NULL) {
            
            if (xSemaphoreTake(s4533087_SemaphoreDisplayRefresh, 10) == 
                    pdTRUE) {

                SCREEN_CLEAR();
                SCREEN_HOME();
                
                s4533087_os_lta1000g_toggle_bit_9();
                
                taskENTER_CRITICAL();
                strcpy(tempAliveColour, aliveColour);
                strcpy(tempDeadColour, deadColour);
                taskEXIT_CRITICAL();

                // Make temporary grid, to avoid getting interrupted mid-print
                uint8_t** tempGrid = pvPortMalloc(GRID_Y_SIZE * 
                        sizeof(uint8_t*));
                for (uint8_t i = 0; i < GRID_Y_SIZE; i++) {
                    tempGrid[i] = pvPortMalloc(GRID_X_SIZE * sizeof(uint8_t));
                }
                
                taskENTER_CRITICAL();
                for (uint8_t y = 0; y < GRID_Y_SIZE; y++) {
                    
                    for (uint8_t x = 0; x < GRID_X_SIZE; x++) {
                        tempGrid[y][x] = s4533087_gameGrid[y][x];
                    }
                }
                taskEXIT_CRITICAL();

                // Draw out grid
                // Iterate over rows
                for (uint8_t row = 0; row < GRID_Y_SIZE; row++) {
                    
                    // Each row is repeated "CELL_SIZE" times
                    for (uint8_t yRepeat = 0; yRepeat < CELL_SIZE; yRepeat++) {

                        // Iterate over columns
                        for (uint8_t column = 0; column < GRID_X_SIZE; 
                                column++) {
                        
                            // Like above, columns repeated "CELL_SIZE" times
                            for (uint8_t xRepeat = 0; xRepeat < CELL_SIZE; 
                                    xRepeat++) {
        
                                // If cell alive, print alive colour 
                                // If cell dead, print dead colour
                                if (tempGrid[row][column]) {

                                    uart_print(tempAliveColour);
                                } else {

                                    uart_print(tempDeadColour);
                                }
                                uart_print("  ");
                            }
                        }
                        uart_print("\r\n");
                    }
                }
                // Set back to black, so it doesn't colour the whole screen
                SET_CELL_COLOUR(FG_BLACK, BG_BLACK);   
                
                // Free our temporay board
                for (uint8_t i = 0; i < GRID_Y_SIZE; i++) {
                    vPortFree(tempGrid[i]);
                }
                vPortFree(tempGrid);
            }
        }
        vTaskDelay(1);
    }
}

/**
  * @brief  Task to control display's colour
  * @params None
  * @retval None
  */
void s4533087_os_cag_display_colour_controller(void) {

    // Wait for initialisations to be done
    vTaskDelay(1);

    // Done, now we can get to work
    EventBits_t colourBits;

    s4533087_EventSimulatorColour = xEventGroupCreate();

    for (;;) {

        // Wait on colour bits (these control the cell colours)
        colourBits = xEventGroupWaitBits(s4533087_EventSimulatorColour, 
                COLOUR_CONTROL_EVENT, pdTRUE, pdFALSE, 5);
 
        // Set cell colour in accordance with colour event bits
        // Assumes only one bit is set at a time. PLEASE ENSURE THIS.
        switch (colourBits) {
            
            case EVENT_COLOUR_BLACK:
                
                // Set alive cell colour to black
                strcpy(aliveColour, CELL_BLACK);
                colourBits = xEventGroupClearBits(
                        s4533087_EventSimulatorColour, EVENT_COLOUR_BLACK);
                break;

            case EVENT_COLOUR_RED:
                
                // Set alive cell colour to black
                strcpy(aliveColour, CELL_RED);
                colourBits = xEventGroupClearBits(
                        s4533087_EventSimulatorColour, EVENT_COLOUR_RED);
                break;

            case EVENT_COLOUR_GREEN:
                
                // Set alive cell colour to black
                strcpy(aliveColour, CELL_GREEN);
                colourBits = xEventGroupClearBits(
                        s4533087_EventSimulatorColour, EVENT_COLOUR_GREEN);
                break;

            case EVENT_COLOUR_YELLOW:
                
                // Set alive cell colour to black
                strcpy(aliveColour, CELL_YELLOW);
                colourBits = xEventGroupClearBits(
                        s4533087_EventSimulatorColour, EVENT_COLOUR_YELLOW);
                break;

            case EVENT_COLOUR_BLUE:
                
                // Set alive cell colour to black
                strcpy(aliveColour, CELL_BLUE);
                colourBits = xEventGroupClearBits(
                        s4533087_EventSimulatorColour, EVENT_COLOUR_BLUE);
                break;

            case EVENT_COLOUR_MAGENTA:
                
                // Set alive cell colour to black
                strcpy(aliveColour, CELL_MAGENTA);
                colourBits = xEventGroupClearBits(
                        s4533087_EventSimulatorColour, EVENT_COLOUR_MAGENTA);
                break;

            case EVENT_COLOUR_CYAN:
                
                // Set alive cell colour to black
                strcpy(aliveColour, CELL_CYAN);
                colourBits = xEventGroupClearBits(
                        s4533087_EventSimulatorColour, EVENT_COLOUR_CYAN);
                break;

            case EVENT_COLOUR_WHITE:
                
                // Set alive cell colour to black
                strcpy(aliveColour, CELL_WHITE);
                colourBits = xEventGroupClearBits(
                        s4533087_EventSimulatorColour, EVENT_COLOUR_WHITE);
                break;
            
            default:
                
                // Must have no bits set, or multiple bits set at a time
                // Clear all bits in case there are multiple set
                strcpy(aliveColour, CELL_BLUE);
                colourBits = xEventGroupClearBits(
                        s4533087_EventSimulatorColour, COLOUR_CONTROL_EVENT);
                break;
        }
        vTaskDelay(1);
    }   
}


/**
  * @brief  Task to control display functions (i.e. stop, clear, etc.)
  * @params None
  * @retval None
  */
void s4533087_os_cag_display_action_controller(void) {

    // Wait for necessary inits to complete
    vTaskDelay(10);

    EventBits_t actionBits;
    s4533087_EventSimulatorAction = xEventGroupCreate();

    for (;;) {

        // Wait on action bits (these control the simulation)
        actionBits = xEventGroupWaitBits(s4533087_EventSimulatorAction, 
                SIMULATOR_CONTROL_EVENT, pdTRUE, pdFALSE, 5);

        // Check for any time change bits
        switch (actionBits) {
        
            case EVENT_SIMULATOR_TIME_0_5:
            
                // Change update time to 0.5s
                s4533087_displayUpdateTime = 500;
                actionBits = xEventGroupClearBits(s4533087_EventSimulatorAction, 
                        EVENT_SIMULATOR_TIME_0_5);
                break;
    
            case EVENT_SIMULATOR_TIME_1:
    
                // Change update time to 1s
                s4533087_displayUpdateTime = 1000;
                actionBits = xEventGroupClearBits(s4533087_EventSimulatorAction, 
                        EVENT_SIMULATOR_TIME_1);
                break;
    
            case EVENT_SIMULATOR_TIME_2:
    
                // Change update time to 2s
                s4533087_displayUpdateTime = 2000;
                actionBits = xEventGroupClearBits(s4533087_EventSimulatorAction, 
                        EVENT_SIMULATOR_TIME_2);
                break;
    
            case EVENT_SIMULATOR_TIME_5:
                
                // Change update time to 5s
                s4533087_displayUpdateTime = 5000;
                actionBits = xEventGroupClearBits(s4533087_EventSimulatorAction, 
                        EVENT_SIMULATOR_TIME_5);
                break;
    
            case EVENT_SIMULATOR_TIME_10:
            
                // Change update time to 10s
                s4533087_displayUpdateTime = 10000;
                actionBits = xEventGroupClearBits(s4533087_EventSimulatorAction, 
                        EVENT_SIMULATOR_TIME_10);
                break;

            default:

                // No time changes
                break;
        }

        // And check for the other bits. Thesse are not done within the switch
        // block as we want these to be able to occur simultaneously
        if (!!(actionBits & EVENT_SIMULATOR_CLEAR_GRID)) {
                
            // Clear the grid
            clearGrid(s4533087_gameGrid);
            actionBits = xEventGroupClearBits(s4533087_EventSimulatorAction,
                    EVENT_SIMULATOR_CLEAR_GRID);
        }

        if (!!(actionBits & EVENT_SIMULATOR_START)) {
            
            // Resume the simulation
            #ifdef LEGACY_PAUSE    
            if (s4533087_SemaphoreGameResume != NULL) {
                
                xSemaphoreGive(s4533087_SemaphoreGameResume);
            }
            actionBits = xEventGroupClearBits(s4533087_EventSimulatorAction,
                    EVENT_SIMULATOR_START);
            #else
            // First, suspend the alt ticker
            vTaskSuspend(s4533087_taskSimulatorAltRefHandle);
            vTaskDelay(1);

            // Then, resume the simulator task
            vTaskResume(s4533087_taskSimulatorHandle);
            #endif
    
        }
            
        if (!!(actionBits & EVENT_SIMULATOR_STOP)) {
                
            // Pause the simulation
            #ifdef LEGACY_PAUSE
            if (s4533087_SemaphoreGamePause != NULL) {
                    
                xSemaphoreGive(s4533087_SemaphoreGamePause);
            }
            actionBits = xEventGroupClearBits(s4533087_EventSimulatorAction,
                    EVENT_SIMULATOR_STOP);
            #else
            // First, suspend the simulator
            vTaskSuspend(s4533087_taskSimulatorHandle);
            vTaskDelay(1);

            // Then, resume the alt ticker
            vTaskResume(s4533087_taskSimulatorAltRefHandle);           
            #endif
        }
    }
}

/**
  * @brief  Clears a nominated grid 
  * @params grid: A grid to clear. Assuming it is GRID_X_SIZE x GRID_Y_SIZE
  * @retval None
  */
void clearGrid(uint8_t** grid) {

    for (uint8_t y = 0; y < GRID_Y_SIZE; y++) {
        
        for (uint8_t x = 0; x < GRID_X_SIZE; x++) {
            
            grid[y][x] = 0;
        }
    }
}

/**
  * @brief  Provides an alternative implementation of debug_printf
  * @params string: String to print
  * @retval None
  */
void uart_print(char* string) {
    
    portENTER_CRITICAL();
    for (uint8_t i = 0; string[i]; i++) {

        BRD_debuguart_putc(string[i]);
    }
    portEXIT_CRITICAL();
}
