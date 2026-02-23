#include "SX130x.h"
#include <string.h>

#if !RADIOLIB_EXCLUDE_SX130X

SX130x::SX130x(Module* mod) : PhysicalLayer() {
  this->freqStep = RADIOLIB_SX130X_FREQUENCY_STEP_SIZE;
  this->maxPacketLength = RADIOLIB_SX130X_MAX_PACKET_LENGTH;
  this->mod = mod;
  memset(this->loraChannels, 0, sizeof(this->loraChannels));
  memset(&this->loraStdChannel, 0, sizeof(this->loraStdChannel));
  memset(&this->fskChannel, 0, sizeof(this->fskChannel));
}

int16_t SX130x::begin(float freqA, float freqB) {
  this->mod->init();

  // reset the chip
  int16_t state = reset();
  RADIOLIB_ASSERT(state);

  // check version
  int16_t ver = getChipVersion();
  if(ver < 0) {
    return(ver);
  }

  // enable radio chains
  state = writeRegister(RADIOLIB_SX130X_REG_RADIO_A_EN, RADIOLIB_SX130X_RADIO_EN_ALL);
  RADIOLIB_ASSERT(state);

  state = writeRegister(RADIOLIB_SX130X_REG_RADIO_B_EN, RADIOLIB_SX130X_RADIO_EN_ALL);
  RADIOLIB_ASSERT(state);

  // set radio frequencies
  state = setFrequencyA(freqA);
  RADIOLIB_ASSERT(state);

  state = setFrequencyB(freqB);
  RADIOLIB_ASSERT(state);

  return(RADIOLIB_ERR_NONE);
}

int16_t SX130x::reset() {
  this->mod->hal->pinMode(this->mod->getRst(), this->mod->hal->GpioModeOutput);
  this->mod->hal->digitalWrite(this->mod->getRst(), this->mod->hal->GpioLevelLow);
  this->mod->hal->delay(10);
  this->mod->hal->digitalWrite(this->mod->getRst(), this->mod->hal->GpioLevelHigh);
  this->mod->hal->delay(10);

  // soft reset via register
  int16_t state = writeRegister(RADIOLIB_SX130X_REG_PAGE_RESET, RADIOLIB_SX130X_SOFT_RESET);
  RADIOLIB_ASSERT(state);

  this->mod->hal->delay(5);
  return(RADIOLIB_ERR_NONE);
}

int16_t SX130x::transmit(const uint8_t* data, size_t len, uint8_t addr) {
  (void)addr;
  if(len > RADIOLIB_SX130X_MAX_PACKET_LENGTH) {
    return(RADIOLIB_ERR_PACKET_TOO_LONG);
  }

  // write payload to TX buffer
  int16_t state = writeRegister(RADIOLIB_SX130X_REG_TX_CTRL, RADIOLIB_SX130X_TX_MODE_IMMEDIATE);
  RADIOLIB_ASSERT(state);

  state = writeRegisterBurst(RADIOLIB_SX130X_REG_RX_DATA_BUF_ADDR, data, len);
  RADIOLIB_ASSERT(state);

  // trigger transmission
  state = writeRegister(RADIOLIB_SX130X_REG_TX_START, 0x01);
  RADIOLIB_ASSERT(state);

  return(finishTransmit());
}

int16_t SX130x::receive(uint8_t* data, size_t len, RadioLibTime_t timeout) {
  RadioLibTime_t start = this->mod->hal->millis();
  while(true) {
    int16_t count = getPacketCount();
    if(count < 0) {
      return(count);
    }
    if(count > 0) {
      return(readData(data, len));
    }
    if(this->mod->hal->millis() - start > timeout) {
      return(RADIOLIB_ERR_RX_TIMEOUT);
    }
    this->mod->hal->delay(1);
  }
}

