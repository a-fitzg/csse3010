/**
  *********************************************************
  * @file       stage1/main.c
  * @author     Alexander FitzGerald - s4533087
  * @date       05032020
  * @brief      Stage 1 for CSSE3010
  *********************************************************
  */

/* Includes -------------------------------------------- */
#include "board.h"
#include "processor_hal.h"
#include "debug_printf.h"
#include "s4533087_hal_lta1000g.h"
#include "s4533087_hal_iss.h"

/* Private typedef ------------------------------------- */
/* Private define -------------------------------------- */
/* Private macro --------------------------------------- */
/* Private variables ----------------------------------- */
unsigned short ledbarValue;
unsigned short position;
/* Private function prototypes ------------------------- */

/**
  * @brief      Main program: Display values on an LedBar
  * @param      None
  * @retval     None
  */    
int main(void) {

    BRD_init();
    s4533087_hal_lta1000g_init();
    s4533087_hal_iss_init();

    ledbarValue = 0;
    position = 9;

    // 1 = to right, -1 = to left
    s4533087_hal_lta1000g_set_direction((signed char)1);

    // Main processing loop
    while(1) {
    }
}

