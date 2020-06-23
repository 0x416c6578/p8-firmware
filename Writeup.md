# Notes on the P8 smartwatch
## Display
### LCD
- Aaron Christophel used a rect based implementation for writing characters to the screen
  - This involved working through the font, writing rects (squares) everywhere there is a pixel
  - This was fast enough for most text displaying needs, however it was noticeably slow when writing a full screen of text
  - One factor of the slow speed was to do with the fact that the whole _write into display RAM_ sequence was run 5*8=40 times per character
- My implementation does the work before writing, writing pixel data into the LCD Buffer directly, then using EasyDMA to write that whole buffer into display RAM
  - This is different to Aarons, which draws a rect for each pixel
  - This implementation is faster still than Aarons implementation, and by the naked eye, writing a full screen of text is equally as fast as drawing a rect equal to the display size (theoretically the fastest you could write to the display)
- There are still further optimizations that could be made to this routine (and parameterising it for use with different fonts), however I am happy with the current implementation
### Touch
- The touch controller used in the P8 is the CST716S, rather than the CST816S
- It seems to be an equivalent controller in terms of data output
## RTC
- There is a 32768KHz oscillator in the main CPU, however I am still researching how to use that for RTC needs (and whether the `millis()` function uses that or the base system clock for timing)
## Heartrate sensor
- There are two LEDs on the heartrate sensor board, however these are independently controllable separate to the heartrate sensor itself (which has its own LED in the package) through pin 27

<img src="Images/leds.jpg" alt=" " width=" 600" />

## Accelerometer

## Internal Images

<img src="Images/leds.jpg" alt=" " width=" 600" />
<img src="Images/batteryCon.jpg" alt=" " width=" 600" />
<img src="Images/overview.jpg" alt=" " width=" 600" />