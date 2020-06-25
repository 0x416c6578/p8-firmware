#include "headers/p8Time.h"

/*
  Get a String object of the current time, with seconds
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
}

/*
  Set the time (just a wrapper for the library implementation (here for convenience))
*/
void setTimeWrapper(int yr, int mth, int dayy, int hr, int minn, int sec) {
  setTime(hr, minn, sec, dayy, mth, yr);
}
