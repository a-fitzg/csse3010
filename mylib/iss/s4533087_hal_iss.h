/**
  *****************************************************************************
  * @file       s4533087_hal_iss.h
  * @author     Alexander FitzGerald - s4533087
  * @date       06032020
  * @brief      Input Signal Synchroniser HAL driver header
  *****************************************************************************
  * EXTERNAL FUNCTIONS
  *****************************************************************************
  * s4533087_hal_iss_init() - Initialise ISS counters
  * s4533087_hal_iss_synchroniser() - Synchronise input signal
  * s4533087_hal_iss_eventcounter_read() - Return specified counter value
  * s4533087_hal_iss_lasttimer_read() - Returns specified last time value
  * s4533087_hal_iss_eventcounter_reset() - Reset specified counter value
  * s4533087_hal_iss_lasttimer_reset() - Reset specified last time value
  * s4533087_hal_iss_delaytimer_ctrl() - Set delay value for specified source
  ****************************************************************************
  */

#ifndef S4533087_HAL_ISS_H
#define S4533087_HAL_ISS_H

#define S4533087_HAL_ISS_SOURCE_1 0
#define S4533087_HAL_ISS_SOURCE_2 1
#define S4533087_HAL_ISS_SOURCE_3 2


#define SIGNAL_SOURCES 3

// Function prototypes
void s4533087_hal_iss_init(void);
void s4533087_hal_iss_synchroniser(unsigned char signal_source_index);
uint32_t s4533087_hal_iss_eventcounter_read(unsigned char signal_source_index);
uint32_t s4533087_hal_iss_lasttimer_read(unsigned char signal_source_index);
void s4533087_hal_iss_eventcounter_reset(unsigned char signal_source_index);
void s4533087_hal_iss_lasttimer_reset(unsigned char signal_source_index);
void s4533087_hal_iss_delaytimer_ctrl(unsigned char signal_source_index, 
        int delay_value);

#endif
