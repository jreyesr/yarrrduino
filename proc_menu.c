#include <stdbool.h>
#include <stdlib.h>
//#include <pic.h>

#include "types.h"
#include "core.h"
#include "configuration.h"
#include "proc_menu.h"
#include "base.h"
#include "messages.h"
#include "aux_pin.h"
#include "hardware.h"

/**
   ASCII scancode for the NUL character.
*/
#define ASCII_NUL 0x00

/**
   ASCII scancode for the SOH character.
*/
#define ASCII_SOH 0x01

/**
   ASCII scancode for the STX character.
*/
#define ASCII_STX 0x02

/**
   ASCII scancode for the EOT character.
*/
#define ASCII_EOT 0x04

/**
   ASCII scancode for the ENQ character.
*/
#define ASCII_ENQ 0x05

/**
   ASCII scancode for the ACK character.
*/
#define ASCII_ACK 0x06

/**
   ASCII scancode for the BACKSPACE key.
*/
#define ASCII_BS 0x08

/**
   ASCII scancode for the LF character.
*/
#define ASCII_LF 0x0A

/**
   ASCII scancode for the CR character.
*/
#define ASCII_CR 0x0D

/**
   ASCII scancode for the SO character.
*/
#define ASCII_SO 0x0E

/**
   ASCII scancode for the DLE character.
*/
#define ASCII_DLE 0x10

/**
   ASCII scancode for the ESCAPE key.
*/
#define ASCII_ESCAPE 0x1B

/**
   ASCII scancode for the DELETE key.
*/
#define ASCII_DELETE 0x7F

extern bus_pirate_configuration_t bus_pirate_configuration;
extern mode_configuration_t mode_configuration;
extern command_t last_command;
extern bus_pirate_protocol_t enabled_protocols[ENABLED_PROTOCOLS_COUNT];

char cmdbuf[BP_COMMAND_BUFFER_SIZE] = {0};
unsigned int cmdend;
unsigned int cmdstart;

static const uint8_t READ_DISPLAY_BASE[] = {'x', 'd', 'b', 'w'};

/**
   Handles the backspace key being sent to the user menu.
*/
void handle_backspace(void);

/**
   Handles the delete key being sent to the user menu.
*/
void handle_delete(void);

/**
   Handles the escape key being sent to the user menu.
*/
void handle_escape_key(void);

/**
   Refreshes the current mode prompt.
*/
void refresh_mode_prompt(void);

/**
   Refreshes the command line with the data starting at the given offset.

   @param[in] start the offset in the command buffer where the new command line
   starts.
*/
void refresh_command_line(const uint16_t start);

/**
   Removes the currently pointed character in the command line buffer.
*/
void remove_current_character_from_command_line(void);

/**
   Handles the left arrow key being sent to the user menu.
*/
void handle_left_arrow(void);

/**
   Handles the right arrow key being sent to the user menu.
*/
void handle_right_arrow(void);

/**
   Handles the up arrow key being sent to the user menu.
*/
void handle_up_arrow(void);

/**
   Handles the down arrow key being sent to the user menu.
*/
void handle_down_arrow(void);

/**
   Handles the home key being sent to the user menu.
*/
void handle_home_key(void);

/**
   Handles the end key being sent to the user menu.
*/
void handle_end_key(void);

/**
   Handles the enter key being sent to the user menu.
*/
void handle_enter_key(void);

/**
   Handles a non-control character being sent to the user menu.
*/
void handle_character(const uint8_t character);

/**
   Outputs '0' to the serial port if the given value is zero, '1' otherwise.

   @param[in] value the value to test the state for.
*/
void echo_state(const uint16_t value);

/**
   Outputs both direction and state to the serial port for all available pins.
*/
void print_pins_information(void);

/**
   Outputs the direction (input/output) of the given pin to the serial port.

   @warning if the pin bit mask has more than one bit set to one, the result
   will print the AND result of all directions marked by the pin mask.  This
   should probably be not used that way, as it does not make much sense in the
   first place.

   @param[in] pin the pin to use for printing signal direction.
*/
void print_pin_direction(const uint16_t pin);

/**
   Outputs the state (low/high) of the given pin to the serial port.

   @warning if the pin bit mask h as more than one bit set to one, the result
   will print the AND result of all states marked by the pin mask.  This
   should probably be not used that way, as it does not make much sense in the
   first place.

   @param[in] pin the pin to use for printing signal state.
*/
void print_pin_state(const uint16_t pin);

/**
   Turns on the power supplies.
*/
void switch_psu_on(void);

/**
   Turns off the power supplies.
*/
void switch_psu_off(void);

void convert_value(const bool reversed);

void set_display_mode(void);
void print_status_info(void);

uint8_t change_read_display(void);

/**
   The user menu global state container structure.
*/
typedef struct {
  /** Cursor position inside the command line buffer. */
  uint16_t cursor_position;

  /** History entry index in the command line buffer. */
  uint16_t history_entry_counter;

  /** Binary mode entering sequence counter. */
  uint8_t binary_mode_counter;

  /** Flag indicating if a potentially valid command has been entered. */
  bool command_present;
} menu_state_t;

/**
   The user menu state.
*/
static menu_state_t menu_state = {0};

