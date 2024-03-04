This is a repository made to enable my computer to have a wireless connection with another HC-06 module.

It needs an arduino device to be connected via serial COM port to the computer and a HC-06 module to be wired as described below:
STATE -> Not connected<br \>
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

STATE -> Not connected
EN -> Not connected
RX -> Arduino TX with a voltage devider(Not necessary in short runs)
TX -> Arduino RX
GND -> GND
VCC -> 5v

After turning on both modules you can use BT_Init function any time so master device connects to the slave BUT:
***IMORTANT NOTICE***
After connecting master to slave, slave print BAD DATA for 2000 milisecconds and everything in this time period MUS BE IGNORED by the arduino borad on receiver side

Data send can be tested to see if we remove the "\r\n" from the data lines we send after BT_INIT success, will any problem rise or not
Currently we can send 920 commands per seccond for the robot but if we remove the last two unnecessary bytes update rate will be 1600 commands per seccond 
