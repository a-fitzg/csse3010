/* board.h
 * Created: 2/1/2020
 * Updated: 2/1/2020
 * Author: mds
 */

#ifndef PMOD_BASIC_H
#define PMOD_BASIC_H

#include <stdio.h>
#include "board.h"
#include "processor_hal.h"

#define PMOD_A0              BRD_A0                 // ADC ADC123_IN3 Mask
#define PMOD_A0_PIN          BRD_A0_PIN             // Pin Index
#define PMOD_A0_GPIO_PORT    BRD_A0_GPIO_PORT       // GPIO Port for pin
#define __PMOD_A0_GPIO_CLK() __BRD_A0_GPIO_CLK()    // GPIO Port clock
#define PMOD_A0_EXTI_IRQ     BRD_A0_EXTI_IRQ        // External interrupt IRQ
#define PMOD_A0_ADC_CHAN     BRD_A0_ADC_CHAN

#define PMOD_A1              BRD_A1                   // ADC ADC123_IN3 Mask
#define PMOD_A1_PIN          BRD_A1_PIN          // Pin Index
#define PMOD_A1_GPIO_PORT    BRD_A1_GPIO_PORT               // GPIO Port for pin
#define __PMOD_A1_GPIO_CLK() __BRD_A1_GPIO_CLK()    // GPIO Port clock
#define PMOD_A1_EXTI_IRQ     BRD_A1_EXTI_IRQ          // External interrupt IRQ
#define PMOD_A1_ADC_CHAN     BRD_A1_ADC_CHAN

#define PMOD_A2              BRD_A2                   // ADC ADC123_IN3 Mask
#define PMOD_A2_PIN          BRD_A2_PIN          // Pin Index
#define PMOD_A2_GPIO_PORT    BRD_A2_GPIO_PORT               // GPIO Port for pin
#define __PMOD_A2_GPIO_CLK() __BRD_A2_GPIO_CLK()    // GPIO Port clock
#define PMOD_A2_EXTI_IRQ     BRD_A2_EXTI_IRQ          // External interrupt IRQ
#define PMOD_A2_ADC_CHAN     BRD_A2_ADC_CHAN

#define PMOD_A3              BRD_A3                   // ADC ADC123_IN3 Mask
#define PMOD_A3_PIN          BRD_A3_PIN          // Pin Index
#define PMOD_A3_GPIO_PORT    BRD_A3_GPIO_PORT               // GPIO Port for pin
#define __PMOD_A3_GPIO_CLK() __BRD_A3_GPIO_CLK()    // GPIO Port clock
#define PMOD_A3_EXTI_IRQ     BRD_A3_EXTI_IRQ          // External interrupt IRQ
#define PMOD_A3_ADC_CHAN     BRD_A3_ADC_CHAN

#define PMOD_A4              BRD_A4                   // ADC ADC123_IN3 Mask
#define PMOD_A4_PIN          BRD_A4_PIN          // Pin Index
#define PMOD_A4_GPIO_PORT    BRD_A4_GPIO_PORT               // GPIO Port for pin
#define __PMOD_A4_GPIO_CLK() __BRD_A4_GPIO_CLK()    // GPIO Port clock
#define PMOD_A4_EXTI_IRQ     BRD_A4_EXTI_IRQ          // External interrupt IRQ
#define PMOD_A4_ADC_CHAN     BRD_A4_ADC_CHAN

#define PMOD_A5              BRD_A5                   // ADC ADC123_IN3 Mask
#define PMOD_A5_PIN          BRD_A5_PIN          // Pin Index
#define PMOD_A5_GPIO_PORT    BRD_A5_GPIO_PORT               // GPIO Port for pin
#define __PMOD_A5_GPIO_CLK() __BRD_A5_GPIO_CLK()    // GPIO Port clock
#define PMOD_A5_EXTI_IRQ     BRD_A5_EXTI_IRQ          // External interrupt IRQ
#define PMOD_A5_ADC_CHAN     BRD_A5_ADC_CHAN

