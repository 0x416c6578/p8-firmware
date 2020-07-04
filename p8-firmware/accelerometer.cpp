#include "headers/accelerometer.h"

static uint8_t bmaAddress = BMA4_I2C_ADDR_PRIMARY;
struct bma4_dev BMAInfoStruct;
struct bma4_accel_config accelConfig;

/* 
  Initialize the accelerometer
 */
void initAccel() {
  pinMode(BMA421_INT, INPUT);
  BMAInfoStruct.intf = BMA4_I2C_INTF;
  BMAInfoStruct.bus_read = acclI2CRead;    //Function pointer to bus read function
  BMAInfoStruct.bus_write = acclI2CWrite;  //Function pointer to bus write function
  BMAInfoStruct.variant = BMA42X_VARIANT;
  BMAInfoStruct.intf_ptr = &bmaAddress;
  BMAInfoStruct.delay_us = acclDelay;
  BMAInfoStruct.read_write_len = 8;

  accelConfig.odr = BMA4_OUTPUT_DATA_RATE_100HZ;
  accelConfig.range = BMA4_ACCEL_RANGE_2G;
  accelConfig.bandwidth = BMA4_ACCEL_NORMAL_AVG4;
  accelConfig.perf_mode = BMA4_CIC_AVG_MODE;

  bma423_reset_step_counter(&BMAInfoStruct);

  uint16_t result = 0;
  ledPing();
  feedWatchdog();
  result = result | bma423_init(&BMAInfoStruct);
  result = result | bma423_write_config_file(&BMAInfoStruct);
  result = result | bma4_set_accel_enable(1, &BMAInfoStruct);
  result = result | bma4_set_accel_config(&accelConfig, &BMAInfoStruct);
  result = result | bma423_feature_enable(BMA423_STEP_CNTR | BMA423_STEP_ACT, 1, &BMAInfoStruct);
  /* struct bma4_int_pin_config int_pin_config;
  int_pin_config.edge_ctrl = BMA4_LEVEL_TRIGGER;
  int_pin_config.lvl = BMA4_ACTIVE_LOW;
  int_pin_config.od = BMA4_PUSH_PULL;
  int_pin_config.output_en = BMA4_OUTPUT_ENABLE;
  int_pin_config.input_en = BMA4_INPUT_DISABLE;
  bma4_set_int_pin_config(&int_pin_config, BMA4_INTR1_MAP, &BMAInfoStruct); */
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
  if (getI2CState() == I2C_LOCKED){
    return 1;
  }
  lockI2C();
  Wire.beginTransmission(BMA4_I2C_ADDR_PRIMARY);
  Wire.write(registerAddress);
  if (Wire.endTransmission()) {
    //If we have an error in ending the transmission
    return 1;
  }
  Wire.requestFrom(BMA4_I2C_ADDR_PRIMARY, readBufLength);
  for (int i = 0; i < readBufLength; i++) {
    *readBuf++ = Wire.read();
  }
  unlockI2C();
  return BMA4_INTF_RET_SUCCESS;
}

/* 
  This is the user defined function for writing to the i2c bus
 */
int8_t acclI2CWrite(uint8_t registerAddress, const uint8_t *writeBuf, uint32_t writeBufLength, void *intf_ptr) {
  //ledPing();
  if (getI2CState() == I2C_LOCKED){
    return 1;
  }
  lockI2C();
  Wire.beginTransmission(BMA4_I2C_ADDR_PRIMARY);
  Wire.write(registerAddress);
  for (int i = 0; i < writeBufLength; i++) {
    Wire.write(*writeBuf++); //Write the data and increment the pointer
  }
  if (Wire.endTransmission()) {
    //If we have an error in ending the transmission
    return 1;
  }
  unlockI2C();
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
void getAcclData(struct bma4_accel* data) {
  uint8_t result = bma4_read_accel_xyz(data, &BMAInfoStruct);
  if (result == 0)
    return;
  data->x = 69;
  data->y = 69;
  data->z = 69;
}