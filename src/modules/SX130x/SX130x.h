#if !defined(_RADIOLIB_SX130X_H)
#define _RADIOLIB_SX130X_H

#include "../../TypeDef.h"

#if !RADIOLIB_EXCLUDE_SX130X

#include "../../Module.h"
#include "../../protocols/PhysicalLayer/PhysicalLayer.h"

// SX130x physical layer properties
#define RADIOLIB_SX130X_FREQUENCY_STEP_SIZE                     61.03515625
#define RADIOLIB_SX130X_MAX_PACKET_LENGTH                       255
#define RADIOLIB_SX130X_CRYSTAL_FREQ                            32.0f
#define RADIOLIB_SX130X_DIV_EXPONENT                            19

// SX125x radio frontend registers (accessed via SX1301 SPI bridge)
#define RADIOLIB_SX125X_REG_MODE                                0x00
#define RADIOLIB_SX125X_REG_FREQ_MSB                            0x01
#define RADIOLIB_SX125X_REG_FREQ_MID                            0x02
#define RADIOLIB_SX125X_REG_FREQ_LSB                            0x03
#define RADIOLIB_SX125X_REG_TX_DAC_CLK                          0x10
#define RADIOLIB_SX125X_REG_TX_GAIN                             0x08
#define RADIOLIB_SX125X_REG_TX_BW                               0x0A
#define RADIOLIB_SX125X_REG_TX_DAC_BW                           0x0B
#define RADIOLIB_SX125X_REG_RX_ANA                              0x0C
#define RADIOLIB_SX125X_REG_RX_BW                               0x0D
#define RADIOLIB_SX125X_REG_RX_PLL                              0x0E
#define RADIOLIB_SX125X_REG_XOSC_SX1255                         0x28
#define RADIOLIB_SX125X_REG_XOSC_SX1257                         0x26
#define RADIOLIB_SX125X_REG_PLL_LOCK                            0x11

// SX125x MODE register values
#define RADIOLIB_SX125X_MODE_XTAL_ON                            0x01
#define RADIOLIB_SX125X_MODE_RX_ON                              0x03

// SX125x default gain/trim values (from Semtech reference)
#define RADIOLIB_SX125X_TX_DAC_CLK_SEL                          1
#define RADIOLIB_SX125X_TX_DAC_GAIN                             2
#define RADIOLIB_SX125X_TX_MIX_GAIN                             14
#define RADIOLIB_SX125X_TX_PLL_BW                               1
#define RADIOLIB_SX125X_TX_ANA_BW                               0
#define RADIOLIB_SX125X_TX_DAC_BW                               5
#define RADIOLIB_SX125X_RX_LNA_GAIN                             1
#define RADIOLIB_SX125X_RX_BB_GAIN                              12
#define RADIOLIB_SX125X_LNA_ZIN                                 1
#define RADIOLIB_SX125X_RX_ADC_BW                               7
#define RADIOLIB_SX125X_RX_ADC_TRIM                             6
#define RADIOLIB_SX125X_RX_BB_BW                                0
#define RADIOLIB_SX125X_RX_PLL_BW                               0
#define RADIOLIB_SX125X_ADC_TEMP                                0
#define RADIOLIB_SX125X_XOSC_GM_STARTUP                         13
#define RADIOLIB_SX125X_XOSC_DISABLE                            2
#define RADIOLIB_SX125X_32MHZ_FRAC                              15625UL

/*!
  \enum SX125xType_t
  \brief Radio frontend type used with the SX1301 concentrator.
*/
enum SX125xType_t {
  /*! \brief SX1255 radio frontend */
  RADIOLIB_SX125X_TYPE_SX1255 = 0,
  /*! \brief SX1257 radio frontend (used by RAK7244/RAK2245) */
  RADIOLIB_SX125X_TYPE_SX1257 = 1,
};

// Maximum number of LoRa multi-SF channels
#define RADIOLIB_SX130X_NUM_LORA_CHANNELS                       8
// Maximum number of FSK channels
#define RADIOLIB_SX130X_NUM_FSK_CHANNELS                        1
// Maximum number of LoRa single-SF (LoRa service) channels
#define RADIOLIB_SX130X_NUM_LORA_STD_CHANNELS                   1

