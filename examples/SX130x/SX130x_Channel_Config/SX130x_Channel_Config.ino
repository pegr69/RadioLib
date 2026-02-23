/*
  RadioLib SX130x Channel Configuration Example

  This example demonstrates how to configure the SX130x concentrator
  for different regional channel plans. The SX130x supports:
    - 8 LoRa multi-SF channels (SF7-SF12 decoded simultaneously per channel)
    - 1 LoRa single-SF (standard) channel
    - 1 FSK channel

  Each channel is defined as an IF offset from one of two radio chain
  center frequencies (radio A or radio B).

  Three regional plans are shown:
    - EU868 (default, 8 channels, 200 kHz spacing)
    - US915 (8 of 64 uplink channels, 200 kHz spacing)
    - AU915 (same plan as US915 but different frequencies)

  For full API reference, see the GitHub Pages
  https://jgromes.github.io/RadioLib/
*/

#include <RadioLib.h>

// SX1301 has the following connections:
// NSS pin:   10
// IRQ pin:   2
// NRST pin:  3
// BUSY pin:  9
SX1301 concentrator = new Module(10, 2, 3, 9);

// select regional plan
#define PLAN_EU868
//#define PLAN_US915
//#define PLAN_AU915

void setup() {
  Serial.begin(9600);

#if defined(PLAN_EU868)
  float freqA = 867.5, freqB = 868.5;
  float chanBase = 867.1, chanStep = 0.2;
  const uint8_t radioAssign[8] = { 0, 0, 0, 0, 0, 1, 1, 1 };
  // LoRa std: 868.5 MHz, SF9, 250 kHz
  SX130xLoRaStdChannelConfig_t stdCfg = { true, 1, 0, 250.0, 9 };
  // FSK: 868.8 MHz
  SX130xFSKChannelConfig_t fskCfg = { true, 1, 300000, 50.0, 250.0, 25.0 };
  Serial.println(F("[SX1301] Using EU868 channel plan"));

#elif defined(PLAN_US915)
  // US915 sub-band 1: 902.3 - 903.7 MHz (channels 0-7)
  float freqA = 902.7, freqB = 903.5;
  float chanBase = 902.3, chanStep = 0.2;
  const uint8_t radioAssign[8] = { 0, 0, 0, 0, 1, 1, 1, 1 };
  // LoRa std: 903.0 MHz, SF8, 500 kHz
  SX130xLoRaStdChannelConfig_t stdCfg = { true, 0, 300000, 500.0, 8 };
  SX130xFSKChannelConfig_t fskCfg = { false, 0, 0, 0, 0, 0 };
  Serial.println(F("[SX1301] Using US915 channel plan (sub-band 1)"));

#elif defined(PLAN_AU915)
  // AU915 sub-band 1: 915.2 - 916.6 MHz (channels 0-7)
  float freqA = 915.6, freqB = 916.4;
  float chanBase = 915.2, chanStep = 0.2;
  const uint8_t radioAssign[8] = { 0, 0, 0, 0, 1, 1, 1, 1 };
  // LoRa std: 915.9 MHz, SF8, 500 kHz
  SX130xLoRaStdChannelConfig_t stdCfg = { true, 0, 300000, 500.0, 8 };
  SX130xFSKChannelConfig_t fskCfg = { false, 0, 0, 0, 0, 0 };
  Serial.println(F("[SX1301] Using AU915 channel plan (sub-band 1)"));
#endif

  // initialize with the two radio chain center frequencies
  Serial.print(F("[SX1301] Initializing ... "));
  int state = concentrator.begin(freqA, freqB);
  if (state == RADIOLIB_ERR_NONE) {
    Serial.println(F("success!"));
  } else {
    Serial.print(F("failed, code "));
    Serial.println(state);
    while (true) { delay(10); }
  }

  // set all 8 multi-SF channels using the regional plan
  Serial.print(F("[SX1301] Setting LoRa channels ... "));
  state = concentrator.setLoRaChannels(chanBase, chanStep, radioAssign);
  if (state == RADIOLIB_ERR_NONE) {
    Serial.println(F("success!"));
  } else {
    Serial.print(F("failed, code "));
    Serial.println(state);
    while (true) { delay(10); }
  }

  // set LoRa single-SF channel
  Serial.print(F("[SX1301] Setting LoRa std channel ... "));
  state = concentrator.setLoRaStdChannel(stdCfg);
  if (state == RADIOLIB_ERR_NONE) {
    Serial.println(F("success!"));
  } else {
    Serial.print(F("failed, code "));
    Serial.println(state);
    while (true) { delay(10); }
  }

  // set FSK channel
  Serial.print(F("[SX1301] Setting FSK channel ... "));
  state = concentrator.setFSKChannel(fskCfg);
  if (state == RADIOLIB_ERR_NONE) {
    Serial.println(F("success!"));
  } else {
    Serial.print(F("failed, code "));
    Serial.println(state);
    while (true) { delay(10); }
  }

  // alternatively, configure individual channels manually for custom plans
  // this sets channel 0 to radio A with a -400 kHz IF offset
  /*
  SX130xChannelConfig_t ch0 = {
    .enabled = true,
    .radio = 0,
    .ifFreq = -400000
  };
  state = concentrator.setLoRaChannel(0, ch0);
  */

  Serial.print(F("[SX1301] Starting receive ... "));
  state = concentrator.startReceive();
  if (state == RADIOLIB_ERR_NONE) {
    Serial.println(F("success!"));
  } else {
    Serial.print(F("failed, code "));
    Serial.println(state);
    while (true) { delay(10); }
  }

  Serial.println(F("[SX1301] Gateway ready."));
}

void loop() {
  int16_t count = concentrator.getPacketCount();
  if (count <= 0) {
    delay(10);
    return;
  }

  SX130xPacket_t pkt;
  int state = concentrator.readPacket(&pkt);
  if (state == RADIOLIB_ERR_NONE) {
    Serial.print(F("[SX1301] Packet on "));
    Serial.print(pkt.freq, 3);
    Serial.print(F(" MHz SF"));
    Serial.print(pkt.sf);
    Serial.print(F(" RSSI="));
    Serial.print(pkt.rssi);
    Serial.print(F(" dBm len="));
    Serial.println(pkt.len);
  }
}
