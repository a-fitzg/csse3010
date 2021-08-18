/**
  *****************************************************************************
  * @file    s4533087_os_cag_keypad.c
  * @author  Alexander FitzGerald - s4533087
  * @date    08052020
  * @brief   Alphanumeric keypad OS CAG driver
  *****************************************************************************
  * EXTERNAL FUNCTIONS
  *****************************************************************************
  * s4533087_os_keypad_master()
  * s4533087_os_keypad_grid()
  * s4533087_os_keypad_mnemonic()
  *****************************************************************************
  */

/* Includes ---------------------------------------------------------------- */
#include <string.h>

#include "board.h"
#include "processor_hal.h"
#include "debug_printf.h"

// FreeRTOS includes
#include "FreeRTOS.h"
#include "task.h"
#include "event_groups.h"
#include "semphr.h"
#include "queue.h"

// Mylib includes

#ifdef S4533087_MYLIB_CONFIGURE_INC
#include "s4533087_mylib_configure.h"
#endif

#include "s4533087_os_keypad.h"
#include "s4533087_os_cag_display.h"
#include "s4533087_os_cag_keypad.h"
#include "s4533087_os_lta1000g.h"
#include "s4533087_os_cag_joystick.h"
#include "s4533087_os_joystick.h"
#include "s4533087_os_pb.h"
#include "s4533087_os_cag_globals.h"

/* Private typedef --------------------------------------------------------- */
/* Private define ---------------------------------------------------------- */
// Task priorities
#define KEYPAD_DEFAULT_TASK_PRIORITY     (tskIDLE_PRIORITY + 2)

// Task stack allocations
#define KEYPAD_DEFAULT_STACK_SIZE        (configMINIMAL_STACK_SIZE * 4)

#define true    1
#define false   0

// Definitions for Mnemonic FSM
#define M_SLEEP_STATE       0
#define M_INIT_STATE        1
#define M_STILL_STATE       2
#define M_OSCILLATOR_STATE  3
#define M_GLIDER_STATE      4
#define M_DELETE_STATE      5
#define M_CREATE_STATE      6

/* Private macro ----------------------------------------------------------- */
/* Private variables ------------------------------------------------------- */
// Task handles
TaskHandle_t taskKeypadGridHandle;
TaskHandle_t taskKeypadMnemonicHandle;

// Keypad variables
char currentKey;
char previousKey;
char asciiValue;
char mnemonicKeyLog[KEY_LOG_LENGTH];

/* Private function prototypes --------------------------------------------- */
uint8_t increment_subgrid(uint8_t);
void key_log_push(char[KEY_LOG_LENGTH], char);
void key_log_clear(char[KEY_LOG_LENGTH]);
uint8_t compare_for_still(char[KEY_LOG_LENGTH]);
uint8_t compare_for_oscillator(char[KEY_LOG_LENGTH]);
uint8_t compare_for_glider(char[KEY_LOG_LENGTH]);
uint8_t compare_for_delete(char[KEY_LOG_LENGTH]);
uint8_t compare_for_create(char[KEY_LOG_LENGTH]);

void debug_print_log(char[KEY_LOG_LENGTH]);
void get_last_keys(char[KEY_LOG_LENGTH], char*, uint8_t);

/**
  * @brief  Task for the keypad input
  * @params None
  * @retval None
  */