void serviceuser(void) {
  int newstart;
  int oldstart;
  unsigned int sendw, received;
  int repeat;
  int temp;
  int numbits;
  unsigned char oldDmode; // temporarily holds the default display mode, while a
  // different display read is performed
  unsigned char newDmode;

  // init
  cmdstart = 0;
  cmdend = 0;
  menu_state.cursor_position = cmdend;
  bus_pirate_configuration.bus_mode = BP_HIZ;
  mode_configuration.command_error = NO;

  newstart = 0;
  oldstart = 0;

  oldDmode = 0; // temporarily holds the default display mode, while a different
  // display read is performed
  newDmode = 0;


  for (;;) {
    bp_write_string(enabled_protocols[bus_pirate_configuration.bus_mode].name);
    bp_write_string(">");
    while (!menu_state.command_present) {

      /* Reset serial overflows if any are found. */

      /*if (user_serial_check_overflow()) {
          user_serial_clear_overflow();
          continue;
        }*/

      /* Process incoming byte. */

      uint8_t user_byte = user_serial_read_byte();
      //bp_write_hex_byte(user_byte);

      switch (user_byte) {

        case ASCII_BS:
          handle_backspace();
          break;

        case ASCII_STX:
          //bp_write_line("Left arrow!");
          handle_left_arrow();
          break;

        case ASCII_ACK:
          handle_right_arrow();
          break;

        case ASCII_EOT:
        case ASCII_DELETE:
          handle_delete();
          break;

        case ASCII_ESCAPE:
          handle_escape_key();
          break;

        case ASCII_DLE:
          handle_up_arrow();
          break;

        case ASCII_SO:
          handle_down_arrow();
          break;

        case ASCII_SOH:
          handle_home_key();
          break;

        case ASCII_ENQ:
          handle_end_key();
          break;

        case ASCII_CR:
        case ASCII_LF:
          handle_enter_key();
          break;
        default:
          handle_character(user_byte);
          break;
      }
    }

    /* Process current command line. */

    newstart = cmdend;
    oldstart = cmdstart;
    menu_state.command_present = NO;

    mode_configuration.command_error = NO;

    bool stop = NO;

    oldDmode = 0; // temporarily holds the default display mode, while a
    // different display read is performed
    newDmode = 0;

    while (!stop) {
      uint8_t c = cmdbuf[cmdstart];

      switch (c) {
        case '?':
          print_help();
          break;

        case 'i':
          print_version_info();
          if (bus_pirate_configuration.bus_mode != BP_HIZ) {
            print_status_info();
          }
          break;

        case 'm':
          changemode();
          break;

        /*case 'b':
            set_baud_rate();
            break;*/

        case 'o':
          set_display_mode();
          break;

        case 'v':
          print_pins_information();
          break;

        /*case 'f':
            bp_frequency_counter_setup();
            break;

          case 'g':
            if (bus_pirate_configuration.bus_mode == BP_HIZ) {
                BPMSG1088;
            } else {
                bp_pwm_setup();
            }
            break;*/

        case 'c':
          mode_configuration.alternate_aux = 0;
          BPMSG1086;
          break;

        case 'C':
          mode_configuration.alternate_aux = 1;
          BPMSG1087;
          break;

        case 'L': // Set little endian
          mode_configuration.little_endian = YES;
          BPMSG1124;
          bpBR;
          break;

        case 'l': // Set big endian
          mode_configuration.little_endian = NO;
          BPMSG1123;
          bpBR;
          break;
        /*
                            case 'p':
                                // don't allow pullups on some modules. also: V0a limitation of 2
                                // resistors
                                if (bus_pirate_configuration.bus_mode == BP_HIZ) {
                                    BPMSG1088;
                                } else {
                                    bp_disable_pullup();
                                    BPMSG1089;
                                    bpBR;
                                }
                                break;

                            case 'P':
                                // don't allow pullups on some modules. also: V0a limitation of 2
                                // resistors
                                if (bus_pirate_configuration.bus_mode == BP_HIZ) {
                                    BPMSG1088;
                                } else {
                                    if (mode_configuration.high_impedance == NO) {
                                        BPMSG1209;
                                    }
                                    bp_enable_pullup();
                                    BPMSG1091;
                                    bpBR;

                                    bp_enable_adc();
                                    if (bp_read_adc(BP_ADC_VPU) < 0x50) {
                                        // No voltage on pullup pin detected.
                                        MSG_NO_VOLTAGE_ON_PULLUP_PIN;
                                    }
                                    bp_disable_adc();
                                }
                                break;
        */
        case '=': // Print value in hex, bin and dec
          convert_value(NO);
          break;

        case '|': // Invert bits, then print value in hex, bin and dec
          convert_value(YES);
          break;
        /*
                            case '~': // Run self test
                                if (bus_pirate_configuration.bus_mode == BP_HIZ) {
                                    perform_selftest(true, true);
                                } else {
                                    BPMSG1092;
                                }
                                break;
        */
        case '#': // Reset
          BPMSG1093;
          user_serial_wait_transmission_done();
          __asm volatile("JMP 0"); // Fake SW reset!
          break;
        /*
                            case '$': // Jump to bootloader
                                if (agree()) {
                                    skip_pgc_pgd_check = true;
                                    BPMSG1094;
                                    bp_delay_ms(100);
                                    bp_reset_board_state();
                                    user_serial_wait_transmission_done();
                                    __asm volatile("RESET");
                                }
                                break;
        */
        case 'a':
          bp_aux_pin_set_low();
          break;

        case 'A':
          bp_aux_pin_set_high();
          break;

        case '@':
          repeat = getrepeat() + 1;
          while (--repeat) {
            BPMSG1095;
            echo_state(bp_aux_pin_read());
            bpBR;
          }
          break;

        case 'W':
          switch_psu_on();
          break;

        case 'w':
          switch_psu_off();
          break;

        case 'd': // ADC single-shot
          MSG_ADC_VOLTAGE_PROBE_HEADER;
          bp_adc_probe();
          MSG_VOLTAGE_UNIT;
          bpBR;
          break;

        case 'D': // ADC continuous
          bp_adc_continuous_probe();
          break;

        case '&':
          repeat = getrepeat();
          BPMSG1099;
          bp_write_dec_word(repeat);
          BPMSG1100;
          bp_delay_us(repeat);
          break;

        case '%':
          repeat = getrepeat();
          BPMSG1099;
          bp_write_dec_word(repeat);
          BPMSG1212;
          bp_delay_ms(repeat);
          break;
        /*
                            case 'S': // Servos, NOPE
                                if (bus_pirate_configuration.bus_mode == BP_HIZ) {
                                    BPMSG1088;
                                } else {
                                    bp_servo_setup();
                                }
                                break;
        */
        // command for subsys (i2c, UART, etc)
        case '(': // Macros
          cmdstart = (cmdstart + 1) & CMDLENMSK;
          sendw = getint();
          consumewhitechars();
          if (cmdbuf[((cmdstart) & CMDLENMSK)] == ')') {
            enabled_protocols[bus_pirate_configuration.bus_mode].run_macro(sendw);
            bpBR;
          } else {
            mode_configuration.command_error = YES;
          }
          break;

        case '"':
          mode_configuration.command_error = YES;
          temp = 1;

          while (cmdbuf[((cmdstart + temp) & CMDLENMSK)] != 0x00) {
            if (cmdbuf[((cmdstart + temp) & CMDLENMSK)] == '"')
              mode_configuration.command_error =
                NO; // clear error if we found a " before the command ends
            temp++;
          }

          if (mode_configuration.command_error == NO) {
            BPMSG1101;
            user_serial_transmit_character('"');
            while (cmdbuf[((++cmdstart) & CMDLENMSK)] != '"') {
              cmdstart &= CMDLENMSK;
              user_serial_transmit_character(cmdbuf[cmdstart]);
              sendw = cmdbuf[cmdstart];
              if (mode_configuration.little_endian == YES) {
                sendw = bp_reverse_integer(sendw, mode_configuration.numbits);
              }
              enabled_protocols[bus_pirate_configuration.bus_mode].send(sendw);
            }
            cmdstart &= CMDLENMSK;
            user_serial_transmit_character('"');
            bpBR;
          }
          break;
        case '[':
          enabled_protocols[bus_pirate_configuration.bus_mode].start();
          break;

        case '{':
          enabled_protocols[bus_pirate_configuration.bus_mode].start_with_read();
          break;

        case ']':
          enabled_protocols[bus_pirate_configuration.bus_mode].stop();
          break;

        case '}':
          enabled_protocols[bus_pirate_configuration.bus_mode].stop_from_read();
          break;

        case '0':
        case '1':
        case '2':
        case '3':
        case '4':
        case '5':
        case '6':
        case '7':
        case '8':
        case '9':
          BPMSG1101;
          sendw = getint();
          cmdstart--;
          cmdstart &= CMDLENMSK;
          repeat = getrepeat() + 1;
          numbits = getnumbits();
          if (numbits) {
            mode_configuration.numbits = numbits;
            mode_configuration.int16 = numbits > 8 ? YES : NO;
          }
          while (--repeat) {
            bp_write_formatted_integer(sendw);
            if (((mode_configuration.int16 == NO) &&
                 (mode_configuration.numbits != 8)) ||
                ((mode_configuration.int16 == YES) &&
                 (mode_configuration.numbits != 16))) {
              user_serial_transmit_character(';');
              bp_write_dec_byte(mode_configuration.numbits);
            }
            if (mode_configuration.little_endian == YES) {
              sendw = bp_reverse_integer(sendw, mode_configuration.numbits);
            }
            received =
              enabled_protocols[bus_pirate_configuration.bus_mode].send(sendw);
            bpSP;
            if (mode_configuration.write_with_read) {
              MSG_READ_HEADER;
              if (mode_configuration.little_endian == YES) {
                received =
                  bp_reverse_integer(received, mode_configuration.numbits);
              }
              bp_write_formatted_integer(received);
              bpSP;
            }
          }
          bpBR;
          break;

        case 'r':
          MSG_READ_HEADER;
          newDmode = change_read_display();
          repeat = getrepeat() + 1;
          numbits = getnumbits();
          if (numbits) {
            mode_configuration.numbits = numbits;
            mode_configuration.int16 = (numbits > 8) ? YES : NO;
          }
          if (newDmode) {
            oldDmode = bus_pirate_configuration.display_mode;
            bus_pirate_configuration.display_mode = newDmode - 1;
          }
          while (--repeat) {
            received =
              enabled_protocols[bus_pirate_configuration.bus_mode].read();
            if (mode_configuration.little_endian == YES) {
              received = bp_reverse_integer(received, mode_configuration.numbits);
            }
            bp_write_formatted_integer(received);
            if (((mode_configuration.int16 == NO) &&
                 (mode_configuration.numbits != 8)) ||
                ((mode_configuration.int16 == YES) &&
                 (mode_configuration.numbits != 16))) {
              user_serial_transmit_character(';');
              bp_write_dec_byte(mode_configuration.numbits);
            }
            bpSP;
          }
          if (newDmode) {
            bus_pirate_configuration.display_mode = oldDmode;
            newDmode = 0;
          }
          bpBR;
          break;

        case '/':
          BPMSG1103;
          enabled_protocols[bus_pirate_configuration.bus_mode].clock_high();
          break;

        case '\\':
          BPMSG1104;
          enabled_protocols[bus_pirate_configuration.bus_mode].clock_low();
          break;

        case '-':
          BPMSG1105;
          enabled_protocols[bus_pirate_configuration.bus_mode].data_high();
          break;

        case '_':
          BPMSG1106;
          enabled_protocols[bus_pirate_configuration.bus_mode].data_low();
          break;

        case '.':
          BPMSG1098;
          echo_state(
            enabled_protocols[bus_pirate_configuration.bus_mode].data_state());
          break;

        case '^':
          repeat = getrepeat();
          BPMSG1108;
          bp_write_formatted_integer(repeat);
          repeat++;
          while (--repeat) {
            enabled_protocols[bus_pirate_configuration.bus_mode].clock_pulse();
          }
          bpBR;
          break;

        case '!':
          repeat = getrepeat() + 1;
          BPMSG1109;
          while (--repeat) {
            echo_state(
              enabled_protocols[bus_pirate_configuration.bus_mode].read_bit());
            bpSP;
          }
          BPMSG1107;
          break;

        case ASCII_NUL:
        case ASCII_CR:
        case ASCII_LF:
        case ' ':
        case ',':
          break;

        default:
          mode_configuration.command_error = YES;
          break;
      }

      cmdstart = (cmdstart + 1) & CMDLENMSK;

      if (mode_configuration.command_error == YES) {
        BPMSG1110;
        if (cmdstart > oldstart) {
          bp_write_dec_byte(cmdstart - oldstart);
        } else {
          bp_write_dec_byte((BP_COMMAND_BUFFER_SIZE + cmdstart) - oldstart);
        }
        mode_configuration.command_error = NO;
        stop = YES;
        bpBR;
      }

      if (cmdstart == cmdend) {
        stop = YES;
      }
    }

    cmdstart = newstart;
    cmdend = newstart;
    menu_state.command_present = NO;
  }
}

