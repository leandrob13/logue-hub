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

void OSC_CYCLE(
        const user_osc_param_t * const params,
        int32_t *yn,
        const uint32_t frames
) {
  const float w0 = osc_w0f_for_note((params->pitch)>>8, params->pitch & 0xFF);
  const float lfo = q31_to_f32(params->shape_lfo);
  
  q31_t * __restrict y = (q31_t *)yn;
  const q31_t * y_e = y + frames;

  for (; y != y_e; ) {
    float sig = 0.f;
    for (int i = 0; i < 6; i++) {
        float p = osc.phases[i];
        int div = osc.partial(i);
        float w = w0 * div;
        p = (p < 0.f) ? 1.f - p : p - (uint32_t)p;
        float folded = 0.75f * osc.fold(osc_sinf(p), lfo);
        p = p + folded;
        sig += osc_sinf(p) * osc.gains[i];
        osc.phases[i] += w;
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

float scale_input(int value) {
  float ratio = (float)value / 500.f;
  return ratio;
}

void OSC_PARAM(uint16_t index, uint16_t value) {
  const float valf = param_val_to_f32(value);
  
  switch (index) {
    case k_user_osc_param_id1:
      osc.gains[1] = scale_input(value);
      break;
    case k_user_osc_param_id2:
      osc.gains[2] = scale_input(value);
      break;
    case k_user_osc_param_id3:
      osc.gains[3] = scale_input(value);
      break;
    case k_user_osc_param_id4:
      osc.gains[4] = scale_input(value);
      break;
    case k_user_osc_param_id5:
      osc.gains[5] = scale_input(value);
      break;
    case k_user_osc_param_id6:
      osc.spread = static_cast<Spread>(value);
      break;
    case k_user_osc_param_shape:
      osc.drive = value / 700.0f;
      break;
    case k_user_osc_param_shiftshape:
      osc.offset = value / 700.0f;
      break;
    default:
      break;
  }
}

