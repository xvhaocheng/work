## Bounded diagnostic proposal for `linux/drivers/misc/idma.c`

### Rationale

The existing batch/observation logs are emitted only after the run has reached `idma_observation_finish()` / `idma_batch_start_log()`, or only on specific error paths. They therefore cannot tell which top-level phase hangs when the frozen composition case stops responding.

This proposal adds one default-false module parameter and 15 short, composition-only stage markers in `idma_run_session2()`. Each marker is guarded by both:

- `composition != NULL` (the composition request path), and
- `idma_composition_trace == true`.

No marker is added inside `idma_batch_start()`’s IRQ/preemption-disabled paired `NEXT_ID` read section. No MMIO, barrier, timing, DMA, memory-layout, UAPI, timeout, or scientific-result changes are made. When the parameter is false, the only added runtime work is the branch around the guard.

### Proposed unified diff

```diff
--- a/linux/drivers/misc/idma.c
+++ b/linux/drivers/misc/idma.c
@@ -955,8 +955,18 @@
 static bool idma_observe;
 module_param_named(observe, idma_observe, bool, 0644);
 MODULE_PARM_DESC(observe, "Collect passive E4 hardware timing (requires observation BIT)");
 static bool idma_path_observe = true;
 module_param_named(path_observe, idma_path_observe, bool, 0644);
 MODULE_PARM_DESC(path_observe, "Require v3 per-transaction path timing in batch experiments");
+
+static bool idma_composition_trace;
+module_param_named(composition_trace, idma_composition_trace, bool, 0644);
+MODULE_PARM_DESC(composition_trace, "Trace composition-session stage boundaries (default false)");
+
+#define idma_comp_trace(comp, dev, stage, ret) \
+	do { \
+		if (unlikely((comp) && idma_composition_trace)) \
+			dev_info((dev), "idma_comp stage=%s ret=%d\n", (stage), (ret)); \
+	} while (0)
 
 struct idma_observation {
@@ -2415,19 +2425,21 @@
 	if (composition) {
 		ret = idma_composition_configure(victim, composition->mode,
 						composition->protected_did, &composition->generation);
+		idma_comp_trace(composition, victim->miscdev.parent, "config-done", ret);
 		if (ret)
 			goto out_put;
 		composition->configured = 1;
 		composition->restore_status = -EINPROGRESS;
 	}
 	ret = idma_stream_prepare(v, victim, e.victim_pages, e.chunk_bytes, &e.victim);
 	if (ret)
 		goto out_cleanup;
 	if (a) {
 		ret = idma_stream_prepare(a, aggressor, e.aggressor_pages,
 					  e.chunk_bytes, &e.aggressor);
 		if (ret)
 			goto out_cleanup;
 		a->reverse = cfg && cfg->reverse;
 	}
+	idma_comp_trace(composition, victim->miscdev.parent, "prepare-done", ret);
 	if (pair) {
@@ -2480,99 +2492,108 @@
 	ret = idma_run_window(v, a, e.warmup_transfers, deadline, false, &end);
 	if (diag) {
 		diag->warm_end_ns = ktime_get_ns();
 		diag->lane[0].warm_in_flight = v->in_flight;
 		diag->lane[0].warm_last_offset = v->offset;
 		diag->lane[0].warm_next_offset = v->next_offset;
 		if (a) {
 			diag->lane[1].warm_in_flight = a->in_flight;
 			diag->lane[1].warm_last_offset = a->offset;
 			diag->lane[1].warm_next_offset = a->next_offset;
 		}
 	}
+	idma_comp_trace(composition, victim->miscdev.parent, "warm-done", ret);
 	if (ret)
 		goto out_drain;
 	/* Every measured V task starts at the same address after warmup. */
 	v->next_offset = 0;
 	v->last_complete_ns = 0;
 	if (a)
 		a->last_complete_ns = 0;
 	if (batch) {
 		/* Software warmup is drained before enabling the autonomous mode. */
 		if (a) {
 			ret = idma_drain(a);
+			idma_comp_trace(composition, victim->miscdev.parent, "warmdrain-done", ret);
 			if (ret)
 				goto out_drain;
 		}
 		if (diag) {
 			diag->warm_drained_ns = ktime_get_ns();
 			idma_snapshot(hpm, v, a, &after, active_after);
 			idma_snapshot_delta(&diag->lane[0].warm_hpm, 0, &before, &after,
 					    active_before[0], active_after[0]);
 			if (a)
 				idma_snapshot_delta(&diag->lane[1].warm_hpm, 1, &before, &after,
 						    active_before[1], active_after[1]);
 			diag->warm_complete = 1;
 		}
 		ret = idma_batch_configure(v, e.victim_transfers);
 		if (!ret && a)
 			ret = idma_batch_configure(a, IDMA_EXPERIMENT_MAX_TRANSFERS);
+		idma_comp_trace(composition, victim->miscdev.parent, "batch-config-done", ret);
 		if (ret)
 			goto out_drain;
 	}
 	if (p4m) {
 		ret = idma_gap_program_readback(v, composition ? composition->victim_gap_cycles : 0);
 		if (!ret && a)
 			ret = idma_gap_program_readback(a,
 						p4m->aggressor_gap_cycles);
+		idma_comp_trace(composition, victim->miscdev.parent, "gap-done", ret);
 		if (ret)
 			goto out_drain;
 	}
 	e.victim.in_flight_at_start = v->in_flight;
 	e.aggressor.in_flight_at_start = a && a->in_flight;
 	if (diag)
 		diag->arm_begin_ns = ktime_get_ns();
 	if (research) {
 		research->mapping[0] = v->src_iova;
 		research->mapping[1] = v->dst_iova;
 		research->mapping[2] = a ? a->src_iova : 0;
 		research->mapping[3] = a ? a->dst_iova : 0;
 		ret = idma_research_begin(victim, research);
 		if (ret)
 			goto out_drain;
 	}
 	if (cfg) {
 		ret = idma_research2_pre_arm(victim, cfg);
 		if (ret)
 			goto out_drain;
 	}
 	ret = idma_observation_arm(&observation, victim, aggressor, e.victim_transfers);
+	idma_comp_trace(composition, victim->miscdev.parent, "arm-done", ret);
 	if (ret)
 		goto out_drain;
 	stamp = ktime_get_ns();
 	if (diag)
 		diag->arm_end_ns = stamp;
 	idma_snapshot(hpm, v, a, &before, active_before);
 	start = ktime_get_ns();
 	e.snapshot_before_ns = start - stamp;
 	e.measurement_started = 1;
+	idma_comp_trace(composition, victim->miscdev.parent, "snapshot-before", 0);
 	ret = batch ? idma_batch_window(v, a, deadline, &end, &batch_start) :
 		idma_run_window(v, a, e.victim_transfers, deadline, true, &end);
 	e.victim_elapsed_ns = end - start;
 	e.measurement_complete = !ret;
+	idma_comp_trace(composition, victim->miscdev.parent, "launch-done", ret);
 	e.victim.in_flight_at_end = v->in_flight;
 	e.aggressor.in_flight_at_end = a && a->in_flight;
 	/* Stop autonomous A after V observation; hardware window is already frozen. */
 	if (batch) {
 		idma_batch_stop(a);
 		if (ret)
 			idma_batch_stop(v);
 	}
+	idma_comp_trace(composition, victim->miscdev.parent, "stop-done", ret);
 	/* No more software A refills after V's final observed completion. */
 	idma_snapshot(hpm, v, a, &after, active_after);
 	e.snapshot_after_ns = ktime_get_ns() - end;
 	idma_snapshot_delta(&e.victim, 0, &before, &after,
 			    active_before[0], active_after[0]);
 	if (a)
 		idma_snapshot_delta(&e.aggressor, 1, &before, &after,
 				    active_before[1], active_after[1]);
+	idma_comp_trace(composition, victim->miscdev.parent, "snapshot-after", ret);
  out_drain:
@@ -2595,11 +2616,12 @@
 	if (batch) {
 		drain_ret = idma_batch_collect(v);
 		if (drain_ret && !ret)
 			ret = drain_ret;
 		drain_ret = idma_batch_collect(a);
 		if (drain_ret && !ret)
 			ret = drain_ret;
 		if (!ret && e.victim.completed != e.victim_transfers)
 			ret = -EIO;
 	}
+	idma_comp_trace(composition, victim->miscdev.parent, "drain-done", ret);
 	riscv_iommu_hpm_release(hpm);
@@ -2619,4 +2641,5 @@
 	observation_ret = idma_observation_finish(&observation, &e, ret);
 	if (observation_ret && !ret)
 		ret = observation_ret;
+	idma_comp_trace(composition, victim->miscdev.parent, "obs-finish-done", ret);
 	if (p4m) {
@@ -2654,9 +2677,11 @@
 	if (composition && composition->configured) {
 		int capture_ret = idma_composition_capture(victim, composition, ret);
 
 		if (!ret)
 			ret = capture_ret;
+		idma_comp_trace(composition, victim->miscdev.parent, "capture-done", capture_ret);
 		composition->restore_status = idma_composition_restore(victim, composition);
 		if (!ret)
 			ret = composition->restore_status;
+		idma_comp_trace(composition, victim->miscdev.parent, "restore-done", composition->restore_status);
 	}
```

