///////////////////////////////////////////////////////////////////////////////
// NIXIE Clock                                                               //
// Adam Greig, April 2009                                                    //
// Main Code                                                                 //
///////////////////////////////////////////////////////////////////////////////

//============================================================================
// Includes
//----------------------------------------------

#include <stdlib.h>
#include <avr/io.h>
#include <util/delay.h>

#include "input.h"
#include "rtc.h"
#include "shift.h"

//============================================================================
// Constants
//----------------------------------------------

// Power Supply shutdown
#define SHDN PC0

//============================================================================
// Globals
//----------------------------------------------

// Store the date and time in BCD
unsigned short int sec_t, sec_u, min_t, min_u, hour_t, hour_u, day;
unsigned short int date_t, date_u, month_t, month_u, year_t, year_u;

//============================================================================
// Power Supply Control
//----------------------------------------------

// Turn the power supply on (ground SHDN)
void power_on() {
    PORTC &= ~(1<<SHDN);
}

// Turn the power supply off (raise SHDN)
void power_off() {
    PORTC |= (1<<SHDN);
}

//============================================================================
//============================================================================
// Initialisation
//----------------------------------------------

void setup() {

    // Zero all the ports to start with
    DDRB  = DDRC  = DDRD  = 0;
    PORTB = PORTC = PORTD = 0;
    
    // Set SHDN to an output and set it low
    DDRC |= (1<<SHDN);
    PORTC &= ~(1<<SHDN);

    i2c_init();
    input_init();
    shift_init();
    
}

//============================================================================
// Main Loop
//----------------------------------------------

void loop() {
    // Get the current time from the RTC
    rtc_read();
    
    // Check if it's too early in the morning to be showing the time
    if((10*hour_t + hour_u) >= 7) {
        // If so, power on the PSU, check the input buttons, then show the time
        power_on();
        input_scan();
        shift_out_time();
    } else {
        // Or if not, turn off the PSU, disabling the tubes
        power_off();
    }
    
    _delay_ms(400);
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
