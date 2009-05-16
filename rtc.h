///////////////////////////////////////////////////////////////////////////////
// NIXIE Clock                                                               //
// Adam Greig, April 2009                                                    //
// RTC Header                                                                //
///////////////////////////////////////////////////////////////////////////////

#pragma once

//============================================================================
// Includes
//----------------------------------------------

#include <stdlib.h>
#include <avr/io.h>

#include "i2c.h"

//============================================================================
// Constants
//----------------------------------------------

#define DS1307 0x68

//============================================================================
// Globals
//----------------------------------------------

// Store the date and time in BCD
extern unsigned short int sec_t, sec_u, min_t, min_u, hour_t, hour_u, day;
extern unsigned short int date_t, date_u, month_t, month_u, year_t, year_u;

//============================================================================
// Real Time Clock functions
//----------------------------------------------

// Send the internal date to the RTC
void rtc_set();

// Read the date from the RTC, storing it internally
void rtc_read();
