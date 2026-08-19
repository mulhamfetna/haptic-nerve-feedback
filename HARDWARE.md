# H1 bench hardware — shopping list & rig plan

Target: reproduce the referred fingertip sensation (Afference effect) with off-the-shelf
parts, then graduate to computer-controlled stimulation. Total budget stage A+B ≈ $60–100.

## Stage A — bare replication rig (order first; blocks everything)

| Item | Selection criteria | Notes |
|------|--------------------|-------|
| **TENS unit** (the current source) | CE/FDA-cleared consumer unit, battery-powered, dual channel, **adjustable pulse width (≈50–300 µs)** and **frequency (≈2–150 Hz)**, biphasic output. | e.g. the TENS 7000 class of devices (~$30–40): adjustable PW 30–260 µs, 2–150 Hz, 0–100 mA into 500 Ω, well documented. Any equivalent with adjustable PW/f qualifies; avoid units with only fixed "massage modes". |
| **Small hydrogel electrodes** | Smallest available reusable pads, ~⌀25 mm or 25×25 mm, 2 mm snap or pin connector, **plus one cuttable sheet** | We cut down to ~10–15 mm² contacts for the finger (never below 10 mm² — current density/comfort limit). Buy spares: small pads dry out fast. |
| **Conductive gel** | standard TENS/ECG gel | Rehydrates pads, improves contact on cut edges |
| Lead wires | 2 mm pin/snap matching the unit | usually included |
| **Multimeter** (if not owned) | any with mA AC range | sanity-check currents; a 1 kΩ 1% resistor in series + voltmeter gives a crude current probe |
| Skin prep | alcohol wipes | site cleaning |

**Rig A assembly:** cut two ~3×5 mm strips from the sheet electrode, place on the radial
and ulnar **sides of the proximal phalanx** of the index finger (patent FIG 9A geometry),
hold with a velcro strap or a 3D-printed open ring. Run the session per
`experiments/PROTOCOL.md`. Success criterion: any reproducible sensation reported at the
**fingertip** (`referred=y` in the log) while electrodes sit at the phalanx.

## Stage B — computer control (order together with A; build after A works)

| Item | Criteria / example | Purpose |
|------|--------------------|---------|
| **Arduino Nano** (or clone) | ATmega328 classic for openEMSstim compatibility | control board |
| **HM-10 / AT-09 BLE module** | UART BLE | phone/PC → stimulator commands |
| **Digital potentiometer / openEMSstim board parts** | per the [openEMSstim BOM](https://github.com/PedroLopes/openEMSstim) | modulates the **certified TENS unit's** amplitude — the medical device stays in the loop as the safety layer |
| Breadboard, jumper wires, 2× AA battery holder | — | |

Rig B = Rig A with openEMSstim between TENS unit and electrodes. This is the minimum for
the S1 demo: a keypress over BLE triggers a pre-tuned pattern → one button, one feeling.

## Stage C — dedicated research stimulator (later, after S1 works)

- **[OpenXstim](https://github.com/OpenMedTech-Lab/OpenXstim)** — open-source
  constant-current stimulator, up to 128 mA / 96 V compliance per channel; real waveform
  control (pulse width, frequency, biphasic timing) instead of amplitude-only modulation.
- Alternative: ESP32 + high-voltage shift-register drivers (Kajimoto-lab architecture)
  when we move to **multi-electrode ring arrays** with software anode/cathode selection.
- Add an **impedance measurement channel** here (voltage sense across a shunt during
  pulses) — the key to session-to-session repeatability (see ELECTROTACTILE-PARAMETERS.md §5).
- Ring mechanics: 3D-printed adjustable ring carrying 4–8 gold-plated contacts ≥10 mm²
  each, elastic inner surface (patent-style tension fit).

## Safety architecture (applies to every stage)

1. Battery power only; never charge during use; no mains connection to anything touching skin.
2. The certified TENS unit (A/B) or a **hardware current limiter independent of
   firmware** (C) is always between the electronics and the body.
3. Biphasic charge-balanced only; both electrodes of a pair on the same limb; hand/arm
   sites only; 6 mA cap for finger work.
4. Full checklist: `experiments/PROTOCOL.md`.
