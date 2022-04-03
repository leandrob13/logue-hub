# Logue 

Custom oscillators for the [minilogue xd](https://www.korg.com/products/synthesizers/minilogue_xd) and [Nu:Tekt NTS-1 digital kit](https://www.korg.com/products/dj/nts_1) synthesizers.
The use of the contents of this repo is subject to `BSD 3-Clause License`. See license file for details.

#### Compatibility Notes

In order to run user units built with SDK version 1.1-0, the following firmware versions are required:
* minilogue xd: >= v2.00
* Nu:Tekt NTS-1 digital: >= v1.02

## Contents
- [Compiled Oscillators](#compiled-oscillators)
- [Prerequisites](#prerequisites)
- [Build](#build)
- [Custom Oscillator Reference](#custom-oscillator-reference)
    - [SubHarmonikorg](#subharmonikorg)
    - [Harmonik](#harmonik)
    - [Kord](#kord)

## Compiled Oscillators

The compiled oscillators are found in the oscillators folder under its respective platform name:
- nts-1.
- minilogue-xd.

## Prerequisites

In order to build the oscillators you need to install the `gcc-arm-none-eabi` compiler:
```
./gcc_compiler/get_gcc_osx.sh
```

## Build

To generate the oscillator files to upload to your hardware, you need to execute in root folder:
```
make -C builds/{platform}/osc/{oscillator dir}
```
Where platform is either `minilogue-xd` or `nts-1`. Oscillator dir corresponds to the oscillator folder you want to build.

## Custom Oscillator Reference:

### SubHarmonikorg

A three oscillator setup based on the [undertone series](https://en.wikipedia.org/wiki/Undertone_series) which allows the use of subharmonic tones. 
It is composed of a main oscillator and two subharmonic oscillators. The available subharmonics are up to the 7th element in the undertone series.

#### Features

- One main harmonic oscillator with semitone calibration up to one octave higher and one octave lower.
- Two subharmonic oscillators that divide the main oscillator tone up to the 7th undertone.
- Choose different undertones for each suboscillator.
- Mix between the main oscillator and the suboscillators with the `shape` and `alt` knobs respectively.
- Choose between square, saw and sine waveforms.
- Modulate the main oscillator mix with the LFO.
- Subharmonic oscillators arpeggiator with clock adjustment.

 #### Parameters
 
| Parameter      | Range        | Description                                                            |
| :------------: | :----------: | ---------------------------------------------------------------------: |
| SHAPE          | 0 to 100     |level mix of main oscillator                                            |
| ALT            | 0 to 100     |level mix of suboscillators                                             |
| semitone       | 1 to 13      |semitone tuning of main oscillator (1 as root note, 13 higher octave)   |
| undertone 1    | 1 to 7       |undertone value of first suboscillator                                  |
| undertone 2    | 1 to 7       |undertone value of second suboscillator                                 |
| oscT           | 1 to  3      |The oscillator type 1: saw, 2: square, 3: sine                          |
| Arp Type       | 1 to  3      |The arpeggiator type 1: off, 2: down, 3: up-down                        |
| Arp Clock      | 0 to  100    |The arpeggiator clock percentage of max frequency.                      |

### Harmonik

The Harmonik is an additive oscillator comprised of 5 sine wave oscillators, each of them has an amplitude control and are summed to get the final tones.
By default, each voice is spread apart by the harmonic series with the root note as the fundamental frequency. The Spread setting can change this so that we get even or odd partials in the mix. 
For more harmonic addition, a wave folder can be controlled with the Shape knob.   

#### Features

- Five sine wave oscillators with amplitude control.
- A wave folder that folds each sine wave before summing the result to the final signal.
- Spread control for the partials so that we have harmonic, odd or even partial overtones.
- The wave folder can be modulated with the LFO.
- Total gain control with distortion possibility when all the voices gains are at max.

 #### Parameters
 
| Parameter      | Range        | Description                                                            |
| :------------: | :----------: | ---------------------------------------------------------------------: |
| SHAPE          | 0 to 100     |Wave folder control                                                     |
| ALT            | 0 to 100     |Gain of the final signal                                                |
| ROOT           | 0 to 100     |The fundamental frequency amplitude controller                          |
| P1             | 0 to 100     |The first partial frequency amplitude controller                        |
| P2             | 0 to 100     |The second partial frequency amplitude controller                       |
| P3             | 0 to 100     |The third partial frequency amplitude controller                        |
| P4             | 0 to 100     |The fourth partial frequency amplitude controller                       |
| SPRD           | 0 to 2       |The spread controller 0: harmonic, 1: odd, 2: even                      |

### Kord

Kord took inspiration from two synthesizers: Korg Volca Keys and Make Noise tELHARMONIC. After developing the basic chord progression logic I found out that the Renaissance firmware for Mutable Instruments Braids had already implemented similar features. 
My motivation for developing this oscillator was to have a better chord progression and scale control than the tELHARMONIC with a more classical sound like the one found in the volca keys.

The Kord has 3 oscillators which can be used in three different arrangements: unison, octaves, fifths and triads. In each voice type you can set up three scales: chromatic, mayor and minor. When in chromatic, all the 12 semitones will be available for unison, octaves and fifths modes. For triad mode, chromatic will produce mayor chords for every semitone root chord. If one of the other two scales is selected the available notes will be constrained to the 7 diatonic notes in the given scale. For triad mode the chords produced will be relative to the scale, meaning that you will have chord progressions of mayor, minor and diminished chords according to the scale. It is also included a suboscillator which can be set to be one or two octaves lower than the root note.


#### Features

- Three oscillators with two different waveforms: Saw and Square.
- Four different oscillator arrangements: unison, octaves, fifths and triads.
- Three musical scales: chromatic, mayor and minor.
- If the scale is not chromatic, the triad arrangement will produce chord progressions that are in the given scale: Mayor (I, ii, iii, IV, V, vi, vii•), Minor (i, ii•, III, iv, v, VI, vii•). 
- Oscillator detune for Saw and PWM for Square.
- Blend between saw and square waveforms.
- One Square wave suboscillator that can go one or two octaves lower than the root note.

#### Parameters

| Parameter      | Range        | Description                                                            |
| :------------: | :----------: | ---------------------------------------------------------------------: |
| SHAPE          | 0 to 100     |detune (Saw), PWM (Square) control.                                     |
| ALT            | 0 to 100     |Blend control for waveforms.                                            |
| Voic           | 0 to 3       |The voice arrangement: 0: unison, 1: octaves, 2: fifths, 3: triads.     |
| Root           | 0 to 12      |The root note of the scale.                                             |
| Scal           | 0 to 2       |The musical scale: 0: chromatic, 1: mayor, 2: minor.                    |
| Sub            | 0 to 2       |Enables the square suboscillator: 0: disabled, 1: one octave lower, 2: two octaves lower.|
| PW             | 0 to 100     |Pulse width of the Square wave oscillators                              |
| LFO            | 30 to 100    |The rate of the LFO controlling PWM                                     |