void consumewhitechars(void) {
  while (cmdbuf[cmdstart] == 0x20) {
    cmdstart = (cmdstart + 1) & CMDLENMSK;
  }
}

uint8_t change_read_display(void) {
  size_t index;

  for (index = 0; index < sizeof(READ_DISPLAY_BASE); index++) {
    if (cmdbuf[(cmdstart + 1) & CMDLENMSK] == READ_DISPLAY_BASE[index]) {
      return index + 1;
    }
  }

  return 0;
}

void handle_delete(void) {
  if (menu_state.cursor_position == cmdstart) {
    /* Cursor at the beginning of the line - nothing to handle. */
    user_serial_transmit_character(BELL);
    return;
  }

  /* Cursor in the middle of the line. */
  /* Remove characters. */
  remove_current_character_from_command_line();
}

void handle_backspace(void) {
  if (menu_state.cursor_position == cmdstart) {
    /* Cursor at the beginning of the line - nothing to handle. */
    user_serial_transmit_character(BELL);
    return;
  }

  if (menu_state.cursor_position == cmdend) {
    /* Cursor at the end of the line - remove item. */

    /* Update pointer. */
    cmdend = (cmdend - 1) & CMDLENMSK;

    /* Clear buffer segment. */
    cmdbuf[cmdend] = 0x00;

    /* Update cursor position. */
    menu_state.cursor_position = cmdend;

    /* Move remote cursor. */
    MSG_DESTRUCTIVE_BACKSPACE;

    return;
  }

  /* Cursor in the middle of the line. */

  /* Move remote cursor. */
  MSG_CURSOR_LEFT;

  /* Update current pointer. */
  menu_state.cursor_position = (menu_state.cursor_position - 1) & CMDLENMSK;

  /* Remove characters. */
  remove_current_character_from_command_line();
}

