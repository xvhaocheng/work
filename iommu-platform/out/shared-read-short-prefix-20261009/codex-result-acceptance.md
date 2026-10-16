# §75 ordinary ShortPrefix acceptance audit

本报告由 audit75.py 生成。audit75.py 不执行 RTL，不修改 RTL/CPP，不删除旧 run 数据。

- DUT executions read: 29
- Full-copy PASS: 27
- Expected old route-not-bound terminal failures: 2 (run17, run21)
- Invalid CLI attempts excluded: 7 (all exit 2, not DUT)
- DUT exit distribution: {'0': 27, '1': 2}

## Repair pairs

- run22 fixes run17: build=build02-short1, decided=1, share=0, source_R=512.
- run23 fixes run21: build=build02-short0, decided=1, share=0, source_R=512.

## Regression pairs

- run24 vs run06: CSV byte-exact, metrics equal except trace.
- run25 vs run05: CSV byte-exact, metrics equal except trace.

## run29 vs stage74 run08

- run29 build=build02-short0, prior_stage74=run08.
- SOURCE JSON only adds: cutover_after_rv, cutover_after_rv_basis, cutover_after_rv_bytes, terminal_v_rbytes.
- Other metric diffs: none.

## Observer / opportunity note

- SHORT75-OBS was parsed per run. J is the initial observation; terminal cutover_vcut may differ and is not conflated with J.
- CSV source trace has par/rfirst/rlast rows only; it has no per-beat pr rows. The script matches each accepted source PAR burst to a real rfirst/rlast by physical owner ID, address and length, preserving per-ID order. R beat totals are checked against DUT/harness per-R handshake counters, not inferred from per-beat CSV rows.
- A-source bursts are identified by the AXI ID owner bit (ID bit 2 in this fixture: id 33=V, id 37=A), not by an asrc CSV region. The immutable source PA base is taken from the first V rfirst/par and asserted equal to 0x80000000. For short-prefix runs, first A addr==base and len+1==J; the next fallback A burst starts at base+J*8. The fallback length may be E-J or N-J depending on timeout and is recorded, not over-claimed.
- Shared suffix A bursts also carry the A owner ID and are matched as A. The initial repair burst is not counted as the shared suffix fallback.
- opportunities=0 is an observation limited to this fixture, not a theoretical impossibility proof.

## Scope and attribution

Scope: behavioral DDR real RTL fixture. No board/synthesis/MIG/area/fault/remap/zero-AR-R/general proof.
Attribution: DeepSeek 提出 audit75.py 提案；Codex 进行实际审查与执行。本脚本不执行 RTL，不虚构跑过。

See root-cardinality-audit.json and execution-ledger.json for exact hashes, manifests and scopes.
