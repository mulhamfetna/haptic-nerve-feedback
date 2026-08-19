# H1 stimulation session protocol

Every stimulation session follows this protocol. Every trial is logged in `LOG.csv`
(schema in `LOGGING.md`). The log is the S2 sensation-library dataset and future
publication data — no unlogged trials.

## Safety preconditions (all mandatory, every session)

- [ ] Stimulator is a **certified, battery-powered TENS-class device** (or a rig whose
      current is hardware-limited independent of firmware). Never mains-powered, never
      charging during use.
- [ ] **Biphasic, charge-balanced** pulses only.
- [ ] Electrodes on **hand/arm only** — never trans-thoracic (both arms simultaneously
      counts as trans-thoracic: keep both electrodes of a pair on the same limb), never
      neck/head.
- [ ] Amplitude hard cap for finger work: **6 mA**. Session stops at first sharp pain,
      involuntary contraction, or skin irritation.
- [ ] Skin intact (no cuts/eczema at the site); site cleaned; fresh or well-hydrated
      hydrogel electrodes (dry electrodes concentrate current → hot spots).
- [ ] No implanted electronic devices (pacemaker etc.); not pregnant; no known epilepsy.
- [ ] Timer visible: impedance drops up to 4× in the first 15 min — expect the same
      dial setting to feel stronger over time; re-check comfort at ~15 min.

## Session sequence

1. **Setup** — record in the log header: date, hardware, electrode type/size, placement
   (site, spacing, which electrode is cathodic), skin condition.
2. **Calibration (every session, per site)** — step amplitude up slowly from zero at the
   session's default waveform (biphasic square, 200 µs, 20 Hz):
   - record **sensation threshold** (first reliable feeling), then continue in small
     steps to the first "uncomfortable but not painful" level → **discomfort threshold**;
   - working range for the session = threshold + 20% up to ~80% of discomfort;
   - expect thresholds to stabilize from session 2 onward (familiarization effect).
3. **Trials** — one parameter change at a time, following the sweep plan in
   `ELECTROTACTILE-PARAMETERS.md` §7 (frequency bands first: 5–30 Hz, 50–100 Hz,
   150–250 Hz; then pulse width 100–400 µs; then polarity swap; then burst mode
   6 @ 200 Hz / 37 ms). Each trial: stimulate 2–5 s, rest ≥ 10 s, log immediately.
4. **The H1 question, asked every trial:** *where* is the sensation felt — under the
   electrodes, or **referred distally** (fingertip)? `referred=y` trials are the ones
   that replicate the Afference effect.
5. **Wrap-up** — note skin condition after removal, total session time, subjective
   fatigue/adaptation, and any drift observed.

## Stop conditions (abort session immediately)

Sharp/burning pain · involuntary muscle contraction · skin redness beyond mild transient
marks · numbness persisting after stimulation ends · any device fault (unexpected
intensity jump).
