/*
    BSD 3-Clause License

    Copyright (c) 2018, KORG INC.
    All rights reserved.

    Redistribution and use in source and binary forms, with or without
    modification, are permitted provided that the following conditions are met:

    * Redistributions of source code must retain the above copyright notice, this
      list of conditions and the following disclaimer.

    * Redistributions in binary form must reproduce the above copyright notice,
      this list of conditions and the following disclaimer in the documentation
      and/or other materials provided with the distribution.

    * Neither the name of the copyright holder nor the names of its
      contributors may be used to endorse or promote products derived from
      this software without specific prior written permission.

    THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
    AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
    IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
    DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
    FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
    DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
    SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
    CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
    OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
    OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

//*/

#include "userosc.h"
#include "kord.hpp"
#include "simplelfo.hpp"

static Oscillator osc;
static Oscillator sub_osc;
static ScaleChords scale_chords;
static dsp::SimpleLFO s_lfo;

void OSC_INIT(uint32_t platform, uint32_t api)
{
  osc.wave_shape = saw;
  sub_osc.wave_shape = sqr;
  s_lfo.reset();
  s_lfo.setF0(30.f, k_samplerate_recipf);
}

void OSC_CYCLE(
  const user_osc_param_t * const params,
  int32_t *yn,
  const uint32_t frames
) {  
  s_lfo.cycle();
  osc.mod = s_lfo.triangle_bi();
  const uint16_t note = params->pitch>>8;
  int * chord = scale_chords.get_chord(note, osc.voice_type);
  
  float w[3];
  for (int i = 0; i < 3; i ++) {
    w[i] = osc_w0f_for_note(chord[i], params->pitch & 0xFF) + osc.detunes[i];
  }
  float sw = (sub_osc.sub_octave == 0) ? 0.f : w[0] / sub_osc.sub_octave;

  q31_t * __restrict y = (q31_t *)yn;
  const q31_t * y_e = y + frames;
  
  for (; y != y_e; ) {
    float sig = 0.f;
    for (int i = 0; i < 3; i++) {
      if (osc.detunes[i] == 0 && osc.voice_type == unison) osc.phases[i] = osc.phases[0];
      sig += osc.get_wave(osc.phases[i]) * 0.2;
      osc.phases[i] += w[i];
      osc.phases[i] -= (uint32_t)osc.phases[i];      
    }
    sig += (sub_osc.sub_octave == 0) ? 0 : sub_osc.get_wave(sub_osc.phases[0]) * 0.2;
    sub_osc.phases[0] += sw;
    sub_osc.phases[0] -= (uint32_t)sub_osc.phases[0];
    *(y++) = f32_to_q31(sig);
  }
}

void OSC_NOTEON(const user_osc_param_t * const params) {
  (void)params;
}

void OSC_NOTEOFF(const user_osc_param_t * const params) {
  (void)params;
}

void OSC_PARAM(uint16_t index, uint16_t value) {
  const float valf = param_val_to_f32(value);
  
  switch (index) {
    case k_user_osc_param_id1:
      osc.voice_type = VoiceType(value);
      break;
    case k_user_osc_param_id2:
      scale_chords.root_note = RootNote(value);
      break;
    case k_user_osc_param_id3:
      scale_chords.scale = Scale(value);
      break;
    case k_user_osc_param_id4: {
      sub_osc.sub_octave = value * 2;
      break;
    }
    case k_user_osc_param_id5: {
      float ratio = (float)value / 100.f;
      osc.shape = param_val_to_f32( (int)(1408 * ratio) );
      if (ratio < 0.1) ratio = 0.1;
      if (ratio > 0.9) ratio = 0.9;
      osc.pw = ratio;
      break;
    }
    case k_user_osc_param_id6:
      s_lfo.setF0(value, k_samplerate_recipf);
      break;
    case k_user_osc_param_shape: {
      float detune = (osc.wave_shape == saw) ? valf * 0.0001 : 0;
      osc.detunes[1] = detune;
      osc.detunes[2] = detune * 1.5;
      osc.mod_amount = valf * 0.5;
      break;
    }
    case k_user_osc_param_shiftshape: {
      int input = value / 384; 
      osc.wave_shape = WaveShape(input);
      break;
    }
    default:
      break;
  }
}

