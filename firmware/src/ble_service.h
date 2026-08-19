// BLE GATT interface (NimBLE).
//
// Service  a5f30001-... "NeuroHaptics H1"
//   a5f30002 PARAMS  (write, JSON)   {"pw":200,"f":20,"amp":35,"dur":2000,
//                                     "burst":{"n":6,"fi":200,"gap":37}}
//   a5f30003 CONTROL (write, text)   ARM | DISARM | GO | STOP | PING |
//                                    MODE WAVE | MODE GATE
//   a5f30004 STATUS  (read/notify, JSON) state, params, telemetry, fault
//
// Rules enforced here (on top of Safety):
//  - PARAMS writes are rejected while an event is running.
//  - Any client must PING (or write anything) at least every HEARTBEAT_TIMEOUT_MS
//    while armed, else Safety faults.
//  - Disconnect while armed -> Safety faults (handled in main poll()).
#pragma once
#include <Arduino.h>
#include "params.h"

namespace stim {

class Safety;
class PulseEngine;

class BleService {
 public:
  void begin(Safety* safety, PulseEngine* engine);
  bool connected() const;
  void notify_status();            // call ~2 Hz from loop
  StimParams current_params() const { return params_; }

 private:
  friend class ParamsCb;
  friend class ControlCb;
  Safety* safety_ = nullptr;
  PulseEngine* engine_ = nullptr;
  StimParams params_;              // last accepted parameter set
};

}  // namespace stim
