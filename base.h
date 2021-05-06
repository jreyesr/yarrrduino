/*
   File:   base.h
   Author: reyes

   Created on May 3, 2021, 11:37 AM
*/
#include <stdbool.h>
#include <stdint.h>

#ifndef BASE_H
#define	BASE_H

//#include <pic16f1455.h>
#define _XTAL_FREQ 48000000
//#include <pic.h>

#include "hardware.h"
#include "messages.h"
#include "configuration.h"
#include "types.h"


#ifdef	__cplusplus
extern "C" {
#endif

#define BP_FIRMWARE_STRING "Community Firmware v7.1 - goo.gl/gCzQnW - Modified by jreyesr "
//#define BP_FIRMWARE_STRING "No space!"

/**
  @brief Current mode configuration settings structure.

  This is used to let protocol implementations to interact with the Bus Pirate
  event loop.  Every time a mode is changed (a.k.a. a new protocol gets
  activated) this structure is cleared.
*/
typedef struct {
  uint8_t speed;
  uint8_t numbits;

  /**
     Which pin is currently used as the `AUX` I/O.
  */
  uint8_t alternate_aux : 2;

  uint8_t periodicService : 1;

  /**
     Values that can be either big or little endian are forced to read as
     little endian.
  */
  uint8_t little_endian : 1;

  /**
     The device is in high impedance mode.
  */
  uint8_t high_impedance : 1;

  /**
     Values are 16-bits wide.
  */
  uint8_t int16 : 1;

  /**
     Each I/O write from the protocol must be followed by a read operation.
  */
  uint8_t write_with_read : 1;

  /**
     The last command triggered an error.
  */
  uint8_t command_error : 1;

} __attribute__((packed)) mode_configuration_t;

typedef struct {
  uint16_t num;
  uint16_t repeat;
  uint8_t cmd;
} __attribute__((packed)) command_t;

/**
   @brief Reverses the bits in the given value and returns it.

   @param[in] value the value whose bits should be reversed.
   @param[in] bits width of the value to reverse, in bits.

   @return the value with reversed bits.
*/
uint16_t bp_reverse_integer(const uint16_t value, const uint8_t bits);

/**
   @brief Checks whether the transmission queue is empty or not.

   @return YES if the transmission queue is empty, NO otherwise.
*/
bool user_serial_transmit_done(void);


/**
   @brief Spins and blocks the execution until serial has finished transmission
*/
void user_serial_wait_transmission_done(void);

/**
   @brief Checks whether the reception queue is full or not.

   @return YES if the reception queue is not full, NO otherwise.
*/
bool user_serial_ready_to_read(void);

/**
   @brief Blocks execution until a byte arrives on the user-facing serial port
   and returns said value.

   @return the byte read from the serial port.
*/
uint8_t user_serial_read_byte(void);

/**
   @brief Checks whether the transmission queue is full or not.

   @return YES if the transmission queue is not full, NO otherwise.
*/
bool user_serial_ready_to_tx(void);

/**
   @brief Prints the given value to the user terminal according to the format
   settings specified by bus_pirate_configuration_t.display_mode.

   @param[in] value the value to print to the serial port.
*/
void bp_write_formatted_integer(const uint16_t value);


/**
   @brief Pauses execution for the given amount of milliseconds.

   @param[in] milliseconds the amount of milliseconds to wait.
*/
void bp_delay_ms(uint16_t milliseconds);

/**
   @brief Pauses execution for the given amount of microseconds.

   @param[in] microseconds the amount of microseconds to wait.
*/
void bp_delay_us(uint16_t microseconds);

/**
   Writes the given NULL-terminated string to the serial port.

   @param[in] string the string to write.
*/
void bp_write_string(const char *string);

/**
   @brief Writes the given 8-bits value to the serial port in decimal form.

   @param[in] value the value to write.
*/
void bp_write_dec_byte(const uint8_t value);

/**
   @brief Writes the given 16-bits value to the serial port in hexadecimal form.

   @param[in] value the value to write.
*/
void bp_write_hex_word(const uint16_t value);

/**
   @brief Writes the given 8-bits value to the serial port in hexadecimal form.

   @param[in] value the value to write.
*/
void bp_write_hex_byte(const uint8_t value);


/**
   @brief Writes the given 8-bits value to the serial port in binary form.

   @param[in] value the value to write.
*/
void bp_write_bin_byte(const uint8_t value);

/**
   @brief Writes the given 16-bits value to the serial port in decimal form.

   @param[in] value the value to write.
*/
void bp_write_dec_word(const uint16_t value);

/**
 * @brief Writes the given ADC reading to the serial port in human-readable
 * form.
 *
 * @param[in] adc the ADC reading to print.
 */
void bp_write_voltage(const uint16_t adc);

/**
   @brief Writes the given NULL-terminated string to the serial port, followed
   by a line break.

   @param[in] string the string to write.
*/
void bp_write_line(const char *string);

/**
   @brief Writes the given character to the user-facing serial port, blocking
   until the transmission completes.

   @param[in] character the character to write.
*/
void user_serial_transmit_character(const char character);

/**
   @brief Shortcut for writing an empty line to the user-facing serial port.
*/
#define bpBR bp_write_line("")

/**
   @brief Shortcut for writing a space to the user-facing serial port.
*/
#define bpSP user_serial_transmit_character(' ')

/**
   @brief Clears the user-facing serial port overflow error flag.
*/
void user_serial_clear_overflow(void);

/**
   @brief Returns the user-facing serial port overflow error flag.

   @return YES if an overflow error was detected, NO otherwise.
*/
bool user_serial_check_overflow(void);

/**
   @brief Bring the board to a clean slate shortly before switching to a new
   operational mode.
*/
void bp_reset_board_state(void);

/**
  @brief Reads a value from the ADC on the given channel.

  @warning this function assumes the ADC is already enabled, and will not turn
           it on or off.

  @param[in] channel the channel to read data from.
  @return the value read from the channel.
*/
uint16_t bp_read_adc(const uint16_t channel);

/**
   @brief Takes one single ADC measurement and prints it to the serial port.
*/
void bp_adc_probe(void);

/**
   @brief Takes ADC measurements and prints them to the serial port until a byte
   is sent to the serial port.
*/
void bp_adc_continuous_probe(void);

#ifdef	__cplusplus
}
#endif

#endif	/* BASE_H */
