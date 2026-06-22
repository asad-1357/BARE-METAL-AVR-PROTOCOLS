# Bare-Metal AVR Protocol Implementations

A collection of register-level firmware projects on the ATmega328P
(Arduino Uno), implementing the three core embedded communication
protocols — UART, I2C, and SPI — entirely from scratch using direct
register manipulation, without Arduino libraries (`Serial`, `Wire`, `SPI`).

Built to deepen low-level understanding of microcontroller peripherals:
clock configuration, interrupt handling, and hardware communication
state machines.

## Projects

| Project | Protocol | Description |
|---|---|---|
| [`uart-interrupt-counter`](./uart-interrupt-counter) | UART + External Interrupt | Interrupt-driven button counter transmitting live counts over serial |
| [`i2c-mpu6050`](./i2c-mpu6050) | I2C (TWI) | Reads live accelerometer data from an MPU6050 sensor |
| [`spi-oled-display`](./spi-oled-display) | SPI | Drives an SSD1306 OLED display via direct SPI register control |

## Why Bare-Metal?

Each project avoids high-level Arduino abstractions in favor of direct
register access (`DDRx`, `PORTx`, `UCSR0x`, `TWCR`, `SPCR`, etc.), the
same level at which production embedded firmware is typically written.
This demonstrates understanding of:
- Microcontroller peripheral configuration at the hardware level
- Protocol-specific state machines (START/STOP conditions, ACK/NACK,
  interrupt flags)
- Reading and applying datasheet register descriptions directly

## Toolchain
- AVR-GCC / avrdude
- Tested in TinkerCAD Circuits; portable to real Arduino Uno hardware

## Author
Asad Sajid — Final Year Electronics Engineering, NED University of
Engineering & Technology
