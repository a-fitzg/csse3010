/**
  *****************************************************************************
  * @file    s4533087_hal_keypad.c
  * @author  Alexander FitzGerald - s4533087
  * @date    07042020
  * @brief   Alphanumeric keypad HAL driver
  *****************************************************************************
  * EXTERNAL FUNCTIONS
  *****************************************************************************
  * void s4533087_hal_keypad_init()
  * void s4533087_hal_keypad_deinit()
  * void s4533087_hal_keypad_fsmprocessing()
  * int s4533087_hal_keypad_read_status()
  * uint8_t s4533087_hal_keypad_read_key()
  * char s4533087_hal_keypad_read_ascii()
  *****************************************************************************
  */

/* Includes ---------------------------------------------------------------- */
#include "board.h"
#include "processor_hal.h"
#include "debug_printf.h"

#include "FreeRTOS.h"
#include "task.h"

#include "s4533087_hal_keypad.h"

/* Private typedef --------------------------------------------------------- */
/* Private define ---------------------------------------------------------- */
#define INIT_STATE      0
#define COL1_STATE      1
#define COL2_STATE      2
#define COL3_STATE      3
#define COL4_STATE      4
#define ROWSCAN_STATE   5
#define DEINIT_STATE    6

/* Private macro ----------------------------------------------------------- */
#define keypad_col1()       keypad_writecol(0x01)
#define keypad_col2()       keypad_writecol(0x02)
#define keypad_col3()       keypad_writecol(0x04)
#define keypad_col4()       keypad_writecol(0x08)

/* Private variables ------------------------------------------------------- */
int KeypadFsmCurrentstate   = INIT_STATE;
int KeypadStatus            = 0;
unsigned char KeypadValue   = 0x00;
uint8_t coords              = 0;
uint8_t colValue            = 0;
uint8_t rowValue            = 0;
uint8_t triggered           = 0;
uint8_t triggeringColumn    = 0;
uint8_t valueQueue          = 0xFF;
unsigned char asciiQueue    = 0;
GPIO_InitTypeDef GPIO_InitStructure;

/* Private function prototypes --------------------------------------------- */
void keypad_gpio_init(void);
void keypad_gpio_deinit(void);
void keypad_writecol(uint8_t);
uint8_t keypad_readrow(void);
void decode_coords();

/**
  * @brief  Initialise keypad
  * @params None
  * @retval None
  */
void s4533087_hal_keypad_init(void) {

    KeypadFsmCurrentstate = INIT_STATE;
    keypad_gpio_init();
}

/**
  * @brief  De-initialise keypad
  * @params None
  * @retval None
  */
void s4533087_hal_keypad_deinit(void) {

    KeypadFsmCurrentstate = DEINIT_STATE;
    keypad_gpio_deinit();
}

/**
  * @brief  Keypad FSM processing loop. To be called repeatedly (in main loop)
  * @params None
  * @retval None
  */
void s4533087_hal_keypad_fsmprocessing(void) {

    // FSM state checks and switching
    if (KeypadFsmCurrentstate == INIT_STATE) {
    
        KeypadFsmCurrentstate = COL1_STATE;
    }
    
    if (KeypadFsmCurrentstate == COL1_STATE) {
        
        colValue = 0x01;
        keypad_col1();
        KeypadFsmCurrentstate = ROWSCAN_STATE;
    }
    
     if (KeypadFsmCurrentstate == COL2_STATE) {
        
        colValue = 0x02;
        keypad_col2();
        KeypadFsmCurrentstate = ROWSCAN_STATE;
    }
   
    if (KeypadFsmCurrentstate == COL3_STATE) {
        
        colValue = 0x04;
        keypad_col3();
        KeypadFsmCurrentstate = ROWSCAN_STATE;
    }

    if (KeypadFsmCurrentstate == COL4_STATE) {
        
        colValue = 0x08;
        keypad_col4();
        KeypadFsmCurrentstate = ROWSCAN_STATE;
    }

    if (KeypadFsmCurrentstate == ROWSCAN_STATE) {

        rowValue = keypad_readrow();

        // Checks to see if the column of the last keypress is now deactivated
        // This is done to ensure only one key "press" is registered
        if ((colValue == triggeringColumn) && !rowValue) {
            
            triggered = 0;
        }

        // If a key is pressed and nothing has recently been pressed
        if (rowValue && !triggered) {

            coords = (0x0F & colValue) | ((0x0F & rowValue) << 4);
            decode_coords();
            char asciiValue;
            KeypadStatus = 1;
            triggered = 1;
            triggeringColumn = colValue;

            valueQueue = KeypadValue;

            if (KeypadValue <= 0x09) {
    
                asciiValue = (KeypadValue + '0');
            } else {
    
                // Offset, scales 0x0A - 0x0F -> 'A' - 'F'
                asciiValue =  (KeypadValue - 10 + 'A');
            }
            
            asciiQueue = asciiValue;

        } else {
            
            coords = 0;
            KeypadStatus = 0;
        }


        if (colValue == 0x01) {
        
            KeypadFsmCurrentstate = COL2_STATE;
        }
        
        if (colValue == 0x02) {
        
            KeypadFsmCurrentstate = COL3_STATE;
        }
        
        if (colValue == 0x04) {
            
            KeypadFsmCurrentstate = COL4_STATE;
        }
        
        if (colValue == 0x08) {
            
            KeypadFsmCurrentstate = COL1_STATE;
        }
    }
}

