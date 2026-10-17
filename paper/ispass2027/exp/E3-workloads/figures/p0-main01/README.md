## Material Passport

- Origin Skill / Mode: academic-research-suite / experiment-agent / validate
- Origin Date: 2026-10-18 JST
- Verification Status: VERIFIED for offline arithmetic and figure reproduction; hardware was not rerun.
- Version Label: e3_p0_figures_main01

# E3 P0: same-block effects from the accepted 45 trials

**This figure set reuses Root's complete `p0-final01` checkpoint: `source_board_cases=45`, `additional_board_cases=0`.** It is not an additional experiment or sample set, and includes the nine trials in the earlier progress snapshot. The frozen checkpoint remains the data owner; figures, contrasts and CSV rows must not be added to its trial count.

| Figure | PDF for paper | Editable SVG | 300 dpi preview |
| --- | --- | --- | --- |
| Primary: A8/A1 extra time | [PDF](p0-extra-time.pdf) | [SVG](p0-extra-time.svg) | [PNG](p0-extra-time.png) |
| Optional: within-block change from 4 KiB | [PDF](p0-size-contrast.pdf) | [SVG](p0-size-contrast.svg) | [PNG](p0-size-contrast.png) |

Both canvases are 3.5 × 2.75 inches, with 8–8.5 pt text and grayscale encodings. Circles are the five actual block estimates (fixed left-to-right order, blocks 0–4); the separate black crossbar marks the median and capped line marks Type7 Q1–Q3. Tiny primary IQRs are drawn at their true scale. Numeric labels are medians. The primary dot-plot axis begins at 9.6%; the contrast axis begins at zero.

## Caption drafts

**Primary.** DS0 victim extra time, `100(T_A8/T_A1 − 1)`, paired by boot, block and V transfer size (n=5). Hollow circles show the five paired values; black crossbars and capped lines show medians and Type7 Q1–Q3, respectively (IQR, not confidence intervals). Every trial executes 4096 V descriptors, giving 256 KiB, 4 MiB and 16 MiB of effective V work for 64 B, 1 KiB and 4 KiB transfers; only same-size, equal-byte runs are divided. A1/A8 use 4 KiB background transfers over one/eight pages. A1 includes co-run bandwidth/arbitration and shared-path costs; this comparison does not isolate pure translation. Results describe one boot; no significance test is implied.

**Optional contrast.** Within-block change in A8/A1 extra-time percentage relative to 4 KiB. Each hollow circle subtracts the 4 KiB extra-time percentage from the smaller-size percentage in the same boot and block (n=5). Black crossbars and capped lines mark medians and Type7 Q1–Q3 (IQR, not confidence intervals). These contrasts reuse the same P0 trials. All workloads execute 4096 V descriptors: 256 KiB/4 MiB/16 MiB for 64 B/1 KiB/4 KiB, so contrasts compare normalized, same-size A8/A1 effects with different total V work across sizes. A1 already includes co-run bandwidth/arbitration; differences do not isolate pure translation or establish statistical significance.

## Numbers and boundaries

| V size | Paired n | Extra time median (%) | Type7 Q1–Q3 (%) | Throughput loss median (%) |
| --- | --- | --- | --- | --- |
| 64 B | 5 | 14.2460901 | 14.1920746–14.2632868 | 12.4696522 |
| 1 KiB | 5 | 13.8811452 | 13.8760348–13.9157112 | 12.1891514 |
| 4 KiB | 5 | 10.4000223 | 10.3991458–10.4505509 | 9.4203081 |

Throughput loss is independently transformed **per pair** as `100(1 − T_A1/T_A8)` before summarizing; it is not the extra-time percentage. Median within-block extra-time contrasts are 3.8469443 pp (64 B minus 4 KiB) and 3.5156890 pp (1 KiB minus 4 KiB), each n=5. Neither cell means nor ratios of cell medians enter these figures.

[paired-samples.csv](paired-samples.csv) retains all 15 same-size pairs, cycles, case IDs and trial keys; [size-contrasts.csv](size-contrasts.csv) retains all 10 within-block contrasts and four source case IDs per point. [descriptive-statistics.csv](descriptive-statistics.csv) gives the complete min/Q1/median/Q3/max/IQR table, including the unplotted throughput metric. [manifest.json](manifest.json) records input SHA256s, the summary-to-size-effects binding, retained-record binding, all **45 original trial keys**, workload and rendering parameters, script identity and artifact hashes.

This limited P0 packed-ring protocol uses one-page V source/destination, 256 warmup descriptors, one boot and five repeats/cell. It does not cover the later equal-16-MiB workload protocol, other platforms, 16 B transfers, confidence intervals or causal isolation. HPM/A completion counters and PATH maxima are not turned into extra samples or request distributions.

## Reproduce offline

Run from the repository root with an accepted checkpoint directory. No UART/JTAG/SD, hardware build or collection code is used. Inputs are read only; the plotter rejects changed frozen summary or size-effects bytes. `--output` can select a separate artifact directory.

```sh
python3 -m venv /tmp/e3-p0-plot-venv
/tmp/e3-p0-plot-venv/bin/pip install matplotlib==3.10.9 numpy==2.2.6
/tmp/e3-p0-plot-venv/bin/python paper/ispass2027/exp/E3-workloads/plot_p0_figures.py \
  --input /home/xvhaocheng/work-root-a/paper/ispass2027/exp/E3-workloads/checkpoints/p0-final01 \
  --output /tmp/e3-p0-reproduced01
/tmp/e3-p0-plot-venv/bin/python paper/ispass2027/exp/E3-workloads/figures/p0-main01/verify_numbers.py \
  --input /home/xvhaocheng/work-root-a/paper/ispass2027/exp/E3-workloads/checkpoints/p0-final01
```

Actual checks used the existing `/tmp/ispass-ds-sweep-plot-venv/bin/python` (Python 3.10.12, Matplotlib 3.10.9, NumPy 2.2.6). [numerical-verification.json](numerical-verification.json) records an independent `paper.csv` + exact-Fraction formula check, with standard-library inclusive quartiles, rather than importing the plotter. [reproducibility-verification.json](reproducibility-verification.json) records byte-identical regeneration. [validation.md](validation.md) records the statistical-boundary and rendering review; [render-verification.json](render-verification.json) binds the inspected PDF renders. Root's completed raw-source audit was not repeated.

Root preserved the original producer source as [plot_p0_figures.as-executed.py](plot_p0_figures.as-executed.py), whose SHA matches the frozen figure manifest. The current top-level plotter requires an explicit, new `--output` directory and refuses to replace an existing figure set. Its formulas and rendering code are unchanged; the original rendering/reproduction receipts continue to describe the preserved producer version.
