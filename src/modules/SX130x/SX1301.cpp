#include "SX1301.h"

#if !RADIOLIB_EXCLUDE_SX130X

SX1301::SX1301(Module* mod) : SX130x(mod) {}

int16_t SX1301::begin(float freqA, float freqB, SX125xType_t radioType) {
  this->radioTypeA = radioType;
  this->radioTypeB = radioType;
  int16_t state = SX130x::begin(freqA, freqB);
  RADIOLIB_ASSERT(state);

  // verify chip version
  int16_t ver = getChipVersion();
  if(ver != RADIOLIB_SX1301_CHIP_VERSION) {
    return(RADIOLIB_ERR_CHIP_NOT_FOUND);
  }

  return(RADIOLIB_ERR_NONE);
}

int16_t SX1301::setFrequency(float freq) {
  RADIOLIB_CHECK_RANGE(freq, 430.0f, 928.0f, RADIOLIB_ERR_INVALID_FREQUENCY);
  return(setFrequencyA(freq));
}

int16_t SX1301::setOutputPower(int8_t power) {
  int16_t state = checkOutputPower(power, NULL);
  RADIOLIB_ASSERT(state);
  return(SX130x::setOutputPower(power));
}

int16_t SX1301::checkOutputPower(int8_t power, int8_t* clipped) {
  if(clipped) {
    *clipped = RADIOLIB_MAX(RADIOLIB_SX1301_MIN_OUTPUT_POWER, RADIOLIB_MIN(RADIOLIB_SX1301_MAX_OUTPUT_POWER, power));
  }
  RADIOLIB_CHECK_RANGE(power, RADIOLIB_SX1301_MIN_OUTPUT_POWER, RADIOLIB_SX1301_MAX_OUTPUT_POWER, RADIOLIB_ERR_INVALID_OUTPUT_POWER);
  return(RADIOLIB_ERR_NONE);
}

#endif
