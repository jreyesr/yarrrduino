#include "messages.h"
#include "base.h"


void bp_message_write_buffer(char *strptr) {
  for (; *strptr != '\0'; strptr++)
    user_serial_transmit_character(*strptr);
}

void bp_message_write_buffer_progmem(const char *strptr) {
  char buf[strlen_P(strptr) + 1];
  strcpy_P(buf, strptr);
  char *bufptr = buf;
  for (; *bufptr != '\0'; bufptr++)
    user_serial_transmit_character(*bufptr);
}

void bp_message_write_line(char *strptr) {
  bp_message_write_buffer(strptr);
  bpBR;
}

void bp_message_write_line_progmem(const char *strptr) {
  bp_message_write_buffer_progmem(strptr);
  bpBR;
}

void print_help(void) {
  //HLPSHORT;
  // There is no almost no flash. Replace message
  HLP1000;
    HLP1001;
    HLP1002;
    HLP1003;
    HLP1004;
    HLP1005;
    HLP1006;
    HLP1007;
    HLP1008;
    HLP1009;
    HLP1010;
    HLP1011;
    HLP1012;
    HLP1013;
    HLP1014;
    HLP1015;
    HLP1016;
    HLP1017;
    HLP1018;
    HLP1019;
    HLP1020;
    HLP1021;
    HLP1022;
}
