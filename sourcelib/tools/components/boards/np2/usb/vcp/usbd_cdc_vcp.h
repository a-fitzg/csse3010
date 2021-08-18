/**
  ******************************************************************************
  * @file    usbd_cdc_vcp.h
  * @author  MCD Application Team
  * @version V1.1.0
  * @date    19-March-2012
  * @brief   Header for usbd_cdc_vcp.c file.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2012 STMicroelectronics</center></h2>
  *
  * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        http://www.st.com/software_license_agreement_liberty_v2
  *
  * Unless required by applicable law or agreed to in writing, software 
  * distributed under the License is distributed on an "AS IS" BASIS, 
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __USBD_CDC_VCP_H
#define __USBD_CDC_VCP_H

/* Includes ------------------------------------------------------------------*/
#ifdef STM32F2XX
 #include "stm32f2xx.h"
#elif defined(STM32F10X_CL)
 #include "stm32f10x.h"
#endif /* STM32F2XX */

#include "usbd_conf.h"
#include "usbd_cdc.h"

#define VCP_RX_BUFFER_SIZE	200			//Size of ring buffer


/* Exported typef ------------------------------------------------------------*/
/* The following structures groups all needed parameters to be configured for the 
   ComPort. These parameters can modified on the fly by the host through CDC class
   command class requests. */
typedef struct
{
  uint32_t bitrate;
  uint8_t  format;
  uint8_t  paritytype;
  uint8_t  datatype;
}LINE_CODING;

extern USBD_CDC_ItfTypeDef  USBD_CDC_VCP_fops;
USBD_HandleTypeDef hUSBDDevice;

//extern uint8_t  APP_Rx_Buffer []; /* Write CDC received data in this buffer.
                                     /*These data will be sent over USB IN endpoint
                                     in the CDC core functions. */
uint32_t APP_Rx_ptr_in;    /* Increment this pointer or roll it back to
                                     start address when writing received data
                                     in the buffer APP_Rx_Buffer. */

uint8_t VCP_Rx_Buffer[VCP_RX_BUFFER_SIZE];		//Ring buffer to hold received characters
uint8_t VCP_Tx_Buffer[VCP_RX_BUFFER_SIZE];		//Ring buffer to hold received characters


#define DEFAULT_CONFIG                  0
#define OTHER_CONFIG                    1

/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
uint16_t VCP_Transmit   (uint8_t* Buf, uint32_t Len);
int VCP_getc(uint8_t *rxbyte);
uint16_t VCP_putc (char c);
uint16_t VCP_puts(char *c);
uint16_t VCP_putm(char *c, int len);
void VCP_txflush(void);
void VCP_rxflush(void);
int VCP_getbuffer(uint8_t *buffer);

#endif /* __USBD_CDC_VCP_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
