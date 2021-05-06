/* 
 * File:   raw_common.h
 * Author: reyes
 *
 * Created on May 5, 2021, 4:16 PM
 */

#ifndef RAW_COMMON_H
#define	RAW_COMMON_H

#include "configuration.h"

#ifdef	__cplusplus
extern "C" {
#endif

#if defined(BP_ENABLE_RAW_2WIRE_SUPPORT) || defined(BP_ENABLE_RAW_3WIRE_SUPPORT)

#include <stdbool.h>
#include <stdint.h>

#include "base.h"
#include "bitbang.h"

    static inline bool raw_read_bit(void) {
        return bitbang_read_bit();
    }

    static inline uint16_t raw_read_pin(void) {
        return bitbang_read_miso();
    }

    static inline void raw_set_clock_high(void) {
        bitbang_set_clk(HIGH);
    }

    static inline void raw_set_clock_low(void) {
        bitbang_set_clk(LOW);
    }

    static inline void raw_set_data_low(void) {
        bitbang_set_mosi(LOW);
    }

    static inline void raw_set_data_high(void) {
        bitbang_set_mosi(HIGH);
    }

    static inline void raw_advance_clock(void) {
        bitbang_advance_clock_ticks(1);
    }

#endif /* BP_ENABLE_RAW_2WIRE_SUPPORT || BP_ENABLE_RAW_3WIRE_SUPPORT */


#ifdef	__cplusplus
}
#endif

#endif	/* RAW_COMMON_H */

