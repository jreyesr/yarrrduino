/*
   File:   i2c.h
   Author: reyes

   Created on May 6, 2021, 11:01 AM
*/

#ifndef I2C_H
#define  I2C_H

#include "configuration.h"

#ifdef  __cplusplus
extern "C" {
#endif

#ifdef BP_ENABLE_I2C_SUPPORT

#include <stdint.h>

/**
   Starts I2C operations.
*/
void i2c_start(void);

/**
   Stops I2C operations.
*/
void i2c_stop(void);

/**
   Cleans up after I2C operations.
*/
void i2c_cleanup(void);

/**
   Prints the I2C mode settings to the serial port.
*/
void i2c_print_settings(void);

/**
   Prints the I2C pins state to the serial port.
*/
void i2c_pins_state(void);

uint16_t i2c_read(void);
uint16_t i2c_write(const uint16_t value);
void i2c_setup_prepare(void);
void i2c_setup_execute(void);
void i2c_macro(const uint16_t macro);

void binary_io_enter_i2c_mode(void);

#endif /* BP_ENABLE_I2C_SUPPORT */


#ifdef  __cplusplus
}
#endif

#endif  /* I2C_H */
