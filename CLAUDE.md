# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## What this project is

An early-stage **research project to build skin-bypassing haptic feedback** — electrical
stimulation of afferent nerves (no mechanical actuators) that the brain reads as touch.
Strategy: first **replicate** existing products (primarily the Afference neural haptic ring),
then push further. There is no code or build system yet — the content is curated markdown
documents plus a local-only archive of papers and transcripts.

Public repo: **github.com/mulhamfetna/haptic-nerve-feedback** (note: repo name fixes the
typo in the local directory name `haptic-nervouse-feedback`). License: AGPL-3.0-or-later.
The repo is Zenodo-connected: each GitHub Release is archived with a DOI (see
`CITATION.cff` / `.zenodo.json`; bump `version` and `date-released` before tagging a release).

Candidate application areas are listed in `USECASES.md`: prosthetics, gaming, virtual
labs/simulation, remote robot teleoperation with feedback, and sensory recording.

## Structure and conventions

- `ROADMAP.md` — the project vision and staging: replicate the Afference-style neural haptic
  ring first (single finger, single sensation, no mechanical parts), then scale hardware
  (H1 ring → H2 wrist band → H3 toward the CNS → H4 spine patch) and software
  (button-triggered single sensation → sensation library → screen/AR/VR integration).
  Read this first to understand intent.
- `RESEARCH-LANDSCAPE.md` — deep-dive survey of competing products (Afference, OWO,
  Teslasuit), academic labs, patents, open-source stimulator hardware, and safety limits.
- `REFERENCES.md` — the master index of sources, linking to original URLs only.
- `USECASES.md` — running list of target use cases for the research.
- **Local-only archive (gitignored, never commit):** `*.pdf` papers and
  `*transcipt.md`/`*transcpit.md` video transcripts are copyrighted third-party material and
  must not enter the public, DOI-archived repo. `.gitignore` enforces this — don't weaken it.
- GitHub conventions: issue labels `stage:H1`…`stage:H4`, `type:research`, `type:hardware`,
  `safety`; milestones mirror the hardware stages. CI validates `CITATION.cff` and
  link-checks markdown (informational).

## Working here

- When adding a new source, keep the local file if needed but put only the original URL in
  `REFERENCES.md`.
- Some local filenames contain spaces, pipes (`|`), and colons — always quote paths in
  shell commands.
- This directory sits inside the `/mnt/data/projects` multi-project workspace (see the
  workspace-level CLAUDE.md); treat this directory as the project root and this git repo
  as the one to operate on (confirm with `git rev-parse --show-toplevel`).
- Safety rules for any future stimulation work: biphasic charge-balanced pulses only,
  TENS-class current limits, hardware current limiting independent of firmware, no
  trans-thoracic or head electrode placement.
