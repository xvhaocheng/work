# §74 Root acceptance: early V/A metadata handoff

20 new executions: 20 complete-copy PASS. All are 2 KiB; declared CSV par len+1 equals actual driver R and equals E+256 when decided share else 512. Target writes are 256 beats each and all four SHARE byte counters are 2048.

## Final build02 closure pairs (early0 vs early1)

| pair | K | first | V delta | A delta | makespan delta | source R |
|---|---:|---|---:|---:|---:|---:|
| run15/run16 | 64 | a | -12 | -12 | -12 | 320 |
| run17/run18 | 128 | a | -14 | -14 | -14 | 384 |
| run19/run20 | 64 | v | -12 | -12 | -12 | 320 |

Final6 EARLY74-OBS is enabled/moved/actual_ar_before_rawW_closure = 0/0/0 for 15/17/19 and 1/1/1 for 16/18/20. Older initial runs did not print this field; absence there is not evidence of early slack.

Signed deltas are -12/-14/-12 cycles (~0.13%) with identical source R 320/384. Treat this as ordinary DMA pipeline overlap; it is not independent algorithmic novelty.

## Closure build pairs (early0 vs early1)

- run09/run10: CSV exact, metrics exact except trace, K=16, delay=0.
- run11/run12: CSV exact, metrics exact except trace, K=16, delay=1000.
- run13/run14: CSV exact, metrics exact except trace, K=7, delay=0.

## Stage73 sibling controls

Runs01..08 compared to stage73 sibling controls named in result.stage73_control. CSV exact: 8/8; metrics equal except trace: 8/8; argv knobs equal: 8/8. Any mismatches are recorded exactly in root-cardinality-audit.json; no advantage is inferred.

## Scope and attribution

Scope: behavioral DDR and real DMA/IOMMU prototype only. No board, MIG, area, fault, remap, zero-AR-R, or general AXI proof.
Stage74 performance remains unpromising; accepted only actual correctness and existing latency overlap.
Attribution: authored_by=DeepSeek (proposal); root_execution_review=Codex; Codex design review, application, execution.

See root-cardinality-audit.json and execution-ledger.json for exact hashes, snapshots and scopes.
