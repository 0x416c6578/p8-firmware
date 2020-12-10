# Documentation

Firstly a general hardware description taken from the Colmi P8 web page can be found [here](HARDWARE.md).

I will attempt to document the code at the point it was forked from 0x416c6578.

The P8 firmware starts in [pi-firmware.ino](../p8-firmware/p8-firmware.ino).

The setup() method runs once. It first checks to see if the hardware button is being pressed. If so, it goes into bootloader mode, and restarts.

If the hardware button is not being pressed, it begins initializing the different components.

1) [GPIO ports](GPIO.md).
1) [SPI stuff](SPI.md).
2) [Display](Display.md)
3) Draws a smiley glyph on the scren
4) Initializes the arduino Wire library (eases the communication with I2C devices) and sets the clock to 250,000)
5) [Touch](Touch.md)
6) [Interrupts](Interrupts.md)
7) Sets the power mode for when the CPU goes into sleep state.
8) [screenController](screenController.md]

After all the setup() steps are completed, the p8-firmware::loop() begins.

1) Don't feed the watchdog if the button is pressed
2) calculate the battery reading
3) If the screen is on, run the main loop of the current screen
4) If the screen is off, put the MCU in sleep mode, only waking on interrupt
5) screenControllerLoop() or sleepWait() have returned, handleInterrupts() and start the loop over.
   
