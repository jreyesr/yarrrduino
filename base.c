#include <stdint.h>

#include "core.h"
#include "base.h"
#include "hardware.h"
#include "messages.h"

/**
 * @brief Prefix string for hexadecimal values in human-readable form.
 */
static const uint8_t HEX_PREFIX[] = {'0', 'x'};

/**
 * @brief Look-up table for hexadecimal to ASCII transformations.
 */
static const unsigned char HEX_ASCII_TABLE[] = {'0', '1', '2', '3', '4', '5',
    '6', '7', '8', '9', 'A', 'B',
    'C', 'D', 'E', 'F'};

extern bus_pirate_configuration_t bus_pirate_configuration;
extern mode_configuration_t mode_configuration;

/**
 * @brief Clear configuration on mode change.
 */
static void clear_mode_configuration(void);

/**
 * @brief Prints the given value to the serial port as a decimal number.
 *
 * @warning the denominator *must* be equal to 10^digits.
 *
 * @param[in] value the value to print.
 * @param[in] denominator the divisor to use when printing the value.
 * @param[in] digits how many digits to print.
 */
static void print_decimal(const uint32_t value, const uint32_t denominator,
        const uint8_t digits);

void clear_mode_configuration(void) {
    mode_configuration.high_impedance = OFF;
    mode_configuration.speed = 0;
    mode_configuration.periodicService = OFF;
    mode_configuration.alternate_aux = 0;
    mode_configuration.little_endian = NO;
}

uint16_t bp_reverse_integer(const uint16_t value, const uint8_t bits) {
    uint16_t reversed;
    uint16_t bitmask;

    reversed = 0;
    bitmask = 1;
    while (bitmask != 0) {
        reversed <<= 1;
        if (value & bitmask) {
            reversed |= 0b00000001;
        }
        bitmask <<= 1;
    }

    return reversed >> ((sizeof (uint16_t) * 8) - bits);
}

void bp_write_string(const char *string) {
    char character;
    while ((character = *string++)) {
        user_serial_transmit_character(character);
    }
}

void bp_write_line(const char *string) {
    bp_write_string(string);

    user_serial_transmit_character('\r');
    user_serial_transmit_character('\n');
}

void bp_write_formatted_integer(const uint16_t value) {
    uint16_t integer;

    integer = (mode_configuration.numbits < 16)
            ? MASKBOTTOM16(value, mode_configuration.numbits)
            : value;

    switch (bus_pirate_configuration.display_mode) {
        case DISPLAY_HEX:
            if (mode_configuration.int16 == YES) {
                bp_write_hex_word(value);
            } else {
                bp_write_hex_byte(value);
            }
            break;

        case DISPLAY_DEC:
            if (mode_configuration.int16 == YES) {
                bp_write_dec_word(value);
            } else {
                bp_write_dec_byte(value);
            }
            break;

        case DISPLAY_BIN:
            if (mode_configuration.int16 == YES) {
                bp_write_bin_byte(value >> 8);
                bpSP;
            }
            bp_write_bin_byte(value & 0xFF);
            break;

        case DISPLAY_RAW:
            if (mode_configuration.int16 == YES) {
                user_serial_transmit_character(value >> 8);
            }
            user_serial_transmit_character(value & 0xFF);
            break;
    }
}

void print_decimal(const uint32_t value, const uint32_t denominator,
        const uint8_t digits) {
    uint32_t number;
    uint32_t divisor;
    uint32_t current;
    uint8_t digit;
    bool first;

    first = false;
    number = value;
    divisor = denominator;

    if (!value) {
        user_serial_transmit_character('0');
        return;
    }

    for (digit = 0; digit < digits; digit++) {
        current = number / divisor;
        if (first || (current > 0)) {
            user_serial_transmit_character((uint8_t) current + '0');
            number -= (current * divisor);
            first = true;
        }

        divisor /= 10;
    }
}

void bp_write_dec_byte(const uint8_t value) {
    print_decimal(value, 100, 3);
}

void bp_write_dec_word(const uint16_t value) {
    print_decimal(value, 10000, 5);
}

void bp_write_hex_byte(const uint8_t value) {
    MSG_HEXADECIMAL_NUMBER_PREFIX;
    user_serial_transmit_character(HEX_ASCII_TABLE[(value >> 4) & 0x0F]);
    user_serial_transmit_character(HEX_ASCII_TABLE[value & 0x0F]);
}

void bp_write_hex_word(const uint16_t value) {
    MSG_HEXADECIMAL_NUMBER_PREFIX;
    user_serial_transmit_character(HEX_ASCII_TABLE[(value >> 12) & 0x0F]);
    user_serial_transmit_character(HEX_ASCII_TABLE[(value >> 8) & 0x0F]);
    user_serial_transmit_character(HEX_ASCII_TABLE[(value >> 4) & 0x0F]);
    user_serial_transmit_character(HEX_ASCII_TABLE[value & 0x0F]);
}

void bp_write_bin_byte(const uint8_t value) {
    uint8_t mask = 0x80;
    uint8_t index;

    MSG_BINARY_NUMBER_PREFIX;

    for (index = 0; index < 8; index++) {
        user_serial_transmit_character((value & mask) ? '1' : '0');
        mask >>= 1;
    }
}

uint8_t user_serial_read_byte(void) {
    while (!user_serial_ready_to_read());

    return UDR0;
}

void user_serial_transmit_character(const char character) {
    /* Do not transmit if the board should be quiet. */
    if (bus_pirate_configuration.quiet) {
        return;
    }

    /* Wait until transmission can take place. */
    while (!user_serial_ready_to_tx());

    UDR0 = character;
}

void user_serial_wait_transmission_done(void) {
    while (UCSR0Abits.txc0 == NO) ;
}

bool user_serial_transmit_done(void) {
    return UCSR0Abits.txc0;
}

bool user_serial_ready_to_tx(void) {
    return UCSR0Abits.udre0;
}

bool user_serial_ready_to_read(void) {
    return UCSR0Abits.rxc0;
}

bool user_serial_check_overflow(void) {
    return UCSR0Abits.dor0;
}

void user_serial_clear_overflow(void) {
    UCSR0Abits.dor0 = NO;
}

void bp_delay_ms(uint16_t milliseconds) {
    delay(milliseconds);
}

void bp_delay_us(uint16_t microseconds) {
    delayMicroseconds(microseconds);
}

void bp_reset_board_state(void) {
    BP_MOSI_DIR = INPUT;
    BP_CLK_DIR = INPUT;
    BP_MISO_DIR = INPUT;
    BP_CS_DIR = INPUT;
    BP_AUX0_DIR = INPUT;
    bp_disable_mode_led();
    bp_enable_usb_led();

    /* Detach source from the currently-set AUX pin. */
    //BP_AUX_RPOUT = OFF;

    bus_pirate_configuration.bus_mode = BP_HIZ;
    clear_mode_configuration();
    bp_disable_pullup();
    bp_disable_voltage_regulator();
}
