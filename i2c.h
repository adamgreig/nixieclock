///////////////////////////////////////////////////////////////////////////////
// NIXIE Clock                                                               //
// Adam Greig, April 2009                                                    //
// I2C Header                                                                //
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

#define SDA PC4
#define SCL PC5

//============================================================================
// Globals
//----------------------------------------------

// Store the date and time in BCD
extern unsigned short int sec_t, sec_u, min_t, min_u, hour_t, hour_u, day;
extern unsigned short int date_t, date_u, month_t, month_u, year_t, year_u;

//============================================================================
// I2C Functions
//----------------------------------------------

// Set up the I2C registers
void i2c_init();

// Generate a START (or repeated START) condition and wait for it to happen
void i2c_start();

// Generate a STOP condition
void i2c_stop();

// Send one byte over I2C
void i2c_send(char dat);

// Read one byte, generating an ACK, used to indicate you do want more data
char i2c_read();

// Read one byte, generating a NAK, used to indicate you don't want more data
char i2c_read_last();
