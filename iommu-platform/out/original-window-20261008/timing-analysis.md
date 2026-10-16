# Owner-ready timing addendum

- collection_valid: True
- all_dut_runs_completed: False
- combined counts: {"all_dut_runs_completed": false, "capacity": {"builds": 3, "completed": 8, "runs": 14, "stalled": 6}, "note": "original summary remains 14-run artifact; addendum is combined 16-run; zero extra DUT/model reruns", "timing": {"builds": 1, "completed": 2, "runs": 2, "stalled": 0}, "total": {"builds": 4, "completed": 10, "runs": 16, "stalled": 6}}

## Owner-ready V-first
- {"a_issue_to_done": 27341, "mean_issue_to_done": 25713.0, "order": "v", "v_issue_to_done": 24085}
## Owner-ready A-first mirror
- {"a_issue_to_done": 24085, "mean_issue_to_done": 25713.0, "order": "a", "v_issue_to_done": 27341}
## Raw early2 primary
- {"a": 27342, "mean": 25714.0, "v": 24086}
## Post-window1 comparison
- {"ordinary_minus_post_cycles": 1723.5, "ordinary_owner_ready_mean": 25713.0, "post_reduction_percent_of_ordinary": 6.702835141757088, "post_window1_mean": 23989.5}
## Failures
- none

## Caveats
- No WINDOW-JSON in this pre-WINDOW model; not required/fabricated.
- NATURAL var=64 is two virtual children per parent, not original parent32 at IOMMU port.
- No area-equality, fault/invalidation, board, or production-novelty claims.
- A-first is mirror only, not an independent primary metric.