void convert_value(const bool reversed) {
  cmdstart = (cmdstart + 1) & CMDLENMSK;
  consumewhitechars();
  int value = getint();
  if (reversed) {
    value = bp_reverse_integer(value, mode_configuration.numbits);
  }
  bp_write_hex_byte(value);
  MSG_BASE_CONVERTER_EQUAL_SIGN;
  bp_write_dec_byte(value);
  MSG_BASE_CONVERTER_EQUAL_SIGN;
  bp_write_bin_byte(value);
  bpBR;
}

void remove_current_character_from_command_line(void) {
  uint16_t characters_to_move = 0;

  for (size_t index = menu_state.cursor_position; index != cmdend;
       index = (index + 1) & CMDLENMSK) {
    /* Move right-hand character over their left-hand counterpart. */
    cmdbuf[index] = cmdbuf[index + 1];

    /* Write the moved character. */
    user_serial_transmit_character(cmdbuf[index] != NULL ? cmdbuf[index] : ' ');

    /* Update pointer. */
    characters_to_move++;
  }

  /* Move the buffer end to its new position. */
  cmdend = (cmdend - 1) & CMDLENMSK;

  /* Move remote cursor. */
  bp_write_string("\x1B[");
  bp_write_dec_word(characters_to_move);
  bp_write_string("D");
}

void handle_left_arrow(void) {
  if (menu_state.cursor_position != cmdstart) {
    menu_state.cursor_position = (menu_state.cursor_position - 1) & CMDLENMSK;
    MSG_CURSOR_LEFT;
  } else {
    user_serial_transmit_character(BELL);
  }
}

void handle_right_arrow(void) {
  if (menu_state.cursor_position == cmdend) {
    user_serial_transmit_character(BELL);
    return;
  }
  menu_state.cursor_position = (menu_state.cursor_position + 1) & CMDLENMSK;
  MSG_CURSOR_RIGHT;
}

void refresh_mode_prompt(void) {
  /* Clear line and carriage return. */
  MSG_CLEAR_LINE_WITH_CR;

  /* Write mode header. */
  bp_write_string(enabled_protocols[bus_pirate_configuration.bus_mode].name);
  bp_write_string(">");
}

