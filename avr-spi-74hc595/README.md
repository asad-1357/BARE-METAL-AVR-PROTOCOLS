# 8-Bit Binary Counter using Bare-Metal AVR SPI & 74HC595

This repository contains a bare-metal AVR C implementation of an 8-bit binary counter running on an ATmega328P, driving a **74HC595 Shift Register** over the hardware SPI bus. 

## 🛠️ Hardware Wiring & Logic Analyzer Mapping

To verify the synchronous SPI timings and binary outputs, an 8-channel logic analyzer was configured in the Wokwi simulation environment as follows:

| Analyzer Channel | Target Pin / Microcontroller | Description / Function |
| :--- | :--- | :--- |
| **D0** | Arduino Pin 13 (SCK / PB5) | **SHCP (Shift Clock):** Generates 8 pulses per burst |
| **D1** | Arduino Pin 11 (MOSI / PB3) | **DS (Data Serial):** Transmits binary bits (MSB first) |
| **D2** | Arduino Pin 10 (SS / CS / PB2) | **STCP (Storage/Latch Clock):** Updates output pins instantly on RISING edge |
| **D3** | 74HC595 Pin 15 ($Q_0$) | LSB (Least Significant Bit) - Toggles every 200ms |
| **D4** | 74HC595 Pin 1 ($Q_1$) | Bit 1 - Toggles every 400ms |
| **D5** | 74HC595 Pin 2 ($Q_2$) | Bit 2 - Toggles every 800ms |
| **D6** | 74HC595 Pin 3 ($Q_3$) | Bit 3 - Toggles every 1.6s |
| **D7** | 74HC595 Pin 7 ($Q_7$) | MSB (Most Significant Bit) - Toggles every 25.6s |

<img width="650" height="567" alt="image" src="https://github.com/user-attachments/assets/04bc51fa-81b2-4472-92c3-6a0bbc14bd26" />


## 📈 Waveform Analysis & Observations

By exporting the simulation trace as a `.vcd` (Value Change Dump) file and loading it into a waveform viewer (`vcdrom.github.io`), we observe the exact clock-to-data behavior:

<img width="983" height="616" alt="image" src="https://github.com/user-attachments/assets/6409022a-ccf1-4c89-be7c-85f5f5129c46" />

<img width="983" height="616" alt="image" src="https://github.com/user-attachments/assets/178c3ef5-a05b-435d-8a44-954983df979d" />

<img width="983" height="616" alt="image" src="https://github.com/user-attachments/assets/7b5f32cf-4225-4dc2-a39f-43c1f76c15b6" />

<img width="983" height="616" alt="image" src="https://github.com/user-attachments/assets/a63c138a-5554-4f4c-8907-c8962a08cd4b" />








### 1. SPI Protocol Verification (D0, D1, D2)
* **Clock Idle State:** The SPI peripheral is configured in **Mode 0** ($\text{CPOL}=0$, $\text{CPHA}=0$). The clock line (`D0`) remains stable **LOW** during idle periods and bursts into exactly 8 clean square wave pulses only during active transmissions.
* **Data Serial Stream (`D1`):** Represents the serialized byte value. For instance:
  * At $200\text{ms}$ (`Count = 1` $\rightarrow$ `00000001`), `D1` stays LOW for the first 7 clock pulses and transitions **HIGH** exactly on the 8th pulse.
  * At $1000\text{ms}$ (`Count = 5` $\rightarrow$ `00000101`), `D1` shows clear alternating pulses reflecting the bit sequence.

### 2. Synchronous Latch & Binary Outputs
* Unlike asynchronous ripple counters where stage changes trigger successive pins sequentially, the 74HC595 updates its output lines **simultaneously**.
* The exact microsecond the Latch line (`D2`) experiences a rising edge, the parallel outputs (`D3` through `D7`) shift states instantly in a perfectly aligned vertical transition vector.
