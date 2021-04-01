# Logue 

Custom oscillators for the [minilogue xd](https://www.korg.com/products/synthesizers/minilogue_xd) and [Nu:Tekt NTS-1 digital kit](https://www.korg.com/products/dj/nts_1) synthesizers.
The use of the contents of this repo is subject to `BSD 3-Clause License`. See license file for details.

#### Compatibility Notes

In order to run user units built with SDK version 1.1-0, the following firmware versions are required:
* minilogue xd: >= v2.00
* Nu:Tekt NTS-1 digital: >= v1.02

## Oscillators

The compiled oscillators are found in the oscillators folder under its respective platform name:
- nts-1.
- minilogue-xd.

At the moment, only nts-1 is available.

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
Where platform is either `minilogue-xd` or `nts-1` (currently supporting on the the latter). Oscillator dir corresponds to the oscillator folder you want to build.

## Oscillators:

### SubH

A three oscillator setup based on the [undertone series](https://en.wikipedia.org/wiki/Undertone_series) which allows the use of subharmonic tones. 
It is composed of a main harmonic oscillator and two subharmonic oscillators. The available subharmonics are up to the 7th element in the undertone series.

#### Features

- One main harmonic oscillator with semitone calibration up to one octave higher and one octave lower.
- Two subharmonic oscillators that divide the main oscillator tone up to the 7th undertone.
- Choose different undertones for each suboscillator.
- Mix between the main oscillator and the suboscillators with the `shape` and `alt` knobs respectively.
- Choose between square, saw and sine waveforms.
- Modulate the main oscillator mix with the LFO.

 ## Parameters
 
| Parameter      | Range        |                                                                        |
| :------------: | :----------: | ---------------------------------------------------------------------: |
| SHAPE          | 0 to 100     |level mix of main oscillator                                            |
| ALT            | 0 to 100     |level mix of suboscillators                                             |
| semitone       | -12 to 12    |semitone tuning of main oscillator (-12 lower octave, 12 higher octave) |
| undertone 1    | 1 to 7       |undertone value of first suboscillator                                  |
| undertone 2    | 1 to 7       |undertone value of second suboscillator                                 |
| oscT           | 1 to  3      |1: saw, 2: square, 3: sine                                              |