/**
  * @brief  Initialise GPIO pins
  * @params None
  * @retval None
  */
void keypad_gpio_init(void) {

    // Enable all GPIO clocks
    S4533087_HAL_KEYPAD_COL1PINCLK;
    S4533087_HAL_KEYPAD_COL2PINCLK;
    S4533087_HAL_KEYPAD_COL3PINCLK;
    S4533087_HAL_KEYPAD_COL4PINCLK;
    S4533087_HAL_KEYPAD_ROW1PINCLK;
    S4533087_HAL_KEYPAD_ROW2PINCLK;
    S4533087_HAL_KEYPAD_ROW3PINCLK;
    S4533087_HAL_KEYPAD_ROW4PINCLK;

    // Configure individual GPIO ports
    // Column 1
    GPIO_InitStructure.Pin      = S4533087_HAL_KEYPAD_COL1PIN;
    GPIO_InitStructure.Mode     = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStructure.Pull     = GPIO_PULLUP;
    GPIO_InitStructure.Speed    = GPIO_SPEED_FAST;
    HAL_GPIO_Init(S4533087_HAL_KEYPAD_COL1PINPORT, &GPIO_InitStructure);

    // Column 2
    GPIO_InitStructure.Pin      = S4533087_HAL_KEYPAD_COL2PIN;
    GPIO_InitStructure.Mode     = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStructure.Pull     = GPIO_PULLUP;
    GPIO_InitStructure.Speed    = GPIO_SPEED_FAST;
    HAL_GPIO_Init(S4533087_HAL_KEYPAD_COL2PINPORT, &GPIO_InitStructure);

    // Column 3
    GPIO_InitStructure.Pin      = S4533087_HAL_KEYPAD_COL3PIN;
    GPIO_InitStructure.Mode     = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStructure.Pull     = GPIO_PULLUP;
    GPIO_InitStructure.Speed    = GPIO_SPEED_FAST;
    HAL_GPIO_Init(S4533087_HAL_KEYPAD_COL3PINPORT, &GPIO_InitStructure);

    // Column 4
    GPIO_InitStructure.Pin      = S4533087_HAL_KEYPAD_COL4PIN;
    GPIO_InitStructure.Mode     = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStructure.Pull     = GPIO_PULLUP;
    GPIO_InitStructure.Speed    = GPIO_SPEED_FAST;
    HAL_GPIO_Init(S4533087_HAL_KEYPAD_COL4PINPORT, &GPIO_InitStructure);

    // Row 1
    GPIO_InitStructure.Pin      = S4533087_HAL_KEYPAD_ROW1PIN;
    GPIO_InitStructure.Mode     = GPIO_MODE_INPUT;
    GPIO_InitStructure.Pull     = GPIO_PULLUP;
    GPIO_InitStructure.Speed    = GPIO_SPEED_FAST;
    HAL_GPIO_Init(S4533087_HAL_KEYPAD_ROW1PINPORT, &GPIO_InitStructure);

    // Row 2
    GPIO_InitStructure.Pin      = S4533087_HAL_KEYPAD_ROW2PIN;
    GPIO_InitStructure.Mode     = GPIO_MODE_INPUT;
    GPIO_InitStructure.Pull     = GPIO_PULLUP;
    GPIO_InitStructure.Speed    = GPIO_SPEED_FAST;
    HAL_GPIO_Init(S4533087_HAL_KEYPAD_ROW2PINPORT, &GPIO_InitStructure);

    // Row 3
    GPIO_InitStructure.Pin      = S4533087_HAL_KEYPAD_ROW3PIN;
    GPIO_InitStructure.Mode     = GPIO_MODE_INPUT;
    GPIO_InitStructure.Pull     = GPIO_PULLUP;
    GPIO_InitStructure.Speed    = GPIO_SPEED_FAST;
    HAL_GPIO_Init(S4533087_HAL_KEYPAD_ROW3PINPORT, &GPIO_InitStructure);

    // Row 4
    GPIO_InitStructure.Pin      = S4533087_HAL_KEYPAD_ROW4PIN;
    GPIO_InitStructure.Mode     = GPIO_MODE_INPUT;
    GPIO_InitStructure.Pull     = GPIO_PULLUP;
    GPIO_InitStructure.Speed    = GPIO_SPEED_FAST;
    HAL_GPIO_Init(S4533087_HAL_KEYPAD_ROW4PINPORT, &GPIO_InitStructure);
}

