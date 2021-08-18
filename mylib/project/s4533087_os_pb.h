/**
  *********************************************************
  * @file       s4533087_os_pb.h
  * @author     Alexander FitzGerald - s4533087
  * @date       06032020
  * @brief      Pushbutton OS driver header
  *********************************************************
  * EXTERNAL FUNCTIONS
  *********************************************************
  * s4533087_os_pb_on_init()
  * s4533087_os_pb_on_deinit()
  * s4533087_os_pb_read_button()
  *********************************************************
  */

#ifndef S4533087_OS_PB_H
#define S4533087_OS_PB_H

//#include "s4533087_hal_pb.h"

#include "FreeRTOS.h"
#include "semphr.h"

// Semaphore for button press
SemaphoreHandle_t s4533087_SemaphoreButtonPress;

// Function prototypes
void s4533087_os_pb_on_init(void);
void s4533087_os_pb_on_deinit(void);
uint8_t s4533087_os_pb_read_button(void);

#endif
