#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>

#define DDR_SPI   DDRB
#define CS_PIN    PB2   // Digital 10 (Latch Pin)
#define MOSI_PIN  PB3   // Digital 11 (Data Pin)
#define SCK_PIN   PB5   // Digital 13 (Clock Pin)

void SPI_init(void) {
    // Set MOSI, SCK, and CS as output
    DDR_SPI |= (1 << MOSI_PIN) | (1 << SCK_PIN) | (1 << CS_PIN);
    
    // Enable SPI, set as Master, clock rate = F_CPU/16
    SPCR = (1 << SPE) | (1 << MSTR) | (1 << SPR0);
    
    // Set CS pin High (Idle status)
    PORTB |= (1 << CS_PIN);
}

void SPI_transmit(uint8_t data) {
    PORTB &= ~(1 << CS_PIN); // Pull CS Low to select the chip
    
    SPDR = data;             // Load data into transmission register
    while (!(SPSR & (1 << SPIF))); // Wait until transmission is complete
    
    PORTB |= (1 << CS_PIN);  // Pull CS High to latch data out
}

int main(void) {
    SPI_init();
    uint8_t count = 0;

    while (1) {
        SPI_transmit(count++); // Send data via SPI to shift register
        _delay_ms(200);        // Increment binary count every 200ms
    }
}
