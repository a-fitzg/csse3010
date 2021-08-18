# put your *.o targets here, make should handle the rest!

#BRD_PATH=$(SOURCELIB_ROOT)/components/boards/nucleo-f429zi
#HAL_PATH=$(SOURCELIB_ROOT)/components/hal/stm32/STM32F4xx_HAL_Driver
BRD_PATH=$(SOURCELIB_ROOT)/components/boards/nucleo-l496zg
HAL_PATH=$(SOURCELIB_ROOT)/components/hal/stm32/STM32L4xx_HAL_Driver
UTIL_PATH=$(SOURCELIB_ROOT)/components/util
CMSIS_PATH=$(SOURCELIB_ROOT)/components/hal/stm32/CMSIS


PROJ_NAME=main

SRCS = $(PROJ_NAME).c
SRCS += $(BRD_PATH)/Src/*.c
SRCS += $(HAL_PATH)/Src/*.c #$(UTIL_PATH)/*.c
#SRCS += $(FREERTOS_PATH)/*.c $(FREERTOS_PATH)/portable/GCC/ARM_CM4F/*.c $(FREERTOS_PATH)/portable/MemMang/heap_3.c
#SRCS += $(CLI_PATH)/*.c
#SRCS += $(PERIPH_PATH)/nrf24l01plus/*.c
# all the files will be generated with this name (main.elf, main.bin, main.hex, etc)

###################################################

CC=arm-none-eabi-gcc
OBJCOPY=arm-none-eabi-objcopy

#CFLAGS  = -g -O1 -T$(BRD_PATH)/STM32F429ZITx_FLASH.ld -Wmaybe-uninitialized
CFLAGS  = -g -O1 -T$(BRD_PATH)/STM32L496ZGTx_FLASH.ld -Wmaybe-uninitialized
CFLAGS += -mlittle-endian -mthumb -mcpu=cortex-m4 -mthumb-interwork
CFLAGS += -mfloat-abi=hard -mfpu=fpv4-sp-d16

###################################################

vpath %.c src
vpath %.a lib

ROOT=$(shell pwd)

CFLAGS += -I. -I..
CFLAGS += -I$(CMSIS_PATH)/Include -I$(BRD_PATH)/Inc -I$(HAL_PATH)/Inc -I$(UTIL_PATH)
#CFLAGS += -I$(FREERTOS_PATH)/include -I$(FREERTOS_PATH)/portable/GCC/ARM_CM4F
#CFLAGS += -I$(CLI_PATH)
#CFLAGS += -I$(PERIPH_PATH)/nrf24l01plus/ -I$(PERIPH_PATH)/mpu6050/
#CFLAGS += -DENABLE_VCP #Enable USB VCP for debug_printf
#CFLAGS += -DENABLE_FRVCP #Enable USB VCP for debug_printf with FreeRTOS
CFLAGS += -DENABLE_DEBUG_UART	#Enable for debug printf
#CFLAGS += -DSTM32F429xx
CFLAGS += -DSTM32L496xx
#CFLAGS += -DHAVE_PLATFORMCONF_H

#SRCS += $(BRD_PATH)/startup_stm32f429xx.s # add startup file to build
SRCS += $(BRD_PATH)/startup_stm32l496xx.s # add startup file to build

OBJS = $(SRCS:.c=.o)

###################################################

.PHONY: lib proj

all: proj

lib:
	$(MAKE) -C lib

proj: 	$(PROJ_NAME).elf

$(PROJ_NAME).elf: $(SRCS)
	$(CC) $(CFLAGS) $^ -o $@ -Llib -lm -lc -lgcc -lnosys #-lstm32f4
	$(OBJCOPY) -O ihex $(PROJ_NAME).elf $(PROJ_NAME).hex
	$(OBJCOPY) -O binary $(PROJ_NAME).elf $(PROJ_NAME).bin

prog:
	sudo dfu-util -d 0483:df11 -c 1 -i 0 -a 0 -s 0x08000000 -D $(PROJ_NAME).bin

pron:
	sudo st-flash write $(PROJ_NAME).bin 0x8000000					#Program Nucleo

clean:
	rm -f *.o
	rm -f $(PROJ_NAME).elf
	rm -f $(PROJ_NAME).hex
	rm -f $(PROJ_NAME).bin
