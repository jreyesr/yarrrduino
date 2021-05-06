/* 
 * File:   raw2wire.h
 * Author: reyes
 *
 * Created on May 5, 2021, 4:15 PM
 */

#ifndef RAW2WIRE_H
#define	RAW2WIRE_H

#include "configuration.h"

#ifdef	__cplusplus
extern "C" {
#endif

#ifdef BP_ENABLE_RAW_2WIRE_SUPPORT

#include <stdbool.h>
#include <stdint.h>

void raw2wire_start(void);
void raw2wire_stop(void);
uint16_t raw2wire_write(const uint16_t value);
uint16_t raw2wire_read(void);
void raw2wire_run_macro(const uint16_t macro_id);
void raw2wire_setup_execute(void);
void raw2wire_setup_prepare(void);
void raw2wire_print_pins_state(void);
void raw2wire_print_settings(void);

#endif /* BP_ENABLE_RAW_2WIRE_SUPPORT */


#ifdef	__cplusplus
}
#endif

#endif	/* RAW2WIRE_H */

