#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>

#define DDR_SPI DDRB
#define DC_PIN   PB1   // D9
#define RES_PIN  PB0   // D8
#define CS_PIN   PB2   // D10
#define MOSI_PIN PB3   // D11
#define SCK_PIN  PB5   // D13

// ================= SPI =================

void SPI_init(void) {
    DDR_SPI |= (1 << MOSI_PIN) | (1 << SCK_PIN) | (1 << CS_PIN) | (1 << DC_PIN) | (1 << RES_PIN);
    SPCR = (1 << SPE) | (1 << MSTR); // SPI enable, Master mode, clock = F_CPU/4
    PORTB |= (1 << CS_PIN);          // CS idle HIGH (not selected)
}

void SPI_transmit(uint8_t data) {
    SPDR = data;
    while (!(SPSR & (1 << SPIF)));   // wait until transfer complete
}

// ================= SSD1306 control =================

void OLED_command(uint8_t cmd) {
    PORTB &= ~(1 << CS_PIN);   // select display
    PORTB &= ~(1 << DC_PIN);   // DC=0 -> this byte is a COMMAND
    SPI_transmit(cmd);
    PORTB |= (1 << CS_PIN);    // deselect
}

void OLED_data(uint8_t data) {
    PORTB &= ~(1 << CS_PIN);
    PORTB |= (1 << DC_PIN);    // DC=1 -> this byte is DATA
    SPI_transmit(data);
    PORTB |= (1 << CS_PIN);
}

void OLED_reset(void) {
    PORTB |= (1 << RES_PIN);
    _delay_ms(10);
    PORTB &= ~(1 << RES_PIN);
    _delay_ms(10);
    PORTB |= (1 << RES_PIN);
    _delay_ms(10);
}

void OLED_init(void) {
    OLED_reset();

    OLED_command(0xAE); // display OFF
    OLED_command(0xD5); OLED_command(0x80); // clock divide
    OLED_command(0xA8); OLED_command(0x3F); // multiplex ratio (64 rows)
    OLED_command(0xD3); OLED_command(0x00); // display offset
    OLED_command(0x40); // start line = 0
    OLED_command(0x8D); OLED_command(0x14); // charge pump enable
    OLED_command(0x20); OLED_command(0x00); // memory addressing mode = horizontal
    OLED_command(0xA1); // segment remap
    OLED_command(0xC8); // COM scan direction
    OLED_command(0xDA); OLED_command(0x12); // COM pins config
    OLED_command(0x81); OLED_command(0xCF); // contrast
    OLED_command(0xD9); OLED_command(0xF1); // pre-charge period
    OLED_command(0xDB); OLED_command(0x40); // VCOMH deselect level
    OLED_command(0xA4); // resume to RAM content display
    OLED_command(0xA6); // normal display (not inverted)
    OLED_command(0xAF); // display ON
}

void OLED_fillScreen(uint8_t pattern) {
    for (uint8_t page = 0; page < 8; page++) {
        OLED_command(0xB0 + page); // set page address
        OLED_command(0x00);         // set lower column = 0
        OLED_command(0x10);         // set higher column = 0

        for (uint8_t col = 0; col < 128; col++) {
            OLED_data(pattern);
        }
    }
}

// ================= Main =================

int main(void) {
    SPI_init();
    OLED_init();

    OLED_fillScreen(0x00); // clear screen first
    _delay_ms(1000);

    while (1) {
        OLED_fillScreen(0xFF); // all pixels ON
        _delay_ms(500);
        OLED_fillScreen(0x00); // all pixels OFF
        _delay_ms(500);
    }
}