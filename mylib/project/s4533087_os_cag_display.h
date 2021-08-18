/**
  *********************************************************
  * @file       s4533087_os_cag_display.h
  * @author     Alexander FitzGerald - s4533087
  * @date       13052020
  * @brief      Display CAG driver header
  *********************************************************
  * EXTERNAL FUNCTIONS
  *********************************************************
  * s4533087_os_cag_display()
  * s4533087_os_cag_display_colour_controller()
  * s4533087_os_cag_display_action_controller()
  *********************************************************
  */

#ifndef S4533087_OS_CAG_DISPLAY_H
#define S4533087_OS_CAG_DISPLAY_H

#include "board.h"
#include "debug_printf.h"
#include "processor_hal.h"

#include "FreeRTOS.h"
#include "semphr.h"
#include "queue.h"
#include "event_groups.h"

// Macros
// Clears the entire screen
#define SCREEN_CLEAR()          debug_printf("\e[2J")

// Moves the cursor to upper-left hand corner of the screen
// Typically used after SCREEN_CLEAR()
#define SCREEN_HOME()           debug_printf("\e[H")

// Sets cell foreground and background to specified colours
#define SET_CELL_COLOUR(fg, bg) debug_printf("\e[0;%d;%dm", fg, bg)

// Paused / resume state definitions
#define PAUSED      1
#define UNPAUSED    0

// COLOURS
// Foreground colours
#define FG_BLACK        30
#define FG_RED          31
#define FG_GREEN        32
#define FG_YELLOW       33
#define FG_BLUE         34
#define FG_MAGENTA      35
#define FG_CYAN         36
#define FG_WHITE        37

// Background colours
#define BG_BLACK        40
#define BG_RED          41
#define BG_GREEN        42
#define BG_YELLOW       43
#define BG_BLUE         44
#define BG_MAGENTA      45
#define BG_CYAN         46
#define BG_WHITE        47

// Cell colours
#define CELL_BLACK      "\e[7;30;40m"
#define CELL_RED        "\e[7;31;41m"
#define CELL_GREEN      "\e[7;32;42m"
#define CELL_YELLOW     "\e[7;33;43m"
#define CELL_BLUE       "\e[7;34;44m"
#define CELL_MAGENTA    "\e[7;35;45m"
#define CELL_CYAN       "\e[7;36;46m"
#define CELL_WHITE      "\e[7;37;47m"

#define GRID_X_SIZE     15
#define GRID_Y_SIZE     15
#define CELL_SIZE       2

// Definitions of driver controller event bits
#define EVENT_DRIVER_SIMULATOR_DELETE   (1 << 0)
#define EVENT_DRIVER_JOYSTICK_DELETE    (1 << 1)
#define EVENT_DRIVER_SIMULATOR_CREATE   (1 << 2)
#define EVENT_DRIVER_JOYSTICK_CREATE    (1 << 3)
#define DRIVER_CONTROL_EVENT           (EVENT_DRIVER_SIMULATOR_DELETE   | \
                                        EVENT_DRIVER_JOYSTICK_DELETE    | \
                                        EVENT_DRIVER_SIMULATOR_CREATE   | \
                                        EVENT_DRIVER_JOYSTICK_CREATE)


// Global variables
// 2D array representing game grid
uint8_t** s4533087_gameGrid;

// Stores update time
uint16_t s4533087_displayUpdateTime;

// Boolean for pause / resume
uint8_t s4533087_gamePaused;

// Message for displaying items on the screen
typedef struct caMessage {
    // Type - cell or lifeform
    uint8_t type;

    // Cell x position
    uint8_t cell_x;

    // Cell y position
    uint8_t cell_y;
} caMessage_t;

QueueHandle_t s4533087_QueueDisplayLife;

// Semaphore for refreshing display
SemaphoreHandle_t s4533087_SemaphoreDisplayRefresh;

// Semaphores for pausing / resuming display
SemaphoreHandle_t s4533087_SemaphoreGamePause;
SemaphoreHandle_t s4533087_SemaphoreGameResume;

EventGroupHandle_t s4533087_EventDriverController;

// Function prototypes
void s4533087_os_cag_display(void);
void s4533087_os_cag_display_colour_controller(void);
void s4533087_os_cag_display_action_controller(void);

#endif