// SX130x SPI registers (common)
#define RADIOLIB_SX130X_REG_PAGE_RESET                          0x00
#define RADIOLIB_SX130X_REG_VERSION                             0x01
#define RADIOLIB_SX130X_REG_MCU_PROM_ADDR                       0x08
#define RADIOLIB_SX130X_REG_MCU_PROM_DATA                       0x09
#define RADIOLIB_SX130X_REG_MCU_AGC_STATUS                      0x0C
#define RADIOLIB_SX130X_REG_MCU_CTRL                            0x0D
#define RADIOLIB_SX130X_REG_GPIO_SEL_0                          0x10
#define RADIOLIB_SX130X_REG_GPIO_SEL_1                          0x11
#define RADIOLIB_SX130X_REG_GPIO_SEL_2                          0x12
#define RADIOLIB_SX130X_REG_GPIO_SEL_3                          0x13
#define RADIOLIB_SX130X_REG_GPIO_DIR_0                          0x14
#define RADIOLIB_SX130X_REG_GPIO_DIR_1                          0x15
#define RADIOLIB_SX130X_REG_GPIO_DIR_2                          0x16
#define RADIOLIB_SX130X_REG_GPIO_DIR_3                          0x17
#define RADIOLIB_SX130X_REG_GPIO_OUT_0                          0x18
#define RADIOLIB_SX130X_REG_GPIO_OUT_1                          0x19
#define RADIOLIB_SX130X_REG_GPIO_OUT_2                          0x1A
#define RADIOLIB_SX130X_REG_GPIO_OUT_3                          0x1B
#define RADIOLIB_SX130X_REG_GPIO_IN_0                           0x1C
#define RADIOLIB_SX130X_REG_GPIO_IN_1                           0x1D
#define RADIOLIB_SX130X_REG_GPIO_IN_2                           0x1E
#define RADIOLIB_SX130X_REG_GPIO_IN_3                           0x1F
#define RADIOLIB_SX130X_REG_SPI_MUX                             0x20
#define RADIOLIB_SX130X_REG_RADIO_A_SPI_DATA                    0x21
#define RADIOLIB_SX130X_REG_RADIO_B_SPI_DATA                    0x22
#define RADIOLIB_SX130X_REG_RADIO_A_SPI_CLK_DIV                 0x23
#define RADIOLIB_SX130X_REG_RADIO_B_SPI_CLK_DIV                 0x24
#define RADIOLIB_SX130X_REG_RADIO_A_EN                          0x28
#define RADIOLIB_SX130X_REG_RADIO_B_EN                          0x29
#define RADIOLIB_SX130X_REG_RADIO_A_CLK                         0x2A
#define RADIOLIB_SX130X_REG_RADIO_B_CLK                         0x2B
#define RADIOLIB_SX130X_REG_RADIO_A_STAT                        0x2C
#define RADIOLIB_SX130X_REG_RADIO_B_STAT                        0x2D
#define RADIOLIB_SX130X_REG_RADIO_A_CFG                         0x2E
#define RADIOLIB_SX130X_REG_RADIO_B_CFG                         0x2F
#define RADIOLIB_SX130X_REG_TX_GAIN                             0x40
#define RADIOLIB_SX130X_REG_TX_CTRL                             0x41
#define RADIOLIB_SX130X_REG_TX_START                            0x42
#define RADIOLIB_SX130X_REG_TX_TRIG_DELAY                       0x43
#define RADIOLIB_SX130X_REG_TX_NOTCH_FREQ                       0x44
#define RADIOLIB_SX130X_REG_TIMESTAMP                           0x48
#define RADIOLIB_SX130X_REG_RX_DATA_BUF_ADDR                    0x50
#define RADIOLIB_SX130X_REG_RX_DATA_BUF_DATA                    0x51
#define RADIOLIB_SX130X_REG_RX_STATUS                           0x52
#define RADIOLIB_SX130X_REG_RX_STAT_FIFO_COUNT                  0x53
#define RADIOLIB_SX130X_REG_RX_STAT_PKT_COUNT                   0x54
#define RADIOLIB_SX130X_REG_RX_STAT_PKT_FWD                     0x55
#define RADIOLIB_SX130X_REG_RX_STAT_PAYLOAD_CRC_ERR             0x56
#define RADIOLIB_SX130X_REG_RX_STAT_HEADER_ERR                  0x57
#define RADIOLIB_SX130X_REG_RX_STAT_FALSE_DETECT                0x58
#define RADIOLIB_SX130X_REG_MULTI_SF_CHAN_0                      0x60
#define RADIOLIB_SX130X_REG_MULTI_SF_CHAN_1                      0x61
#define RADIOLIB_SX130X_REG_MULTI_SF_CHAN_2                      0x62
#define RADIOLIB_SX130X_REG_MULTI_SF_CHAN_3                      0x63
#define RADIOLIB_SX130X_REG_MULTI_SF_CHAN_4                      0x64
#define RADIOLIB_SX130X_REG_MULTI_SF_CHAN_5                      0x65
#define RADIOLIB_SX130X_REG_MULTI_SF_CHAN_6                      0x66
#define RADIOLIB_SX130X_REG_MULTI_SF_CHAN_7                      0x67
#define RADIOLIB_SX130X_REG_LORA_STD_CHAN                        0x68
#define RADIOLIB_SX130X_REG_FSK_CHAN                             0x69

