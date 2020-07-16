#ifndef _PINOUT_
#define _PINOUT_

#define COMMENT_FOR_PINETIME  //Comment this for pinetime

#ifdef COMMENT_FOR_PINETIME

#define P8

//LCD Stuff
#define LCD_SCK 2     //SPI Clock
#define LCD_SDI 3     //SPI MOSI
#define LCD_CS 25     //Chip select
#define LCD_RESET 26  //Display reset
#define LCD_RS 18     //Command/data selector
#define LCD_DET 9     //LCD detect?

//Flash chip
#define SPI_SCK 2
#define SPI_MOSI 3
#define SPI_MISO 4
#define SPI_CE 5

//Touchscreen
#define TP_SDA 6
#define TP_SCL 7
#define TP_RESET 13
#define TP_INT 28  //Touchscreen interrupt (by default the controller is in sleep mode, and will wake up (and send an interrupt) when a touch event is detected)

//Accelerometer
#define BMA421_SDA 6
#define BMA421_SCL 7
#define BMA421_INT 8

//Heartrate sensor
#define HR_SDA 6
#define HR_SCL 7
#define HR_TEST 30

//Battery
#define CHARGE_INDICATION -1
#define POWER_INDICATION 19
#define BATTERY_VOLTAGE 31
#define POWER_CONTROL 24

//Various IO
#define GREEN_LEDS 27
#define VIBRATOR_OUT 16
#define PUSH_BUTTON_IN 17
#define PUSH_BUTTON_OUT -1

//Backlight control
#define LCD_BACKLIGHT_LOW 14
#define LCD_BACKLIGHT_MID 22
#define LCD_BACKLIGHT_HIGH 23

#else

//PINETIME:

//LCD Stuff
#define LCD_SCK 2     //SPI Clock
#define LCD_SDI 3     //SPI MOSI
#define LCD_CS 25     //Chip select
#define LCD_RESET 26  //Display reset
#define LCD_RS 18     //Command/data selector
#define LCD_DET 9     //LCD detect?

//Flash chip
#define SPI_SCK 2
#define SPI_MOSI 3
#define SPI_MISO 4
#define SPI_CE 5

//Touchscreen
#define TP_SDA 6
#define TP_SCL 7
#define TP_RESET 10
#define TP_INT 28  //Touchscreen interrupt (by default the controller is in sleep mode, and will wake up (and send an interrupt) when a touch event is detected)

//Accelerometer
#define BMA421_SDA 6
#define BMA421_SCL 7
#define BMA421_INT 8

//Heartrate sensor
#define HR_SDA 6
#define HR_SCL 7
#define HR_TEST 30

//Battery
#define CHARGE_INDICATION 12
#define POWER_INDICATION 19
#define BATTERY_VOLTAGE 31
#define POWER_CONTROL 24

//Various IO
#define GREEN_LEDS 27
#define VIBRATOR_OUT 16
#define PUSH_BUTTON_IN 13
#define PUSH_BUTTON_OUT 15

//Backlight control
#define LCD_BACKLIGHT_LOW 14
#define LCD_BACKLIGHT_MID 22
#define LCD_BACKLIGHT_HIGH 23
#endif  //COMMENT_FOR_PINETIME
#endif  //_PINOUT_