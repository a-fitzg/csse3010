/**
  *****************************************************************************
  * @file       stage2/main.c
  * @author     Alexander FitzGerald - s4533087
  * @date       05032020
  * @brief      Stage 2 for CSSE3010
  *****************************************************************************
  */

/* Includes ---------------------------------------------------------------- */
#include "board.h"
#include "processor_hal.h"
#include "debug_printf.h"
#include "s4533087_hal_atimer.h"
#include "s4533087_hal_joystick.h"
#include "s4533087_hal_pwm.h"
#include "s4533087_hal_lta1000g.h"

/* Private typedef --------------------------------------------------------- */
/* Private define ---------------------------------------------------------- */
/* Private macro ----------------------------------------------------------- */
/* Private variables ------------------------------------------------------- */
/* Private function prototypes --------------------------------------------- */

/**
  * @brief      Main
  * @param      None
  * @retval     None
  */    
int main(void) {

    BRD_init();
    s4533087_hal_lta1000g_init();
    s4533087_hal_atimer_init();
    s4533087_hal_atimer_clkspeed_set(25000);
    s4533087_hal_atimer_period_set(2);
    s4533087_hal_joystick_init();
    s4533087_hal_pwm_init();

    unsigned int xValue;
    unsigned int yValue;
    unsigned int sValue;
    unsigned int intermediateValue;
    unsigned int ledValue;

    char command = 0;

    // Main processing loop
    while(1) {
      
        // X value conversion
        HAL_ADC_Start(&AdcXHandle);
        while (HAL_ADC_PollForConversion(&AdcXHandle, 10) != HAL_OK);
        xValue = (uint16_t)(HAL_ADC_GetValue(&AdcXHandle));
        
        // Y value conversion
        HAL_ADC_Start(&AdcYHandle);
        while (HAL_ADC_PollForConversion(&AdcYHandle, 10) != HAL_OK);
        yValue = (uint16_t)(HAL_ADC_GetValue(&AdcYHandle));

        // Button conversion
        sValue = 1 - HAL_GPIO_ReadPin(BRD_A3_GPIO_PORT, BRD_A3_PIN);

        // ************************* Y VALUE **********************************
        // Converts y value full scale to a 0 - 10 value.
        // Weird "magic numbers" were obtained through trial and error because
        // they map the joystick value to a 0 - 10 scale most linearly
        intermediateValue = (int)(((double)yValue + 50) / (double)409.5);

        switch (intermediateValue) {

            case 0:
                // 0 bars
                ledValue = 0;
                break;
            
            case 1:
                // 1 bar
                ledValue = 0x0001;
                break;
            
            case 2:
                // 2 bars
                ledValue = 0x0003;
                break;
            
            case 3:
                // 3 bars
                ledValue = 0x0007;
                break;
            
            case 4:
                // 4 bars
                ledValue = 0x000F;
                break;
            
            case 5:
                // 5 bars
                ledValue = 0x001F;
                break;
            
            case 6:
                // 6 bars
                ledValue = 0x003F;
                break;
            
            case 7:
                // 7 bars
                ledValue = 0x007F;
                break;
            
            case 8:
                // 8 bars
                ledValue = 0x00FF;
                break;
            
            case 9:
                // 9 bars
                ledValue = 0x01FF;
                break;
            
            case 10:
                // 10 bars
                ledValue = 0x03FF;
                break;
            
            default:
                ledValue = 0;
                break;

        }

        // If no press read from joystick, display value
        // else, clear the leds
        if (sValue == 0) {

            s4533087_hal_lta1000g_write(ledValue);
        } else {
            
            s4533087_hal_lta1000g_write(0);
        }

        // ************************* X VALUE **********************************
        S4533087_HAL_PWM_DC_SET((int)(((double)xValue / (double)4095) * 
                (double)S4533087_HAL_PWM_PERIOD));

        // Process terminal commands for atimer
        command = debug_getc();
        if (command != '\0') {
            
            switch (command) {
                
                case 'f':
                    s4533087_hal_atimer_timer_pause();
                    break;
                
                case 'r':
                    s4533087_hal_atimer_timer_resume();
                    break;

                case 'z':
                    s4533087_hal_atimer_timer_reset();
                    break;
                
                case 'c':
                    s4533087_hal_atimer_timer_printticks();
                    break;
                
                case 't':
                    s4533087_hal_atimer_timer_printms();
                    break;
                    
                case '+':
                    s4533087_hal_atimer_timer_addperiod(10);
                    break;
                
                case '-':
                    s4533087_hal_atimer_timer_addperiod(-10);
                    break;
                    
                case 'i':
                    s4533087_hal_atimer_timer_addfrequency(1000);
                    break;
                
                case 'd':
                    s4533087_hal_atimer_timer_addfrequency(-1000);
                    break;
                
                case 'm':
                    // Metronome, future feature
                    break;
            }
        }
        
        // Prevents terminal character reading from doing weird things
        HAL_Delay(1);
        
    }
}