// REG_PAGE_RESET
#define RADIOLIB_SX130X_SOFT_RESET                              0b10000000  //  7     7   soft reset
#define RADIOLIB_SX130X_PAGE_0                                  0b00000000  //  1     0   register page 0
#define RADIOLIB_SX130X_PAGE_1                                  0b00000001  //  1     0   register page 1
#define RADIOLIB_SX130X_PAGE_2                                  0b00000010  //  1     0   register page 2
#define RADIOLIB_SX130X_PAGE_3                                  0b00000011  //  1     0   register page 3

// REG_RADIO_A/B_EN
#define RADIOLIB_SX130X_RADIO_EN_ALL                            0b00000111  //  2     0   enable radio A/B

// REG_TX_CTRL
#define RADIOLIB_SX130X_TX_MODE_IMMEDIATE                       0b00000000  //  1     0   transmit immediately
#define RADIOLIB_SX130X_TX_MODE_TIMESTAMPED                     0b00000001  //  1     0   transmit at timestamp
#define RADIOLIB_SX130X_TX_MODE_ON_GPS                          0b00000010  //  1     0   transmit on GPS PPS

// REG_RX_STATUS
#define RADIOLIB_SX130X_RX_STATUS_EMPTY                         0b00000000  //  5     0   FIFO empty
#define RADIOLIB_SX130X_RX_STATUS_LORA_MULTI                    0b00010000  //  5     4   LoRa multi-SF packet
#define RADIOLIB_SX130X_RX_STATUS_LORA_STD                      0b00100000  //  5     4   LoRa standard packet
#define RADIOLIB_SX130X_RX_STATUS_FSK                           0b00110000  //  5     4   FSK packet

// Channel IF frequencies (relative to radio center frequency)
// These are the standard EU868 IF offsets
#define RADIOLIB_SX130X_IF_FREQ_0                               (-400000)   // Hz
#define RADIOLIB_SX130X_IF_FREQ_1                               (-200000)   // Hz
#define RADIOLIB_SX130X_IF_FREQ_2                               0           // Hz
#define RADIOLIB_SX130X_IF_FREQ_3                               200000      // Hz
#define RADIOLIB_SX130X_IF_FREQ_4                               400000      // Hz
#define RADIOLIB_SX130X_IF_FREQ_5                               (-300000)   // Hz
#define RADIOLIB_SX130X_IF_FREQ_6                               (-100000)   // Hz
#define RADIOLIB_SX130X_IF_FREQ_7                               100000      // Hz

