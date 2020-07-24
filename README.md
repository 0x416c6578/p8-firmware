# p8-firmware
A firmware for the P8 smartwatch based off of the code from ATC1441.  
I rewrote a lot of the code from [here](https://github.com/atc1441/ATCwatch) as a way to learn about embedded programming.  
All code is commented so that people wanting to learn about how Aaron's firmware works can do so more easily.    
If you would like to know how to set up the toolchain in Linux, see the documentation I wrote [here](https://github.com/0x416c6578/ATCwatch/blob/master/linux-tutorial.md).  
HUGE HUGE thanks to [Aaron](https://github.com/atc1441) for all his work, I would have never have got this far on my own. This has been a great learning experience in writing efficient embedded C(++), and learning about the nRF SDK.
## TODO:
- [x] ~~Implement hyphenated text wrapping~~ This has been removed since it was unused, and didn't take some edge-cases into account
- [x] Improve character printing by creating, populating and eventually displaying a single buffer for an entire character, rather than writing many small rects as pixels
- [ ] Implement stripped down version of <Wire.h> for increased power efficiency (use EasyDMA directly instead of a wrapper)
#### In the future
- [x] Get RTC working
- [x] Get touch display working
- [x] Get accelerometer working
- [x] Get heart rate sensor working
- [x] Implement main screen, and a framework for adding new screens
## New Features over ATCWatch
- Faster bulk test displaying with ignoreBlankPixels parameter for writing characters
  - This will skip over drawing the rects for blank pixels in characters
  - It should only be used when the area you are writing to is blank (perhaps clear the screen before)
- Coloured text
  - Coloured FG and BG text was implemented, but removed due to lack of use
- Writing strings as String objects, and as char[]s
- Writing integers to the display (both with and without preceding zeroes)

## Notes on the P8
See [Writeup](Writeup.md)
