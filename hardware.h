/* 
 * File:   hardware.h
 * Author: reyes
 *
 * Created on May 3, 2021, 11:37 AM
 */

#include "types.h"
#include "regmap.h"

#ifndef HARDWARE_H
#define	HARDWARE_H

#ifdef	__cplusplus
extern "C" {
#endif

#define __nop() do { __asm__ __volatile__ ("nop"); } while (0)

    // this is a different type of pin identifier used by the bitbang library
#define AUX  0b01000000 // PD6
#define MOSI 0b00000100 // PD2
#define CLK  0b00001000 // PD3
#define MISO 0b00010000 // PD4
#define CS   0b00100000 // PD5
#define IOPOR PIND
#define IOLAT PORTD
#define IODIR DDRD

    // these macros set the pin IO used in each module
#define BP_MOSI PORTDbits.pd2
#define BP_CLK PORTDbits.pd3
#define BP_MISO PORTDbits.pd4
#define BP_CS PORTDbits.pd5
#define BP_CS_PIN PINDbits.pind5
#define BP_AUX0 PORTDbits.pd6   // moved pin5 (AUX) to RC1
#define BP_AUX0_PIN PINDbits.pind6   // moved pin5 (AUX) to RC1
#define BP_LEDMODE PORTCbits.pc0 // single MODE led on v2a
#define BP_VREGEN PORTCbits.pc1  // single vreg enable pin on v2a
#define BP_PULLUP PORTCbits.pc2 // Pull-up pin on V2a
//#define BP_PGD PORTCbits.pc3    // PGD pin on programming header

    // these macros set the direction registers for each IO pin
#define BP_MOSI_DIR DDRDbits.ddd2
#define BP_CLK_DIR DDRDbits.ddd3
#define BP_MISO_DIR DDRDbits.ddd4
#define BP_CS_DIR DDRDbits.ddd5
#define BP_AUX0_DIR DDRDbits.ddd6
#define BP_LEDMODE_DIR DDRCbits.ddc0
#define BP_VREGEN_DIR DDRCbits.ddc1
#define BP_PULLUP_DIR DDRCbits.ddc2
//#define BP_PGD_DIR DDRCbits.ddc3

    static inline void bp_enable_usb_led(void) {
    }

    static inline void bp_disable_usb_led(void) {
    }

    static inline void bp_toggle_usb_led(void) {
    }

    static inline void bp_enable_mode_led(void) {
        BP_LEDMODE_DIR = OUTPUT;
        BP_LEDMODE = ON;
    }

    static inline void bp_disable_mode_led(void) {
        BP_LEDMODE_DIR = OUTPUT;
        BP_LEDMODE = OFF;
    }

    static inline void bp_set_mode_led_state(const bool state) {
        if (state == ON) {
            bp_enable_mode_led();
        } else {
            bp_disable_mode_led();
        }
    }

    static inline void bp_toggle_mode_led(void) {
        BP_LEDMODE_DIR = OUTPUT;
        BP_LEDMODE ^= ON;
    }

    static inline void bp_enable_pullup(void) {
        BP_PULLUP_DIR = INPUT;
        BP_PULLUP = LOW;
    }

    static inline void bp_disable_pullup(void) {
        BP_PULLUP = LOW;
        BP_PULLUP_DIR = OUTPUT;
    }

    static inline void bp_set_pullup_state(bool state) {
        if (state == ON) {
            bp_enable_pullup();
        } else {
            bp_disable_pullup();
        }
    }

    static inline void bp_enable_voltage_regulator(void) {
        BP_VREGEN_DIR = OUTPUT;
        BP_VREGEN = ON;
    }

    static inline void bp_disable_voltage_regulator(void) {
        BP_VREGEN_DIR = OUTPUT;
        BP_VREGEN = OFF;
    }

    static inline bool bp_get_voltage_regulator_state(void) {
        return BP_VREGEN;
    }

    static inline void bp_set_voltage_regulator_state(bool state) {
        if (state == ON) {
            bp_enable_voltage_regulator();
        } else {
            bp_disable_voltage_regulator();
        }
    }


#ifdef	__cplusplus
}
#endif

#endif	/* HARDWARE_H */