/*!
  \struct SX130xChannelConfig_t
  \brief Configuration for a single LoRa multi-SF channel.
*/
struct SX130xChannelConfig_t {
  /*! \brief Channel enabled */
  bool enabled;
  /*! \brief Radio chain to use (0 = radio A, 1 = radio B) */
  uint8_t radio;
  /*! \brief IF frequency offset from radio center frequency in Hz */
  int32_t ifFreq;
};

/*!
  \struct SX130xLoRaStdChannelConfig_t
  \brief Configuration for the LoRa single-SF (standard) channel.
*/
struct SX130xLoRaStdChannelConfig_t {
  /*! \brief Channel enabled */
  bool enabled;
  /*! \brief Radio chain to use (0 = radio A, 1 = radio B) */
  uint8_t radio;
  /*! \brief IF frequency offset from radio center frequency in Hz */
  int32_t ifFreq;
  /*! \brief Bandwidth in kHz */
  float bandwidth;
  /*! \brief Spreading factor */
  uint8_t spreadingFactor;
};

/*!
  \struct SX130xFSKChannelConfig_t
  \brief Configuration for the FSK channel.
*/
struct SX130xFSKChannelConfig_t {
  /*! \brief Channel enabled */
  bool enabled;
  /*! \brief Radio chain to use (0 = radio A, 1 = radio B) */
  uint8_t radio;
  /*! \brief IF frequency offset from radio center frequency in Hz */
  int32_t ifFreq;
  /*! \brief Bit rate in kbps */
  float bitRate;
  /*! \brief Bandwidth in kHz */
  float bandwidth;
  /*! \brief Frequency deviation in kHz */
  float freqDev;
};

/*!
  \struct SX130xPacket_t
  \brief Received packet from the SX130x concentrator.
*/
struct SX130xPacket_t {
  /*! \brief Frequency in MHz */
  float freq;
  /*! \brief Spreading factor (LoRa only) */
  uint8_t sf;
  /*! \brief Bandwidth in kHz (LoRa only) */
  float bw;
  /*! \brief Coding rate (LoRa only) */
  uint8_t cr;
  /*! \brief RSSI in dBm */
  float rssi;
  /*! \brief SNR in dB (LoRa only) */
  float snr;
  /*! \brief Payload length */
  uint8_t len;
  /*! \brief Payload data */
  uint8_t payload[RADIOLIB_SX130X_MAX_PACKET_LENGTH];
  /*! \brief Timestamp (internal counter) */
  uint32_t timestamp;
  /*! \brief CRC status: true = CRC OK */
  bool crcOk;
  /*! \brief Modem type: 0 = LoRa multi-SF, 1 = LoRa std, 2 = FSK */
  uint8_t modem;
};

/*!
  \class SX130x
  \brief Base class for %SX130x series LoRaWAN concentrators.
  All derived classes for %SX130x (e.g. SX1301 or SX1302) inherit from this base class.
  This class should not be instantiated directly from user code, only from its derived classes.

  Unlike single-channel transceivers, the SX130x is a multi-channel concentrator designed
  for LoRaWAN gateway applications. It can receive on 8 multi-SF LoRa channels simultaneously,
  plus 1 LoRa single-SF channel and 1 FSK channel.
*/
class SX130x: public PhysicalLayer {
  public:
    // introduce PhysicalLayer overloads
    using PhysicalLayer::transmit;
    using PhysicalLayer::receive;
    using PhysicalLayer::startTransmit;
    using PhysicalLayer::startReceive;
    using PhysicalLayer::readData;

    /*!
      \brief Default constructor.
      \param mod Instance of Module that will be used to communicate with the concentrator.
    */
    explicit SX130x(Module* mod);

    // basic methods

    /*!
      \brief Initialization method. Sets up the concentrator with radio A and B center frequencies.
      \param freqA Center frequency for radio chain A in MHz.
      \param freqB Center frequency for radio chain B in MHz.
      \returns \ref status_codes
    */
    int16_t begin(float freqA, float freqB);

    /*!
      \brief Reset the concentrator using the RST pin.
      \returns \ref status_codes
    */
    int16_t reset();

