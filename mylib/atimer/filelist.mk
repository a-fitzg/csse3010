###################################################
# LIBRARY SOURCES
###################################################

ATIMER_MYLIBPATH=$(HOME)/csse3010/mylib/atimer
CONFIG_STAGEPATH=$(HOME)/csse3010/stages/stage2

# Set folder path with header files to include.
CFLAGS += -I$(ATIMER_MYLIBPATH)
CFLAGS += -I$(CONFIG_STAGEPATH)

# List all c files locations that must be included (use space as separate e.g. LIBSRCS += path_to/file1.c path_to/file2.c)
LIBSRCS += $(ATIMER_MYLIBPATH)/s4533087_hal_atimer.c
