#include "p8Time.h"

String getTimeWithSecs(){
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

String getTime(){
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

String getDate(){
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
String getDay(){
  
}
void setTimeWrapper(int yr, int mth, int day, int hr, int min, int sec){}
