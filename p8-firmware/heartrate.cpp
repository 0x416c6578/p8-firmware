#include "headers/heartrate.h"

HRS3300lib HRS3300;
void initHeartrate(){
  pinMode(HR_TEST, INPUT);
  HRS3300.begin();
  HRS3300.end();
}