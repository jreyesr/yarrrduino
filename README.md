# Yarrrduino: a Bus Pirate, but for Arduino
// TODO Basic explanation

## Implemented functionality

* Menu navigation works.
* The `raw2wire` protocol is fully implemented. That should mean I2C too.

## Pending/won't implement functionality

### Pending but planned

* There is no ADC measurement. It _may_ be implemented later.
* Pullups don't currently work.
* `raw3wire` support may be added.
* 1-Wire support may be added.
* SPI support may be added (but only soft SPI. There is already enough trouble without attempting to integrate the hard SPI).

### Not even planned

* The frequency counter doesn't work.
* PWM/servo control is not implemented.
* There is no self-test.
* Macros will not be implemented.
* BASIC scripting will not be even attempted. 
* Direct I/O will not be implemented.
* PIC programming and JTAG will not be implemented.

## Credits

First off: all credit for the name goes to [Toby Jaffey](https://github.com/tobyjaffey) and his [Bus Ninja](https://github.com/tobyjaffey/bus-ninja) project. 

> Q. Why is it called Bus Ninja? A. Because Ninjas are better than Pirates and Yarrrrrduino sounds silly.

Silly as it may be, it's better than anything I could come up with, so I'll take it. I find it kind of funny anyways.

Almost all the code comes from [the Bus Pirate community firmware](https://github.com/BusPirate/Bus_Pirate). Thank you all! The code seems to be under [CC0](http://creativecommons.org/publicdomain/zero/1.0/), so there (should?) be no problems there.