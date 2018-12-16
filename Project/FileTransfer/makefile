#**********************************************************************
# CSP Project                            							  *
# PACKET STORM 2.0 Makefile                                           *
# Created: September 19, 2017                                         *
# Modified: December 4, 2017                                          *
# *********************************************************************

######## Build files for CSP ########
P_BASEDIR ?= $(CURDIR)

#either freertos, linux, stub, u-boot, win32
P_OS ?= linux

LFLAGS = -L$(P_BASEDIR)/libcsp/build

LIBS		+= 		$(P_BASEDIR)/libcsp/build/libcsp.a -pthread
 
INCLUDES 	+=		-I $(P_BASEDIR)/libcsp/include

SRCS = $(P_BASEDIR)/simple.c

OBJS = $(SRCS:.c=.o)

CC=gcc
CFLAGS 		= -Werror -Wall -g -D_POSIX_C_SOURCE=200809L -D_DEFAULT_SOURCE
CFLAGS 		+= $(INCLUDES)

MAIN = simple
GROUND = ConnectionlessGround
SATELLITE = ConnectionlessSatellite
LOGS=./logs


.PHONY: depend clean


all: $(MAIN)
	@echo  Simple compiler named mycc has been compiled

$(MAIN): $(OBJS) 
	$(CC) $(CFLAGS) $(INCLUDES) -o $(MAIN) $(OBJS) $(LFLAGS) $(LIBS)

ConnectionlessGround: $(GROUND).o
	$(CC) $(CFLAGS) $(INCLUDES) -o $(GROUND) $(GROUND).o $(LFLAGS) $(LIBS)

ConnectionlessSattelite: $(SATELLITE).o
	$(CC) $(CFLAGS) $(INCLUDES) -o $(SATELLITE) $(SATELLITE).o $(LFLAGS) $(LIBS)

.c.o:
	$(CC) $(CFLAGS) $(INCLUDES) -c $<  -o $@

clean:
	$(RM) *.o *~ $(MAIN)

depend: $(SRCS)
	makedepend $(INCLUDES) $^
