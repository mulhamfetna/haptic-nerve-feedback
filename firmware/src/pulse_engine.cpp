#include "pulse_engine.h"

namespace stim {

void PulseEngine::begin(Safety* safety) {
  safety_ = safety;
  pinMode(PIN_HB_A, OUTPUT);
  pinMode(PIN_HB_B, OUTPUT);
  pinMode(PIN_ENABLE, OUTPUT);
  outputs_off();

  ledcSetup(AMP_LEDC_CH, AMP_LEDC_FREQ, AMP_LEDC_RES_BITS);
  ledcAttachPin(PIN_AMP_PWM, AMP_LEDC_CH);
  set_amplitude_pct(0);

  analogReadResolution(12);

  xTaskCreatePinnedToCore(task_trampoline, "pulse", 4096, this,
                          configMAX_PRIORITIES - 2, &task_, 1);
}

bool PulseEngine::start_event(const StimParams& p) {
  if (running_) return false;
  if (validate(p) != ParamError::OK) return false;
  if (!safety_ || !safety_->begin_event()) return false;
  active_ = p;                 // engine works on its own copy for the whole event
  abort_request_ = false;
  run_request_ = true;
  return true;
}

void PulseEngine::abort_event() {
  abort_request_ = true;
  outputs_off();               // belt: kill outputs immediately from any context
}

void PulseEngine::task_trampoline(void* self) {
  static_cast<PulseEngine*>(self)->task_loop();
}

void PulseEngine::task_loop() {
  for (;;) {
    if (run_request_) {
      run_request_ = false;
      running_ = true;
      run_event();
      running_ = false;
      outputs_off();
      if (safety_) safety_->end_event();
      telem_.events_total = telem_.events_total + 1;
    }
    vTaskDelay(pdMS_TO_TICKS(5));
  }
}

void PulseEngine::run_event() {
  const StimParams& p = active_;
  const uint32_t t_start = millis();
  const uint32_t period_us = 1000000UL / p.freq_hz;
  const uint32_t intra_us =
      p.burst.n > 0 ? 1000000UL / p.burst.fi_hz : period_us;

  if (mode_ == OutputMode::GATE_EXTERNAL) {
    // Amplitude ramp, then hold the series switch closed for the duration.
    digitalWrite(PIN_ENABLE, HIGH);
    while (millis() - t_start < p.dur_ms) {
      if (abort_request_ || !safety_ ||
          safety_->state() != State::STIMULATING) break;
      uint32_t el = millis() - t_start;
      uint8_t amp = el < RAMP_MS
          ? (uint8_t)((uint32_t)p.amp_pct * el / RAMP_MS) : p.amp_pct;
      set_amplitude_pct(amp);
      vTaskDelay(pdMS_TO_TICKS(10));
    }
    set_amplitude_pct(0);
    digitalWrite(PIN_ENABLE, LOW);
    return;
  }

  // WAVE_INTERNAL: shape every pulse ourselves.
  digitalWrite(PIN_ENABLE, HIGH);
  delayMicroseconds(DEAD_TIME_US);

  while (millis() - t_start < p.dur_ms) {
    if (abort_request_ || !safety_ ||
        safety_->state() != State::STIMULATING) break;

    uint32_t el = millis() - t_start;
    uint8_t amp = el < RAMP_MS
        ? (uint8_t)((uint32_t)p.amp_pct * el / RAMP_MS) : p.amp_pct;

    const uint32_t n = p.burst.n > 0 ? p.burst.n : 1;
    const uint32_t cycle_t0 = micros();
    for (uint32_t i = 0; i < n && !abort_request_; ++i) {
      one_biphasic_pulse(p.pw_us, amp);
      telem_.pulses_total = telem_.pulses_total + 1;
      if (telem_.overcurrent) {
        if (safety_) safety_->fault("overcurrent");
        return;
      }
      if (i + 1 < n) {
        // wait out the remainder of the intra-burst period
        uint32_t next = cycle_t0 + (i + 1) * intra_us;
        while ((int32_t)(next - micros()) > 0) { /* spin, sub-ms */ }
      }
    }
    if (p.burst.n > 0) {
      // burst mode: repeat rate = burst body + gap (freq_hz ignored)
      vTaskDelay(pdMS_TO_TICKS(p.burst.gap_ms));
    } else {
      // wait out the remainder of the period (ms part sleeps, tail spins)
      uint32_t next = cycle_t0 + period_us;
      int32_t remain = (int32_t)(next - micros());
      if (remain > 2000) vTaskDelay(pdMS_TO_TICKS((remain - 1000) / 1000));
      while ((int32_t)(next - micros()) > 0 && !abort_request_) { /* spin */ }
    }
  }
}

// One charge-balanced biphasic pulse, cathodic-first. Both phases use the same
// "stamp t0, do work, busy-wait to t0+pw" structure so the ADC read in phase A
// cannot make the phases asymmetric.
void PulseEngine::one_biphasic_pulse(uint32_t pw_us, uint8_t amp_duty_pct) {
  set_amplitude_pct(amp_duty_pct);

  uint32_t t0 = micros();
  digitalWrite(PIN_HB_A, HIGH);
  float ma = read_current_ma();                 // ~50-100 us, inside the phase
  while ((uint32_t)(micros() - t0) < pw_us) {}
  digitalWrite(PIN_HB_A, LOW);

  telem_.last_peak_ma = ma;
  if (ma > SW_CURRENT_LIMIT_MA) telem_.overcurrent = true;

  delayMicroseconds(INTERPHASE_GAP_US);

  t0 = micros();
  digitalWrite(PIN_HB_B, HIGH);
  while ((uint32_t)(micros() - t0) < pw_us) {}
  digitalWrite(PIN_HB_B, LOW);

  delayMicroseconds(DEAD_TIME_US);
}

void PulseEngine::outputs_off() {
  digitalWrite(PIN_HB_A, LOW);
  digitalWrite(PIN_HB_B, LOW);
  digitalWrite(PIN_ENABLE, LOW);
  set_amplitude_pct(0);
}

void PulseEngine::set_amplitude_pct(uint8_t pct) {
  if (pct > 100) pct = 100;
  uint32_t duty = ((1u << AMP_LEDC_RES_BITS) - 1) * pct / 100;
  ledcWrite(AMP_LEDC_CH, duty);
}

float PulseEngine::read_current_ma() {
  // Low-side shunt: V = I * SHUNT_OHMS. analogReadMilliVolts is calibrated.
  uint32_t mv = analogReadMilliVolts(PIN_ISENSE);
  return (float)mv / SHUNT_OHMS;  // mV / ohm == mA
}

}  // namespace stim
