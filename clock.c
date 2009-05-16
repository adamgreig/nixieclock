// NIXIE Clock
// Adam Greig, April 2009

//============================================================================
// Includes
//----------------------------------------------
#include <stdlib.h>
#include <avr/interrupt.h>
#include <avr/io.h>
#include <util/delay.h>

//============================================================================
// Constants
//----------------------------------------------

#define SHDN PC0
#define SDA PC4
#define SCL PC5

#define SCK PD5
#define RCK PD6
#define SER PD7

#define C1 PD0
#define C2 PD1
#define C3 PD2
#define C4 PD3

#define R1 PB0
#define R2 PB1

#define DS1307 0x68

//============================================================================
// Globals
//----------------------------------------------

unsigned short int sec_t, sec_u, min_t, min_u, hour_t, hour_u, day;
unsigned short int date_t, date_u, month_t, month_u, year_t, year_u;

//============================================================================
//============================================================================
// Input Functions
//----------------------------------------------

//Scan the input matrix
void scan_input() {
    
    char op = 0x00;
    char arg = 0x00;
    
    //Row 1
    PORTB = (0<<R1) | (1<<R2);
    _delay_ms(5);
    
    if((PIND & (1<<C4)) == 0)
        op = '+';
    
    if((PIND & (1<<C3)) == 0)
        arg = 'Y';
    
    if((PIND & (1<<C2)) == 0)
        arg = 'M';
    
    if((PIND & (1<<C1)) == 0)
        arg = 'D';
    
    //Row 2
    PORTB = (1<<R1) | (0<<R2);
    _delay_ms(5);
    
    if((PIND & (1<<C4)) == 0)
        op = '-';
    
    if((PIND & (1<<C3)) == 0)
        arg = 's';
    
    if((PIND & (1<<C2)) == 0)
        arg = 'm';
    
    if((PIND & (1<<C1)) == 0)
        arg = 'h';
    
    //Finish
    PORTB = (1<<R1) | (1<<R2);
    
    //Apply
    switch(arg) {
        case 'Y':
            if(op == '+') {
                if(year_u == 9) {
                    year_u = 0;
                    year_t = (year_t + 1) % 10;
                } else {
                    year_u++;
                }
            } else if(op == '-') {
                if(year_u == 0) {
                    year_u = 9;
                    year_t = (year_t - 1) % 10;
                } else {
                    year_u--;
                }
            }
            break;
        case 'M':
            if(op == '+') {
                if(month_u == 9) {
                    month_u = 0;
                    month_t = (month_t + 1) % 2;
                } else {
                    month_u++;
                }
            } else if(op == '-') {
                if(month_u == 0) {
                    month_u = 9;
                    month_t = (month_t - 1) % 2;
                } else {
                    month_u--;
                }
            }
            break;
        case 'D':
            if(op == '+') {
                day = (day + 1) % 7;
                if(date_u == 9) {
                    date_u = 0;
                    date_t = (date_t + 1) % 4;
                } else {
                    date_u++;
                }
            } else if(op == '-') {
                day = (day - 1) % 7;
                if(date_u == 0) {
                    date_u = 9;
                    date_t = (date_t - 1) % 4;
                } else {
                    date_u--;
                }
            }
            break;
        case 'h':
            if(op == '+') {
                if(hour_u == 9) {
                    hour_u = 0;
                    hour_t = (hour_t + 1) % 3;
                } else {
                    hour_u++;
                }
            } else if(op == '-') {
                if(hour_u == 0) {
                    hour_u = 9;
                    hour_t = (hour_t - 1) % 3;
                } else {
                    hour_u--;
                }
            }
            break;
        case 'm':
            if(op == '+') {
                if(min_u == 9) {
                    min_u = 0;
                    min_t = (min_t + 1) % 6;
                } else {
                    min_u++;
                }
            } else if(op == '-') {
                if(min_u == 0) {
                    min_u = 9;
                    min_t = (min_t - 1) % 6;
                } else {
                    min_u--;
                }
            }
            break;
        case 's':
            if(op == '+') {
                if(sec_u == 9) {
                    sec_u = 0;
                    sec_t = (sec_t + 1) % 6;
                } else {
                    sec_u++;
                }
            } else if(op == '-') {
                if(sec_u == 0) {
                    sec_u = 9;
                    sec_t = (sec_t - 1) % 6;
                } else {
                    sec_u--;
                }
            }
            break;
    }
    
    if(op == '+' || op == '-')
        rtc_set();

}

void rtc_set() {
    TWCR = (1<<TWINT) | (1<<TWEN) | (1<<TWSTA);
    while(!(TWCR & (1<<TWINT)));
    TWDR = DS1307 << 1 | 0;
    TWCR = (1<<TWINT) | (1<<TWEN);
    while(!(TWCR & (1<<TWINT)));
    
    TWDR = 0x00;
    TWCR = (1<<TWINT) | (1<<TWEN);
    while(!(TWCR & (1<<TWINT)));
    
    TWDR = (0<<7) | (sec_t << 4) | (sec_u);
    TWCR = (1<<TWINT) | (1<<TWEN);
    while(!(TWCR & (1<<TWINT)));
    
    TWDR = (min_t << 4) | (min_u);
    TWCR = (1<<TWINT) | (1<<TWEN);
    while(!(TWCR & (1<<TWINT)));
    
    TWDR = (0<<6) | (hour_t << 4) | (hour_u);
    TWCR = (1<<TWINT) | (1<<TWEN);
    while(!(TWCR & (1<<TWINT)));
    
    TWDR = (day);
    TWCR = (1<<TWINT) | (1<<TWEN);
    while(!(TWCR & (1<<TWINT)));
    
    TWDR = (date_t << 4) | (date_u);
    TWCR = (1<<TWINT) | (1<<TWEN);
    while(!(TWCR & (1<<TWINT)));
    
    TWDR = (month_t << 4) | (month_u);
    TWCR = (1<<TWINT) | (1<<TWEN);
    while(!(TWCR & (1<<TWINT)));
    
    TWDR = (year_t << 4) | (year_u);
    TWCR = (1<<TWINT) | (1<<TWEN);
    while(!(TWCR & (1<<TWINT)));
    
    TWCR = (1<<TWINT) | (1<<TWEN) | (1<<TWSTO);
}

