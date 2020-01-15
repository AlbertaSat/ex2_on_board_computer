Contains code that tests using two way communication over CAN (without using interrupts) on two TMS570LC43x microcontrollers

The DCAN1 folder contains code that will transmit a message to the second microcontroller and await for a response. DCAN2 contains code for the second microcontroller that will receive the data, modify it, and send a modified version of the data back to the first microcontroller

The DCAN2 code should be started first, as it will sit in a while loop waiting for a message to arrive. The DCAN1 code should be started after the DCAN2 code.

For unknown reasons, the code doesn’t appear to work when both microcontrollers are plugged into my laptop, but has worked when each one is plugged into a seperate computer
