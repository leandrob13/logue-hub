/**	
	LeandroB
*/

#pragma once
#include "userosc.h"

typedef struct Oscillator {
  float phase;
  float sub_phase;
  uint16_t type;
  float gain;
  float sub_gain;
  uint16_t semitone;
  uint16_t undertone;
} Oscillator;