void refresh_command_line(const uint16_t start) {
  /* Clear current command line. */
  if (cmdstart != cmdend) {
    while (cmdend != cmdstart) {
      cmdbuf[cmdend] = 0x00;
      cmdend = (cmdend - 1) & CMDLENMSK;
    }

    cmdbuf[cmdend] = 0x00;
  }

  uint16_t repeat = (start - 1) & CMDLENMSK;
  uint16_t old_command_start = 0;
  while (repeat != cmdend) {
    if (!cmdbuf[repeat]) {
      old_command_start = (repeat + 1) & CMDLENMSK;
      break;
    }
    repeat = (repeat - 1) & CMDLENMSK;
  }

  refresh_mode_prompt();

  /* Write old command line. */
  for (repeat = old_command_start; repeat != start;
       repeat = (repeat + 1) & CMDLENMSK) {
    user_serial_transmit_character(cmdbuf[repeat]);
    cmdbuf[cmdend] = cmdbuf[repeat];
    cmdend = (cmdend + 1) & CMDLENMSK;
  }
  cmdbuf[cmdend] = 0x00;

  menu_state.cursor_position = cmdend;
}

void handle_up_arrow(void) {
  uint16_t history_entry_index = 0;
  uint16_t character_offset;

  for (character_offset = (cmdstart - 1) & CMDLENMSK;
       character_offset != cmdend;
       character_offset = (character_offset - 1) & CMDLENMSK) {

    if (!cmdbuf[character_offset] &&
        cmdbuf[(character_offset - 1) & CMDLENMSK]) {

      /* Found an earlier history entry in the buffer. */

      history_entry_index++;
      if (history_entry_index > menu_state.history_entry_counter) {
        menu_state.history_entry_counter++;

        refresh_command_line(character_offset);
      }
    }
  }

  /* First entry in the history list? */
  if (character_offset == cmdend) {
    user_serial_transmit_character(BELL);
  }
}

void handle_down_arrow(void) {
  uint16_t history_entry_index = 0;
  uint16_t character_offset;

  for (character_offset = (cmdstart - 1) & CMDLENMSK;
       character_offset != cmdend;
       character_offset = (character_offset - 1) & CMDLENMSK) {
    if (!cmdbuf[character_offset] &&
        cmdbuf[(character_offset - 1) & CMDLENMSK]) {

      /* Found an earlier history entry in the buffer. */

      history_entry_index++;
      if (history_entry_index == (menu_state.history_entry_counter - 1)) {
        menu_state.history_entry_counter--;
        refresh_command_line(character_offset);
      }
    }
  }

  if (character_offset != cmdend) {
    return;
  }

  if (menu_state.history_entry_counter != 1) {
    /* Top entry, send a beep. */
    user_serial_transmit_character(BELL);
    return;
  }

  refresh_mode_prompt();

  /* Clear rest of buffer. */

  while (cmdend != cmdstart) {
    cmdbuf[cmdend] = 0x00;
    cmdend = (cmdend - 1) & CMDLENMSK;
  }
  cmdbuf[cmdend] = 0x00;
  menu_state.cursor_position = cmdend;
  menu_state.history_entry_counter = 0;
}

void handle_home_key(void) {
  if (menu_state.cursor_position == cmdstart) {
    user_serial_transmit_character(BELL);
  }

  uint8_t repeat = (menu_state.cursor_position - cmdstart) & CMDLENMSK;
  bp_write_string("\x1B[");
  bp_write_dec_word(repeat);
  bp_write_string("D");
  menu_state.cursor_position = cmdstart;
}

void handle_end_key(void) {
  if (menu_state.cursor_position == cmdend) {
    user_serial_transmit_character(BELL);
  }

  uint8_t repeat = (cmdend - menu_state.cursor_position) & CMDLENMSK;
  bp_write_string("\x1B[");
  bp_write_dec_word(repeat);
  bp_write_string("C");
  menu_state.cursor_position = cmdend;
}

void handle_escape_key(void) {
  if (user_serial_read_byte() != '[') {
    return;
  }

  switch (user_serial_read_byte()) {
    case 'D':
      handle_left_arrow();
      break;

    case 'C':
      handle_right_arrow();
      break;

    case 'A':
      handle_up_arrow();
      break;

    case 'B':
      handle_down_arrow();
      break;

    case '1':
      if (user_serial_read_byte() == '~') {
        handle_home_key();
      }
      break;

    case '4':
      if (user_serial_read_byte() == '~') {
        handle_end_key();
      }
      break;

    default:
      break;
  }
}

void handle_enter_key(void) {
  menu_state.command_present = YES;
  menu_state.history_entry_counter = 0;
  cmdbuf[cmdend] = 0x00;
  cmdend = (cmdend + 1) & CMDLENMSK;
  menu_state.cursor_position = cmdend;
  bpBR;
}

