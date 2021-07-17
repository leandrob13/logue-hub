/**
	LeandroB
*/

#pragma once
#include "userosc.h"

typedef struct Oscillator {
    float phases[5] = { 0 };
    float gains[5]= { 1.f, 0.f, 0.f, 0.f, 0.f };
    float drive;
} Oscillator;
