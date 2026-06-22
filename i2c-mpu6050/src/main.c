#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>

#define MPU6050_ADDR 0x68

// ================= UART (for printing results) =================
void UART_init(void) {
    uint16_t ubrr = 103;
    UBRR0H = (uint8_t)(ubrr >> 8);
    UBRR0L = (uint8_t)ubrr;
    UCSR0B = (1 << TXEN0);
    UCSR0C = (1 << UCSZ01) | (1 << UCSZ00);
}

void UART_transmit(char data) {
    while (!(UCSR0A & (1 << UDRE0)));
    UDR0 = data;
}

void UART_print(const char *str) {
    while (*str) UART_transmit(*str++);
}

void UART_printNumber(int16_t num) {
    char buffer[7];
    uint8_t i = 0;

    if (num < 0) {
        UART_transmit('-');
        num = -num;
    }
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

// ================= TWI / I2C =================

void TWI_init(void) {
    TWBR = 72;          // 100kHz SCL with 16MHz F_CPU, prescaler=1
    TWSR = 0x00;         // prescaler bits = 00 (prescaler = 1)
}

void TWI_start(void) {
    TWCR = (1 << TWSTA) | (1 << TWEN) | (1 << TWINT);
    while (!(TWCR & (1 << TWINT)));
}

void TWI_stop(void) {
    TWCR = (1 << TWSTO) | (1 << TWEN) | (1 << TWINT);
    _delay_us(10); // allow stop condition to complete (no TWINT flag for stop)
}

void TWI_write(uint8_t data) {
    TWDR = data;
    TWCR = (1 << TWEN) | (1 << TWINT);
    while (!(TWCR & (1 << TWINT)));
}

uint8_t TWI_readACK(void) {
    TWCR = (1 << TWEN) | (1 << TWINT) | (1 << TWEA); // TWEA=1 -> send ACK after read
    while (!(TWCR & (1 << TWINT)));
    return TWDR;
}

uint8_t TWI_readNACK(void) {
    TWCR = (1 << TWEN) | (1 << TWINT); // TWEA=0 -> send NACK after read (last byte)
    while (!(TWCR & (1 << TWINT)));
    return TWDR;
}

// ================= MPU6050 specific =================

void MPU6050_init(void) {
    TWI_start();
    TWI_write(MPU6050_ADDR << 1);   // address + WRITE
    TWI_write(0x6B);                 // PWR_MGMT_1 register
    TWI_write(0x00);                 // wake up the sensor (clear sleep bit)
    TWI_stop();
}

void MPU6050_readAccel(int16_t *ax, int16_t *ay, int16_t *az) {
    uint8_t data[6];

    TWI_start();
    TWI_write(MPU6050_ADDR << 1);    // address + WRITE
    TWI_write(0x3B);                  // ACCEL_XOUT_H register (start of accel data)

    TWI_start();                      // repeated START
    TWI_write((MPU6050_ADDR << 1) | 1); // address + READ

    data[0] = TWI_readACK();   // ACCEL_XOUT_H
    data[1] = TWI_readACK();   // ACCEL_XOUT_L
    data[2] = TWI_readACK();   // ACCEL_YOUT_H
    data[3] = TWI_readACK();   // ACCEL_YOUT_L
    data[4] = TWI_readACK();   // ACCEL_ZOUT_H
    data[5] = TWI_readNACK();  // ACCEL_ZOUT_L (last byte -> NACK)

    TWI_stop();

    *ax = (data[0] << 8) | data[1];
    *ay = (data[2] << 8) | data[3];
    *az = (data[4] << 8) | data[5];
}

// ================= Main =================

int main(void) {
    int16_t ax, ay, az;

    UART_init();
    TWI_init();
    MPU6050_init();

    UART_print("MPU6050 Ready\r\n");

    while (1) {
        MPU6050_readAccel(&ax, &ay, &az);

        UART_print("AX: ");
        UART_printNumber(ax);
        UART_print("  AY: ");
        UART_printNumber(ay);
        UART_print("  AZ: ");
        UART_printNumber(az);
        UART_print("\r\n");

        _delay_ms(500);
    }
}