void s4533087_os_keypad_master(void) {

    for (uint8_t i = 0; i < KEY_LOG_LENGTH; i++) {

        mnemonicKeyLog[i] = 0;
    }

    s4533087_SemaphoreKeypadMutex = xSemaphoreCreateMutex();

    s4533087_QueueDisplayLife = xQueueCreate(10, sizeof(
            s4533087_QueueDisplayLife));

    // HAL inits from mylib
    s4533087_os_lta1000g_init();
    s4533087_os_keypad_init();
    s4533087_os_pb_on_init();

    // Initialisation functions may not be thread-safe
    portDISABLE_INTERRUPTS();

    // Initialse on-board LEDS for use later
    BRD_LEDInit();

    // Initialise the semaphore that the pushbutton sends to the keypad
    s4533087_SemaphoreButtonPress = xSemaphoreCreateBinary();

    // Initialise necessary hardware
    BRD_LEDBlueOff();
    BRD_LEDGreenOff();

    // We start in grid mode, so red LED is on at startup
    BRD_LEDRedOn();

    currentKey = 0;
    previousKey = 0;

    portENABLE_INTERRUPTS();

    for (;;) {
        
        // Keep a look out for semaphore, change keypad state when we get it
        if (s4533087_SemaphoreButtonPress != NULL) {

            // Try and take a semaphore, wait at most 1 tick
            if (xSemaphoreTake(s4533087_SemaphoreButtonPress, 10) == pdTRUE) {
           
                // Try to get another semaphore in prescribed debounce period 
                // (DEBOUNCE_PERIOD), meaning there has been another press.
                // Essentially, wait until there are no bounces for a while 
                while(xSemaphoreTake(s4533087_SemaphoreButtonPress, 
                        DEBOUNCE_PERIOD) == pdTRUE) {
                }
                // We've got the last press in a while. Let's see if the button
                // is still pressed...
                if (s4533087_os_pb_read_button()) {

                    // This code region is the "debounced" region.
                    // Toggle keypad state
                    s4533087_keypadFsmState = !s4533087_keypadFsmState;
                    BRD_LEDOff(BRD_BLUE_LEDMASK | BRD_RED_LEDMASK);

                    // Determine which LED to turn on
                    if (s4533087_keypadFsmState == GRID_STATE) {
                        
                        // Grid mode - turn on red LED
                        BRD_LEDRedOn();
                    } else {
                        
                        // Mnemonic mode - turn on blue LED
                        BRD_LEDBlueOn();
                    }
                }

            } else {
                // Didn't get a semaphore :(
            }
        }
        vTaskDelay(1);
    }
}

/**
  * @brief  Task for the keypad in grid mode
  * @params None
  * @retval None
  */
void s4533087_os_keypad_grid(void) {

    // Just wait a bit for things to get set up
    vTaskDelay(100);

    const uint8_t INIT_STATE = 0;
    const uint8_t SLEEP_STATE = 3;

    uint8_t gridFsmState = 0;

    uint8_t fullByte = 0;
    uint8_t recentTransition = 0;

    int8_t x = -1;
    int8_t y = -1;

    for (;;) {

        // Check if this task is supposed to be doing work (fsm state = grid)
        if (s4533087_keypadFsmState == GRID_STATE) {

            xSemaphoreTake(s4533087_SemaphoreKeypadMutex, MUTEX_WAIT_TIME);

            vTaskDelay(2);

            if (gridFsmState == SLEEP_STATE) {
                
                currentKey = 0;
                previousKey = 0;
                gridFsmState = INIT_STATE;
            }
            
            s4533087_os_keypad_fsmprocessing();

            asciiValue = s4533087_os_keypad_read_ascii();
            if (asciiValue) {
            
                // Toggle green LED (as per spec)
                BRD_LEDGreenToggle();
               
                previousKey = currentKey;
                currentKey = asciiValue;   
            }

            // If we've just switched from the other mode, currentKey
            // will be null if nothing since has been pressed
            if (currentKey == 0) {
                
                gridFsmState = INIT_STATE;
            } else {
                
                switch(currentKey) {

                    case '0': 

                        // Increment x
                        x = increment_subgrid(x);
                        s4533087_os_lta1000g_write_x(x + 1);
                        break;

                    case 'F':

                        // Increment y
                        y = increment_subgrid(y);
                        s4533087_os_lta1000g_write_y(y + 1);
                        break;

                    case 'A':

                        // Resume simulation                        
                        xEventGroupSetBits(s4533087_EventSimulatorAction, 
                                EVENT_SIMULATOR_START);
                        break;

                    case 'B':
                        
                        // Stop simulation
                        xEventGroupSetBits(s4533087_EventSimulatorAction, 
                                EVENT_SIMULATOR_STOP);
                        break;

                    case 'C':

                        // Clear simulation grid
                        xEventGroupSetBits(s4533087_EventSimulatorAction, 
                                EVENT_SIMULATOR_CLEAR_GRID);
                        break;

                    default:
                        
                        if (currentKey >= '1' && currentKey <= '9') {
                            
                            // Place on grid
                            // Check if BOTH x and y coordinates have been set
                            if (x >= 0 && y >= 0) {

                                uint8_t x_coord = ((currentKey - '1') % 3) + 
                                        (x * 3);
                                uint8_t y_coord = ((currentKey - '1') / 3) + 
                                        (y * 3);

                                // Now we need to set the relevant cell
                                caMessage_t message;
                                message.type = (1 << 4) | (1);
                                message.cell_x = x_coord;
                                message.cell_y = y_coord;   

                                // Then we send the message off onto the queue
                                if (s4533087_QueueDisplayLife != NULL) {
                                    
                                    xQueueSendToFront(s4533087_QueueDisplayLife,
                                    (void*)&message, (portTickType)10);
                                }
    
                                x = -1;
                                y = -1;
                                s4533087_os_lta1000g_write_x(0);
                                s4533087_os_lta1000g_write_y(0);
                            }
                        }
                        break;
                }
                previousKey = currentKey;
                currentKey = 0;
            }

            xSemaphoreGive(s4533087_SemaphoreKeypadMutex);

        } else {
            
             gridFsmState = SLEEP_STATE;
        }
        vTaskDelay(10);
    }
}