### Existing printed logs and why they do not yet distinguish the hang phase

The current file already prints several useful post-run diagnostics:

- `idma_batch_start_log()` is defined at `linux/drivers/misc/idma.c:635-650` and prints `idma_batch_start version=1 seq=... error=%d`. It is called at `linux/drivers/misc/idma.c:2673-2674` and `linux/drivers/misc/idma.c:2694-2695`. This line appears only after the batch/session has reached the end of `idma_run_session2()`, so it cannot localise a hang before that point.
- `idma_observation_finish()` emits batch observation records at `linux/drivers/misc/idma.c:1113-1117` (`idma_batch_obs ... part=time`), `linux/drivers/misc/idma.c:1125-1130` (`part=axi_v`) and `linux/drivers/misc/idma.c:1131-1136` (`part=axi_a`), plus path records at `linux/drivers/misc/idma.c:1139-1143`. It is called from `linux/drivers/misc/idma.c:2619`, again only after the timed window and drain phase.
- `idma_quarantine()` prints `experiment DMA not safely drained` at `linux/drivers/misc/idma.c:406`, but only on a specific failure/quarantine path.
- `idma_verify()` prints the large `experiment data mismatch` line at `linux/drivers/misc/idma.c:793-799`, but only when verification runs after drain and finds a mismatch.
- `idma_probe()` prints `registered %s` at `linux/drivers/misc/idma.c:2941`, which only confirms device registration.

