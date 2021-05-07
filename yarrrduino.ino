//#include "mcc_generated_files/mcc.h"
#include "types.h"
#include "configuration.h"
#include "core.h"
#include "base.h"
#include "proc_menu.h"
#include <stdio.h>

void initialize_board(void);

/**
 * Internal terminal buffer area.
 */
static uint8_t bp_buffer[BP_TERMINAL_BUFFER_SIZE];

/**
 * Global configuration data holder.
 */
bus_pirate_configuration_t bus_pirate_configuration = {.terminal_input =
    bp_buffer};

/**
 * Mode-specific configuration data holder.
 */
mode_configuration_t mode_configuration;

/**
 * The last command being input by the user.
 */
command_t last_command;

/*
                         Main application
 */

int main(void) {
    initialize_board();

    serviceuser();

    return 0;
}

void initialize_board(void) {
    volatile unsigned long delay = 0xFFFF;

    while (delay--) {
    }

    bus_pirate_configuration.terminal_speed = 8;

    /* Default to print numbers in hexadecimal format. */
    bus_pirate_configuration.display_mode = DISPLAY_HEX;

    /* Start from a known, clear state. */
    bp_reset_board_state();

    // Not required, SYSTEM_Initialize() already does the work
    Serial.begin(115200);

    bus_pirate_configuration.device_type =
            0xDE;
    bus_pirate_configuration.device_revision =
            0xAD;

    /* Get the revision identifier. */
    bus_pirate_configuration.hardware_version 
=            //BPV3_HARDWARE_VERSION_TABLE[(PORTB >> 2) & 0b00000011];
            '?';

    bus_pirate_configuration.quiet = OFF;
    mode_configuration.numbits = 8;
}
