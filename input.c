///////////////////////////////////////////////////////////////////////////////
// NIXIE Clock                                                               //
// Adam Greig, April 2009                                                    //
// Input Code                                                                //
///////////////////////////////////////////////////////////////////////////////

//============================================================================
// Includes
//----------------------------------------------

#include "input.h"

//============================================================================
// Input Functions
//----------------------------------------------

// Set up the ports for the button matrix
void input_init() {
    
    // Set R1, R2 as outputs and C1, C2, C3, C4 as inputs
    DDRB |= (1<<R1 | 1<<R2);
    DDRD &= ~(1<<C1 | 1<<C2 | 1<<C3 | 1<<C4);
    
    // Leave R1, R2 high and enable pullups on C1, C2, C3, C4
    PORTB |= (1<<R1 | 1<<R2);
    PORTD |= (1<<C1 | 1<<C2 | 1<<C3 | 1<<C4);
    
}

// Scan the input matrix, processing any pressed keys
void input_scan() {
    char op  = 0x00;
    char arg = 0x00;
    
    // Set row 1 low then scan the columns
    PORTB &= ~(1<<R1);
    PORTB |=  (1<<R2);
    _delay_ms(5);
    
    if((PIND & (1<<C4)) == 0)
        op = '+';
    
    if((PIND & (1<<C3)) == 0)
        arg = 'Y';
    
    if((PIND & (1<<C2)) == 0)
        arg = 'M';
    
    if((PIND & (1<<C1)) == 0)
        arg = 'D';
    
    // Set row 2 low then scan the columns
    PORTB |=  (1<<R1);
    PORTB &= ~(1<<R2);
    _delay_ms(5);
    
    if((PIND & (1<<C4)) == 0)
        op = '-';
    
    if((PIND & (1<<C3)) == 0)
        arg = 's';
    
    if((PIND & (1<<C2)) == 0)
        arg = 'm';
    
    if((PIND & (1<<C1)) == 0)
        arg = 'h';
    
    // Finish up, setting both rows high again
    PORTB |= (1<<R1) | (1<<R2);
    
    // Update the internal time based on the button presses
    switch(arg) {
        // Due to a compiler bug/"feature", you can't have a declaration
        //  directly after a label. The fix is to shove a ; after the label.
        // http://gcc.gnu.org/ml/gcc-bugs/2008-08/msg01894.html
        case 'Y':;
            // Update year, wrapping around at 99
            unsigned short int year = 10*year_t + year_u;
            if(op == '+') {
                year = (year + 1) % 100;
            } else if(op == '-') {
                year = (year - 1) % 100;
            }
            year_t = year / 10;
            year_u = year % 10;
            break;
        case 'M':;
            // Update month, wrapping around at 12, correcting for 0
            unsigned short int month = 10*month_t + month_u;
            if(op == '+') {
                month = (month + 1) % 13;
            } else if(op == '-') {
                month = (month - 1) % 13;
                if(month == 0)
                    month = 12;
            }
            month_t = month / 10;
            month_u = month % 10;
            break;
        case 'D':;
            // Update day of month, wrapping at 31, correcting for 0
            unsigned short int date = 10*date_t + date_u;
            if(op == '+') {
                date = (date + 1) % 32;
            } else if(op == '-') {
                date = (date - 1) % 32;
                if(date == 0)
                    date = 31;
            }
            date_t = date / 10;
            date_u = date % 10;
            break;
        case 'h':;
            // Update hour, wrapping at 23
            unsigned short int hour = 10*hour_t + hour_u;
            if(op == '+') {
                hour = (hour + 1) % 24;
            } else if(op == '-') {
                hour = (hour - 1) % 24;
            }
            hour_t = hour / 10;
            hour_u = hour % 10;
            break;
        case 'm':;
            // Update minute, wrapping at 59
            unsigned short int min = 10*min_t + min_u;
            if(op == '+') {
                min = (min + 1) % 60;
            } else if(op == '-') {
                min = (min - 1) % 60;
            }
            min_t = min / 10;
            min_u = min % 10;
            break;
        case 's':;
            // Update seconds, wrapping at 59
            unsigned short int sec = 10*sec_t + sec_u;
            if(op == '+') {
                sec = (sec + 1) % 60;
            } else if(op == '-') {
                sec = (sec - 1) % 60;
            }
            sec_t = sec / 10;
            sec_u = sec % 10;
            break;
    }
    
    // Actually send the new time, if + or - happened
    if(op == '+' || op == '-')
        rtc_set();

}
