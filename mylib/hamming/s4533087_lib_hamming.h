/**
  *****************************************************************************
  * @file   s4533087_lib_hamming.h
  * @author Alexander FitzGerald - s4533087
  * @date   07032020
  * @brief  Hamming encoding library header
  *****************************************************************************
  * EXTERNAL FUNCTIONS
  *****************************************************************************
  * s4533087_lib_hamming_byte_encode()
  * s4533087_lib_hamming_byte_decode()
  * s4533087_lib_hamming_parity_error()
  * s4533087_lib_hamming_syndrome()
  * s4533087_lib_hamming_correct_byte();
  *****************************************************************************
  */

#ifndef S4533087_LIB_HAMMING_H
#define S4533087_LIB_HAMMING_H

// Includes
#ifdef S4533087_MYLIB_CONFIGURE_INC
#include "s4533087_mylib_configure.h"
#endif

// Function prototypes
uint16_t s4533087_lib_hamming_byte_encode(uint8_t);
uint8_t s4533087_lib_hamming_byte_decode(uint8_t);
uint8_t s4533087_lib_hamming_parity_error(uint8_t);
uint8_t s4533087_lib_hamming_syndrome(uint8_t);
uint8_t s4533087_lib_hamming_correct_byte(uint8_t);
#endif
