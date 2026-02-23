# RadioLib Semtech Modules - Skills and Capabilities

This document outlines the skills and capabilities required to work with Semtech radio modules in RadioLib.

## Overview

RadioLib supports a comprehensive range of Semtech radio modules, from legacy FSK/OOK transceivers to modern LoRa modules with advanced features like GNSS and WiFi scanning.

## Supported Semtech Modules

### 1. SX126x Series (SX1261, SX1262, SX1268)
**Modulation Modes:**
- LoRa (Spreading Factor 5-12, Bandwidth 62.5-500 kHz)
- FSK/GFSK (0.6-300 kbps)
- LR-FHSS (Long Range Frequency Hopping Spread Spectrum)

**Key Features:**
- Output power: -9 to +22 dBm
- Frequency range: 150-960 MHz
- Low power consumption with sleep modes
- Channel Activity Detection (CAD)
- Spectral scan capability
- TCXO support
- DC-DC or LDO regulator modes

**Skills Required:**
- Understanding of LoRa modulation parameters (SF, BW, CR)
- FSK configuration (bit rate, frequency deviation, receiver bandwidth)
- Power amplifier configuration and optimization
- Interrupt-driven communication
- Time-on-air calculations
- Image rejection calibration

### 2. SX127x Series (SX1272, SX1273, SX1276, SX1277, SX1278, SX1279)
**Modulation Modes:**
- LoRa (Spreading Factor 6-12, Bandwidth 7.8-500 kHz)
- FSK/OOK (0.6-300 kbps)

**Key Features:**
- Output power: -4 to +20 dBm (PA_BOOST), -1 to +14 dBm (RFO)
- Frequency range: 137-1020 MHz
- FHSS (Frequency Hopping Spread Spectrum) support
- Direct mode for digital modulation schemes
- Automatic Frequency Correction (AFC)
- Temperature sensor

**Skills Required:**
- Legacy LoRa compatibility
- PA selection and configuration (RFO vs PA_BOOST)
- FHSS implementation
- Direct mode operation for RTTY, Morse, etc.
- OOK threshold configuration
- DIO pin mapping and interrupt handling

### 3. SX128x Series (SX1280, SX1281, SX1282)
**Modulation Modes:**
- LoRa (Spreading Factor 5-12, Bandwidth 203-1625 kHz)
- GFSK (125-2000 kbps)
- FLRC (Fast Long Range Communication, 260-1300 kbps)
- BLE (Bluetooth Low Energy compatible)

**Key Features:**
- Frequency range: 2.4-2.5 GHz
- Output power: -18 to +13 dBm
- Ranging capability
- High data rate support
- Low latency

**Skills Required:**
- 2.4 GHz band operation
- FLRC modulation configuration
- BLE packet format handling
- Ranging protocol implementation
- High-speed data transmission

### 4. SX123x Series (SX1231, SX1233)
**Modulation Modes:**
- FSK (0.6-300 kbps)
- OOK

**Key Features:**
- Frequency range: 290-1020 MHz
- Output power: -18 to +20 dBm
- High sensitivity receiver
- Automatic modes and sequencer

**Skills Required:**
- FSK/OOK configuration
- Packet handler setup
- Address filtering
- Automatic mode sequencing

### 5. LLCC68
**Modulation Modes:**
- LoRa (Spreading Factor 5-11, Bandwidth 125-500 kHz)
- FSK (0.6-300 kbps)
- LR-FHSS

**Key Features:**
- Based on SX126x architecture
- Reduced spreading factor range (SF5-11)
- Limited bandwidth options (125, 250, 500 kHz)
- Lower cost alternative to SX1262

**Skills Required:**
- SX126x compatibility knowledge
- Understanding of LLCC68 limitations vs SX1262
- LR-FHSS configuration

### 6. LR11x0 Series (LR1110, LR1120, LR1121)
**Modulation Modes:**
- LoRa (Spreading Factor 5-12, Bandwidth 62.5-500 kHz sub-GHz, 203-812 kHz 2.4 GHz)
- GFSK (0.6-300 kbps)
- LR-FHSS

