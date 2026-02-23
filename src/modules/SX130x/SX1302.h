#if !defined(_RADIOLIB_SX1302_H)
#define _RADIOLIB_SX1302_H

#include "../../TypeDef.h"

#if !RADIOLIB_EXCLUDE_SX130X

#include "../../Module.h"
#include "SX130x.h"

#define RADIOLIB_SX1302_CHIP_VERSION                            0x10
#define RADIOLIB_SX1302_MAX_OUTPUT_POWER                        27
#define RADIOLIB_SX1302_MIN_OUTPUT_POWER                        (-9)

// SX1302-specific registers (additions over SX1301)
#define RADIOLIB_SX1302_REG_AGC_MCU_CTRL                        0x0D
#define RADIOLIB_SX1302_REG_TIMESTAMP_MSB                       0x48
#define RADIOLIB_SX1302_REG_TIMESTAMP_LSB                       0x4C

/*!
  \class SX1302
  \brief Derived class for %SX1302 LoRaWAN concentrator modules.
  The SX1302 is the successor to the SX1301, featuring lower power consumption,
  improved sensitivity and support for LoRa 2.4 GHz (via companion SX1250 chips).
  Supports 8 multi-SF LoRa channels, 1 LoRa single-SF channel and 1 FSK channel.
*/
class SX1302: public SX130x {
  public:
    /*!
      \brief Default constructor.
      \param mod Instance of Module that will be used to communicate with the concentrator.
    */
    SX1302(Module* mod); // cppcheck-suppress noExplicitConstructor

    /*!
      \brief Initialization method.
      \param freqA Center frequency for radio chain A in MHz.
      \param freqB Center frequency for radio chain B in MHz.
      \returns \ref status_codes
    */
    int16_t begin(float freqA = 867.5, float freqB = 868.5);

    /*!
      \brief Sets carrier frequency for radio chain A. Allowed range: 430-928 MHz.
      \param freq Frequency in MHz.
      \returns \ref status_codes
    */
    int16_t setFrequency(float freq) override;

    /*!
      \brief Sets output power. Allowed values are in range from -9 to 27 dBm.
      \param power Output power to be set in dBm.
      \returns \ref status_codes
    */
    int16_t setOutputPower(int8_t power) override;

    /*!
      \brief Check if output power is configurable.
      \param power Output power in dBm.
      \param clipped Clipped output power value.
      \returns \ref status_codes
    */
    int16_t checkOutputPower(int8_t power, int8_t* clipped) override;

    /*!
      \brief Get the fine-resolution 32-bit timestamp counter.
      The SX1302 provides a higher-resolution timestamp than the SX1301.
      \returns 32-bit timestamp value in microseconds.
    */
    uint32_t getTimestamp() override;
};

#endif

#endif
