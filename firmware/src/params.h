// Stimulation parameters + validation. Pure C++ (no Arduino deps) so the safety
// clamps are unit-testable on the host (test/test_params).
//
// Limits follow ELECTROTACTILE-PARAMETERS.md: the field's working envelope is
// 0-6 mA, <=500 us/phase, <=250 Hz; short pulses preferentially recruit touch
// fibers over pain fibers, so we clamp pulse width hard.
#pragma once
#include <cstdint>

namespace stim {

// Hard ceilings — a validated StimParams can never exceed these.
constexpr uint32_t PW_MIN_US = 50;
constexpr uint32_t PW_MAX_US = 500;
constexpr uint32_t FREQ_MIN_HZ = 1;
constexpr uint32_t FREQ_MAX_HZ = 250;
constexpr uint32_t BURST_N_MAX = 6;        // >6 pulses/burst adds nothing (Kaczmarek)
constexpr uint32_t BURST_FI_MAX_HZ = 250;
constexpr uint32_t BURST_GAP_MIN_MS = 10;
constexpr uint32_t BURST_GAP_MAX_MS = 500;
constexpr uint32_t DUR_MAX_MS = 5000;      // one event never exceeds 5 s
constexpr uint8_t  AMP_MAX_PCT = 100;      // % of the hardware-limited ceiling
constexpr uint32_t INTERPHASE_GAP_US = 50; // between cathodic and anodic phase
constexpr uint32_t DEAD_TIME_US = 20;      // H-bridge shoot-through guard

struct BurstCfg {
  uint32_t n = 0;        // pulses per burst; 0 = burst mode off
  uint32_t fi_hz = 200;  // intra-burst pulse rate
  uint32_t gap_ms = 37;  // pause between bursts
};

struct StimParams {
  uint32_t pw_us = 200;    // per phase
  uint32_t freq_hz = 20;   // pulse rate. IGNORED when burst.n > 0: the burst
                           // repeat rate is derived as 1/(burst body + gap),
                           // e.g. 6@200Hz + 37ms -> ~15 Hz (Kaczmarek optimum)
  BurstCfg burst;
  uint8_t  amp_pct = 0;    // 0..100 of hardware ceiling; events start at 0 and ramp
  uint32_t dur_ms = 2000;  // event duration
};

enum class ParamError {
  OK = 0, PW_RANGE, FREQ_RANGE, BURST_N, BURST_FI, BURST_GAP,
  DUR_RANGE, AMP_RANGE, PERIOD_TOO_SHORT,
};

// A full biphasic pulse needs 2*pw + interphase gap + 2*dead time.
inline uint32_t pulse_footprint_us(const StimParams& p) {
  return 2 * p.pw_us + INTERPHASE_GAP_US + 2 * DEAD_TIME_US;
}

inline ParamError validate(const StimParams& p) {
  if (p.pw_us < PW_MIN_US || p.pw_us > PW_MAX_US) return ParamError::PW_RANGE;
  if (p.freq_hz < FREQ_MIN_HZ || p.freq_hz > FREQ_MAX_HZ) return ParamError::FREQ_RANGE;
  if (p.dur_ms == 0 || p.dur_ms > DUR_MAX_MS) return ParamError::DUR_RANGE;
  if (p.amp_pct > AMP_MAX_PCT) return ParamError::AMP_RANGE;
  if (p.burst.n > 0) {
    if (p.burst.n > BURST_N_MAX) return ParamError::BURST_N;
    if (p.burst.fi_hz < FREQ_MIN_HZ || p.burst.fi_hz > BURST_FI_MAX_HZ)
      return ParamError::BURST_FI;
    if (p.burst.gap_ms < BURST_GAP_MIN_MS || p.burst.gap_ms > BURST_GAP_MAX_MS)
      return ParamError::BURST_GAP;
    // pulses inside a burst must fit their intra-burst period; the outer
    // repeat rate is derived (body + gap), so no outer-period check needed
    if (pulse_footprint_us(p) >= 1000000UL / p.burst.fi_hz)
      return ParamError::PERIOD_TOO_SHORT;
  } else {
    if (pulse_footprint_us(p) >= 1000000UL / p.freq_hz)
      return ParamError::PERIOD_TOO_SHORT;
  }
  return ParamError::OK;
}

inline const char* to_string(ParamError e) {
  switch (e) {
    case ParamError::OK: return "ok";
    case ParamError::PW_RANGE: return "pw out of range (50-500 us)";
    case ParamError::FREQ_RANGE: return "freq out of range (1-250 Hz)";
    case ParamError::BURST_N: return "burst n out of range (1-6)";
    case ParamError::BURST_FI: return "burst fi out of range (1-250 Hz)";
    case ParamError::BURST_GAP: return "burst gap out of range (10-500 ms)";
    case ParamError::DUR_RANGE: return "dur out of range (1-5000 ms)";
    case ParamError::AMP_RANGE: return "amp out of range (0-100 %)";
    case ParamError::PERIOD_TOO_SHORT: return "pulses do not fit period";
  }
  return "?";
}

}  // namespace stim
