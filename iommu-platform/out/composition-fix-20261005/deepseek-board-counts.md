Read-only extraction from the six supplied `record.json` files and `scripts/composition_plan.py`. No execution, no access to unprovided files.

## Aliases

- **F1** = `out/composition-board-net3-20261005/mac-records/results/composition-routeopt-net3-20261005-01/cases/explore-b1-g0-a8-ms/record.json`
- **F2** = `.../composition-routeopt-net3-20261005-01/cases/explore-b1-g0-a8-mn/record.json`
- **F3** = `.../composition-routeopt-net3-20261005-01/cases/explore-b1-g0-a8-mpn/record.json`
- **F4** = `.../composition-routeopt-net3-20261005-01/cases/explore-b1-g1000-a1-mn/record.json`
- **F5** = `.../composition-routeopt-net3-20261005-01/cases/explore-b1-g1000-a1-mpn/record.json`
- **F6** = `out/composition-fix-20261005/mac-records/results/composition-fix-20261005-compare01/cases/explore-b1-g1000-a8-ms/record.json`

All six have `success:true` (`:296`), `errno:0` (`:19`), `error:null` (`:20`), `schema:"idma-composition-v1"` (`:285`), `result.status:0` (`:281`), `restore_status:0` (`:279`), `configured:1` (`:38`), `raw_captured:1` (`:278`). All six have `semantics.calibration:"unverified"` (`:288`).

## Field interpretation from `composition_plan.py`

- Raw V normal lookup/hit/miss: `raw[12]`, `raw[13]`, `raw[14]` — `scripts/composition_plan.py:84-86`.
- Raw A normal lookup/hit/miss: `raw[15]`, `raw[16]`, `raw[17]` — `scripts/composition_plan.py:87-89`.
- Raw fast counts: `raw[21]` V fast, `raw[22]` A fast, `raw[23]` other fast — `scripts/composition_plan.py:93-95`.
- Raw observed V gap / A gap: `raw[24]`, `raw[25]` — `scripts/composition_plan.py:96-97`; `raw[24]` is “actual occupied gap” and subtracting nominal gap is not service latency — `scripts/composition_plan.py:475`.
- Raw fast-while-N-disabled: `raw[33]` — `scripts/composition_plan.py:105`; validator rejects nonzero — `scripts/composition_plan.py:395-397`.
- Window/observation V issued/done are the fields the validator constrains to `transfers`: `window[13]`, `window[14]`, `observation[7]`, `observation[8]` — `scripts/composition_plan.py:424-429`.
- `window[3]` is A jobs completed inside the open/close window — `scripts/composition_plan.py:472`.

## Compact numeric table

| Alias | mode/a_pages/reqVgap | V_L | V_H | V_M | A_L | A_H | A_M | V_fast | A_fast | Other_fast | raw24 V_gap | raw25 A_gap | V_comp | A_comp | victim_elapsed_ns | victim_Bps | victim_MiBps |
|---|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|
| F1 | s/8/0 | 16384 | 16384 | 0 | 16382 | 10970 | 5412 | 0 | 0 | 0 | 0 | 0 | 4096 | 4126 | 180764200 | 92,812,714 | 88.513 |
| F2 | n/8/0 | 10240 | 10239 | 1 | 16380 | 13308 | 3072 | 6144 | 2 | 0 | 0 | 0 | 4096 | 4164 | 173217640 | 96,856,279 | 92.369 |
| F3 | pn/8/0 | 8484 | 8484 | 0 | 16384 | 9802 | 6582 | 7900 | 0 | 0 | 0 | 0 | 4096 | 4147 | 201124920 | 83,416,893 | 79.553 |
| F4 | n/1/1000 | 16384 | 16384 | 0 | 32764 | 32764 | 0 | 0 | 0 | 0 | 4095000 | 0 | 4096 | 8702 | 175069600 | 95,831,692 | 91.392 |
| F5 | pn/1/1000 | 16384 | 16384 | 0 | 32764 | 32764 | 0 | 0 | 0 | 0 | 4095000 | 0 | 4096 | 8627 | 173829080 | 96,515,589 | 92.044 |
| F6 | s/8/1000 | 16384 | 16384 | 0 | 21916 | 17474 | 4442 | 0 | 0 | 0 | 4095000 | 0 | 4096 | 10924 | 268616000 | 62,457,992 | 59.565 |

`victim_Bps = (/result/experiment/victim/completed_bytes * 1e9) / /result/experiment/victim_elapsed_ns`; all have `victim.completed_bytes = 16777216` (`:70`). `victim_MiBps = victim_Bps / 1048576`.

## Window/status table

| Alias | raw33 fast_N_disabled | p4m.aggressor_gap_cycles | window[1] cycles | window[3] A_completed_window | window[13]/[14] V issued/done | obs[7]/[8] V issued/done | window[15] has_a | generation -> restored | status/restore/configured/raw_captured |
|---|---:|---:|---:|---:|---:|---:|---:|---:|---:|
| F1 | 0 | 0 | 9015188 | 4096 | 4096/4096 | 4096/4096 | 7 | 77 -> 78 | 0/0/1/1 |
| F2 | 0 | 0 | 8612690 | 4096 | 4096/4096 | 4096/4096 | 7 | 79 -> 80 | 0/0/1/1 |
| F3 | 0 | 0 | 10004608 | 4096 | 4096/4096 | 4096/4096 | 7 | 75 -> 76 | 0/0/1/1 |
| F4 | 0 | 0 | 8405303 | 8191 | 4096/4096 | 4096/4096 | 7 | 103 -> 104 | 0/0/1/1 |
| F5 | 0 | 0 | 8395862 | 8191 | 4096/4096 | 4096/4096 | 7 | 99 -> 100 | 0/0/1/1 |
| F6 | 0 | 0 | 9658789 | 5479 | 4096/4096 | 4096/4096 | 7 | 1 -> 2 | 0/0/1/1 |