////////////////////////////////////////////////////
// PMOD J3 Connector
////////////////////////////////////////////////////
#define PMOD_0              BRD_D0               // Mask
#define PMOD_0_PIN          BRD_D0_PIN           // Pin Index
#define PMOD_0_GPIO_PORT    BRD_D0_GPIO_PORT     // GPIO Port for pin
#define __PMOD_0_GPIO_CLK() __BRD_D0_GPIO_CLK()  // GPIO Port clock
#define PMOD_0_EXTI_IRQ     BRD_D0_EXTI_IRQ      // External interrupt IRQ

#define PMOD_1              BRD_D1               // Mask
#define PMOD_1_PIN          BRD_D1_PIN           // Pin Index
#define PMOD_1_GPIO_PORT    BRD_D1_GPIO_PORT     // GPIO Port for pin
#define __PMOD_1_GPIO_CLK() __BRD_D1_GPIO_CLK()  // GPIO Port clock
#define PMOD_1_EXTI_IRQ     BRD_D1_EXTI_IRQ      // External interrupt IRQ

#define PMOD_2              BRD_D2               // Mask
#define PMOD_2_PIN          BRD_D2_PIN           // Pin Index
#define PMOD_2_GPIO_PORT    BRD_D2_GPIO_PORT     // GPIO Port for pin
#define __PMOD_2_GPIO_CLK() __BRD_D2_GPIO_CLK()  // GPIO Port clock
#define PMOD_2_EXTI_IRQ     BRD_D2_EXTI_IRQ      // External interrupt IRQ

#define PMOD_3              BRD_D3               // Mask
#define PMOD_3_PIN          BRD_D3_PIN           // Pin Index
#define PMOD_3_GPIO_PORT    BRD_D3_GPIO_PORT     // GPIO Port for pin
#define __PMOD_3_GPIO_CLK() __BRD_D3_GPIO_CLK()  // GPIO Port clock
#define PMOD_3_EXTI_IRQ     BRD_D3_EXTI_IRQ      // External interrupt IRQ

#define PMOD_4              BRD_D4               // Mask
#define PMOD_4_PIN          BRD_D4_PIN           // Pin Index
#define PMOD_4_GPIO_PORT    BRD_D4_GPIO_PORT     // GPIO Port for pin
#define __PMOD_4_GPIO_CLK() __BRD_D4_GPIO_CLK()  // GPIO Port clock
#define PMOD_4_EXTI_IRQ     BRD_D4_EXTI_IRQ      // External interrupt IRQ

#define PMOD_5              BRD_D5               // Mask
#define PMOD_5_PIN          BRD_D5_PIN           // Pin Index
#define PMOD_5_GPIO_PORT    BRD_D5_GPIO_PORT     // GPIO Port for pin
#define __PMOD_5_GPIO_CLK() __BRD_D5_GPIO_CLK()  // GPIO Port clock
#define PMOD_5_EXTI_IRQ     BRD_D5_EXTI_IRQ      // External interrupt IRQ

#define PMOD_6              BRD_D6               // Mask
#define PMOD_6_PIN          BRD_D6_PIN           // Pin Index
#define PMOD_6_GPIO_PORT    BRD_D6_GPIO_PORT     // GPIO Port for pin
#define __PMOD_6_GPIO_CLK() __BRD_D6_GPIO_CLK()  // GPIO Port clock
#define PMOD_6_EXTI_IRQ     BRD_D6_EXTI_IRQ      // External interrupt IRQ

#define PMOD_7              BRD_D7               // Mask
#define PMOD_7_PIN          BRD_D7_PIN           // Pin Index
#define PMOD_7_GPIO_PORT    BRD_D7_GPIO_PORT     // GPIO Port for pin
#define __PMOD_7_GPIO_CLK() __BRD_D7_GPIO_CLK()  // GPIO Port clock
#define PMOD_7_EXTI_IRQ     BRD_D7_EXTI_IRQ      // External interrupt IRQ

