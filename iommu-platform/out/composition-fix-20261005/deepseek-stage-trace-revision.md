## Rationale (concise)

- **UAPI untouched**: only an internal `bool composition_trace` is added to `struct idma_stream` (line 132 area), plus a module param and a tiny helper. No ioctl structs touched.
- **Why a field, not the module param**: `idma_batch_start()` runs deep in the call chain (`idma_batch_window → idma_batch_start`) and is also used by non‑composition paths (RUN_BATCH, P4M, E3 pair). Gating inside it on `v->composition_trace` makes the pair markers *composition‑only* without changing its signature or the existing `idma_batch_start_log()` harness.
- **Why snapshot after prepare**: `idma_stream_prepare()` populates `s->dev`/`s->idma` etc.; snapshotting `composition != NULL && READ_ONCE(idma_composition_trace)` into `v->composition_trace` immediately after the victim prepare is guaranteed to see the final stream state.
- **pre-config/post-config before the snapshot**: those two markers precede `idma_stream_prepare()`; `v->composition_trace` is still zero there, so the same value is captured once into a local `comp_trace` at function top and assigned to `v->composition_trace` after prepare. All post‑prepare markers (and `idma_batch_start`) use `v->composition_trace` only.
- **Naming**: the old `launch-done` is renamed `window-return` (it is emitted *after* the poll that ends the window). Requirement‑7's "postwindow" is emitted as `window-return`.
- **No instrumentation inside the IRQ‑off section**: `pair-enter` is emitted immediately *before* `preempt_disable()/local_irq_save()`; `pair-return` immediately *after* the `local_irq_restore()/preempt_enable()` of the normal (non‑`out_untriggered`, non A‑fail) branch. The other two restore sites emit nothing.
- Trace line budget per composition batch case: pre-config, post-config, warm-start, warm-done, batch-config-done, gap-done, arm-done, prewindow, pair-enter, pair-return, window-return, snapshot-after, drain-start, drain-end, obs-finish, restored = **16** lines (≤ 18).

## Changes to actual MMIO / IRQ call sequence

**None.** The patch adds only `dev_info()` calls (composition_trace == true) and one `bool` assignment. Specifically:
- No `readq`/`writeq`/`readq_poll_timeout` added, removed or reordered.
- No `local_irq_save/restore`, `preempt_disable/enable`, barriers, `local_flush_icache_all`, `cond_resched`, or `ktime_get_ns` added/moved.
- The IRQ‑disabled region in `idma_batch_start()` is byte‑for‑byte unchanged between `preempt_disable()` and `preempt_enable()`; both new logs are outside it.
- No `struct idma_experiment` scientific fields, timeouts or thresholds altered.
- `idma_batch_start()` signature and `idma_batch_start_log()` unchanged; all existing calls retained.

## Known trade-off (flag for Codex)

`pair-return` is emitted *immediately* after `preempt_enable()` on the normal branch, i.e. before the `if (!v_id)` validation. If both triggers were attempted but V read back 0 (rare post‑trigger failure), `pair-return` is still logged (with `ret=0`). If Codex wants the label strictly "successful‑pair", move the call to just after the `if (!v_id) { … }` block — but that is no longer "immediately after restore".

## Complete unified diff