void handle_character(const uint8_t character) {
  if ((((cmdend + 1) & CMDLENMSK) != cmdstart) && (character >= 0x20) &&
      (character < 0x7F)) {

    if (cmdend == menu_state.cursor_position) {
      /* Append character. */
      user_serial_transmit_character(character);
      cmdbuf[cmdend] = character;
      cmdend = (cmdend + 1) & CMDLENMSK;
      cmdbuf[cmdend] = 0x00;
      menu_state.cursor_position = cmdend;
    } else {
      /* Insert character. */
      size_t repeat = (cmdend - menu_state.cursor_position) & CMDLENMSK;
      bp_write_string("\x1B[");
      bp_write_dec_word(repeat);
      bp_write_string("C");
      uint16_t offset = cmdend;
      while (offset != ((menu_state.cursor_position - 1) & CMDLENMSK)) {
        cmdbuf[offset + 1] = cmdbuf[offset];
        if (cmdbuf[offset]) {
          user_serial_transmit_character(cmdbuf[offset]);
          MSG_CURSOR_LEFT_TWO;
        }
        offset = (offset - 1) & CMDLENMSK;
      }
      user_serial_transmit_character(character);
      cmdbuf[menu_state.cursor_position] = character;
      menu_state.cursor_position = (menu_state.cursor_position + 1) & CMDLENMSK;
      cmdend = (cmdend + 1) & CMDLENMSK;
    }
  } else {
    user_serial_transmit_character(BELL);
  }
}

void print_version_info(void) {
  bp_write_string(BP_VERSION_STRING);
  user_serial_transmit_character('.');
  user_serial_transmit_character(bus_pirate_configuration.hardware_version);
  bpBR;

  bp_write_string(BP_FIRMWARE_STRING);

  user_serial_transmit_character('[');
  for (size_t index = 0; index < ENABLED_PROTOCOLS_COUNT; index++) {
    if (index) {
      bpSP;
    }
    bp_write_string(enabled_protocols[index].name);
  }
  user_serial_transmit_character(']');

  // Bootloader not here!
  bpBR;

  BPMSG1117;
  bp_write_hex_word(bus_pirate_configuration.device_type);

  BPMSG1210;
  bp_write_hex_word(bus_pirate_configuration.device_revision);
  MSG_CHIP_REVISION_ID;

  switch (bus_pirate_configuration.device_revision) {
    default:
      MSG_CHIP_REVISION_UNKNOWN;
      break;
  }

  bp_write_line(")");
  BPMSG1118;
}

void print_status_info(void) {
  /*BPMSG1136;
    bp_write_hex_word(bp_read_from_flash(CFG_ADDR_UPPER, CFG_ADDR_1));
    BPMSG1137;
    bp_write_hex_word(bp_read_from_flash(CFG_ADDR_UPPER, CFG_ADDR_2));
    bpBR;*/

  BPMSG1119;

  print_pins_information();

  if (bp_get_voltage_regulator_state()) {
    MSG_POWER_SUPPLIES_ON;
  } else {
    MSG_POWER_SUPPLIES_OFF;
  }

  user_serial_transmit_character(',');
  bpSP;

  // pullups available, enabled?
  // was modeConfig.pullupEN
  if (BP_PULLUP == 1)
    BPMSG1091;
  else
    BPMSG1089;
  user_serial_transmit_character(',');
  bpSP;

  // open collector outputs?
  if (mode_configuration.high_impedance == YES) {
    BPMSG1120;
  } else {
    BPMSG1121;
  }

  // bitorder toggle available, enabled
  if (mode_configuration.little_endian == NO) {
    BPMSG1123;
  } else {
    BPMSG1124;
  }

  user_serial_transmit_character(',');
  bpSP;

  // show partial writes
  // bpWline("Number of bits read/write: ");
  BPMSG1252;
  bp_write_dec_byte(mode_configuration.numbits);
  bpBR;

  // AUX pin setting
  if (mode_configuration.alternate_aux == 1) {
    BPMSG1087;
  } else {
    BPMSG1086;
  }

  enabled_protocols[bus_pirate_configuration.bus_mode].print_settings();
  BPMSG1119;
}

void print_pins_information(void) {
  BPMSG1226;
  BPMSG1233; // bpWstring("1.(BR)\t2.(RD)\t3.(OR)\t4.(YW)\t5.(GN)\t6.(BL)\t7.(PU)\t8.(GR)\t9.(WT)\t0.(BLK)");
  BPMSG1227; // bpWstring("GND\t3.3V\t5.0V\tADC\tVPU\tAUX\t");

  enabled_protocols[bus_pirate_configuration.bus_mode].print_pins_state();
  BPMSG1228;

  print_pin_direction(AUX);
  print_pin_direction(CLK);
  print_pin_direction(MOSI);
  print_pin_direction(CS);
  print_pin_direction(MISO);
  bpBR;
  BPMSG1234;
  /*bp_enable_adc();

    #if defined(BP_VERSION2_SUPPORT) && (BP_VERSION2_SUPPORT == 1)
    bp_write_voltage(bp_read_adc(BP_ADC_PROBE));
    #else
    bp_write_voltage(bp_read_adc(BP_ADC_3V3));
    #endif
    MSG_VOLTAGE_UNIT;
    user_serial_transmit_character('\t');

    bp_write_voltage(bp_read_adc(BP_ADC_5V0));
    MSG_VOLTAGE_UNIT;
    user_serial_transmit_character('\t');

    #if defined(BP_VERSION2_SUPPORT) && (BP_VERSION2_SUPPORT == 1)
    bp_write_voltage(bp_read_adc(BP_ADC_3V3));
    #else
    bp_write_voltage(bp_read_adc(BP_ADC_PROBE));
    #endif
    MSG_VOLTAGE_UNIT;
    user_serial_transmit_character('\t');

    bp_write_voltage(bp_read_adc(BP_ADC_VPU));
    MSG_VOLTAGE_UNIT;
    user_serial_transmit_character('\t');

    bp_disable_adc();*/

  print_pin_state(AUX);
  print_pin_state(CLK);
  print_pin_state(MOSI);
  print_pin_state(CS);
  print_pin_state(MISO);
  bpBR;
}

void print_pin_direction(const uint16_t pin) {
  bp_write_string(IODIR & pin ? "O\t" : "I\t");
}