#define PMOD_26              BRD_D27               // Mask
#define PMOD_26_PIN          BRD_D27_PIN           // Pin Index
#define PMOD_26_GPIO_PORT    BRD_D27_GPIO_PORT     // GPIO Port for pin
#define __PMOD_26_GPIO_CLK() __BRD_D27_GPIO_CLK()  // GPIO Port clock
#define PMOD_26_EXTI_IRQ     BRD_D27_EXTI_IRQ      // External interrupt IRQ

#define PMOD_27              BRD_D26               // Mask
#define PMOD_27_PIN          BRD_D26_PIN           // Pin Index
#define PMOD_27_GPIO_PORT    BRD_D26_GPIO_PORT     // GPIO Port for pin
#define __PMOD_27_GPIO_CLK() __BRD_D26_GPIO_CLK()  // GPIO Port clock
#define PMOD_27_EXTI_IRQ     BRD_D26_EXTI_IRQ      // External interrupt IRQ

#define PMOD_28              BRD_A8               // Mask
#define PMOD_28_PIN          BRD_A8_PIN           // Pin Index
#define PMOD_28_GPIO_PORT    BRD_A8_GPIO_PORT     // GPIO Port for pin
#define __PMOD_28_GPIO_CLK() __BRD_A8_GPIO_CLK()  // GPIO Port clock
#define PMOD_28_EXTI_IRQ     BRD_A8_EXTI_IRQ      // External interrupt IRQ

#define PMOD_29              BRD_A7               // Mask
#define PMOD_29_PIN          BRD_A7_PIN           // Pin Index
#define PMOD_29_GPIO_PORT    BRD_A7_GPIO_PORT     // GPIO Port for pin
#define __PMOD_29_GPIO_CLK() __BRD_A7_GPIO_CLK()  // GPIO Port clock
#define PMOD_29_EXTI_IRQ     BRD_A7_EXTI_IRQ      // External interrupt IRQ

#define PMOD_30              BRD_A6               // Mask
#define PMOD_30_PIN          BRD_A6_PIN           // Pin Index
#define PMOD_30_GPIO_PORT    BRD_A6_GPIO_PORT     // GPIO Port for pin
#define __PMOD_30_GPIO_CLK() __BRD_A6_GPIO_CLK()  // GPIO Port clock
#define PMOD_30_EXTI_IRQ     BRD_A6_EXTI_IRQ      // External interrupt IRQ

////////////////////////////////////////////////////
// PMOD J1 Connector
////////////////////////////////////////////////////
#define PMOD_8              BRD_D8               // Mask
#define PMOD_8_PIN          BRD_D8_PIN           // Pin Index
#define PMOD_8_GPIO_PORT    BRD_D8_GPIO_PORT     // GPIO Port for pin
#define __PMOD_8_GPIO_CLK() __BRD_D8_GPIO_CLK()  // GPIO Port clock
#define PMOD_8_EXTI_IRQ     BRD_D8_EXTI_IRQ      // External interrupt IRQ

#define PMOD_9              BRD_D9               // Mask
#define PMOD_9_PIN          BRD_D9_PIN           // Pin Index
#define PMOD_9_GPIO_PORT    BRD_D9_GPIO_PORT     // GPIO Port for pin
#define __PMOD_9_GPIO_CLK() __BRD_D9_GPIO_CLK()  // GPIO Port clock
#define PMOD_9_EXTI_IRQ     BRD_D9_EXTI_IRQ      // External interrupt IRQ

#define PMOD_10              BRD_D10               // Mask
#define PMOD_10_PIN          BRD_D10_PIN           // Pin Index
#define PMOD_10_GPIO_PORT    BRD_D10_GPIO_PORT     // GPIO Port for pin
#define __PMOD_10_GPIO_CLK() __BRD_D10_GPIO_CLK()  // GPIO Port clock
#define PMOD_10_EXTI_IRQ     BRD_D10_EXTI_IRQ      // External interrupt IRQ

