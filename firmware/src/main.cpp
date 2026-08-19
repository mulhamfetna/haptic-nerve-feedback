// H1 neural-haptics stimulator — ESP32-S3 firmware entry point.
//
// SAFETY: this firmware is defense-in-depth ONLY. The hardware chain
// (series R_limit + DC-blocking capacitor, HARDWARE.md) must cap current on its
// own. Run every build against a 1 kOhm dummy load and verify amplitude, pulse
// width, biphasic symmetry, and the worst-case (shorted-load) current BEFORE
// any electrode touches skin. See experiments/PROTOCOL.md.
#include <Arduino.h>

#include "ble_service.h"
#include "config.h"
#include "pulse_engine.h"
#include "safety.h"

static stim::Safety g_safety;
static stim::PulseEngine g_engine;
static stim::BleService g_ble;

static uint32_t g_last_status_ms = 0;
static uint32_t g_btn_down_ms = 0;

void setup() {
  Serial.begin(115200);

  pinMode(PIN_ESTOP, INPUT_PULLUP);       // NC switch to GND; open/press = fault
  pinMode(PIN_BTN_TRIGGER, INPUT_PULLUP); // BOOT button = S1 trigger

  g_safety.begin();
  g_engine.begin(&g_safety);              // boots with all outputs off, amp 0
  g_ble.begin(&g_safety, &g_engine);

  Serial.println("[h1] ready — DISARMED. Connect via BLE, write ARM, then GO.");
}

void loop() {
  // Safety poll: e-stop, heartbeat, connection. Faults abort any running event.
  if (!g_safety.poll(g_ble.connected())) {
    g_engine.abort_event();
  }

  // Physical S1 button: press (debounced) while ARMED fires the configured
  // event — "one button -> one feeling", no phone in the loop.
  if (digitalRead(PIN_BTN_TRIGGER) == LOW) {
    if (g_btn_down_ms == 0) g_btn_down_ms = millis();
    else if (millis() - g_btn_down_ms > 30 && !g_engine.event_running()) {
      g_safety.heartbeat();               // a human at the button counts as alive
      g_engine.start_event(g_ble.current_params());
    }
  } else {
    g_btn_down_ms = 0;
  }

  if (millis() - g_last_status_ms > 500) {
    g_last_status_ms = millis();
    g_ble.notify_status();
  }

  delay(5);
}
