#pragma once
#include "Arduino.h"
#include "WatchScreenBase.h"
#include "display.h"
#include "p8Time.h"
#include "pinoutP8.h"

/* 
  A screen with a public method bool doesImplementSwipe____ returning true says to the screen controller
  that upon receiving a swipe event when that screen is the currentScreen, the controller should
  not send that event to the screen, and instead do a more generic task of switching screens or something else
  This means that if a screen implements part of the app drawer, they can say to the controller, "don't send me a
  swipe left event" and instead the controller will move to the next drawer of apps
*/

/*
  This demo screen is used for testing purposes
*/
class DemoScreen : public WatchScreenBase {
 public:
  void screenSetup() {
    clearDisplay();
    writeString(5, 25, 1, "X:");
    writeString(5, 45, 1, "Y:");
    writeIntWithPrecedingZeroes(20, 20, 2, 0);
    writeIntWithPrecedingZeroes(20, 40, 2, 0);
  }
  void screenDestroy() {}
  void screenLoop() {}
  void screenTap(uint8_t x, uint8_t y) {
    writeIntWithPrecedingZeroes(20, 20, 2, x);
    writeIntWithPrecedingZeroes(20, 40, 2, y);
  }
  bool doesImplementSwipeRight() { return false; }
  bool doesImplementSwipeLeft() { return false; }
  void swipeUp() {}
  void swipeDown() {}
};

/* 
  This screen is used to just display the time and date
 */
class TimeScreen : public WatchScreenBase {
 public:
  void screenSetup() { clearDisplay(); }
  void screenDestroy() {}
  void screenLoop() {
    writeString(20, 20, 3, getTimeWithSecs());
    writeString(20, 50, 3, getDate());
  }
  void screenTap(uint8_t x, uint8_t y) {}
  bool doesImplementSwipeRight() { return false; }
  bool doesImplementSwipeLeft() { return false; }
  void swipeUp() {}
  void swipeDown() {}
};

/* 
  This screen is a basic stopwatch
 */
class StopWatchScreen : public WatchScreenBase {
 private:
  bool hasStarted = false;
  int startTime = 0;

 public:
  void screenSetup() {
    clearDisplay();
    drawRect(0, 0, 120, 60, COLOUR_GREEN);
    drawRect(120, 0, 120, 60, COLOUR_RED);
    writeString(15, 18, 3, "Start", 0, COLOUR_GREEN);  //Look at screenTap() for more info
    writeString(145, 18, 3, "Stop", COLOUR_WHITE, COLOUR_RED);
  }
  void screenDestroy() {}
  void screenLoop() {
    if (hasStarted) {
      writeString(26, 130, 4, getStopWatchTime(startTime, millis()));
    }
  }
  void screenTap(uint8_t x, uint8_t y) {
    //This is effectively a button touch listener, making sure the touch is in bounds of the button
    if (x < 120 && y < 60) {
      startStopWatch();
    } else if (x > 120 && y < 60) {
      stopStopWatch();
    }
  }
  bool doesImplementSwipeRight() { return false; }
  bool doesImplementSwipeLeft() { return false; }
  void swipeUp() { startStopWatch(); }
  void swipeDown() { stopStopWatch(); }
  void startStopWatch() {
    startTime = millis();
    hasStarted = true;
  }
  void stopStopWatch() {
    hasStarted = false;
  }
};