/**
  * @brief  Task for the keypad in mnemonic mode
  * @params None
  * @retval None
  */
void s4533087_os_keypad_mnemonic(void) {

    // Just wait a bit for things to get set up
    vTaskDelay(100);

    uint8_t mnemonicFsmState = M_SLEEP_STATE;

    // Message for queue
    caMessage_t message;

    // Type for lifeforms
    uint8_t type = 0;

    char commandType = 0;
    int8_t x = -1;
    int8_t y = -1;

    uint8_t fullByte = 0;
    uint8_t recentTransition = 0;

    uint8_t ready = false;

    key_log_clear(mnemonicKeyLog);

    for (;;) {

        // Check if this task is supposed to be doing work
        if (s4533087_keypadFsmState == MNEMONIC_STATE) {

            xSemaphoreTake(s4533087_SemaphoreKeypadMutex, MUTEX_WAIT_TIME);

            vTaskDelay(2);

            if (mnemonicFsmState == M_SLEEP_STATE) {

                currentKey = 0;
                previousKey = 0;
                mnemonicFsmState = M_INIT_STATE;
                key_log_clear(mnemonicKeyLog);
            }
            
            s4533087_os_keypad_fsmprocessing();

            asciiValue = s4533087_os_keypad_read_ascii();
            if (asciiValue) {
            
                // Toggle green LED (as per spec)
                BRD_LEDGreenToggle();
               
                previousKey = currentKey;
                currentKey = asciiValue;
            }
            
            // If we've just switched from the other mode, currentKey will be 
            // null if nothing since has been pressed
            if (currentKey == 0) {

                //mnemonicFsmState = M_INIT_STATE;
            } else {

                key_log_push(mnemonicKeyLog, currentKey);

                //debug_print_log(mnemonicKeyLog);

                // We've received a key press, now interpret it...
                switch (mnemonicFsmState) {

                    case M_INIT_STATE:
                        
                        // At this point, the log should either be clear, or 
                        // filled with a meaningless character sequence
                        
                        // First, we need to check for the special sequences
                        // '*'(F), '#'(D), and '0' that work immediately

                        // Check for *
                        if (currentKey == 'F') {

                            // Start the simulator
                            xEventGroupSetBits(s4533087_EventSimulatorAction,
                                    EVENT_SIMULATOR_START);
                        }

                        // Check for #
                        if (currentKey == 'D') {

                            // Stop the simulator
                            xEventGroupSetBits(s4533087_EventSimulatorAction,
                                    EVENT_SIMULATOR_STOP);
                        }

                        // Check for 0
                        if (currentKey == '0') {

                            // Clear the display
                            xEventGroupSetBits(s4533087_EventSimulatorAction,
                                    EVENT_SIMULATOR_CLEAR_GRID);
                        }

                        // Compare for all the sequences with actions: Still,
                        // oscillator, glider, delete, create
                        if (compare_for_still(mnemonicKeyLog)) {

                            mnemonicFsmState = M_STILL_STATE;
                            key_log_clear(mnemonicKeyLog);
                        } else if (compare_for_oscillator(mnemonicKeyLog)) {

                            mnemonicFsmState = M_OSCILLATOR_STATE;
                            key_log_clear(mnemonicKeyLog);
                        } else if (compare_for_glider(mnemonicKeyLog)) {

                            mnemonicFsmState = M_GLIDER_STATE;
                            key_log_clear(mnemonicKeyLog);
                        } else if (compare_for_delete(mnemonicKeyLog)) {

                            mnemonicFsmState = M_DELETE_STATE;
                            key_log_clear(mnemonicKeyLog);
                        } else if (compare_for_create(mnemonicKeyLog)) {

                            mnemonicFsmState = M_CREATE_STATE;
                            key_log_clear(mnemonicKeyLog);
                        }
                        break;

                    case M_STILL_STATE:

                        // We've received the "still life" sequence already, 
                        // checking the arguments for it
                        
                        ready = true;

                        // Check if all log entries are null except for the most
                        // recent 3
                        for (uint8_t i = 0; i < 3; i++) {

                            if (mnemonicKeyLog[i] == 0) {

                                // We're not ready to parse the input yet, skip
                                ready = false;
                                break;
                            }
                        }
                        // Check if there are actually all null chars
                        for (uint8_t i = 3; i < (KEY_LOG_LENGTH - 1); i++) {

                            if (mnemonicKeyLog[i] != 0) {

                                ready = false;
                                break;
                            }
                        }
                        
                        // If we're not ready to parse this input, break
                        if (!ready) {
                        
                            break;
                        }

                        // Now, check if the last 3 chars in the queue are valid
                        // The first number must be between 0 and 3
                        if (!((mnemonicKeyLog[2] >= '0') && 
                                (mnemonicKeyLog[2] <= '3'))) {

                            // Disqualified. Clear the log and start again
                            key_log_clear(mnemonicKeyLog);
                            mnemonicFsmState = M_INIT_STATE;
                        }
                        // The second number must be between 1 and 5
                        if (!((mnemonicKeyLog[1] >= '1') && 
                                (mnemonicKeyLog[1] <= '5'))) {

                            // Disqualified. Clear the log and start again
                            key_log_clear(mnemonicKeyLog);
                            mnemonicFsmState = M_INIT_STATE;
                        }
                        // The third number must be between 1 and 5
                        if (!((mnemonicKeyLog[0] >= '1') && 
                                (mnemonicKeyLog[0] <= '5'))) {

                            // Disqualified. Clear the log and start again
                            key_log_clear(mnemonicKeyLog);
                            mnemonicFsmState = M_INIT_STATE;
                        }

                        // We have determined that this is a valid sequence
                        type = 0;
                        
                        // Bits [7 to 4] are 2 for still life
                        type |= (2 << 4);
                        type |= (mnemonicKeyLog[2] - '0');
                        message.type = type;

                        // Subgrid numbers have to be converted to coordinates
                        message.cell_x = (mnemonicKeyLog[1] - '1') * 3;
                        message.cell_y = (mnemonicKeyLog[0] - '1') * 3;

                        // Then we send the message off onto the queue
                        
                        if (s4533087_QueueDisplayLife != NULL) {

                            xQueueSendToFront(s4533087_QueueDisplayLife,
                                    (void*)&message, (portTickType)10);
                        }                   
                        
                        key_log_clear(mnemonicKeyLog);
                        mnemonicFsmState = M_INIT_STATE;
                        break;

                    case M_OSCILLATOR_STATE:

                        // We've received the "oscillator" sequence already,
                        // checking the arguments
                        ready = true;

                        // Check if all log entries are null except for the most
                        // recent 3
                        for (uint8_t i = 0; i < 3; i++) {

                            if (mnemonicKeyLog[i] == 0) {

                                // We're not ready to parse the input yet, skip
                                ready = false;
                                break;
                            }
                        }
                        // Check if there are actually all null chars
                        for (uint8_t i = 3; i < (KEY_LOG_LENGTH - 1); i++) {

                            if (mnemonicKeyLog[i] != 0) {

                                ready = false;
                                break;
                            }
                        }
                        
                        // If we're not ready to parse this input, break
                        if (!ready) {
                        
                            break;
                        }

                        // Now, check if the last 3 chars in the queue are valid
                        // The first number must be between 0 and 3
                        if (!((mnemonicKeyLog[2] >= '0') && 
                                (mnemonicKeyLog[2] <= '3'))) {

                            // Disqualified. Clear the log and start again
                            key_log_clear(mnemonicKeyLog);
                            mnemonicFsmState = M_INIT_STATE;
                        }
                        // The second number must be between 1 and 5
                        if (!((mnemonicKeyLog[1] >= '1') && 
                                (mnemonicKeyLog[1] <= '5'))) {

                            // Disqualified. Clear the log and start again
                            key_log_clear(mnemonicKeyLog);
                            mnemonicFsmState = M_INIT_STATE;
                        }
                        // The third number must be between 1 and 5
                        if (!((mnemonicKeyLog[0] >= '1') && 
                                (mnemonicKeyLog[0] <= '5'))) {

                            // Disqualified. Clear the log and start again
                            key_log_clear(mnemonicKeyLog);
                            mnemonicFsmState = M_INIT_STATE;
                        }

                        // We have determined that this is a valid sequence
                        type = 0;

                        // Bits [7 to 4] are 3 for oscillator
                        type |= (3 << 4);
                        type |= (mnemonicKeyLog[2] - '0');
                        message.type = type;

                        // Subgrid numbers have to be converted to coordinates
                        message.cell_x = (mnemonicKeyLog[1] - '1') * 3;
                        message.cell_y = (mnemonicKeyLog[0] - '1') * 3;

                        // Then we send the message off onto the queue
                        
                        if (s4533087_QueueDisplayLife != NULL) {

                            xQueueSendToFront(s4533087_QueueDisplayLife,
                                    (void*)&message, (portTickType)10);
                        }                   
                        
                        key_log_clear(mnemonicKeyLog);
                        mnemonicFsmState = M_INIT_STATE;
                        break;

                    case M_GLIDER_STATE:

                        // We've received the "glider" sequence already,
                        // checking the arguments
                        ready = true;

                        // Check if all log entries are null except for the most
                        // recent 2
                        for (uint8_t i = 0; i < 2; i++) {

                            if (mnemonicKeyLog[i] == 0) {

                                // We're not ready to parse the input yet, skip
                                ready = false;
                                break;
                            }
                        }
                        // Check if there are actually all null chars
                        for (uint8_t i = 2; i < (KEY_LOG_LENGTH - 1); i++) {

                            if (mnemonicKeyLog[i] != 0) {

                                ready = false;
                                break;
                            }
                        }
                        
                        // If we're not ready to parse this input, break
                        if (!ready) {
                        
                            break;
                        }

                        // Now, check if the last 2 chars in the queue are valid
                        // The first number must be between 1 and 5
                        if (!((mnemonicKeyLog[1] >= '1') && 
                                (mnemonicKeyLog[1] <= '5'))) {

                            // Disqualified. Clear the log and start again
                            key_log_clear(mnemonicKeyLog);
                            mnemonicFsmState = M_INIT_STATE;
                        }
                        // The second number must be between 1 and 5
                        if (!((mnemonicKeyLog[0] >= '1') && 
                                (mnemonicKeyLog[0] <= '5'))) {

                            // Disqualified. Clear the log and start again
                            key_log_clear(mnemonicKeyLog);
                            mnemonicFsmState = M_INIT_STATE;
                        }

                        // We have determined that this is a valid sequence
                        type = 0;

                        // Bits [7 to 4] are 4 for spaceship / glider
                        type |= (4 << 4);
                        message.type = type;

                        // Subgrid numbers have to be converted to coordinates
                        message.cell_x = (mnemonicKeyLog[1] - '1') * 3;
                        message.cell_y = (mnemonicKeyLog[0] - '1') * 3;

                        // Then we send the message off onto the queue
                        
                        if (s4533087_QueueDisplayLife != NULL) {

                            xQueueSendToFront(s4533087_QueueDisplayLife,
                                    (void*)&message, (portTickType)10);
                        }                   
                        
                        key_log_clear(mnemonicKeyLog);
                        mnemonicFsmState = M_INIT_STATE;
                        break;

                    case M_DELETE_STATE:

                        // We've received the "delete" sequence already,
                        // checking the arguments
                        ready = true;

                        // Check if all log entries are null except for the most
                        // recent 1
                        if (mnemonicKeyLog[0] == 0) {

                            // We're not ready to parse the input yet, skip
                            ready = false;
                            break;
                        }
                        // Check if there are actually all null chars
                        for (uint8_t i = 1; i < (KEY_LOG_LENGTH - 1); i++) {

                            if (mnemonicKeyLog[i] != 0) {

                                ready = false;
                                break;
                            }
                        }
                        
                        // If we're not ready to parse this input, break
                        if (!ready) {
                        
                            break;
                        }

                        // Now, check if the last char in the queue is valid
                        // The first number must be 0 or 1
                        if (!((mnemonicKeyLog[0] == '0') || 
                                (mnemonicKeyLog[0] == '1'))) {

                            // Disqualified. Clear the log and start again
                            key_log_clear(mnemonicKeyLog);
                            mnemonicFsmState = M_INIT_STATE;
                        }
                        // We have determined that this is a valid sequence
                        type = mnemonicKeyLog[0] - '0';

                        switch (type) {

                            case 0:

                                // Disable simulator
                                vTaskSuspend(s4533087_taskSimulatorHandle);
                                vTaskDelay(1);
                                vTaskResume(
                                        s4533087_taskSimulatorAltRefHandle);
                                break;
                        
                            case 1:
                                
                                // Disable joystick
                                // First, suspend the task
                                vTaskSuspend(s4533087_taskJoystickHandle);
                                vTaskDelay(1);
                                
                                // Then deinitialise the joystick GPIO pins
                                portDISABLE_INTERRUPTS();
                                s4533087_os_joystick_deinit();
                                portENABLE_INTERRUPTS();
                                break;

                            default:

                                // The input's invalid - shouldn't reach here
                                break;
                        }
                       
                        key_log_clear(mnemonicKeyLog);
                        mnemonicFsmState = M_INIT_STATE;
                        break;

                    case M_CREATE_STATE:

                        // We've received the "create" sequence already,
                        // checking the arguments
                        ready = true;

                        // Check if all log entries are null except for the most
                        // recent 1
                        if (mnemonicKeyLog[0] == 0) {

                            // We're not ready to parse the input yet, skip
                            ready = false;
                            break;
                        }
                        // Check if there are actually all null chars
                        for (uint8_t i = 1; i < (KEY_LOG_LENGTH - 1); i++) {

                            if (mnemonicKeyLog[i] != 0) {

                                ready = false;
                                break;
                            }
                        }
                        
                        // If we're not ready to parse this input, break
                        if (!ready) {
                        
                            break;
                        }

                        // Now, check if the last char in the queue is valid
                        // The first number must be 0 or 1
                        if (!((mnemonicKeyLog[0] == '0') || 
                                (mnemonicKeyLog[0] == '1'))) {

                            // Disqualified. Clear the log and start again
                            key_log_clear(mnemonicKeyLog);
                            mnemonicFsmState = M_INIT_STATE;
                        }

                        // We have determined that this is a valid sequence
                        type = mnemonicKeyLog[0] - '0';

                        switch (type) {

                            case 0:
                                
                                // Enable simulator
                                vTaskSuspend(
                                        s4533087_taskSimulatorAltRefHandle);
                                vTaskDelay(1);
                                vTaskResume(s4533087_taskSimulatorHandle);
                                break;
                        
                            case 1:
                                
                                // Enable joystick
                                // First, reinit
                                portDISABLE_INTERRUPTS();
                                s4533087_os_joystick_init();
                                portENABLE_INTERRUPTS();
                                vTaskDelay(1);

                                // Then resume the task
                                vTaskResume(s4533087_taskJoystickHandle);
                                break;

                            default:

                                // The input's invalid - shouldn't reach here
                                break;
                        }
                      

                        key_log_clear(mnemonicKeyLog);
                        mnemonicFsmState = M_INIT_STATE;
                        break;

                    default:

                        break;
                }

                previousKey = currentKey;
                currentKey = 0;
            }

            xSemaphoreGive(s4533087_SemaphoreKeypadMutex);

        } else {

            mnemonicFsmState = M_SLEEP_STATE;
        }
        vTaskDelay(10);
    }
}