    /*!
      \brief Blocking transmit. Sends a single downlink packet.
      \param data Binary data to be sent.
      \param len Number of bytes to send.
      \param addr Unused, kept for PhysicalLayer compatibility.
      \returns \ref status_codes
    */
    int16_t transmit(const uint8_t* data, size_t len, uint8_t addr = 0) override;

    /*!
      \brief Blocking receive. Reads one packet from the FIFO if available.
      \param data Pointer to array to save the received binary data.
      \param len Number of bytes to read.
      \param timeout Timeout in milliseconds.
      \returns \ref status_codes
    */
    int16_t receive(uint8_t* data, size_t len, RadioLibTime_t timeout = 0) override;

    /*!
      \brief Reads one full packet (with metadata) from the concentrator FIFO.
      \param pkt Pointer to SX130xPacket_t structure to populate.
      \returns \ref status_codes
    */
    int16_t readPacket(SX130xPacket_t* pkt);

    /*!
      \brief Returns the number of packets waiting in the FIFO.
      \returns Number of packets, or negative error code.
    */
    int16_t getPacketCount();

    /*!
      \brief Starts continuous receive mode (concentrator always listens).
      \returns \ref status_codes
    */
    int16_t startReceive() override;

    /*!
      \brief Reads data after startReceive. Alias for readData.
      \param data Pointer to array to save the received binary data.
      \param len Number of bytes to read.
      \returns \ref status_codes
    */
    int16_t readData(uint8_t* data, size_t len) override;

    /*!
      \brief Clean up after transmission.
      \returns \ref status_codes
    */
    int16_t finishTransmit() override;

    /*!
      \brief Clean up after reception.
      \returns \ref status_codes
    */
    int16_t finishReceive() override;

    /*!
      \brief Sets the module to sleep mode.
      \returns \ref status_codes
    */
    int16_t sleep() override;

    /*!
      \brief Sets the module to standby mode.
      \returns \ref status_codes
    */
    int16_t standby() override;

    /*!
      \brief Sets the module to standby mode.
      \param mode Standby mode (unused, for PhysicalLayer compatibility).
      \returns \ref status_codes
    */
    int16_t standby(uint8_t mode) override;

    // Not applicable for concentrators - stub implementations for PhysicalLayer compatibility
    int16_t transmitDirect(uint32_t frf = 0) override;
    int16_t receiveDirect() override;
    int16_t scanChannel() override;
    int16_t scanChannel(const ChannelScanConfig_t &config) override;
    int16_t startChannelScan() override;
    int16_t startChannelScan(const ChannelScanConfig_t &config) override;
    int16_t getChannelScanResult() override;
    uint32_t getIrqFlags() override;
    int16_t setIrqFlags(uint32_t irq) override;
    int16_t clearIrqFlags(uint32_t irq) override;
    uint8_t randomByte() override;
    int16_t invertIQ(bool enable) override;
    int16_t setEncoding(uint8_t encoding) override;
    int16_t setDataShaping(uint8_t sh) override;
    int16_t setFrequencyDeviation(float freqDev) override;
    int16_t setBitRate(float br) override;
    int16_t setPreambleLength(size_t preambleLength) override;
    int16_t setSyncWord(uint8_t* syncWord, size_t len) override;
    size_t getPacketLength(bool update = true) override;
    RadioLibTime_t getTimeOnAir(size_t len) override;
    RadioLibTime_t calculateRxTimeout(RadioLibTime_t timeoutUs) override;
    int16_t stageMode(RadioModeType_t mode, RadioModeConfig_t* cfg) override;
    int16_t launchMode() override;
    int16_t getModem(ModemType_t* modem) override;

    #if !RADIOLIB_EXCLUDE_DIRECT_RECEIVE
    void setDirectAction(void (*func)(void)) override;
    void readBit(uint32_t pin) override;
    #endif

    // channel configuration

    /*!
      \brief Configure a LoRa multi-SF channel.
      \param channel Channel index (0-7).
      \param cfg Channel configuration.
      \returns \ref status_codes
    */
    int16_t setLoRaChannel(uint8_t channel, const SX130xChannelConfig_t& cfg);

