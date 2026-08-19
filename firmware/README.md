# H1 stimulator firmware — ESP32-S3

BLE-controlled biphasic stimulation controller for the H1 bench rig (see
`../HARDWARE.md`). Supports both current-source paths:

- **WAVE mode** (A3, full DIY): the ESP32-S3 shapes each charge-balanced biphasic
  pulse on an H-bridge (cathodic-first, 50 µs interphase gap, 20 µs dead time).
- **GATE mode** (A1/A2): a certified TENS/EMS unit makes the waveform; the ESP32-S3
  only closes a series analog switch for the event duration and sets amplitude
  (PWM→RC or digipot), openEMSstim-style.

> ⚠️ **Firmware is defense-in-depth only.** The hardware chain (series R_limit +
> DC-blocking capacitor) must limit current on its own. Verify every build against
> a **1 kΩ dummy load** — amplitude, pulse width, phase symmetry, shorted-load
> current — before any electrode touches skin. Procedure: `../experiments/PROTOCOL.md`.

## Build & flash

```bash
pio run                 # build (PlatformIO, Arduino core, NimBLE + ArduinoJson)
pio run -t upload       # flash over USB
pio device monitor      # 115200 baud
pio test -e native      # host-side tests of the safety-critical param clamps
```

## Wiring (defaults in `src/config.h`)

| GPIO | Signal | Notes |
|------|--------|-------|
| 4 | H-bridge phase A (cathodic) | driver needs external pulldowns |
| 5 | H-bridge phase B (anodic) | |
| 6 | ENABLE (bridge driver / series switch in GATE mode) | external pulldown → off at boot |
| 7 | amplitude PWM (20 kHz, 12-bit) → RC → boost setpoint / digipot | |
| 1 | ADC current sense (low-side 100 Ω shunt) | software backup limit 7 mA |
| 0 | BOOT button = S1 trigger ("one button → one feeling") | |
| 2 | E-stop, **normally-closed switch to GND** | open/press/wire-break ⇒ latched fault |

## BLE protocol (test with nRF Connect)

Device `H1-Stimulator`, service `a5f30001-…`:

| Char | Access | Content |
|------|--------|---------|
| `a5f30002` PARAMS | write | JSON: `{"pw":200,"f":20,"amp":35,"dur":2000}` — optional `"burst":{"n":6,"fi":200,"gap":37}` |
| `a5f30003` CONTROL | write | `ARM` · `GO` · `STOP` · `DISARM` · `PING` · `MODE WAVE` · `MODE GATE` |
| `a5f30004` STATUS | read/notify | JSON: state, fault, active params, pulse/event counters, measured peak mA |

Rules baked in:

- Params are clamped to the survey envelope (`src/params.h`): PW 50–500 µs,
  1–250 Hz, burst ≤ 6 pulses, event ≤ 5 s, amplitude in % of the
  **hardware-limited** ceiling. Invalid sets are rejected atomically.
- In burst mode the repeat rate is *derived* (body + gap): the classic
  6 @ 200 Hz + 37 ms ⇒ ~15 Hz, which also satisfies the <15 Hz anti-adaptation rule.
- State machine `DISARMED → ARMED → STIMULATING`; any fault latches until
  `DISARM`. Faults: e-stop, BLE disconnect while armed, missing heartbeat
  (any write, or `PING`, ≥ every 3 s), software overcurrent (>7 mA on the shunt).
- Every event starts at amplitude 0 and ramps over 500 ms; ≥ 250 ms enforced
  rest between events; params locked while an event runs.
- All outputs boot low; amplitude boots at 0.

## Bench bring-up sequence

1. Dummy load (1 kΩ) in place of electrodes; scope or multimeter across it.
2. Flash, connect with nRF Connect, write `ARM`, then `GO` with defaults
   (200 µs / 20 Hz / amp 10% / 2 s). Verify shape, width, symmetry.
3. Raise `amp` stepwise; log measured peak mA from STATUS; confirm the
   hardware R_limit caps the current with the load **shorted**.
4. Only after 1–3 pass: first skin session per `../experiments/PROTOCOL.md`,
   with the physical BOOT button as the S1 trigger.