/**
  * @brief  Deinitialise GPIO pins
  * @params None
  * @retval None
  */
void keypad_gpio_deinit(void) {

    HAL_GPIO_DeInit(S4533087_HAL_KEYPAD_COL1PINPORT, 
            S4533087_HAL_KEYPAD_COL1PIN);
    HAL_GPIO_DeInit(S4533087_HAL_KEYPAD_COL2PINPORT, 
            S4533087_HAL_KEYPAD_COL2PIN);
    HAL_GPIO_DeInit(S4533087_HAL_KEYPAD_COL3PINPORT, 
            S4533087_HAL_KEYPAD_COL3PIN);
    HAL_GPIO_DeInit(S4533087_HAL_KEYPAD_COL4PINPORT, 
            S4533087_HAL_KEYPAD_COL4PIN);
    HAL_GPIO_DeInit(S4533087_HAL_KEYPAD_ROW1PINPORT, 
            S4533087_HAL_KEYPAD_ROW1PIN);
    HAL_GPIO_DeInit(S4533087_HAL_KEYPAD_ROW2PINPORT, 
            S4533087_HAL_KEYPAD_ROW1PIN);
    HAL_GPIO_DeInit(S4533087_HAL_KEYPAD_ROW3PINPORT, 
            S4533087_HAL_KEYPAD_ROW3PIN);
    HAL_GPIO_DeInit(S4533087_HAL_KEYPAD_ROW4PINPORT, 
            S4533087_HAL_KEYPAD_ROW4PIN);
}


/**
  * @brief  Activates a column according to lower 4 bits of input
  * @params colval: half-byte corresponding to columns to set
  * @retval None
  */
void keypad_writecol(uint8_t colval) {

    // Ensure all columns are set to HIGH (i.e. deactivated)
    HAL_GPIO_WritePin(S4533087_HAL_KEYPAD_COL1PINPORT, 
            S4533087_HAL_KEYPAD_COL1PIN, 1);
    HAL_GPIO_WritePin(S4533087_HAL_KEYPAD_COL2PINPORT, 
            S4533087_HAL_KEYPAD_COL2PIN, 1);
    HAL_GPIO_WritePin(S4533087_HAL_KEYPAD_COL3PINPORT, 
            S4533087_HAL_KEYPAD_COL3PIN, 1);
    HAL_GPIO_WritePin(S4533087_HAL_KEYPAD_COL4PINPORT, 
            S4533087_HAL_KEYPAD_COL4PIN, 1);

    uint8_t c0 = !!(colval & 0x01);
    uint8_t c1 = !!(colval & 0x02);
    uint8_t c2 = !!(colval & 0x04);
    uint8_t c3 = !!(colval & 0x08);

    if (c0) {
        
        // Pull column 1 down
        HAL_GPIO_WritePin(S4533087_HAL_KEYPAD_COL1PINPORT, 
                S4533087_HAL_KEYPAD_COL1PIN, 0);
        return;
    } 

    if (c1) {
    
        // Pull column 2 down
        HAL_GPIO_WritePin(S4533087_HAL_KEYPAD_COL2PINPORT, 
                S4533087_HAL_KEYPAD_COL2PIN, 0);
        return;
    }

    if (c2) {
        
        // Pull column 3 down
        HAL_GPIO_WritePin(S4533087_HAL_KEYPAD_COL3PINPORT, 
                S4533087_HAL_KEYPAD_COL3PIN, 0);
        return;
    } 

    if (c3) {
    
        // Pull column 4 down
        HAL_GPIO_WritePin(S4533087_HAL_KEYPAD_COL4PINPORT, 
                S4533087_HAL_KEYPAD_COL4PIN, 0);
        return;
    }

    // Default    
    KeypadStatus = 0;
}

