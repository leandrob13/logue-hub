/**	
	LeandroB
*/

#include "userosc.h"
#include "subh.hpp"

static Oscillator oscillator;

void OSC_INIT(uint32_t platform, uint32_t api) {
  oscillator.phase = 0.f;
  oscillator.sub_phase = 0.f;
  oscillator.gain = 0.5f;
  oscillator.sub_gain = 0.5f;
  oscillator.semitone = 0;
  oscillator.undertone = 0;
  oscillator.type = 1;
}

float osc_type(float phase, uint16_t type) {
    switch(type) {
	    case 1:
	        return osc_sawf(phase);
	    case 2:
			return osc_sqrf(phase);
    }
}

float inline signal(float p, float gain) {
    float p0 = (p <= 0) ? 1.f - p : p - (uint32_t)p;
    return osc_softclipf(0.05f, osc_type(p0, oscillator.type) * gain);
}

void OSC_CYCLE(const user_osc_param_t * const params, int32_t *yn, const uint32_t frames) {
  const float w1 = osc_w0f_for_note(((params->pitch)>>8) + oscillator.semitone, params->pitch & 0xFF);
  const float sw1 = w1 / oscillator.undertone;
  
  // LFO
  float lfo = q31_to_f32(params->shape_lfo);

  q31_t * __restrict y = (q31_t *)yn;
  
  const q31_t * y_e = y + frames;
	
  for (; y != y_e; ) {
      // Main harmonic
      const float sig1 = signal(oscillator.phase, oscillator.gain);
      oscillator.phase += w1;
      oscillator.phase -= (uint32_t)oscillator.phase;
      // Sub harmonic
      const float subsig1  = signal(oscillator.sub_phase, oscillator.sub_gain);
      oscillator.sub_phase += sw1;
      oscillator.sub_phase -= (uint32_t)oscillator.sub_phase;
		
      *(y++) = f32_to_q31(sig1 + subsig1);
  }
}

void OSC_NOTEON(const user_osc_param_t * const params) {}

void OSC_NOTEOFF(const user_osc_param_t * const params){
  (void)params;
}

void OSC_PARAM(uint16_t index, uint16_t value) {
  switch (index) {
  case k_user_osc_param_id1:
      oscillator.semitone = value;
      break;
  case k_user_osc_param_id2:
      oscillator.undertone = value;
      break;
  case k_user_osc_param_id3:
      oscillator.type = value;
      break;
  case k_user_osc_param_id4:
  case k_user_osc_param_id5:
  case k_user_osc_param_id6:
  case k_user_osc_param_shape:
      oscillator.gain = param_val_to_f32(value);
      break;
  case k_user_osc_param_shiftshape:
      oscillator.sub_gain = param_val_to_f32(value);
      break;
  default:
    break;
  }
}

