/**
	LeandroB
*/

#pragma once
#include "userosc.h"

typedef enum {
    harmonic = 0,
    odd = 1,
    even = 2,
    octaves = 3
} Spread;

typedef struct Oscillator {
    float phases[6] = { 0 };
    float gains[6]= { 0.17, 0.f, 0.f, 0.f, 0.f, 0.f };
    float drive;
    Spread spread;
} Oscillator;
