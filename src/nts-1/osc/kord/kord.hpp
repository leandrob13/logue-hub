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
    Scale scale = Mayor;
    int note_index = 0;
    int * all_notes = get_all_notes();
    int mayor_notes[7] = { 0, 2, 4, 5, 7, 9, 11 };
    int minor_notes[7] = { 0, 2, 3, 5, 7, 8, 10 };

    uint16_t get_scaled_note(uint16_t note) {
        int ranged_note = all_notes[note];
        int increment = 1;
        int * selected_scale = get_scale_notes();
        for (int i = 0; i < 7; i++) {
            if (selected_scale[i] == ranged_note) {
                increment = 0;
                break;
            } 
        }
        int scaled_note = note + increment;
        int inc_note = ranged_note + increment;
        for (int i = 0; i < 7; i++) {
            if (selected_scale[i] == inc_note) {
                note_index = i;
                break;
            }
        }

        return scaled_note;
    }

    int * get_scale_notes() {
        static int * selected_scale;
        switch (scale) {
            case Mayor:
                selected_scale = mayor_notes;
                break;
            case Minor:
                selected_scale = minor_notes;
                break;
            default:
                break;
        }
        return selected_scale;
    }

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

} Scales;

typedef enum {
    MAYOR = 0,
    MINOR = 1,
    MINOR_DIM = 2 
} ChordType;

typedef struct Chords {
    Scale scale = Mayor;
    RootNote root_note = C;

    ChordType mayor_scale_chords[7] = { 
        MAYOR, 
        MINOR, 
        MINOR, 
        MAYOR, 
        MAYOR, 
        MINOR, 
        MINOR_DIM
    };

    ChordType minor_scale_chords[7] = { 
        MINOR, 
        MINOR_DIM, 
        MAYOR, 
        MINOR, 
        MAYOR, 
        MAYOR, 
        MINOR_DIM
    };

    int * get_chord(uint16_t note, int index) {
        ChordType * selected_scale_chords = get_scale_chords(); 
        ChordType chord_type = selected_scale_chords[index];
        static int chord[3] = {0};
        int transposed_note = note + (int)root_note;
        chord[0] = transposed_note;
        switch (chord_type) {
            case MAYOR:
                chord[1] = transposed_note + 4;
                chord[2] = transposed_note + 7;
                break;
            case MINOR:
                chord[1] = transposed_note + 3;
                chord[2] = transposed_note + 7;
                break;
            case MINOR_DIM:
                chord[1] = transposed_note + 3;
                chord[2] = transposed_note + 6;
                break;    
            default:
                break;
        }
        return chord;
    }

    ChordType * get_scale_chords() {
        static ChordType * selected_chords;
        switch (scale){
            case Mayor:
                selected_chords = mayor_scale_chords;
                break;
            case Minor:
                selected_chords = minor_scale_chords;
                break;
            default:
                break;
        }
        return selected_chords;
    }
} Chords;

typedef enum {
    unison = 0,
    octave = 1,
    fifth = 2,
    mayor_triad = 3,
    minor_triad = 4
} VoiceType;

typedef struct Oscillator {
    float phase1 = 0.f;
    float phase2 = 0.f;
    float phase3 = 0.f;
    float drive;
} Oscillator;
