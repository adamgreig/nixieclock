///////////////////////////////////////////////////////////////////////////////
// NIXIE Clock                                                               //
// Adam Greig, April 2009                                                    //
// Input Header                                                              //
///////////////////////////////////////////////////////////////////////////////

#pragma once

//============================================================================
// Includes
//----------------------------------------------

#include <stdlib.h>
#include <avr/io.h>
#include <util/delay.h>

#include "rtc.h"

//============================================================================
// Constants
//----------------------------------------------

#define C1 PD0
#define C2 PD1
#define C3 PD2
#define C4 PD3

#define R1 PB0
#define R2 PB1

//============================================================================
// Globals
//----------------------------------------------

// Store the date and time in BCD
extern unsigned short int sec_t, sec_u, min_t, min_u, hour_t, hour_u, day;
extern unsigned short int date_t, date_u, month_t, month_u, year_t, year_u;

//============================================================================
// Input Functions
//----------------------------------------------

// Set up the ports for the button matrix
void input_init();


// Scan the input matrix, processing any pressed keys
void input_scan();