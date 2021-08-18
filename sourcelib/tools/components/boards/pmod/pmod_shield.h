/* board.h
 * Created: 2/1/2020
 * Updated: 2/1/2020
 * Author: mds
 */

#ifndef PMOD_SHIELD_H
#define PMOD_SHIELD_H

#include <stdio.h>
#include "board.h"
#include "processor_hal.h"


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

#define PMOD_SDA              BRD_D14               // Mask
#define PMOD_SDA_PIN          BRD_D14_PIN           // Pin Index
#define PMOD_SDA_GPIO_PORT    BRD_D14_GPIO_PORT     // GPIO Port for pin
#define __PMOD_SDA_GPIO_CLK() __BRD_D14_GPIO_CLK()  // GPIO Port clock
#define PMOD_SDA_EXTI_IRQ     BRD_D14_EXTI_IRQ      // External interrupt IRQ

#define PMOD_SCL              BRD_D15               // Mask
#define PMOD_SCL_PIN          BRD_D15_PIN           // Pin Index
#define PMOD_SCL_GPIO_PORT    BRD_D15_GPIO_PORT     // GPIO Port for pin
#define __PMOD_SCL_GPIO_CLK() __BRD_D15_GPIO_CLK()  // GPIO Port clock
#define PMOD_SCL_EXTI_IRQ     BRD_D15_EXTI_IRQ      // External interrupt IRQ

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
// PMOD J4 Connector
////////////////////////////////////////////////////
#define PMOD_A0              BRD_A0               // Mask
#define PMOD_A0_PIN          BRD_A0_PIN           // Pin Index
#define PMOD_A0_GPIO_PORT    BRD_A0_GPIO_PORT     // GPIO Port for pin
#define __PMOD_A0_GPIO_CLK() __BRD_A0_GPIO_CLK()  // GPIO Port clock
#define PMOD_A0_EXTI_IRQ     BRD_A0_EXTI_IRQ      // External interrupt IRQ

#define PMOD_A1              BRD_A1               // Mask
#define PMOD_A1_PIN          BRD_A1_PIN           // Pin Index
#define PMOD_A1_GPIO_PORT    BRD_A1_GPIO_PORT     // GPIO Port for pin
#define __PMOD_A1_GPIO_CLK() __BRD_A1_GPIO_CLK()  // GPIO Port clock
#define PMOD_A1_EXTI_IRQ     BRD_A1_EXTI_IRQ      // External interrupt IRQ

#define PMOD_A2              BRD_A2               // Mask
#define PMOD_A2_PIN          BRD_A2_PIN           // Pin Index
#define PMOD_A2_GPIO_PORT    BRD_A2_GPIO_PORT     // GPIO Port for pin
#define __PMOD_A2_GPIO_CLK() __BRD_A2_GPIO_CLK()  // GPIO Port clock
#define PMOD_A2_EXTI_IRQ     BRD_A2_EXTI_IRQ      // External interrupt IRQ

#define PMOD_A3              BRD_A3               // Mask
#define PMOD_A3_PIN          BRD_A3_PIN           // Pin Index
#define PMOD_A3_GPIO_PORT    BRD_A3_GPIO_PORT     // GPIO Port for pin
#define __PMOD_A3_GPIO_CLK() __BRD_A3_GPIO_CLK()  // GPIO Port clock
#define PMOD_A3_EXTI_IRQ     BRD_A3_EXTI_IRQ      // External interrupt IRQ

#define PMOD_A4              BRD_A4               // Mask
#define PMOD_A4_PIN          BRD_A4_PIN           // Pin Index
#define PMOD_A4_GPIO_PORT    BRD_A4_GPIO_PORT     // GPIO Port for pin
#define __PMOD_A4_GPIO_CLK() __BRD_A4_GPIO_CLK()  // GPIO Port clock
#define PMOD_A4_EXTI_IRQ     BRD_A4_EXTI_IRQ      // External interrupt IRQ

