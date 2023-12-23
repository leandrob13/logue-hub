# Logue 

Custom oscillators for the [Nu:Tekt NTS-1 digital kit](https://www.korg.com/products/dj/nts_1) synthesizer.
The use of the contents of this repo is subject to `BSD 3-Clause License`. See license file for details.

#### Compatibility Notes

In order to run user units built with SDK version 1.1-0, the following firmware versions are required:
* Nu:Tekt NTS-1 digital: >= v1.02

## Contents
- [Compiled Oscillators and FX](#compiled-oscillators-and-fx)
- [Prerequisites](#prerequisites)
- [Build](#build)
- [Custom Oscillator Reference](#custom-oscillator-reference)
    - [SubHarmonikorg](#subharmonikorg)
    - [Harmonik](#harmonik)
    - [Kord](#kord)

## Compiled Oscillators and FX

The compiled oscillators are found in the releases section of this repo.

## Prerequisites

In order to build the oscillators you need to install the `gcc-arm-none-eabi` compiler.

For macOS:
```
./gcc_compiler/get_gcc_osx.sh
```

## Build

To generate the oscillator or FX package files, you need to execute in root folder:
```
make -C {application dir}
```
 The application dir corresponds to the folder of the oscillator or FX you want to build.

## Custom Oscillator and FX Reference:

- [Harmonik](harmonik): six sine wave harmonic oscillator.
- [Kord](kord): diatonic chord oscillator.
- [Subharmonikorg](subh): subharmonic oscillator.