int16_t SX130x::readPacket(SX130xPacket_t* pkt) {
  if(!pkt) {
    return(RADIOLIB_ERR_NULL_POINTER);
  }

  // check FIFO
  int16_t count = getPacketCount();
  if(count <= 0) {
    return(count == 0 ? RADIOLIB_ERR_RX_TIMEOUT : count);
  }

  // read status byte
  uint8_t status = 0;
  int16_t state = readRegister(RADIOLIB_SX130X_REG_RX_STATUS, &status);
  RADIOLIB_ASSERT(state);

  pkt->modem = (status >> 4) & 0x03;

  // read packet length
  uint8_t pktLen = 0;
  state = readRegister(RADIOLIB_SX130X_REG_RX_DATA_BUF_ADDR, &pktLen);
  RADIOLIB_ASSERT(state);

  pkt->len = pktLen;

  // read payload
  state = readRegisterBurst(RADIOLIB_SX130X_REG_RX_DATA_BUF_DATA, pkt->payload, pkt->len);
  RADIOLIB_ASSERT(state);

  // read RSSI (1 byte, value = -RSSI/2 dBm)
  uint8_t rssiRaw = 0;
  state = readRegister(RADIOLIB_SX130X_REG_RX_DATA_BUF_DATA, &rssiRaw);
  RADIOLIB_ASSERT(state);
  pkt->rssi = -(float)rssiRaw / 2.0f;
  this->lastRssi = pkt->rssi;

  // read SNR (1 byte, signed, value = SNR/4 dB)
  uint8_t snrRaw = 0;
  state = readRegister(RADIOLIB_SX130X_REG_RX_DATA_BUF_DATA, &snrRaw);
  RADIOLIB_ASSERT(state);
  pkt->snr = (float)(int8_t)snrRaw / 4.0f;
  this->lastSnr = pkt->snr;

  this->lastPacketLen = pkt->len;
  pkt->crcOk = (status & 0x01) != 0;

  return(RADIOLIB_ERR_NONE);
}

int16_t SX130x::getPacketCount() {
  uint8_t count = 0;
  int16_t state = readRegister(RADIOLIB_SX130X_REG_RX_STAT_FIFO_COUNT, &count);
  RADIOLIB_ASSERT(state);
  return((int16_t)count);
}

int16_t SX130x::startReceive() {
  // concentrator is always in receive mode after begin()
  // just ensure radio chains are enabled
  int16_t state = writeRegister(RADIOLIB_SX130X_REG_RADIO_A_EN, RADIOLIB_SX130X_RADIO_EN_ALL);
  RADIOLIB_ASSERT(state);
  return(writeRegister(RADIOLIB_SX130X_REG_RADIO_B_EN, RADIOLIB_SX130X_RADIO_EN_ALL));
}

int16_t SX130x::readData(uint8_t* data, size_t len) {
  SX130xPacket_t pkt;
  int16_t state = readPacket(&pkt);
  RADIOLIB_ASSERT(state);

  size_t copyLen = (len < pkt.len) ? len : pkt.len;
  memcpy(data, pkt.payload, copyLen);
  return(RADIOLIB_ERR_NONE);
}

int16_t SX130x::finishTransmit() {
  // wait for TX done (poll TX_START register cleared)
  RadioLibTime_t start = this->mod->hal->millis();
  while(true) {
    uint8_t val = 0;
    int16_t state = readRegister(RADIOLIB_SX130X_REG_TX_START, &val);
    RADIOLIB_ASSERT(state);
    if(val == 0) {
      break;
    }
    if(this->mod->hal->millis() - start > 5000) {
      return(RADIOLIB_ERR_TX_TIMEOUT);
    }
    this->mod->hal->delay(1);
  }
  return(RADIOLIB_ERR_NONE);
}

int16_t SX130x::finishReceive() {
  return(RADIOLIB_ERR_NONE);
}

int16_t SX130x::sleep() {
  int16_t state = writeRegister(RADIOLIB_SX130X_REG_RADIO_A_EN, 0x00);
  RADIOLIB_ASSERT(state);
  return(writeRegister(RADIOLIB_SX130X_REG_RADIO_B_EN, 0x00));
}

int16_t SX130x::standby() {
  return(sleep());
}

int16_t SX130x::standby(uint8_t mode) {
  (void)mode;
  return(standby());
}

