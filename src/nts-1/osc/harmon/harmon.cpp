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

/*
 * File: sine.cpp
 *
 * Naive sine oscillator test
 *
 */

#include "userosc.h"
#include "harmon.hpp"

static Oscillator osc;
static float harmonics[5];

void OSC_INIT(uint32_t platform, uint32_t api)
{
  osc.drive = 0.f;
}

static inline float fold(float x)
{
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

void OSC_CYCLE(
        const user_osc_param_t * const params,
        int32_t *yn,
        const uint32_t frames
) {
  float signals[5] = { 0 };
  const float w0 = osc_w0f_for_note((params->pitch)>>8, params->pitch & 0xFF);
  const float drive = osc.drive;
  const float lfo = q31_to_f32(params->shape_lfo);
  
  q31_t * __restrict y = (q31_t *)yn;
  const q31_t * y_e = y + frames;

  for (; y != y_e; ) {
    for (int i = 0; i < 5; i++) {
        float p = osc.phases[i];
        float g = osc.gains[i];
        int div = i + 1;
        float w = w0 * div;
        p = (p < 0.f) ? 1.f - p : p - (uint32_t)p;
        float folded = 0.75f * (drive + lfo) * fold(osc_sinf(p));
        p = p + folded;
        signals[i] = (osc_sinf(p)) * g;
        osc.phases[i] += w;
        osc.phases[i] -= (uint32_t)osc.phases[i];
    }

    // Main signal
    float sig = 0.f;
    for (int i = 0; i < 5 ; i++) {
        sig += signals[i];
    }

    *(y++) = f32_to_q31(sig);
  }
}

void OSC_NOTEON(const user_osc_param_t * const params)
{
    (void)params;
}

void OSC_NOTEOFF(const user_osc_param_t * const params)
{
  (void)params;
}

void OSC_PARAM(uint16_t index, uint16_t value)
{
  const float valf = param_val_to_f32(value);
  
  switch (index) {
  case k_user_osc_param_id1:
      osc.gains[0] = valf;
      break;
  case k_user_osc_param_id2:
      osc.gains[1] = valf;
      break;
  case k_user_osc_param_id3:
      osc.gains[2] = valf;
      break;
  case k_user_osc_param_id4:
      osc.gains[3] = valf;
      break;
  case k_user_osc_param_id5:
      osc.gains[4] = valf;
      break;
  case k_user_osc_param_id6:
    break;
  case k_user_osc_param_shape:
    osc.drive = valf;
    break;
  case k_user_osc_param_shiftshape:
    break;
  default:
    break;
  }
}

