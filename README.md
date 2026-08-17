# haptic-nerve-feedback

[![DOI](https://zenodo.org/badge/REPO_ID.svg)](https://zenodo.org/badge/latestdoi/REPO_ID)
[![License: AGPL-3.0](https://img.shields.io/badge/License-AGPL_v3-blue.svg)](LICENSE)
[![Cite this repo](https://img.shields.io/badge/Cite-CITATION.cff-green.svg)](CITATION.cff)

**Skin-bypassing haptic feedback via electrical nerve stimulation** — an open research
project to make the brain feel touch that isn't there, with **no mechanical actuators**:
the electrode talks directly to the afferent nerves, and the sensation appears where the
nerve innervates (a *referred sensation*), not where the device sits.

## The idea

Conventional haptics vibrate or press the skin. Neural haptics stimulate the nerve
underneath instead — smaller, lower-power, and able to place sensations away from the
device. Products like the Afference Ring have shown this works; this project aims to
**replicate that capability openly, then push it further.**

## Staged plan

**Hardware** (periphery → toward the central nervous system):

1. **H1 — Ring:** single finger, single evoked sensation, zero moving parts.
2. **H2 — Wrist band:** whole-hand sensations via median/ulnar/radial nerve stimulation.
3. **H3 — Up the arm / closer to the CNS.**
4. **H4 (far horizon) — Spine patch:** whole-body sensory interface through spinal nerves.

**Software** (single stimulus → full XR):

1. One button → one feeling (proof of concept)
2. A tunable, reproducible **sensation library**
3. Camera + screen → smartphone AR → smart-glasses VR/AR
4. Applications: gaming, virtual labs, robot teleoperation with feedback, prosthetics,
   virtual touchable UI (e.g. floating keyboards)

Details: [ROADMAP.md](ROADMAP.md) · Use cases: [USECASES.md](USECASES.md)

## Research grounding

A deep survey of the field — products (Afference, OWO, Teslasuit), academic labs
(Kajimoto, Lopes, Fisher), key patents, open-source stimulator hardware, and safety
limits — lives in [RESEARCH-LANDSCAPE.md](RESEARCH-LANDSCAPE.md), with the source index
in [REFERENCES.md](REFERENCES.md).

## Status & honest limitations

- **Current stage: research groundwork (v0.1.x).** No hardware or code exists yet; this
  release archives the project vision, the literature/product landscape, and the plan.
- Electrically-evoked sensations are well-documented for pressure/tingling/vibration;
  thermal and pain-adjacent percepts are unstable and near the pain threshold — the
  sensation vocabulary will start narrow.
- Electrical stimulation of the body carries real risk. All future hardware follows
  TENS-class limits (IEC 60601-2-10 territory), biphasic charge-balanced pulses, and
  hardware current limiting. **Nothing in this repository is medical advice or a medical
  device.**

## Contributing

Issues and discussions are open — see [CONTRIBUTING.md](CONTRIBUTING.md). Research
sources, replication notes, and safety review are especially welcome.

## Citation

If you use this work, please cite it (GitHub's *"Cite this repository"* button uses
[CITATION.cff](CITATION.cff)). A DOI for each release is archived on Zenodo.

## Author

**Mulham Fetna** — robotics / mechatronics engineer & researcher
[Email](mailto:contact@mulhamfetna.com) ·
[ORCID](https://orcid.org/0009-0006-4432-798X) ·
[Google Scholar](https://scholar.google.com/citations?user=h1Yvl2QAAAAJ&hl=en) ·
[ResearchGate](https://www.researchgate.net/profile/Mulham-Fetna) ·
[GitHub](https://github.com/mulhamfetna) ·
[X](https://x.com/MulhamFetna) ·
[Bluesky](https://bsky.app/profile/mulhamfetna.bsky.social) ·
[YouTube](https://www.youtube.com/@eng.mulham-fetna)

## License

[AGPL-3.0-or-later](LICENSE) — derivatives must stay open, including when run as a
network service.
