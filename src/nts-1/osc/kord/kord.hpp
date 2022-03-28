/**
	LeandroB
*/

#pragma once
#include "userosc.h"

typedef enum {
    C = 0,
    C_sharp = 1,
    D = 2,
    D_sharp = 3,
    E = 4,
    F = 5,
    F_sharp = 6,
    G = 7,
    G_sharp = 8,
    A = 9,
    A_sharp = 10,
    B = 11
} RootNote;

typedef enum {
    mayor,
    minor
} Scale;

int * get_all_notes() {
    static int notes[156];
    for (int i = 1; i <= 13; i++) {
        int max = 12 * i;
        int min = max - 12;
        for (int j = 0; j < 12; j++) {
            int index = j + min;
            notes[index] = j;
        }
    }
    return notes;
}

typedef struct Scales {
    int mayor[7] = { 0, 2, 4, 5, 7, 9, 11 };
    int minor[7] = { 0, 2, 3, 5, 7, 8, 10 };

    uint16_t get_inc(uint16_t note) {
        bool found = false;
        int inc = 1;
        for (int i = 0; i < 7; i++) {
            if (mayor[i] == note) {
              found = true;
              inc = 0;
              break;
            } 
        }
        uint16_t res = (found) ? note : note + 1;
        
        return inc;
    }

    int get_index(uint16_t note) {
        int index = 0;
        for (int i = 0; i < 7; i++) {
            if (mayor[i] == note) {
                index = i;
                break;
            }
        }
        return index;
    }
} Scales;

typedef enum {
    MAYOR = 0,
    MINOR = 1,
    MINOR_DIM = 2 
} ChordType;

typedef struct Chords {
    int chord[3] = {0};
    int mayor[3] = { 0, 4, 7 };
    int minor[3] = { 0, 3, 7 };
    int minor_dim[3] = { 0, 3, 6 };

    ChordType mayor_scale_chords[7] = { 
        MAYOR, 
        MINOR, 
        MINOR, 
        MAYOR, 
        MAYOR, 
        MINOR, 
        MINOR_DIM
    };

    int * get_chords(int index) {
        ChordType chord_type = mayor_scale_chords[index];
        
        switch (chord_type) {
            case MAYOR:
                chord[0] = 0;
                chord[1] = 4;
                chord[2] = 7;
                break;
            case MINOR:
                chord[0] = 0;
                chord[1] = 3;
                chord[2] = 7;
                break;
            case MINOR_DIM:
                chord[0] = 0;
                chord[1] = 3;
                chord[2] = 6;
                break;    
            default:
                break;
        }
        return chord;
    }
} Chords;

typedef enum {
    unison = 0,
    octave = 1,
    fifth = 2,
    Mayor = 3,
    Minor = 4
} VoiceType;

typedef struct Oscillator {
    float phase1 = 0.f;
    float phase2 = 0.f;
    float phase3 = 0.f;
    float drive;
    uint16_t semitone;
} Oscillator;
