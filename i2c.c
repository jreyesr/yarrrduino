#include "i2c.h"

#ifdef BP_ENABLE_I2C_SUPPORT

#include "aux_pin.h"
#include "base.h"
#include "bitbang.h"
#include "core.h"
#include "proc_menu.h"

/**
   Use a software I2C communication implementation
*/
#define I2C_TYPE_SOFTWARE 0

/**
   Use the built-in hardware I2C communication implementation
*/
#define I2C_TYPE_HARDWARE 1

/**
   I2C ACK bit value.
*/
#define I2C_ACK_BIT 0

/**
   I2C NACK bit value.
*/
#define I2C_NACK_BIT 1

#define I2C_SNIFFER_ESCAPE '\\'
#define I2C_SNIFFER_NACK '-'
#define I2C_SNIFFER_ACK '+'
#define I2C_SNIFFER_START '['
#define I2C_SNIFFER_STOP ']'

typedef struct {

  /**
     Flag indicating whether a software-only I2C implementation should be
     used instead of the built-in hardware I2C interface.

     @see I2C_TYPE_SOFTWARE
     @see I2C_TYPE_HARDWARE
  */
  uint8_t mode : 1;

  /**
     Flag indicating whether there is either an ACK/NACK to be received.
  */
  uint8_t acknowledgment_pending : 1;

} i2c_state_t;

/**
   Current I2C module state.
*/
static i2c_state_t i2c_state = {0};

#define SCL BP_CLK
#define SCL_TRIS BP_CLK_DIR
#define SDA BP_MOSI
#define SDA_TRIS BP_MOSI_DIR

extern bus_pirate_configuration_t bus_pirate_configuration;
extern mode_configuration_t mode_configuration;
extern command_t last_command;

/**
   Handles a pending acknowledgement by sending either an ACK or a NACK on the
   bus.

   @param[in] bus_bit false for sending an ACK, true for sending a NACK.

   @see I2C_ACK_BIT
   @see I2C_NACK_BIT
*/
static void handle_pending_ack(const bool bus_bit);

/**
   Performs the bulk of the write-then-read I2C binary IO command.

   @return true if the operation was successful, false otherwise.
*/
static bool i2c_write_then_read(void);

uint16_t i2c_read(void) {
  uint8_t value;

  if (i2c_state.acknowledgment_pending) {
    handle_pending_ack(I2C_ACK_BIT);
    bpSP;
    MSG_ACK;
    bpSP;
  }

  value = bitbang_read_value();
  i2c_state.acknowledgment_pending = true;
  return value;
}

unsigned int i2c_write(unsigned int c) {
  if (i2c_state.acknowledgment_pending) {
    bpSP;
    MSG_ACK;
    bpSP;
    handle_pending_ack(I2C_ACK_BIT);
  }

  bitbang_write_value(c);
  c = bitbang_read_bit();

  bpSP;
  if (c == 0) {
    MSG_ACK;
    return 0x300; // bit 9=ack
  }

  MSG_NACK;
  return 0x100; // bit 9=ack
}

void i2c_start(void) {
  /* Reset the bus state if an acknowledgement is pending. */

  if (i2c_state.acknowledgment_pending) {
    MSG_NACK;
    bpBR;

    handle_pending_ack(I2C_NACK_BIT);
  }

  /* Send a start signal on the bus. */

  if (bitbang_i2c_start(BITBANG_I2C_START_ONE_SHOT)) {
    /* There is a short or pull-ups are wrong. */
    MSG_WARNING_HEADER;
    MSG_WARNING_SHORT_OR_NO_PULLUP;
    bpBR;
  }

  MSG_I2C_START_BIT;
}

void i2c_stop(void) {
  if (i2c_state.acknowledgment_pending) {
    MSG_NACK;
    bpBR;

    handle_pending_ack(I2C_NACK_BIT);
  }

  bitbang_i2c_stop();

  MSG_I2C_STOP_BIT;
}

void i2c_print_settings(void) {
  BPMSG1068;
  bp_write_dec_byte(0);
  bpSP;
  bp_write_dec_byte(mode_configuration.speed);
  bp_write_line(" )");
}

void i2c_setup_prepare(void) {
  int speed;

  i2c_state.mode = I2C_TYPE_SOFTWARE;

  consumewhitechars();
  speed = getint();

  if ((speed > 0) && (speed <= 4)) {
    mode_configuration.speed = speed - 1;
  } else {
    speed = 0;
  }

  if (speed == 0) {
    mode_configuration.command_error = NO;

    i2c_state.mode = I2C_TYPE_SOFTWARE;

    MSG_SOFTWARE_MODE_SPEED_PROMPT;
    mode_configuration.speed = (getnumber(1, 1, 4, 0) - 1);
  } else {
    i2c_print_settings();

    i2c_state.acknowledgment_pending = false;
  }

  mode_configuration.high_impedance = ON;
}

void i2c_setup_execute(void) {
  SDA_TRIS = INPUT;
  SCL_TRIS = INPUT;
  SCL = LOW;
  SDA = LOW;
  bitbang_setup(2, mode_configuration.speed);
}

void i2c_cleanup(void) {
  /* Clear any pending ACK from previous use. */
  i2c_state.acknowledgment_pending = false;
}

