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

## Oscillators:

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

 ## Parameters
 
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

The Harmonik is an additive oscillator comprised of 5 sine wave voices, each voice has an amplitude control and are summed to get the final tones.
By default, each voice is spread apart by the harmonic series with the root note as the fundamental frequency. The Spread setting can change this so that we get even or odd partials in the mix. 
For more harmonic addition, a wave folder can be controlled with the Shape knob.   

#### Features

- Five sine wave oscillators with amplitude control.
- A wave folder that folds each sine wave before summing the result to the final signal.
- Spread control for the partials so that we have harmonic, odd or even partial overtones.
- The wave folder can be modulated with the LFO.

 ## Parameters
 
| Parameter      | Range        | Description                                                            |
| :------------: | :----------: | ---------------------------------------------------------------------: |
| SHAPE          | 0 to 100     |Wave folder control                                                     |
| ROOT           | 0 to 100     |The fundamental frequency amplitude controller                          |
| P1             | 0 to 100     |The first partial frequency amplitude controller                        |
| P2             | 0 to 100     |The second partial frequency amplitude controller                       |
| P3             | 0 to 100     |The third partial frequency amplitude controller                        |
| P4             | 0 to 100     |The fourth partial frequency amplitude controller                       |
| SPRD           | 0 to 2       |The spread controller 0: harmonic, 1: odd, 2: even                      |