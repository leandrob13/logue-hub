/**	
	LeandroB
*/

#pragma once
#include "userosc.h"

typedef struct Oscillator {
    float phase;
    float sub_phase1;
    float sub_phase2;
    uint16_t type;
    float gain;
    float sub_gain;
    uint16_t semitone;
    uint16_t undertone1;
    uint16_t undertone2;
} Oscillator;

typedef enum {
    saw = 1,
    square = 2,
    sine = 3
} OscillatorType;

typedef enum {
    off = 0,
    down = 1,
    up_down = 2
} ArpType;

typedef struct SubArp {
    uint16_t undertone;
    ArpType arp_type;
    uint16_t reset;
    bool up_slope;
} SubArp;