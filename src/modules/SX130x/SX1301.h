#if !defined(_RADIOLIB_SX1301_H)
#define _RADIOLIB_SX1301_H

#include "../../TypeDef.h"

#if !RADIOLIB_EXCLUDE_SX130X

#include "../../Module.h"
#include "SX130x.h"

#define RADIOLIB_SX1301_CHIP_VERSION                            0x67
#define RADIOLIB_SX1301_MAX_OUTPUT_POWER                        27
#define RADIOLIB_SX1301_MIN_OUTPUT_POWER                        (-6)

/*!
  \class SX1301
  \brief Derived class for %SX1301 LoRaWAN concentrator modules.
  The SX1301 is a baseband processor for LoRaWAN gateways, supporting
  8 multi-SF LoRa channels, 1 LoRa single-SF channel and 1 FSK channel.
*/
class SX1301: public SX130x {
  public:
    /*!
      \brief Default constructor.
      \param mod Instance of Module that will be used to communicate with the concentrator.
    */
    SX1301(Module* mod); // cppcheck-suppress noExplicitConstructor

    /*!
      \brief Initialization method.
      \param freqA Center frequency for radio chain A in MHz.
      \param freqB Center frequency for radio chain B in MHz.
      \param radioType Radio frontend type (SX1255 or SX1257). RAK7244/RAK2245 use SX1257.
      \returns \ref status_codes
    */
    int16_t begin(float freqA = 867.5, float freqB = 868.5, SX125xType_t radioType = RADIOLIB_SX125X_TYPE_SX1257);

    /*!
      \brief Sets carrier frequency for radio chain A. Allowed range: 430-928 MHz.
      \param freq Frequency in MHz.
      \returns \ref status_codes
    */
    int16_t setFrequency(float freq) override;

    /*!
      \brief Sets output power. Allowed values are in range from -6 to 27 dBm.
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
};

#endif

#endif
