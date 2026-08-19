# Research Landscape — Neural / Electrotactile Haptics (deep dive, 2026-08-17)

Survey of products, companies, academic labs, patents, open hardware, and safety context for
skin-bypassing haptic feedback via electrical nerve stimulation. Organized from "closest to
what we want to build" outward. See `ROADMAP.md` for how each section maps to our stages.

---

## 1. Direct competitor / replication target: Afference

- **Afference Inc.** (Boulder, CO; founded by neural engineers, CEO/co-founder **Jacob Segil**).
  Their **Phantom** glove (CES 2024 Innovation Award) and follow-on **Afference Ring**
  (CES 2025) stimulate finger nerves electrically from a ring around the **proximal phalanx**,
  producing sensation *at the fingertip* — a phenomenon they brand **"referred sensation."**
  No moving parts, chip-scale, claimed 5–7× less power than any mechanical haptic actuator.
  - Company: https://www.afference.io/ | News: https://www.afference.io/news
  - Funding: **$5M total** ($1.5M pre-seed + $3.5M seed, Oct 2024; Konvoy, Pathway
    Bioventures, Hannah Grey VC, **Samsung Next**) — [Samsung Next rationale](https://www.samsungnext.com/blog/why-we-invested-in-afference), [VC News Daily](https://vcnewsdaily.com/afference/venture-capital-funding/cjzgzsgxcr)
  - Press: [GamesBeat](https://gamesbeat.com/afference-creates-artificial-touch-sensations-with-rings-on-your-fingers/), [Freethink](https://www.freethink.com/ar-vr/device-hacks-nervous-system-to-bring-touch-to-virtual-worlds), [Interesting Engineering (CES award)](https://interestingengineering.com/ces-2024/this-neural-haptic-glove-lets-you-feel-things-that-arent-there)
- **Key patent — read this first when replicating:**
  [US11809629B1 — "Wearable electronic device for inducing transient sensory events as user feedback"](https://patents.google.com/patent/US11809629B1/en)
  (Afference). Claims: electrode array on a finger ring worn on the proximal phalanx,
  transdermal stimulation of a branch of the **median nerve**, evoking a sensory impression
  in the innervated area (fingertip pressure), *not* at the electrode site.
  **Full-text deep read done → [PATENT-US11809629B1.md](./PATENT-US11809629B1.md)**
  (architecture, electrode/materials details, calibration workflow, claims/FTO analysis,
  H1 takeaways).
  Also browse [Jacob Segil's patents](https://patents.justia.com/inventor/jacob-segil) and the
  unrelated-assignee ["Haptic ring" US11285291](https://patents.justia.com/patent/11285291)
  for freedom-to-operate awareness. **We are replicating for research; if this ever becomes a
  product, do a real FTO analysis.**

## 2. Adjacent commercial products (electrical, but coarser — TENS/EMS class)

- **OWO** (Spain) — haptic gaming vest, **TENS-based**, 10 muscle zones, 165+ games; sensations
  from stabs to wind to "low health." https://owogame.com/ | [Hands-on review](https://skarredghost.com/2022/06/05/owo-vest-hands-on-review/)
- **Teslasuit** — full-body **EMS + TENS** suit, 68 haptic points + ECG/EMG biometrics; targets
  enterprise/VR training, not consumers. https://teslasuit.io/
- These prove the market accepts electrical stimulation as consumer haptics, but they stimulate
  broadly through skin/muscle; they do not target specific afferent nerves for localized
  referred touch the way Afference (and we) do.

## 3. Contrast group: mechanical / skin-actuating haptics (what we are NOT building)

For positioning and benchmarks: **HaptX** (microfluidic gloves, 135 actuators),
**SenseGlove** (force feedback, Nova 2 ≈ $5,999), **bHaptics** (vibrotactile, budget),
**Ultraleap** (mid-air ultrasound). Market forecasts: haptic tech ~$16.8B (2021) → $209B (2030),
25.3% CAGR. [Overview](https://augmentedenterprisesummit.com/5-vr-gloves-you-can-buy-or-pre-order-today/), [CB Insights on HaptX](https://www.cbinsights.com/company/axonvr)

## 4. Academic electrotactile research (Stage H1/H2 core literature)

**Surveys — start here:**
- [A Comprehensive Survey of Electrical Stimulation Haptic Feedback in HCI (2025, arXiv)](https://arxiv.org/pdf/2504.21477)
- [Electrotactile feedback for hand/arm interactions: systematic review & meta-analysis (arXiv)](https://arxiv.org/pdf/2105.05343)
- [Virtual tactile feedback via microcurrent stimulation: status, challenges, prospects (Frontiers in Neuroscience, 2025)](https://www.frontiersin.org/journals/neuroscience/articles/10.3389/fnins.2025.1519758/full)
- [Wearable Haptic Feedback Interfaces for Augmenting Human Touch (Adv. Funct. Materials, 2026)](https://advanced.onlinelibrary.wiley.com/doi/10.1002/adfm.202417906)

**Kajimoto Lab (Univ. of Electro-Communications, Tokyo)** — the deepest electrotactile-display
group; our local PDF "Full-hand Electro-Tactile Feedback without Obstructing Palmar Side of
Hand" is from this line of work. Techniques: anodic vs cathodic stimulation to select
mechanoreceptor axons, dense fingertip electrode grids (e.g. 10×6, 1 mm pitch, ESP32 +
high-voltage shift-register drivers — directly reusable architecture for our ring).
[Receptor-selective stimulation (2002)](https://onlinelibrary.wiley.com/doi/abs/10.1002/ecjb.10056), [Electro-tactile display with force feedback](https://www.semanticscholar.org/paper/9f1b5e918794453705ff3dfb4e0a828ce15ae10b)

**Wrist-level stimulation (Stage H2 evidence):**
- [Exploration of sensations evoked during electrical stimulation of the median nerve at the wrist (J. Neural Eng., 2023)](https://iopscience.iop.org/article/10.1088/1741-2552/ad10d0) —
  comfortable referred sensations in palm/index/middle/ring fingers. **This validates the wrist-band stage.**
- [ReaWristic: remote touch to fingers from a wristband via visually-augmented electrotactile feedback (arXiv 2024)](https://arxiv.org/pdf/2410.23193) — cross-modal visual cues raise localized perception (~50% for thumb).
- [Wrist posture unpredictably affects perception of targeted TENS (2024)](https://www.ncbi.nlm.nih.gov/pmc/articles/PMC11667115/) — **key risk for H2:** electrode-nerve geometry shifts with wrist posture.
- [Neurostimulation-based haptic feedback platform for grasp interactions in VR (Frontiers in Virtual Reality, 2022)](https://www.frontiersin.org/journals/virtual-reality/articles/10.3389/frvir.2022.910379/full)
- [Discrimination of phantom finger sensation through TENS (PMC)](https://www.ncbi.nlm.nih.gov/pmc/articles/PMC5937010/)

**Pedro Lopes Lab (Univ. of Chicago; formerly HPI)** — EMS/electrical haptics for HCI: force
feedback, VR walls, smartwatch-integrated stimulators; authors of the CHI 2024
**Haptic Source-effector** paper (full-body haptics via TMS) we already have transcribed.
https://plopes.org/

**Specific sensations ("burning", thermal, etc.) — Stage S1/S2 reality check:**
- Electrotactile parameters (pulse width, frequency, amplitude, polarity) span tingling →
  pressure → vibration; **cold** is easier to evoke than **warmth** (anodic stimulation tends
  cold); thermal percepts are unstable because pain threshold sits close to thermal threshold.
  [Thermal sensation via electrical stimulation (IEEE, 2021)](https://ieeexplore.ieee.org/document/9517195/), [Coldness presentation to forearm (AHs 2023)](https://dl.acm.org/doi/10.1145/3582700.3582713), [Electrotactile improves thermal referral (arXiv)](https://arxiv.org/pdf/2605.00240)
- Implication: for H1's first "single feeling," **pressure/tap at the fingertip is the
  low-risk choice** (it's also what Afference demos); "burning" rides near the pain threshold —
  treat as a later, carefully-consented experiment.

## 5. Sensory substitution precedent

- **BrainPort V100** (Wicab) — 400-electrode **electrotactile tongue display**, camera-to-tongue
  vision substitution; CE 2013, **FDA-cleared 2015**; 69% object-ID after training, no serious
  adverse events. Proof that dense electrotactile arrays can carry rich, learnable information
  streams safely. [FDA clearance](https://www.popsci.com/fda-allows-marketing-device-lets-blind-see-their-tongues/), [Performance study](https://www.tandfonline.com/doi/full/10.1080/17434440.2016.1237287)

## 6. Invasive / clinical research (far stages H3–H4; also our local PDFs' context)

- **Utah Slanted Electrode Array (USEA)** in residual arm nerves: up to **131 distinct
  cutaneous + proprioceptive percepts**, biomimetic feedback improves grip precision.
  [Science Robotics 2019](https://www.science.org/doi/10.1126/scirobotics.aax2352), [USEA percepts study](https://pmc.ncbi.nlm.nih.gov/articles/PMC7819250/)
  (Our local PDF "Biomimetic approaches to bionic touch through a peripheral nerve interface"
  — Saal & Bensmaia, Neuropsychologia 2015 — is the theory companion to this.)
- **Epidural spinal cord stimulation** (Lee Fisher lab, Pitt/UPMC): standard pain-clinic SCS
  implants evoked percepts localized to the **missing hand** in upper-limb amputees, stable
  over ~29 days; intensity scales linearly with amplitude, though percepts are often
  paresthesia rather than natural touch. [eLife 2020](https://elifesciences.org/articles/54349), [UPMC](https://www.upmc.com/media/news/121423-spinal-cord-stimulation)
  **This is the closest existing science to the H4 "spine sticker" idea — but it is implanted.**
- **Transcutaneous spinal cord stimulation (tSCS)** — non-invasive electrodes over the spine
  activate sensory afferents in posterior roots; today used for motor/tactile *rehabilitation*
  (documented tactile-sensation improvements in SCI and peripheral injury), not yet for
  synthetic haptic percepts. The non-invasive H4 path exists but is scientifically open.
  [Review (2022)](https://pmc.ncbi.nlm.nih.gov/articles/PMC9267622/), [Pilot study (2023)](https://www.frontiersin.org/journals/neuroscience/articles/10.3389/fnins.2023.1210328/full)
- **Vagus nerve stimulation** (our IBD transcript) — non-haptic, but the same
  "electrode-to-nerve, no mechanics" principle in clinical use.

## 7. Open-source hardware for Stage H1 replication

- [**OpenXstim**](https://github.com/OpenMedTech-Lab/OpenXstim) — programmable constant-current
  transcutaneous stimulator; up to 128 mA, 96 V compliance per channel. Most capable.
- [**openEMSstim**](https://github.com/PedroLopes/openEMSstim) (Pedro Lopes) — Arduino Nano + BLE
  module that modulates a commercial TENS/EMS unit's amplitude. **Safest fast start: the
  medically-certified TENS box stays in the loop.**
- [**OpenStim**](https://github.com/Open-Stim/openstim) — Arduino shield, biphasic
  constant-current pulses.
- [Compact high-voltage FES device (Arduino Nano ESP32, real-time current monitoring)](https://arxiv.org/html/2412.12064v1);
  [Arduino TENS + smartphone control](https://www.researchgate.net/publication/368057200_A_programmable_transcutaneous_electrical_nerve_stimulation_device_based_on_arduino_and_remote_control_using_a_smartphone)
- Kajimoto-style fingertip arrays: ESP32 + HV shift registers (see §4) for when we need
  spatial patterns, not just one channel.

## 8. Safety (non-negotiable for self-experimentation)

- TENS-class practice: ~6–10 mA typical, ≤25 mA tolerated; current density 0.76–1.3 mA/cm² vs
  a 25 mA/cm² conservative ceiling (McCreery). Device standard: **IEC 60601-2-10** (nerve and
  muscle stimulators) + IEC 60601-1-2 EMC. [Example FDA 510(k) for a TENS/EMS unit](https://www.accessdata.fda.gov/cdrh_docs/pdf8/K083030.pdf)
- Rules we adopt: biphasic charge-balanced pulses only; hardware current limiting independent
  of firmware; never trans-thoracic electrode placement; start sub-threshold and titrate up;
  BrainPort's clean safety record (§5) shows low-current electrotactile is benign when done right.

## 9. Gaps & opportunities (where "push it further" can land)

1. **Open platform:** Afference is closed and pre-market. No open-source neural-haptics ring
   exists. An open, citable research platform (our stack) is itself a contribution.
2. **Sensation vocabulary:** published work maps parameters → sensations ad hoc; a systematic,
   reproducible "sensation library" (S2) with published protocol would be novel.
3. **Wrist-band robustness:** posture sensitivity (§4) is unsolved — adaptive electrode
   selection / closed-loop calibration is an open research problem tailor-made for us.
4. **Cross-modal boosting:** ReaWristic shows visual context sharpens electrically-evoked
   localization — cheap win for the AR/VR software stages.
5. **Non-invasive spine-level haptics (H4):** nobody has shown synthetic touch percepts from
   *transcutaneous* spinal stimulation. High-risk, high-novelty, far horizon.

## 10. Suggested next actions

1. Read the two surveys in §4, the Afference patent (§1), and the wrist median-nerve paper (§4).
2. H1 bench prototype: commercial TENS unit + openEMSstim for modulation + ring-mounted
   hydrogel electrodes at the proximal phalanx; reproduce fingertip referred sensation.
3. Log every stimulation parameter set + perceived sensation from day one — that log becomes
   the S2 sensation library and future publication data.
