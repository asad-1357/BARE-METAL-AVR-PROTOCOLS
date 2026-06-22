# Interrupt + UART Button Counter (Bare-Metal AVR)

Register-level firmware for ATmega328P (Arduino Uno) demonstrating external
interrupt handling and UART communication without Arduino libraries.

## Overview
Counts button presses using a hardware interrupt (INT0) and transmits the
live count over UART serial — built entirely with direct AVR register
manipulation (no `digitalRead()`, `attachInterrupt()`, or `Serial.print()`).

## Features
- External interrupt (INT0) configured for falling-edge detection
- Software debouncing
- UART transmission at 9600 baud via direct UDR0/UCSR0 register access
- ISR-to-main communication using a `volatile` flag pattern
- LED toggle feedback on each valid press

## Hardware
- Arduino Uno (ATmega328P)
- Pushbutton → D2 (INT0), internal pull-up enabled
- LED → D13 (optional, onboard LED works too)

## Registers Used
| Register | Purpose |
|---|---|
| `DDRD`, `PORTD` | Configure PD2 as input with pull-up |
| `EICRA`, `EIMSK` | Configure and enable INT0 (falling edge) |
| `UBRR0`, `UCSR0B`, `UCSR0C` | UART initialization (9600 baud, 8N1) |
| `UDR0`, `UCSR0A` | UART data transmission |
| `DDRB`, `PORTB` | LED output and toggle |

## How It Works
1. Button press triggers a falling edge on PD2 → fires `ISR(INT0_vect)`
2. ISR debounces (50ms), confirms press, increments counter, sets a flag
3. Main loop detects the flag, toggles LED, and transmits the count over UART
4. Heavy work (UART) is kept out of the ISR — standard embedded practice

## Build & Run
Tested in TinkerCAD Circuits (AVR-GCC toolchain). To run locally with a real
Arduino Uno:
```
avr-gcc -mmcu=atmega328p -DF_CPU=16000000UL -Os -o main.elf src/main.c
avr-objcopy -O ihex main.elf main.hex
avrdude -c arduino -p atmega328p -P <port> -U flash:w:main.hex
```

## Demo
Serial Monitor output (9600 baud):
```
System Ready. Press button...
Button pressed! Count = 1
Button pressed! Count = 2
```

## Author
Asad Sajid — Final Year Electronics Engineering, NED University of
Engineering & Technology