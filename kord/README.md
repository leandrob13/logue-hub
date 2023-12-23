# Kord

Kord took inspiration from two synthesizers: Korg Volca Keys and Make Noise tELHARMONIC. After developing the basic chord progression logic I found out that the Renaissance firmware for Mutable Instruments Braids had already implemented similar features. 
My motivation for developing this oscillator was to have a better chord progression and scale control than the tELHARMONIC with a more classical sound like the one found in the volca keys.

The Kord has 3 oscillators which can be used in four different arrangements: unison, octaves, fifths and triads. In each voice type you can set up three scales: chromatic, mayor and minor. When in chromatic, all the 12 semitones will be available for unison, octaves and fifths modes. For triad mode, chromatic will produce mayor chords for every semitone. If one of the other two scales is selected the available notes will be constrained to the 7 diatonic notes in the given scale. For triad mode the chords produced will be relative to the scale, meaning that you will have chord progressions of mayor, minor and diminished chords according to the scale. It is also included a suboscillator which can be set to be one or two octaves lower than the root note.


## Features

- Three oscillators with three different waveforms: Saw, Square and Sine.
- Four different oscillator arrangements: unison, octaves, fifths and triads.
- Three musical scales: chromatic, mayor and minor.
- If the scale is not chromatic, the triad arrangement will produce chord progressions that are in the given scale: Mayor (I, ii, iii, IV, V, vi, vii•), Minor (i, ii•, III, iv, v, VI, vii•). 
- Select between saw, square and sine waveforms.
- Oscillator detune for Saw, PWM for Square, Fold for Sine.
- One Square wave suboscillator that can go one or two octaves lower than the root note.

## Parameters

| Parameter      | Range        | Description                                                            |
| :------------: | :----------: | ---------------------------------------------------------------------: |
| SHAPE          | 0 to 100     |Detune (Saw), PWM (Square), Fold (Sine) LFO modulation amount control.  |
| ALT            | 0 to 100     |Wave form selector: 0: Saw, 1: Square, 2: Sine.                         |
| Voic           | 0 to 3       |The voice arrangement: 0: unison, 1: octaves, 2: fifths, 3: triads.     |
| Root           | 0 to 12      |The root note of the scale.                                             |
| Scal           | 0 to 2       |The musical scale: 0: chromatic, 1: mayor, 2: minor.                    |
| Sub            | 0 to 2       |Enables the suboscillator: 0: disabled, 1: 1 octave, 2: 2 octaves.      |
| SHP            | 0 to 100     |Pulse width of the Square wave or fold amount for the Sine wave         |
| LFO            | 30 to 100    |The rate of the LFO controlling PWM                                     |