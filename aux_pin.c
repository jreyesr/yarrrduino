//#include <pic16f1455.h>
//#include <builtins.h>

#include "aux_pin.h"
#include "base.h"
#include "proc_menu.h"
#include "hardware.h"
#include "messages.h"
#include "core.h"

extern mode_configuration_t mode_configuration;


void bp_aux_pin_set_high_impedance(void) {
  if (mode_configuration.alternate_aux == 0) {
    BP_AUX0_DIR = INPUT;
  } else {
    BP_CS_DIR = INPUT;
  }
  BPMSG1039;
}

void bp_aux_pin_set_high(void) {
  if (mode_configuration.alternate_aux == 0) {
    BP_AUX0_DIR = OUTPUT;
    BP_AUX0 = ON;
  } else {
    BP_CS_DIR = OUTPUT;
    BP_CS = ON;
  }
  BPMSG1040;
}

void bp_aux_pin_set_low(void) {
  if (mode_configuration.alternate_aux == 0) {
    BP_AUX0_DIR = OUTPUT;
    BP_AUX0 = OFF;
  } else {
    BP_CS_DIR = OUTPUT;
    BP_CS = OFF;
  }
  BPMSG1041;
}

bool bp_aux_pin_read(void) {
  if (mode_configuration.alternate_aux == 0) {
    BP_AUX0_DIR = INPUT;
    __nop();
    __nop();
    return BP_AUX0_PIN;
  }

  BP_CS_DIR = INPUT;
  __nop();
  __nop();
  return BP_CS_PIN;
}
