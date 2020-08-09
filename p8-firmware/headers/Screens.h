#pragma once
#include "Arduino.h"
#include "WatchScreenBase.h"
#include "accelerometer.h"
#include "display.h"
#include "p8Time.h"
#include "pinout.h"
#include "powerControl.h"

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

class DemoScreen : public WatchScreenBase {
 private:
  uint8_t charX, charY;
  char charToWrite = '~';

 public:
  void screenSetup() {
    clearDisplay(true);
    writeNewChar(20, 20, charToWrite);
  }
  void screenLoop() {}
  bool doesImplementSwipeRight() { return false; }
  bool doesImplementSwipeLeft() { return false; }
  uint8_t getScreenUpdateTimeMS() { return 1; }  //Fast update time
};

class TimeScreen : public WatchScreenBase {
 private:
  uint8_t lastDay = 255;
  char distanceChar[10];
  uint32_t steps;
  char timeStr[6];   //00:00\0
  char dateStr[11];  //01.01.1970\0
  char dayStr[10];   //wednesday\0

 public:
  void screenSetup() {
    clearDisplay(true);
    drawChar(80, 145, 3, '%', COLOUR_WHITE, COLOUR_BLACK);
    drawChar(20, 172, 3, GLYPH_WALKING, COLOUR_WHITE, COLOUR_BLACK);
  }
  void screenLoop() {
    getTime(timeStr);
    drawString(20, 15, 5, timeStr);
    getDate(dateStr);
    drawString(20, 70, 3, dateStr);
    //If we are on a new day, reset the current step count
    if (getDayOfWeek() != lastDay) {
      lastDay = getDayOfWeek();
      resetStepCounter();
      drawString(40, 175, 3, "          ");
    }
    getDay(dayStr);
    drawString(20, 100, 3, dayStr);

    if (!getChargeState()) {
      if (getBatteryPercent() < 99)
        drawChar(20, 142, 3, GLYPH_BATTERY, COLOUR_RED, COLOUR_BLACK);
      else
        drawChar(20, 142, 3, GLYPH_BATTERY, COLOUR_GREEN, COLOUR_BLACK);
    } else {
      drawChar(20, 142, 3, GLYPH_BATTERY, COLOUR_WHITE, COLOUR_BLACK);
    }
    drawIntWithoutPrecedingZeroes(40, 145, 3, getBatteryPercent());

    steps = getStepCount();
    drawIntWithoutPrecedingZeroes(40, 175, 3, steps);
  }
  void screenTap(uint8_t x, uint8_t y) {}
  bool doesImplementSwipeRight() { return false; }
  bool doesImplementSwipeLeft() { return false; }
  uint8_t getScreenUpdateTimeMS() { return 20; }  //20ms update time
};

class ExerciseScreen : public WatchScreenBase {
 private:
  char timeBuf[9];  //String for any times needing writing
  enum currentWindow {
    ACTIVITY,
    LAST_ACT
  };
  currentWindow currentExerciseWindow = ACTIVITY;

  bool hasStartedWorkout = false;
  int startStepCount;           //Starting step count, to calculate difference
  int currentSteps;             //Current step count
  char distanceChar[10];        //Buffer for distance sprintf
  int startTime;                //Millis when workout was started
  bool hasExerciseLog = false;  //Bool to check if we have a previous log
  ExerciseInfoStruct lastLog;   //Info from last log

 public:
  void screenSetup() {
    clearDisplay(true);
    if (currentExerciseWindow == ACTIVITY) {  //If we are on the main window:

      /* 
        Text x pos offset values were calculated with a helper program in the folder.
        (this removed the need for the function(s) in display.cpp to calculate the length of a string)
      */
      if (hasStartedWorkout == false) {
        drawUnfilledRectWithChar(50, 50, 140, 100, 5, COLOUR_WHITE, GLYPH_WALKING_NO_EARTH, 8);  //Button to start workout
        drawString(120 - STR_WIDTH("Start", 4) / 2, 160, 4, "Start");                             //Button label
        if (hasExerciseLog == true)
          drawString(120 - STR_WIDTH("^ Last Activity ^", 2) / 2, 0, 2, "^ Last Activity ^");
      } else {
        drawString(0, 0, 3, "Exercise:");                                                                      //Exercise label
        drawChar(0, 78, 3, GLYPH_WALKING, COLOUR_WHITE, COLOUR_BLACK);                                         //Walking glyph
        drawChar(0, 108, 3, GLYPH_KM, COLOUR_WHITE, COLOUR_BLACK);                                             //KM glyph
        drawString(120 - STR_WIDTH("Long tap to stop logging.", 1) / 2, 204, 1, "Long tap to stop logging.");  //Info at bottom of screen
      }
    } else if (currentExerciseWindow == LAST_ACT) {
      //Show last activity info
      drawString(0, 0, 3, "Previous Log");
      drawChar(0, 61, 3, GLYPH_CLOCK_UNSEL, COLOUR_WHITE, COLOUR_BLACK);
      drawChar(0, 87, 3, GLYPH_WALKING, COLOUR_WHITE, COLOUR_BLACK);
      drawChar(0, 118, 3, GLYPH_KM, COLOUR_WHITE, COLOUR_BLACK);
      getStopWatchTime(timeBuf, lastLog.startTime, lastLog.endTime);
      drawString(20, 60, 3, timeBuf);
      drawIntWithoutPrecedingZeroes(20, 90, 3, lastLog.numSteps);
      sprintf(distanceChar, "%.3f", (lastLog.numSteps) * KM_PER_STEP);
      drawString(20, 120, 3, distanceChar);
    }
  }

