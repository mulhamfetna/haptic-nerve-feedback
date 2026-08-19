// Pin map and hard limits — H1 stimulator, ESP32-S3 DevKitC-1.
// The REAL safety layer is hardware (series R_limit + DC-blocking cap, HARDWARE.md);
// everything in firmware is defense-in-depth on top of it, never a replacement.
#pragma once

// ---- Pins -------------------------------------------------------------------
// H-bridge phase inputs (A3 full-DIY mode). Driver must have external pulldowns
// so the bridge is off while the ESP32 boots / is unpowered.
constexpr int PIN_HB_A = 4;       // cathodic-first phase
constexpr int PIN_HB_B = 5;       // anodic (charge-recovery) phase
constexpr int PIN_ENABLE = 6;     // active-high output enable (bridge driver / series
                                  // analog switch in GATE mode). External pulldown.
constexpr int PIN_AMP_PWM = 7;    // LEDC PWM -> RC filter -> boost-rail setpoint
                                  // (A3) or digipot alternative (A1/A2)
constexpr int PIN_ISENSE = 1;     // ADC: low-side current-sense shunt
constexpr int PIN_BTN_TRIGGER = 0;// BOOT button = the S1 "one button, one feeling"
constexpr int PIN_ESTOP = 2;      // normally-closed switch to GND (INPUT_PULLUP).
                                  // Wire break or press -> HIGH -> latched fault.

// ---- Current sensing --------------------------------------------------------
constexpr float SHUNT_OHMS = 100.0f;   // low-side shunt (adjust to build)
constexpr float SW_CURRENT_LIMIT_MA = 7.0f; // software backup limit; hardware
                                            // R_limit must already cap below ~6 mA

// ---- Amplitude PWM ----------------------------------------------------------
constexpr int AMP_LEDC_CH = 0;
constexpr int AMP_LEDC_FREQ = 20000;   // 20 kHz, easy to RC-filter
constexpr int AMP_LEDC_RES_BITS = 12;  // duty 0..4095

// ---- Watchdogs / session rules ---------------------------------------------
constexpr uint32_t HEARTBEAT_TIMEOUT_MS = 3000; // BLE client must ping while ARMED
constexpr uint32_t RAMP_MS = 500;               // amplitude soft-ramp at event start
constexpr uint32_t MIN_EVENT_GAP_MS = 250;      // enforced rest between events
