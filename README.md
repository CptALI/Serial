This is a repository made to enable my computer to have a wireless connection with another HC-06 module.

It needs an arduino device to be connected via serial COM port to the computer and a HC-06 module to be wired as described below:
STATE -> Not connected
EN -> 5v
RX -> Arduino RX with a voltage devider(Not necessary in short runs)
TX -> Arduino TX
GND -> GND
VCC -> 5v

If starting with a new module, baud rate has to be set to 38400 using AT commands
*NOTICE: some boards have 9600 default and some have 38400 
*NOTICE: each and every command whether sending or receiving must end with "\r\n" corresponding to 13th and 10th ASCII charracters
*NOTICE: HC-06 module directly connected to PC must be set to master mode using AT commands
*NOTICE: HC-06 AT commands datasheet is not existant and does have differences with HC-05 for example commands that get information do not end with "?" in HC-06 and HC+INQ (response...) HC+CONN(number) is the only way connect to a slave device

The sccond HC-06 module is connected to an arduino device (robot side) and the wiring is as described:
EN -> not connected

