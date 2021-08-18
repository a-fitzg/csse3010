/**
  *****************************************************************************
  * @file    s4533087_lib_hamming.h
  * @author  Alexander FitzGerald - s4533087
  * @date    07042020
  * @brief   Hamming encoding library
  *****************************************************************************
  * EXTERNAL FUNCTIONS
  *****************************************************************************
  * s4533087_lib_hamming_byte_encode()
  * s4533087_lib_hamming_byte_decode()
  * s4533087_lib_hamming_parity_error()
  * s4533087_lib_hamming_syndrome()
  *****************************************************************************
  */

/* Includes ---------------------------------------------------------------- */
#include "board.h"
#include "processor_hal.h"
#include "debug_printf.h"
#include "s4533087_lib_hamming.h"

/* Private typedef --------------------------------------------------------- */
/* Private define ---------------------------------------------------------- */
/* Private macro ----------------------------------------------------------- */
/* Private variables ------------------------------------------------------- */
/* Private function prototypes --------------------------------------------- */
uint8_t hamming_hbyte_encode(uint8_t);

/**
  * @brief  Encodes a byte, returns 16-bit encoding of the byte
  * @params byte: byte to encode
  * @retval 16-bit encoding of input byte
  */
uint16_t s4533087_lib_hamming_byte_encode(uint8_t byte) {

    // Least significant nibble
    uint8_t lsn = byte & 0x0F;
    
    // Most significant nibble
    uint8_t msn = (byte & 0xF0) >> 4;

    // Least significant byte (encoded)
    uint8_t lsb_encoded = hamming_hbyte_encode(lsn);
    
    // Most significant byte (encoded)
    uint8_t msb_encoded = hamming_hbyte_encode(msn);
   
    // Combine 
    uint16_t output = lsb_encoded | (msb_encoded << 8);
    
    return output;
}

/**
  * @brief  Returns decoded half-byte from an input byte
  * @params byte: Encoded input byte to decode
  * @retval half-byte decoding of input byte
  */
uint8_t s4533087_lib_hamming_byte_decode(uint8_t byte) {

    // End parity bit
    uint8_t p0 = !!(byte & 0x01);
    
    // Hamming bits
    uint8_t h0 = !!(byte & 0x02);
    uint8_t h1 = !!(byte & 0x04);
    uint8_t h2 = !!(byte & 0x08);
    
    // Data bits
    uint8_t d0 = !!(byte & 0x10);
    uint8_t d1 = !!(byte & 0x20);
    uint8_t d2 = !!(byte & 0x40);
    uint8_t d3 = !!(byte & 0x80);
   
    // Get syndrome
    uint8_t syndrome = s4533087_lib_hamming_syndrome(byte);

    // Extract syndrome bits
    uint8_t s0 = !!(syndrome & 0x01);
    uint8_t s1 = !!(syndrome & 0x02);
    uint8_t s2 = !!(syndrome & 0x04);   

    // XOR all the bits together, do not correct if XORing equals 0
    if (s4533087_lib_hamming_parity_error(byte) == 0 && syndrome != 0) {

        // 2-bit error, can't correct
        return (byte >> 4);        
    }
    if (syndrome == 0) {
        
        // No errors occured
        return (byte >> 4);
    } else {

        // Else, one-bit correctable error has occured. Correcting here
        if (s0 && !s1 && !s2) {

            // Column 0 error [1 0 0]            
            h0 = !h0;
        } else if (!s0 && s1 && !s2) {
        
            // Column 1 error [0 1 0]
            h1 = !h1;
        } else if (!s0 && !s1 && s2) {

            // Column 2 error [0 0 1]
            h2 = !h2;
        } else if (!s0 && s1 && s2) {
            
            // Column 3 error [0 1 1]
            d0 = !d0;
        } else if (s0 && !s1 && s2) {
            
            // Column 4 error [1 0 1]
            d1 = !d1;
        } else if (s0 && s1 && !s2) {
            
            // Column 5 error [1 1 0]
            d2 = !d2;
        } else if (s0 && s1 && s2) {
            
            // Column 6 error [1 1 1]
            d3 = !d3;
        } else {

            // Shouldn't reach here, something went wrong
            return (byte >> 4);
        }
        // Compose data bits back 
        uint8_t output = (d0 << 0) + (d1 << 1) + (d2 << 2);
        return output;
    }             
}