## Exact JSON pointer / line citations

Line numbers are identical across F1-F6 for these pointers because the record shape is the same.

- Request: `/request/mode` `:27`, `/request/a_pages` `:22`, `/request/victim_gap_cycles` `:31`, `/request/victim_transfers` `:33`, `/request/warmup_transfers` `:34`.
- Raw: `/result/raw/12` `:161`, `/result/raw/13` `:162`, `/result/raw/14` `:163`, `/result/raw/15` `:164`, `/result/raw/16` `:165`, `/result/raw/17` `:166`, `/result/raw/21` `:170`, `/result/raw/22` `:171`, `/result/raw/23` `:172`, `/result/raw/24` `:173`, `/result/raw/25` `:174`, `/result/raw/33` `:182`.
- Experiment: `/result/experiment/aggressor/completed` `:43`, `/result/experiment/victim/completed` `:69`, `/result/experiment/victim/completed_bytes` `:70`, `/result/experiment/victim_elapsed_ns` `:85`.
- P4M: `/result/p4m/aggressor_gap_cycles` `:89`, `/result/p4m/window/1` `:131`, `/result/p4m/window/3` `:133`, `/result/p4m/window/13` `:143`, `/result/p4m/window/14` `:144`, `/result/p4m/window/15` `:145`, `/result/p4m/observation/7` `:98`, `/result/p4m/observation/8` `:99`.
- Result status: `/result/generation` `:87`, `/result/restored_generation` `:280`, `/result/status` `:281`, `/result/restore_status` `:279`, `/result/configured` `:38`, `/result/raw_captured` `:278`.
- Top-level: `/errno` `:19`, `/error` `:20`, `/schema` `:285`, `/success` `:296`, `/semantics/calibration` `:288`.

## Comparison notes

- **A8 gap 0**
  - S: V_fast 0, A_fast 0; V_L/H/M 16384/16384/0; A_L/H/M 16382/10970/5412; raw V_gap 0; V_comp 4096, A_comp 4126; elapsed 180,764,200 ns; 92,812,714 B/s.
  - N: V_fast 6144, A_fast 2, other_fast 0; V_L/H/M 10240/10239/1; A_L/H/M 16380/13308/3072; raw V_gap 0; V_comp 4096, A_comp 4164; elapsed 173,217,640 ns; 96,856,279 B/s.
  - PN: V_fast 7900, A_fast 0; V_L/H/M 8484/8484/0; A_L/H/M 16384/9802/6582; raw V_gap 0; V_comp 4096, A_comp 4147; elapsed 201,124,920 ns; 83,416,893 B/s.
- **A1 gap 1000**
  - N: V_fast 0, A_fast 0; V_L/H/M 16384/16384/0; A_L/H/M 32764/32764/0; raw V_gap 4,095,000; V_comp 4096, A_comp 8702; elapsed 175,069,600 ns; 95,831,692 B/s.
  - PN: V_fast 0, A_fast 0; V_L/H/M 16384/16384/0; A_L/H/M 32764/32764/0; raw V_gap 4,095,000; V_comp 4096, A_comp 8627; elapsed 173,829,080 ns; 96,515,589 B/s.
- **Diagnostic S gap 1000 A8**
  - V_fast 0, A_fast 0; V_L/H/M 16384/16384/0; A_L/H/M 21916/17474/4442; raw V_gap 4,095,000; V_comp 4096, A_comp 10924; elapsed 268,616,000 ns; 62,457,992 B/s.
- All six records have `raw[25] A_gap = 0` and `p4m.aggressor_gap_cycles = 0`.
- Gap-1000 records all show `raw[24] = 4,095,000` cycles; gap-0 records all show `raw[24] = 0`.

## Fast count scope note

`composition_plan.py` defines raw fast indices but does **not** state that raw fast counts are restricted to the measured window. The window/observation V issued/done fields are constrained to equal `transfers` (`composition_plan.py:424-429`), and in all six records `window[13]=window[14]=4096` and `observation[7]=observation[8]=4096`, matching `transfers=4096`. Raw V/A lookup and fast counters are not forced to `transfers`: e.g. F2 raw V_L=10240 and V_fast=6144; F3 V_L=8484 and V_fast=7900; F4/F5/F6 V_L=16384. Therefore the supplied evidence does **not** support treating raw fast counts as restricted to the 4096-transfer measured window; they may include warmup or other lookups. The plan does not define warmup scope for raw counters. This is an uncertainty.

## Kernel boot mode identity and taint

No supplied record contains `/meta`, `meta`, `exit`, `taint_before`, `taint_after`, or a kernel boot mode identity field. `composition_plan.py` expects such metadata only if passed separately to `_validate_meta` (`scripts/composition_plan.py:442-451`). Therefore exact kernel boot mode identity and taint cannot be extracted from the provided sources. Missing metadata is not treated as a reboot signal; no reboot inference is made.

## Scope / uncertainty / no claims

No root-cause claim, no rerun recommendation, and no single-block performance-benefit claim is made. Throughput values are derived arithmetic from the cited JSON fields; all other values are direct field reads. Raw counter scope relative to warmup versus measured window remains unstated by `composition_plan.py`.