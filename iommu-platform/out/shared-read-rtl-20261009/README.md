# shared-read RTL stage README

## Build
Use the final RTL sources and `build-argv.json` in this directory. Do not rerun `integrate.py`; it is the initial integration step and is stale. Rebuild directly from JSON argv using Python, without unsafe shell expansion.

## Run
Run binaries with their corresponding `runNN-argv.json`. Logs are `runNN.log`, traces `runNN.csv`. The execution ledger is `execution-ledger.json`.

## Key files
- `shared_pretl_read.sv`: shared physical read ownership and skid.
- `q_write_splitter.sv`: q-W adapter and child B joining.
- `hit_bypass_parent_translation.sv`: parent translation reuse cache.
- `codex-result-acceptance.md`: root acceptance summary.
- `critical-write-recipe-review.md`: targeted review of WRITE recipe and early-resume fence.

## Acceptance
Final accepted runs: 14–21, 24–27. Preserve all failures and excluded runs in place.

Architecture failure snapshots: before-write-recipe/ (run02), before-resume-fence/ (run08/09). Observer failure source: multi_test-before-counter-fix.cpp (run12/13). run22/23 have ignored +ptw-lat and must not be used as latency0 results; run26/27 use actual +ptw-latency=100.

Authors: DeepSeek proposed ordinary C++ fixture/driver/observer and documentation; Codex designed/reviewed the critical RTL, diagnosed dependency cycles, applied proposals and executed all builds/runs. No Claude or FPGA board actions.

