/**
  *****************************************************************************
  * @file   s4533087_hal_joystick.h
  * @author Alexander FitzGerald - s4533087
  * @date   11032020
  * @brief  Joystick HAL driver header
  *****************************************************************************
  * EXTERNAL FUNCTIONS
  *****************************************************************************
  * s4533087_hal_joystick_init()
  * s4533087_hal_joystick_deinit()
  *****************************************************************************
  */

#ifndef S4533087_HAL_JOYSTICK_H
#define S4533087_HAL_JOYSTICK_H

// Variables
ADC_HandleTypeDef s4533087_AdcXHandle;
ADC_HandleTypeDef s4533087_AdcYHandle;
ADC_ChannelConfTypeDef AdcXChanConfig;
ADC_ChannelConfTypeDef AdcYChanConfig;

// Function prototypes
void s4533087_hal_joystick_init(void);
void s4533087_hal_joystick_deinit(void);

#endif
