# Project Roadmap — Skin-Bypassing Neural Haptic Feedback

**Goal:** haptic feedback delivered directly to the nervous system by electrical nerve
stimulation — no mechanical actuators, no skin-surface vibration. Strategy: **replicate
existing products first, then push further.**

The primary product to replicate is the **Afference Phantom / Afference Ring** (electrical
stimulation of finger nerves from a ring, producing "referred sensation" at the fingertip).

## Hardware stages (small → big, periphery → toward the CNS)

| Stage | Form factor | Target | Notes |
|-------|-------------|--------|-------|
| H1 | **Single ring** on one finger (fingertip or root of finger) | Evoke ONE distinct sensation (e.g. pressure, tingling, burning) via nerve stimulation only — zero mechanical parts | Proof of concept. This is the Afference-replication stage |
| H2 | **Wrist band** | Whole-hand sensations via median/ulnar/radial nerve stimulation at the wrist (referred sensation into the fingers/palm) | Research shows this works but localization is coarser than at the finger |
| H3 | Moving **up the arm / closer to the CNS** | Larger receptive fields, more of the body per electrode site | |
| H4 (far future) | **Single patch/"sticker" on the spine** | Stimulate the whole sensory system through the spinal nerves | Today this exists as epidural (implanted) spinal cord stimulation research and non-invasive tSCS rehab research — see RESEARCH-LANDSCAPE.md |

## Software stages (single stimulus → full XR integration)

1. **One button → one feeling.** Press a physical button, feel one programmed sensation. Proof of concept.
2. **Expand the sensation vocabulary** (pressure, vibration, tingling, thermal…) — a "sensation library" with tunable pulse width / frequency / amplitude / polarity per sensation.
3. **Camera + normal screen** as the sense source (simple virtual reality on a monitor).
4. **Smartphone AR** — phone camera + screen drive the stimulation.
5. **Smart glasses VR/AR** — sync with games, virtual labs.
6. **Applications:** gaming, virtual labs/simulations, remote robot teleoperation with feedback, prosthetics, sensory recording/playback, virtual floating keyboard and other touchable UI.

(Use cases tracked in `USECASES.md`.)

## Guiding constraints

- Purely electrical stimulation of afferent nerves; the skin is a conductor, not the display.
- Start with off-the-shelf-adjacent, non-invasive, low-cost hardware (TENS-class currents, open-source stimulators) before any custom silicon.
- Safety first: biphasic charge-balanced pulses, current-limited, within TENS-class limits (see safety section of RESEARCH-LANDSCAPE.md).