#define PMOD_11              BRD_D11               // Mask
#define PMOD_11_PIN          BRD_D11_PIN           // Pin Index
#define PMOD_11_GPIO_PORT    BRD_D11_GPIO_PORT     // GPIO Port for pin
#define __PMOD_11_GPIO_CLK() __BRD_D11_GPIO_CLK()  // GPIO Port clock
#define PMOD_11_EXTI_IRQ     BRD_D11_EXTI_IRQ      // External interrupt IRQ

#define PMOD_12              BRD_D12               // Mask
#define PMOD_12_PIN          BRD_D12_PIN           // Pin Index
#define PMOD_12_GPIO_PORT    BRD_D12_GPIO_PORT     // GPIO Port for pin
#define __PMOD_12_GPIO_CLK() __BRD_D12_GPIO_CLK()  // GPIO Port clock
#define PMOD_12_EXTI_IRQ     BRD_D12_EXTI_IRQ      // External interrupt IRQ

#define PMOD_13              BRD_D13               // Mask
#define PMOD_13_PIN          BRD_D13_PIN           // Pin Index
#define PMOD_13_GPIO_PORT    BRD_D13_GPIO_PORT     // GPIO Port for pin
#define __PMOD_13_GPIO_CLK() __BRD_D13_GPIO_CLK()  // GPIO Port clock
#define PMOD_13_EXTI_IRQ     BRD_D13_EXTI_IRQ      // External interrupt IRQ

#define PMOD_34              BRD_D25               // Mask
#define PMOD_34_PIN          BRD_D25_PIN           // Pin Index
#define PMOD_34_GPIO_PORT    BRD_D25_GPIO_PORT     // GPIO Port for pin
#define __PMOD_34_GPIO_CLK() __BRD_D25_GPIO_CLK()  // GPIO Port clock
#define PMOD_34_EXTI_IRQ     BRD_D25_EXTI_IRQ      // External interrupt IRQ

#define PMOD_35              BRD_D24               // Mask
#define PMOD_35_PIN          BRD_D24_PIN           // Pin Index
#define PMOD_35_GPIO_PORT    BRD_D24_GPIO_PORT     // GPIO Port for pin
#define __PMOD_35_GPIO_CLK() __BRD_D24_GPIO_CLK()  // GPIO Port clock
#define PMOD_35_EXTI_IRQ     BRD_D24_EXTI_IRQ      // External interrupt IRQ

#define PMOD_36              BRD_D23               // Mask
#define PMOD_36_PIN          BRD_D23_PIN           // Pin Index
#define PMOD_36_GPIO_PORT    BRD_D23_GPIO_PORT     // GPIO Port for pin
#define __PMOD_36_GPIO_CLK() __BRD_D23_GPIO_CLK()  // GPIO Port clock
#define PMOD_36_EXTI_IRQ     BRD_D23_EXTI_IRQ      // External interrupt IRQ

#define PMOD_37              BRD_D22               // Mask
#define PMOD_37_PIN          BRD_D22_PIN           // Pin Index
#define PMOD_37_GPIO_PORT    BRD_D22_GPIO_PORT     // GPIO Port for pin
#define __PMOD_37_GPIO_CLK() __BRD_D22_GPIO_CLK()  // GPIO Port clock
#define PMOD_37_EXTI_IRQ     BRD_D22_EXTI_IRQ      // External interrupt IRQ

#define PMOD_38              BRD_D21               // Mask
#define PMOD_38_PIN          BRD_D21_PIN           // Pin Index
#define PMOD_38_GPIO_PORT    BRD_D21_GPIO_PORT     // GPIO Port for pin
#define __PMOD_38_GPIO_CLK() __BRD_D21_GPIO_CLK()  // GPIO Port clock
#define PMOD_38_EXTI_IRQ     BRD_D21_EXTI_IRQ      // External interrupt IRQ

