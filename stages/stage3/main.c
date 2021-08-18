/**
  *****************************************************************************
  * @file       stage3/main.c
  * @author     Alexander FitzGerald - s4533087
  * @date       08042020
  * @brief      Stage 3 for CSSE3010
  *****************************************************************************
  */

/* Includes ---------------------------------------------------------------- */
#include "board.h"
#include "processor_hal.h"
#include "debug_printf.h"
#include "s4533087_hal_atimer.h"
#include "s4533087_lib_hamming.h"
#include "s4533087_hal_keypad.h"

#ifdef S4533087_MYLIB_CONFIGURE_INC
#include "s4533087_mylib_configure.h"
#endif

/* Private typedef --------------------------------------------------------- */
/* Private define ---------------------------------------------------------- */
#define INIT_STATE      0
#define IDLE_STATE      1
#define ENCODE_STATE    2
#define DECODE_STATE    3

/* Private macro ----------------------------------------------------------- */
/* Private variables ------------------------------------------------------- */
char currentKey;
char previousKey;
uint8_t hammingFsmState;
/* Private function prototypes --------------------------------------------- */
uint8_t hex_to_byte(char, char);
uint8_t hex_to_nibble(char);
void print_binary(uint8_t, uint8_t);
uint8_t char_to_hex(char);

/**
  * @brief      Main
  * @param      None
  * @retval     None
  */    
int main(void) {

    BRD_init();

    s4533087_hal_atimer_init();
    s4533087_hal_atimer_clkspeed_set(2000);
    s4533087_hal_atimer_period_set(1);
    
    s4533087_hal_keypad_init();

    hammingFsmState = IDLE_STATE;
    currentKey = 0;
    previousKey = 0;
    uint8_t fullByte = 0;
    uint8_t recentTransition = 0;

    while (1) {
        
        // Only proceed in code if the timer has ticked over 2ms
        // ###### The spec says to use 50ms here, but the keypad is too ######
        // ###### unresponsive at this period and is difficult to use.  ######
        if (s4533087_hal_atimer_timer_getms() < 2) {
            
            continue;
        }
        s4533087_hal_atimer_timer_reset();
        
        s4533087_hal_keypad_fsmprocessing();

        char asciiValue = s4533087_hal_keypad_read_ascii();
        if (asciiValue) {
            
            debug_printf("<%c>", asciiValue);
            previousKey = currentKey;
            currentKey = asciiValue;
        }

        // Hamming FSM
        switch(hammingFsmState) {
    
            case IDLE_STATE:
                
                // Waiting for instructions
                if (previousKey == 'D' && currentKey == 'D') {
                        
                    hammingFsmState = DECODE_STATE;
                    fullByte = 0;
                    recentTransition = 1;
                    break;
                }
                if (previousKey == 'E' && currentKey == 'E') {
                    
                    hammingFsmState = ENCODE_STATE;
                    fullByte = 0;
                    recentTransition = 1;
                    break;
                }
                break;

            case ENCODE_STATE:
                
                // Encoding
                if (asciiValue) {

                    if (fullByte && !recentTransition) {
                        
                        if (previousKey == 'D' && currentKey == 'D') {

                            // Goto decode state
                            hammingFsmState = DECODE_STATE;
                            recentTransition = 1;
                            fullByte = 0;
                            break;
                        }
                        if (previousKey == 'E' && currentKey == 'E') {
                            
                            // Goto encode state
                            hammingFsmState = ENCODE_STATE;
                            recentTransition = 1;
                            fullByte = 0;
                            break;
                        }
                        if (previousKey == 'F' && currentKey == 'F') {
                            
                            // Goto idle state
                            hammingFsmState = IDLE_STATE;
                            recentTransition = 1;
                            fullByte = 0;
                            break;
                        }

                        // Combine both chars into one byte
                        uint8_t rawInput = (char_to_hex(currentKey) & 0x0F) | 
                                ((char_to_hex(previousKey) & 0x0F) << 4);
                        uint16_t encodedValue = 
                                s4533087_lib_hamming_byte_encode(rawInput);

                        // Extract hamming bits out of encoding
                        uint8_t lsnHammingBits = (encodedValue & 0x000F) >> 1;
                        uint8_t msnHammingBits = (encodedValue & 0x0F00) >> 9;
                        debug_printf("\r\nHE %c%x%c%x \r\n", 
                                previousKey, msnHammingBits, 
                                currentKey, lsnHammingBits);

                    }
                    recentTransition = 0;
                    fullByte ^= 1;
                }
                break;
            
            case DECODE_STATE:
                
                // Decoding
                if (asciiValue) {
                    
                    if (fullByte && !recentTransition) {
                        
                        if (previousKey == 'D' && currentKey == 'D') {

                            // Goto decode state
                            hammingFsmState = DECODE_STATE;
                            recentTransition = 1;
                            fullByte = 0;
                            break;
                        }
                        if (previousKey == 'E' && currentKey == 'E') {
                            
                            // Goto encode state
                            hammingFsmState = ENCODE_STATE;
                            recentTransition = 1;
                            fullByte = 0;
                            break;
                        }
                        if (previousKey == 'F' && currentKey == 'F') {
                            
                            // Goto idle state
                            hammingFsmState = IDLE_STATE;
                            recentTransition = 1;
                            fullByte = 0;
                            break;
                        }
                        uint8_t input = (char_to_hex(currentKey) & 0x0F) | 
                                ((char_to_hex(previousKey) & 0x0F) << 4);
                        uint8_t decoding = 0x0F & 
                                s4533087_lib_hamming_byte_decode(input);
                        debug_printf("\r\nHD %x P%x\r\n", decoding, 
                                s4533087_lib_hamming_parity_error(input));
                    }
                    recentTransition = 0;
                    fullByte ^= 1;
                }
                break;
        }
    }
}

