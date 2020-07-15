#pragma once
#include "Arduino.h"
#include "WatchScreenBase.h"
#include "accelerometer.h"
#include "display.h"
#include "p8Time.h"
#include "pinoutP8.h"
#include "powerControl.h"

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
 private:
  bma4_accel data;
  long lastStepRead = 0;

 public:
  void screenSetup() {
    clearDisplay(true);
  }
  void screenDestroy() {}
  void screenLoop() {
    getAcclData(&data);
    writeIntWithPrecedingZeroes(0, 0, 2, abs(data.x));
    writeIntWithPrecedingZeroes(0, 20, 2, abs(data.y));
    writeIntWithPrecedingZeroes(0, 40, 2, abs(data.z));
    if (millis() - lastStepRead > 100) {
      writeIntWithoutPrecedingZeroes(0, 60, 2, (int)getStepCount());
      lastStepRead = millis();
    }
  }
  void screenTap(uint8_t x, uint8_t y) {}
  bool doesImplementSwipeRight() { return false; }
  bool doesImplementSwipeLeft() { return false; }
  void swipeUp() {}
  void swipeDown() {}
  uint8_t getScreenUpdateTimeMS(){return 1;} //Fast update time
};

/* 
  This screen is used to just display the time and date
 */
class TimeScreen : public WatchScreenBase {
 private:
  uint8_t currentDay = -1;

 public:
  void screenSetup() {
    clearDisplay(true);
    currentDay = -1;
    writeString(80, 125, 3, "%");
    writeChar(20, 152, 3, GLYPH_WALKING, COLOUR_WHITE, COLOUR_BLACK);
  }
  void screenDestroy() {}
  void screenLoop() {
    writeString(20, 20, 4, getTimeWithSecs());
    writeString(20, 65, 3, getDate());
    //Only update the day string on a new day
    if (getDayOfWeek(day(), month(), year()) != currentDay) {
      writeString(20, 95, 3, getDay());
    }
    if (!getChargeState()) {
      if (getBatteryPercent() < 99)
        writeChar(20, 122, 3, GLYPH_BATTERY, COLOUR_RED, COLOUR_BLACK);
      else
        writeChar(20, 122, 3, GLYPH_BATTERY, COLOUR_GREEN, COLOUR_BLACK);
    } else {
      writeChar(20, 122, 3, GLYPH_BATTERY, COLOUR_WHITE, COLOUR_BLACK);
    }
    writeIntWithoutPrecedingZeroes(40, 125, 3, getBatteryPercent());
    writeIntWithoutPrecedingZeroes(40, 155, 3, getStepCount());
  }
  void screenTap(uint8_t x, uint8_t y) {}
  bool doesImplementSwipeRight() { return false; }
  bool doesImplementSwipeLeft() { return false; }
  void swipeUp() {}
  void swipeDown() {}
  uint8_t getScreenUpdateTimeMS() { return 20; } //20ms update time
};

/* 
  This screen is a basic stopwatch
 */
class StopWatchScreen : public WatchScreenBase {
 private:
  bool hasStarted = false;
  long startTime = 0;