```diff
--- a/linux/drivers/misc/idma.c
+++ b/linux/drivers/misc/idma.c
@@ -129,7 +129,8 @@ struct idma_stream {
 	u32 chunk;
 	u64 transfer_id, deadline_ns, last_complete_ns;
 	bool mapped_src, mapped_dst, in_flight, retain;
 	bool batch_configured, batch_started, reverse;
+	bool composition_trace;
 	struct idma_experiment_stats *stats;
 	struct idma_e3_diag_lane *diagnostic;
 };
@@ -464,7 +465,18 @@ static int idma_submit(struct idma_stream *s, bool measured)
 	idma_submit_commit(s, id, measured, now);
 	return 0;
 }
 
+static bool idma_composition_trace;
+module_param_named(composition_trace, idma_composition_trace, bool, 0644);
+MODULE_PARM_DESC(composition_trace, "Trace composition stages (default false)");
+
+static void idma_comp_stage(bool enabled, struct device *dev, const char *stage,
+			    int ret)
+{
+	if (enabled)
+		dev_info(dev, "idma_comp stage=%s ret=%d\n", stage, ret);
+}
+
 /*
  * Bounded software batch start.  Both register sets are configured before any
  * NEXT_ID trigger.  Only the ordered A-then-V trigger reads and their software
@@ -519,6 +531,7 @@ static int idma_batch_start(struct idma_stream *v, struct idma_stream *a,
 	idma_submit_config(v);
 
 	start->attempted = true;
+	idma_comp_stage(v->composition_trace, v->dev, "pair-enter", 0);
 	preempt_disable();
 	local_irq_save(flags);
 
@@ -570,6 +583,7 @@ static int idma_batch_start(struct idma_stream *v, struct idma_stream *a,
 	/* Commit bookkeeping never runs with IRQs or preemption disabled. */
 	local_irq_restore(flags);
 	preempt_enable();
+	idma_comp_stage(v->composition_trace, v->dev, "pair-return", 0);
 
 	if (!v_id) {
 		/*
@@ -2356,6 +2370,8 @@ static long idma_run_session2(struct idma_device *victim,
 	u64 start, end = 0, deadline, stamp;
 	int ret, drain_ret, verify_ret, observation_ret, p4m_ret;
 	unsigned int round = 0;
+	bool comp_trace = composition != NULL &&
+		READ_ONCE(idma_composition_trace);
 
 	memset(&e.status, 0, sizeof(e) - offsetof(struct idma_experiment, status));
 	e.victim_did = victim->did;
@@ -2412,17 +2428,22 @@ static long idma_run_session2(struct idma_device *victim,
 		ret = -EBUSY;
 		goto out_put;
 	}
 	if (composition) {
+		idma_comp_stage(comp_trace, victim->miscdev.parent,
+				"pre-config", composition->mode);
 		ret = idma_composition_configure(victim, composition->mode,
 						 composition->protected_did, &composition->generation);
+		idma_comp_stage(comp_trace, victim->miscdev.parent,
+				"post-config", ret);
 		if (ret)
 			goto out_put;
 		composition->configured = 1;
 		composition->restore_status = -EINPROGRESS;
 	}
 	ret = idma_stream_prepare(v, victim, e.victim_pages, e.chunk_bytes, &e.victim);
 	if (ret)
 		goto out_cleanup;
+	v->composition_trace = comp_trace;
 	if (a) {
 		ret = idma_stream_prepare(a, aggressor, e.aggressor_pages,
 					  e.chunk_bytes, &e.aggressor);
@@ -2472,6 +2493,7 @@ static long idma_run_session2(struct idma_device *victim,
 		idma_snapshot(hpm, v, a, &before, active_before);
 		diag->warm_begin_ns = ktime_get_ns();
 	}
+	idma_comp_stage(v->composition_trace, v->dev, "warm-start", 0);
 	if (a && (!batch || e.warmup_transfers)) {
 		ret = idma_submit(a, false);
 		if (ret)
@@ -2489,6 +2511,7 @@ static long idma_run_session2(struct idma_device *victim,
 			diag->lane[1].warm_next_offset = a->next_offset;
 		}
 	}
+	idma_comp_stage(v->composition_trace, v->dev, "warm-done", ret);
 	if (ret)
 		goto out_drain;
 	/* Every measured V task starts at the same address after warmup. */
@@ -2516,6 +2539,8 @@ static long idma_run_session2(struct idma_device *victim,
 		ret = idma_batch_configure(v, e.victim_transfers);
 		if (!ret && a)
 			ret = idma_batch_configure(a, IDMA_EXPERIMENT_MAX_TRANSFERS);
+		idma_comp_stage(v->composition_trace, v->dev,
+				"batch-config-done", ret);
 		if (ret)
 			goto out_drain;
 	}
@@ -2524,6 +2549,7 @@ static long idma_run_session2(struct idma_device *victim,
 		if (!ret && a)
 			ret = idma_gap_program_readback(a,
 							p4m->aggressor_gap_cycles);
+		idma_comp_stage(v->composition_trace, v->dev, "gap-done", ret);
 		if (ret)
 			goto out_drain;
 	}
@@ -2546,6 +2572,7 @@ static long idma_run_session2(struct idma_device *victim,
 			goto out_drain;
 	}
 	ret = idma_observation_arm(&observation, victim, aggressor, e.victim_transfers);
+	idma_comp_stage(v->composition_trace, v->dev, "arm-done", ret);
 	if (ret)
 		goto out_drain;
 	stamp = ktime_get_ns();
@@ -2555,10 +2582,12 @@ static long idma_run_session2(struct idma_device *victim,
 	start = ktime_get_ns();
 	e.snapshot_before_ns = start - stamp;
 	e.measurement_started = 1;
+	idma_comp_stage(v->composition_trace, v->dev, "prewindow", 0);
 	ret = batch ? idma_batch_window(v, a, deadline, &end, &batch_start) :
 		idma_run_window(v, a, e.victim_transfers, deadline, true, &end);
 	e.victim_elapsed_ns = end - start;
 	e.measurement_complete = !ret;
+	idma_comp_stage(v->composition_trace, v->dev, "window-return", ret);
 	e.victim.in_flight_at_end = v->in_flight;
 	e.aggressor.in_flight_at_end = a && a->in_flight;
 	/* Stop autonomous A after V observation; hardware window is already frozen. */
@@ -2575,8 +2604,10 @@ static long idma_run_session2(struct idma_device *victim,
 	if (a)
 		idma_snapshot_delta(&e.aggressor, 1, &before, &after,
 				    active_before[1], active_after[1]);
+	idma_comp_stage(v->composition_trace, v->dev, "snapshot-after", ret);
  out_drain:
+	idma_comp_stage(v->composition_trace, v->dev, "drain-start", ret);
 	if (batch) {
 		idma_batch_stop(v);
 		idma_batch_stop(a);
 	}
@@ -2602,6 +2633,7 @@ static long idma_run_session2(struct idma_device *victim,
 		if (!ret && e.victim.completed != e.victim_transfers)
 			ret = -EIO;
 	}
+	idma_comp_stage(v->composition_trace, v->dev, "drain-end", ret);
 	riscv_iommu_hpm_release(hpm);
 	if (!ret) {
 		/* Both lanes are drained: reacquire CPU visibility before any check. */
@@ -2619,6 +2651,7 @@ static long idma_run_session2(struct idma_device *victim,
 	observation_ret = idma_observation_finish(&observation, &e, ret);
 	if (observation_ret && !ret)
 		ret = observation_ret;
+	idma_comp_stage(v->composition_trace, v->dev, "obs-finish", ret);
 	if (p4m) {
 		p4m_ret = idma_p4m_finish(&observation, &e, p4m, ret);
 		if (p4m_ret && !ret)
@@ -2659,6 +2692,8 @@ static long idma_run_session2(struct idma_device *victim,
 		composition->restore_status = idma_composition_restore(victim, composition);
 		if (!ret)
 			ret = composition->restore_status;
+		idma_comp_stage(v->composition_trace, v->dev, "restored",
+				composition->restore_status);
 	}
 	if (pair) {
 		if (observation.armed) {
```

