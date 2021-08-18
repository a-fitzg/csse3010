/**
  *****************************************************************************
  * @file   s4533087_os_cag_globals.h
  * @author Alexander FitzGerald - s4533087
  * @date   28052020
  * @brief  Global variables for CAG functions
  *****************************************************************************
  */

#ifndef S4533087_OS_CAG_GLOBALS_H
#define S4533087_OS_CAG_GLOBALS_H

#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
#include "event_groups.h"


// Simulator event group
#define EVENT_SIMULATOR_CLEAR_GRID  (1 << 0)
#define EVENT_SIMULATOR_START       (1 << 1)
#define EVENT_SIMULATOR_STOP        (1 << 2)
#define EVENT_SIMULATOR_TIME_0_5    (1 << 3)
#define EVENT_SIMULATOR_TIME_1      (1 << 4)
#define EVENT_SIMULATOR_TIME_2      (1 << 5)
#define EVENT_SIMULATOR_TIME_5      (1 << 6)
#define EVENT_SIMULATOR_TIME_10     (1 << 7)
#define SIMULATOR_CONTROL_EVENT    (EVENT_SIMULATOR_CLEAR_GRID      | \
                                    EVENT_SIMULATOR_START           | \
                                    EVENT_SIMULATOR_STOP            | \
                                    EVENT_SIMULATOR_TIME_0_5        | \
                                    EVENT_SIMULATOR_TIME_1          | \
                                    EVENT_SIMULATOR_TIME_2          | \
                                    EVENT_SIMULATOR_TIME_5          | \
                                    EVENT_SIMULATOR_TIME_10)

// Cell colour event group
#define EVENT_COLOUR_BLACK          (1 << 0)
#define EVENT_COLOUR_RED            (1 << 1)
#define EVENT_COLOUR_GREEN          (1 << 2)
#define EVENT_COLOUR_YELLOW         (1 << 3)
#define EVENT_COLOUR_BLUE           (1 << 4)
#define EVENT_COLOUR_MAGENTA        (1 << 5)
#define EVENT_COLOUR_CYAN           (1 << 6)
#define EVENT_COLOUR_WHITE          (1 << 7)
#define COLOUR_CONTROL_EVENT       (EVENT_COLOUR_BLACK      | \
                                    EVENT_COLOUR_RED        | \
                                    EVENT_COLOUR_GREEN      | \
                                    EVENT_COLOUR_YELLOW     | \
                                    EVENT_COLOUR_BLUE       | \
                                    EVENT_COLOUR_MAGENTA    | \
                                    EVENT_COLOUR_CYAN       | \
                                    EVENT_COLOUR_WHITE)

// Event group for simulator actions
EventGroupHandle_t s4533087_EventSimulatorAction;

// Event group for simulator cell colours
EventGroupHandle_t s4533087_EventSimulatorColour;

// Task handles
TaskHandle_t s4533087_taskKeypadMasterHandle;
TaskHandle_t s4533087_taskKeypadGridHandle;
TaskHandle_t s4533087_taskKeypadMnemonicHandle;
TaskHandle_t s4533087_taskDisplayHandle;
TaskHandle_t s4533087_taskDisplayColourHandle;
TaskHandle_t s4533087_taskDisplayActionHandle;
TaskHandle_t s4533087_taskSimulatorHandle;
TaskHandle_t s4533087_taskJoystickHandle;
TaskHandle_t s4533087_taskSimulatorAltRefHandle;

#endif