int16_t SX130x::setLoRaChannel(uint8_t channel, const SX130xChannelConfig_t& cfg) {
  if(channel >= RADIOLIB_SX130X_NUM_LORA_CHANNELS) {
    return(RADIOLIB_ERR_INVALID_DIO_PIN);
  }
  memcpy(&this->loraChannels[channel], &cfg, sizeof(SX130xChannelConfig_t));
  return(configChannels());
}

int16_t SX130x::setLoRaStdChannel(const SX130xLoRaStdChannelConfig_t& cfg) {
  memcpy(&this->loraStdChannel, &cfg, sizeof(SX130xLoRaStdChannelConfig_t));
  return(configChannels());
}

int16_t SX130x::setFSKChannel(const SX130xFSKChannelConfig_t& cfg) {
  memcpy(&this->fskChannel, &cfg, sizeof(SX130xFSKChannelConfig_t));
  return(configChannels());
}

int16_t SX130x::setLoRaChannels(float freqBase, float freqStep, const uint8_t radio[RADIOLIB_SX130X_NUM_LORA_CHANNELS]) {
  // calculate IF offsets relative to radio A center frequency
  for(uint8_t i = 0; i < RADIOLIB_SX130X_NUM_LORA_CHANNELS; i++) {
    this->loraChannels[i].enabled = true;
    this->loraChannels[i].radio = radio[i];
    float chanFreq = freqBase + (float)i * freqStep;
    float radioFreq = (radio[i] == 0) ? this->freqA : this->freqB;
    this->loraChannels[i].ifFreq = (int32_t)((chanFreq - radioFreq) * 1e6f);
  }
  return(configChannels());
}

int16_t SX130x::setFrequencyA(float freq) {
  this->freqA = freq;
  return(sx125xSetup(0, this->radioTypeA, freq, true));
}

int16_t SX130x::setFrequencyB(float freq) {
  this->freqB = freq;
  return(sx125xSetup(1, this->radioTypeB, freq, false));
}

int16_t SX130x::setOutputPower(int8_t power) {
  int16_t state = checkOutputPower(power, NULL);
  RADIOLIB_ASSERT(state);
  this->txPower = power;
  // TX gain register: value = (power + offset) mapped to 0-15
  uint8_t gainVal = (uint8_t)((power + 9) & 0x0F);
  return(writeRegister(RADIOLIB_SX130X_REG_TX_GAIN, gainVal));
}

int16_t SX130x::checkOutputPower(int8_t power, int8_t* clipped) {
  if(clipped) {
    *clipped = RADIOLIB_MAX(-6, RADIOLIB_MIN(27, power));
  }
  RADIOLIB_CHECK_RANGE(power, -6, 27, RADIOLIB_ERR_INVALID_OUTPUT_POWER);
  return(RADIOLIB_ERR_NONE);
}

uint32_t SX130x::getTimestamp() {
  uint8_t buf[4] = { 0 };
  (void)readRegisterBurst(RADIOLIB_SX130X_REG_TIMESTAMP, buf, 4);
  return(((uint32_t)buf[0] << 24) | ((uint32_t)buf[1] << 16) | ((uint32_t)buf[2] << 8) | buf[3]);
}

int16_t SX130x::getChipVersion() {
  uint8_t ver = 0;
  int16_t state = readRegister(RADIOLIB_SX130X_REG_VERSION, &ver);
  RADIOLIB_ASSERT(state);
  return((int16_t)ver);
}

float SX130x::getRSSI() {
  return(this->lastRssi);
}

float SX130x::getSNR() {
  return(this->lastSnr);
}

void SX130x::setRfSwitchPins(uint32_t rxEn, uint32_t txEn) {
  this->mod->setRfSwitchPins(rxEn, txEn);
}

void SX130x::setRfSwitchTable(const uint32_t (&pins)[Module::RFSWITCH_MAX_PINS], const Module::RfSwitchMode_t table[]) {
  this->mod->setRfSwitchTable(pins, table);
}

Module* SX130x::getMod() {
  return(this->mod);
}

int16_t SX130x::writeRegister(uint8_t addr, uint8_t value) {
  this->mod->SPIwriteRegister(addr, value);
  return(RADIOLIB_ERR_NONE);
}

int16_t SX130x::readRegister(uint8_t addr, uint8_t* value) {
  *value = this->mod->SPIreadRegister(addr);
  return(RADIOLIB_ERR_NONE);
}