## Assumptions

- `struct device` / `dev_info()` are already available (miscdevice.h + existing `dev_info` uses).
- The composition path always passes `batch=true` and `p4m=&p->p4m`, so `idma_batch_window()` (hence `idma_batch_start()`) and the `gap-done` marker are hit exactly once per case; non‑composition paths leave `v->composition_trace == false`.
- `idma_stream_reseed()` does not clear `composition_trace` (it resets only the enumerated fields), which is consistent with composition's single round; if a future path reseeded into `run_round` it would still be correct since composition doesn't take that `goto`.
- `MODULE_PARM_DESC` line is 79 cols; all added lines ≤ 80 with tabs=8.

## Suggested acceptance checks

1. Compile the actual tree (`make <defconfig> && make M=drivers/misc` or full kernel build) with the patch applied — must be warning‑free for `idma.c`.
2. Boot with param absent/`idma.composition_trace=0`, run the composition ioctl: **zero** `idma_comp` lines.
3. Boot with `idma.composition_trace=1`, run composition (mode 0..3, with/without A): expect ≤18 `idma_comp stage=` lines in this order: `pre-config`, `post-config`, `warm-start`, `warm-done`, `batch-config-done`, `gap-done`, `arm-done`, `prewindow`, `pair-enter`, `pair-return`, `window-return`, `snapshot-after`, `drain-start`, `drain-end`, `obs-finish`, `restored` (=16).
4. With `idma.composition_trace=1`, run `RUN_BATCH`, `RUN_P4M`, `RUN_E3_PAIR`, `RUN_E3_CONTROLLED`: expect **zero** `idma_comp` lines.
5. Regression: with param on, verify `idma_batch_start version=1 …`, `idma_batch_obs`, `idma_obs`, `idma_path_obs` outputs and the ioctl result codes are identical to the same runs without the patch (proves no MMIO/IRQ/timing change).
6. Verify the logged `irq_span_ns`/`a_begin_ns`… bracket values are unchanged relative to the unpatched baseline (confirms pair‑enter/pair‑return sit outside the protected region).