#define PMOD_39              BRD_D20               // Mask
#define PMOD_39_PIN          BRD_D20_PIN           // Pin Index
#define PMOD_39_GPIO_PORT    BRD_D20_GPIO_PORT     // GPIO Port for pin
#define __PMOD_39_GPIO_CLK() __BRD_D20_GPIO_CLK()  // GPIO Port clock
#define PMOD_39_EXTI_IRQ     BRD_D20_EXTI_IRQ      // External interrupt IRQ

#define PMOD_40              BRD_D19               // Mask
#define PMOD_40_PIN          BRD_D19_PIN           // Pin Index
#define PMOD_40_GPIO_PORT    BRD_D19_GPIO_PORT     // GPIO Port for pin
#define __PMOD_40_GPIO_CLK() __BRD_D19_GPIO_CLK()  // GPIO Port clock
#define PMOD_40_EXTI_IRQ     BRD_D19_EXTI_IRQ      // External interrupt IRQ

#define PMOD_41              BRD_D18               // Mask
#define PMOD_41_PIN          BRD_D18_PIN           // Pin Index
#define PMOD_41_GPIO_PORT    BRD_D18_GPIO_PORT     // GPIO Port for pin
#define __PMOD_41_GPIO_CLK() __BRD_D18_GPIO_CLK()  // GPIO Port clock
#define PMOD_41_EXTI_IRQ     BRD_D18_EXTI_IRQ      // External interrupt IRQ

////////////////////////////////////////////////////
// LED 1 to 5 (6 to 8 are not used)
////////////////////////////////////////////////////
#define PMOD_LD5              PMOD_30               // Mask
#define PMOD_LD5_PIN          PMOD_30_PIN           // Pin Index
#define PMOD_LD5_GPIO_PORT    PMOD_30_GPIO_PORT     // GPIO Port for pin
#define __PMOD_LD5_GPIO_CLK() __PMOD_30_GPIO_CLK()  // GPIO Port clock
#define PMOD_LD5_EXTI_IRQ     PMOD_30_EXTI_IRQ      // External interrupt IRQ

#define PMOD_LD4              PMOD_29               // Mask
#define PMOD_LD4_PIN          PMOD_29_PIN           // Pin Index
#define PMOD_LD4_GPIO_PORT    PMOD_29_GPIO_PORT     // GPIO Port for pin
#define __PMOD_LD4_GPIO_CLK() __PMOD_29_GPIO_CLK()  // GPIO Port clock
#define PMOD_LD4_EXTI_IRQ     PMOD_29_EXTI_IRQ      // External interrupt IRQ

#define PMOD_LD3              PMOD_28               // Mask
#define PMOD_LD3_PIN          PMOD_28_PIN           // Pin Index
#define PMOD_LD3_GPIO_PORT    PMOD_28_GPIO_PORT     // GPIO Port for pin
#define __PMOD_LD3_GPIO_CLK() __PMOD_28_GPIO_CLK()  // GPIO Port clock
#define PMOD_LD3_EXTI_IRQ     PMOD_28_EXTI_IRQ      // External interrupt IRQ

#define PMOD_LD2              PMOD_27               // Mask
#define PMOD_LD2_PIN          PMOD_27_PIN           // Pin Index
#define PMOD_LD2_GPIO_PORT    PMOD_27_GPIO_PORT     // GPIO Port for pin
#define __PMOD_LD2_GPIO_CLK() __PMOD_27_GPIO_CLK()  // GPIO Port clock
#define PMOD_LD2_EXTI_IRQ     PMOD_27_EXTI_IRQ      // External interrupt IRQ

#define PMOD_LD1              PMOD_26               // Mask
#define PMOD_LD1_PIN          PMOD_26_PIN           // Pin Index
#define PMOD_LD1_GPIO_PORT    PMOD_26_GPIO_PORT     // GPIO Port for pin
#define __PMOD_LD1_GPIO_CLK() __PMOD_26_GPIO_CLK()  // GPIO Port clock
#define PMOD_LD1_EXTI_IRQ     PMOD_26_EXTI_IRQ      // External interrupt IRQ