/**
  * @brief  Increments subgrid value, and wraps back to 0 from 4
  * @params value: subgrid value to increment
  * @retval uint8_t: subgrid value incremented (or wrapped)
  */
uint8_t increment_subgrid(uint8_t value) {

    const int8_t MAX_SUBGRID_VALUE = 4;
    if (value < MAX_SUBGRID_VALUE) {
            
        return ++value;
    } else {
        
        return 0;
    }
}

/**
  * @brief  Pushes a key to the start of the log, and deletes the one on the end
  * @params log: The log of keys pressed
  * @retval None
  */
void key_log_push(char keyLog[KEY_LOG_LENGTH], char value) {

    for (uint8_t i = (KEY_LOG_LENGTH - 1); i > 0; i--) {

        keyLog[i] = keyLog[i - 1];
    }
    keyLog[0] = value;
}

/**
  * @brief  Clears the key log
  * @params keyLog: The key log to clear
  * @retval None
  */
void key_log_clear(char keyLog[KEY_LOG_LENGTH]) {

    for (uint8_t i = 0; i < KEY_LOG_LENGTH; i++) {

        keyLog[i] = 0;
    }
}

/**
  * @brief  Checks a key log to see if there is an instruction for a still life
  * @params keyLog: The log of keys pressed
  * @retval True/false: If the sequence matches the still life instruction
  */
