// NIXIE Clock
// Adam Greig, April 2009

//============================================================================
// Includes
//----------------------------------------------
#include <WProgram.h>
#include <stdlib.h>
#include <avr/interrupt.h>
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
// Helper Functions
//----------------------------------------------

//Set the latch pin low
void start_data() {
    PORTD = (0<<RCK);
}

//Set latch pin high
void end_data() {
    PORTD = (1<<RCK);
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

    DDRB  = (1<<PB0)  | (1<<PB1);
    DDRC  = (1<<SHDN) | (1<<PC4) | (1<<PC5);
    DDRD  = (0<<PD0)  | (0<<PD1) | (0<<PD2) | (0<<PD3);
    DDRD |= (1<<RCK)  | (1<<SCK) | (1<<SER);
    
    PORTC = (0<<SHDN);
    PORTD = (1<<RCK);

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
// Arduino Wrapper
//----------------------------------------------

int main(void) {
    init();
    setup();
    for(;;)
        loop();
    return 0;
}
