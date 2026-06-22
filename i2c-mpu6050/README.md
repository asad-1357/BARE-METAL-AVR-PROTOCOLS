# I2C MPU6050 Accelerometer Reader (Bare-Metal AVR)

Register-level firmware for ATmega328P (Arduino Uno) demonstrating I2C
(TWI) master communication with an MPU6050 accelerometer/gyroscope,
without using the Arduino `Wire` library.

## Overview
Initializes the MPU6050 over I2C, wakes it from sleep mode, then
continuously reads raw X/Y/Z accelerometer values and streams them over
UART — all using direct AVR TWI register manipulation.

## Features
- I2C master implementation using raw TWI registers (no `Wire.h`)
- START / repeated START / STOP condition handling
- 100kHz I2C clock configuration via `TWBR`
- Multi-byte sequential read with ACK/NACK control
- Results streamed over UART (9600 baud) for live monitoring

## Hardware
- Arduino Uno (ATmega328P)
- MPU6050 accelerometer/gyroscope module
- Wiring: SCL → A5, SDA → A4, VCC → 5V, GND → GND

## Registers Used
| Register | Purpose |
|---|---|
| `TWBR`, `TWSR` | Set I2C clock speed (100kHz) |
| `TWCR` | Issue START, STOP, and transfer commands |
| `TWDR` | Holds address/data byte being sent or received |

## How It Works
1. `TWI_init()` configures the bus speed
2. `MPU6050_init()` sends a START, addresses the device, and writes `0x00`
   to the power management register to wake it up
3. `MPU6050_readAccel()` performs a write (pointing to the accelerometer
   data register) followed by a repeated START and a 6-byte burst read
4. Each pair of bytes is combined into a 16-bit signed accelerometer value
5. Values are printed over UART every 500ms

## Build & Run
Tested in TinkerCAD Circuits (AVR-GCC toolchain). To run locally:
```
avr-gcc -mmcu=atmega328p -DF_CPU=16000000UL -Os -o main.elf src/main.c
avr-objcopy -O ihex main.elf main.hex
avrdude -c arduino -p atmega328p -P <port> -U flash:w:main.hex
```

## Demo
Serial Monitor output (9600 baud):
```
MPU6050 Ready
AX: 120  AY: -340  AZ: 16384
AX: 118  AY: -335  AZ: 16390
```

## Author
Asad Sajid — Final Year Electronics Engineering, NED University of
Engineering & Technology