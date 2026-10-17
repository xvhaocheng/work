## Material Passport

- Origin Skill / Mode: academic-research-suite / experiment-agent / validate
- Origin Date: 2026-10-18 JST
- Verification Status: VERIFIED for offline numeric reproduction and rendered figures; no new board execution.
- Version Label: e3_p0_figures_validation01

## Numerical validation

The plotter reads the frozen records snapshot and reconstructs each same-size ratio from hardware cycles, after checking accepted summary/size-effects SHA256s and their binding. It compares all 15 pairs, their Type7 summaries and all 10 within-block size contrasts with the accepted results. It retains the original 45 boot/sequence/command-SHA trial identities, including unplotted Solo cases. Source trial count remains 45.

The separate verifier reads only the 45 individual hardware-cycle rows in the accepted `paper.csv` as its numerical source. It uses exact rational `100(C8−C1)/C1` and `100(C8−C1)/C8` formulas and standard-library inclusive quartiles, rather than the plotter's floating-point ratio and interpolation function. It checks 15 pairs, 10 contrasts and 10 complete statistic sets. The maximum absolute difference is recorded in `numerical-verification.json` (about 2.0e-14; tolerance 1e-10 absolute / 1e-12 relative). This is arithmetic reproduction of accepted data, not an independent hardware replication.

## Statistical fallacy scan: 11/11 covered

| Type | Finding / boundary |
| --- | --- |
| Simpson's paradox | No pooled cross-size ratio: each A8/A1 division has identical size, boot and block; all contrasts retain their block. |
| Ecological fallacy | Individual trial cycles generate pair estimates; no cell mean or ratio of medians substitutes for a pair. Descriptive n=5 does not imply population inference. |
| Berkson's paradox | The frozen 45-case protocol is the selected configuration, not a platform sample. No filtering by observed speed was added. |
| Collider bias | No regression adjustment, outcome-conditioned covariate or conditioning model is used. |
| Base-rate neglect | Not a classifier/diagnostic probability result; not applicable. |
| Regression to the mean | No extreme-case selection or pre/post improvement claim; all five block estimates remain visible. |
| Survivorship bias | Accepted campaign reports all 45 planned cases passed with no failures, replacements or backfill. This work retains all 45; Root's audit is reused. |
| Look-elsewhere effect | No p-values or significance selection. Three fixed P0 sizes and two declared size contrasts are descriptive; unplotted throughput summaries remain available. |
| Garden of forking paths | Explicit fixed formulas and Type7 rule; no outlier exclusion, random point jitter or alternative estimator selection. Figure choice is a descriptive presentation choice. |
| Correlation/causation | A1 and A8 include shared-path/bandwidth/arbitration effects; their difference is not isolated translation cost. Fixed descriptor count also changes V total bytes with size. |
| Reverse causality | No inferred causal direction from a correlation or claim beyond the configured comparison; execution order is retained by the source cases. |

The one-boot scope, changed total V work across sizes and lack of independent repetitions across boots limit inference. No confidence intervals, significant/non-significant claim, stars, pure-translation attribution or platform-wide conclusion is produced.

## Actual rendering review

Both PDFs were rendered at 300 dpi using Poppler and opened for visual inspection. The final render shows all five circles separately, with summary crossbars to their right, no clipped labels, and no point/median-label overlap. The optional plot labels its points as block contrasts. Both pages are 252 × 198 points (3.5 × 2.75 inches); DejaVu Sans is embedded as subset CID TrueType, with no Type3 font. The SVGs retain text and the PNGs are 1050 × 825 pixels. Grayscale encodings do not depend on color perception.

The primary intervals are small (IQR 0.040–0.071 pp) relative to the between-size effect; their exact Q1/Q3 heights are drawn without artificial enlargement and tabulated in the CSV/README. They can appear as short stacked black marks at single-column scale. The primary dot-plot y axis starts at 9.6%, explicitly recorded in the manifest; no bar height encodes a truncated baseline. The optional contrast plot has a zero origin. PDF renders and their hashes are retained under `render-check/` and `render-verification.json`.

SVG is an editable alternative; the embedded-font PDF is the primary manuscript artifact. Changing font/runtime versions can change file hashes and layout, so the actual tested versions are recorded. The frozen input data and shared result documents were not modified.