void rtc_read() {
    
    TWCR = (1<<TWINT) | (1<<TWEN) | (1<<TWSTA);
    while(!(TWCR & (1<<TWINT)));
    TWDR = DS1307 << 1 | 0;
    TWCR = (1<<TWINT) | (1<<TWEN);
    while(!(TWCR & (1<<TWINT)));
    
    TWDR = 0x00;
    TWCR = (1<<TWINT) | (1<<TWEN);
    while(!(TWCR & (1<<TWINT)));
    
    TWCR = (1<<TWINT) | (1<<TWEN) | (1<<TWSTA);
    while(!(TWCR & (1<<TWINT)));
    TWDR = DS1307 << 1 | 1;
    TWCR = (1<<TWINT) | (1<<TWEN);
    while(!(TWCR & (1<<TWINT)));
    
    unsigned short int i;
    for(i=0; i<7; i++) {
        if(i == 5)
            TWCR = (1<<TWINT) | (1<<TWEN) | (0<<TWEA);
        else
            TWCR = (1<<TWINT) | (1<<TWEN) | (1<<TWEA);
        
        while(!(TWCR & (1<<TWINT)));
        char byte = TWDR;
        
        switch(i) {
            case 0:
                sec_t = (byte & 0x70) >> 4;
                sec_u = (byte & 0x0F);
                break;
            case 1:
                min_t = (byte & 0xF0) >> 4;
                min_u = (byte & 0x0F);
                break;
            case 2:
                hour_t = (byte & 0x30) >> 4;
                hour_u = (byte & 0x0F);
                break;
            case 3:
                day    = (byte & 0x07);
                break;
            case 4:
                date_t = (byte & 0xF0) >> 4;
                date_u = (byte & 0x0F);
                break;
            case 5:
                month_t = (byte & 0xF0) >> 4;
                month_u = (byte & 0x0F);
                break;
            case 6:
                year_t = (byte & 0xF0) >> 4;
                year_u = (byte & 0x0F);
                break;
        }
        
    }
    
    TWCR = (1<<TWINT) | (1<<TWEN) | (1<<TWSTO);
}

//============================================================================
// Output Functions
//----------------------------------------------

//Disable interrupts and set the latch pin low
void start_data() {
    EIMSK = (0<<INT1);
    PORTD = (0<<C1) | (0<<C2)   | (0<<C3)   | (0<<C4)   | (0<<RCK);
}

//Set latch pin high and re-enable interrupts
void end_data() {
    PORTD = (1<<C1) | (1<<C2)   | (1<<C3)   | (1<<C4)   | (1<<RCK);
    EIMSK = (0<<INT1);
}

//Shift out one given digit, correcting for mirrored nixie
void shift_num(char num) {
    num = (11 - num) % 10;

    PORTD = (0<<SCK) | (0<<SER);
    if(num & 8)
        PORTD |= (1<<SER);
    PORTD |= (1<<SCK);
    
    PORTD = (0<<SCK) | (0<<SER);
    if(num & 4)
        PORTD |= (1<<SER);
    PORTD |= (1<<SCK);
    
    PORTD = (0<<SCK) | (0<<SER);
    if(num & 2)
        PORTD |= (1<<SER);
    PORTD |= (1<<SCK);
    
    PORTD = (0<<SCK) | (0<<SER);
    if(num & 1)
        PORTD |= (1<<SER);
    PORTD |= (1<<SCK);
    
}


void out_sec() {
    shift_num(sec_u);
    shift_num(sec_t);
}

void out_min() {
    shift_num(min_u);
    shift_num(min_t);
}

void out_hour() {
    shift_num(hour_u);
    shift_num(hour_t);
}

void out_time() {
    start_data();
    out_sec();
    out_min();
    out_hour();
    end_data();
}

//============================================================================
//============================================================================
// Initialisation
//----------------------------------------------

void setup() {

    DDRB  = (1<<R1)     | (1<<R2);
    DDRC  = (1<<SHDN)   | (0<<SDA)  | (0<<SCL);
    DDRD  = (0<<C1)     | (0<<C2)   | (0<<C3)   | (0<<C4);
    DDRD |= (1<<RCK)    | (1<<SCK)  | (1<<SER);
    
    PORTB = (1<<R1)     | (1<<R2);
    PORTC = (0<<SHDN)   | (1<<SDA)  | (1<<SCL);
    PORTD = (1<<C1)     | (1<<C2)   | (1<<C3)   | (1<<C4)   | (1<<RCK);
    
    EICRA = (0<<ISC11)  | (0<<ISC10);
    
    TWBR  = (1<<1);
    TWCR  = (1<<TWEN);
    TWSR  = (1<<TWPS0)  | (1<<TWPS1);
    
}

//============================================================================
// Main Loop
//----------------------------------------------

void loop() {
    scan_input();
    rtc_read();
    out_time();
    _delay_ms(400);
}

//============================================================================
//============================================================================
// Interrupt Handlers
//----------------------------------------------

ISR(INT1_vect) {
    
}

//============================================================================
//============================================================================
// Code entry point
//----------------------------------------------

int main(void) {
    setup();
    for(;;)
        loop();
    return 0;
}
