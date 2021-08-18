###################################################
# LIBRARY SOURCES
###################################################

MYLIBPATH=$(HOME)/csse3010/mylib
PROJECT_MYLIBPATH=$(MYLIBPATH)/project

# Set folder path with header files to include.
CFLAGS += -I$(PROJECT_MYLIBPATH)
CFLAGS += -I$(MYLIBPATH)/keypad
CFLAGS += -I$(MYLIBPATH)/lta1000g
CFLGAS += -I$(MYLIBPATH)/joystick

# List all c files locations that must be included (use space as separate e.g. LIBSRCS += path_to/file1.c path_to/file2.c)
LIBSRCS +=  $(MYLIBPATH)/keypad/s4533087_os_keypad.c \
            $(MYLIBPATH)/joystick/s4533087_os_joystick.c \
            $(MYLIBPATH)/lta1000g/s4533087_os_lta1000g.c \
            $(PROJECT_MYLIBPATH)/s4533087_hal_pb.c \
            $(PROJECT_MYLIBPATH)/s4533087_os_pb.c \
            $(PROJECT_MYLIBPATH)/s4533087_os_cag_keypad.c \
            $(PROJECT_MYLIBPATH)/s4533087_os_cag_display.c \
            $(PROJECT_MYLIBPATH)/s4533087_os_cag_simulator.c \
            $(PROJECT_MYLIBPATH)/s4533087_os_cag_joystick.c
