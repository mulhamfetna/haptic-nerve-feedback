# Electrotactile parameters → sensations: the working reference

Distilled from full reads (2026-08-19) of the two core surveys, for designing H1 ring
experiments and the S2 sensation library:

- **[S1]** Yang, Wang, Li, Lee, Braud, Hui — *A Comprehensive Survey of Electrical
  Stimulation Haptic Feedback in HCI* (2025, 110 studies, PRISMA) — [arXiv:2504.21477](https://arxiv.org/abs/2504.21477)
- **[S2]** Kourtesis, Argelaguet, Vizcay, Marchal, Pacchierotti — *Electrotactile feedback
  for hand and arm interactions: systematic review & meta-analysis* (IEEE ToH, 78 studies,
  2010–2021) — [arXiv:2105.05343](https://arxiv.org/abs/2105.05343)

Numbers in brackets like [S1-94] mean "reference 94 inside survey S1" (traceable there).

## 1. Receptor physiology (what we are trying to address)

| Receptor | Type | Feels like | Depth | Frequency band | Receptive field |
|----------|------|-----------|-------|----------------|-----------------|
| Merkel | SA-I | indentation / static pressure | basal epidermis | 0.4–100 Hz | ~11 mm² (small, precise) |
| Meissner | FA-I | movement, low-freq vibration | dermal papillae | 10–200 Hz | ~13 mm² (small, precise) |
| Pacinian | FA-II | high-freq vibration | deep dermis | 40–800 Hz | ~101 mm² (large, diffuse) |
| Ruffini | SA-II | skin stretch | dermis | ~7 Hz | ~59 mm² |

Fingertip = highest mechanoreceptor density → most sensitive site (detection threshold
**< 1 mA**); palm center and knuckles are less sensitive (> 1 mA) [S1-119, S1-142].

## 2. Activation physics (Kajimoto/Rattay model — design rules)

- Nerve **parallel to skin** → **cathodic** current depolarizes it; nerve **perpendicular**
  → **anodic** required. Deeper nerves are harder to activate; **wider electrode spacing →
  deeper, more substantial stimulation**; multiple electrodes' activation functions can
  cancel each other.
- **Strength–duration tradeoff:** threshold amplitude is inversely proportional to pulse
  width. Pain fibers are **thinner** than mechanoreceptor axons → **narrow pulses
  preferentially recruit touch fibers over pain fibers**. This is the single most useful
  safety/comfort lever: keep pulses short, raise amplitude, rather than the reverse.
- Perceived-intensity model: **PI = log(PA) − 0.53·log(PW)** (amplitude ~2× the perceptual
  weight of pulse width) [S1-155]. Perception follows Weber–Fechner (nonlinear).

## 3. The parameter → sensation map (core of the S2 sensation library)

**Ranges actually used across 110 studies:** intensity mostly **0–6 mA**, pulse width
**0–400 µs**, frequency **0–300 Hz** [S1 density plots]. Pulse-width classes: short
< 200 µs, medium 200–700 µs, long > 700 µs.

### Frequency (the main "sensation type" dial)

| Frequency | Typical percept |
|-----------|-----------------|
| < 15 Hz bursts | prevents adaptation/tolerance [S2-43] |
| ~2 Hz | "prodding" — most recognizable pattern (84%) [S1-114] |
| < 50 Hz | twitching, pricking, tapping |
| 25 pps | enhances **vibration** perception |
| 50–250 Hz | vibrating, tingling ("stimulating") |
| ~100 pps | reads as **static pressure** |
| > 250 Hz | squeezing, itching (squeezing poorly recognized, 14%) |
| 20–60 pps | roughness (texture); 80–100 pps blends/blurs texture |

- Users reliably distinguish only ~**4 intensity levels** and struggle to tell frequencies
  apart **above ~30 Hz** [S1-122]; frequency matters more than pulse width for perception
  [S1-84]; amplitude helps frequency discrimination, not vice versa [S1-143].
- One electrode, frequency-modulated, can already produce **pressure vs tapping** as two
  distinct percepts [S1-72] — enough for our S1 "one button → one feeling" demo and a
  second feeling for free.
- Bursts: **6 pulses per burst @ 200 Hz internal, ~37 ms between bursts** is the classic
  optimum (Kaczmarek); > 6 pulses/burst adds nothing [S2-44].

### Polarity

- **Cathodic**: stronger overall; stronger **pressure & vibration** [S1-55, S1-133].
- **Anodic**: more likely to read as **cold**; sharper/more localized at high intensity;
  more comfortable in general [S2-42].
- **At the fingertip specifically**, cathodic can turn weak and diffuse — anodic works
  better there [S2-42]. (Matches the patent's per-electrode anode/cathode role assignment.)
- **Biphasic charge-balanced** beats monophasic: more diverse sensations, no half-cell
  reaction, less skin irritation, more comfortable [S2-34, S2-44] — and it is already our
  safety rule.

### Waveform

- **Square pulses dominate** (fastest depolarization, easiest hardware).
- Time-varying/modulated square gives the most dynamic "rhythmic pressing"; constant
  square wave is the weakest and least comfortable variant [S1-94]. Waveform identity is
  itself discriminable (94% accuracy across waveforms) [S1-86].
- Texture: low-frequency + higher voltage ≈ rough (rock); high-frequency + low voltage ≈
  smooth (glass); texture recognition up to 98% [S1-123, S1-132].

### Amplitude / comfort (forearm data, our H1 ceiling guide)

- ~**3–5 mA** moderate and tolerable; **7 mA** rated painful/uncomfortable [S1-66].
- Lower frequencies (~35 Hz) more comfortable than high (~200 Hz) [S1-66].
- Higher intensity/pulse width/frequency all raise urgency + annoyance; valence drops
  above ~30 Hz [S1-122, S1-124].
- EMS-adjacent optimum for "clear but painless" cutaneous feeling: PW 0.2–5 ms,
  45–70 Hz, ≤ 0.5 mA (deeper sensations as PW/f rise) [S1-87].

### Thresholds vary by person (→ per-user profile, exactly as the Afference patent)

Males > females; thresholds rise with age; dominant hand higher than non-dominant;
middle finger most sensitive, then thumb/index [S1-119, S1-78, S1-142]. Sensation and
discomfort thresholds **stabilize after the 2nd calibration session** (familiarization
effect) [S2-138].

## 4. Electrode design rules

- **Size ≥ 10 mm²** — smaller concentrates current density and hurts [S2-34, S2-36].
- **Concentric electrodes**: more localized sensation, less EMG interference, fewer
  painful/prickly events [S1-79]; concentric + conductive graphene ink = flexible,
  reusable ~1 year [S2-139/140]. Flexible printed/tattoo electrodes work but last ~8 h.
- Materials: noble metals / conductive polymers (avoid electrochemical byproducts);
  Ag/AgCl standard; gold-plated superior on the wrist; PEDOT:PSS and silver-nanoparticle
  inkjet printing are the flexible frontier [S1 §III-B].
- Two-point discrimination (electrode pitch guide): fingertip ~7.25 mm classical (down to
  2 mm in recent work), palm 7.7 mm, forearm 8.9 mm, upper arm 9.5 mm [S2-37/38/39].
- Skin: thicker glabrous skin → higher impedance, lower sensitivity; sweat swings
  conductance (sensation dropouts or sudden discomfort); central palm folds → avoid,
  peripheral palm is more receptive [S2-72].

## 5. Impedance & calibration (the repeatability problem)

- Skin impedance **drops up to 4× within 15 minutes** of stimulation and recovers after;
  perspiration and electrode motion shift it continuously [S1-83].
- I²·t alone does not predict perception; **phase charge + peak impedance** correlate
  linearly [S1-83]. Real-time compensation via **pulse-width modulation + impedance
  monitoring** (Kajimoto) and Kalman filtering stabilizes perceived intensity [S1-144].
- Calibration protocol (standard across the field, matches the Afference patent):
  step-wise amplitude ramp per site → record **sensation threshold**, continue →
  **discomfort threshold**; operate in between; recalibrate per session and per site.
- Known automation approaches: Cole–Cole skin-electrode circuit model, fuzzy-logic (TSK)
  preference identification, bio-impedance subdivision mapping [S2-57, S2-58, S2-127].

## 6. Direct evidence for our roadmap stages

- **Referred sensation from the phalanx (H1's principle) is replicated academia-side:**
  stimulating the **middle phalanx** of the index evokes touch perceived **at the
  fingertip** while the fingertip stays free — used for augmented touch on real surfaces
  [S2-96 Yoshimoto; S2-103]. Afference is not magic; it's engineering on this effect.
- **All-five-proximal-phalanges stimulation** already enabled faster-learning **virtual
  typing** [S2-130 Pamungkas & Turnip] — our "virtual floating keyboard" use case has a
  published precursor.
- Electrotactile vs vibrotactile: **sharper, more localized, lower error rates, higher
  spatial resolution**; vibro is comfier and more familiar — electro wins for precision
  tasks [S1 §III-E, S2-114]. Ring/wrist comparison favors electrical for spatial precision
  [S1-117].
- Teleoperation and prosthetics: electrotactile feedback measurably improves grasp force
  control, stiffness discrimination, and embodiment (dozens of studies, incl. amputees)
  [S2 §3.6–3.7] — the strongest-evidence application areas for USECASES.md.
- Field-wide gaps we can attack (both surveys agree): no hardware standardization,
  open-loop rendering, recurrent-calibration burden, small samples (66% of studies N≤10;
  aim **N ≥ 15**), and almost nobody stimulates outside forearm/fingertip.

## 7. H1 starting recipe (synthesis — our first bench protocol)

| Parameter | Start value | Sweep range | Why |
|-----------|-------------|-------------|-----|
| Waveform | biphasic square, charge-balanced, cathodic-first | — | comfort + safety + fast depolarization |
| Pulse width | 200 µs/phase | 100–400 µs | medium class; short PW favors touch fibers over pain fibers |
| Frequency | 20 Hz | 5–30 (tap/pressure), 50–100 (vibration), 150–250 (tingle) | the sensation-type dial |
| Burst mode | 6 pulses @ 200 Hz, 37 ms gap | on/off | anti-adaptation optimum |
| Amplitude | ramp from 0.5 mA | 0.5–6 mA hard cap | fingertip threshold <1 mA; 7 mA = pain on forearm |
| Electrodes | ≥10 mm² hydrogel pair, sides of proximal phalanx | try anodic↔cathodic swap | patent geometry; polarity changes percept |
| Session | calibrate thresholds first, every session | — | impedance drifts 4× in 15 min; stabilizes after 2nd session |

Log every trial per the S2 logging protocol (issue #4): waveform, PW, f, amplitude,
polarity, electrode size/placement, perceived location, quality, intensity 1–10, comfort.

*Both survey PDFs are held in the local research archive (gitignored); links above go to
the originals.*
