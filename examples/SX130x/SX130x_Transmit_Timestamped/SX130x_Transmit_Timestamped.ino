/*
  RadioLib SX130x Timestamped Transmit Example

  This example demonstrates the SX130x downlink (transmit) capability,
  which is unique to gateway concentrators. In LoRaWAN, gateways send
  downlinks at precise times relative to the uplink receive timestamp.

  The SX130x maintains an internal free-running counter used to schedule
  transmissions. The SX1302 provides microsecond resolution (32-bit),
  while the SX1301 provides millisecond resolution.

  Typical LoRaWAN downlink windows:
    RX1: 1,000,000 us after uplink timestamp
    RX2: 2,000,000 us after uplink timestamp (fixed frequency/SF)

  This example receives a packet, then schedules a downlink reply
  in the RX1 window using the packet's timestamp.

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

// RX1 delay in microseconds (1 second for LoRaWAN Class A)
#define RX1_DELAY_US  1000000UL

void setup() {
  Serial.begin(9600);

  Serial.print(F("[SX1302] Initializing ... "));
  int state = concentrator.begin(867.5, 868.5);
  if (state == RADIOLIB_ERR_NONE) {
    Serial.println(F("success!"));
  } else {
    Serial.print(F("failed, code "));
    Serial.println(state);
    while (true) { delay(10); }
  }

  // EU868 channel plan
  const uint8_t radioAssign[8] = { 0, 0, 0, 0, 0, 1, 1, 1 };
  concentrator.setLoRaChannels(867.1, 0.2, radioAssign);

  SX130xLoRaStdChannelConfig_t stdCfg = { true, 1, 0, 250.0, 9 };
  concentrator.setLoRaStdChannel(stdCfg);

  SX130xFSKChannelConfig_t fskCfg = { true, 1, 300000, 50.0, 250.0, 25.0 };
  concentrator.setFSKChannel(fskCfg);

  concentrator.startReceive();
  Serial.println(F("[SX1302] Listening for uplinks ..."));
}

void loop() {
  if (concentrator.getPacketCount() <= 0) {
    delay(1);
    return;
  }

  SX130xPacket_t pkt;
  if (concentrator.readPacket(&pkt) != RADIOLIB_ERR_NONE) {
    return;
  }

  if (!pkt.crcOk) {
    Serial.println(F("[SX1302] CRC error, skipping downlink"));
    return;
  }

  Serial.print(F("[SX1302] Uplink received at timestamp "));
  Serial.println(pkt.timestamp);

  // read the current concentrator timestamp (microseconds on SX1302)
  uint32_t now = concentrator.getTimestamp();
  uint32_t rxWindow = pkt.timestamp + RX1_DELAY_US;

  // check if we still have time to schedule the RX1 downlink
  // (need some margin for processing)
  if ((int32_t)(rxWindow - now) < 10000) {
    Serial.println(F("[SX1302] Too late for RX1, skipping"));
    return;
  }

  // wait until the RX1 window opens
  while ((int32_t)(rxWindow - concentrator.getTimestamp()) > 0) {
    // busy-wait; in production use a timer interrupt instead
  }

  // transmit a downlink reply on the same frequency as the uplink
  // in a real gateway this would be the LoRaWAN downlink payload
  uint8_t downlink[] = { 0x60, 0x01, 0x02, 0x03, 0x04 };
  Serial.print(F("[SX1302] Sending downlink ... "));
  int state = concentrator.transmit(downlink, sizeof(downlink));
  if (state == RADIOLIB_ERR_NONE) {
    Serial.println(F("success!"));
  } else {
    Serial.print(F("failed, code "));
    Serial.println(state);
  }

  // resume receive after transmit
  concentrator.startReceive();
}
