#include "headers/p8Time.h"

/* 
  Put current time with seconds into string provided
 */
void getTimeWithSecs(char* str) {
  sprintf(str, "%02d:%02d:%02d", hour(), minute(), second());
}

/* 
  Put the current time without seconds into string provided
 */
void getTime(char* str) {
  sprintf(str, "%02d:%02d", hour(), minute());
}

/* 
  Put the current date into string provided
 */
void getDate(char* str) {
  sprintf(str, "%02d.%02d.%04d", day(), month(), year());
}

/* 
  Put current day into string provided
  Memset is used to make buffer all spaces initially, since the string length is variable
 */
void getDay(char* str) {
  memset(str, ' ', 9);
  switch (getDayOfWeek()) {
    case 0:
      sprintf(str, "Sunday");
      break;
    case 1:
      sprintf(str, "Monday");
      break;
    case 2:
      sprintf(str, "Tuesday");
      break;
    case 3:
      sprintf(str, "Wednesday");
      break;
    case 4:
      sprintf(str, "Thursday");
      break;
    case 5:
      sprintf(str, "Friday");
      break;
    case 6:
      sprintf(str, "Saturday");
      break;
    default:
      sprintf(str, "UNK");
      break;
  }
}

/* 
  Get the weekday from a given day month and year
 */
uint8_t getDayOfWeek(int d, int m, int y) {
  int t[] = {0, 3, 2, 5, 0, 3, 5, 1, 4, 6, 2, 4};
  y -= m < 3;
  return (y + y / 4 - y / 100 + y / 400 + t[m - 1] + d) % 7;
}

/* 
  Get the current weekday
 */
uint8_t getDayOfWeek() {
  return getDayOfWeek(day(), month(), year());
}

/*
  Set the time (just a wrapper for the library implementation (here for convenience))
*/
void setTimeWrapper(int yr, int mth, int _day, int hr, int _min, int sec) {
  setTime(hr, _min, sec, _day, mth, yr);
}

/* 
  Write the elapsed time between two times in milliseconds into string provided
 */
void getStopWatchTime(char* str, int startTime, int currentTime) {
  int elapsedTimeMillis = currentTime - startTime;
  sprintf(str, "%02d:%02d:%02d", (elapsedTimeMillis / 3600000), (elapsedTimeMillis / 60000) % 60, (elapsedTimeMillis / 1000) % 60);
}