#include "SX1302.h"

#if !RADIOLIB_EXCLUDE_SX130X

SX1302::SX1302(Module* mod) : SX130x(mod) {}

int16_t SX1302::begin(float freqA, float freqB) {
  int16_t state = SX130x::begin(freqA, freqB);
  RADIOLIB_ASSERT(state);

  // verify chip version
  int16_t ver = getChipVersion();
  if(ver != RADIOLIB_SX1302_CHIP_VERSION) {
    return(RADIOLIB_ERR_CHIP_NOT_FOUND);
  }

  return(RADIOLIB_ERR_NONE);
}

int16_t SX1302::setFrequency(float freq) {
  RADIOLIB_CHECK_RANGE(freq, 430.0f, 928.0f, RADIOLIB_ERR_INVALID_FREQUENCY);
  return(setFrequencyA(freq));
}

int16_t SX1302::setOutputPower(int8_t power) {
  int16_t state = checkOutputPower(power, NULL);
  RADIOLIB_ASSERT(state);
  return(SX130x::setOutputPower(power));
}

int16_t SX1302::checkOutputPower(int8_t power, int8_t* clipped) {
  if(clipped) {
    *clipped = RADIOLIB_MAX(RADIOLIB_SX1302_MIN_OUTPUT_POWER, RADIOLIB_MIN(RADIOLIB_SX1302_MAX_OUTPUT_POWER, power));
  }
  RADIOLIB_CHECK_RANGE(power, RADIOLIB_SX1302_MIN_OUTPUT_POWER, RADIOLIB_SX1302_MAX_OUTPUT_POWER, RADIOLIB_ERR_INVALID_OUTPUT_POWER);
  return(RADIOLIB_ERR_NONE);
}

uint32_t SX1302::getTimestamp() {
  // SX1302 has a 32-bit microsecond counter split across 4 registers
  uint8_t buf[4] = { 0 };
  (void)readRegisterBurst(RADIOLIB_SX1302_REG_TIMESTAMP_MSB, buf, 4);
  return(((uint32_t)buf[0] << 24) | ((uint32_t)buf[1] << 16) | ((uint32_t)buf[2] << 8) | buf[3]);
}

#endif
