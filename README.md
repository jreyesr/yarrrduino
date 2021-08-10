# Yarrrduino: a Bus Pirate, but for Arduino
> WARNING: Yarrrduino is not being actively developed. Use at your own risk. Anything but the most basic functions are likely to not be implemented.

Yarrrduino is a [Bus Pirate](http://dangerousprototypes.com/docs/Bus_Pirate) port to the Arduino framework, tested on an Arduino Uno. It aims to provide the interactivity and interfacing goodness of the original Bus Pirate, without the need for a specialized piece of hardware, instead replacing it with a _much_ more generic board.

## Pins

[These](http://dangerousprototypes.com/docs/Bus_Pirate_I/O_Pin_Descriptions) is the original Bus Pirate pinout, for reference.

| Bus Pirate pin | Arduino pin          | Functionality                                                |
| -------------- | -------------------- | ------------------------------------------------------------ |
| MOSI           | Digital pin 2 (PD2)  | SDA on I2C mode, data out on SPI and UART.                   |
| MISO           | Digital pin 4 (PD4)  | Data in on SPI and UART. Unused on I2C.                      |
| CLK            | Digital pin 3 (PD3)  | Clock out signal.                                            |
| CS             | Digital pin 5 (PD5)  | Chip select on SPI and similar protocols.                    |
| AUX            | Digital pin 6 (PD6)  | Auxiliary GPIO. Controlled with the `a`, `A` and `@` commands (set low, set high and read, respectively) |
|                |                      |                                                              |
| ADC            | Analog pin 1 (PC1)   | Analog voltage input (5V max)                                |
| VREGEN         | Digital pin 13 (PB5) | Enable signal for external PSU. Positive logic. Currently unused (tie to the Enable pin of a voltage regulator to implement a BusPirate-like power source). Controlled with the `w` and `W` commands (turn PSU off and on, respectively) |
| PULLUP         | Analog pin 2 (PC2)   | Currently unused                                             |
| MODE           | Analog pin 0 (PC0)   | Currently unused                                             |

## Commands

[These](http://dangerousprototypes.com/docs/Bus_Pirate_user_interface) [are](http://dangerousprototypes.com/docs/Bus_Pirate_menu_options_guide) the original Bus Pirate commands. Yarrrduino implements some commands, but not all. If unsure, try to send a command. If a "syntax error" message is returned, that command was not implemented :)

## Implemented functionality

* Menu navigation works.
* The `raw2wire` protocol is fully implemented.
* The I2C protocol is fully implemented.
* ADC measurements work (both single measurements and continuous).
* Macros (but I reserve the right to remove some macros found in the original BP if I deem them too specialized or low-use, in order to preserve that precious Flash space).

## Pending/won't implement functionality

### Pending but planned

* Actually verify all implemented modes:
  * Raw 2 wire
  * I2C
* There is no ADC measurement of voltage rails (for example, to detect a shorted VREG). It _may_ be implemented later, since the ADC already works.
* Pullups don't currently work.
* `raw3wire` support may be added.
* 1-Wire support may be added.
* SPI support may be added (but only soft SPI. There is already enough trouble without attempting to integrate the hard SPI).

### Nice to have but not planned

* A shield board to hold some additional circuits (namely, some LEDs, 5V and 3.3V power supplies with user control, level shifters and pullup resistor control)

### Not even planned

* The frequency counter doesn't work.
* PWM/servo control is not implemented.
* There is no self-test.
* BASIC scripting will not be even attempted. 
* Direct I/O will not be implemented.
* PIC programming and JTAG will not be implemented.

## Credits

First off: all credit for the name goes to [Toby Jaffey](https://github.com/tobyjaffey) and his [Bus Ninja](https://github.com/tobyjaffey/bus-ninja) project. 

> Q. Why is it called Bus Ninja? A. Because Ninjas are better than Pirates and Yarrrrrduino sounds silly.

Silly as it may be, it's better than anything I could come up with, so I'll take it. I find it kind of funny anyways.



Almost all the code comes from [the Bus Pirate community firmware](https://github.com/BusPirate/Bus_Pirate). Thank you all! The code seems to be under [CC0](http://creativecommons.org/publicdomain/zero/1.0/), so there (should?) be no problems there.