#include "accelerometer.h"

static uint8_t bmaAddress = BMA4_I2C_ADDR_PRIMARY;
struct bma4_dev BMAInfoStruct;
struct bma4_accel_config accelConfig;

void initAccel() {
  pinMode(BMA421_INT, INPUT);
  Wire.begin();
  Wire.setClock(250000);
  BMAInfoStruct.intf = BMA4_I2C_INTF;
  BMAInfoStruct.bus_read = busRead;    //Function pointer to bus read function
  BMAInfoStruct.bus_write = busWrite;  //Function pointer to bus write function
  BMAInfoStruct.variant = BMA42X_VARIANT;
  BMAInfoStruct.intf_ptr = &bmaAddress;
  BMAInfoStruct.delay_us = delayFunction;
  BMAInfoStruct.read_write_len = 8;

  accelConfig.odr = BMA4_OUTPUT_DATA_RATE_100HZ;
  accelConfig.range = BMA4_ACCEL_RANGE_2G;
  accelConfig.bandwidth = BMA4_ACCEL_NORMAL_AVG4;
  accelConfig.perf_mode = BMA4_CIC_AVG_MODE;

  bma423_reset_step_counter(&BMAInfoStruct);

  uint16_t result = 0;
  for (uint8_t i = 0; i < 5; i++) {
    feedWatchdog();
    bma423_reset_step_counter(&BMAInfoStruct);
    delay(100);
    result = result | bma423_init(&BMAInfoStruct);
    result = result | bma423_write_config_file(&BMAInfoStruct);
    result = result | bma4_set_accel_enable(1, &BMAInfoStruct);
    result = result | bma4_set_accel_config(&accelConfig, &BMAInfoStruct);
    result = result | bma423_feature_enable(BMA423_STEP_CNTR | BMA423_STEP_ACT, 1, &BMAInfoStruct);
    struct bma4_int_pin_config int_pin_config;
    int_pin_config.edge_ctrl = BMA4_LEVEL_TRIGGER;
    int_pin_config.lvl = BMA4_ACTIVE_LOW;
    int_pin_config.od = BMA4_PUSH_PULL;
    int_pin_config.output_en = BMA4_OUTPUT_ENABLE;
    int_pin_config.input_en = BMA4_INPUT_DISABLE;
    bma4_set_int_pin_config(&int_pin_config, BMA4_INTR1_MAP, &BMAInfoStruct);
    if (result == 0) {
      break;
    }
  }
}

/* 
The library requires pointers to functions that read and write to the i2c bus, so this is that function
for reading
 */
int8_t busRead(uint8_t reg_addr, uint8_t *reg_data, uint32_t length, void *intf_ptr) {
  Wire.beginTransmission(BMA4_I2C_ADDR_PRIMARY);
  Wire.write(reg_addr);
  if (Wire.endTransmission()) return -1;
  Wire.requestFrom(BMA4_I2C_ADDR_PRIMARY, length);
  for (int i = 0; i < length; i++) {
    *reg_data++ = Wire.read();
  }
  return 0;
}

/* 
This is the user defined function for writing to the i2c bus
 */
int8_t busWrite(uint8_t reg_addr, const uint8_t *reg_data, uint32_t length, void *intf_ptr) {
  byte error;
  Wire.beginTransmission(BMA4_I2C_ADDR_PRIMARY);
  Wire.write(reg_addr);
  for (int i = 0; i < length; i++) {
    Wire.write(*reg_data++);
  }
  if (Wire.endTransmission()) return -1;
  return 0;
}

/*
This is the user defined function to do a delay in us that the library needs
*/
void delayFunction(uint32_t period_us, void *intf_ptr) {
  delayMicroseconds(period_us);
}