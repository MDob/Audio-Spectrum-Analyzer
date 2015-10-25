# Audio Spectrum Analyzer

This repository contains code for a SAMD21 based board

**Board Features:**
- SAMD21E17A MCU
	- 30MHz & 32.768kHz external crystals
- Microphone
- 3.5mm audio jack
- USB via FTDI
- Bluetooth
- 5V level shifted general SERCOM output
- Two-axis accelerometer

## Modules In Progress

The following modules are currently in progress.

### USART
Connection to a computer and basic text echoing has been achieved.

### SPI
Accurate transmission of data and basic control of WS2812B has been achieved.

### DMA
An application specific DMA module has been implemented.

### RGB LED Control
Basic control of WS2812B LEDs has been accomplished.

### ADC
Reading microphone and audio jack voltage has been achieved.

## Completed Modules
None.

## Unstarted Modules

This project is still in its infancy so there are several modules that have not been started yet. These include the following.

### Bluetooth Parser
Parses Bluetooth packets into a format usable by the main packet parser.

### Main Packet Parser
Used to parse packets received over FTDI or from the Bluetooth packet parser. This module may be turned into an FTDI-specific module for efficiency.

### Packet Former
Creates packets for transmission over either FTDI or Bluetooth.

### FFT
Performs fast Fourier transforms on audio data.