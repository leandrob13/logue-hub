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
    Mayor = 0,
    Minor = 1
} Scale;

typedef struct ScaleChords {
    Scale scale = Mayor;
    RootNote root_note = C;

    int scale_notes[2][12] = {
        { 0, 0, 2, 2, 4, 5, 5, 7, 7, 9, 9, 11 }, // Mayor
        { 0, 0, 2, 3, 3, 5, 5, 7, 8, 8, 10, 10 } // Minor
    };

    int scale_chords[2][12][3] = {
        {   // Mayor
            {0, 4, 7}, // MAYOR,
            {0, 4, 7}, // MAYOR, 
            {0, 3, 7}, // MINOR, 
            {0, 3, 7}, // MINOR, 
            {0, 3, 7}, // MINOR, 
            {0, 4, 7}, // MAYOR, 
            {0, 4, 7}, // MAYOR, 
            {0, 4, 7}, // MAYOR, 
            {0, 4, 7}, // MAYOR, 
            {0, 3, 7}, // MINOR, 
            {0, 3, 7}, // MINOR, 
            {0, 3, 6} // MINOR_DIM
        },
        {   //Minor
            {0, 3, 7}, // MINOR,
            {0, 3, 7}, // MINOR,  
            {0, 3, 6}, // MINOR_DIM
            {0, 4, 7}, // MAYOR, 
            {0, 4, 7}, // MAYOR, 
            {0, 3, 7}, // MINOR, 
            {0, 3, 7}, // MINOR, 
            {0, 4, 7}, // MAYOR, 
            {0, 4, 7}, // MAYOR, 
            {0, 4, 7}, // MAYOR, 
            {0, 3, 6} // MINOR_DIM
        }
    };

    int * get_chord(uint16_t note) {
        int scale_index = (int)scale;
        int note_index = note % 12;
        static int chord[3] = { 0 };
        int transposed_note = scale_notes[scale_index][note_index] + (int)root_note;

        for (int i = 0; i < 3; i++) {
            chord[i] = scale_chords[scale_index][note_index][i] + transposed_note;
        }

        return chord;
    }
} Scales;

typedef enum {
    unison = 0,
    octave = 1,
    fifth = 2,
    mayor_triad = 3,
    minor_triad = 4
} VoiceType;

typedef struct Oscillator {
    float phases[3] = { 0 };
    float phase1 = 0.f;
    float phase2 = 0.f;
    float phase3 = 0.f;
    float drive;
} Oscillator;
