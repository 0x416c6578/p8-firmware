# P8
## Implementing '_screens_'
- Similar to ATCWatch, I plan to implement screens as classes
- There will be one `WatchScreen` class that will be extended by different implementations
- They will have a main `loop()` method that will be run constantly
- They will also have constructors and destructors that will be run at screen instantiation and destruction
- The screen class will also have methods that will be run by the interrupt handler when a display input is received
  - For example `swipeLeft()`, `tap(uint8_t x, uint8_t y)`
- There will also be an app _launcher_ which itself extends the screen class
- The main loop of the program will have a reference to the currently showed screen, and will run the `loop()` method accordingly
- 