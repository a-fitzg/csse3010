###################################################
# LIBRARY SOURCES
###################################################

HAMMING_MYLIBPATH=$(HOME)/csse3010/mylib/hamming

# Set folder path with header files to include.
CFLAGS += -I$(HAMMING_MYLIBPATH)

# List all c files locations that must be included (use space as separate e.g. LIBSRCS += path_to/file1.c path_to/file2.c)
LIBSRCS += $(HAMMING_MYLIBPATH)/s4533087_lib_hamming.c