uint8_t compare_for_still(char keyLog[KEY_LOG_LENGTH]) {

    // Still sequence is: 77777 88 5555
    char stillSequence[] = {'7', '7', '7', '7', '7', '8', '8', '5', '5', '5', 
            '5'};    

    const uint8_t stillSequenceLength = 11;

    // Basically a calloc()
    char* recentSequence = pvPortMalloc(stillSequenceLength * sizeof(char));
    for (uint8_t i = 0; i < stillSequenceLength; i++) {

        recentSequence[i] = 0;
    }

    get_last_keys(keyLog, recentSequence, stillSequenceLength);

    for (uint8_t i = 0; i < stillSequenceLength; i++) {
    
        if (recentSequence[i] != stillSequence[(stillSequenceLength - 1) - i]) {

            // Once charatcter is off, sequences don't match
            vPortFree(recentSequence);
            return false;
        }
    }
    
    // Otherwise, sequences completely match
    vPortFree(recentSequence);
    return true;
}

/**
  * @brief  Checks a key log to see if there is an instruction for an oscillator
  * @params keyLog: The log of keys pressed
  * @retval True/false: If the sequence matches the oscillator instruction
  */
uint8_t compare_for_oscillator(char keyLog[KEY_LOG_LENGTH]) {

    // Oscillator sequence: 6666 77777 2222
    char oscillatorSequence[] = {'6', '6', '6', '6', '7', '7', '7', '7', '7', 
            '2', '2', '2', '2'};

    const uint8_t oscillatorSequenceLength = 13;

    // Basically a calloc()
    char* recentSequence = pvPortMalloc(oscillatorSequenceLength * 
            sizeof(char));
    for (uint8_t i = 0; i < oscillatorSequenceLength; i++) {

        recentSequence[i] = 0;
    }

    get_last_keys(keyLog, recentSequence, oscillatorSequenceLength);

    for (uint8_t i = 0; i < oscillatorSequenceLength; i++) {
    
        if (recentSequence[i] != oscillatorSequence[(oscillatorSequenceLength -
                1) - i]) {

            // Once charatcter is off, sequences don't match
            vPortFree(recentSequence);
            return false;
        }
    }
    
    // Otherwise, sequences completely match
    vPortFree(recentSequence);
    return true;
}