#define PMOD_A5              BRD_A5               // Mask
#define PMOD_A5_PIN          BRD_A5_PIN           // Pin Index
#define PMOD_A5_GPIO_PORT    BRD_A5_GPIO_PORT     // GPIO Port for pin
#define __PMOD_A5_GPIO_CLK() __BRD_A5_GPIO_CLK()  // GPIO Port clock
#define PMOD_A5_EXTI_IRQ     BRD_A5_EXTI_IRQ      // External interrupt IRQ

#define PMOD_A6              BRD_D51               // Mask
#define PMOD_A6_PIN          BRD_D51_PIN           // Pin Index
#define PMOD_A6_GPIO_PORT    BRD_D51_GPIO_PORT     // GPIO Port for pin
#define __PMOD_A6_GPIO_CLK() __BRD_D51_GPIO_CLK()  // GPIO Port clock
#define PMOD_A6_EXTI_IRQ     BRD_D51_EXTI_IRQ      // External interrupt IRQ

#define PMOD_A7              BRD_D52               // Mask
#define PMOD_A7_PIN          BRD_D52_PIN           // Pin Index
#define PMOD_A7_GPIO_PORT    BRD_D52_GPIO_PORT     // GPIO Port for pin
#define __PMOD_A7_GPIO_CLK() __BRD_D52_GPIO_CLK()  // GPIO Port clock
#define PMOD_A7_EXTI_IRQ     BRD_D52_EXTI_IRQ      // External interrupt IRQ

#define PMOD_A8              BRD_D53               // Mask
#define PMOD_A8_PIN          BRD_D53_PIN           // Pin Index
#define PMOD_A8_GPIO_PORT    BRD_D53_GPIO_PORT     // GPIO Port for pin
#define __PMOD_A8_GPIO_CLK() __BRD_D53_GPIO_CLK()  // GPIO Port clock
#define PMOD_A8_EXTI_IRQ     BRD_D53_EXTI_IRQ      // External interrupt IRQ

#define PMOD_A9              BRD_D54               // Mask
#define PMOD_A9_PIN          BRD_D54_PIN           // Pin Index
#define PMOD_A9_GPIO_PORT    BRD_D54_GPIO_PORT     // GPIO Port for pin
#define __PMOD_A9_GPIO_CLK() __BRD_D54_GPIO_CLK()  // GPIO Port clock
#define PMOD_A9_EXTI_IRQ     BRD_D54_EXTI_IRQ      // External interrupt IRQ

#define PMOD_A10              BRD_D55               // Mask
#define PMOD_A10_PIN          BRD_D55_PIN           // Pin Index
#define PMOD_A10_GPIO_PORT    BRD_D55_GPIO_PORT     // GPIO Port for pin
#define __PMOD_A10_GPIO_CLK() __BRD_D55_GPIO_CLK()  // GPIO Port clock
#define PMOD_A10_EXTI_IRQ     BRD_D55_EXTI_IRQ      // External interrupt IRQ

////////////////////////////////////////////////////
// PMOD JA Port
////////////////////////////////////////////////////
#define PMOD_JA1              PMOD_26               // Mask
#define PMOD_JA1_PIN          PMOD_26_PIN           // Pin Index
#define PMOD_JA1_GPIO_PORT    PMOD_26_GPIO_PORT     // GPIO Port for pin
#define __PMOD_JA1_GPIO_CLK() __PMOD_26_GPIO_CLK()  // GPIO Port clock
#define PMOD_JA1_EXTI_IRQ     PMOD_26_EXTI_IRQ      // External interrupt IRQ

#define PMOD_JA2              PMOD_27               // Mask
#define PMOD_JA2_PIN          PMOD_27_PIN           // Pin Index
#define PMOD_JA2_GPIO_PORT    PMOD_27_GPIO_PORT     // GPIO Port for pin
#define __PMOD_JA2_GPIO_CLK() __PMOD_27_GPIO_CLK()  // GPIO Port clock
#define PMOD_JA2_EXTI_IRQ     PMOD_27_EXTI_IRQ      // External interrupt IRQ

