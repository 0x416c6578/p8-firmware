#include "Arduino.h"
#include "WatchScreenBase.h"
#include "display.h"
#include "p8Time.h"
#include "pinoutP8.h"

/*
  This screen will show the current time at (20,20)
*/
class TimeScreen : public WatchScreenBase {
 public:
  void screenSetup() {}
  void screenDestroy() {}
  void screenLoop() { writeString(20, 20, 3, getTimeWithSecs()); }
  void swipeLeft() {}
  void swipeRight() {}
  void swipeUp() {}
  void swipeDown() {}
};