void print_pin_state(const uint16_t pin) {
  bp_write_string(IOPOR & pin ? "H\t" : "L\t");
}

void set_display_mode(void) {
  cmdstart = (cmdstart + 1) & CMDLENMSK;
  consumewhitechars();
  int mode = getint();

  if ((mode > 0) && (mode <= 4)) {
    bus_pirate_configuration.display_mode = mode - 1;
  } else {
    mode_configuration.command_error = NO;
    BPMSG1127;
    bus_pirate_configuration.display_mode = getnumber(1, 1, 4, 0) - 1;
  }
  BPMSG1128;
}

void echo_state(const uint16_t value) {
  user_serial_transmit_character(value ? '1' : '0');
}

void changemode(void) {
  int i, busmode;

  busmode = 0;
  cmdstart = (cmdstart + 1) & CMDLENMSK;
  consumewhitechars();
  busmode = getint();

  if (!busmode) // no argument entered
  {
    for (i = 0; i < ENABLED_PROTOCOLS_COUNT; i++) {
      bp_write_dec_byte(i + 1);
      bp_write_string(". ");
      bp_write_line(enabled_protocols[i].name);
    }
    // bpWline("x. exit(without change)");
    BPMSG1111;
    mode_configuration.command_error =
      NO; // error is set because no number found, but it is no
    // error here:S eeeh confusing right?
    busmode = getnumber(1, 1, ENABLED_PROTOCOLS_COUNT, 1) - 1;
    if ((busmode == -2) || (busmode == -1)) {
      // bpWline("no mode change");
      BPMSG1112;
    } else {
      enabled_protocols[bus_pirate_configuration.bus_mode].cleanup();
      bp_reset_board_state();
      bus_pirate_configuration.bus_mode = busmode;
      enabled_protocols[bus_pirate_configuration.bus_mode].setup_prepare();
      MSG_CLUTCH_DISENGAGED;
      if (busmode) {
        //BP_LEDMODE = 1; // mode led is on when proto >0
        MSG_FINISH_SETUP_PROMPT;
      }
      // bpWmessage(MSG_READY);
      BPMSG1085;
    }
  } else // number entered
  {
    busmode--; // save a couple of programwords to do it here :D
    if (busmode < ENABLED_PROTOCOLS_COUNT) {
      enabled_protocols[bus_pirate_configuration.bus_mode].cleanup();
      bp_reset_board_state();
      bus_pirate_configuration.bus_mode = busmode;
      enabled_protocols[bus_pirate_configuration.bus_mode].setup_prepare();
      if (busmode)
        //BP_LEDMODE = 1; // mode led is on when proto >0
        // bpWmessage(MSG_READY);
        BPMSG1085;
    } else { // bpWline("Nonexistent protocol!!");
      BPMSG1114;
    }
  }
  cmdstart = (cmdend - 1) & CMDLENMSK;
}

int getint(void) // get int from user (accept decimal, hex (0x) or binairy (0b)
{
  int i;
  int number;

  i = 0;
  number = 0;

  if ((cmdbuf[((cmdstart + i) & CMDLENMSK)] >= 0x31) &&
      (cmdbuf[((cmdstart + i) & CMDLENMSK)] <= 0x39)) // 1-9 is for sure decimal
  {
    number = cmdbuf[(cmdstart + i) & CMDLENMSK] - 0x30;
    i++;
    while ((cmdbuf[((cmdstart + i) & CMDLENMSK)] >= 0x30) &&
           (cmdbuf[((cmdstart + i) & CMDLENMSK)] <= 0x39)) // consume all digits
    {
      number *= 10;
      number += cmdbuf[((cmdstart + i) & CMDLENMSK)] - 0x30;
      i++;
    }
  } else if (cmdbuf[((cmdstart + i) & CMDLENMSK)] ==
             0x30) // 0 could be anything
  {
    i++;
    if ((cmdbuf[((cmdstart + i) & CMDLENMSK)] == 'b') ||
        (cmdbuf[((cmdstart + i) & CMDLENMSK)] == 'B')) {
      i++;
      while ((cmdbuf[((cmdstart + i) & CMDLENMSK)] == '1') ||
             (cmdbuf[((cmdstart + i) & CMDLENMSK)] == '0')) {
        number <<= 1;
        number += cmdbuf[((cmdstart + i) & CMDLENMSK)] - 0x30;
        i++;
      }
    } else if ((cmdbuf[((cmdstart + i) & CMDLENMSK)] == 'x') ||
               (cmdbuf[((cmdstart + i) & CMDLENMSK)] == 'X')) {
      i++;
      while (((cmdbuf[((cmdstart + i) & CMDLENMSK)] >= 0x30) &&
              (cmdbuf[((cmdstart + i) & CMDLENMSK)] <= 0x39)) ||
             ((cmdbuf[((cmdstart + i) & CMDLENMSK)] >= 'A') &&
              (cmdbuf[((cmdstart + i) & CMDLENMSK)] <= 'F')) ||
             ((cmdbuf[((cmdstart + i) & CMDLENMSK)] >= 'a') &&
              (cmdbuf[((cmdstart + i) & CMDLENMSK)] <= 'f'))) {
        number <<= 4;
        if ((cmdbuf[(cmdstart + i) & CMDLENMSK] >= 0x30) &&
            (cmdbuf[((cmdstart + i) & CMDLENMSK)] <= 0x39)) {
          number += cmdbuf[((cmdstart + i) & CMDLENMSK)] - 0x30;
        } else {
          cmdbuf[((cmdstart + i) & CMDLENMSK)] |= 0x20; // make it lowercase
          number +=
            cmdbuf[((cmdstart + i) & CMDLENMSK)] - 0x57; // 'a'-0x61+0x0a
        }
        i++;
      }
    } else if ((cmdbuf[((cmdstart + i) & CMDLENMSK)] >= 0x30) &&
               (cmdbuf[((cmdstart + i) & CMDLENMSK)] <= 0x39)) {
      number = cmdbuf[((cmdstart + i) & CMDLENMSK)] - 0x30;
      while (
        (cmdbuf[((cmdstart + i) & CMDLENMSK)] >= 0x30) &&
        (cmdbuf[((cmdstart + i) & CMDLENMSK)] <= 0x39)) // consume all digits
      {
        number *= 10;
        number += cmdbuf[((cmdstart + i) & CMDLENMSK)] - 0x30;
        i++;
      }
    }
  } else // how did we come here??
  {
    mode_configuration.command_error = YES;
    return 0;
  }

  cmdstart += i; // we used i chars
  cmdstart &= CMDLENMSK;
  return number;
} // getint(void)