#define PMOD_JA3              PMOD_28               // Mask
#define PMOD_JA3_PIN          PMOD_28_PIN           // Pin Index
#define PMOD_JA3_GPIO_PORT    PMOD_28_GPIO_PORT     // GPIO Port for pin
#define __PMOD_JA3_GPIO_CLK() __PMOD_28_GPIO_CLK()  // GPIO Port clock
#define PMOD_JA3_EXTI_IRQ     PMOD_28_EXTI_IRQ      // External interrupt IRQ

#define PMOD_JA4              PMOD_29               // Mask
#define PMOD_JA4_PIN          PMOD_29_PIN           // Pin Index
#define PMOD_JA4_GPIO_PORT    PMOD_29_GPIO_PORT     // GPIO Port for pin
#define __PMOD_JA4_GPIO_CLK() __PMOD_29_GPIO_CLK()  // GPIO Port clock
#define PMOD_JA4_EXTI_IRQ     PMOD_29_EXTI_IRQ      // External interrupt IRQ

#define PMOD_JA7              PMOD_30               // Mask
#define PMOD_JA7_PIN          PMOD_30_PIN           // Pin Index
#define PMOD_JA7_GPIO_PORT    PMOD_30_GPIO_PORT     // GPIO Port for pin
#define __PMOD_JA7_GPIO_CLK() __PMOD_30_GPIO_CLK()  // GPIO Port clock
#define PMOD_JA7_EXTI_IRQ     PMOD_30_EXTI_IRQ      // External interrupt IRQ

////////////////////////////////////////////////////
// PMOD JB Port
////////////////////////////////////////////////////
#define PMOD_JB1              PMOD_7               // Mask
#define PMOD_JB1_PIN          PMOD_7_PIN           // Pin Index
#define PMOD_JB1_GPIO_PORT    PMOD_7_GPIO_PORT     // GPIO Port for pin
#define __PMOD_JB1_GPIO_CLK() __PMOD_7_GPIO_CLK()  // GPIO Port clock
#define PMOD_JB1_EXTI_IRQ     PMOD_7_EXTI_IRQ      // External interrupt IRQ

#define PMOD_JB2              PMOD_1               // Mask
#define PMOD_JB2_PIN          PMOD_1_PIN           // Pin Index
#define PMOD_JB2_GPIO_PORT    PMOD_1_GPIO_PORT     // GPIO Port for pin
#define __PMOD_JB2_GPIO_CLK() __PMOD_1_GPIO_CLK()  // GPIO Port clock
#define PMOD_JB2_EXTI_IRQ     PMOD_1_EXTI_IRQ      // External interrupt IRQ

#define PMOD_JB3              PMOD_0               // Mask
#define PMOD_JB3_PIN          PMOD_0_PIN           // Pin Index
#define PMOD_JB3_GPIO_PORT    PMOD_0_GPIO_PORT     // GPIO Port for pin
#define __PMOD_JB3_GPIO_CLK() __PMOD_0_GPIO_CLK()  // GPIO Port clock
#define PMOD_JB3_EXTI_IRQ     PMOD_0_EXTI_IRQ      // External interrupt IRQ

#define PMOD_JB4              PMOD_38               // Mask
#define PMOD_JB4_PIN          PMOD_38_PIN           // Pin Index
#define PMOD_JB4_GPIO_PORT    PMOD_38_GPIO_PORT     // GPIO Port for pin
#define __PMOD_JB4_GPIO_CLK() __PMOD_38_GPIO_CLK()  // GPIO Port clock
#define PMOD_JB4_EXTI_IRQ     PMOD_38_EXTI_IRQ      // External interrupt IRQ

