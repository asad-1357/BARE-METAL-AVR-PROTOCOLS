# SPI SSD1306 OLED Display Driver (Bare-Metal AVR)

Register-level firmware for ATmega328P (Arduino Uno) demonstrating SPI
master communication with an SSD1306 OLED display, without using the
Arduino `SPI.h` library.

## Overview
Initializes an SSD1306 128x64 OLED display over SPI and drives the full
screen buffer directly via raw AVR SPI registers — no abstraction
libraries used.

## Features
- SPI master implementation using raw `SPCR`/`SPSR`/`SPDR` registers
- Manual Chip Select (CS) and Data/Command (DC) line control
- Full SSD1306 initialization sequence (charge pump, addressing mode,
  contrast, COM configuration)
- Full-screen fill routine demonstrating page/column addressing

## Hardware
- Arduino Uno (ATmega328P)
- SSD1306 128x64 SPI OLED display
- Wiring: SCK → D13, MOSI → D11, CS → D10, DC → D9, RES → D8

## Registers Used
| Register | Purpose |
|---|---|
| `SPCR` | Enable SPI, set Master mode and clock rate |
| `SPSR` | Poll `SPIF` flag to detect transfer completion |
| `SPDR` | Byte being transmitted over MOSI |
| `DDRB`, `PORTB` | Control CS, DC, and RES lines manually |

## How It Works
1. `SPI_init()` configures SPI pins and enables master mode
2. `OLED_reset()` pulses the RES line to hardware-reset the display
3. `OLED_init()` sends the SSD1306's required ~15-command setup sequence
4. `OLED_command()` / `OLED_data()` toggle the DC line to distinguish
   commands from pixel data, with CS pulled low during each transfer
5. `OLED_fillScreen()` writes a full 128x64 pixel pattern across all 8
   display pages

## Build & Run
Tested in TinkerCAD Circuits (AVR-GCC toolchain). To run locally:
```
avr-gcc -mmcu=atmega328p -DF_CPU=16000000UL -Os -o main.elf src/main.c
avr-objcopy -O ihex main.elf main.hex
avrdude -c arduino -p atmega328p -P <port> -U flash:w:main.hex
```

## Demo
Display alternates between fully lit and fully cleared every 500ms,
confirming successful SPI command/data transfer and page addressing.

## Author
Asad Sajid — Final Year Electronics Engineering, NED University of
Engineering & Technology