int16_t SX130x::writeRegisterBurst(uint8_t addr, const uint8_t* data, size_t len) {
  this->mod->SPIwriteRegisterBurst(addr, data, len);
  return(RADIOLIB_ERR_NONE);
}

int16_t SX130x::readRegisterBurst(uint8_t addr, uint8_t* data, size_t len) {
  this->mod->SPIreadRegisterBurst(addr, len, data);
  return(RADIOLIB_ERR_NONE);
}

int16_t SX130x::setPage(uint8_t page) {
  if(this->currentPage == page) {
    return(RADIOLIB_ERR_NONE);
  }
  int16_t state = writeRegister(RADIOLIB_SX130X_REG_PAGE_RESET, page & 0x03);
  RADIOLIB_ASSERT(state);
  this->currentPage = page;
  return(RADIOLIB_ERR_NONE);
}

int16_t SX130x::setRadioFrequency(uint8_t radio, float freq) {
  return(sx125xSetup(radio, (radio == 0) ? this->radioTypeA : this->radioTypeB, freq, radio == 0));
}

int16_t SX130x::sx125xWrite(uint8_t radio, uint8_t addr, uint8_t data) {
  // SX1301 internal SPI bridge: write addr (with write bit set) then data
  uint8_t addrReg = (radio == 0) ? RADIOLIB_SX130X_REG_RADIO_A_SPI_DATA : RADIOLIB_SX130X_REG_RADIO_B_SPI_DATA;
  uint8_t clkReg  = (radio == 0) ? RADIOLIB_SX130X_REG_RADIO_A_CLK      : RADIOLIB_SX130X_REG_RADIO_B_CLK;
  uint8_t csReg   = (radio == 0) ? RADIOLIB_SX130X_REG_RADIO_A_STAT      : RADIOLIB_SX130X_REG_RADIO_B_STAT;

  int16_t state = writeRegister(csReg, 0);
  RADIOLIB_ASSERT(state);
  state = writeRegister(addrReg, 0x80 | addr);  // MSB=1 for write
  RADIOLIB_ASSERT(state);
  state = writeRegister(addrReg + 1, data);     // DATA register is addr+1
  RADIOLIB_ASSERT(state);
  state = writeRegister(clkReg, 1);
  RADIOLIB_ASSERT(state);
  state = writeRegister(clkReg, 0);
  RADIOLIB_ASSERT(state);
  return(writeRegister(csReg, 0));
}

