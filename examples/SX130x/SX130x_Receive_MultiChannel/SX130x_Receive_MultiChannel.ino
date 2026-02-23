/*
  RadioLib SX130x Multi-Channel Receive Example

  This example demonstrates the primary use case of the SX130x concentrator:
  simultaneous reception on 8 LoRa multi-SF channels, 1 LoRa single-SF channel,
  and 1 FSK channel. This is the foundation of a LoRaWAN gateway.

  Unlike single-channel transceivers, the SX130x does not need to be tuned to
  a specific spreading factor - it decodes all SFs (7-12) on each channel at once.

  The concentrator uses two radio chains (A and B), each tuned to a center
  frequency. Channels are defined as IF offsets from these center frequencies.

  This example uses EU868 channel plan as a reference:
    Radio A: 867.5 MHz  (channels 0-4)
    Radio B: 868.5 MHz  (channels 5-7 + LoRa std + FSK)

  For full API reference, see the GitHub Pages
  https://jgromes.github.io/RadioLib/
*/

#include <RadioLib.h>

// SX1302 has the following connections:
// NSS pin:   10
// IRQ pin:   2
// NRST pin:  3
// BUSY pin:  9
SX1302 concentrator = new Module(10, 2, 3, 9);

void setup() {
  Serial.begin(9600);

  // initialize SX1302 with EU868 radio chain center frequencies
  Serial.print(F("[SX1302] Initializing ... "));
  int state = concentrator.begin(867.5, 868.5);
  if (state == RADIOLIB_ERR_NONE) {
    Serial.println(F("success!"));
  } else {
    Serial.print(F("failed, code "));
    Serial.println(state);
    while (true) { delay(10); }
  }

  // configure 8 multi-SF LoRa channels (EU868 standard plan)
  // channels 0-4 on radio A (867.5 MHz center), channels 5-7 on radio B (868.5 MHz center)
  // IF offsets place channels at 867.1, 867.3, 867.5, 867.7, 867.9, 868.1, 868.3, 868.5 MHz
  const uint8_t radioAssign[RADIOLIB_SX130X_NUM_LORA_CHANNELS] = { 0, 0, 0, 0, 0, 1, 1, 1 };
  Serial.print(F("[SX1302] Setting LoRa channels ... "));
  state = concentrator.setLoRaChannels(867.1, 0.2, radioAssign);
  if (state == RADIOLIB_ERR_NONE) {
    Serial.println(F("success!"));
  } else {
    Serial.print(F("failed, code "));
    Serial.println(state);
    while (true) { delay(10); }
  }

  // configure LoRa single-SF channel at 868.5 MHz (radio B, SF9, 250 kHz BW)
  SX130xLoRaStdChannelConfig_t stdCfg = {
    .enabled = true,
    .radio = 1,
    .ifFreq = 0,
    .bandwidth = 250.0,
    .spreadingFactor = 9
  };
  Serial.print(F("[SX1302] Setting LoRa std channel ... "));
  state = concentrator.setLoRaStdChannel(stdCfg);
  if (state == RADIOLIB_ERR_NONE) {
    Serial.println(F("success!"));
  } else {
    Serial.print(F("failed, code "));
    Serial.println(state);
    while (true) { delay(10); }
  }

  // configure FSK channel at 868.8 MHz (radio B, 50 kbps, 250 kHz BW)
  SX130xFSKChannelConfig_t fskCfg = {
    .enabled = true,
    .radio = 1,
    .ifFreq = 300000,
    .bitRate = 50.0,
    .bandwidth = 250.0,
    .freqDev = 25.0
  };
  Serial.print(F("[SX1302] Setting FSK channel ... "));
  state = concentrator.setFSKChannel(fskCfg);
  if (state == RADIOLIB_ERR_NONE) {
    Serial.println(F("success!"));
  } else {
    Serial.print(F("failed, code "));
    Serial.println(state);
    while (true) { delay(10); }
  }

  // start continuous receive - concentrator listens on all channels simultaneously
  Serial.print(F("[SX1302] Starting receive ... "));
  state = concentrator.startReceive();
  if (state == RADIOLIB_ERR_NONE) {
    Serial.println(F("success!"));
  } else {
    Serial.print(F("failed, code "));
    Serial.println(state);
    while (true) { delay(10); }
  }

  Serial.println(F("[SX1302] Listening on all channels ..."));
}

void loop() {
  // check how many packets are waiting in the FIFO
  int16_t count = concentrator.getPacketCount();
  if (count <= 0) {
    delay(10);
    return;
  }

  // read one full packet with metadata
  SX130xPacket_t pkt;
  int state = concentrator.readPacket(&pkt);
  if (state != RADIOLIB_ERR_NONE) {
    Serial.print(F("[SX1302] Read failed, code "));
    Serial.println(state);
    return;
  }

  Serial.println(F("[SX1302] Received packet!"));

  // modem type: 0 = LoRa multi-SF, 1 = LoRa std, 2 = FSK
  Serial.print(F("[SX1302] Modem:\t\t"));
  if (pkt.modem == 0) {
    Serial.println(F("LoRa multi-SF"));
  } else if (pkt.modem == 1) {
    Serial.println(F("LoRa std"));
  } else {
    Serial.println(F("FSK"));
  }

  Serial.print(F("[SX1302] Frequency:\t"));
  Serial.print(pkt.freq, 3);
  Serial.println(F(" MHz"));

  if (pkt.modem != 2) {
    Serial.print(F("[SX1302] SF:\t\tSF"));
    Serial.println(pkt.sf);
    Serial.print(F("[SX1302] BW:\t\t"));
    Serial.print(pkt.bw, 0);
    Serial.println(F(" kHz"));
  }

  Serial.print(F("[SX1302] RSSI:\t\t"));
  Serial.print(pkt.rssi);
  Serial.println(F(" dBm"));

  if (pkt.modem != 2) {
    Serial.print(F("[SX1302] SNR:\t\t"));
    Serial.print(pkt.snr);
    Serial.println(F(" dB"));
  }

  Serial.print(F("[SX1302] CRC:\t\t"));
  Serial.println(pkt.crcOk ? F("OK") : F("ERROR"));

  // internal timestamp (microseconds for SX1302, milliseconds for SX1301)
  Serial.print(F("[SX1302] Timestamp:\t"));
  Serial.println(pkt.timestamp);

  Serial.print(F("[SX1302] Data ("));
  Serial.print(pkt.len);
  Serial.print(F(" bytes):\t"));
  for (uint8_t i = 0; i < pkt.len; i++) {
    Serial.print(pkt.payload[i], HEX);
    Serial.print(' ');
  }
  Serial.println();
}
