# Add additional project sources like this:
# SRCS += X.c
#
# all the files will be generated with this name (main.elf, main.bin, main.hex, etc)

#Name of project
PROJ_NAME=main

#Your local source files
SRCS = $(PROJ_NAME).c
#ENABLE_VCP=1 #enable USB Virtual Com Port
ENABLE_HID=1	#enable USB HID
ENABLE_DEBUG_UART=1	#enable debug UART
###################################################
ROOT=$(shell pwd)

OBJS=$(SRCS:.c=.o)

###################################################

.PHONY: $(shell mkdir -p obj) lib proj all depend
all: proj

#include NP2 makefile
include $(STM32_ROOT)/src/boards/netduinoplus2/Makefile.np2

lib:
	$(MAKE) -C lib

proj: 	$(PROJ_NAME).elf

$(PROJ_NAME).elf:	$(STM32OBJ) $(OBJS)
	$(CC) $(LDFLAGS) -o $@ -Llib $(OBJS) $(STM32OBJ) $(LD_SYS_LIBS)
	$(OBJCOPY) -O ihex $(PROJ_NAME).elf $(PROJ_NAME).hex
	$(OBJCOPY) -O binary $(PROJ_NAME).elf $(PROJ_NAME).bin

%.o:
	$(CC) $(CFLAGS) -c $< -o $@

depend: .depend

.depend: $(SRCS)
	rm -f ./.depend
	mkdir -p obj
	$(CC) $(CFLAGS) -MM $^>>./.depend;

dfu:
	sudo dfu-util -d 0483:df11 -c 1 -i 0 -a 0 -s 0x08000000 -D $(PROJ_NAME).bin		#Program NP2

flash:
	sudo st-flash write $(PROJ_NAME).bin 0x8000000	#Program Nucleo

erase:
	sudo st-flash erase	#erase Nucleo

qemu:
	np2-qemu $(PROJ_NAME).elf

gdb:
	np2-gdb-qemu $(PROJ_NAME).elf

clean:
	rm -f *.o
	rm -f $(OBJDIR)/*.o
	rm -f $(PROJ_NAME).elf
	rm -f $(PROJ_NAME).hex
	rm -f $(PROJ_NAME).bin
	rm -f $(LIBSTM32)
	rm -rf .depend
	rmdir $(OBJDIR)


include .depend
