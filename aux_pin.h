/* 
 * File:   aux_pin.h
 * Author: reyes
 *
 * Created on May 5, 2021, 12:45 PM
 */

#ifndef AUX_PIN_H
#define	AUX_PIN_H

#include <stdbool.h>

#ifdef	__cplusplus
extern "C" {
#endif


    /**
     * @brief Sets the currently chosen AUX pin into INPUT/HiZ mode.
     */
    void bp_aux_pin_set_high_impedance(void);

    /**
     * @brief Pulls the currently chosen AUX pin HIGH.
     */
    void bp_aux_pin_set_high(void);

    /**
     * @brief Brings the currently chosen AUX pin LOW.
     */
    void bp_aux_pin_set_low(void);

    /**
     * @breif Reads the state of the currently chosen AUX pin.
     *
     * @return `HIGH` or `LOW` according to the chosen AUX pin state.
     */
    bool bp_aux_pin_read(void);

#ifdef	__cplusplus
}
#endif

#endif	/* AUX_PIN_H */

