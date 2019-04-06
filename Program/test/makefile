
PROJDIR = $(CURDIR)/..
INCLUDE += -I$(PROJDIR)/include
CC = gcc 

#---------------------------File Names---------------------------
CFILES += $(PROJDIR)/src/utils.c
CFILES +=  $(PROJDIR)/src/filesystem_funcs.c
CFILES += $(PROJDIR)/src/port.c
CFILES += $(PROJDIR)/src/server.c
CFILES += $(PROJDIR)/src/protocol_handler.c
CFILES += $(PROJDIR)/src/list_callbacks.c
CFILES += $(PROJDIR)/src/mib.c

#entrypoint
MAIN = main

#---------------------------Compiler Warnings---------------------------
CWARNS += -W
CWARNS += -Wall
#CWARNS += -Werror
CWARNS += -Wextra
CWARNS += -Wformat
CWARNS += -Wmissing-braces
CWARNS += -Wno-cast-align
CWARNS += -Wparentheses
CWARNS += -Wshadow
CWARNS += -Wno-sign-compare
CWARNS += -Wswitch
CWARNS += -Wuninitialized
CWARNS += -Wunknown-pragmas
CWARNS += -Wunused-function
CWARNS += -Wunused-label
CWARNS += -Wunused-parameter
CWARNS += -Wunused-value
CWARNS += -Wunused-variable
CWARNS += -Wmissing-prototypes

#---------------------------Libs---------------------------
LINKFLAGS = 
LIBS = 

#---------------------------Compiler flags---------------------------

#32 bit machines, leave this out if your machine is 64 bit
CFLAGS += -m32
CFLAGS += -g -UUSE_STDIO -D__GCC_POSIX__=1
CFLAGS += -pthread
CFLAGS += -DDEBUG=1

# MAX_NUMBER_OF_TASKS = max pthreads used in the POSIX port. 
# Default value is 64 (_POSIX_THREAD_THREADS_MAX), the minimum number required by POSIX.
CFLAGS += -DMAX_NUMBER_OF_TASKS=300
CFLAGS += $(INCLUDE) $(CWARNS)

#---------------------------Build---------------------------
OBJS_FILES = $(patsubst %.c, %.o, $(CFILES))

all: $(MAIN)

.PHONY: clean 

$(MAIN): $(OBJS_FILES)

clean: 
	rm -f *.o $(MAIN)