#define PMOD_JB7              PMOD_36               // Mask
#define PMOD_JB7_PIN          PMOD_36_PIN           // Pin Index
#define PMOD_JB7_GPIO_PORT    PMOD_36_GPIO_PORT     // GPIO Port for pin
#define __PMOD_JB7_GPIO_CLK() __PMOD_36_GPIO_CLK()  // GPIO Port clock
#define PMOD_JB7_EXTI_IRQ     PMOD_36_EXTI_IRQ      // External interrupt IRQ

#define PMOD_JB8              PMOD_34               // Mask
#define PMOD_JB8_PIN          PMOD_34_PIN           // Pin Index
#define PMOD_JB8_GPIO_PORT    PMOD_34_GPIO_PORT     // GPIO Port for pin
#define __PMOD_JB8_GPIO_CLK() __PMOD_34_GPIO_CLK()  // GPIO Port clock
#define PMOD_JB8_EXTI_IRQ     PMOD_34_EXTI_IRQ      // External interrupt IRQ

#define PMOD_JB9              PMOD_10               // Mask
#define PMOD_JB9_PIN          PMOD_10_PIN           // Pin Index
#define PMOD_JB9_GPIO_PORT    PMOD_10_GPIO_PORT     // GPIO Port for pin
#define __PMOD_JB9_GPIO_CLK() __PMOD_10_GPIO_CLK()  // GPIO Port clock
#define PMOD_JB9_EXTI_IRQ     PMOD_10_EXTI_IRQ      // External interrupt IRQ

#define PMOD_JB10              PMOD_37               // Mask
#define PMOD_JB10_PIN          PMOD_37_PIN           // Pin Index
#define PMOD_JB10_GPIO_PORT    PMOD_37_GPIO_PORT     // GPIO Port for pin
#define __PMOD_JB10_GPIO_CLK() __PMOD_37_GPIO_CLK()  // GPIO Port clock
#define PMOD_JB10_EXTI_IRQ     PMOD_37_EXTI_IRQ      // External interrupt IRQ

////////////////////////////////////////////////////
// PMOD JC Port
////////////////////////////////////////////////////
#define PMOD_JC7              PMOD_2               // Mask
#define PMOD_JC7_PIN          PMOD_2_PIN           // Pin Index
#define PMOD_JC7_GPIO_PORT    PMOD_2_GPIO_PORT     // GPIO Port for pin
#define __PMOD_JC7_GPIO_CLK() __PMOD_2_GPIO_CLK()  // GPIO Port clock
#define PMOD_JC7_EXTI_IRQ     PMOD_2_EXTI_IRQ      // External interrupt IRQ

#define PMOD_JC8              PMOD_A7               // Mask
#define PMOD_JC8_PIN          PMOD_A7_PIN           // Pin Index
#define PMOD_JC8_GPIO_PORT    PMOD_A7_GPIO_PORT     // GPIO Port for pin
#define __PMOD_JC8_GPIO_CLK() __PMOD_A7_GPIO_CLK()  // GPIO Port clock
#define PMOD_JC8_EXTI_IRQ     PMOD_A7_EXTI_IRQ      // External interrupt IRQ

#define PMOD_JC9              PMOD_A1               // Mask
#define PMOD_JC9_PIN          PMOD_A1_PIN           // Pin Index
#define PMOD_JC9_GPIO_PORT    PMOD_A1_GPIO_PORT     // GPIO Port for pin
#define __PMOD_JC9_GPIO_CLK() __PMOD_A1_GPIO_CLK()  // GPIO Port clock
#define PMOD_JC9_EXTI_IRQ     PMOD_A1_EXTI_IRQ      // External interrupt IRQ

#define PMOD_JC10              PMOD_A6               // Mask
#define PMOD_JC10_PIN          PMOD_A6_PIN           // Pin Index
#define PMOD_JC10_GPIO_PORT    PMOD_A6_GPIO_PORT     // GPIO Port for pin
#define __PMOD_JC10_GPIO_CLK() __PMOD_A6_GPIO_CLK()  // GPIO Port clock
#define PMOD_JC10_EXTI_IRQ     PMOD_A6_EXTI_IRQ      // External interrupt IRQ

