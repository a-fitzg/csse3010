###################################################
# LIBRARY SOURCES
###################################################

JOYSTICK_MYLIBPATH=$(HOME)/csse3010/mylib/joystick

# Set folder path with header files to include.
CFLAGS += -I$(JOYSTICK_MYLIBPATH)

# List all c files locations that must be included (use space as separate e.g. LIBSRCS += path_to/file1.c path_to/file2.c)
LIBSRCS += $(JOYSTICK_MYLIBPATH)/s4533087_hal_joystick.c #$(JOYSTICK_MYLIBPATH)/s4533087_os_joystick.c