/**
  * @brief  Read a row from the keypad
  * @params None
  * @retval Bit mask corresponding to the row signal
  */
uint8_t keypad_readrow(void) {

    uint8_t column = 0;

    // This algorithm is created in such a way that it is easy to transform to
    // be able to read multiple key presses simultaneously.

    // Check if row 1 pulled LOW (i.e. activated)
    if (!HAL_GPIO_ReadPin(S4533087_HAL_KEYPAD_ROW1PINPORT, 
            S4533087_HAL_KEYPAD_ROW1PIN)) {
    
        column |= (1 << 0);
        KeypadStatus = 1;
        return column;
    }

    // Check if row 2 pulled LOW (i.e. activated)
    if (!HAL_GPIO_ReadPin(S4533087_HAL_KEYPAD_ROW2PINPORT, 
            S4533087_HAL_KEYPAD_ROW2PIN)) {
    
        column |= (1 << 1);
        KeypadStatus = 1;
        return column;
    }  

    // Check if row 3 pulled LOW (i.e. activated)
    if (!HAL_GPIO_ReadPin(S4533087_HAL_KEYPAD_ROW3PINPORT, 
            S4533087_HAL_KEYPAD_ROW3PIN)) {
    
        column |= (1 << 2);
        KeypadStatus = 1;
        return column;
    }

    // Check if row 4 pulled LOW (i.e. activated)
    if (!HAL_GPIO_ReadPin(S4533087_HAL_KEYPAD_ROW4PINPORT, 
            S4533087_HAL_KEYPAD_ROW4PIN)) {
    
        column |= (1 << 3);
        KeypadStatus = 1;
        return column;
    }

    KeypadStatus = 0;
    return column;
}

/**
  * @brief  Read the KeypadStatus variable
  * @params None
  * @retval KeypadStatus: current status of keypad
  */
int s4533087_hal_keypad_read_status(void) {

    return KeypadStatus;
}

/**
  * @brief  Returns current hex value of the keypad
  * @params None
  * @retval Hex value of keypad (mask)
  */
uint8_t s4533087_hal_keypad_read_key(void) {

    // Pops value off the queue. Default value is 0xFF
    uint8_t retval = valueQueue;
    valueQueue = 0xFF;
    return retval;
}

/**
  * @brief  Returns ASCII value of keypad
  * @params None
  * @retval Current ASCII value of the keypad
  */
unsigned char s4533087_hal_keypad_read_ascii(void) {

    // Pops value off the queue. Default value is 0
    unsigned char retval = asciiQueue;
    asciiQueue = 0;
    return retval;
}

void decode_coords(void) {

    uint8_t rowNumber = 0;
    uint8_t colNumber = 0;
    
    // Find row number
    switch(coords & 0x0F) {
    
        case 0x01:
            colNumber = 1;
            break;
        
        case 0x02:
            colNumber = 2;
            break;
        
        case 0x04:
            colNumber = 3;
            break;
        
        case 0x08:
            colNumber = 4;
            break;
        
        default:
            colNumber = 0;
            break;
    }

    // Find column number
    switch((coords & 0xF0) >> 4) {
    
        case 0x01:
            rowNumber = 1;
            break;
        
        case 0x02:
            rowNumber = 2;
            break;
        
        case 0x04:
            rowNumber = 3;
            break;
        
        case 0x08:
            rowNumber = 4;
            break;
        
        default:
            rowNumber = 0;
            break;
    }

    if (!rowNumber || !colNumber) {
        
        KeypadStatus = 0;
        KeypadValue = 0;
        return;
    }

    // Coords are encoded as rrrr cccc (lsn is column value, msn is row value)
    // Keypad has the following values:
    //     [ 1 2 3 A ]
    //     [ 4 5 6 B ]
    //     [ 7 8 9 C ]
    //     [ 0 F E D ]
    int lookupGrid[4][4] = {
            {0x01, 0x02, 0x03, 0x0A},
            {0x04, 0x05, 0x06, 0x0B},
            {0x07, 0x08, 0x09, 0x0C},
            {0x00, 0x0F, 0x0E, 0x0D}
    };

    KeypadStatus = 1;
    KeypadValue = lookupGrid[rowNumber - 1][colNumber - 1];
}
