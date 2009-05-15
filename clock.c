// NIXIE Clock
// Adam Greig, April 2009

//============================================================================
// Includes
//----------------------------------------------
#include <WProgram.h>
#include <stdlib.h>
#include <avr/interrupt.h>
#include <avr/sig-avr.h>
#include <avr/io.h>

//============================================================================
// Constants
//----------------------------------------------

#define SHDN PC0

#define SCK PD5
#define RCK PD6
#define SER PD7

#define C1 PD0
#define C2 PD1
#define C3 PD2
#define C4 PD3

#define R1 PB0
#define R2 PB1

//============================================================================
// Output Functions
//----------------------------------------------

//Disable interrupts and set the latch pin low
void start_data() {
    cli();
    PORTD = (0<<RCK);
}

//Set latch pin high and re-enable interrupts
void end_data() {
    PORTD = (1<<RCK);
    sei();
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

//Top level function to output two digits
void out_num(unsigned short int num) {
    char n1 = (char)(num / 10);
    char n2 = (char)(num % 10);
    shift_num(n2);
    shift_num(n1);
}

//============================================================================
//============================================================================
// Initialisation
//----------------------------------------------

void setup() {

    DDRB  = (1<<R1)     | (1<<R2);
    DDRC  = (1<<SHDN)   | (1<<PC4)  | (1<<PC5);
    DDRD  = (0<<C1)     | (0<<C2)   | (0<<C3)   | (0<<C4);
    DDRD |= (1<<RCK)    | (1<<SCK)  | (1<<SER);
    
    PORTB = (0<<R1)     | (0<<R2);
    PORTC = (0<<SHDN);
    PORTD = (1<<C1)     | (1<<C2)   | (1<<C3)   | (1<<C4)   | (1<<RCK);
    
    EICRA = (0<<ISC11)  | (0<<ISC10);
    EIMSK = (1<<INT1);
    
    sei();

}

//============================================================================
//============================================================================
// Main Loop
//----------------------------------------------


void loop() {
    unsigned int i;
    for(i=0; i<100; i++) {
        start_data();
        out_num(i);
        end_data();
        delay(300);
    }
}

//============================================================================
//============================================================================
// Interrupt Handlers
//----------------------------------------------

INTERRUPT(SIG_INTERRUPT1) {
    char op, arg;
    
    PORTB = (0<<R1) | (1<<R2);
    
    if(PIND & (1<<C4))
        op = '+';
    
    if(PIND & (1<<C1))
        arg = 'Y';
    else if(PIND & (1<<C2))
        arg = 'M';
    else if(PIND & (1<<C3))
        arg = 'D';
    
    PORTB = (1<<R1) | (0<<R2);
    
    if(PIND & (1<<C4))
        op = '-';
    
    if(PIND & (1<<C1))
        arg = 'h';
    else if(PIND & (1<<C2))
        arg = 'm';
    else if(PIND & (1<<C3))
        arg = 's';
    
    PORTB = (0<<R1) | (0<<R2);
    
}

//============================================================================
//============================================================================
// Arduino Wrapper
//----------------------------------------------

int main(void) {
    init();
    setup();
    for(;;)
        loop();
    return 0;
}
