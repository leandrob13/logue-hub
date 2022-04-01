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

static Oscillator osc;
static ScaleChords scale_chords;

void OSC_INIT(uint32_t platform, uint32_t api)
{
  osc.phase1 = 0.f;
  osc.phase2 = 0.f;
  osc.phase3 = 0.f;
}

void OSC_CYCLE(
  const user_osc_param_t * const params,
  int32_t *yn,
  const uint32_t frames
) {  

  const uint16_t note = params->pitch>>8;
  int * chord = scale_chords.get_chord(note);

  float w[3];
  for (int i = 0; i < 3; i ++) {
    w[i] = osc_w0f_for_note(chord[i], params->pitch & 0xFF);
  }
  
  q31_t * __restrict y = (q31_t *)yn;
  const q31_t * y_e = y + frames;
  float gain = 0.25f;
  
  for (; y != y_e; ) {
    float sig = 0.f;
    for (int i = 0; i < 3; i++) {
      sig += osc_softclipf(0.25f,osc_sawf(osc.phases[i]) * gain);
      osc.phases[i] += w[i];
      osc.phases[i] -= (uint32_t)osc.phases[i];
    }
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
      scale_chords.root_note = RootNote(value);
      break;
    case k_user_osc_param_id2: {
      scale_chords.scale = Scale(value);
      break;
    }
      
    case k_user_osc_param_id3:
      break;
    case k_user_osc_param_id4:
      break;
    case k_user_osc_param_id5:
      break;
    case k_user_osc_param_id6:
      break;
    case k_user_osc_param_shape:
      break;
    case k_user_osc_param_shiftshape:
      break;
    default:
      break;
  }
}

