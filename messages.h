/*
   File:   messages.h
   Author: reyes

   Created on May 3, 2021, 12:07 PM
*/

#ifndef MESSAGES_H
#define	MESSAGES_H

#include <avr/pgmspace.h>

#ifdef	__cplusplus
extern "C" {
#endif

void bp_message_write_buffer(char *strptr);
void bp_message_write_line(char *strptr);

void bp_message_write_buffer_progmem(const char *strptr);
void bp_message_write_line_progmem(const char *strptr);

#define F(string_literal) (PSTR(string_literal))

#define BP_VERSION_STRING ("Bus Pirate v3")

#define BPMSG1085 bp_message_write_line_progmem(F("Ready"))
#define BPMSG1093 bp_message_write_line_progmem(F("RESET"))
#define BPMSG1039 bp_message_write_buffer_progmem(F("AUX HI-Z"))
#define BPMSG1040 bp_message_write_line_progmem(F("AUX 1"))
#define BPMSG1041 bp_message_write_line_progmem(F("AUX 0"))
#define BPMSG1086 bp_message_write_line_progmem(F("a/A/@ controls AUX pin"))
#define BPMSG1087 bp_message_write_line_progmem(F("a/A/@ controls CS pin"))
#define BPMSG1088 bp_message_write_line_progmem(F("Command not used in this mode"))
#define BPMSG1089 bp_message_write_buffer_progmem(F("Pull-up resistors OFF"))
#define BPMSG1091 bp_message_write_buffer_progmem(F("Pull-up resistors ON"))
#define BPMSG1095 bp_message_write_buffer_progmem(F("AUX HI-Z, READ: "))
#define BPMSG1098 bp_message_write_buffer_progmem(F("DATA READ: "))
#define BPMSG1099 bp_message_write_buffer_progmem(F("DELAY "))
#define BPMSG1100 bp_message_write_line_progmem(F("us"))
#define BPMSG1101 bp_message_write_buffer_progmem(F("WRITE: "))
#define BPMSG1103 bp_message_write_line_progmem(F("CLK 1"))
#define BPMSG1104 bp_message_write_line_progmem(F("CLK 0"))
#define BPMSG1105 bp_message_write_line_progmem(F("DAT OUT 1"))
#define BPMSG1106 bp_message_write_line_progmem(F("DAT OUT 0"))
#define BPMSG1107 bp_message_write_line_progmem(F(" *pin is HiZ"))
#define BPMSG1108 bp_message_write_buffer_progmem(F("TICKS: "))
#define BPMSG1109 bp_message_write_buffer_progmem(F("READ BIT: "))
#define BPMSG1110 bp_message_write_buffer_progmem(F("Syntax error, char "))
#define BPMSG1111 bp_message_write_line_progmem(F("x. cancel"))
#define BPMSG1112 bp_message_write_line_progmem(F("No mode change"))
#define BPMSG1114 bp_message_write_line_progmem(F("Nonexistent protocol!"))
#define BPMSG1117 bp_message_write_buffer_progmem(F("DEVID:"))
#define BPMSG1118 bp_message_write_line_progmem(F("http://dangerousprototypes.com"))
#define BPMSG1119 bp_message_write_line_progmem(F("*----------*"))
#define BPMSG1120 bp_message_write_line_progmem(F("Open drain outputs (H=Hi-Z, L=GND)"))
#define BPMSG1121 bp_message_write_line_progmem(F("Normal outputs (H=3.3v, L=GND)"))
#define BPMSG1123 bp_message_write_buffer_progmem(F("MOST sig bit first"))
#define BPMSG1124 bp_message_write_buffer_progmem(F("LEAST sig bit first"))
#define BPMSG1127 bp_message_write_line_progmem(F(" 1. HEX\r\n 2. DEC\r\n 3. BIN\r\n 4. RAW"))
#define BPMSG1128 bp_message_write_line_progmem(F("Display format set"))
#define BPMSG1210 bp_message_write_buffer_progmem(F(" REVID:"))
#define BPMSG1211 bp_message_write_line_progmem(F("\r\nInvalid choice"))
#define BPMSG1212 bp_message_write_line_progmem(F("ms"))
#define BPMSG1226 bp_message_write_line_progmem(F("Pinstates:"))
#define BPMSG1227 bp_message_write_buffer_progmem(F("GND\t3.3V\t5.0V\tADC\tVPU\tAUX\t"))
#define BPMSG1228 bp_message_write_buffer_progmem(F("P\tP\tP\tI\tI\t"))
#define BPMSG1233 bp_message_write_line_progmem(F("1.(BR)\t2.(RD)\t3.(OR)\t4.(YW)\t5.(GN)\t6.(BL)\t7.(PU)\t8.(GR)\t9.(WT)\t0.(Blk)"))
#define BPMSG1234 bp_message_write_buffer_progmem(F("GND\t"))
#define BPMSG1252 bp_message_write_buffer_progmem(F("Number of bits read/write: "))
#define MSG_ADC_VOLTAGE_PROBE_HEADER bp_message_write_buffer_progmem(F("VOLTAGE PROBE: "))
#define MSG_ADC_VOLTMETER_MODE bp_message_write_line_progmem(F("VOLTMETER MODE"))
#define MSG_ANY_KEY_TO_EXIT_PROMPT bp_message_write_line_progmem(F("Any key to exit"))
#define MSG_BASE_CONVERTER_EQUAL_SIGN bp_message_write_buffer_progmem(F(" = "))
#define MSG_BINARY_NUMBER_PREFIX bp_message_write_buffer_progmem(F("0b"))
#define MSG_CHIP_REVISION_ID bp_message_write_buffer_progmem(F(" (16F1455 "))
#define MSG_CHIP_REVISION_UNKNOWN bp_message_write_buffer_progmem(F("UNK"))
#define MSG_CLEAR_LINE_WITH_CR bp_message_write_buffer_progmem(F("\x1B[2K\x0D"))
#define MSG_CLUTCH_DISENGAGED bp_message_write_line_progmem(F("Clutch disengaged!"))
#define MSG_CLUTCH_ENGAGED bp_message_write_line_progmem(F("Clutch engaged!"))
#define MSG_CURSOR_LEFT bp_message_write_buffer_progmem(F("\x1b[D"))
#define MSG_CURSOR_LEFT_TWO bp_message_write_buffer_progmem(F("\x1b[2D"))
#define MSG_CURSOR_RIGHT bp_message_write_buffer_progmem(F("\x1b[C"))
#define MSG_DESTRUCTIVE_BACKSPACE bp_message_write_buffer_progmem(F("\x08 \x08"))
#define MSG_FINISH_SETUP_PROMPT bp_message_write_line_progmem(F("To finish setup, start up the power supplies with command 'W'"))
#define MSG_HEXADECIMAL_NUMBER_PREFIX bp_message_write_buffer_progmem(F("0x"))
#define MSG_I2C_PINS_STATE bp_message_write_line_progmem(F("\tSCL\tSDA"))
#define MSG_I2C_START_BIT bp_message_write_line_progmem(F("I2C START"))
#define MSG_I2C_STOP_BIT bp_message_write_line_progmem(F("I2C STOP"))
#define MSG_MODE_HEADER_END	bp_message_write_line_progmem(F(" )"))
#define MSG_PIN_OUTPUT_TYPE_PROMPT bp_message_write_line_progmem(F(" 1. Open drain\r\n 2. Push-pull"))
#define MSG_POWER_SUPPLIES_ON bp_message_write_buffer_progmem(F("POWER ON"))
#define MSG_POWER_SUPPLIES_OFF bp_message_write_buffer_progmem(F("POWER OFF"))
#define MSG_RAW2WIRE_I2C_START bp_message_write_buffer_progmem(F("(\\-/_\\-)"))
#define MSG_RAW2WIRE_I2C_STOP bp_message_write_buffer_progmem(F("(\\_/-)"))
#define MSG_RAW2WIRE_MACRO_MENU	bp_message_write_buffer_progmem(F("No macros!"))
#define MSG_RAW2WIRE_MODE_HEADER bp_message_write_buffer_progmem(F("R2W (spd hiz)=( "))
#define MSG_READ_HEADER bp_message_write_buffer_progmem(F("READ: "))
#define MSG_SOFTWARE_MODE_SPEED_PROMPT bp_message_write_line_progmem(F("Set speed:\r\n 1. ~5KHz\r\n 2. ~50KHz\r\n 3. ~100KHz\r\n 4. ~400KHz"))
#define MSG_UNKNOWN_MACRO_ERROR bp_message_write_line_progmem(F("No macros!"))
#define MSG_VOLTAGE_UNIT bp_message_write_buffer_progmem(F("V"))
#define MSG_VREG_TOO_LOW bp_message_write_line_progmem(F("VREG too low, is there a short?"))
#define HLPSHORT bp_message_write_line_progmem(F("Too little flash. No help for you!"))
#define HLP1000 bp_message_write_line_progmem(F(" General\t\t\t\t\tProtocol interaction"))
#define HLP1001 bp_message_write_line_progmem(F(" ---------------------------------------------------------------------------"))
#define HLP1002 bp_message_write_line_progmem(F(" ?\tThis help\t\t\t(0)\tList current macros"))
#define HLP1003 bp_message_write_line_progmem(F(" =X/|X\tConverts X/reverse X\t\t(x)\tMacro x"))
#define HLP1004 bp_message_write_line_progmem(F(" ~\tSelftest\t\t\t[\tStart"))
#define HLP1005 bp_message_write_line_progmem(F(" #\tReset the BP   \t\t\t]\tStop"))
#define HLP1006 bp_message_write_line_progmem(F(" $\tJump to bootloader\t\t{\tStart with read"))
#define HLP1007 bp_message_write_line_progmem(F(" &/%\tDelay 1 us/ms\t\t\t}\tStop"))
#define HLP1008 bp_message_write_line_progmem(F(" a/A/@\tAUXPIN (low/HI/READ)\t\t\"abc\"\tSend string"))
#define HLP1009 bp_message_write_line_progmem(F(" b\tSet baudrate\t\t\t123"))
#define HLP1010 bp_message_write_line_progmem(F(" c/C\tAUX assignment (aux/CS)\t\t0x123"))
#define HLP1011 bp_message_write_line_progmem(F(" d/D\tMeasure ADC (once/CONT.)\t0b110\tSend value"))
#define HLP1012 bp_message_write_line_progmem(F(" f\tMeasure frequency\t\tr\tRead"))
#define HLP1013 bp_message_write_line_progmem(F(" g/S\tGenerate PWM/Servo\t\t/\tCLK hi"))
#define HLP1014 bp_message_write_line_progmem(F(" h\tCommandhistory\t\t\t\\\tCLK lo"))
#define HLP1015 bp_message_write_line_progmem(F(" i\tVersioninfo/statusinfo\t\t^\tCLK tick"))
#define HLP1016 bp_message_write_line_progmem(F(" l/L\tBitorder (msb/LSB)\t\t-\tDAT hi"))
#define HLP1017 bp_message_write_line_progmem(F(" m\tChange mode\t\t\t_\tDAT lo"))
#define HLP1018 bp_message_write_line_progmem(F(" o\tSet output type\t\t\t.\tDAT read"))
#define HLP1019 bp_message_write_line_progmem(F(" p/P\tPullup resistors (off/ON)\t!\tBit read"))
#define HLP1020 bp_message_write_line_progmem(F(" s\tScript engine\t\t\t:\tRepeat e.g. r:10"))
#define HLP1021 bp_message_write_line_progmem(F(" v\tShow volts/states\t\t.\tBits to read/write e.g. 0x55.2"))
#define HLP1022 bp_message_write_line_progmem(F(" w/W\tPSU (off/ON)\t\t<x>/<x= >/<0>\tUsermacro x/assign x/list all"))
#define MSG_COMMAND_HAS_NO_EFFECT bp_message_write_line_progmem(F("Command has no effect"))
#define MSG_SPI_PINS_STATE bp_message_write_line_progmem(F("CLK\tMOSI\tCS\tMISO"))


/**
   Prints the help screen information to the serial port.
*/
void print_help(void);

#ifdef	__cplusplus
}
#endif

#endif	/* MESSAGES_H */
