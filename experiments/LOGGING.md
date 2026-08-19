# Trial log schema (`LOG.csv`)

One row per stimulation trial. Session-level facts repeat per row (simplest to analyze).
Units are fixed: µs, Hz, mA, s, mm. Free text only in `notes`.

| Column | Type / values | Meaning |
|--------|---------------|---------|
| `session_id` | `S001`… | one sitting of the protocol |
| `date` | YYYY-MM-DD | |
| `trial` | int | trial number within session |
| `hardware` | text | e.g. `TENS7000`, `TENS7000+openEMSstim`, `OpenXstim` |
| `electrode_type` | text | e.g. `hydrogel`, `gold-plated`, `concentric` |
| `electrode_size_mm2` | number | per contact |
| `site` | text | e.g. `L-index proximal phalanx sides`, `L-wrist median` |
| `spacing_mm` | number | center-to-center electrode distance |
| `cathode_position` | text | which electrode/side is cathodic (e.g. `radial side`) |
| `waveform` | `biphasic-square` \| `biphasic-tri` \| … | monophasic requires explicit justification |
| `pulse_width_us` | number | per phase |
| `frequency_hz` | number | pulse rate (or burst rate if burst mode) |
| `burst` | `none` or `6@200/37` | pulses@internal-Hz/gap-ms |
| `amplitude_ma` | number | best known value; if the device is dial-only, log dial fraction in `notes` |
| `duration_s` | number | stimulation-on time |
| `sensation_threshold_ma` | number | from this session's calibration (repeat per row) |
| `discomfort_threshold_ma` | number | idem |
| `perceived_location` | text | where it was felt, precisely (`fingertip pad`, `under electrodes`, `whole finger`) |
| `referred` | `y`/`n` | felt distal to the electrodes? **The H1 success flag** |
| `quality` | `pressure` \| `tap` \| `vibration` \| `tingle` \| `prick` \| `itch` \| `squeeze` \| `warm` \| `cold` \| `pain` \| `other` | dominant percept (secondary → `notes`) |
| `intensity_0_10` | 0–10 | 0 = nothing, 10 = strongest imaginable non-painful |
| `comfort_0_10` | 0–10 | 10 = fully comfortable, 0 = aborted |
| `minutes_since_start` | number | for tracking impedance/adaptation drift |
| `notes` | text | anything else |

Conventions:
- Log **failed trials too** (no sensation felt) — detection-threshold data is data.
- One parameter change between consecutive trials whenever possible.
- Analysis snippets can live in `experiments/analysis/` later.