////////////////////////////////////////////////////
// PMOD JD Port
////////////////////////////////////////////////////
#define PMOD_JD1              PMOD_A2               // Mask
#define PMOD_JD1_PIN          PMOD_A2_PIN           // Pin Index
#define PMOD_JD1_GPIO_PORT    PMOD_A2_GPIO_PORT     // GPIO Port for pin
#define __PMOD_JD1_GPIO_CLK() __PMOD_A2_GPIO_CLK()  // GPIO Port clock
#define PMOD_JD1_EXTI_IRQ     PMOD_A2_EXTI_IRQ      // External interrupt IRQ

#define PMOD_JD2              PMOD_40               // Mask
#define PMOD_JD2_PIN          PMOD_40_PIN           // Pin Index
#define PMOD_JD2_GPIO_PORT    PMOD_40_GPIO_PORT     // GPIO Port for pin
#define __PMOD_JD2_GPIO_CLK() __PMOD_40_GPIO_CLK()  // GPIO Port clock
#define PMOD_JD2_EXTI_IRQ     PMOD_40_EXTI_IRQ      // External interrupt IRQ

#define PMOD_JD3              PMOD_39               // Mask
#define PMOD_JD3_PIN          PMOD_39_PIN           // Pin Index
#define PMOD_JD3_GPIO_PORT    PMOD_39_GPIO_PORT     // GPIO Port for pin
#define __PMOD_JD3_GPIO_CLK() __PMOD_39_GPIO_CLK()  // GPIO Port clock
#define PMOD_JD3_EXTI_IRQ     PMOD_39_EXTI_IRQ      // External interrupt IRQ

#define PMOD_JD4              PMOD_A5               // Mask
#define PMOD_JD4_PIN          PMOD_A5_PIN           // Pin Index
#define PMOD_JD4_GPIO_PORT    PMOD_A5_GPIO_PORT     // GPIO Port for pin
#define __PMOD_JD4_GPIO_CLK() __PMOD_A5_GPIO_CLK()  // GPIO Port clock
#define PMOD_JD4_EXTI_IRQ     PMOD_A5_EXTI_IRQ      // External interrupt IRQ

#define PMOD_JD8              PMOD_41               // Mask
#define PMOD_JD8_PIN          PMOD_41_PIN           // Pin Index
#define PMOD_JD8_GPIO_PORT    PMOD_41_GPIO_PORT     // GPIO Port for pin
#define __PMOD_JD8_GPIO_CLK() __PMOD_41_GPIO_CLK()  // GPIO Port clock
#define PMOD_JD8_EXTI_IRQ     PMOD_41_EXTI_IRQ      // External interrupt IRQ

#define PMOD_JD9              PMOD_3               // Mask
#define PMOD_JD9_PIN          PMOD_3_PIN           // Pin Index
#define PMOD_JD9_GPIO_PORT    PMOD_3_GPIO_PORT     // GPIO Port for pin
#define __PMOD_JD9_GPIO_CLK() __PMOD_3_GPIO_CLK()  // GPIO Port clock
#define PMOD_JD9_EXTI_IRQ     PMOD_3_EXTI_IRQ      // External interrupt IRQ

#define PMOD_JD10              PMOD_5               // Mask
#define PMOD_JD10_PIN          PMOD_5_PIN           // Pin Index
#define PMOD_JD10_GPIO_PORT    PMOD_5_GPIO_PORT     // GPIO Port for pin
#define __PMOD_JD10_GPIO_CLK() __PMOD_5_GPIO_CLK()  // GPIO Port clock
#define PMOD_JD10_EXTI_IRQ     PMOD_5_EXTI_IRQ      // External interrupt IRQ