int getrepeat(void) {
  int temp;

  if (cmdbuf[(cmdstart + 1) & CMDLENMSK] == ':') {
    cmdstart += 2;
    cmdstart &= CMDLENMSK;
    temp = getint();
    cmdstart--; // to allow [6:10] (start send 6 10 times stop)
    cmdstart &= CMDLENMSK;
    return temp;
  }
  return 1; // no repeat count=1
} // getrepeat

int getnumbits(void) {
  int temp;

  if (cmdbuf[(cmdstart + 1) & CMDLENMSK] == ';') {
    cmdstart = (cmdstart + 2) & CMDLENMSK;
    temp = getint();
    cmdstart = (cmdstart - 1) &
               CMDLENMSK; // to allow [6:10] (start send 6 10 times stop)
    return temp;
  }
  return 0; // no numbits=0;
} // getnumbits

// gets number from input
// -1 = abort (x)
// -2 = input to much
// 0-max return
// x=1 exit is enabled (we don't want that in the mode changes ;)

int getnumber(int def, int min, int max,
              int x) // default, minimum, maximum, show exit option
{
  char c;
  char buf[6]; // max 4 digits;
  int i, j, stop, temp, neg;

again: // need to do it proper with whiles and ifs..

  i = 0;
  stop = 0;
  temp = 0;
  neg = 0;

  bp_write_string("\r\n(");
  if (def < 0) {
    bp_write_string("x");
  } else {
    bp_write_dec_byte(def);
  }
  bp_write_string(")>");

  while (!stop) {
    c = user_serial_read_byte();
    switch (c) {
      case 0x08:
        if (i) {
          i--;
          MSG_DESTRUCTIVE_BACKSPACE;
        } else {
          if (neg) {
            neg = 0;
            MSG_DESTRUCTIVE_BACKSPACE;
          } else {
            user_serial_transmit_character(BELL);
          }
        }
        break;
      case 0x0A:
      case 0x0D:
        stop = 1;
        break;
      case '-':
        if (!i) // enable negative numbers
        {
          user_serial_transmit_character('-');
          neg = 1;
        } else {
          user_serial_transmit_character(BELL);
        }
        break;
      case 'x':
        if (x)
          return -1; // user wants to quit :( only if we enable it :D
      default:
        if ((c >= 0x30) && (c <= 0x39)) // we got a digit
        {
          if (i > 3) // 0-9999 should be enough??
          {
            user_serial_transmit_character(BELL);
            i = 4;
          } else {
            user_serial_transmit_character(c);
            buf[i] = c; // store user input
            i++;
          }
        } else // ignore input :)
        {
          user_serial_transmit_character(BELL);
        }
    }
  }
  bpBR;

  if (i == 0) {
    return def; // no user input so return default option
  } else {
    temp = 0;
    i--;
    j = i;

    for (; i >= 0; i--) {
      temp *= 10;
      temp += (buf[j - i] - 0x30);
    }

    if ((temp >= min) && (temp <= max)) {
      if (neg) {
        return -temp;
      } else {
        return temp;
      }
    } else { // bpWline("\r\nInvalid choice, try again\r\n");
      BPMSG1211;
      goto again;
    }
  }
  return temp; // we dont get here, but keep compiler happy
}

void switch_psu_on(void) {
  if (bus_pirate_configuration.bus_mode == BP_HIZ) {
    BPMSG1088;
    return;
  }

  bp_enable_voltage_regulator();
  bp_message_write_line("VREG on");
  //bp_enable_adc();

  /* Wait for VREG to come up.*/
  //bp_delay_ms(2);

  // Don't read ADC, assume it worked correctly
  //if ((bp_read_adc(BP_ADC_3V3) > V33L) && (bp_read_adc(BP_ADC_5V0) > V5L)) {
  if (1) {
    /* Correct voltages read. */
    MSG_POWER_SUPPLIES_ON;
    bpBR;
    enabled_protocols[bus_pirate_configuration.bus_mode].setup_execute();
    MSG_CLUTCH_ENGAGED;
  } else {
    bp_disable_voltage_regulator();
    MSG_VREG_TOO_LOW;
    MSG_POWER_SUPPLIES_OFF;
    bpBR;
  }

  //bp_disable_adc();
}

void switch_psu_off(void) {
  if (bus_pirate_configuration.bus_mode == BP_HIZ) {
    BPMSG1088;
    return;
  }

  enabled_protocols[bus_pirate_configuration.bus_mode].cleanup();
  MSG_CLUTCH_DISENGAGED;
  bp_disable_voltage_regulator();
  MSG_POWER_SUPPLIES_OFF;
  bpBR;
}
