# GPIO ports

See ioControl.cpp::initIO(). This includes:
- Output: GREEN_LEDS (perhaps the heart monitor?)
- Output: VIBRATOR_OUT
- Output: LCD_BACKLIGHT_LOW
- Output: LCD_BACKLIGHT_MID
- Output: LCD_BACKLIGHT_HIGH
- Output: SPI_CE (LCD display)
- Output: POWER_CONTROL
- Output: PUSH_BUTTON_OUT (non-P8 only)
- Input : PUSH_BUTTON_IN (Note: this was already done in setup() to sense the desire to go into bootloader node. The setting here is probably unnecessary, but at least it keeps things in one place.
- Input : CHARGE_INDICATION (charging indicator (seems to not be used as yet)
- Input : POWER_INDICATION (battery voltage)

The initIO() function also initializes some values in some of these IOs:
- PUSH_BUTTON_OUT set to HIGH
- SPI_CE set to HIGH
- POWER_CONTROL set to HIGH

And external calls to:
- ledOutput(false) LED on heart rate sensor
- motorOutput(false) Vibrator
- setBrightness(3) brightness of LCD