int16_t SX130x::sx125xSetup(uint8_t radio, SX125xType_t type, float freq, bool clkOut) {
  // clock output: chain 0 drives the 32 MHz ref to chain 1
  uint8_t clkSel = clkOut ? (RADIOLIB_SX125X_TX_DAC_CLK_SEL + 2) : RADIOLIB_SX125X_TX_DAC_CLK_SEL;
  int16_t state = sx125xWrite(radio, RADIOLIB_SX125X_REG_TX_DAC_CLK, clkSel);
  RADIOLIB_ASSERT(state);

  // XOSC startup (register address differs between SX1255 and SX1257)
  uint8_t xoscReg = (type == RADIOLIB_SX125X_TYPE_SX1257) ? RADIOLIB_SX125X_REG_XOSC_SX1257 : RADIOLIB_SX125X_REG_XOSC_SX1255;
  state = sx125xWrite(radio, xoscReg, RADIOLIB_SX125X_XOSC_GM_STARTUP + RADIOLIB_SX125X_XOSC_DISABLE * 16);
  RADIOLIB_ASSERT(state);

  // TX gain and trim
  state = sx125xWrite(radio, RADIOLIB_SX125X_REG_TX_GAIN, RADIOLIB_SX125X_TX_MIX_GAIN + RADIOLIB_SX125X_TX_DAC_GAIN * 16);
  RADIOLIB_ASSERT(state);
  state = sx125xWrite(radio, RADIOLIB_SX125X_REG_TX_BW, RADIOLIB_SX125X_TX_ANA_BW + RADIOLIB_SX125X_TX_PLL_BW * 32);
  RADIOLIB_ASSERT(state);
  state = sx125xWrite(radio, RADIOLIB_SX125X_REG_TX_DAC_BW, RADIOLIB_SX125X_TX_DAC_BW);
  RADIOLIB_ASSERT(state);

  // RX gain and trim
  state = sx125xWrite(radio, RADIOLIB_SX125X_REG_RX_ANA, RADIOLIB_SX125X_LNA_ZIN + RADIOLIB_SX125X_RX_BB_GAIN * 2 + RADIOLIB_SX125X_RX_LNA_GAIN * 32);
  RADIOLIB_ASSERT(state);
  state = sx125xWrite(radio, RADIOLIB_SX125X_REG_RX_BW, RADIOLIB_SX125X_RX_BB_BW + RADIOLIB_SX125X_RX_ADC_TRIM * 4 + RADIOLIB_SX125X_RX_ADC_BW * 32);
  RADIOLIB_ASSERT(state);
  state = sx125xWrite(radio, RADIOLIB_SX125X_REG_RX_PLL, RADIOLIB_SX125X_ADC_TEMP + RADIOLIB_SX125X_RX_PLL_BW * 2);
  RADIOLIB_ASSERT(state);

  // PLL frequency word (differs between SX1255 and SX1257)
  uint32_t freqHz = (uint32_t)(freq * 1e6f);
  uint32_t partInt, partFrac;
  if(type == RADIOLIB_SX125X_TYPE_SX1257) {
    partInt  = freqHz / (RADIOLIB_SX125X_32MHZ_FRAC << 8);
    partFrac = ((freqHz % (RADIOLIB_SX125X_32MHZ_FRAC << 8)) << 8) / RADIOLIB_SX125X_32MHZ_FRAC;
  } else {
    partInt  = freqHz / (RADIOLIB_SX125X_32MHZ_FRAC << 7);
    partFrac = ((freqHz % (RADIOLIB_SX125X_32MHZ_FRAC << 7)) << 9) / RADIOLIB_SX125X_32MHZ_FRAC;
  }
  state = sx125xWrite(radio, RADIOLIB_SX125X_REG_FREQ_MSB, (uint8_t)(partInt & 0xFF));
  RADIOLIB_ASSERT(state);
  state = sx125xWrite(radio, RADIOLIB_SX125X_REG_FREQ_MID, (uint8_t)((partFrac >> 8) & 0xFF));
  RADIOLIB_ASSERT(state);
  state = sx125xWrite(radio, RADIOLIB_SX125X_REG_FREQ_LSB, (uint8_t)(partFrac & 0xFF));
  RADIOLIB_ASSERT(state);

  // enable XOSC then RX (PLL+FE), wait for PLL lock
  state = sx125xWrite(radio, RADIOLIB_SX125X_REG_MODE, RADIOLIB_SX125X_MODE_XTAL_ON);
  RADIOLIB_ASSERT(state);
  state = sx125xWrite(radio, RADIOLIB_SX125X_REG_MODE, RADIOLIB_SX125X_MODE_RX_ON);
  RADIOLIB_ASSERT(state);
  this->mod->hal->delay(1);

  return(RADIOLIB_ERR_NONE);
}

int16_t SX130x::configChannels() {
  // write multi-SF channel IF frequencies
  for(uint8_t i = 0; i < RADIOLIB_SX130X_NUM_LORA_CHANNELS; i++) {
    uint8_t reg = RADIOLIB_SX130X_REG_MULTI_SF_CHAN_0 + i;
    uint8_t val = 0;
    if(this->loraChannels[i].enabled) {
      // encode: bit7 = radio select, bits[6:0] = IF/1000 (kHz offset, signed 7-bit)
      int8_t ifKhz = (int8_t)(this->loraChannels[i].ifFreq / 1000);
      val = (uint8_t)((this->loraChannels[i].radio << 7) | (ifKhz & 0x7F));
    }
    int16_t state = writeRegister(reg, val);
    RADIOLIB_ASSERT(state);
  }

  // write LoRa std channel
  if(this->loraStdChannel.enabled) {
    int8_t ifKhz = (int8_t)(this->loraStdChannel.ifFreq / 1000);
    uint8_t val = (uint8_t)((this->loraStdChannel.radio << 7) | (ifKhz & 0x7F));
    int16_t state = writeRegister(RADIOLIB_SX130X_REG_LORA_STD_CHAN, val);
    RADIOLIB_ASSERT(state);
  }

  // write FSK channel
  if(this->fskChannel.enabled) {
    int8_t ifKhz = (int8_t)(this->fskChannel.ifFreq / 1000);
    uint8_t val = (uint8_t)((this->fskChannel.radio << 7) | (ifKhz & 0x7F));
    int16_t state = writeRegister(RADIOLIB_SX130X_REG_FSK_CHAN, val);
    RADIOLIB_ASSERT(state);
  }

  return(RADIOLIB_ERR_NONE);
}

