/**
	LeandroB
*/

#include "userosc.h"
#include "harmon.hpp"

static Oscillator osc;

void OSC_INIT(uint32_t platform, uint32_t api) {
  osc.drive = 0.f;
  osc.spread = harmonic;
}

static inline float fold(float x) {
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

static inline int spread(int index) {
    int s;
    switch (osc.spread) {
        case harmonic:
          s = index + 1;
          break;
        case odd:
          s = 2 * (index + 1) - 1;
          break;
        case even:
          s = (index == 0) ? 1 : 2 * index;
          break;
    }
    return s;
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
        int div = spread(i);
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
    sig *= osc.total_gain;

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
      switch (value) {
          case 0:
              osc.spread = harmonic;
              break;
          case 1:
              osc.spread = odd;
              break;
          case 2:
              osc.spread = even;
              break;
      }
    break;
  case k_user_osc_param_shape:
    osc.drive = valf;
    break;
  case k_user_osc_param_shiftshape:
    osc.total_gain = valf * 5.f;
    break;
  default:
    break;
  }
}

