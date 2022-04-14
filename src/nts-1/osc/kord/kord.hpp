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
    Chromatic = 0,
    Mayor = 1,
    Minor = 2
} Scale;

typedef enum {
    unison = 0,
    octave = 1,
    fifth = 2,
    triad = 3
} VoiceType;

typedef struct ScaleChords {
    Scale scale = Mayor;
    RootNote root_note = C;

    int scale_notes[3][12] = {
        { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11 }, // Chromatic
        { 0, 0, 2, 2, 4, 5, 5, 7, 7, 9, 9, 11 }, // Mayor
        { 0, 0, 2, 3, 3, 5, 5, 7, 8, 8, 10, 10 } // Minor
    };

    int scale_chords[3][12][3] = {
        {   // Chromatic
            {0, 4, 7}, // MAYOR,
            {0, 4, 7}, // MAYOR, 
            {0, 4, 7}, // MAYOR, 
            {0, 4, 7}, // MAYOR, 
            {0, 4, 7}, // MAYOR, 
            {0, 4, 7}, // MAYOR, 
            {0, 4, 7}, // MAYOR, 
            {0, 4, 7}, // MAYOR, 
            {0, 4, 7}, // MAYOR, 
            {0, 4, 7}, // MAYOR, 
            {0, 4, 7}, // MAYOR, 
            {0, 4, 7}, // MAYOR, 
        },
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
            {0, 3, 7}, // MINOR, / 
            {0, 3, 6}, // MINOR_DIM
            {0, 4, 7}, // MAYOR, 
            {0, 4, 7}, // MAYOR, /
            {0, 3, 7}, // MINOR, 
            {0, 3, 7}, // MINOR, /
            {0, 3, 7}, // MINOR, 
            {0, 4, 7}, // MAYOR, 
            {0, 4, 7}, // MAYOR, /
            {0, 3, 6}, // MINOR_DIM
            {0, 3, 6} // MINOR_DIM /
        }
    };

    int * get_chord(uint16_t note, VoiceType voice_type) {
        static int chord[3];
        int scale_index = (int)scale;
        int note_index = note % 12;
        int adjustment = note_index - scale_notes[scale_index][note_index];
        int scaled_note = note - adjustment;
        int transposed_note = scaled_note + (int)root_note;
        switch (voice_type) {
            case unison:
                for (int i = 0; i < 3; i++){
                    chord[i] = transposed_note;
                }
                break;
            case octave: {
                int octaves[3] = {0, 12, 24};
                for (int i = 0; i < 3; i++){
                    chord[i] = transposed_note + octaves[i];
                }
                break;
            }
            case fifth:
                for (int i = 0; i < 3; i++){
                    chord[i] = (i == 2) ? transposed_note + 7 : transposed_note;
                }
                break;
            case triad: {
                for (int i = 0; i < 3; i++) {
                    chord[i] = scale_chords[scale_index][note_index][i] + transposed_note;
                }   
            }
            default:
                break;
        }
        return chord;
    }
} Scales;

typedef enum {
    saw = 0,
    sqr = 1,
    sine = 2
} WaveShape;

typedef struct Oscillator {
    WaveShape wave_shape;
    VoiceType voice_type = unison;
    float phases[3] = { 0 };
    float detunes[3] = { 0.f };
    float pw = 0.5;
    float shape = 0.f;
    float mod = 0;
    float mod_amount = 0;
    int sub_octave = 0; 

    float get_wave(float phase) {
        float wave;
        float p = (phase < 0.f) ? 1.f - phase : phase - (uint32_t)phase;
        switch (wave_shape) {
            case saw:
                wave = osc_sawf(p);
                break;
            case sqr: {
                float pw_value = pw + (mod * mod_amount);
                wave = (p <= pw_value) ? 0.8f : -0.8f;
                break;
            }
            case sine: {
                float folded = 0.75 * (shape + (mod * mod_amount)) * fold(osc_sinf(p));
                p = p + folded;
                wave = osc_sinf(p);
                break;
            }
            default:
                break;
        }
        return wave;
    }

    inline float fold(float x) {
        float fold;
        const float bias = (x < 0) ? -1.f : 1.f;
        int phase = int((x + bias) / 2.f);
        bool isEven = !(phase & 1);
        if (isEven) {
            fold = x - 2.f * phase;
        } else {
            fold = -x + 2.f * phase;
        }
        return fold;
    }
} Oscillator;
