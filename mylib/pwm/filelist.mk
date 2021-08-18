###################################################
# LIBRARY SOURCES
###################################################

PWM_MYLIBPATH=$(HOME)/csse3010/mylib/pwm
CONFIG_STAGEPATH=$(HOME)/csse3010/stages/stage2

# Set folder path with header files to include.
CFLAGS += -I$(PWM_MYLIBPATH)
CFLAGS += -I$(CONFIG_STAGEPATH)

# List all c files locations that must be included (use space as separate e.g. LIBSRCS += path_to/file1.c path_to/file2.c)
LIBSRCS += $(PWM_MYLIBPATH)/s4533087_hal_pwm.c
