#include "headers/accelerometer.h"

static uint8_t bmaAddress = BMA4_I2C_ADDR_PRIMARY;
struct bma4_dev BMAInfoStruct;         //Struct that holds all the information about the accl
struct bma4_accel_config accelConfig;  //Accelerometer config struct

/* 
  Initialize the accelerometer
 */
void initAccel() {
  pinMode(BMA421_INT, INPUT);  //Interrupt pin (not used)
  //Reset the accelerometer

  BMAInfoStruct.intf = BMA4_I2C_INTF;      //Enable I2C interface feature
  BMAInfoStruct.bus_read = acclI2CRead;    //Function pointer to bus read function
  BMAInfoStruct.bus_write = acclI2CWrite;  //Function pointer to bus write function
  BMAInfoStruct.variant = BMA42X_VARIANT;  //Set accl variant
  BMAInfoStruct.intf_ptr = &bmaAddress;    //Set the I2C address
  BMAInfoStruct.delay_us = acclDelay;      //Function pointer to a us delay function
  BMAInfoStruct.read_write_len = 8;        //Read/write in bytes
  BMAInfoStruct.chip_id = BMA423_CHIP_ID;

  accelConfig.odr = BMA4_OUTPUT_DATA_RATE_100HZ;   //Output data rate (originally BMA4_OUTPUT_DATA_RATE_100HZ)
  accelConfig.range = BMA4_ACCEL_RANGE_2G;         //G-range
  accelConfig.bandwidth = BMA4_ACCEL_NORMAL_AVG4;  //Bandwidth parameter
  accelConfig.perf_mode = BMA4_CIC_AVG_MODE;       //Filter performance mode (average or continus)

  feedWatchdog();  //Make sure to feed the watchdog here so the while loop has the most time to succeed

  uint16_t result;
  do {
    Wire.beginTransmission(BMA4_I2C_ADDR_PRIMARY);
    Wire.write(0x7E);
    Wire.write(0xB6);
    Wire.endTransmission();
    delay(100);
    result = 0;
    result = result | bma423_init(&BMAInfoStruct);  //API entry point
    delay(10);
    result = result | bma4_set_accel_enable(1, &BMAInfoStruct);  //Enable accel
    delay(10);
    result = result | bma4_set_accel_config(&accelConfig, &BMAInfoStruct);  //Set accel config
    delay(10);
    result = result | bma423_feature_enable(BMA423_STEP_CNTR | BMA423_STEP_ACT, 1, &BMAInfoStruct);  //Enable step counter
    delay(10);
    result = result | bma423_write_config_file(&BMAInfoStruct);  //Write config file
  } while (result != 0);                                         //Repeat until success or reboot

  // bma423_reset_step_counter(&BMAInfoStruct);  //API call to reset the step counter

  if (result == 0) {
    ledPing();
    delay(50);
    ledPing();
    delay(50);
    ledPing();
    delay(50);
  }
}

/* 
  The library requires pointers to functions that read and write to the i2c bus, so this is that function
  for reading
 */
int8_t acclI2CRead(uint8_t registerAddress, uint8_t *readBuf, uint32_t readBufLength, void *intf_ptr) {
  Wire.beginTransmission(BMA4_I2C_ADDR_PRIMARY);
  Wire.write(registerAddress);
  if (Wire.endTransmission()) {
    //If we have an error in ending the transmission
    return -1;
  }
  Wire.requestFrom(BMA4_I2C_ADDR_PRIMARY, readBufLength);
  for (int i = 0; i < readBufLength; i++) {
    *readBuf++ = Wire.read();
  }
  return BMA4_INTF_RET_SUCCESS;
}

/* 
  This is the user defined function for writing to the i2c bus
 */
int8_t acclI2CWrite(uint8_t registerAddress, const uint8_t *writeBuf, uint32_t writeBufLength, void *intf_ptr) {
  Wire.beginTransmission(BMA4_I2C_ADDR_PRIMARY);
  Wire.write(registerAddress);
  for (int i = 0; i < writeBufLength; i++) {
    Wire.write(*writeBuf++);  //Write the data and increment the pointer
  }
  if (Wire.endTransmission()) {
    //If we have an error in ending the transmission
    return -1;
  }
  return BMA4_INTF_RET_SUCCESS;
}

/*
  This is the user defined function to do a delay in us that the library needs
*/
void acclDelay(uint32_t period_us, void *intf_ptr) {
  delayMicroseconds(period_us);
}

/* 
  Test function to get the accelerometer data
 */
void getAcclData(struct bma4_accel *data) {
  uint8_t result = bma4_read_accel_xyz(data, &BMAInfoStruct);
  if (result == 0)
    return;
  data->x = 69;
  data->y = 69;
  data->z = 69;
}

uint32_t getStepCount() {
  uint32_t steps;
  if (bma423_step_counter_output(&steps, &BMAInfoStruct) == BMA4_OK) {
    return steps;
  }
  return 69;
}
