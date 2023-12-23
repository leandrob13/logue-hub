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
    float phases[6] = { 0.f };
    float gains[6]= { 0.5f, 0.f, 0.f, 0.f, 0.f, 0.f };
    float drive;
    float offset;
    Spread spread;

    int partial(int index) {
        int s;
        switch (spread) {
            case harmonic:
              s = index + 1;
              break;
            case odd:
              s = 2 * (index + 1) - 1;
              break;
            case even:
              s = (index == 0) ? 1 : 2 * index;
              break;
            case octaves:
              s = 1 << index;
              break;
        }
        return s;
    }

    float fold(float x, float mod) {
        x += offset;
        float fold;
        const float bias = (x < 0) ? -1.f : 1.f;
        int phase = int((x + bias) / 2.f);
        bool isEven = !(phase & 1);
        if (isEven) {
            fold = x - 2.f * phase;
        } else {
            fold = -x + 2.f * phase;
        }
        return (drive + mod) * fold;
    }
} Oscillator;
