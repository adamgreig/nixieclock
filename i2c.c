///////////////////////////////////////////////////////////////////////////////
// NIXIE Clock                                                               //
// Adam Greig, April 2009                                                    //
// I2C Code                                                                  //
///////////////////////////////////////////////////////////////////////////////

//============================================================================
// Includes
//----------------------------------------------

#include "i2c.h"

//============================================================================
// I2C Functions
//----------------------------------------------

    /*
     * TWCR: Two Wire Control Register
            TWINT: Set to 1 to start the next action, is cleared when complete
            TWEN:  Set to 1 to enable the TWI interface
            TWSTA: Set to 1 to generate a [repeated] START condition
     * TWDR: Two Wire Data Register
            Stores one byte of data to send.
    */

// Initialise various I2C registers
void i2c_init() {
    
    // Set SDA and SCL to input
    DDRC &= ~(1<<SCL | 1<<SDA);
    // and enable their pullups
    PORTC |= (1<<SCL | 1<<SDA);
    
    // Set up the clock dividers
    TWBR  = 2;
    TWSR  = (1<<TWPS0)  | (1<<TWPS1);
    
    // Enable the I2C peripheral
    TWCR  = (1<<TWEN);
}

// Generate a START (or repeated START) condition and wait for it to happen
void i2c_start() {
    TWCR = (1<<TWINT) | (1<<TWEN) | (1<<TWSTA);
    while(!(TWCR & (1<<TWINT)));
}

// Generate a STOP condition
void i2c_stop() {
    TWCR = (1<<TWINT) | (1<<TWEN) | (1<<TWSTO);
}

// Send one byte over I2C
void i2c_send(char dat) {
    TWDR = dat;
    TWCR = (1<<TWINT) | (1<<TWEN);
    while(!(TWCR & (1<<TWINT)));
}

// Read one byte, generating an ACK, used to indicate you do want more data
char i2c_read() {
    TWCR = (1<<TWINT) | (1<<TWEN) | (1<<TWEA);
    while(!(TWCR & (1<<TWINT)));
    return TWDR;
}

// Read one byte, generating a NAK, used to indicate you don't want more data
char i2c_read_last() {
    TWCR = (1<<TWINT) | (1<<TWEN) | (0<<TWEA);
    while(!(TWCR & (1<<TWINT)));
    return TWDR;
}
