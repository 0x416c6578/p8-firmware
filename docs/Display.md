# The display

This seems to be some functions that communicate to the SPI, probably for ease of use and buffering.

The display::initDisplay() function is called after the SPI is initialized, within p8-firmware.setup(). It sets up the GPIOs related to the screen.
- Output: LCD_CS (chip select)
- Output: LCD_RS (command/data select)
- Output: LCD_RESET (display reset)
- Output: LCD_DET (??)
  
Then, it works to get the display in a usable state. It does so by:
- LCD_CS HIGH disable display SPI communication (active is low)
- LCD_RS HIGH data/command selector
- LCD_RESET HIGH
- (wait 20ms)
- LCD_RESET LOW
- (wait 100ms)
- LCD_RESET HIGH
- (wait 100ms)
- SPI::preWrite() which enables the SPI (sets NRF_SPIM2->ENABLE to 7), then sets LCD_CS LOW (chip select low (select display))
- A bunch of commands are written into the SPI buffer, the display is cleared
- Finally a SPI::postWrite(), which does the opposite of spi::preWrite();