void i2c_macro(unsigned int c) {
  int i;

  switch (c) {

    case 0:
      BPMSG1069;
      break;

    case 1:
      // setup both lines high first
      bitbang_set_pins_high(MOSI | CLK, 0);
      BPMSG1070;
      if ((BP_CLK == LOW) || (BP_MOSI == LOW)) {
        MSG_WARNING_HEADER;
        MSG_WARNING_SHORT_OR_NO_PULLUP;
        bpBR;
        return;
      }

      for (i = 0; i < 0x100; i++) {
        bitbang_i2c_start(BITBANG_I2C_START_ONE_SHOT); // send start
        bitbang_write_value(i);                        // send address
        c = bitbang_read_bit();                        // look for ack

        if (c == I2C_ACK_BIT) {
          bp_write_formatted_integer(i);
          user_serial_transmit_character('(');
          bp_write_formatted_integer((i >> 1));
          /* If the first bit is set, this is a read address. */
          if ((i & 1) == 0) {
            MSG_I2C_WRITE_ADDRESS_END;
          } else {
            if (i2c_state.mode == I2C_TYPE_SOFTWARE) {
              bitbang_read_value();
              bitbang_write_bit(HIGH);
            } else {
            }
            MSG_I2C_READ_ADDRESS_END;
          }
        }

        bitbang_i2c_stop();
      }
      bpBR;
      break;
    case 2:
      i2c_cleanup();
      MSG_SNIFFER_MESSAGE;
      MSG_ANY_KEY_TO_EXIT_PROMPT;
      MSG_UNKNOWN_MACRO_ERROR;
      break;
    default:
      MSG_UNKNOWN_MACRO_ERROR;
      break;
  }
}

void i2c_pins_state(void) {
  MSG_I2C_PINS_STATE;
}

void handle_pending_ack(const bool bus_bit) {
  bitbang_write_bit(bus_bit);
  i2c_state.acknowledgment_pending = false;
}

bool i2c_write_then_read(void) {
  /* Read the amount of bytes to write. */
  size_t bytes_to_write = user_serial_read_big_endian_word();

  /* Read the amount of bytes to read. */
  size_t bytes_to_read = user_serial_read_big_endian_word();

#ifndef BP_I2C_ENABLE_STREAMING_WRITE
  /* Make sure data fits in the internal buffer. */
  if (bytes_to_write > BP_TERMINAL_BUFFER_SIZE) {
    return false;
  }
#endif /* !BP_I2C_ENABLE_STREAMING_WRITE */

#ifndef BP_I2C_ENABLE_STREAMING_READ
  /* Make sure data fits in the internal buffer. */
  if (bytes_to_write > BP_TERMINAL_BUFFER_SIZE) {
    return false;
  }
#endif /* !BP_I2C_ENABLE_STREAMING_READ */

  if ((bytes_to_write == 0) && (bytes_to_read == 0)) {
    return false;
  }

  uint8_t i2c_address = 0;

#ifdef BP_I2C_ENABLE_STREAMING_WRITE

  /* Read I2C address. */
  i2c_address = user_serial_read_byte();

  /* Start streaming. */
  bitbang_i2c_start(BITBANG_I2C_START_ONE_SHOT);

  /* Stream data from the serial port. */
  bitbang_write_value(i2c_address);

  for (size_t counter = 1; counter < bytes_to_write; counter++) {
    bitbang_write_value(user_serial_read_byte());

    if (bitbang_read_bit() == HIGH) {
      /* No ACK read on the bus, bailing out. */
      return false;
    }
  }

#else

  /* Read payload. */
  for (size_t index = 0; index < bytes_to_write; index++) {
    bus_pirate_configuration.terminal_input[index] = user_serial_read_byte();
  }

  /* Get I2C address. */
  i2c_address = bus_pirate_configuration.terminal_input[0];

  /* Signal write start. */
  bitbang_i2c_start(BITBANG_I2C_START_ONE_SHOT);

  /* Write the payload to the I2C bus. */
  for (size_t index = 0; index < bytes_to_write; index++) {
    bitbang_write_value(bus_pirate_configuration.terminal_input[index]);

    if (bitbang_read_bit() == HIGH) {
      /* No ACK read on the bus, bailing out. */
      return false;
    }
  }

#endif /* BP_I2C_ENABLE_STREAMING_WRITE */

  if ((bytes_to_read > 0) && (bytes_to_write > 1)) {
    /* Send a restart signal on the I2C bus. */
    bitbang_i2c_start(BITBANG_I2C_RESTART);

    /* Send the I2C address. */
    bitbang_write_value(i2c_address | 0x01);

    if (bitbang_read_bit() == HIGH) {
      /* No ACK read on the bus, bailing out. */
      return false;
    }
  }

  /* Read the rest of the data. */
  bytes_to_write = bytes_to_read - 1;

#ifdef BP_I2C_ENABLE_STREAMING_READ

  /* Start streaming data. */
  REPORT_IO_SUCCESS();

  for (size_t counter = 0; counter < bytes_to_read; counter++) {
    /* Read byte from the I2C bus. */
    user_serial_transmit_character(bitbang_read_value());

    /* Acknowledge read operation. */
    bitbang_write_bit(counter >= bytes_to_write ? HIGH : LOW);
  }

  /* Stop the I2C bus. */
  bitbang_i2c_stop();

#else

  for (size_t index = 0; index < bytes_to_read; index++) {
    /* Read the byte from the I2C bus. */
    bus_pirate_configuration.terminal_input[index] = bitbang_read_value();

    /* Report ACK or NACK depending on the length. */
    bitbang_write_bit(index >= bytes_to_write ? HIGH : LOW);
  }

  /* Stop the I2C bus operations. */
  bitbang_i2c_stop();

  /* Report operation status. */
  REPORT_IO_SUCCESS();

  /* And send the I2C data over to the UART. */
  bp_write_buffer(&bus_pirate_configuration.terminal_input[0], bytes_to_read);

#endif /* BP_I2C_ENABLE_STREAMING_READ */

  return true;
}

#endif /* BP_ENABLE_I2C_SUPPORT */
