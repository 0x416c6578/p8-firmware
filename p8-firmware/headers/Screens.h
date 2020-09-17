#pragma once
#include "Arduino.h"
#include "WatchScreenBase.h"
#include "display.h"
#include "p8Time.h"
#include "pinout.h"
#include "powerControl.h"
#include "utils.h"

#define KM_PER_STEP 0.00079f  //65cm per step

typedef struct {
  int startTime;
  int endTime;
  int numSteps;
} ExerciseInfoStruct;

/* 
  A screen with a public method bool doesImplementSwipe____ returning true says to the screen controller
  that upon receiving a swipe event when that screen is the currentScreen, the controller should
  not send that event to the screen, and instead do a more generic task of switching screens or something else
  This means that if a screen implements part of the app drawer, they can say to the controller, "don't send me a
  swipe left event" and instead the controller will move to the next drawer of apps
*/

/* 
  Main screen of the watch, shows time and other info
 */
class TimeScreen : public WatchScreenBase {
 private:
  uint8_t lastDay = 255;
  char distanceChar[10];
  char timeStr[6];   //00:00\0
  char dateStr[11];  //01.01.1970\0
  char dayStr[10];   //wednesday\0

 public:
  void screenSetup() {
    clearDisplay(true);
    drawChar({80, 145}, 3, '%', COLOUR_WHITE, COLOUR_BLACK);
  }
  void screenLoop() {
    getTime(timeStr);
    drawString({20, 15}, 5, timeStr);
    getDate(dateStr);
    drawString({20, 70}, 3, dateStr);
    //If we are on a new day, reset the current step count
    if (getDayOfWeek() != lastDay) {
      lastDay = getDayOfWeek();
      drawString({20, 100}, 3, "          ");  //Clear day
    }
    getDay(dayStr);
    drawString({20, 100}, 3, dayStr);

    if (!getChargeState()) {
      if (getBatteryPercent() < 99)
        drawChar({20, 142}, 3, GLYPH_BATTERY, COLOUR_RED, COLOUR_BLACK);
      else
        drawChar({20, 142}, 3, GLYPH_BATTERY, COLOUR_GREEN, COLOUR_BLACK);
    } else {
      drawChar({20, 142}, 3, GLYPH_BATTERY, COLOUR_WHITE, COLOUR_BLACK);
    }
    drawIntWithoutPrecedingZeroes({40, 145}, 3, getBatteryPercent());
  }
  void screenTap(uint8_t x, uint8_t y) {}
  bool doesImplementSwipeRight() { return false; }
  bool doesImplementSwipeLeft() { return false; }
  uint8_t getScreenUpdateTimeMS() { return 20; }  //20ms update time
};

/* 
  Screen that has stopwatch on it
 */
class StopWatchScreen : public WatchScreenBase {
 private:
  bool hasStarted = false;
  long startTime = 0;
  char timeBuf[9];

