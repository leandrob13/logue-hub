/**
	LeandroB
*/

#pragma once
#include "userosc.h"

typedef enum {
    harmonic = 0,
    odd = 1,
    even = 2
} Spread;

typedef struct Oscillator {
    float phases[5] = { 0 };
    float gains[5]= { 1.f, 0.f, 0.f, 0.f, 0.f };
    float drive;
    Spread spread;
} Oscillator;
