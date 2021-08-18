###################################################
# LIBRARY SOURCES
###################################################

KEYPAD_MYLIBPATH=$(HOME)/csse3010/mylib/keypad

# Set folder path with header files to include.
CFLAGS += -I$(KEYPAD_MYLIBPATH)

# List all c files locations that must be included (use space as separate e.g. LIBSRCS += path_to/file1.c path_to/file2.c)
LIBSRCS += $(KEYPAD_MYLIBPATH)/s4533087_hal_keypad.c
