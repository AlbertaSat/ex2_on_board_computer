This contains a ‘send’ project (DCAN_TX) to be loaded onto one Hercules development board that transmits a 
message over CAN, and a ‘receive’ project (DCAN_RX) on a second board that receives the message using the CAN 
module. The obc_base folder is not part of the CAN project.

The projects are created to work with the following boards and CAN modules:
- DCAN_TX is to be loaded onto the TMS570LS12x, using the CAN2 pins (J10 pin 12 and J10 pin 13, for rx and tx respectively)
- DCAN_RX is to be loaded onto the TMS570LC43x, using the CAN4 pins (J2 pin 4 and J2 pin 10, for rx and tx respectively)

To run the code:
1. Load DCAN_TX onto the LS12x board
2. Load DCAN_RX onto the LC43x board
3. Debug DCAN_RX on the LC43x board
4. Plug in (or reset) the LS12x board to run the DCAN_TX code
5. You should see output on the console of the DCAN_RX project showing the program status and the message
sent by DCAN_TX (you should receive the message "ALBERTAS" over CAN)

This code is based off the following tutorial: https://training.ti.com/hercules-how-tutorial-can-communication