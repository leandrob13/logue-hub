/**	
	LeandroB
*/

#include "userosc.h"
#include "subh.hpp"
#include "simplelfo.hpp"

static Oscillator oscillator;
static SubArp sub_arp;
static dsp::SimpleLFO s_lfo;

static bool roll;

void OSC_INIT(uint32_t platform, uint32_t api) {
  oscillator.phase = 0.f;
  oscillator.sub_phase1 = 0.f;
  oscillator.sub_phase2 = 0.f;
  oscillator.gain = 0.5f;
  oscillator.sub_gain = 0.5f;
  oscillator.semitone = 0;
  oscillator.undertone1 = 0;
  oscillator.undertone2 = 0;
  oscillator.type = saw;

  sub_arp.arp_type = off;
  sub_arp.undertone = 1;
  sub_arp.reset = 0;

  s_lfo.reset();
  s_lfo.setF0(0.f, k_samplerate_recipf);
  roll = false;
}

float osc_type(float phase, uint16_t type) {
    float wave;
    switch(type) {
        case saw:
            wave = osc_sawf(phase);
            break;
        case square:
            wave = osc_sqrf(phase);
            break;
        case sine:
            wave = osc_sinf(phase);
            break;
    }
    return wave;
}

float inline signal(float p, float gain) {
    float p0 = (p <= 0) ? 1.f - p : p - (uint32_t)p;
    return osc_softclipf(0.25f, osc_type(p0, oscillator.type) * gain);
}

uint16_t arp_undertone(void) {
    s_lfo.cycle();
    float wave = s_lfo.square_uni();
    if (roll) {
        switch (sub_arp.arp_type) {
            case down: {
                if (wave == 0.f && sub_arp.reset == 0) {
                    if (sub_arp.undertone < 7)
                        sub_arp.undertone += 1;
                    else
                        sub_arp.undertone = 2;
                    sub_arp.reset = 1;
                }
                if (wave == 1.f)
                    sub_arp.reset = 0;
                break;
            }
            case up_down: {
                if (wave == 0.f && sub_arp.reset == 0) {
                    if (sub_arp.undertone < 7 && sub_arp.up_slope)
                        sub_arp.undertone += 1;
                    else if (sub_arp.undertone > 3) {
                        sub_arp.undertone -= 1;
                        sub_arp.up_slope = false;
                    }
                    else {
                        sub_arp.undertone = 2;
                        sub_arp.up_slope = true;
                    }
                    sub_arp.reset = 1;
                }
                if (wave == 1.f)
                    sub_arp.reset = 0;
                break;
            }
            case off:
                sub_arp.undertone = 1;
                break;
        }
    }
    return sub_arp.undertone;
}

void OSC_CYCLE(const user_osc_param_t * const params, int32_t *yn, const uint32_t frames) {
  const float w1 = osc_w0f_for_note(((params->pitch)>>8) + oscillator.semitone, params->pitch & 0xFF);
  float sw1;
  float sw2;

  if (sub_arp.arp_type == off) {
      sw1 = w1 / oscillator.undertone1;
      sw2 = w1 / oscillator.undertone2;
  }
  else
      sw1 = sw2 = w1 / arp_undertone();

  // LFO
  float lfo = q31_to_f32(params->shape_lfo);

  q31_t * __restrict y = (q31_t *)yn;
  
  const q31_t * y_e = y + frames;
	
  for (; y != y_e; ) {
      // Main harmonic
      const float sig1 = signal(oscillator.phase, oscillator.gain + lfo);
      oscillator.phase += w1;
      oscillator.phase -= (uint32_t)oscillator.phase;
      // Sub harmonic 1
      const float subsig1  = signal(oscillator.sub_phase1, oscillator.sub_gain);
      oscillator.sub_phase1 += sw1;
      oscillator.sub_phase1 -= (uint32_t)oscillator.sub_phase1;
      // Sub harmonic 2
      const float subsig2  = signal(oscillator.sub_phase2, oscillator.sub_gain);
      oscillator.sub_phase2 += sw2;
      oscillator.sub_phase2 -= (uint32_t)oscillator.sub_phase2;

      *(y++) = f32_to_q31(sig1 + subsig1 + subsig2);
  }
}

void OSC_NOTEON(const user_osc_param_t * const params) {
    s_lfo.reset();
    sub_arp.undertone = 1;
    sub_arp.reset = 0;
    sub_arp.up_slope = true;
    roll = true;
}

void OSC_NOTEOFF(const user_osc_param_t * const params){
    roll = false;
    sub_arp.reset = 0;
    s_lfo.reset();
}

void OSC_PARAM(uint16_t index, uint16_t value) {
  switch (index) {
  case k_user_osc_param_id1:
      oscillator.semitone = value;
      break;
  case k_user_osc_param_id2:
      oscillator.undertone1 = value;
      break;
  case k_user_osc_param_id3:
      oscillator.undertone2 = value;
      break;
  case k_user_osc_param_id4:
      oscillator.type = value;
      break;
  case k_user_osc_param_id5:
  {
      switch (value) {
          case 0:
              sub_arp.arp_type = off;
              sub_arp.undertone = 1;
              sub_arp.reset = 0;
              roll = false;
              s_lfo.reset();
              break;
          case 1:
              sub_arp.arp_type = down;
              roll = true;
              break;
          case 2:
              sub_arp.arp_type = up_down;
              roll = true;
      }
      break;
  }
  case k_user_osc_param_id6:
  {
      float offset = (value == 0) ? 0.f : 30.f;
      s_lfo.setF0(offset + 4000.f * param_val_to_f32(value), k_samplerate_recipf);
      break;
  }
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