////////////////////////////////////////////////////
// PMOD JE Port
////////////////////////////////////////////////////
#define PMOD_JE1              PMOD_A0               // Mask
#define PMOD_JE1_PIN          PMOD_A0_PIN           // Pin Index
#define PMOD_JE1_GPIO_PORT    PMOD_A0_GPIO_PORT     // GPIO Port for pin
#define __PMOD_JE1_GPIO_CLK() __PMOD_A0_GPIO_CLK()  // GPIO Port clock
#define PMOD_JE1_EXTI_IRQ     PMOD_A0_EXTI_IRQ      // External interrupt IRQ

#define PMOD_JE2              PMOD_6               // Mask
#define PMOD_JE2_PIN          PMOD_6_PIN           // Pin Index
#define PMOD_JE2_GPIO_PORT    PMOD_6_GPIO_PORT     // GPIO Port for pin
#define __PMOD_JE2_GPIO_CLK() __PMOD_6_GPIO_CLK()  // GPIO Port clock
#define PMOD_JE2_EXTI_IRQ     PMOD_6_EXTI_IRQ      // External interrupt IRQ

#define PMOD_JE3              PMOD_8               // Mask
#define PMOD_JE3_PIN          PMOD_8_PIN           // Pin Index
#define PMOD_JE3_GPIO_PORT    PMOD_8_GPIO_PORT     // GPIO Port for pin
#define __PMOD_JE3_GPIO_CLK() __PMOD_8_GPIO_CLK()  // GPIO Port clock
#define PMOD_JE3_EXTI_IRQ     PMOD_8_EXTI_IRQ      // External interrupt IRQ

#define PMOD_JE4              PMOD_A8               // Mask
#define PMOD_JE4_PIN          PMOD_A8_PIN           // Pin Index
#define PMOD_JE4_GPIO_PORT    PMOD_A8_GPIO_PORT     // GPIO Port for pin
#define __PMOD_JE4_GPIO_CLK() __PMOD_A8_GPIO_CLK()  // GPIO Port clock
#define PMOD_JE4_EXTI_IRQ     PMOD_A8_EXTI_IRQ      // External interrupt IRQ

#define PMOD_JE7              PMOD_A4               // Mask
#define PMOD_JE7_PIN          PMOD_A4_PIN           // Pin Index
#define PMOD_JE7_GPIO_PORT    PMOD_A4_GPIO_PORT     // GPIO Port for pin
#define __PMOD_JE7_GPIO_CLK() __PMOD_A4_GPIO_CLK()  // GPIO Port clock
#define PMOD_JE7_EXTI_IRQ     PMOD_A4_EXTI_IRQ      // External interrupt IRQ

#define PMOD_JE8              PMOD_9               // Mask
#define PMOD_JE8_PIN          PMOD_9_PIN           // Pin Index
#define PMOD_JE8_GPIO_PORT    PMOD_9_GPIO_PORT     // GPIO Port for pin
#define __PMOD_JE8_GPIO_CLK() __PMOD_9_GPIO_CLK()  // GPIO Port clock
#define PMOD_JE8_EXTI_IRQ     PMOD_9_EXTI_IRQ      // External interrupt IRQ

#define PMOD_JE9              PMOD_35               // Mask
#define PMOD_JE9_PIN          PMOD_35_PIN           // Pin Index
#define PMOD_JE9_GPIO_PORT    PMOD_35_GPIO_PORT     // GPIO Port for pin
#define __PMOD_JE9_GPIO_CLK() __PMOD_35_GPIO_CLK()  // GPIO Port clock
#define PMOD_JE9_EXTI_IRQ     PMOD_35_EXTI_IRQ      // External interrupt IRQ

#define PMOD_JE10              PMOD_A10               // Mask
#define PMOD_JE10_PIN          PMOD_A10_PIN           // Pin Index
#define PMOD_JE10_GPIO_PORT    PMOD_A10_GPIO_PORT     // GPIO Port for pin
#define __PMOD_JE10_GPIO_CLK() __PMOD_A10_GPIO_CLK()  // GPIO Port clock
#define PMOD_JE10_EXTI_IRQ     PMOD_A10_EXTI_IRQ      // External interrupt IRQ

#endif