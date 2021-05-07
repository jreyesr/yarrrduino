/* 
 * File:   configuration.h
 * Author: reyes
 *
 * Created on May 3, 2021, 11:26 AM
 */

#ifndef CONFIGURATION_H
#define	CONFIGURATION_H

#ifdef	__cplusplus
extern "C" {
#endif

    /**
     * How big the serial command buffer can be, in bytes.
     *
     * @warning This must be set to a power of two, ie. 256, 128, 64, 32, etc.
     */
#define BP_COMMAND_BUFFER_SIZE 128


    /**
     * How big the serial terminal buffer can be, in bytes.
     *
     * @todo Make sure that no hard-coded buffer sizes are present.
     */
#define BP_TERMINAL_BUFFER_SIZE 128

#define BP_ENABLE_RAW_2WIRE_SUPPORT
#define BP_ENABLE_I2C_SUPPORT



#ifdef	__cplusplus
}
#endif

#endif	/* CONFIGURATION_H */
