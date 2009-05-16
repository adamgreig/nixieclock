///////////////////////////////////////////////////////////////////////////////
// NIXIE Clock                                                               //
// Adam Greig, April 2009                                                    //
// RTC Code                                                                  //
///////////////////////////////////////////////////////////////////////////////

//============================================================================
// Includes
//----------------------------------------------

#include "rtc.h"

//============================================================================
// Real Time Clock functions
//----------------------------------------------

// Send the internal date to the RTC
void rtc_set() {
    
    // Start talking
    i2c_start();
    i2c_send(DS1307 << 1 | 0);
    
    // Start writing at register 0x00
    i2c_send(0x00);
    
    // Send the seconds
    i2c_send(sec_t << 4 | sec_u);
    
    // Send the minutes
    i2c_send(min_t << 4 | min_u);
    
    // Send the hours
    i2c_send(hour_t << 4 | hour_u);
    
    // Send the day
    i2c_send(day);
    
    // Send the date
    i2c_send(date_t << 4 | date_u);
    
    // Send the month
    i2c_send(month_t << 4 | month_u);
    
    // Send the year
    i2c_send(year_t << 4 | year_u);
    
    // Finished
    i2c_stop();
}

// Read the date from the RTC, storing it internally
void rtc_read() {
    
    // Start talking
    i2c_start();
    i2c_send(DS1307 << 1 | 0);
    
    // We want to read from register 0x00 onwards
    i2c_send(0x00);
    
    // Start listening
    i2c_start();
    i2c_send(DS1307 << 1 | 1);
    char byte;
    
    // Read in data. Hex constants are masks for that field, see
    //  the DS1307 datasheet. Tens fields get shifted down as well.
    
    // Read in seconds
    byte = i2c_read();
    sec_t = (byte & 0x70) >> 4;
    sec_u = (byte & 0x0F);
    
    // Read in minutes
    byte = i2c_read();
    min_t = (byte & 0xF0) >> 4;
    min_u = (byte & 0x0F);
    
    // Read in hours
    byte = i2c_read();
    hour_t = (byte & 0x30) >> 4;
    hour_u = (byte & 0x0F);
    
    // Read in the day
    byte = i2c_read();
    day = (byte & 0x07);
    
    // Read in the date
    byte = i2c_read();
    date_t = (byte & 0xF0) >> 4;
    date_u = (byte & 0x0F);
    
    // Read in the month
    byte = i2c_read();
    month_t = (byte & 0xF0) >> 4;
    month_u = (byte & 0x0F);
    
    // Read in the year, our last data field
    byte = i2c_read_last();
    year_t = (byte & 0xF0) >> 4;
    year_u = (byte & 0x0F);
    
    // Finished
    i2c_stop();
}
