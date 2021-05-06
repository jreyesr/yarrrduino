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

#define F(string_literal) (PSTR(string_literal))

#define BP_VERSION_STRING ("Bus Pirate v3")

#define BPMSG1085 bp_message_write_line("Ready")
#define BPMSG1093 bp_message_write_line("RESET")
#define BPMSG1039 bp_message_write_buffer("AUX HI-Z")
#define BPMSG1040 bp_message_write_line("AUX 1")
#define BPMSG1041 bp_message_write_line("AUX 0")
#define BPMSG1086 bp_message_write_line("a/A/@ controls AUX pin")
#define BPMSG1087 bp_message_write_line("a/A/@ controls CS pin")
#define BPMSG1088 bp_message_write_line("Command not used in this mode")
#define BPMSG1095 bp_message_write_buffer("AUX HI-Z, READ: ")
#define BPMSG1098 bp_message_write_buffer("DATA READ: ")
#define BPMSG1099 bp_message_write_buffer("DELAY ")
#define BPMSG1100 bp_message_write_line("us")
#define BPMSG1101 bp_message_write_buffer("WRITE: ")
#define BPMSG1103 bp_message_write_line("CLK 1")
#define BPMSG1104 bp_message_write_line("CLK 0")
#define BPMSG1105 bp_message_write_line("DAT OUT 1")
#define BPMSG1106 bp_message_write_line("DAT OUT 0")
#define BPMSG1107 bp_message_write_line(" *pin is HiZ")
#define BPMSG1108 bp_message_write_buffer("TICKS: ")
#define BPMSG1109 bp_message_write_buffer("READ BIT: ")
#define BPMSG1110 bp_message_write_buffer("Syntax error, char ")
#define BPMSG1111 bp_message_write_line("x. cancel")
#define BPMSG1112 bp_message_write_line("No mode change")
#define BPMSG1114 bp_message_write_line("Nonexistent protocol!")
#define BPMSG1117 bp_message_write_buffer("DEVID:")
#define BPMSG1118 bp_message_write_line("http://dangerousprototypes.com")
#define BPMSG1123 bp_message_write_buffer("MOST sig bit first")
#define BPMSG1124 bp_message_write_buffer("LEAST sig bit first")
#define BPMSG1127 bp_message_write_line(" 1. HEX\r\n 2. DEC\r\n 3. BIN\r\n 4. RAW")
#define BPMSG1128 bp_message_write_line("Display format set")
#define BPMSG1210 bp_message_write_buffer(" REVID:")
#define BPMSG1211 bp_message_write_line("\r\nInvalid choice")
#define BPMSG1212 bp_message_write_line("ms")
#define MSG_BASE_CONVERTER_EQUAL_SIGN bp_message_write_buffer(" = ")
#define MSG_BINARY_NUMBER_PREFIX bp_message_write_buffer("0b")
#define MSG_CHIP_REVISION_ID bp_message_write_buffer(" (16F1455 ")
#define MSG_CHIP_REVISION_UNKNOWN bp_message_write_buffer("UNK")
#define MSG_CLUTCH_DISENGAGED bp_message_write_line("Clutch disengaged!")
#define MSG_CLUTCH_ENGAGED bp_message_write_line("Clutch engaged!")
#define MSG_CURSOR_LEFT bp_message_write_buffer("\x1b[D")
#define MSG_CURSOR_LEFT_TWO bp_message_write_buffer("\x1b[2D")
#define MSG_CURSOR_RIGHT bp_message_write_buffer("\x1b[C")
#define MSG_DESTRUCTIVE_BACKSPACE bp_message_write_buffer("\x08 \x08")
#define MSG_FINISH_SETUP_PROMPT bp_message_write_line("To finish setup, start up the power supplies with command 'W'")
#define MSG_HEXADECIMAL_NUMBER_PREFIX bp_message_write_buffer("0x")
#define MSG_I2C_PINS_STATE bp_message_write_line("\tSCL\tSDA")
#define MSG_I2C_START_BIT bp_message_write_line("I2C START")
#define MSG_I2C_STOP_BIT bp_message_write_line("I2C STOP")
#define MSG_MODE_HEADER_END	bp_message_write_line(" )")
#define MSG_PIN_OUTPUT_TYPE_PROMPT bp_message_write_line(" 1. Open drain\r\n 2. Push-pull")
#define MSG_POWER_SUPPLIES_ON bp_message_write_buffer("POWER ON")
#define MSG_POWER_SUPPLIES_OFF bp_message_write_buffer("POWER OFF")
#define MSG_RAW2WIRE_I2C_START bp_message_write_buffer("(\\-/_\\-)")
#define MSG_RAW2WIRE_I2C_STOP bp_message_write_buffer("(\\_/-)")
#define MSG_RAW2WIRE_MACRO_MENU	bp_message_write_buffer("No macros!")
#define MSG_RAW2WIRE_MODE_HEADER bp_message_write_buffer("R2W (spd hiz)=( ")
#define MSG_READ_HEADER bp_message_write_buffer("READ: ")
#define MSG_SOFTWARE_MODE_SPEED_PROMPT bp_message_write_line("Set speed:\r\n 1. ~5KHz\r\n 2. ~50KHz\r\n 3. ~100KHz\r\n 4. ~400KHz")
#define MSG_UNKNOWN_MACRO_ERROR bp_message_write_line("No macros!")
#define MSG_VREG_TOO_LOW bp_message_write_line("VREG too low, is there a short?")
#define HLPSHORT bp_message_write_line("Too little flash. No help for you!")
#define HLP1000 bp_message_write_line(" General\t\t\t\t\tProtocol interaction")
#define HLP1001 bp_message_write_line(" ---------------------------------------------------------------------------")
#define HLP1002 bp_message_write_line(" ?\tThis help\t\t\t(0)\tList current macros")
#define HLP1003 bp_message_write_line(" =X/|X\tConverts X/reverse X\t\t(x)\tMacro x")
#define HLP1004 bp_message_write_line(" ~\tSelftest\t\t\t[\tStart")
#define HLP1005 bp_message_write_line(" #\tReset the BP   \t\t\t]\tStop")
#define HLP1006 bp_message_write_line(" $\tJump to bootloader\t\t{\tStart with read")
#define HLP1007 bp_message_write_line(" &/%\tDelay 1 us/ms\t\t\t}\tStop")
#define HLP1008 bp_message_write_line(" a/A/@\tAUXPIN (low/HI/READ)\t\t\"abc\"\tSend string")
#define HLP1009 bp_message_write_line(" b\tSet baudrate\t\t\t123")
#define HLP1010 bp_message_write_line(" c/C\tAUX assignment (aux/CS)\t\t0x123")
#define HLP1011 bp_message_write_line(" d/D\tMeasure ADC (once/CONT.)\t0b110\tSend value")
#define HLP1012 bp_message_write_line(" f\tMeasure frequency\t\tr\tRead")
#define HLP1013 bp_message_write_line(" g/S\tGenerate PWM/Servo\t\t/\tCLK hi")
#define HLP1014 bp_message_write_line(" h\tCommandhistory\t\t\t\\\tCLK lo")
#define HLP1015 bp_message_write_line(" i\tVersioninfo/statusinfo\t\t^\tCLK tick")
#define HLP1016 bp_message_write_line(" l/L\tBitorder (msb/LSB)\t\t-\tDAT hi")
#define HLP1017 bp_message_write_line(" m\tChange mode\t\t\t_\tDAT lo")
#define HLP1018 bp_message_write_line(" o\tSet output type\t\t\t.\tDAT read")
#define HLP1019 bp_message_write_line(" p/P\tPullup resistors (off/ON)\t!\tBit read")
#define HLP1020 bp_message_write_line(" s\tScript engine\t\t\t:\tRepeat e.g. r:10")
#define HLP1021 bp_message_write_line(" v\tShow volts/states\t\t.\tBits to read/write e.g. 0x55.2")
#define HLP1022 bp_message_write_line(" w/W\tPSU (off/ON)\t\t<x>/<x= >/<0>\tUsermacro x/assign x/list all")
#define MSG_COMMAND_HAS_NO_EFFECT bp_message_write_line("Command has no effect")
#define MSG_SPI_PINS_STATE bp_message_write_line("CLK\tMOSI\tCS\tMISO")


/**
   Prints the help screen information to the serial port.
*/
void print_help(void);

#ifdef	__cplusplus
}
#endif

#endif	/* MESSAGES_H */
