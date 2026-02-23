/*
  RadioLib Non-Arduino Raspberry Pi SX130x Gateway Example

  Supports multiple hardware targets, selected via --hw flag:

    ./rpi-sx1302-gateway --hw waveshare
      Waveshare SX1301 LoRaWAN Hat
      https://www.waveshare.com/wiki/SX1302_LoRaWAN_Gateway_HAT
      NSS=7(CE0)  IRQ=17  NRST=22

    ./rpi-sx1302-gateway --hw rak7244
      RAK7244 / RAK2245 concentrator (SX1301)
      https://docs.rakwireless.com/product-categories/wisgate/rak7244/
      NSS=8(CE0)  IRQ=NONE  NRST=17

  Both use EU868 channel plan. SPI0 CE0 on /dev/spidev0.0.

  For full API reference, see the GitHub Pages
  https://jgromes.github.io/RadioLib/
*/

#include <RadioLib.h>
#include <string.h>
#include "hal/RPi/PiHal.h"

struct HWConfig {
  const char* name;
  uint8_t spiChannel;
  uint32_t nss;
  uint32_t irq;
  uint32_t rst;
};

static const HWConfig HW_WAVESHARE = { "Waveshare SX1301", 0, RADIOLIB_NC, 17, 22 };
static const HWConfig HW_RAK7244   = { "RAK7244",          0, RADIOLIB_NC, RADIOLIB_NC, 17 };

int main(int argc, char** argv) {
  const HWConfig* hw = nullptr;

  for (int i = 1; i < argc; i++) {
    if (strcmp(argv[i], "--hw") == 0 && i + 1 < argc) {
      if (strcmp(argv[i+1], "waveshare") == 0) hw = &HW_WAVESHARE;
      else if (strcmp(argv[i+1], "rak7244") == 0) hw = &HW_RAK7244;
    }
  }

  if (!hw) {
    fprintf(stderr, "Usage: %s --hw <waveshare|rak7244>\n", argv[0]);
    return 1;
  }

  printf("[SX1301] Hardware: %s\n", hw->name);

  PiHal* hal = new PiHal(hw->spiChannel);
  SX1301 concentrator = new Module(hal, hw->nss, hw->irq, hw->rst, RADIOLIB_NC);

  // EU868: radio A = 867.5 MHz, radio B = 868.5 MHz
  // Channel IF offsets from rak7243/global_conf.json:
  //   ch0-2: radio B at -400k/-200k/0
  //   ch3-7: radio A at -400k/-200k/0/+200k/+400k
  printf("[SX1301] Initializing ... ");
  int state = concentrator.begin(867.5, 868.5);
  if (state != RADIOLIB_ERR_NONE) {
    printf("failed, code %d\n", state);
    return 1;
  }
  printf("success!\n");

  // ch0-2 on radio B (idx 1), ch3-7 on radio A (idx 0)
  const uint8_t radioAssign[RADIOLIB_SX130X_NUM_LORA_CHANNELS] = { 1, 1, 1, 0, 0, 0, 0, 0 };
  printf("[SX1301] Setting LoRa channels ... ");
  state = concentrator.setLoRaChannels(868.1, -0.2, radioAssign);
  if (state != RADIOLIB_ERR_NONE) {
    printf("failed, code %d\n", state);
    return 1;
  }
  printf("success!\n");

  // LoRa std: radio B, IF=-200kHz => 868.3 MHz, SF7, 250 kHz BW
  SX130xLoRaStdChannelConfig_t stdCfg = { true, 1, -200000, 250.0, 7 };
  printf("[SX1301] Setting LoRa std channel ... ");
  state = concentrator.setLoRaStdChannel(stdCfg);
  if (state != RADIOLIB_ERR_NONE) {
    printf("failed, code %d\n", state);
    return 1;
  }
  printf("success!\n");

  // FSK: radio B, IF=+300kHz => 868.8 MHz, 50 kbps, 125 kHz BW
  SX130xFSKChannelConfig_t fskCfg = { true, 1, 300000, 50.0, 125.0, 25.0 };
  printf("[SX1301] Setting FSK channel ... ");
  state = concentrator.setFSKChannel(fskCfg);
  if (state != RADIOLIB_ERR_NONE) {
    printf("failed, code %d\n", state);
    return 1;
  }
  printf("success!\n");

  printf("[SX1301] Starting receive ... ");
  state = concentrator.startReceive();
  if (state != RADIOLIB_ERR_NONE) {
    printf("failed, code %d\n", state);
    return 1;
  }
  printf("success!\n");

  printf("[SX1301] Listening on all channels ...\n");

  for (;;) {
    int16_t count = concentrator.getPacketCount();
    if (count <= 0) {
      hal->delay(10);
      continue;
    }

    SX130xPacket_t pkt;
    state = concentrator.readPacket(&pkt);
    if (state != RADIOLIB_ERR_NONE) {
      printf("[SX1301] Read failed, code %d\n", state);
      continue;
    }

    const char* modemStr = (pkt.modem == 0) ? "LoRa multi-SF" : (pkt.modem == 1) ? "LoRa std" : "FSK";
    printf("[SX1301] Packet | modem=%-13s freq=%.3f MHz", modemStr, pkt.freq);

    if (pkt.modem != 2) {
      printf(" SF=%-2u BW=%.0f kHz", pkt.sf, pkt.bw);
    }

    printf(" RSSI=%.1f dBm", pkt.rssi);

    if (pkt.modem != 2) {
      printf(" SNR=%.1f dB", pkt.snr);
    }

    printf(" CRC=%s ts=%u len=%u | ", pkt.crcOk ? "OK" : "ERR", pkt.timestamp, pkt.len);

    for (uint8_t i = 0; i < pkt.len; i++) {
      printf("%02X ", pkt.payload[i]);
    }
    printf("\n");
  }

  return 0;
}