  void screenLoop() {
    //If we are on the activity window, and a workout has started, show current activity info
    if (currentExerciseWindow == ACTIVITY) {
      if (hasStartedWorkout == true) {
        //Print current workout info if we have started a workout
        currentSteps = getStepCount();
        getStopWatchTime(timeBuf, startTime, millis());
        drawString(0, 30, 3, timeBuf);                                                 //Workout time
        drawIntWithoutPrecedingZeroes(20, 80, 3, currentSteps - startStepCount);       //Step count
        sprintf(distanceChar, "%.3f", (currentSteps - startStepCount) * KM_PER_STEP);  //KM walked calc
        drawString(20, 110, 3, distanceChar);
      }
    }
  }
  void screenTap(uint8_t x, uint8_t y) {
    if (currentExerciseWindow == ACTIVITY) {
      if (x > 50 && x < 190 && y > 50 && y < 150 && hasStartedWorkout == false) {
        startWorkout();  //Clicked button to start workout
      } else if (y < 40 && hasExerciseLog == true) {
        currentExerciseWindow = LAST_ACT;  //Move to the last log screen if there is a previous stored log
        screenSetup();
      }
    }
  }
  void screenLongTap(uint8_t x, uint8_t y) {
    //Only stop workout if we have started one, and we are on the activity screen
    if (hasStartedWorkout == true && currentExerciseWindow == ACTIVITY) {
      stopWorkout();
    }
  }
  void swipeDown() {
    //If we are on the activity window, we have a previous log, go to the last activity screen
    if (currentExerciseWindow == ACTIVITY && hasExerciseLog == true) {
      currentExerciseWindow = LAST_ACT;
      screenSetup();
    }
  }
  void swipeUp() {
    //Go back to the current activity window
    if (currentExerciseWindow == LAST_ACT) {
      currentExerciseWindow = ACTIVITY;
      screenSetup();
    }
  }

  void startWorkout() {
    hasStartedWorkout = true;
    screenSetup();
    startStepCount = getStepCount();
    startTime = millis();
  }
  void stopWorkout() {
    hasStartedWorkout = false;
    hasExerciseLog = true;
    //Store exercise info
    lastLog.startTime = startTime;
    lastLog.endTime = millis();
    lastLog.numSteps = currentSteps - startStepCount;
    screenSetup();
  }

  //Setup functions
  bool doesImplementSwipeRight() { return false; }
  bool doesImplementSwipeLeft() { return false; }
  bool doesImplementLongTap() { return true; }     //We do use long tap to stop the workout
  uint8_t getScreenUpdateTimeMS() { return 100; }  //Slower update time
};

class StopWatchScreen : public WatchScreenBase {
 private:
  bool hasStarted = false;
  long startTime = 0;
  char timeBuf[9];

