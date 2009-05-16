///////////////////////////////////////////////////////////////////////////////
// NIXIE Clock                                                               //
// Adam Greig, April 2009                                                    //
// Shift Register Control Code                                               //
///////////////////////////////////////////////////////////////////////////////

//============================================================================
// Includes
//----------------------------------------------

#include "shift.h"

//============================================================================
// Shift Functions
//----------------------------------------------

// Set up ports for the shift registers
void shift_init() {
    
    // Set RCK, SCK, SER as outputs, and leave RCK high (idle)
    DDRD  |= (1<<RCK | 1<<SCK | 1<<SER);
    PORTD |= (1<<RCK);
    
}

// Set the latch pin low
void shift_start_data() {
    PORTD &= ~(1<<RCK);
}

// Set latch pin high
void shift_end_data() {
    PORTD |= (1<<RCK);
}

// Shift out one given digit, correcting for mirrored nixie
void shift_num(char num) {
    num = (11 - num) % 10;
    
    PORTD &= ~(1<<SCK | 1<<SER);
    if(num & 8)
        PORTD |= (1<<SER);
    PORTD |= (1<<SCK);
    
    PORTD &= ~(1<<SCK | 1<<SER);
    if(num & 4)
        PORTD |= (1<<SER);
    PORTD |= (1<<SCK);
    
    PORTD &= ~(1<<SCK | 1<<SER);
    if(num & 2)
        PORTD |= (1<<SER);
    PORTD |= (1<<SCK);
    
    PORTD &= ~(1<<SCK | 1<<SER);
    if(num & 1)
        PORTD |= (1<<SER);
    PORTD |= (1<<SCK);
    
}

// Shift out the current seconds
void shift_out_sec() {
    shift_num(sec_u);
    shift_num(sec_t);
}

// Shift out the current minutes
void shift_out_min() {
    shift_num(min_u);
    shift_num(min_t);
}

// Shift out the current hour
void shift_out_hour() {
    shift_num(hour_u);
    shift_num(hour_t);
}

// Shift out the current time, HHMMSS
void shift_out_time() {
    shift_start_data();
    shift_out_sec();
    shift_out_min();
    shift_out_hour();
    shift_end_data();
}