 public:
  void screenSetup() {
    clearDisplay(true);
    drawUnfilledRect({0, 0}, 110, 60, 7, COLOUR_GREEN);
    drawUnfilledRect({130, 0}, 110, 60, 7, COLOUR_RED);
    drawString({55 - STR_WIDTH("Start", 3) / 2, 18}, 3, "Start");  //Look at screenTap() for more info
    drawString({185 - STR_WIDTH("Stop", 3) / 2, 18}, 3, "Stop");
  }
  void screenLoop() {
    if (hasStarted) {
      getStopWatchTime(timeBuf, startTime, millis());
      drawString({120 - STR_WIDTH("00:00:00", 4) / 2, 115}, 4, timeBuf);
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
  void startStopWatch() {
    startTime = millis();
    hasStarted = true;
  }
  void stopStopWatch() {
    hasStarted = false;
  }
};

/* 
  "Settings" app where you can set the time and date, and the brightness
  _Awefully_ written
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
  void screenLoop() {
    switch (currentSettingsWindow) {
      case BRIGHTNESS:
        drawString({0, 0}, 3, "Brightness");
        drawIntWithPrecedingZeroes({0, 26}, 3, getBrightness());
        break;
      case SECOND:
        drawString({0, 0}, 3, "Second");
        drawIntWithPrecedingZeroes({0, 26}, 3, setSecond);
        break;
      case MINUTE:
        drawString({0, 0}, 3, "Minute");
        drawIntWithPrecedingZeroes({0, 26}, 3, setMinute);
        break;
      case HOUR:
        drawString({0, 0}, 3, "Hour");
        drawIntWithPrecedingZeroes({0, 26}, 3, setHour);
        break;
      case DAY:
        drawString({0, 0}, 3, "Day");
        drawIntWithPrecedingZeroes({0, 26}, 3, setDay);
        break;
      case MONTH:
        drawString({0, 0}, 3, "Month");
        drawIntWithPrecedingZeroes({0, 26}, 3, setMonth);
        break;
      case YEAR:
        drawString({0, 0}, 3, "Year");
        drawIntWithPrecedingZeroes({0, 26}, 3, setYear);
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
          if (setMinute < 60)
            setMinute++;
          break;
        case HOUR:
          if (setHour < 23)
            setHour++;
          break;
        case DAY:
          if (setDay < 31)
            setDay++;
          break;
        case MONTH:
          if (setMonth < 12)
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
        drawFilledRect({0, 0}, 240, 50, COLOUR_BLACK);
        break;
      case SECOND:
        currentSettingsWindow = MINUTE;
        drawFilledRect({0, 0}, 240, 50, COLOUR_BLACK);
        break;
      case MINUTE:
        currentSettingsWindow = HOUR;
        drawFilledRect({0, 0}, 240, 50, COLOUR_BLACK);
        break;
      case HOUR:
        currentSettingsWindow = DAY;
        drawFilledRect({0, 0}, 240, 50, COLOUR_BLACK);
        break;
      case DAY:
        currentSettingsWindow = MONTH;
        drawFilledRect({0, 0}, 240, 50, COLOUR_BLACK);
        break;
      case MONTH:
        currentSettingsWindow = YEAR;
        drawFilledRect({0, 0}, 240, 50, COLOUR_BLACK);
        break;
      default:
        break;
    }
  }
  void swipeUp() {
    switch (currentSettingsWindow) {
      case SECOND:
        currentSettingsWindow = BRIGHTNESS;
        drawFilledRect({0, 0}, 240, 50, COLOUR_BLACK);
        break;
      case MINUTE:
        currentSettingsWindow = SECOND;
        drawFilledRect({0, 0}, 240, 50, COLOUR_BLACK);
        break;
      case HOUR:
        currentSettingsWindow = MINUTE;
        drawFilledRect({0, 0}, 240, 50, COLOUR_BLACK);
        break;
      case DAY:
        currentSettingsWindow = HOUR;
        drawFilledRect({0, 0}, 240, 50, COLOUR_BLACK);
        break;
      case MONTH:
        currentSettingsWindow = DAY;
        drawFilledRect({0, 0}, 240, 50, COLOUR_BLACK);
        break;
      case YEAR:
        currentSettingsWindow = MONTH;
        drawFilledRect({0, 0}, 240, 50, COLOUR_BLACK);
        break;
      default:
        break;
    }
  }
  void drawRects() {
    drawUnfilledRectWithChar({0, 60}, 115, 130, 8, COLOUR_RED, '-', 6);
    drawUnfilledRectWithChar({120, 60}, 115, 130, 8, COLOUR_GREEN, '+', 6);
  }
};

/* 
  Device info screen, with uptime, compile time and other info
 */
class InfoScreen : public WatchScreenBase {
 private:
  char timeBuf[9];

 public:
  void screenSetup() {
    clearDisplay(true);
    drawString({0, 0}, 1, "Firmware by:");
    drawString({0, 10}, 2, "Alex Underwood");
    drawString({0, 30}, 1, "Uptime:");
    drawString({0, 80}, 1, "Compiled:");
    drawString({0, 90}, 2, __DATE__);
    drawString({0, 110}, 2, __TIME__);
  }
  void screenLoop() {
    drawIntWithPrecedingZeroes({0, 40}, 2, millis());
    drawIntWithoutPrecedingZeroes({0, 60}, 2, millis() / 1000 / 60 / 60 / 24);
    getStopWatchTime(timeBuf, 0, millis() % 86400000);
    drawString({35, 60}, 2, timeBuf);
  }
  bool doesImplementSwipeLeft() { return false; }
  bool doesImplementSwipeRight() { return false; }
  uint8_t getScreenUpdateTimeMS() { return 200; }  //Slow update time
};

/* 
  Screen that allows rebooting and reboot to bootloader
 */
class PowerScreen : public WatchScreenBase {
 public:
  void screenSetup() {
    clearDisplay(true);
    drawUnfilledRectWithChar({0, 0}, 70, 70, 5, COLOUR_WHITE, GLYPH_REBOOT_UNSEL, 4);
    drawUnfilledRectWithChar({85, 0}, 70, 70, 5, COLOUR_WHITE, GLYPH_BOOTLOADER_UNSEL, 4);
    drawUnfilledRect({170, 0}, 70, 70, 5, COLOUR_WHITE);
  }
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
    } else if (x > 70 && x < 140 && y < 70) {
      //Enter the bootloader by setting the general purpose retention register to 1 and rebooting
      NRF_POWER->GPREGRET = 0x01;
      NVIC_SystemReset();
    }
  }
  bool doesImplementSwipeLeft() { return false; }
  bool doesImplementSwipeRight() { return false; }
};

/* 
  Random screen for messing with and testing stuff 
*/
class DemoScreen : public WatchScreenBase {
 private:
  uint8_t charX, charY;
  char charToWrite = '~';

 public:
  void screenSetup() {
    clearDisplay(true);
    writeNewChar({0, 0}, charToWrite);
  }
  void screenLoop() {}
  bool doesImplementSwipeRight() { return false; }
  bool doesImplementSwipeLeft() { return false; }
  uint8_t getScreenUpdateTimeMS() { return 1; }  //Fast update time
};