 public:
  void screenSetup() {
    clearDisplay(true);
    drawRect(0, 0, 120, 60, 0b0000011101000000);
    drawRect(120, 0, 120, 60, 0b1110100000000000);
    writeString(60 - (getWidthOfNChars(5, 3) / 2), 18, 3, "Start", COLOUR_WHITE, 0b0000011101000000);  //Look at screenTap() for more info
    writeString(180 - (getWidthOfNChars(4, 3) / 2), 18, 3, "Stop", COLOUR_WHITE, 0b1110100000000000);
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

/* 
  Jank way of setting the time and date and Brightness
 */
class TimeDateSetScreen : public WatchScreenBase {
 private:
  int8_t setHour = 12;
  int8_t setMinute = 30;
  int8_t setSecond = 0;
  int setYear = 2020;
  int8_t setMonth = 6;
  int8_t setDay = 15;

 public:
  enum settingsWindow {
    BRIGHTNESS,
    SECOND,
    MINUTE,
    HOUR,
    DAY,
    MONTH,
    YEAR
  };

  settingsWindow currentSettingsWindow = BRIGHTNESS;
  void screenSetup() {
    clearDisplay(true);
    drawRects();
    currentSettingsWindow = BRIGHTNESS;
  }
  void screenDestroy() {}
  void screenLoop() {
    switch (currentSettingsWindow) {
      case BRIGHTNESS:
        writeString(0, 0, 3, "Brightness");
        writeIntWithPrecedingZeroes(0, 26, 3, getBrightness());
        break;
      case SECOND:
        writeString(0, 0, 3, "Second");
        writeIntWithPrecedingZeroes(0, 26, 3, setSecond);
        break;
      case MINUTE:
        writeString(0, 0, 3, "Minute");
        writeIntWithPrecedingZeroes(0, 26, 3, setMinute);
        break;
      case HOUR:
        writeString(0, 0, 3, "Hour");
        writeIntWithPrecedingZeroes(0, 26, 3, setHour);
        break;
      case DAY:
        writeString(0, 0, 3, "Day");
        writeIntWithPrecedingZeroes(0, 26, 3, setDay);
        break;
      case MONTH:
        writeString(0, 0, 3, "Month");
        writeIntWithPrecedingZeroes(0, 26, 3, setMonth);
        break;
      case YEAR:
        writeString(0, 0, 3, "Year");
        writeIntWithPrecedingZeroes(0, 26, 3, setYear);
        setTimeWrapper(setYear, setMonth, setDay, setHour, setMinute, setSecond);
        break;
    }
  }
  void screenTap(uint8_t x, uint8_t y) {
    if (x <= 120) {
      switch (currentSettingsWindow) {
        case BRIGHTNESS:
          decBrightness();
          break;
        case SECOND:
          if (setSecond > 0)
            setSecond--;
          break;
        case MINUTE:
          if (setMinute > 0)
            setMinute--;
          break;
        case HOUR:
          if (setHour > 0)
            setHour--;
          break;
        case DAY:
          if (setDay > 0)
            setDay--;
          break;
        case MONTH:
          if (setMonth > 0)
            setMonth--;
          break;
        case YEAR:
          if (setYear > 0)
            setYear--;
          break;
      }
    } else if (x > 120) {
      switch (currentSettingsWindow) {
        case BRIGHTNESS:
          incBrightness();
          break;
        case SECOND:
          if (setSecond < 60)
            setSecond++;
          break;
        case MINUTE:
          if (setSecond < 60)
            setMinute++;
          break;
        case HOUR:
          if (setSecond < 23)
            setHour++;
          break;
        case DAY:
          if (setSecond < 31)
            setDay++;
          break;
        case MONTH:
          if (setSecond < 12)
            setMonth++;
          break;
        case YEAR:
          setYear++;
          break;
      }
    }
  }
  bool doesImplementSwipeRight() { return false; }
  bool doesImplementSwipeLeft() { return false; }
  void swipeDown() {
    switch (currentSettingsWindow) {
      case BRIGHTNESS:
        currentSettingsWindow = SECOND;
        drawRect(0, 0, 240, 50, COLOUR_BLACK);
        break;
      case SECOND:
        currentSettingsWindow = MINUTE;
        drawRect(0, 0, 240, 50, COLOUR_BLACK);
        break;
      case MINUTE:
        currentSettingsWindow = HOUR;
        drawRect(0, 0, 240, 50, COLOUR_BLACK);
        break;
      case HOUR:
        currentSettingsWindow = DAY;
        drawRect(0, 0, 240, 50, COLOUR_BLACK);
        break;
      case DAY:
        currentSettingsWindow = MONTH;
        drawRect(0, 0, 240, 50, COLOUR_BLACK);
        break;
      case MONTH:
        currentSettingsWindow = YEAR;
        drawRect(0, 0, 240, 50, COLOUR_BLACK);
        break;
      default:
        break;
    }
  }
  void swipeUp() {
    switch (currentSettingsWindow) {
      case SECOND:
        currentSettingsWindow = BRIGHTNESS;
        drawRect(0, 0, 240, 50, COLOUR_BLACK);
        break;
      case MINUTE:
        currentSettingsWindow = SECOND;
        drawRect(0, 0, 240, 50, COLOUR_BLACK);
        break;
      case HOUR:
        currentSettingsWindow = MINUTE;
        drawRect(0, 0, 240, 50, COLOUR_BLACK);
        break;
      case DAY:
        currentSettingsWindow = HOUR;
        drawRect(0, 0, 240, 50, COLOUR_BLACK);
        break;
      case MONTH:
        currentSettingsWindow = DAY;
        drawRect(0, 0, 240, 50, COLOUR_BLACK);
        break;
      case YEAR:
        currentSettingsWindow = MONTH;
        drawRect(0, 0, 240, 50, COLOUR_BLACK);
        break;
      default:
        break;
    }
  }
  void drawRects() {
    drawRect(0, 50, 120, 150, COLOUR_RED);
    drawRect(120, 50, 120, 150, COLOUR_BLUE);
    writeChar(30, 107, 6, '-', COLOUR_WHITE, COLOUR_RED);
    writeChar(175, 107, 6, '+', COLOUR_WHITE, COLOUR_BLUE);
  }
};

/* 
  Show info/uptime
 */
class InfoScreen : public WatchScreenBase {
 public:
  void screenSetup() {
    clearDisplay(true);
    writeString(0, 0, 1, "Firmware by:");
    writeString(0, 10, 2, "Alex Underwood");
    writeString(0, 30, 1, "Uptime:");
    writeString(0, 60, 1, "Compiled:");
    writeString(0, 70, 2, __DATE__);
    writeString(0, 90, 2, __TIME__);
  }
  void screenDestroy() {}
  void screenLoop() {
    writeIntWithPrecedingZeroes(0, 40, 2, millis());
  }
  void screenTap(uint8_t x, uint8_t y) {}
  bool doesImplementSwipeLeft() { return false; }
  bool doesImplementSwipeRight() { return false; }
  void swipeUp() {}
  void swipeDown() {}
  uint8_t getScreenUpdateTimeMS(){return 200;} //Slow update time test
};

/* 
  Screen to reboot/bootloader etc
 */
class PowerScreen : public WatchScreenBase {
 public:
  void screenSetup() {
    clearDisplay(true);
    drawRectOutlineWithChar(0, 0, 70, 70, 5, COLOUR_WHITE, GLYPH_REBOOT_UNSEL, 4);
    drawRectOutlineWithChar(85, 0, 70, 70, 5, COLOUR_WHITE, GLYPH_BOOTLOADER_UNSEL, 4);
    drawRectOutline(170, 0, 70, 70, 5, COLOUR_WHITE);
  }
  void screenDestroy() {}
  void screenLoop() {}
  void screenTap(uint8_t x, uint8_t y) {
    if (y < 70 && x < 70) {
      __DSB(); /* Ensure all outstanding memory accesses included
                  buffered write are completed before reset */

      //We need to make sure we write the correct key into vectkey otherwise the write will be ignored
      SCB->AIRCR = ((0x5FAUL << SCB_AIRCR_VECTKEY_Pos) |
                    SCB_AIRCR_SYSRESETREQ_Msk);
      __DSB(); /* Ensure completion of memory access */

      for (;;) /* wait until reset */
      {
        __NOP();
      }
    }
  }
  bool doesImplementSwipeLeft() { return false; }
  bool doesImplementSwipeRight() { return false; }
  void swipeUp() {}
  void swipeDown() {}
};