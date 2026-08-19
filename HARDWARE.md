# H1 bench hardware — shopping list & rig plan

Target: reproduce the referred fingertip sensation (Afference effect) with obtainable
parts, then move to computer-controlled stimulation. Controller platform: **ESP32-S3**
(native BLE + Wi-Fi, RMT/LEDC peripherals for precise pulse trains, plenty of ADC
channels for impedance sensing later — and the Kajimoto-lab electrotactile rigs are
ESP32-based too, so reference firmware concepts port naturally).

## Stage A — current source (three obtainable paths, pick what's available)

### A1 — certified TENS unit (preferred when obtainable)
| Criteria | Notes |
|----------|-------|
| CE/FDA-cleared, battery-powered, dual channel, **adjustable pulse width (≈50–300 µs)** and **frequency (≈2–150 Hz)**, biphasic | TENS 7000-class (~$30–40). Avoid units with only fixed "massage modes". Pharmacies often stock physiotherapy "TENS/EMS combo" units that qualify. |

### A2 — ready-made alternatives if a proper TENS unit is hard to get
- **Pharmacy/sport EMS devices** ("muscle stimulator", "physio massager", Compex/PowerDot
  class): same electrical family. Usable if intensity is adjustable from zero; treat
  unknown waveforms with extra caution (verify on a dummy load first, see below).
- **Bare TENS/EMS module boards** (AliExpress-class, $3–10, sold for DIY massagers):
  a small pulse-transformer board, DC 3–5 V in → biphasic high-voltage pulse train out.
  Electrically this *is* a TENS output stage. Quality varies wildly → **mandatory dummy-load
  characterization before skin**, and add our own series current-limit (below).
- Cheap "butterfly massagers": last resort — fixed modes, no pulse-width control; only
  good for confirming that referred sensation happens at all.

### A3 — full DIY: ESP32-S3-driven stimulator (no TENS unit at all)
Yes, this works — it's exactly what OpenXstim/OpenStim and the 2024 compact-FES paper
(which used an Arduino Nano **ESP32**) do. Minimum safe architecture:

```
battery → boost converter (30–90 V adjustable rail)
        → H-bridge (4 HV MOSFETs/optocouplers) ← gate signals ← ESP32-S3 (RMT/LEDC)
        → series R_limit (12–15 kΩ) → series DC-blocking film cap (100 nF–1 µF)
        → electrodes
        + low-side current-sense resistor → ESP32-S3 ADC  (current + impedance readout)
```

- **R_limit is the hardware safety layer**: with a 90 V rail and 15 kΩ in series, worst-case
  current is 6 mA *even at zero skin impedance, regardless of firmware bugs*. Size it to
  your rail; never bypass it.
- **The series capacitor guarantees zero net DC** through skin even if firmware freezes
  mid-pulse — charge balance in hardware, not software.
- H-bridge polarity flipping gives true biphasic; ESP32-S3 RMT generates µs-accurate
  pulse width/frequency/burst patterns (6@200 Hz/37 ms is trivial).
- Why the boost rail: skin+electrode impedance is ~1–10 kΩ at these pulse widths; pushing
  2–6 mA needs tens of volts of compliance. Cheap single-inductor boosts (MT3608) top out
  ~28 V — marginal; a proper adjustable HV boost or the transformer from an A2 module
  board is better.
- Reference designs to crib from: [OpenXstim](https://github.com/OpenMedTech-Lab/OpenXstim)
  (constant-current, 96 V compliance), [OpenStim](https://github.com/Open-Stim/openstim),
  [compact HV FES device (arXiv 2412.12064)](https://arxiv.org/html/2412.12064v1),
  [MyoPulser](https://www.nature.com/articles/s41598-023-29145-3) (ESP32 variant exists).

**Dummy-load rule (applies to A2 and A3, non-negotiable):** before any electrode touches
skin, run the full stimulation program into a **1 kΩ resistor**, verify amplitude, pulse
width, biphasic symmetry, and worst-case current with a multimeter/scope, and confirm the
series R_limit actually caps the current with the load shorted.

## Electrodes (all stages — globally easy to buy)

Standard **snap/pin hydrogel TENS electrode pads** (pharmacy or AliExpress) are exactly
what we need. Buy the smallest (~⌀25 mm) plus a **cuttable sheet**; cut ~3×5 mm strips for
the finger, keep contact area **≥10 mm²** (current-density comfort limit), rehydrate with
conductive/ECG gel. Later ring arrays: gold-plated contacts (nickel-free).

## Stage B — computer control with ESP32-S3

- **Path with A1/A2 source:** ESP32-S3 + digital potentiometer modulates the TENS/EMS
  unit's amplitude (openEMSstim concept — its Arduino firmware ports to ESP32-S3 easily,
  and the S3's built-in BLE replaces the HM-10 module entirely). Certified/characterized
  device stays in the loop as the safety layer.
- **Path A3:** the ESP32-S3 *is* the stimulator controller from day one — skip straight
  to firmware: parameter struct {pw_us, f_hz, burst, amplitude}, BLE GATT control from
  phone/PC, dummy-load test mode. This is the S1 "one button → one feeling" platform.

## Stage C — research-grade rig (after S1 works)

- Grow the A3 design: constant-current output stage (Howland pump / current mirror)
  instead of plain R_limit-set current, per-pulse impedance logging (we already have the
  sense channel), and a **multi-electrode ring** (4–8 gold contacts, HV multiplexer or
  shift-register-driven switches for software anode/cathode selection — Kajimoto-lab
  architecture, ESP32-native).

## Safety architecture (every stage, every path)

1. Battery power only; never charge during use; no mains-connected gear touching skin.
2. A **hardware current limit independent of firmware** always sits between electronics
   and body (certified unit, or R_limit + DC-blocking cap in DIY).
3. Biphasic charge-balanced only; both electrodes of a pair on the same limb; hand/arm
   sites only; 6 mA cap for finger work; dummy-load verification before every new
   firmware/hardware change reaches skin.
4. Full session checklist: `experiments/PROTOCOL.md`.
