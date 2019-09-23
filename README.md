# This is a free Rtos simulator for ex_alta_2


# Starting the Simulator

To run this simulator, you'll need a linux machine. I'm using Ubuntu 18.

In the Project directory is where you'll find our source code. This is the code that
we are actively simulating. The entry point for the Sim is found in main.c. 
If you want to test to see if your code will work with FreeRTOS, make a task, and
add it to main.c.

To install currently running modules:

make sure to install the sub modules (our code) with  
$ git submodule update

You can make the simulator and associated apps with 
$ make all

If you just want to link the object files without rebuilding everything, run
$ make SatelliteSim



# Adding new code

I think the best way to add a new application is to make it into a .a (archive) file. This is just
a bundle of .o files. make an archive file with: 
    $ ar -rsc name_of_archive.a obj.o ob1.o ob2.o ...

In the master makefile (the one in this directory) link your new .a file,
add your .h files. To add your .h files, add it to INCLUDES, to include it in the
include path, followed by your .a file to the STATIC_OBJ varable.

INCLUDES		+= -I$(SRCROOT)/Project/yourproject/include
STATIC_OBJS  	+= $(SRCROOT)/Project/yourproject/your_app.a

Doing this should link your files, and make them run with the simulator, if you want
your app to build when you call 'make all' add a $(make) command to the make all rule.
