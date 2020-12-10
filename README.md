# p8-firmware
A firmware for the P8 smartwatch based off of the code from ATC1441. Forked from [0x416c6578](https://github.com/0x416c6578/p8-firmware).

Near-term goals:
- [ ] Learn the codebase
- [ ] Look at Freetype as font rendering engine
- [ ] Build out framework for apps

Recent docs can be found [here](docs/README.md)

From 0x416c6578:

I rewrote a lot of the code from [here](https://github.com/atc1441/ATCwatch) as a way to learn about embedded programming.  
All code is commented so that people wanting to learn about how Aaron's firmware works can do so more easily.    
If you would like to know how to set up the toolchain in Linux, see the documentation I wrote [here](https://github.com/0x416c6578/ATCwatch/blob/master/linux-tutorial.md).  
HUGE HUGE thanks to [Aaron](https://github.com/atc1441) for all his work, I would have never have got this far on my own. This has been a great learning experience in writing efficient embedded C(++), and learning about the nRF SDK.
## TODO:
#### In the future
- [ ] Get heart rate sensor working
- [x] Implement main screen, and a framework for adding new screens
- [ ] Switch to using LVGL fonts
  - See https://0x416c6578.github.io/ for information about the switch to LVGL fonts
- [ ] Better activity estimation with heartrate sensor
- [ ] Support reading from and writing to the flash chip
## Changes over ATCWatch
- Faster text writing routine that writes an entire character's data into the LCD buffer before writing over SPI
- Writing integers to the display (both with and without preceding zeroes)
- Removed usage of String object
- Activity monitor with approximate distance travelled
- Stopwatch implemented (works in background also)
- The app draw is different to ATCwatch, with a row of apps along the bottom, and buttons / swipes between them
- Device doesn't wakeup on wrist raise (increases battery life a bit)
- Images are just characters