// PhysicalLayer stubs - not applicable for concentrators
int16_t SX130x::transmitDirect(uint32_t frf) { (void)frf; return(RADIOLIB_ERR_UNSUPPORTED); }
int16_t SX130x::receiveDirect() { return(RADIOLIB_ERR_UNSUPPORTED); }
int16_t SX130x::scanChannel() { return(RADIOLIB_ERR_UNSUPPORTED); }
int16_t SX130x::scanChannel(const ChannelScanConfig_t &config) { (void)config; return(RADIOLIB_ERR_UNSUPPORTED); }
int16_t SX130x::startChannelScan() { return(RADIOLIB_ERR_UNSUPPORTED); }
int16_t SX130x::startChannelScan(const ChannelScanConfig_t &config) { (void)config; return(RADIOLIB_ERR_UNSUPPORTED); }
int16_t SX130x::getChannelScanResult() { return(RADIOLIB_ERR_UNSUPPORTED); }
uint32_t SX130x::getIrqFlags() { return(0); }
int16_t SX130x::setIrqFlags(uint32_t irq) { (void)irq; return(RADIOLIB_ERR_UNSUPPORTED); }
int16_t SX130x::clearIrqFlags(uint32_t irq) { (void)irq; return(RADIOLIB_ERR_UNSUPPORTED); }
uint8_t SX130x::randomByte() { return(0); }
int16_t SX130x::invertIQ(bool enable) { (void)enable; return(RADIOLIB_ERR_UNSUPPORTED); }
int16_t SX130x::setEncoding(uint8_t encoding) { (void)encoding; return(RADIOLIB_ERR_UNSUPPORTED); }
int16_t SX130x::setDataShaping(uint8_t sh) { (void)sh; return(RADIOLIB_ERR_UNSUPPORTED); }
int16_t SX130x::setFrequencyDeviation(float freqDev) { (void)freqDev; return(RADIOLIB_ERR_UNSUPPORTED); }
int16_t SX130x::setBitRate(float br) { (void)br; return(RADIOLIB_ERR_UNSUPPORTED); }
int16_t SX130x::setPreambleLength(size_t preambleLength) { (void)preambleLength; return(RADIOLIB_ERR_UNSUPPORTED); }
int16_t SX130x::setSyncWord(uint8_t* syncWord, size_t len) { (void)syncWord; (void)len; return(RADIOLIB_ERR_UNSUPPORTED); }
size_t SX130x::getPacketLength(bool update) { (void)update; return(this->lastPacketLen); }
RadioLibTime_t SX130x::getTimeOnAir(size_t len) { (void)len; return(0); }
RadioLibTime_t SX130x::calculateRxTimeout(RadioLibTime_t timeoutUs) { return(timeoutUs); }
int16_t SX130x::stageMode(RadioModeType_t mode, RadioModeConfig_t* cfg) { (void)mode; (void)cfg; return(RADIOLIB_ERR_UNSUPPORTED); }
int16_t SX130x::launchMode() { return(RADIOLIB_ERR_UNSUPPORTED); }
int16_t SX130x::getModem(ModemType_t* modem) { (void)modem; return(RADIOLIB_ERR_UNSUPPORTED); }

#if !RADIOLIB_EXCLUDE_DIRECT_RECEIVE
void SX130x::setDirectAction(void (*func)(void)) { (void)func; }
void SX130x::readBit(uint32_t pin) { (void)pin; }
#endif

#endif
