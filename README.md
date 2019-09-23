# ex2_on_board_computer
Ex-Alta 2 on board computer firmware

I got this working without the docker component, you should be able to make the sim by navigating to the MakeFile
in this directory and running 'make all' 

Right now it has relience edge hooked up and creating Ramdisks and whatever I was up to before.

You can find the entry point to the simulator in Project/main.c
Project is where we should do our simulator development. 


make sure to install the sub modules with, this will download the various applications 
$ git submodule update

You can make the simulator and associated apps with 
$ make all

If you just want to link the object files without rebuilding everything, run
$ make SatelliteSim