 public:
  void screenSetup() {
    clearDisplay(true);
    drawUnfilledRect(0, 0, 110, 60, 7, COLOUR_GREEN);
    drawUnfilledRect(130, 0, 110, 60, 7, COLOUR_RED);
    drawString(55 - STR_WIDTH("Start", 3) / 2, 18, 3, "Start");  //Look at screenTap() for more info
    drawString(185 - STR_WIDTH("Stop", 3) / 2, 18, 3, "Stop");
  }
  void screenLoop() {
    if (hasStarted) {
      getStopWatchTime(timeBuf, startTime, millis());
      drawString(120 - STR_WIDTH("00:00:00", 4) / 2, 115, 4, timeBuf);
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
        drawString(0, 0, 3, "Brightness");
        drawIntWithPrecedingZeroes(0, 26, 3, getBrightness());
        break;
      case SECOND:
        drawString(0, 0, 3, "Second");
        drawIntWithPrecedingZeroes(0, 26, 3, setSecond);
        break;
      case MINUTE:
        drawString(0, 0, 3, "Minute");
        drawIntWithPrecedingZeroes(0, 26, 3, setMinute);
        break;
      case HOUR:
        drawString(0, 0, 3, "Hour");
        drawIntWithPrecedingZeroes(0, 26, 3, setHour);
        break;
      case DAY:
        drawString(0, 0, 3, "Day");
        drawIntWithPrecedingZeroes(0, 26, 3, setDay);
        break;
      case MONTH:
        drawString(0, 0, 3, "Month");
        drawIntWithPrecedingZeroes(0, 26, 3, setMonth);
        break;
      case YEAR:
        drawString(0, 0, 3, "Year");
        drawIntWithPrecedingZeroes(0, 26, 3, setYear);
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
        drawFilledRect(0, 0, 240, 50, COLOUR_BLACK);
        break;
      case SECOND:
        currentSettingsWindow = MINUTE;
        drawFilledRect(0, 0, 240, 50, COLOUR_BLACK);
        break;
      case MINUTE:
        currentSettingsWindow = HOUR;
        drawFilledRect(0, 0, 240, 50, COLOUR_BLACK);
        break;
      case HOUR:
        currentSettingsWindow = DAY;
        drawFilledRect(0, 0, 240, 50, COLOUR_BLACK);
        break;
      case DAY:
        currentSettingsWindow = MONTH;
        drawFilledRect(0, 0, 240, 50, COLOUR_BLACK);
        break;
      case MONTH:
        currentSettingsWindow = YEAR;
        drawFilledRect(0, 0, 240, 50, COLOUR_BLACK);
        break;
      default:
        break;
    }
  }
  void swipeUp() {
    switch (currentSettingsWindow) {
      case SECOND:
        currentSettingsWindow = BRIGHTNESS;
        drawFilledRect(0, 0, 240, 50, COLOUR_BLACK);
        break;
      case MINUTE:
        currentSettingsWindow = SECOND;
        drawFilledRect(0, 0, 240, 50, COLOUR_BLACK);
        break;
      case HOUR:
        currentSettingsWindow = MINUTE;
        drawFilledRect(0, 0, 240, 50, COLOUR_BLACK);
        break;
      case DAY:
        currentSettingsWindow = HOUR;
        drawFilledRect(0, 0, 240, 50, COLOUR_BLACK);
        break;
      case MONTH:
        currentSettingsWindow = DAY;
        drawFilledRect(0, 0, 240, 50, COLOUR_BLACK);
        break;
      case YEAR:
        currentSettingsWindow = MONTH;
        drawFilledRect(0, 0, 240, 50, COLOUR_BLACK);
        break;
      default:
        break;
    }
  }
  void drawRects() {
    drawUnfilledRectWithChar(0, 60, 115, 130, 8, COLOUR_RED, '-', 6);
    drawUnfilledRectWithChar(120, 60, 115, 130, 8, COLOUR_GREEN, '+', 6);
  }
};

class InfoScreen : public WatchScreenBase {
 private:
  char timeBuf[9];

 public:
  void screenSetup() {
    clearDisplay(true);
    drawString(0, 0, 1, "Firmware by:");
    drawString(0, 10, 2, "Alex Underwood");
    drawString(0, 30, 1, "Uptime:");
    drawString(0, 80, 1, "Compiled:");
    drawString(0, 90, 2, __DATE__);
    drawString(0, 110, 2, __TIME__);
    drawString(0, 130, 1, "Based on firmware by");
    drawString(0, 140, 2, "Aaron Christophel");
    drawString(0, 160, 2, "https://ATCnetz.de");
  }
  void screenLoop() {
    drawIntWithPrecedingZeroes(0, 40, 2, millis());
    drawIntWithoutPrecedingZeroes(0, 60, 2, millis() / 1000 / 60 / 60 / 24);
    getStopWatchTime(timeBuf, 0, millis() % 86400000);
    drawString(35, 60, 2, timeBuf);
  }
  bool doesImplementSwipeLeft() { return false; }
  bool doesImplementSwipeRight() { return false; }
  uint8_t getScreenUpdateTimeMS() { return 200; }  //Slow update time
};

class PowerScreen : public WatchScreenBase {
 public:
  void screenSetup() {
    clearDisplay(true);
    drawUnfilledRectWithChar(0, 0, 70, 70, 5, COLOUR_WHITE, GLYPH_REBOOT_UNSEL, 4);
    drawUnfilledRectWithChar(85, 0, 70, 70, 5, COLOUR_WHITE, GLYPH_BOOTLOADER_UNSEL, 4);
    drawUnfilledRect(170, 0, 70, 70, 5, COLOUR_WHITE);
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