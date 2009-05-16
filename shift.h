///////////////////////////////////////////////////////////////////////////////
// NIXIE Clock                                                               //
// Adam Greig, April 2009                                                    //
// Shift Register Control Header                                             //
///////////////////////////////////////////////////////////////////////////////

#pragma once

//============================================================================
// Includes
//----------------------------------------------

#include <stdlib.h>
#include <avr/io.h>

//============================================================================
// Constants
//----------------------------------------------

#define SCK PD5
#define RCK PD6
#define SER PD7

//============================================================================
// Globals
//----------------------------------------------

// Store the date and time in BCD
extern unsigned short int sec_t, sec_u, min_t, min_u, hour_t, hour_u, day;
extern unsigned short int date_t, date_u, month_t, month_u, year_t, year_u;

//============================================================================
// Shift Functions
//----------------------------------------------

// Set up ports for the shift registers
void shift_init();

// Set the latch pin low
void shift_start_data();

// Set latch pin high
void shift_end_data();

// Shift out one given digit, correcting for mirrored nixie
void shift_num(char num);

// Shift out the current seconds
void shift_out_sec();

// Shift out the current minutes
void shift_out_min();

// Shift out the current hour
void shift_out_hour();

// Shift out the current time, HHMMSS
void shift_out_time();