/**
  * @brief  Checks a key log to see if there is an instruction for a glider
  * @params keyLog: The log of keys pressed
  * @retval True/false: If the sequence matches the glider instruction
  */
uint8_t compare_for_glider(char keyLog[KEY_LOG_LENGTH]) {

    // Glider sequence: 44 5555 33
    char gliderSequence[] = {'4', '4', '5', '5', '5', '5', '3', '3'};

    const uint8_t gliderSequenceLength = 8;

    // Basically a calloc()
    char* recentSequence = pvPortMalloc(gliderSequenceLength * sizeof(char));
    for (uint8_t i = 0; i < gliderSequenceLength; i++) {

        recentSequence[i] = 0;
    }

    get_last_keys(keyLog, recentSequence, gliderSequenceLength);

    for (uint8_t i = 0; i < gliderSequenceLength; i++) {
    
        if (recentSequence[i] != gliderSequence[(gliderSequenceLength - 1) - 
                i]) {

            // Once charatcter is off, sequences don't match
            vPortFree(recentSequence);
            return false;
        }
    }
    
    // Otherwise, sequences completely match
    vPortFree(recentSequence);
    return true;
}

/**
  * @brief  Checks a key log to see if there is an instruction for delete
  * @params keyLog: The log of keys pressed
  * @retval True/false: If the sequence matches the delete instruction
  */