None of these lines is emitted early enough to show whether the frozen composition case hangs in configuration, warm-up, warm drain, gap programming, observation arm, snapshot-before, batch launch/poll, stop, snapshot-after, drain, observation finish, capture or restore. The new `idma_comp stage=...` markers fill exactly that gap.

### Sensitive boundaries

- No changes to `idma_batch_start()` lines `linux/drivers/misc/idma.c:492-622`, especially the IRQ/preemption-disabled paired `NEXT_ID` section at `linux/drivers/misc/idma.c:522-572`.
- No changes inside `idma_batch_window()`, `idma_run_window()`, `idma_drain()`, `idma_observation_finish()`, `idma_composition_capture()` or `idma_composition_restore()`. Adding a trace flag there would change helper interfaces or affect non-composition paths, so the markers are placed around their top-level calls in `idma_run_session2()`.
- No timeout, deadline, measurement-field, status/ret, UAPI field, mapping, MMIO read/write, DMA operation or barrier changes.
- `idma_comp_trace()` is a no-op unless both `composition != NULL` and `idma_composition_trace` is true.
- `idma_composition_trace` defaults to `false`.
- The markers are in ordinary process context, not in the IRQ-disabled paired-trigger section.

### Proposed acceptance checks

- Apply the diff cleanly to the supplied `linux/drivers/misc/idma.c`.
- Build the affected kernel object; confirm no new compiler warnings.
- Boot with default parameters: confirm `/sys/module/idma/parameters/composition_trace` reports `N`, and no `idma_comp stage=` lines appear for ordinary P4M, E3 pair, batch or composition runs.
- Enable with `idma.composition_trace=1` or write `Y` to the parameter, then run the frozen composition case. Expect ordered `idma_comp stage=` lines. The last printed stage identifies the phase in which execution stopped.
- Run non-composition modes with `composition_trace=1`; confirm no `idma_comp stage=` lines are emitted because `composition` is `NULL`.
- Confirm returned `struct idma_composition` fields, `ret`, status values and scientific outputs are unchanged apart from the additional log lines.

### Assumptions and unresolved points

- The first marker is `config-done`. If no `idma_comp stage=` line appears at all, the hang is before or inside `idma_composition_configure()` or earlier setup. If that case occurs and finer resolution is required, an `enter` marker can be added, but it would require removing one of the 15 existing markers to keep the stated budget.
- `warmdrain-done` is emitted inside the `if (a)` block. For the reported A8 composition case, the aggressor exists, so it will be emitted. If composition is ever run without an aggressor, that single marker is skipped.
- `launch-done` covers the entire `idma_batch_window()` call, including launch and polling. This is intentional: adding a marker inside `idma_batch_window()` would require either a helper interface change or a non-composition-specific log. If the hang is inside the poll loop, the last marker will be `snapshot-before` and `launch-done` will be absent.
- The diff is a proposed patch. It has not been applied, compiled, booted, or tested here; the parent/Codex worker must verify application and build results.