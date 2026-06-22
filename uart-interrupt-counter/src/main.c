#define F_CPU 16000000UL
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

volatile uint8_t buttonCount = 0;
volatile uint8_t flagSend = 0;

// ---- UART Init (9600 baud) ----
void UART_init(void) {
    uint16_t ubrr = 103; // 16,000,000 / (16*9600) - 1 = 103
    UBRR0H = (uint8_t)(ubrr >> 8);
    UBRR0L = (uint8_t)ubrr;
    UCSR0B = (1 << TXEN0);              // Enable transmitter
    UCSR0C = (1 << UCSZ01) | (1 << UCSZ00); // 8-bit data, no parity, 1 stop bit
}

// ---- Send single char ----
void UART_transmit(char data) {
    while (!(UCSR0A & (1 << UDRE0)));  // wait until buffer empty
    UDR0 = data;
}

// ---- Send string ----
void UART_print(const char *str) {
    while (*str) {
        UART_transmit(*str++);
    }
}

// ---- Send number (0-255) ----
void UART_printNumber(uint8_t num) {
    char buffer[4];
    uint8_t i = 0;

    if (num == 0) {
        UART_transmit('0');
        return;
    }

    while (num > 0) {
        buffer[i++] = (num % 10) + '0';
        num /= 10;
    }

    while (i > 0) {
        UART_transmit(buffer[--i]);
    }
}

// ---- INT0 Init (Button on D2) ----
void INT0_init(void) {
    DDRD &= ~(1 << PD2);   // PD2 as input
    PORTD |= (1 << PD2);   // Enable pull-up resistor

    EICRA |= (1 << ISC01); // Falling edge triggers interrupt
    EICRA &= ~(1 << ISC00);

    EIMSK |= (1 << INT0);  // Enable INT0
}

// ---- ISR for INT0 ----
ISR(INT0_vect) {
    _delay_ms(50); // simple debounce

    if (!(PIND & (1 << PD2))) { // confirm button still pressed
        buttonCount++;
        flagSend = 1;
    }
}

// ---- LED Init (PB5 / pin 13) ----
void LED_init(void) {
    DDRB |= (1 << PB5);
}

int main(void) {
    UART_init();
    INT0_init();
    LED_init();

    sei(); // enable global interrupts

    UART_print("System Ready. Press button...\r\n");

    while (1) {
        if (flagSend) {
            flagSend = 0;

            // Toggle LED to show press registered
            PORTB ^= (1 << PB5);

            UART_print("Button pressed! Count = ");
            UART_printNumber(buttonCount);
            UART_print("\r\n");
        }
    }
}