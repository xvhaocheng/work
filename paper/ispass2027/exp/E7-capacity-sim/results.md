# E7 capacity simulation results

Real frozen iDMA/IOMMU RTL; FIFO behavioral DDR: 16-cycle head startup, then 8 B/cycle.
No FPGA, CPU, Linux, DS injection, invalidations, partition, or early-hit bypass.
V: one source/destination pair. A1/A8/A64: 1/8/64 pairs (2/16/128 distinct translations).
Ordinary MMIO submits one active COPY per lane; this differs from the board batch frontend.
Both footprints are warmed; V executes 4096 x 4096 B. A keeps submitting through V host completion and drains its last offered COPY.
Each completed COPY checks every source/destination byte, 512 R and W beats, and two B responses.

Calibration: `passed_trend_only`; see [calibration.json](calibration.json) for ratios and missing cases.

| Entries | Condition | V cycles | V/Solo | V/A1 | V walks | A walks | A jobs incl. tail |
|---:|---|---:|---:|---:|---:|---:|---:|
| 16 | Solo | 2519013 | 1.00000 | 0.56524 | 0 | 0 | 0 |
| 16 | A1 | 4456501 | 1.76915 | 1.00000 | 0 | 0 | 4097 |
| 16 | A8 | 4935192 | 1.95918 | 1.10741 | 0 | 5122 | 4097 |
| 16 | A64 | 5186520 | 2.05895 | 1.16381 | 0 | 7810 | 4097 |
| 32 | Solo | 2519013 | 1.00000 | 0.56524 | 0 | 0 | 0 |
| 32 | A1 | 4456501 | 1.76915 | 1.00000 | 0 | 0 | 4097 |
| 32 | A8 | 4456501 | 1.76915 | 1.00000 | 0 | 0 | 4097 |
| 32 | A64 | 5138648 | 2.03995 | 1.15307 | 0 | 7298 | 4097 |
| 64 | Solo | 2519013 | 1.00000 | 0.56524 | 0 | 0 | 0 |
| 64 | A1 | 4456501 | 1.76915 | 1.00000 | 0 | 0 | 4097 |
| 64 | A8 | 4456501 | 1.76915 | 1.00000 | 0 | 0 | 4097 |
| 64 | A64 | 5042904 | 2.00194 | 1.13158 | 0 | 6274 | 4097 |

No confidence interval is inferred from a single deterministic run. Capacity removes residency pressure only when the entire footprint fits.
A zero-walk A1 co-run can still be slower than Solo because payload traffic shares resources. This matrix alone does not separate memory contention from arbitration or head-of-line blocking.
If A8 becomes zero-walk at 32/64 entries, its remaining timing difference from A1 cannot be called pure translation cost.

Reproduce: `python3 prepare.py`; use `run.py --help` to build a new named attempt, then `python3 analyze.py`.
Do not reuse existing run directories. All failed attempts and original manifests are retained.
Actual inputs and commands: [manifests](manifests/), [runs](runs/), [test overlay](test-overlay.patch).
Generated C++, binaries, and objects remain local under ignored `build/`; frozen core RTL is the already-versioned closure referenced by the manifests.