uint8_t compare_for_delete(char keyLog[KEY_LOG_LENGTH]) {

    // Glider sequence: 33 5555 88
    char deleteSequence[] = {'3', '3', '5', '5', '5', '5', '8', '8'};

    const uint8_t deleteSequenceLength = 8;

    // Basically a calloc()
    char* recentSequence = pvPortMalloc(deleteSequenceLength * sizeof(char));
    for (uint8_t i = 0; i < deleteSequenceLength; i++) {

        recentSequence[i] = 0;
    }

    get_last_keys(keyLog, recentSequence, deleteSequenceLength);

    for (uint8_t i = 0; i < deleteSequenceLength; i++) {
    
        if (recentSequence[i] != deleteSequence[(deleteSequenceLength - 1) - 
                i]) {

            // Once charatcter is off, sequences don't match
            vPortFree(recentSequence);
            return false;
        }
    }
    
    // Otherwise, sequences completely match
    vPortFree(recentSequence);
    return true;
}

/**
  * @brief  Checks a key log to see if there is an instruction for create
  * @params keyLog: The log of keys pressed
  * @retval True/false: If the sequence matches the create instruction
  */
uint8_t compare_for_create(char keyLog[KEY_LOG_LENGTH]) {

    // Glider sequence: 2222 7777 333
    char createSequence[] = {'2', '2', '2', '2', '7', '7', '7', '7', '3', '3', 
            '3'};

    const uint8_t createSequenceLength = 11;

    // Basically a calloc()
    char* recentSequence = pvPortMalloc(createSequenceLength * sizeof(char));
    for (uint8_t i = 0; i < createSequenceLength; i++) {

        recentSequence[i] = 0;
    }

    get_last_keys(keyLog, recentSequence, createSequenceLength);

    for (uint8_t i = 0; i < createSequenceLength; i++) {
    
        if (recentSequence[i] != createSequence[(createSequenceLength - 1) - 
                i]) {

            // Once charatcter is off, sequences don't match
            vPortFree(recentSequence);
            return false;
        }
    }
    
    // Otherwise, sequences completely match
    vPortFree(recentSequence);
    return true;
}

/**
  * @brief  Prints the mnemonic key log. FOR DEBUGGING PURPOSES ONLY
  * @params keyLog: The key log to print
  * @retval None
  */
void debug_print_log(char keyLog[KEY_LOG_LENGTH]) {

    portENTER_CRITICAL();
    for (uint8_t i = 0; i < KEY_LOG_LENGTH; i++) {

        debug_printf("%c", keyLog[i]);
    }
    debug_printf("\r\n");
    portEXIT_CRITICAL();
}

/**
  * @brief  Gets the last "num" characters off the log
  * @params keyLog: The key log
            dest: The destination string
            num: Number of items to retrieve
  * @retval None
  */
void get_last_keys(char keyLog[KEY_LOG_LENGTH], char* dest, uint8_t num) {

    uint8_t activeChars = 0;
    for (uint8_t i = 0; i < KEY_LOG_LENGTH; i++) {

        if (keyLog[i] != 0) {

            activeChars++;
        }
    }

    // If the caller tries to ask for too many chars, limit them
    if (activeChars < num) {

        num = activeChars;
    }

    for (uint8_t i = 0; i < num; i++) {

        dest[i] = keyLog[i];
    }
}
