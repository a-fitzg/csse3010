###################################################
# LIBRARY SOURCES
###################################################

LTA1000G_MYLIBPATH=$(HOME)/csse3010/mylib/lta1000g

# Set folder path with header files to include.
CFLAGS += -I$(LTA1000G_MYLIBPATH)

# List all c files locations that must be included (use space as separate e.g. LIBSRCS += path_to/file1.c path_to/file2.c)
LIBSRCS += $(LTA1000G_MYLIBPATH)/s4533087_hal_lta1000g.c 