/**
  * @brief  Returns the syndrome of a hamming byte
  * @params byte: byte to check
  * @retval syndrome
  */
uint8_t s4533087_lib_hamming_syndrome(uint8_t byte) {

    // End parity bit
    uint8_t p0 = !!(byte & 0x01);
    
    // Hamming bits
    uint8_t h0 = !!(byte & 0x02);
    uint8_t h1 = !!(byte & 0x04);
    uint8_t h2 = !!(byte & 0x08);
    
    // Data bits
    uint8_t d0 = !!(byte & 0x10);
    uint8_t d1 = !!(byte & 0x20);
    uint8_t d2 = !!(byte & 0x40);
    uint8_t d3 = !!(byte & 0x80);

    // Calculate syndrome
    uint8_t s0 = h0 ^ d1 ^ d2 ^ d3;
    uint8_t s1 = h1 ^ d0 ^ d2 ^ d3;
    uint8_t s2 = h2 ^ d0 ^ d1 ^ d3;

    uint8_t syndrome = (s0 << 0) + (s1 << 1) + (s2 << 2);
    
    return syndrome;
}

/**
  * @brief  Check for parity error of a byte. 1 = parity error, 0 = no error
  * @params byte: byte to check
  * @retval 1 if error, 0 if no error
  */
uint8_t s4533087_lib_hamming_parity_error(uint8_t byte) {

    // End parity bit
    uint8_t p0 = !!(byte & 0x01);
    
    // Hamming bits
    uint8_t h0 = !!(byte & 0x02);
    uint8_t h1 = !!(byte & 0x04);
    uint8_t h2 = !!(byte & 0x08);
    
    // Data bits
    uint8_t d0 = !!(byte & 0x10);
    uint8_t d1 = !!(byte & 0x20);
    uint8_t d2 = !!(byte & 0x40);
    uint8_t d3 = !!(byte & 0x80);

    uint8_t calculatedParity = (h0 ^ h1 ^ h2 ^ d0 ^ d1 ^ d2 ^ d3);
    if (calculatedParity == p0) {
        
        return 0;
    } else {
        
        return 1;
    }
}

/**
  * @brief  Encodes a half-byte in hamming encoding
  * @params nibble: half-byte to encode
  * @retval byte: hamming encoding of half-byte
  */
uint8_t hamming_hbyte_encode(uint8_t nibble) {

    /* Using matrices:
     *
     * G = [ 0 1 1 | 1 0 0 0
     *       1 0 1 | 0 1 0 0
     *       1 1 0 | 0 0 1 0
     *       1 1 1 | 0 0 0 1 ]
     *
     * H = [ 1 0 0 | 0 1 1 1
     *       0 1 0 | 1 0 1 1 
     *       0 0 1 | 1 1 0 1 ]
     */ 

    // Get individual bits from input
    uint8_t x0 = !!(nibble & 0x01);
    uint8_t x1 = !!(nibble & 0x02);
    uint8_t x2 = !!(nibble & 0x04);
    uint8_t x3 = !!(nibble & 0x08);

    // Find hamming parity bits
    uint8_t h0 = x1 ^ x2 ^ x3;
    uint8_t h1 = x0 ^ x2 ^ x3;
    uint8_t h2 = x0 ^ x1 ^ x3;
    uint8_t p0 = 0;

    // Find output without end parity bit
    uint8_t output = (h0 << 1) | (h1 << 2) | (h2 << 3) | 
            (x0 << 4) | (x1 << 5) | (x2 << 6) | (x3 << 7);
    
    // Calculate end parity bit
    for (int i = 0; i < 7; i++) {
        p0 = p0 ^ !!(output & (1 << i));
    }
    
    // Set p0 (parity bit) to be LSB of output
    output |= p0;
    
    return output;
}