////////////////////////////////////////////////////
// Slide Switch 1 to 4
////////////////////////////////////////////////////
#define PMOD_SW4              PMOD_35               // Mask
#define PMOD_SW4_PIN          PMOD_35_PIN           // Pin Index
#define PMOD_SW4_GPIO_PORT    PMOD_35_GPIO_PORT     // GPIO Port for pin
#define __PMOD_SW4_GPIO_CLK() __PMOD_35_GPIO_CLK()  // GPIO Port clock
#define PMOD_SW4_EXTI_IRQ     PMOD_35_EXTI_IRQ      // External interrupt IRQ

#define PMOD_SW3              PMOD_8               // Mask
#define PMOD_SW3_PIN          PMOD_8_PIN           // Pin Index
#define PMOD_SW3_GPIO_PORT    PMOD_8_GPIO_PORT     // GPIO Port for pin
#define __PMOD_SW3_GPIO_CLK() __PMOD_8_GPIO_CLK()  // GPIO Port clock
#define PMOD_SW3_EXTI_IRQ     PMOD_8_EXTI_IRQ      // External interrupt IRQ

#define PMOD_SW2              PMOD_7               // Mask
#define PMOD_SW2_PIN          PMOD_7_PIN           // Pin Index
#define PMOD_SW2_GPIO_PORT    PMOD_7_GPIO_PORT     // GPIO Port for pin
#define __PMOD_SW2_GPIO_CLK() __PMOD_7_GPIO_CLK()  // GPIO Port clock
#define PMOD_SW2_EXTI_IRQ     PMOD_7_EXTI_IRQ      // External interrupt IRQ

#define PMOD_SW1              PMOD_2               // Mask
#define PMOD_SW1_PIN          PMOD_2_PIN           // Pin Index
#define PMOD_SW1_GPIO_PORT    PMOD_2_GPIO_PORT     // GPIO Port for pin
#define __PMOD_SW1_GPIO_CLK() __PMOD_2_GPIO_CLK()  // GPIO Port clock
#define PMOD_SW1_EXTI_IRQ     PMOD_2_EXTI_IRQ      // External interrupt IRQ

////////////////////////////////////////////////////
// Push Button 1 to 4
////////////////////////////////////////////////////
#define PMOD_BTN4              PMOD_37               // Mask
#define PMOD_BTN4_PIN          PMOD_37_PIN           // Pin Index
#define PMOD_BTN4_GPIO_PORT    PMOD_37_GPIO_PORT     // GPIO Port for pin
#define __PMOD_BTN4_GPIO_CLK() __PMOD_37_GPIO_CLK()  // GPIO Port clock
#define PMOD_BTN4_EXTI_IRQ     PMOD_37_EXTI_IRQ      // External interrupt IRQ

#define PMOD_BTN3              PMOD_36               // Mask
#define PMOD_BTN3_PIN          PMOD_36_PIN           // Pin Index
#define PMOD_BTN3_GPIO_PORT    PMOD_36_GPIO_PORT     // GPIO Port for pin
#define __PMOD_BTN3_GPIO_CLK() __PMOD_36_GPIO_CLK()  // GPIO Port clock
#define PMOD_BTN3_EXTI_IRQ     PMOD_36_EXTI_IRQ      // External interrupt IRQ

#define PMOD_BTN2              PMOD_34               // Mask
#define PMOD_BTN2_PIN          PMOD_34_PIN           // Pin Index
#define PMOD_BTN2_GPIO_PORT    PMOD_34_GPIO_PORT     // GPIO Port for pin
#define __PMOD_BTN2_GPIO_CLK() __PMOD_34_GPIO_CLK()  // GPIO Port clock
#define PMOD_BTN2_EXTI_IRQ     PMOD_34_EXTI_IRQ      // External interrupt IRQ