**Key Features:**
- Multi-band support: 150-960 MHz, 1.9-2.2 GHz, 2.4-2.5 GHz
- Integrated GNSS scanner (GPS, BeiDou)
- WiFi passive scanning (802.11b/g/n)
- Cryptographic engine
- Firmware update capability
- Output power: -9 to +22 dBm (sub-GHz), -18 to +13 dBm (2.4 GHz)

**Skills Required:**
- Multi-band operation and PA selection
- GNSS almanac management and position calculation
- WiFi scanning and MAC address extraction
- Cryptographic operations for LoRaWAN
- Firmware update procedures
- Advanced power management
- Image rejection calibration for multiple bands

### 7. LR2021
**Modulation Modes:**
- LoRa (Spreading Factor 5-12, Bandwidth 31.25-1000 kHz)
- GFSK (0.5-2000 kbps)
- OOK
- LR-FHSS
- FLRC (260-1300 kbps)
- OQPSK (O-QPSK for IEEE 802.15.4)
- BPSK (for Sigfox)

**Key Features:**
- Multi-band: 150-960 MHz, 1.9-2.2 GHz, 2.4-2.5 GHz
- Output power: -9 to +22 dBm (sub-GHz), -19 to +12 dBm (2.4 GHz)
- LoRa side detector (multi-SF reception)
- Advanced OOK detector for ADS-B
- Ranging capability
- Temperature and voltage monitoring

**Skills Required:**
- Multi-modem configuration
- LoRa side detector setup for multi-SF reception
- OOK detector configuration for ADS-B
- FLRC high-speed communication
- OQPSK for IEEE 802.15.4 compatibility
- Ranging protocol
- Advanced gain control

## Common Skills Across All Modules

### 1. Radio Fundamentals
- RF propagation and link budget calculations
- Antenna matching and impedance
- Frequency planning and regulatory compliance
- Modulation theory (FSK, LoRa, OOK, etc.)

### 2. LoRa Specific
- Spreading Factor vs data rate trade-offs
- Bandwidth selection for range/throughput
- Coding rate for error correction
- Adaptive Data Rate (ADR)
- LoRaWAN protocol (Class A, C, multicast)

### 3. Hardware Integration
- SPI communication and timing
- GPIO interrupt handling
- RF switch control
- TCXO vs crystal oscillator selection
- Power supply design (LDO vs DC-DC)

### 4. Software Development
- Interrupt-driven vs polling communication
- Buffer management
- Error handling and recovery
- Time-on-air optimization
- Power consumption optimization

### 5. Protocol Implementation
- AX.25 packet radio
- RTTY (Radioteletype)
- Morse code
- SSTV (Slow Scan Television)
- Hellschreiber
- APRS (Automatic Packet Reporting System)
- POCSAG pager protocol
- ADS-B (Automatic Dependent Surveillance-Broadcast)

### 6. Advanced Features
- Frequency Hopping Spread Spectrum (FHSS)
- Channel Activity Detection (CAD)
- Automatic Gain Control (AGC)
- Automatic Frequency Correction (AFC)
- CRC and whitening configuration
- Address filtering
- Packet length modes (fixed vs variable)

### 7. Testing and Debugging
- RSSI and SNR measurement
- Frequency error measurement
- Spectrum analysis
- Link quality assessment
- SPI debug logging
- Status code interpretation

## Module Selection Guide

| Requirement | Recommended Module |
|-------------|-------------------|
| Long range, low power, sub-GHz | SX126x, LLCC68 |
| Legacy LoRa compatibility | SX127x |
| 2.4 GHz operation | SX128x, LR2021 |
| High data rate | SX128x (FLRC), LR2021 (FLRC) |
| GNSS positioning | LR11x0 |
| WiFi geolocation | LR11x0 |
| Multi-band operation | LR11x0, LR2021 |
| ADS-B reception | LR2021 (OOK) |
| Cost-sensitive | LLCC68, SX127x |
| FSK/OOK only | SX123x |
| LoRaWAN with crypto | LR11x0 |
| Multi-SF reception | LR2021 (side detector) |

