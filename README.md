# p8-firmware
A firmware for the P8 smartwatch based off of the code from ATC1441.  
Contributed documentation by [jmjatlanta](https://github.com/jmjatlanta) can be found [here](docs/README.md).  
Other documentation and random info by 0x416c6578 can be found [here](https://0x416c6578.github.io/p8-firmware/p8-firmware.html).  
Toolchain installation instructions for Linux can be found [here](https://github.com/0x416c6578/ATCwatch/blob/master/linux-tutorial.md).  

___

This project is based on [ATCwatch](https://github.com/atc1441/ATCwatch), a custom firmware for the p8 smartwatch. I did this project as a way to learn about embedded programming, and I have extensively commented / documented my code so that people can learn more about the architecture of ATCwatch (since I followed the same tight `main` loop for control rather than a realtime OS like other projects use. Huge thanks to [Aaron](https://github.com/atc1441) for all his work, I would have never have got this far on my own. This has been a great learning experience in writing efficient embedded C(++), and learning about the nRF SDK.

___

Due to the discontinuation of nrf52832-based p8 smartwatches, and a focus on University work, this project isn't currently worked on.

## TODOs:
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
- Removed usage of C++ Strings, just because
- Activity monitor with approximate distance travelled
- Stopwatch implemented (works in background also)
- The app draw is different to ATCwatch, with a row of apps along the bottom, and buttons / swipes between them
- Device doesn't wakeup on wrist raise (increases battery life a bit)