    /*!
      \brief Configure the LoRa single-SF (standard) channel.
      \param cfg Channel configuration.
      \returns \ref status_codes
    */
    int16_t setLoRaStdChannel(const SX130xLoRaStdChannelConfig_t& cfg);

    /*!
      \brief Configure the FSK channel.
      \param cfg Channel configuration.
      \returns \ref status_codes
    */
    int16_t setFSKChannel(const SX130xFSKChannelConfig_t& cfg);

    /*!
      \brief Set all 8 LoRa multi-SF channels at once using a standard regional plan.
      \param freqBase Base frequency of the first channel in MHz.
      \param freqStep Channel spacing in MHz (e.g. 0.2 for EU868).
      \param radio Radio chain assignment array (8 entries, 0 = radio A, 1 = radio B).
      \returns \ref status_codes
    */
    int16_t setLoRaChannels(float freqBase, float freqStep, const uint8_t radio[RADIOLIB_SX130X_NUM_LORA_CHANNELS]);

    /*!
      \brief Set carrier frequency for radio chain A.
      \param freq Frequency in MHz.
      \returns \ref status_codes
    */
    int16_t setFrequencyA(float freq);

    /*!
      \brief Set carrier frequency for radio chain B.
      \param freq Frequency in MHz.
      \returns \ref status_codes
    */
    int16_t setFrequencyB(float freq);

    /*!
      \brief Set output power for downlink transmission.
      \param power Output power in dBm.
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
      \brief Get the current internal timestamp counter.
      \returns 32-bit timestamp value.
    */
    virtual uint32_t getTimestamp();

    /*!
      \brief Read the chip version register.
      \returns Version register value, or negative error code.
    */
    int16_t getChipVersion();

    /*!
      \brief Get RSSI of the last received packet.
      \returns RSSI in dBm.
    */
    float getRSSI() override;

    /*!
      \brief Get SNR of the last received packet.
      \returns SNR in dB.
    */
    float getSNR() override;

    /*!
      \brief Set RF switch pins.
      \param rxEn RX enable pin.
      \param txEn TX enable pin.
    */
    void setRfSwitchPins(uint32_t rxEn, uint32_t txEn);

    /*!
      \brief Set RF switch table.
      \param pins Array of RF switch pins.
      \param table RF switch mode table.
    */
    void setRfSwitchTable(const uint32_t (&pins)[Module::RFSWITCH_MAX_PINS], const Module::RfSwitchMode_t table[]);

#if !RADIOLIB_GODMODE && !RADIOLIB_LOW_LEVEL
  protected:
#endif
    Module* getMod() override;

    int16_t writeRegister(uint8_t addr, uint8_t value);
    int16_t readRegister(uint8_t addr, uint8_t* value);
    int16_t writeRegisterBurst(uint8_t addr, const uint8_t* data, size_t len);
    int16_t readRegisterBurst(uint8_t addr, uint8_t* data, size_t len);

    int16_t setPage(uint8_t page);
    int16_t setRadioFrequency(uint8_t radio, float freq);
    int16_t sx125xWrite(uint8_t radio, uint8_t addr, uint8_t data);
    int16_t sx125xSetup(uint8_t radio, SX125xType_t type, float freq, bool clkOut);

#if !RADIOLIB_GODMODE
  protected:
#endif
    SX125xType_t radioTypeA = RADIOLIB_SX125X_TYPE_SX1257;
    SX125xType_t radioTypeB = RADIOLIB_SX125X_TYPE_SX1257;
    float freqA = 0;
    float freqB = 0;
    int8_t txPower = 14;
    float lastRssi = 0;
    float lastSnr = 0;
    uint8_t lastPacketLen = 0;

    SX130xChannelConfig_t loraChannels[RADIOLIB_SX130X_NUM_LORA_CHANNELS];
    SX130xLoRaStdChannelConfig_t loraStdChannel;
    SX130xFSKChannelConfig_t fskChannel;

#if !RADIOLIB_GODMODE
  private:
#endif
    Module* mod;
    uint8_t currentPage = 0;

    int16_t configChannels();
};

#endif

#endif
