#include "raw2wire.h"


#ifdef BP_ENABLE_RAW_2WIRE_SUPPORT

#include "aux_pin.h"
#include "base.h"
#include "core.h"
#include "bitbang.h"
#include "proc_menu.h"

#define R2WCLK_TRIS BP_CLK_DIR
#define R2WCLK BP_CLK

#define R2WDIO_TRIS BP_MOSI_DIR
#define R2WDIO BP_MOSI

typedef enum {
  RAW2WIRE_MACRO_MENU = 0,
} raw2wire_macro_identifier_t;

extern mode_configuration_t mode_configuration;
extern command_t last_command;

uint16_t raw2wire_read(void) {
  return bitbang_read_value();
}

uint16_t raw2wire_write(const uint16_t value) {
  bitbang_write_value(value);
  return 0x100;
}

void raw2wire_start(void) {
  bitbang_i2c_start(BITBANG_I2C_START_ONE_SHOT);
  MSG_RAW2WIRE_I2C_START;
  MSG_I2C_START_BIT;
}

void raw2wire_stop(void) {
  bitbang_i2c_stop();
  MSG_RAW2WIRE_I2C_STOP;
  MSG_I2C_STOP_BIT;
}

void raw2wire_print_settings(void) {
  MSG_RAW2WIRE_MODE_HEADER;
  bp_write_dec_byte(mode_configuration.speed);
  bpSP;
  bp_write_dec_byte(mode_configuration.high_impedance);
  bpSP;
  MSG_MODE_HEADER_END;
}

void raw2wire_setup_prepare(void) {
  consumewhitechars();
  int speed = getint();
  consumewhitechars();
  int output = getint();

  bool user_prompt =
    !(((speed > 0) && (speed <= 4)) && ((output > 0) && (output <= 2)));

  if (user_prompt) {
    MSG_SOFTWARE_MODE_SPEED_PROMPT;
    mode_configuration.speed = getnumber(1, 1, 4, 0) - 1;
    MSG_PIN_OUTPUT_TYPE_PROMPT;
    mode_configuration.high_impedance = ~(getnumber(1, 1, 2, 0) - 1);
    mode_configuration.command_error = NO;
  } else {
    mode_configuration.speed = speed - 1;
    mode_configuration.high_impedance = (output - 1) == 0;
    raw2wire_print_settings();
  }
}

void raw2wire_setup_execute(void) {
  R2WCLK = LOW;
  R2WDIO = LOW;
  R2WDIO_TRIS = INPUT;
  R2WCLK_TRIS = OUTPUT;
  bitbang_setup(2, mode_configuration.speed);
}

void raw2wire_run_macro(const uint16_t macro_id) {
  MSG_UNKNOWN_MACRO_ERROR;
}

void raw2wire_print_pins_state(void) {
  MSG_I2C_PINS_STATE;
}


#endif
