// Host-side tests for the safety-critical parameter validation (pio test -e native).
// No hardware, no Arduino — params.h is pure C++ by design.
#include <cassert>
#include <cstdio>

#include "../../src/params.h"

using namespace stim;

static int failures = 0;
#define CHECK(cond)                                              \
  do {                                                           \
    if (!(cond)) {                                               \
      ++failures;                                                \
      printf("FAIL %s:%d  %s\n", __FILE__, __LINE__, #cond);     \
    }                                                            \
  } while (0)

int main() {
  // The H1 starting recipe must validate.
  StimParams ok;  // 200 us, 20 Hz, no burst, amp 0, 2 s
  CHECK(validate(ok) == ParamError::OK);

  // Burst recipe from the surveys: 6 @ 200 Hz, 37 ms gap. The repeat rate is
  // derived (30 ms body + 37 ms gap ~= 15 Hz); freq_hz is ignored in burst mode.
  StimParams b = ok;
  b.burst = {6, 200, 37};
  CHECK(validate(b) == ParamError::OK);

  // Hard ceilings.
  StimParams p;
  p = ok; p.pw_us = 49;   CHECK(validate(p) == ParamError::PW_RANGE);
  p = ok; p.pw_us = 501;  CHECK(validate(p) == ParamError::PW_RANGE);
  p = ok; p.freq_hz = 0;  CHECK(validate(p) == ParamError::FREQ_RANGE);
  p = ok; p.freq_hz = 251; CHECK(validate(p) == ParamError::FREQ_RANGE);
  p = ok; p.dur_ms = 0;   CHECK(validate(p) == ParamError::DUR_RANGE);
  p = ok; p.dur_ms = 5001; CHECK(validate(p) == ParamError::DUR_RANGE);
  p = ok; p.amp_pct = 101; CHECK(validate(p) == ParamError::AMP_RANGE);

  // Kaczmarek: >6 pulses per burst is refused.
  p = b; p.burst.n = 7; CHECK(validate(p) == ParamError::BURST_N);

  // Fit invariant: even the extreme corner of the clamp box fits its period
  // (footprint 2*500 + 50 + 40 = 1090 us < 4000 us period @ 250 Hz), so the
  // PERIOD_TOO_SHORT guard is unreachable within the clamps — pure future-
  // proofing in case a ceiling is ever raised.
  p = ok; p.pw_us = 500; p.freq_hz = 250;
  CHECK(validate(p) == ParamError::OK);
  p = ok; p.pw_us = 500; p.burst = {6, 250, 10};
  CHECK(validate(p) == ParamError::OK);

  // Footprint math is what the engine relies on.
  CHECK(pulse_footprint_us(ok) == 2 * 200 + INTERPHASE_GAP_US + 2 * DEAD_TIME_US);

  if (failures == 0) printf("ALL PARAM TESTS PASSED\n");
  return failures == 0 ? 0 : 1;
}
