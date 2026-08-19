// Biphasic pulse generation.
//
// Two output modes:
//  WAVE_INTERNAL (A3, full DIY): the ESP32-S3 shapes each biphasic pulse on the
//    H-bridge pins. Phase symmetry is enforced by construction: both phases run
//    the same timed code path (busy-wait to a fixed per-phase duration), so an
//    ADC read inside phase A cannot skew the charge balance.
//  GATE_EXTERNAL (A1/A2, openEMSstim-style): a certified TENS/EMS unit makes the
//    waveform; we only close a series analog switch (PIN_ENABLE) for the event
//    duration and set amplitude via PIN_AMP_PWM/digipot. H-bridge pins unused.
//
// The engine runs as a dedicated FreeRTOS task pinned to core 1; BLE/loop stay
// on core 0. Events are started by handing it a validated StimParams copy.
#pragma once
#include <Arduino.h>
#include "config.h"
#include "params.h"
#include "safety.h"

namespace stim {

enum class OutputMode : uint8_t { WAVE_INTERNAL, GATE_EXTERNAL };

struct Telemetry {
  volatile uint32_t pulses_total = 0;
  volatile uint32_t events_total = 0;
  volatile float last_peak_ma = 0;   // measured on the sense shunt (phase A)
  volatile bool overcurrent = false;
};

class PulseEngine {
 public:
  void begin(Safety* safety);
  void set_mode(OutputMode m) { mode_ = m; }
  OutputMode mode() const { return mode_; }

  // Copy params and run one event. Returns false if not ARMED / params invalid.
  bool start_event(const StimParams& p);
  void abort_event();          // immediate outputs-off, from any context

  bool event_running() const { return running_; }
  const Telemetry& telemetry() const { return telem_; }

 private:
  static void task_trampoline(void* self);
  void task_loop();
  void run_event();
  void one_biphasic_pulse(uint32_t pw_us, uint8_t amp_duty_pct);
  void outputs_off();
  void set_amplitude_pct(uint8_t pct);
  float read_current_ma();

  Safety* safety_ = nullptr;
  OutputMode mode_ = OutputMode::WAVE_INTERNAL;
  StimParams active_;
  volatile bool run_request_ = false;
  volatile bool abort_request_ = false;
  volatile bool running_ = false;
  Telemetry telem_;
  TaskHandle_t task_ = nullptr;
};

}  // namespace stim
