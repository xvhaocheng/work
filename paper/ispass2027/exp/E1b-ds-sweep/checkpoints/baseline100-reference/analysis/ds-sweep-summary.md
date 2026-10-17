# E1b DS sweep

## Material Passport

- Origin Skill: academic-research-suite / experiment-agent
- Origin Mode: validate
- Verification Status: ANALYZED (source arithmetic replay; no new board rerun by this script)
- Version Label: ds_sweep_v1

Campaign status: **partial**; accepted 100/200 trials.

Historical DS0/DS50 inputs included: yes. The accepted/200 count includes only explicitly supplied source runs.
DS is the added delay in cycles **in each input/output direction**; DS0 is a direct wire.
All values below are median [Q1, Q3] from actual per-trial values. Quantiles use Type 7: sorted x, h=(n-1)*p; linear interpolation between floor(h) and ceil(h).
IQR error bars are descriptive ranges, not confidence intervals. No significance claim is made.

Extra slowdown pairs Ak and A1 in the same DS and block, computes T_V(Ak)/T_V(A1)-1 per block, then takes the median and quartiles of those ratios.
DS50 blocks 0–4 come from the first boot and 5–9 from the last boot; each pair stays within its boot.

| DS (cycles each direction) | Condition | n | Hardware time (ms) | Throughput (MiB/s) |
| --- | --- | ---: | ---: | ---: |
| 0 | Solo | 10 | 56.538680 [56.501520, 56.564335] | 282.992134 [282.863752, 283.178223] |
| 0 | A1 | 10 | 107.090090 [107.034585, 107.118065] | 149.406916 [149.367897, 149.484397] |
| 0 | A8 | 10 | 118.224410 [118.174270, 118.240790] | 135.335842 [135.317093, 135.393263] |
| 0 | A16 | 10 | 121.735980 [121.632255, 121.908270] | 131.431979 [131.246237, 131.544060] |
| 0 | A64 | 10 | 124.602760 [124.584900, 124.619590] | 128.408071 [128.390729, 128.426479] |
| 25 | Solo | 0 | missing | missing |
| 25 | A1 | 0 | missing | missing |
| 25 | A8 | 0 | missing | missing |
| 25 | A16 | 0 | missing | missing |
| 25 | A64 | 0 | missing | missing |
| 50 | Solo | 10 | 56.549950 [56.478825, 56.628880] | 282.935757 [282.541387, 283.292015] |
| 50 | A1 | 10 | 107.002040 [106.974630, 107.119945] | 149.529861 [149.365275, 149.568174] |
| 50 | A8 | 10 | 154.939610 [154.867270, 154.974725] | 103.266040 [103.242642, 103.314277] |
| 50 | A16 | 10 | 162.780330 [162.734535, 162.826390] | 98.291974 [98.264170, 98.319635] |
| 50 | A64 | 10 | 168.700640 [168.653085, 168.794360] | 94.842558 [94.789897, 94.869300] |
| 100 | Solo | 0 | missing | missing |
| 100 | A1 | 0 | missing | missing |
| 100 | A8 | 0 | missing | missing |
| 100 | A16 | 0 | missing | missing |
| 100 | A64 | 0 | missing | missing |

| DS | Condition / A1 | Pairs | Extra slowdown (ratio) |
| --- | --- | ---: | ---: |
| 0 | A8 | 10 | 0.103962 [0.102943, 0.104688] |
| 0 | A16 | 10 | 0.137155 [0.135466, 0.138466] |
| 0 | A64 | 10 | 0.163512 [0.162988, 0.164085] |
| 25 | A8 | 0 | missing |
| 25 | A16 | 0 | missing |
| 25 | A64 | 0 | missing |
| 50 | A8 | 10 | 0.447831 [0.446825, 0.448259] |
| 50 | A16 | 10 | 0.520975 [0.519882, 0.522166] |
| 50 | A64 | 10 | 0.576313 [0.575175, 0.577469] |
| 100 | A8 | 0 | missing |
| 100 | A16 | 0 | missing |
| 100 | A64 | 0 | missing |

## Sources and limits

Per-trial raw JSON pointers, exact long-table row IDs, pair operands, and input SHA256 values are in [ds-sweep-summary.json](ds-sweep-summary.json).

- DS50 ds50-first: [01-ds50-first-27bd79d9](../raw/01-ds50-first/manifest.json); [import summary](../import/summary.json); boot `9e98d41b-3475-4f5c-b825-ecac880eb8d2`.
- DS0 ds0-middle: [02-ds0-middle-342afe42](../raw/02-ds0-middle/manifest.json); [import summary](../import/summary.json); boot `a816f6c1-b928-4801-98ea-9a55296159a7`.
- DS50 ds50-last: [03-ds50-last-147f5427](../raw/03-ds50-last/manifest.json); [import summary](../import/summary.json); boot `b1eaa85f-01d7-4976-9dac-b57cc8af49b7`.

- DS0 is a direct wire; positive DS values are added input/output delays in cycles each direction.
- Added DS sensitivity is not an estimate of isolated PTW cost.
- Repeated trials within a boot are not independent board restarts; DS50 spans its actual two boots.
- Matching kernel releases do not alone prove payload-byte equality; retain package/load evidence.
- No significance tests or population-level uncertainty claims are made.

**Partial report: figures are withheld. No missing DS point is interpolated or fabricated.**

- 100 of 200 required DS/block/condition points are missing

## Statistical interpretation coverage

11/11 fallacy categories considered within this descriptive analysis:

| Category | Scope disposition |
| --- | --- |
| Simpson's paradox | DS and conditions remain separate; DS50 pooled boot identities are explicit. Boot-level trend equivalence is not claimed. |
| Ecological fallacy | Only this board's measured trial/block summaries are described. |
| Berkson's paradox | Fixed 200-point design; no outcome-based sample selection. |
| Collider bias | No regression or post-outcome covariate adjustment. |
| Base-rate neglect | No diagnostic/classification rates. |
| Regression to the mean | No extreme-outcome selection or pre/post improvement claim. |
| Survivorship bias | Failures and missing points are explicit; any failure prevents a complete curve. |
| Look-elsewhere effect | All fixed DS/conditions reported; no significance testing. |
| Garden of forking paths | Fixed estimators and Type-7 quartiles recorded; this is descriptive analysis, not a preregistered significance test. |
| Correlation versus causation | Added-delay sensitivity only; not isolated PTW attribution. |
| Reverse causality | No directional population claim. |