#define PMOD_BTN1              PMOD_4               // Mask
#define PMOD_BTN1_PIN          PMOD_4_PIN           // Pin Index
#define PMOD_BTN1_GPIO_PORT    PMOD_4_GPIO_PORT     // GPIO Port for pin
#define __PMOD_BTN1_GPIO_CLK() __PMOD_4_GPIO_CLK()  // GPIO Port clock
#define PMOD_BTN1_EXTI_IRQ     PMOD_4_EXTI_IRQ      // External interrupt IRQ

////////////////////////////////////////////////////
// i2c
////////////////////////////////////////////////////
#define PMOD_SCL              PMOD_A5               // Mask
#define PMOD_SCL_PIN          PMOD_A5_PIN           // Pin Index
#define PMOD_SCL_GPIO_PORT    PMOD_A5_GPIO_PORT     // GPIO Port for pin
#define __PMOD_SCL_GPIO_CLK() __PMOD_A5_GPIO_CLK()  // GPIO Port clock

#define PMOD_SDA              PMOD_A4               // Mask
#define PMOD_SDA_PIN          PMOD_A4_PIN           // Pin Index
#define PMOD_SDA_GPIO_PORT    PMOD_A4_GPIO_PORT     // GPIO Port for pin
#define __PMOD_SDA_GPIO_CLK() __PMOD_A4_GPIO_CLK()  // GPIO Port clock

////////////////////////////////////////////////////
// spi
////////////////////////////////////////////////////
#define PMOD_SCK              PMOD_13               // Mask
#define PMOD_SCK_PIN          PMOD_13_PIN           // Pin Index
#define PMOD_SCK_GPIO_PORT    PMOD_13_GPIO_PORT     // GPIO Port for pin
#define __PMOD_SCK_GPIO_CLK() __PMOD_13_GPIO_CLK()  // GPIO Port clock

#define PMOD_MISO              PMOD_39               // Mask
#define PMOD_MISO_PIN          PMOD_39_PIN           // Pin Index
#define PMOD_MISO_GPIO_PORT    PMOD_39_GPIO_PORT     // GPIO Port for pin
#define __PMOD_MISO_GPIO_CLK() __PMOD_39_GPIO_CLK()  // GPIO Port clock

#define PMOD_MOSI              PMOD_12               // Mask
#define PMOD_MOSI_PIN          PMOD_12_PIN           // Pin Index
#define PMOD_MOSI_GPIO_PORT    PMOD_12_GPIO_PORT     // GPIO Port for pin
#define __PMOD_MOSI_GPIO_CLK() __PMOD_12_GPIO_CLK()  // GPIO Port clock

#define PMOD_SS              PMOD_A3               // Mask
#define PMOD_SS_PIN          PMOD_A3_PIN           // Pin Index
#define PMOD_SS_GPIO_PORT    PMOD_A3_GPIO_PORT     // GPIO Port for pin
#define __PMOD_SS_GPIO_CLK() __PMOD_A3_GPIO_CLK()  // GPIO Port clock

#define PMOD_VBAT              PMOD_40               // Mask
#define PMOD_VBAT_PIN          PMOD_40_PIN           // Pin Index
#define PMOD_VBAT_GPIO_PORT    PMOD_40_GPIO_PORT     // GPIO Port for pin
#define __PMOD_VBAT_GPIO_CLK() __PMOD_40_GPIO_CLK()  // GPIO Port clock

#define PMOD_VDD              PMOD_38               // Mask
#define PMOD_VDD_PIN          PMOD_38_PIN           // Pin Index
#define PMOD_VDD_GPIO_PORT    PMOD_38_GPIO_PORT     // GPIO Port for pin
#define __PMOD_VDD_GPIO_CLK() __PMOD_38_GPIO_CLK()  // GPIO Port clock

#endif