/**
  * @brief  Converts 2 chars (interpreted as hex) to a byte
  * @params msn: most significant nibble (left character)
            lsn: least significant nibble (right character)
  * @retval byte, hex value converted to byte
  */
uint8_t hex_to_byte(char msn, char lsn) {

    uint8_t leftNibble = hex_to_nibble(msn);
    uint8_t rightNibble = hex_to_nibble(lsn);
    return (0x0F & rightNibble | (0x0F & leftNibble) << 4);
}

/**
  * @brief  Convert a hex character to a half-byte
  * @params nibble: Char (interpreted as hex) to convert
  * @retval byte, half-byte decoded
  */
uint8_t hex_to_nibble(char nibble) {

    if (nibble >= '0' && nibble <= '9') {
        
        // Numbers between 0 and 9
        return (uint8_t)(nibble - '0');
    } else if (nibble >= 'A' && nibble <= 'F') {
        
        // Numbers between A and F
        return (uint8_t)(nibble - 'A' + 10);
    } else {
        
        // Shouldn't get here, or something's gone wrong
        return 0;
    }
}

/**
  * @brief  Takes byte and prints its binary representation
  * @params byte: Byte to print in binary
  * @retval None
  */
void print_binary(uint8_t byte, uint8_t size) {

    for (int8_t i = (size - 1); i >= 0; --i) {        
        // Prints "i"th bit's binary representation
        debug_printf("%d", !!(byte & (1 << i)));
    }
}

/**
  * @brief  Takes char and returns it in hexadecimal
  * @params input: char representing a hexadecimal digit
  * @retval int: value in hexadecimal
  */
uint8_t char_to_hex(char input) {

    if (input >= '0' && input <= '9') {

        return (uint8_t)(input - '0');
    } else if (input >= 'A' && input <= 'F') {

        return (uint8_t)(input - 'A' + 10);
    } else {

        return 0;
    }
}
