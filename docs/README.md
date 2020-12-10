# Documentation

Firstly a general hardware description taken from the Colmi P8 web page can be found [here](HARDWARE.md).

I will attempt to document the code at the point it was forked from 0x416c6578.

The P8 firmware starts in [pi-firmware.ino](../p8-firmware/p8-firmware.ino).

The setup() method runs once. It first checks to see if the hardware button is being pressed. If so, it goes into bootloader mode, and restarts.

If the hardware button is not being pressed, it begins initializing the different components.

Firstly is the [GPIO ports](GPIO.md). 