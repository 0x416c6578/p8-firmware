# Touch.cpp

The p8-firmware setup() method calls touch::initTouch() which sets up some GPIO pins:
- Output: TP_RESET (reset)
- Input : TP_INT (Interrupt)
  
It then
- Sets TP_RESET HIGH for 50ms
- Sets TP_RESET LOW for 5ms
- Sets TP_RESET HIGH, then waits 50ms

Using the Wire protocol, begins a transmission, passing 0x15, then writing 0xED and 0xC8. It then ends the transmission.

