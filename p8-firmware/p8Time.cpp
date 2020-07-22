#include "headers/p8Time.h"

/*
  Get a String object of the current time, with seconds
  Todo: replace with char* mechanism
*/
String getTimeWithSecs() {
  String timeStr = "";
  if (hour() < 10)
    timeStr += "0";
  timeStr += String(hour());
  timeStr += ":";
  if (minute() < 10)
    timeStr += "0";
  timeStr += String(minute());
  timeStr += ":";
  if (second() < 10)
    timeStr += "0";
  timeStr += String(second());
  return timeStr;
}

/*
  Get a String object of the current time, without seconds
*/
String getTime() {
  String timeStr = "";
  if (hour() < 10)
    timeStr += "0";
  timeStr += String(hour());
  timeStr += ":";
  if (minute() < 10)
    timeStr += "0";
  timeStr += String(minute());
  return timeStr;
}

/*
  Get a String object of the current date
*/
String getDate() {
  String dateStr = "";
  if (day() < 10)
    dateStr += "0";
  dateStr += String(day());
  dateStr += "/";
  if (month() < 10)
    dateStr += "0";
  dateStr += String(month());
  dateStr += "/";
  dateStr += String(year());
  return dateStr;
}

/* 
  Get a String object of the current day
*/
String getDay() {
  switch (getDayOfWeek(day(), month(), year())) {
    case 0:
      return String("Sun");
    case 1:
      return String("Mon");
    case 2:
      return String("Tues");
    case 3:
      return String("Wed");
    case 4:
      return String("Thurs");
    case 5:
      return String("Fri");
    case 6:
      return String("Sat");
    default:
      return String("UNK");
  }
}

/* 
  Function to get the current weekday (sunday = 1)
 */
uint8_t getDayOfWeek(int d, int m, int y) {
  int t[] = {0, 3, 2, 5, 0, 3, 5, 1, 4, 6, 2, 4};
  y -= m < 3;
  return (y + y / 4 - y / 100 + y / 400 + t[m - 1] + d) % 7;
}

/*
  Set the time (just a wrapper for the library implementation (here for convenience))
*/
void setTimeWrapper(int yr, int mth, int dayy, int hr, int minn, int sec) {
  setTime(hr, minn, sec, dayy, mth, yr);
}

/* 
  Get a string representing the current elapsed time of the stopwatch, given the start time, and the current time
 */
String getStopWatchTime(int startTime, int currentTime) {
  int elapsedTimeMillis = currentTime - startTime;
  int second = (elapsedTimeMillis / 1000) % 60;
  int minute = (elapsedTimeMillis / 60000) % 60;
  int hour = (elapsedTimeMillis / 3600000);
  String timeStr = "";
  if (hour < 10)
    timeStr += "0";
  timeStr += String(hour);
  timeStr += ":";
  if (minute < 10)
    timeStr += "0";
  timeStr += String(minute);
  timeStr += ":";
  if (second < 10)
    timeStr += "0";
  timeStr += String(second);
  return timeStr;
}