## Development Resources

- **API Reference**: https://jgromes.github.io/RadioLib
- **Wiki**: https://github.com/jgromes/RadioLib/wiki
- **Status Code Decoder**: https://radiolib-org.github.io/status_decoder/decode.html
- **Debug Log Decoder**: https://radiolib-org.github.io/debug_decoder/decode.html
- **Examples**: See `examples/` directory for each module

## Best Practices

1. **Always check return codes** - RadioLib methods return status codes that should be checked
2. **Calibrate image rejection** - Especially important when changing frequencies
3. **Configure TCXO properly** - Many modules require TCXO voltage configuration
4. **Use appropriate PA** - Select correct power amplifier for frequency band
5. **Implement proper error handling** - Handle timeouts and transmission failures
6. **Optimize power consumption** - Use sleep modes and duty cycling
7. **Test thoroughly** - Verify range, data rate, and reliability in target environment
8. **Follow regulatory limits** - Respect duty cycle and power limits for your region

## Common Pitfalls

1. **Incorrect TCXO configuration** - Error codes -706/-707 often indicate TCXO issues
2. **Wrong PA selection** - Using sub-GHz PA for 2.4 GHz or vice versa
3. **Insufficient calibration** - Not calibrating after frequency changes
4. **Interrupt conflicts** - Multiple modules sharing same interrupt pin
5. **Buffer overruns** - Not reading received data quickly enough
6. **Time-on-air violations** - Exceeding regulatory duty cycle limits
7. **Impedance mismatch** - Poor antenna matching causing reduced range
8. **Power supply issues** - Insufficient current capability during transmission

## Conclusion

Working with Semtech modules in RadioLib requires understanding of both RF fundamentals and specific module capabilities. Start with basic LoRa or FSK communication, then progress to advanced features like GNSS, WiFi scanning, or multi-modem operation as needed.

---

## SX130x Series (SX1301, SX1302) - LoRaWAN Concentrators

The SX130x series are **LoRaWAN gateway concentrators**, fundamentally different from single-channel transceivers. They are designed to receive from many end-nodes simultaneously.

**Modulation Modes:**
- LoRa multi-SF (8 channels, all spreading factors simultaneously)
- LoRa single-SF (1 dedicated channel, configurable SF/BW)
- FSK (1 channel)

**Key Features:**
- 8 simultaneous LoRa multi-SF receive channels
- Frequency range: 430-928 MHz
- Output power: -6 to +27 dBm (SX1301), -9 to +27 dBm (SX1302)
- Internal timestamp counter for time-stamped reception
- Dual radio chain (radio A + radio B) for wide channel coverage
- SX1302: lower power, improved sensitivity, higher-resolution timestamp

**Gateway-Specific Skills Required:**
- Multi-channel configuration (8 LoRa + 1 std + 1 FSK)
- Dual radio chain frequency planning
- IF frequency offset calculation per channel
- LoRaWAN regional channel plans (EU868, US915, AU915, etc.)
- Downlink scheduling with timestamps
- FIFO packet management
- Gateway-to-network-server protocol (Semtech UDP packet forwarder)

**Channel Configuration Example (EU868):**
```cpp
SX1302 gw(new Module(CS, IRQ, RST, BUSY));
gw.begin(867.5, 868.5);  // radio A = 867.5 MHz, radio B = 868.5 MHz

// EU868 radio assignments: channels 0-4 on radio A, 5-7 on radio B
const uint8_t radio[] = {0, 0, 0, 0, 0, 1, 1, 1};
gw.setLoRaChannels(867.1, 0.2, radio);  // 867.1, 867.3, ..., 868.5 MHz
```
