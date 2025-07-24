/*
 * Copyright (c) 2022 Dhruva Gole
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <Wire.h>
#include <zephyr/sys/util_macro.h>

arduino::ZephyrI2C::ZephyrI2C(const struct device *i2c) : i2c_dev(i2c)
{
}

void arduino::ZephyrI2C::begin() {
  ring_buf_init(&rxRingBuffer.rb, sizeof(rxRingBuffer.buffer), rxRingBuffer.buffer);
}

void arduino::ZephyrI2C::begin(uint8_t slaveAddr) {

}

void arduino::ZephyrI2C::end() {}

void arduino::ZephyrI2C::setClock(uint32_t freq) {
    uint8_t speed = 	I2C_SPEED_STANDARD;
	if(freq >  0x06u ) {
		if(freq == 100000) {
		  speed = I2C_SPEED_STANDARD;
		} else if(freq == 400000) {
		  speed = I2C_SPEED_FAST;
		} else if(freq == 1000000) {
		  speed = I2C_SPEED_FAST_PLUS;
		} else {
		  speed = I2C_SPEED_STANDARD;
		}
	} else {
		speed = (uint8_t) freq;
	}
	uint32_t i2c_cfg = I2C_MODE_CONTROLLER |
					I2C_SPEED_SET(speed);

	if (i2c_configure(i2c_dev, i2c_cfg)) {
		//Serial.println("Failed to configure i2c interface.");
	}
}

void arduino::ZephyrI2C::beginTransmission(uint8_t address) { // TODO for ADS1115
  _address = address;
  usedTxBuffer = 0;
}

uint8_t arduino::ZephyrI2C::endTransmission(bool stopBit) {
  int ret = i2c_write(i2c_dev, txBuffer, usedTxBuffer, _address);
  if (ret) {
    return 1; // fail
  }
  return 0;
}

uint8_t arduino::ZephyrI2C::endTransmission(void) { // TODO for ADS1115
  return endTransmission(true);
}

size_t arduino::ZephyrI2C::requestFrom(uint8_t address, size_t len,
                                       bool stopBit) {
  uint8_t buf[len];
  int ret = i2c_read(i2c_dev, buf, len, address);
  if (ret != 0)
  {
    return 0;
  }
  ret = ring_buf_put(&rxRingBuffer.rb, buf, len);
  if (ret == 0)
  {
    return 0;
  }
  return len;
}

size_t arduino::ZephyrI2C::requestFrom(uint8_t address, size_t len) { // TODO for ADS1115
  return requestFrom(address, len, true);
}

size_t arduino::ZephyrI2C::write(uint8_t data) {  // TODO for ADS1115
  txBuffer[usedTxBuffer++] = data;
  return 1;
}

size_t arduino::ZephyrI2C::write(const uint8_t *buffer, size_t size) {
    if (usedTxBuffer + size > 256) {
    size = 256 - usedTxBuffer;
  }
	memcpy(txBuffer + usedTxBuffer, buffer, size);
	usedTxBuffer += size;	
  return size;
}

int arduino::ZephyrI2C::read() {
  uint8_t buf[1];
  if (ring_buf_size_get(&rxRingBuffer.rb)) {
        int ret = ring_buf_get(&rxRingBuffer.rb, buf, 1);
        if (ret == 0) {
            return -1;
        }
		return (int)buf[0];
  }
  return -1;
}

int arduino::ZephyrI2C::available() {
  return ring_buf_size_get(&rxRingBuffer.rb);
}

int arduino::ZephyrI2C::peek() {
  uint8_t buf[1];
  int bytes_read = ring_buf_peek(&rxRingBuffer.rb, buf, 1);
  if (bytes_read == 0){
    return 0;
  } 
  return (int)buf[0];
}

void arduino::ZephyrI2C::flush() {}

void arduino::ZephyrI2C::onReceive(voidFuncPtrParamInt cb) {}
void arduino::ZephyrI2C::onRequest(voidFuncPtr cb) {}

#if DT_NODE_HAS_PROP(DT_PATH(zephyr_user), i2cs)
#if (DT_PROP_LEN(DT_PATH(zephyr_user), i2cs) > 1)
#define ARDUINO_WIRE_DEFINED_0 1
#define DECL_WIRE_0(n, p, i)   arduino::ZephyrI2C Wire(DEVICE_DT_GET(DT_PHANDLE_BY_IDX(n, p, i)));
#define DECL_WIRE_N(n, p, i)   arduino::ZephyrI2C Wire##i(DEVICE_DT_GET(DT_PHANDLE_BY_IDX(n, p, i)));
#define DECLARE_WIRE_N(n, p, i)                                                                    \
	COND_CODE_1(ARDUINO_WIRE_DEFINED_##i, (DECL_WIRE_0(n, p, i)), (DECL_WIRE_N(n, p, i)))

/* Declare Wire, Wire1, Wire2, ... */
DT_FOREACH_PROP_ELEM(DT_PATH(zephyr_user), i2cs, DECLARE_WIRE_N)

#undef DECLARE_WIRE_N
#undef DECL_WIRE_N
#undef DECL_WIRE_0
#undef ARDUINO_WIRE_DEFINED_0
#elif (DT_PROP_LEN(DT_PATH(zephyr_user), i2cs) == 1)
/* When PROP_LEN(i2cs) == 1, DT_FOREACH_PROP_ELEM work not correctly. */
arduino::ZephyrI2C Wire(DEVICE_DT_GET(DT_PHANDLE_BY_IDX(DT_PATH(zephyr_user), i2cs, 0)));
#endif // HAS_PORP(i2cs)
/* If i2cs node is not defined, tries to use arduino_i2c */
#elif DT_NODE_EXISTS(DT_NODELABEL(arduino_i2c))
arduino::ZephyrI2C Wire(DEVICE_DT_GET(DT_NODELABEL(arduino_i2c)));
#endif
