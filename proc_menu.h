/* 
 * File:   proc_menu.h
 * Author: reyes
 *
 * Created on May 3, 2021, 11:43 AM
 */

#ifndef PROC_MENU_H
#define	PROC_MENU_H

#include "configuration.h"

#ifdef	__cplusplus
extern "C" {
#endif

#define BELL 0x07

#define CMDLENMSK (BP_COMMAND_BUFFER_SIZE - 1)

    extern char cmdbuf[BP_COMMAND_BUFFER_SIZE];
    extern unsigned int cmdend;
    extern unsigned int cmdstart;
    extern bool command_error;

    void serviceuser(void);

    void changemode(void);

    void print_version_info(void);

    void consumewhitechars(void);

    int getint(void);
    int getrepeat(void);
    int getnumbits(void);

    int getnumber(int def, int min, int max, int x);


#ifdef	__cplusplus
}
#endif

#endif	/* PROC_MENU_H */

