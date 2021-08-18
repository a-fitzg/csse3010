/**
  *********************************************************
  * @file       s4533087_hal_pb.h
  * @author     Alexander FitzGerald - s4533087
  * @date       06032020
  * @brief      Pushbutton HAL driver header
  *********************************************************
  * EXTERNAL FUNCTIONS
  *********************************************************
  * s4533087_hal_pb_on_init()
  * s4533087_hal_pb_on_isr()
  * s4533087_hal_pb_on_deinit()
  *********************************************************
  */

#ifndef S4533087_HAL_PB_H
#define S4533087_HAL_PB_H

// Function prototypes
void s4533087_hal_pb_on_init(void);
void s4533087_hal_pb_on_isr(void);
uint8_t s4533087_hal_pb_on_read(void);
void s4533087_hal_pb_on_deinit(void);

#endif
