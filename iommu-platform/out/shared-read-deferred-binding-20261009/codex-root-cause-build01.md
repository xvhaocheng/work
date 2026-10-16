# Actual §72 build01 A-suffix dependency failure

24 executions: 16 complete-copy PASS and 8 actual watchdog failures (run01, run03, run04, run05, run07, run09, run10, run15). Counts are computed from raw result files.

Root directly inspected failure logs and source. Every failed terminal source state has V returned256/done, A returned exactlyE, no physical busy/offered/skid. A needs a new [E,256) AR after its fixed prefix. phase2 A builder wrongly requires both READ cache_valid0/1 and both WRITE frontiers even on independent route. V logical LAST marks done, top parent_valid[V][READ] drops, hit_bypass valid[V] becomesfalse by its legitimate per-parent generation/validity rule. Therefore no A suffix can be created after faster V completion.

The previous single-A-AR §69 control did not expose this: A entire256 had already been offered before V completed. New common prefix changes this lifetime boundary. This is an out-only prototype bug authored during §72, not a production IOMMU/PN diagnosis.

Fix: on independent A reads require only A successful READ and A certified full WRITE. On shared mode retain both prerequisites. Initial cutover still requires all independently successful grants/equal immutable attributes; no permission is borrowed. A completed V parent need not stay artificially live. Four exact build01 source/executable snapshots preserve every failed run. No failure is hidden; acceptance audit must include every raw execution.
