// Safety state machine. DISARMED -> ARMED -> (event) STIMULATING -> ARMED ...
// Any fault latches FAULTED until explicit DISARM + re-ARM.
//
// Faults: e-stop open, BLE heartbeat timeout, BLE disconnect while armed,
// software overcurrent, parameter tampering mid-event.
#pragma once
#include <Arduino.h>
#include "config.h"
#include "params.h"

namespace stim {

enum class State : uint8_t { DISARMED, ARMED, STIMULATING, FAULTED };

class Safety {
 public:
  void begin() { state_ = State::DISARMED; }

  State state() const { return state_; }
  const char* fault_reason() const { return fault_reason_; }

  bool arm() {
    if (state_ != State::DISARMED) return false;
    state_ = State::ARMED;
    last_heartbeat_ms_ = millis();
    return true;
  }

  void disarm() {           // always allowed; clears a latched fault
    state_ = State::DISARMED;
    fault_reason_ = "";
  }

  bool begin_event() {
    if (state_ != State::ARMED) return false;
    uint32_t now = millis();
    if (now - last_event_end_ms_ < MIN_EVENT_GAP_MS) return false;
    state_ = State::STIMULATING;
    return true;
  }

  void end_event() {
    if (state_ == State::STIMULATING) state_ = State::ARMED;
    last_event_end_ms_ = millis();
  }

  void heartbeat() { last_heartbeat_ms_ = millis(); }

  void fault(const char* reason) {
    state_ = State::FAULTED;
    fault_reason_ = reason;
  }

  // Called from the main loop at high rate. Returns true while it is safe to
  // be ARMED/STIMULATING.
  bool poll(bool ble_connected) {
    if (state_ == State::DISARMED || state_ == State::FAULTED)
      return state_ != State::FAULTED;
    if (digitalRead(PIN_ESTOP) == HIGH) { fault("estop"); return false; }
    if (!ble_connected) { fault("ble-disconnect"); return false; }
    if (millis() - last_heartbeat_ms_ > HEARTBEAT_TIMEOUT_MS) {
      fault("heartbeat-timeout");
      return false;
    }
    return true;
  }

 private:
  volatile State state_ = State::DISARMED;
  const char* fault_reason_ = "";
  uint32_t last_heartbeat_ms_ = 0;
  uint32_t last_event_end_ms_ = 0;
};

inline const char* to_string(State s) {
  switch (s) {
    case State::DISARMED: return "disarmed";
    case State::ARMED: return "armed";
    case State::STIMULATING: return "stimulating";
    case State::FAULTED: return "faulted";
  }
  return "?";
}

}  // namespace stim
