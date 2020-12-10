# SPI

The SPI bus handles the display and flash memory (latter yet to be implemented). It appears that the code does some performance tuning by buffering. More research needed here.

p8-firmware::setup calls fastSPI::initFastSPI() early in the process. This routine sets some GPIOs:
- Output: LCD_SCK (the SPI clock on pin 2)
- Output: LCD_SDI (the SPI MOSI on pin 3)
- Output: LCD_CS (the LCD slave select)
- Input : SPI_MISO (the flash memory MISO on pin 4)

It then sets all the outputs to HIGH, and some values in the NRF_SPIM2 structure.
