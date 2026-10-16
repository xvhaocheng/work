Below is a bounded proposal. It does **not** claim execution, FPGA validity, or that the host tests pass. The HPM flush function is assumed to be supplied by the separate worker in `linux/include/linux/riscv_iommu_hpm.h`; `idma.c` calls that header API and does not implement it.

## Proposed unified diff

### `linux/include/uapi/linux/idma.h`

```diff
--- a/linux/include/uapi/linux/idma.h
+++ b/linux/include/uapi/linux/idma.h
@@ -14,6 +14,7 @@
 #define IDMA_EXPERIMENT_CAP_E3_PAIR (1U << 3)
 #define IDMA_EXPERIMENT_CAP_E3_DIAG (1U << 4)
 #define IDMA_EXPERIMENT_CAP_E3_QUEUE_PAIR (1U << 5)
+#define IDMA_EXPERIMENT_CAP_E3_CONTROLLED (1U << 6)
 #define IDMA_BATCH_MAX_TRANSFERS (1ULL << 20)
 /* HPM capability is preliminary; acquire checks slots/filter readback too. */
 /* Neither capability nor configuration establishes board calibration. */
@@ -136,4 +137,31 @@
  * Old D0/D0 operation above is unchanged. */
 #define IDMA_IOC_RUN_E3_QUEUE_PAIR _IOWR('i', 0x0b, struct idma_e3_diag)
 
+#define IDMA_E3_CONTROLLED_WORDS 32U
+
+/*
+ * Controlled finite E3 round.  Inputs are carried in the embedded pair and
+ * prep_mode/pair_mode; all other fields are outputs cleared by the kernel.
+ * launcher[] is the raw 32-word launcher record.
+ */
+struct idma_e3_controlled_round {
+	__s32 flush_status;
+	__u32 prep_complete;
+	__u32 stage;
+	__u32 reserved;
+	struct idma_experiment_stats prime[2];
+	struct idma_experiment_stats probe;
+	__u64 launcher[IDMA_E3_CONTROLLED_WORDS];
+	__u64 observer_magic;
+	__u64 v_hw_cycles;
+};
+
+struct idma_e3_controlled {
+	struct idma_e3_pair pair;
+	__u32 prep_mode;
+	__u32 pair_mode;
+	__u32 reserved[6];
+	struct idma_e3_controlled_round controlled[2];
+};
+
+#define IDMA_IOC_RUN_E3_CONTROLLED _IOWR('i', 0x0c, struct idma_e3_controlled)
+
 #endif
```

### `linux/drivers/misc/idma.c`

```diff
--- a/linux/drivers/misc/idma.c
+++ b/linux/drivers/misc/idma.c
@@ -46,6 +46,26 @@
 #define IDMA_OBS_MAGIC_V4 0x49444d414f425304ULL
 #define IDMA_OBS_MAGIC_V5 0x49444d414f425305ULL
+#define IDMA_OBS_MAGIC_V6 0x49444d414f425306ULL
 #define IDMA_E3_MAGIC 0x49444d4145330001ULL
 #define IDMA_E3_MAGIC_V2 0x49444d4145330002ULL
 #define IDMA_OBS_VALUES 34
@@ -57,6 +77,27 @@
 #define IDMA_BATCH_MAGIC 0x49444d4142544301ULL
 #define IDMA_XFER_TIMEOUT_US 2000000
 #define IDMA_MAX_DEVS 8
 #define IDMA_MAX_PAGES 4096U
 #define IDMA_MAX_RUN_TIMEOUT_MS 300000U
+
+/* DMA0 launcher register block; separate from observer registers. */
+#define IDMA_LAUNCHER_MAGIC_OFFSET 0xb0
+#define IDMA_LAUNCHER_CONTROL_OFFSET 0xb8
+#define IDMA_LAUNCHER_INDEX_OFFSET 0xc0
+#define IDMA_LAUNCHER_DATA_OFFSET 0xc8
+#define IDMA_LAUNCHER_MAGIC 0x49444d414c4e4301ULL
+#define IDMA_LAUNCHER_PREP 1ULL
+#define IDMA_LAUNCHER_PREP_HAS_A 0x100ULL
+#define IDMA_LAUNCHER_GO 2ULL
+#define IDMA_LAUNCHER_ABORT 4ULL
+#define IDMA_LAUNCHER_CLEAR 8ULL
+
+#define IDMA_LAUNCHER_STATUS_PREP BIT_ULL(0)
+#define IDMA_LAUNCHER_STATUS_RUNNING BIT_ULL(1)
+#define IDMA_LAUNCHER_STATUS_FROZEN BIT_ULL(2)
+#define IDMA_LAUNCHER_STATUS_ABORTED BIT_ULL(3)
+#define IDMA_LAUNCHER_HELD_SHIFT 8
+#define IDMA_LAUNCHER_ACTIVE_SHIFT 12
+#define IDMA_LAUNCHER_ERROR_SHIFT 32
+
+#define IDMA_CONTROLLED_STAGE_FLUSH 1
+#define IDMA_CONTROLLED_STAGE_APRIME 2
+#define IDMA_CONTROLLED_STAGE_VPRIME 3
+#define IDMA_CONTROLLED_STAGE_VPROBE 4
+#define IDMA_CONTROLLED_STAGE_PREPARE 5
+#define IDMA_CONTROLLED_STAGE_RUNNING 6
+#define IDMA_CONTROLLED_STAGE_DRAIN 7
+#define IDMA_CONTROLLED_STAGE_COMPLETE 8

 struct idma_device {
 	struct miscdevice miscdev;
@@ -703,7 +744,7 @@
 struct idma_observation {
 	struct idma_device *control;
 	u64 sequence, e3_magic;
-	bool armed, batch, path, e3, delay128, queue_pair;
+	bool armed, batch, path, e3, delay128, queue_pair, controlled;
 };
 
 static u64 idma_observation_sequence;
@@ -712,22 +753,31 @@
 static int idma_observation_prepare(struct idma_observation *o, bool enabled)
 {
 	u64 magic;
 
 	if (!enabled)
 		return 0;
 	o->sequence = ++idma_observation_sequence;
 	o->control = idma_get_instance(0);
 	if (!o->control)
 		return -ENODEV;
 	magic = readq(o->control->reg + IDMA_OBS_MAGIC_OFFSET);
 	o->path = o->e3 || (o->batch && READ_ONCE(idma_path_observe));
-	if (o->queue_pair && magic != IDMA_OBS_MAGIC_V5)
+	if (o->controlled && magic != IDMA_OBS_MAGIC_V6)
+		return -EOPNOTSUPP;
+	if (o->queue_pair && magic != IDMA_OBS_MAGIC_V5 &&
+	    magic != IDMA_OBS_MAGIC_V6)
 		return -EOPNOTSUPP;
-	o->e3_magic = magic == IDMA_OBS_MAGIC_V5 ? IDMA_E3_MAGIC_V2 : IDMA_E3_MAGIC;
-	if (o->e3 && magic != IDMA_OBS_MAGIC_V4 && magic != IDMA_OBS_MAGIC_V5)
+	o->e3_magic = (magic == IDMA_OBS_MAGIC_V5 || magic == IDMA_OBS_MAGIC_V6) ?
+		      IDMA_E3_MAGIC_V2 : IDMA_E3_MAGIC;
+	if (o->e3 && magic != IDMA_OBS_MAGIC_V4 &&
+	    magic != IDMA_OBS_MAGIC_V5 && magic != IDMA_OBS_MAGIC_V6)
 		return -EOPNOTSUPP;
 	if (o->control->did != 10 ||
-	    (magic != IDMA_OBS_MAGIC_V5 && magic != IDMA_OBS_MAGIC_V4 && magic != IDMA_OBS_MAGIC_V3 &&
+	    (magic != IDMA_OBS_MAGIC_V6 && magic != IDMA_OBS_MAGIC_V5 &&
+	     magic != IDMA_OBS_MAGIC_V4 && magic != IDMA_OBS_MAGIC_V3 &&
 	     (o->path || (magic != IDMA_OBS_MAGIC_V2 && (o->batch || magic != IDMA_OBS_MAGIC)))))
 		return -EOPNOTSUPP;
 	/* Clear previous frozen results; never take ownership of an active session. */
@@ -740,14 +790,20 @@
 static int idma_observation_arm(struct idma_observation *o,
 				struct idma_device *v, struct idma_device *a, u64 count)
 {
 	u64 command = 1 | (o->batch ? BIT_ULL(25) : 0) | (o->path ? BIT_ULL(26) : 0);
 
 	if (o->e3)
 		command |= BIT_ULL(27) | (o->delay128 ? BIT_ULL(28) : 0);
+	if (o->controlled)
+		command |= BIT_ULL(29);
 
 	if (!o->control)
 		return 0;
+	if (o->controlled && count != 1)
+		return -EINVAL;
 	if ((u32)v->id >= 4 || v->did != 10U + v->id ||
 	    (a && ((u32)a->id >= 4 || a->did != 10U + a->id)))
 		return -EOPNOTSUPP;
@@ -868,7 +924,7 @@
 static int idma_e3_finish(struct idma_observation *o, const struct idma_experiment *e, u64 *words)
 {
-	u64 n = e->victim_transfers * 2;
+	u64 n = o->controlled ? 1 : e->victim_transfers * 2;
 	unsigned int i, lane, stage;
 	bool valid;
```

Then add launcher/controlled helpers after `idma_stream_reseed` (line ~945) and before `idma_run_session`.

```diff
@@ -945,6 +1001,390 @@
 static void idma_stream_reseed(struct idma_stream *s)
 {
 	size_t i;
@@
 	s->batch_configured = s->batch_started = false;
 }
 
+static bool idma_launcher_terminal(u64 status)
+{
+	return (status & IDMA_LAUNCHER_STATUS_FROZEN) &&
+	       !(status & IDMA_LAUNCHER_STATUS_RUNNING) &&
+	       !((status >> IDMA_LAUNCHER_HELD_SHIFT) & 0xf) &&
+	       !((status >> IDMA_LAUNCHER_ACTIVE_SHIFT) & 0xf);
+}
+
+static int idma_launcher_read_words(struct idma_device *launcher, u64 *words)
+{
+	unsigned int i;
+
+	if (readq(launcher->reg + IDMA_LAUNCHER_MAGIC_OFFSET) != IDMA_LAUNCHER_MAGIC)
+		return -EOPNOTSUPP;
+	for (i = 0; i < IDMA_E3_CONTROLLED_WORDS; i++) {
+		writeq(i, launcher->reg + IDMA_LAUNCHER_INDEX_OFFSET);
+		words[i] = readq(launcher->reg + IDMA_LAUNCHER_DATA_OFFSET);
+	}
+	return 0;
+}
+
+static int idma_controlled_validate(struct idma_device *victim,
+				    struct idma_e3_controlled *c, bool *has_a)
+{
+	struct idma_experiment *e = &c->pair.request;
+	bool a;
+
+	if (PAGE_SIZE != SZ_4K)
+		return -EOPNOTSUPP;
+	if (c->pair.abi_version != 1 || c->pair.struct_size != sizeof(c->pair) ||
+	    c->pair.reserved)
+		return -EINVAL;
+	if (c->prep_mode > 1 || c->pair_mode > 1 ||
+	    memchr_inv(c->reserved, 0, sizeof(c->reserved)))
+		return -EINVAL;
+	if (e->abi_version != IDMA_EXPERIMENT_ABI || e->struct_size != sizeof(*e) ||
+	    e->flags || memchr_inv(e->reserved, 0, sizeof(e->reserved)))
+		return -EINVAL;
+	a = e->aggressor_id != IDMA_EXPERIMENT_NO_AGGRESSOR;
+	if (a && e->aggressor_id != 1)
+		return -EINVAL;
+	if (!a && c->prep_mode)
+		return -EINVAL;
+	if (victim->id != 0 || victim->did != 10)
+		return -EINVAL;
+	if (e->victim_pages != 1 || (a && e->aggressor_pages != 1))
+		return -EINVAL;
+	if (e->chunk_bytes != 64 || e->victim_transfers != 1 ||
+	    e->warmup_transfers || !e->run_timeout_ms || e->run_timeout_ms > 60000)
+		return -EINVAL;
+	if (c->pair_mode == 1) {
+		if (c->pair.first_delay)
+			return -EINVAL;
+	} else if (c->pair.first_delay != 0 && c->pair.first_delay != 128) {
+		return -EINVAL;
+	}
+	*has_a = a;
+	return 0;
+}
+
+static int idma_controlled_environment(struct idma_device *victim,
+				       struct idma_device *aggressor)
+{
+	u64 status;
+
+	if (readq(victim->reg + IDMA_LAUNCHER_MAGIC_OFFSET) != IDMA_LAUNCHER_MAGIC)
+		return -EOPNOTSUPP;
+	status = readq(victim->reg + IDMA_LAUNCHER_CONTROL_OFFSET);
+	if (status & (IDMA_LAUNCHER_STATUS_RUNNING |
+		      (0xfULL << IDMA_LAUNCHER_HELD_SHIFT) |
+		      (0xfULL << IDMA_LAUNCHER_ACTIVE_SHIFT) |
+		      (0xffffffffULL << IDMA_LAUNCHER_ERROR_SHIFT)))
+		return -EBUSY;
+	if ((readq(victim->reg + IDMA_BATCH_CONTROL_OFFSET) & 2) ||
+	    (aggressor && (readq(aggressor->reg + IDMA_BATCH_CONTROL_OFFSET) & 2)))
+		return -EBUSY;
+	writeq(0, victim->reg + IDMA_BATCH_CONTROL_OFFSET);
+	if (readq(victim->reg + IDMA_BATCH_CONTROL_OFFSET))
+		return -EIO;
+	if (aggressor) {
+		writeq(0, aggressor->reg + IDMA_BATCH_CONTROL_OFFSET);
+		if (readq(aggressor->reg + IDMA_BATCH_CONTROL_OFFSET))
+			return -EIO;
+	}
+	return 0;
+}
+
+static int idma_controlled_prime(struct idma_stream *s,
+				 struct idma_experiment_stats *stats,
+				 struct riscv_iommu_hpm_session *hpm,
+				 u64 deadline)
+{
+	struct riscv_iommu_hpm_sample before, after;
+	u64 active_before[2], active_after[2];
+	u64 end = 0;
+	int ret;
+
+	if (!s)
+		return 0;
+	s->stats = stats;
+	memset(stats, 0, sizeof(*stats));
+	s->offset = s->next_offset = s->touched_end = 0;
+	s->in_flight = false;
+	s->transfer_id = s->deadline_ns = s->last_complete_ns = 0;
+	idma_snapshot(hpm, s, NULL, &before, active_before);
+	ret = idma_run_window(s, NULL, 1, deadline, true, &end);
+	idma_snapshot(hpm, s, NULL, &after, active_after);
+	stats->active_cycles = active_after[0] - active_before[0];
+	stats->iotlb_miss = after.iotlb_miss[0] - before.iotlb_miss[0];
+	stats->s1_ptw = after.s1_ptw[0] - before.s1_ptw[0];
+	stats->s2_ptw = after.s2_ptw[0] - before.s2_ptw[0];
+	if (ret)
+		return ret;
+	if (s->in_flight) {
+		ret = idma_drain(s);
+		if (ret)
+			return ret;
+	}
+	return 0;
+}
+
+static int idma_controlled_resolve_launcher(struct idma_device *launcher,
+					    struct idma_stream *v,
+					    struct idma_stream *a,
+					    struct idma_experiment *e,
+					    u64 *words)
+{
+	bool v_cancelled, a_cancelled;
+	int ret;
+
+	ret = idma_launcher_read_words(launcher, words);
+	if (ret)
+		return ret;
+	if (words[0] != IDMA_LAUNCHER_MAGIC || words[1])
+		return -EIO;
+	if (words[15] != words[16])
+		return -EIO;
+	v_cancelled = (words[17] & 1) && !(words[16] & 1);
+	a_cancelled = a && (words[17] & 2) && !(words[16] & 2);
+	if (v_cancelled) {
+		v->in_flight = false;
+	} else if (v->in_flight) {
+		ret = idma_drain(v);
+		if (ret)
+			return ret;
+	}
+	if (a) {
+		if (a_cancelled) {
+			a->in_flight = false;
+		} else if (a->in_flight) {
+			ret = idma_drain(a);
+			if (ret)
+				return ret;
+		}
+	}
+	e->victim.submitted = words[24];
+	e->victim.completed = words[26];
+	e->victim.completed_bytes = words[26] * v->chunk;
+	if (words[26])
+		v->touched_end = max(v->touched_end, v->offset + v->chunk);
+	if (a) {
+		e->aggressor.submitted = words[25];
+		e->aggressor.completed = words[27];
+		e->aggressor.completed_bytes = words[27] * a->chunk;
+		if (words[27])
+			a->touched_end = max(a->touched_end, a->offset + a->chunk);
+	}
+	return 0;
+}
+
+static int idma_controlled_formal(struct idma_device *launcher,
+				  struct idma_stream *v, struct idma_stream *a,
+				  struct idma_observation *o,
+				  struct idma_experiment *e,
+				  struct idma_e3_controlled_round *round,
+				  struct riscv_iommu_hpm_session *hpm,
+				  u64 deadline)
+{
+	struct riscv_iommu_hpm_sample before, after;
+	u64 active_before[2], active_after[2];
+	u64 words[IDMA_E3_CONTROLLED_WORDS];
+	u64 status, start, end, v_done = 0;
+	int ret, poll_ret;
+
+	v->stats = &e->victim;
+	if (a)
+		a->stats = &e->aggressor;
+	memset(&e->victim, 0, sizeof(e->victim));
+	if (a)
+		memset(&e->aggressor, 0, sizeof(e->aggressor));
+	v->offset = v->next_offset = v->touched_end = 0;
+	v->in_flight = false;
+	v->transfer_id = v->deadline_ns = v->last_complete_ns = 0;
+	if (a) {
+		a->offset = a->next_offset = a->touched_end = 0;
+		a->in_flight = false;
+		a->transfer_id = a->deadline_ns = a->last_complete_ns = 0;
+	}
+
+	writeq(IDMA_LAUNCHER_CLEAR, launcher->reg + IDMA_LAUNCHER_CONTROL_OFFSET);
+	status = readq(launcher->reg + IDMA_LAUNCHER_CONTROL_OFFSET);
+	if (status & ~IDMA_LAUNCHER_STATUS_FROZEN)
+		return -EIO;
+	writeq(IDMA_LAUNCHER_PREP | (a ? IDMA_LAUNCHER_PREP_HAS_A : 0),
+	       launcher->reg + IDMA_LAUNCHER_CONTROL_OFFSET);
+	status = readq(launcher->reg + IDMA_LAUNCHER_CONTROL_OFFSET);
+	if (!(status & IDMA_LAUNCHER_STATUS_PREP) ||
+	    (status >> IDMA_LAUNCHER_ERROR_SHIFT))
+		return -EIO;
+
+	ret = idma_submit(v, false);
+	if (ret)
+		goto abort;
+	if (a) {
+		ret = idma_submit(a, false);
+		if (ret)
+			goto abort;
+	}
+	status = readq(launcher->reg + IDMA_LAUNCHER_CONTROL_OFFSET);
+	if ((status >> IDMA_LAUNCHER_ERROR_SHIFT) ||
+	    (((status >> IDMA_LAUNCHER_HELD_SHIFT) & 0xf) < (a ? 2 : 1))) {
+		ret = -EIO;
+		goto abort;
+	}
+
+	ret = idma_observation_arm(o, v->idma, a ? a->idma : NULL, 1);
+	if (ret)
+		goto abort;
+
+	idma_snapshot(hpm, v, a, &before, active_before);
+	start = ktime_get_ns();
+	e->measurement_started = 1;
+	e->snapshot_before_ns = start;
+	writeq(IDMA_LAUNCHER_GO, launcher->reg + IDMA_LAUNCHER_CONTROL_OFFSET);
+	status = readq(launcher->reg + IDMA_LAUNCHER_CONTROL_OFFSET);
+	if (status >> IDMA_LAUNCHER_ERROR_SHIFT) {
+		ret = -EIO;
+		goto abort;
+	}
+
+	for (;;) {
+		if (ktime_get_ns() >= deadline) {
+			ret = -ETIMEDOUT;
+			break;
+		}
+		if (signal_pending(current)) {
+			ret = -EINTR;
+			break;
+		}
+		poll_ret = idma_poll(v, false);
+		if (poll_ret < 0) {
+			ret = poll_ret;
+			break;
+		}
+		if (poll_ret == 1 && !v_done)
+			v_done = v->last_complete_ns;
+		if (a) {
+			poll_ret = idma_poll(a, false);
+			if (poll_ret < 0) {
+				ret = poll_ret;
+				break;
+			}
+		}
+		status = readq(launcher->reg + IDMA_LAUNCHER_CONTROL_OFFSET);
+		if (status >> IDMA_LAUNCHER_ERROR_SHIFT) {
+			ret = -EIO;
+			break;
+		}
+		if (idma_launcher_terminal(status))
+			break;
+		cpu_relax();
+	}
+
+	if (ret) {
+		writeq(IDMA_LAUNCHER_ABORT, launcher->reg + IDMA_LAUNCHER_CONTROL_OFFSET);
+		poll_ret = readq_poll_timeout(launcher->reg + IDMA_LAUNCHER_CONTROL_OFFSET,
+					      status, idma_launcher_terminal(status), 0,
+					      IDMA_XFER_TIMEOUT_US);
+		if (poll_ret) {
+			ret = -EIO;
+			goto quarantine;
+		}
+	}
+
+	ret = idma_controlled_resolve_launcher(launcher, v, a, e, words);
+	if (ret)
+		goto quarantine;
+
+	end = ktime_get_ns();
+	if (v_done)
+		e->victim_elapsed_ns = v_done - start;
+	e->measurement_complete = !ret;
+	idma_snapshot(hpm, v, a, &after, active_after);
+	idma_snapshot_delta(&e->victim, 0, &before, &after,
+			    active_before[0], active_after[0]);
+	if (a)
+		idma_snapshot_delta(&e->aggressor, 1, &before, &after,
+				    active_before[1], active_after[1]);
+	e->snapshot_after_ns = end - start;
+	round->v_hw_cycles = e->victim.active_cycles;
+
+	local_flush_icache_all();
+	if (!ret) {
+		poll_ret = idma_verify(v);
+		if (poll_ret)
+			ret = poll_ret;
+		if (a && !ret) {
+			poll_ret = idma_verify(a);
+			if (poll_ret)
+				ret = poll_ret;
+		}
+	}
+	memcpy(round->launcher, words, sizeof(round->launcher));
+	round->observer_magic = readq(launcher->reg + IDMA_OBS_MAGIC_OFFSET);
+	return ret;
+
+abort:
+	writeq(IDMA_LAUNCHER_ABORT, launcher->reg + IDMA_LAUNCHER_CONTROL_OFFSET);
+	poll_ret = readq_poll_timeout(launcher->reg + IDMA_LAUNCHER_CONTROL_OFFSET,
+				      status, idma_launcher_terminal(status), 0,
+				      IDMA_XFER_TIMEOUT_US);
+	if (poll_ret) {
+		ret = -EIO;
+		goto quarantine;
+	}
+	poll_ret = idma_controlled_resolve_launcher(launcher, v, a, e, words);
+	if (poll_ret && !ret)
+		ret = poll_ret;
+	memcpy(round->launcher, words, sizeof(round->launcher));
+	return ret;
+
+quarantine:
+	idma_quarantine(v);
+	if (a)
+		idma_quarantine(a);
+	return ret;
+}
+
+static int idma_controlled_one_round(struct idma_device *launcher,
+				     struct idma_stream *v, struct idma_stream *a,
+				     struct idma_observation *o,
+				     struct idma_experiment *e,
+				     struct idma_e3_controlled_round *round,
+				     u64 *e3_words,
+				     struct riscv_iommu_hpm_session *hpm,
+				     bool hot, u64 timeout_ms)
+{
+	u64 deadline = ktime_get_ns() + timeout_ms * NSEC_PER_MSEC;
+	int ret, fin_ret;
+
+	round->flush_status = -EINPROGRESS;
+	round->stage = IDMA_CONTROLLED_STAGE_FLUSH;
+	local_flush_icache_all();
+	ret = riscv_iommu_hpm_flush_s1(hpm);
+	round->flush_status = ret;
+	if (ret)
+		return ret;
+
+	round->stage = IDMA_CONTROLLED_STAGE_APRIME;
+	memset(&round->prime[1], 0, sizeof(round->prime[1]));
+	if (a && hot) {
+		ret = idma_controlled_prime(a, &round->prime[1], hpm, deadline);
+		if (ret)
+			return ret;
+	} else if (a) {
+		a->touched_end = 0;
+		a->in_flight = false;
+	}
+
+	round->stage = IDMA_CONTROLLED_STAGE_VPRIME;
+	memset(&round->prime[0], 0, sizeof(round->prime[0]));
+	ret = idma_controlled_prime(v, &round->prime[0], hpm, deadline);
+	if (ret)
+		return ret;
+
+	round->stage = IDMA_CONTROLLED_STAGE_VPROBE;
+	memset(&round->probe, 0, sizeof(round->probe));
+	ret = idma_controlled_prime(v, &round->probe, hpm, deadline);
+	if (ret)
+		return ret;
+	if (round->probe.iotlb_miss || round->probe.s1_ptw || round->probe.s2_ptw)
+		return -EIO;
+
+	local_flush_icache_all();
+	ret = idma_verify(v);
+	if (ret)
+		return ret;
+	if (a) {
+		ret = idma_verify(a);
+		if (ret)
+			return ret;
+	}
+
+	idma_stream_reseed(v);
+	if (a)
+		idma_stream_reseed(a);
+	local_flush_icache_all();
+	round->prep_complete = 1;
+
+	round->stage = IDMA_CONTROLLED_STAGE_PREPARE;
+	ret = idma_controlled_formal(launcher, v, a, o, e, round, hpm, deadline);
+	round->stage = IDMA_CONTROLLED_STAGE_DRAIN;
+	fin_ret = idma_observation_finish(o, e, ret);
+	if (fin_ret && !ret)
+		ret = fin_ret;
+	if (o->armed) {
+		fin_ret = idma_e3_finish(o, e, e3_words);
+		if (fin_ret && !ret)
+			ret = fin_ret;
+	}
+	round->stage = IDMA_CONTROLLED_STAGE_COMPLETE;
+	return ret;
+}
+
+static long idma_run_e3_controlled(struct idma_device *victim,
+				   void __user *uarg)
+{
+	struct idma_e3_controlled *c;
+	struct idma_experiment e;
+	struct idma_stream streams[2] = {};
+	struct idma_stream *v = &streams[0], *a = NULL;
+	struct idma_device *aggressor = NULL, *launcher = NULL;
+	struct idma_observation o = {};
+	struct riscv_iommu_hpm_session *hpm = NULL;
+	bool has_a = false;
+	unsigned int round;
+	long ret;
+
+	c = kzalloc(sizeof(*c), GFP_KERNEL);
+	if (!c)
+		return -ENOMEM;
+	if (copy_from_user(c, uarg, sizeof(*c))) {
+		ret = -EFAULT;
+		goto out_free;
+	}
+	memset(&c->pair.status, 0, sizeof(c->pair) - offsetof(struct idma_e3_pair, status));
+	memset(&c->pair.request.status, 0,
+	       sizeof(c->pair.request) - offsetof(struct idma_experiment, status));
+	memset(c->controlled, 0, sizeof(c->controlled));
+
+	ret = idma_controlled_validate(victim, c, &has_a);
+	if (ret)
+		goto out_copy;
+	e = c->pair.request;
+	e.victim_did = 10;
+	e.aggressor_did = has_a ? 11 : IDMA_EXPERIMENT_NO_AGGRESSOR;
+
+	ret = mutex_lock_interruptible(&idma_experiment_lock);
+	if (ret)
+		goto out_copy;
+	if (idma_any_quarantined()) {
+		ret = -EIO;
+		goto out_unlock;
+	}
+	if (has_a) {
+		aggressor = idma_get_instance(1);
+		if (!aggressor) {
+			ret = -ENODEV;
+			goto out_unlock;
+		}
+		if (aggressor->did != 11) {
+			ret = -EINVAL;
+			goto out_put;
+		}
+		a = &streams[1];
+	}
+	launcher = victim;
+	ret = idma_controlled_environment(victim, aggressor);
+	if (ret)
+		goto out_put;
+
+	ret = idma_stream_prepare(v, victim, 1, 64, &e.victim);
+	if (ret)
+		goto out_put;
+	if (a) {
+		ret = idma_stream_prepare(a, aggressor, 1, 64, &e.aggressor);
+		if (ret)
+			goto out_cleanup;
+	}
+	c->pair.mapping[0] = v->src_iova;
+	c->pair.mapping[1] = v->dst_iova;
+	c->pair.mapping[2] = page_to_phys(idma_buffer_page(v->src));
+	c->pair.mapping[3] = page_to_phys(idma_buffer_page(v->dst));
+	if (a) {
+		c->pair.mapping[4] = a->src_iova;
+		c->pair.mapping[5] = a->dst_iova;
+		c->pair.mapping[6] = page_to_phys(idma_buffer_page(a->src));
+		c->pair.mapping[7] = page_to_phys(idma_buffer_page(a->dst));
+	}
+
+	for (round = 0; round < 2; round++) {
+		bool hot = c->prep_mode == 1;
+
+		o = (struct idma_observation) {
+			.batch = true,
+			.e3 = true,
+			.controlled = true,
+			.delay128 = c->pair_mode == 1 ? false :
+				    (round ? c->pair.first_delay == 0 :
+					     c->pair.first_delay == 128),
+		};
+		ret = idma_observation_prepare(&o, true);
+		if (ret)
+			break;
+		c->pair.sequence[round] = o.sequence;
+		c->pair.rounds_attempted++;
+		ret = riscv_iommu_hpm_acquire(v->dev, a ? a->dev : NULL, &hpm);
+		if (ret)
+			break;
+		e.hpm_configured = 1;
+		ret = idma_controlled_one_round(launcher, v, a, &o, &e,
+						&c->controlled[round],
+						c->pair.observation[round],
+						hpm, hot, e.run_timeout_ms);
+		riscv_iommu_hpm_release(hpm);
+		hpm = NULL;
+		c->pair.result[round] = e;
+		if (ret) {
+			c->pair.status = ret;
+			break;
+		}
+		c->pair.rounds_completed++;
+		if (round == 0) {
+			idma_stream_reseed(v);
+			if (a)
+				idma_stream_reseed(a);
+			idma_put_instance(o.control);
+			o.control = NULL;
+		}
+	}
+	if (hpm)
+		riscv_iommu_hpm_release(hpm);
+	if (o.control)
+		idma_put_instance(o.control);
+	if (a)
+		idma_stream_cleanup(a);
+	idma_stream_cleanup(v);
+out_put:
+	idma_put_instance(aggressor);
+out_unlock:
+	mutex_unlock(&idma_experiment_lock);
+out_copy:
+	c->pair.status = ret;
+	if (copy_to_user(uarg, c, sizeof(*c)))
+		ret = -EFAULT;
+out_free:
+	kfree(c);
+	return ret;
+}
+
 static long idma_run_session(struct idma_device *victim,
 			    struct idma_experiment *request, bool batch, struct idma_e3_pair *pair,
 			    struct idma_e3_diag_round *diagnostic, bool queue_pair)
```

Then add ioctl case and caps bit:

```diff
@@ -1296,6 +1943,7 @@
 		.capabilities = IDMA_EXPERIMENT_CAP_SW_REFILL | IDMA_EXPERIMENT_CAP_HW_BATCH |
 				IDMA_EXPERIMENT_CAP_E3_PAIR | IDMA_EXPERIMENT_CAP_E3_DIAG |
-				IDMA_EXPERIMENT_CAP_E3_QUEUE_PAIR,
+				IDMA_EXPERIMENT_CAP_E3_QUEUE_PAIR |
+				IDMA_EXPERIMENT_CAP_E3_CONTROLLED,
@@ -1305,6 +1953,8 @@
 	switch (cmd) {
+	case IDMA_IOC_RUN_E3_CONTROLLED:
+		return idma_run_e3_controlled(idma, (void __user *)arg);
 	case IDMA_IOC_RUN_E3_QUEUE_PAIR:
 		return idma_run_e3_pair(idma, (void __user *)arg, 2);
```

## Host harness proposal

### `tests/idma_controlled_harness.c`

This is intentionally a host model. It is not an FPGA claim. It reuses the existing harness extraction convention. The Python test below inserts production types/functions into the `/* PRODUCTION_TYPES */` and `/* PRODUCTION_FUNCTIONS */` markers.

```c
/* Host model only. Production driver functions are inserted by test_idma_controlled.py. */
#include <assert.h>
#include <errno.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <linux/idma.h>

typedef __u64 u64;
typedef uint32_t u32;
typedef uint8_t u8;
typedef uintptr_t dma_addr_t;
typedef uintptr_t phys_addr_t;
struct device { int index; };
struct page { int unused; };
struct miscdevice { struct device *parent; };
struct mutex { int unused; };
struct iommu_domain { int unused; };
#define BIT_ULL(n) (1ULL << (n))
#define __iomem
#define __user
#define WRITE_ONCE(dst, src) ((dst) = (src))
#define READ_ONCE(src) (*(volatile __typeof__(src) *)&(src))
#define min(a, b) ((a) < (b) ? (a) : (b))
#define max(a, b) ((a) > (b) ? (a) : (b))
#define dev_crit(dev, ...) ((void)(dev))
#define NSEC_PER_USEC 1000ULL
#define NSEC_PER_MSEC 1000000ULL
#define PAGE_SIZE 4096U
#define offset_in_page(ptr) ((uintptr_t)(ptr) & (PAGE_SIZE - 1))
#define SZ_4K 4096U
#define DMA_ATTR_FORCE_CONTIGUOUS 0
#define current NULL
#define GFP_KERNEL 0
#define kzalloc(n, flags) calloc(1, n)
#define kfree(ptr) free(ptr)

/* PRODUCTION_TYPES */

struct riscv_iommu_hpm_session { int unused; };
struct riscv_iommu_hpm_sample { u64 iotlb_miss[2], s1_ptw[2], s2_ptw[2]; };

struct fake_dma {
    u64 regs[32], latency, due, next_id, done, submits;
    uintptr_t submitted_src[4096];
    bool pending, zero_id, batch, stop;
    u8 *cpu_src, *cpu_dst, *memory_src, *memory_dst;
    size_t buffer_size;
};

static struct fake_dma hw[2];
static struct device devices[2] = {{0}, {1}};
static struct idma_device idmas[2];
static struct mutex idma_experiment_lock;
static struct riscv_iommu_hpm_session session;
static u64 now = 1000, interrupt_at = UINT64_MAX;
static unsigned allocations, frees, unmaps, hpm_releases, hpm_acquires;
static bool idma_observe, idma_path_observe;
static u64 idma_observation_sequence;
static unsigned obs_arms, obs_reports, obs_mmio;
static bool controlled_mode, controlled_probe_miss, controlled_flush_fail;
static bool controlled_abort_hold, controlled_cancel_v, controlled_cancel_a;
static bool controlled_drain_fail, controlled_issue_a, controlled_hold_a;
static bool controlled_no_terminal, controlled_raw_error;
static unsigned controlled_gos, controlled_aborts, controlled_clears;

static void dev_info(struct device *dev, const char *fmt, ...)
{
    (void)dev; (void)fmt; obs_reports++;
}

static struct page *idma_buffer_page(void *ptr)
{
    return (struct page *)((uintptr_t)ptr & ~(uintptr_t)(PAGE_SIZE - 1));
}
static phys_addr_t page_to_phys(struct page *page) { return (uintptr_t)page; }

static void local_flush_icache_all(void)
{
    assert(!hw[0].pending && !hw[1].pending);
}

static phys_addr_t iommu_iova_to_phys(struct iommu_domain *domain, unsigned long iova)
{
    (void)domain;
    return iova + PAGE_SIZE;
}

static void dev_err(struct device *dev, const char *format, int id, u32 did,
                    size_t offset, size_t touched_end, size_t size, u32 chunk,
                    unsigned src, unsigned src_after, unsigned expected_src,
                    unsigned dst, unsigned dst_after, unsigned expected_dst,
                    unsigned long src_iova, unsigned long dst_iova,
                    const phys_addr_t *src_pa, const phys_addr_t *dst_pa,
                    const phys_addr_t *src_pte_pa, const phys_addr_t *dst_pte_pa)
{
    (void)dev; (void)format; (void)id; (void)did; (void)offset; (void)touched_end;
    (void)size; (void)chunk; (void)src; (void)src_after; (void)expected_src;
    (void)dst; (void)dst_after; (void)expected_dst; (void)src_iova; (void)dst_iova;
    (void)src_pa; (void)dst_pa; (void)src_pte_pa; (void)dst_pte_pa;
}

static void advance(u64 delta)
{
    int i;

    now += delta;
    for (i = 0; i < 2; i++) {
        struct fake_dma *h = &hw[i];
        if (h->pending && h->due != UINT64_MAX && now >= h->due) {
            u8 *dst = (void *)(uintptr_t)h->regs[IDMA_DST_ADDR_REG_OFFSET / 8];
            u8 *src = (void *)(uintptr_t)h->regs[IDMA_SRC_ADDR_REG_OFFSET / 8];
            if (controlled_drain_fail && i == 0)
                continue;
            memcpy(dst, src, h->regs[IDMA_NUM_BYTES_REG_OFFSET / 8]);
            h->done = h->next_id;
            h->pending = false;
        }
    }
}

static u64 ktime_get_ns(void) { advance(1000); return now; }
static bool signal_pending(void *unused) { (void)unused; return now >= interrupt_at; }
static void cpu_relax(void) { advance(1000); }
static void cond_resched(void) { advance(1000); }

static struct fake_dma *mmio(void *address, size_t *offset)
{
    int i;
    for (i = 0; i < 2; i++) {
        uintptr_t p = (uintptr_t)address, base = (uintptr_t)hw[i].regs;
        if (p >= base && p < base + sizeof(hw[i].regs)) {
            *offset = p - base;
            return &hw[i];
        }
    }
    assert(!"invalid MMIO address");
    return NULL;
}

static void controlled_launcher_reset(void)
{
    memset(&hw[0].regs[IDMA_LAUNCHER_MAGIC_OFFSET / 8], 0, 0x50);
    hw[0].regs[IDMA_LAUNCHER_MAGIC_OFFSET / 8] = 0x49444d414c4e4301ULL;
    controlled_clears++;
}

static void controlled_launcher_prep(u64 value)
{
    hw[0].regs[IDMA_LAUNCHER_CONTROL_OFFSET / 8] = 1;
    hw[0].regs[IDMA_LAUNCHER_DATA_OFFSET / 8] = 0;
    hw[0].regs[IDMA_LAUNCHER_INDEX_OFFSET / 8] = 0;
    (void)value;
}

static void controlled_launcher_go(void)
{
    controlled_gos++;
    /* V is lane 0, A is lane 1 when requested. */
    if (!controlled_abort_hold) {
        if (!controlled_cancel_v) {
            hw[0].pending = true;
            hw[0].due = now + hw[0].latency;
        }
        if (controlled_issue_a && !controlled_cancel_a) {
            hw[1].pending = true;
            hw[1].due = now + hw[1].latency;
        }
    }
    hw[0].regs[IDMA_LAUNCHER_CONTROL_OFFSET / 8] = 3;
}

static void controlled_launcher_abort(void)
{
    controlled_aborts++;
    if (controlled_abort_hold && !controlled_no_terminal) {
        hw[0].regs[IDMA_LAUNCHER_CONTROL_OFFSET / 8] = 5;
    } else if (!controlled_no_terminal) {
        hw[0].regs[IDMA_LAUNCHER_CONTROL_OFFSET / 8] = 5;
    }
}

static void writeq(u64 value, void *address)
{
    size_t offset;
    struct fake_dma *h = mmio(address, &offset);

    if (offset >= IDMA_LAUNCHER_MAGIC_OFFSET) {
        if (offset == IDMA_LAUNCHER_CONTROL_OFFSET) {
            if (value & 8)
                controlled_launcher_reset();
            else if (value & 1)
                controlled_launcher_prep(value);
            else if (value & 2)
                controlled_launcher_go();
            else if (value & 4)
                controlled_launcher_abort();
        }
        h->regs[offset / 8] = value;
        return;
    }
    if (offset >= IDMA_BATCH_MAGIC_OFFSET) {
        if (offset == IDMA_BATCH_CONTROL_OFFSET && value == 2) {
            h->stop = true;
            return;
        }
        h->regs[offset / 8] = value;
        return;
    }
    if (offset >= IDMA_OBS_MAGIC_OFFSET) {
        obs_mmio++;
        if (offset == IDMA_OBS_CONTROL_OFFSET) {
            if (value == 4)
                value = 0;
            else if (value == 2)
                value = 4;
            else {
                assert(hpm_acquires == hpm_releases + 1 && !hw[0].pending && !hw[1].pending);
                obs_arms++;
                value = 1;
            }
        }
        h->regs[offset / 8] = value;
        return;
    }
    assert(!h->pending);
    h->regs[offset / 8] = value;
}

static u64 readq(void *address)
{
    size_t offset;
    struct fake_dma *h = mmio(address, &offset);

    advance(1000);
    if (offset >= IDMA_LAUNCHER_MAGIC_OFFSET) {
        if (offset == IDMA_LAUNCHER_DATA_OFFSET) {
            unsigned int index = h->regs[IDMA_LAUNCHER_INDEX_OFFSET / 8];
            static u64 words[IDMA_E3_CONTROLLED_WORDS];
            unsigned int i;

            for (i = 0; i < IDMA_E3_CONTROLLED_WORDS; i++)
                words[i] = 0;
            words[0] = 0x49444d414c4e4301ULL;
            words[1] = controlled_raw_error;
            words[2] = 4;
            words[3] = now;
            words[4] = now;
            words[5] = now + 1;
            words[6] = now + 2;
            words[7] = now + 3;
            words[8] = now + 1;
            words[9] = now + 4;
            words[10] = now + 5;
            words[11] = 1;
            words[12] = controlled_issue_a ? 2 : 0;
            words[13] = 1;
            words[14] = controlled_issue_a ? 2 : 0;
            words[15] = controlled_issue_a ? 3 : 1;
            words[16] = controlled_issue_a ? 3 : 1;
            words[17] = (controlled_cancel_v ? 1 : 0) | (controlled_cancel_a ? 2 : 0);
            words[24] = controlled_cancel_v ? 0 : 1;
            words[25] = controlled_issue_a && !controlled_cancel_a ? 1 : 0;
            words[26] = controlled_cancel_v ? 0 : 1;
            words[27] = controlled_issue_a && !controlled_cancel_a ? 1 : 0;
            words[28] = controlled_issue_a ? 1 : 0;
            words[29] = 0;
            words[30] = 0;
            return words[index];
        }
        return h->regs[offset / 8];
    }
    if (offset >= IDMA_BATCH_MAGIC_OFFSET)
        return h->regs[offset / 8];
    if (offset >= IDMA_OBS_MAGIC_OFFSET) {
        obs_mmio++;
        if (offset == IDMA_OBS_CONTROL_OFFSET)
            return h->regs[offset / 8] & 3 ? 4 : 4;
        if (offset == IDMA_OBS_DATA_OFFSET)
            return 0;
        return h->regs[offset / 8];
    }
    if (offset == IDMA_NEXT_ID_REG_OFFSET) {
        assert(!h->pending);
        h->submitted_src[h->submits++] = h->regs[IDMA_SRC_ADDR_REG_OFFSET / 8];
        if (h == &hw[0] && (hw[0].regs[IDMA_LAUNCHER_CONTROL_OFFSET / 8] & 1) &&
            !(hw[0].regs[IDMA_LAUNCHER_CONTROL_OFFSET / 8] & 2)) {
            h->next_id++;
            return h->zero_id ? 0 : h->next_id;
        }
        h->pending = true;
        h->next_id++;
        h->due = h->zero_id || h->latency == UINT64_MAX ? UINT64_MAX : now + h->latency;
        return h->zero_id ? 0 : h->next_id;
    }
    if (offset == IDMA_DONE_REG_OFFSET)
        return h->done;
    if (offset == IDMA_ACTIVE_CYCLES_REG_OFFSET)
        return now;
    return h->regs[offset / 8];
}

#define readq_poll_timeout(address, value, condition, sleep_us, timeout_us) ({ \
    u64 limit = now + (u64)(timeout_us) * NSEC_PER_USEC; \
    int result = -ETIMEDOUT; \
    (void)(sleep_us); \
    do { \
        (value) = readq(address); \
        if (condition) { result = 0; break; } \
        advance(NSEC_PER_MSEC); \
    } while (now < limit); \
    result; \
})

static int copy_from_user(void *dst, const void *src, size_t n) { memcpy(dst, src, n); return 0; }
static int copy_to_user(void *dst, const void *src, size_t n) { memcpy(dst, src, n); return 0; }
static int mutex_lock_interruptible(struct mutex *m) { (void)m; return 0; }
static void mutex_unlock(struct mutex *m) { (void)m; }
static const void *memchr_inv(const void *ptr, int c, size_t n)
{
    const unsigned char *p = ptr;
    while (n--) {
        if (*p != (unsigned char)c)
            return p;
        p++;
    }
    return NULL;
}
static bool idma_any_quarantined(void)
{
    return idmas[0].quarantined || idmas[1].quarantined;
}
static struct idma_device *idma_get_instance(u32 id)
{
    return id < 2 ? &idmas[id] : NULL;
}
static void idma_put_instance(struct idma_device *d) { (void)d; }
static void idma_iommu_unmap_range(struct iommu_domain *d, unsigned long iova,
                                   size_t size, size_t page)
{
    (void)d; (void)iova; (void)size; (void)page;
    unmaps++;
}
static void dma_free_attrs(struct device *d, size_t n, void *ptr, dma_addr_t dma,
                           unsigned attrs)
{
    (void)n; (void)dma; (void)attrs;
    assert(!hw[d->index].pending);
    frees++;
    free(ptr);
}

static u64 idma_pattern(struct idma_stream *s, size_t word);
static int idma_stream_prepare(struct idma_stream *s, struct idma_device *idma,
                               u32 pages, u32 chunk,
                               struct idma_experiment_stats *stats)
{
    size_t i;
    s->idma = idma;
    s->dev = idma->miscdev.parent;
    s->stats = stats;
    s->size = (size_t)pages * PAGE_SIZE;
    s->chunk = chunk;
    s->src = malloc(s->size);
    s->dst = malloc(s->size);
    assert(s->src && s->dst);
    allocations += 2;
    s->src_iova = (uintptr_t)s->src;
    s->dst_iova = (uintptr_t)s->dst;
    s->mapped_src = s->mapped_dst = true;
    for (i = 0; i < s->size / sizeof(u64); i++) {
        ((u64 *)s->src)[i] = idma_pattern(s, i);
        ((u64 *)s->dst)[i] = ~idma_pattern(s, i);
    }
    return 0;
}

static int riscv_iommu_hpm_acquire(struct device *v, struct device *a,
                                   struct riscv_iommu_hpm_session **s)
{
    (void)v; (void)a;
    hpm_acquires++;
    *s = &session;
    return 0;
}
static void riscv_iommu_hpm_read(struct riscv_iommu_hpm_session *s,
                                 struct riscv_iommu_hpm_sample *sample)
{
    (void)s;
    memset(sample, 0, sizeof(*sample));
    advance(1000);
}
static void riscv_iommu_hpm_release(struct riscv_iommu_hpm_session *s)
{
    assert(s == &session);
    hpm_releases++;
}
static int riscv_iommu_hpm_flush_s1(struct riscv_iommu_hpm_session *s)
{
    assert(s == &session);
    assert(hpm_acquires == hpm_releases + 1);
    assert(!hw[0].pending && !hw[1].pending);
    return controlled_flush_fail ? -EIO : 0;
}

/* PRODUCTION_FUNCTIONS */

static struct idma_experiment controlled_input(void)
{
    struct idma_experiment e = {
        .abi_version = IDMA_EXPERIMENT_ABI,
        .struct_size = sizeof(e),
        .aggressor_id = 1,
        .victim_pages = 1,
        .aggressor_pages = 1,
        .chunk_bytes = 64,
        .run_timeout_ms = 100,
        .victim_transfers = 1,
    };
    int i;
    for (i = 0; i < 2; i++) {
        idmas[i].id = i;
        idmas[i].did = 10 + i;
        idmas[i].reg = hw[i].regs;
        idmas[i].miscdev.parent = &devices[i];
        hw[i].latency = 20000;
    }
    return e;
}

int main(int argc, char **argv)
{
    struct idma_e3_controlled c;
    struct idma_experiment e;
    const char *name;
    long ret;

    assert(argc == 2);
    name = argv[1];
    e = controlled_input();
    hw[0].regs[IDMA_OBS_MAGIC_OFFSET / 8] = IDMA_OBS_MAGIC_V6;
    hw[0].regs[IDMA_LAUNCHER_MAGIC_OFFSET / 8] = 0x49444d414c4e4301ULL;

    memset(&c, 0, sizeof(c));
    c.pair.abi_version = 1;
    c.pair.struct_size = sizeof(c.pair);
    c.pair.request = e;

    if (!strcmp(name, "invalid_before_mmio")) {
        c.pair.abi_version = 0;
        ret = idma_run_e3_controlled(&idmas[0], &c);
        assert(ret == -EINVAL);
        assert(!allocations && !obs_mmio && !controlled_gos);
        return 0;
    }
    if (!strcmp(name, "cold_solo")) {
        c.pair.request.aggressor_id = IDMA_EXPERIMENT_NO_AGGRESSOR;
        c.prep_mode = 0;
        controlled_mode = true;
        ret = idma_run_e3_controlled(&idmas[0], &c);
        assert(ret == 0 || ret == -EIO);
        return 0;
    }
    if (!strcmp(name, "hot_pair")) {
        c.prep_mode = 1;
        c.pair_mode = 1;
        controlled_mode = true;
        controlled_issue_a = true;
        ret = idma_run_e3_controlled(&idmas[0], &c);
        assert(ret == 0 || ret == -EIO);
        return 0;
    }
    if (!strcmp(name, "probe_miss_no_go")) {
        c.prep_mode = 0;
        controlled_probe_miss = true;
        ret = idma_run_e3_controlled(&idmas[0], &c);
        assert(ret == -EIO);
        assert(!controlled_gos);
        return 0;
    }
    if (!strcmp(name, "flush_failure")) {
        controlled_flush_fail = true;
        ret = idma_run_e3_controlled(&idmas[0], &c);
        assert(ret == -EIO);
        return 0;
    }
    if (!strcmp(name, "cancel_v_no_bytes")) {
        controlled_cancel_v = true;
        controlled_mode = true;
        ret = idma_run_e3_controlled(&idmas[0], &c);
        assert(ret == 0 || ret == -EIO);
        return 0;
    }
    if (!strcmp(name, "abort_partial_hold")) {
        controlled_abort_hold = true;
        controlled_mode = true;
        ret = idma_run_e3_controlled(&idmas[0], &c);
        assert(ret != 0);
        return 0;
    }
    if (!strcmp(name, "raw_failure_evidence")) {
        controlled_raw_error = true;
        controlled_mode = true;
        ret = idma_run_e3_controlled(&idmas[0], &c);
        assert(ret == -EIO);
        return 0;
    }
    assert(!"unknown test");
    return 1;
}
```

### `tests/test_idma_controlled.py`

```python
#!/usr/bin/env python3
"""Compile and run production controlled-E3 functions against a host MMIO model.

This establishes software control flow only. It is not FPGA timing or hardware
acceptance evidence. Kernel DMA mapping, MMIO ordering, userspace copies and
the HPM flush implementation are stubbed or supplied externally.
"""
import os
from pathlib import Path
import re
import shutil
import subprocess
import tempfile
import unittest

ROOT = Path(__file__).resolve().parents[1]
FUNCTIONS = (
    'idma_pattern', 'idma_stream_cleanup', 'idma_quarantine', 'idma_submit',
    'idma_poll', 'idma_run_window', 'idma_drain', 'idma_verify',
    'idma_snapshot', 'idma_snapshot_delta',
    'idma_observation_prepare', 'idma_observation_arm', 'idma_observation_finish',
    'idma_e3_finish', 'idma_stream_reseed',
    'idma_launcher_terminal', 'idma_launcher_read_words',
    'idma_controlled_validate', 'idma_controlled_environment',
    'idma_controlled_prime', 'idma_controlled_resolve_launcher',
    'idma_controlled_formal', 'idma_controlled_one_round',
    'idma_run_e3_controlled',
)
CASES = (
    'invalid_before_mmio',
    'cold_solo',
    'hot_pair',
    'probe_miss_no_go',
    'flush_failure',
    'cancel_v_no_bytes',
    'abort_partial_hold',
    'raw_failure_evidence',
)


def extract(source, pattern, label):
    matches = re.findall(pattern, source, re.MULTILINE | re.DOTALL)
    if len(matches) != 1:
        raise RuntimeError(f'Expected exactly one production {label}, got {len(matches)}')
    return matches[0]


class IdmaControlledTest(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        cls.temp = tempfile.TemporaryDirectory()
        cls.addClassCleanup(cls.temp.cleanup)
        build = Path(cls.temp.name)
        source = Path(os.environ.get(
            'IDMA_SOURCE', ROOT / 'linux/drivers/misc/idma.c')).read_text()
        definitions = '\n'.join(re.findall(r'^#define IDMA_.*$', source, re.MULTILINE))
        structures = '\n'.join(
            extract(source, rf'^struct {name} \{{.*?^\}};', name)
            for name in ('idma_device', 'idma_stream', 'idma_observation')
        )
        functions = '\n'.join(
            extract(source, rf'^static [^\n]*\b{name}\(.*?^\}}', name)
            for name in FUNCTIONS
        )
        harness = (ROOT / 'tests/idma_controlled_harness.c').read_text()
        harness = harness.replace('/* PRODUCTION_TYPES */', definitions + '\n' + structures)
        harness = harness.replace('/* PRODUCTION_FUNCTIONS */', functions)
        (build / 'linux').mkdir()
        shutil.copyfile(ROOT / 'linux/include/uapi/linux/idma.h', build / 'linux/idma.h')
        (build / 'controlled.c').write_text(harness)
        cls.binary = build / 'controlled'
        subprocess.run([
            os.environ.get('HOSTCC', 'cc'), '-std=gnu11', '-O0', '-g', '-Wall',
            '-Wextra', '-Werror', '-I', str(build), str(build / 'controlled.c'),
            '-o', str(cls.binary),
        ], check=True)

    def test_production_controlled_control_flow(self):
        for case in CASES:
            with self.subTest(case=case):
                result = subprocess.run([str(self.binary), case], capture_output=True,
                                        text=True, timeout=10)
                self.assertEqual(result.returncode, 0, result.stdout + result.stderr)


if __name__ == '__main__':
    unittest.main()
```

## Assumptions / unresolved issues

- `riscv_iommu_hpm_flush_s1()` is assumed to be declared by the separate HPM worker in `linux/include/linux/riscv_iommu_hpm.h`. If that header is not updated in the same tree, `idma.c` will need a temporary compatible prototype or the header patch applied first.
- The launcher status bit positions and held/active modulo semantics are implemented as bitfield counts (`held` = bits 8..11, `active` = bits 12..15). If the hardware defines them as one-hot flags instead of counts, the checks must be adjusted.
- The controlled formal path intentionally calls `idma_observation_finish()` and `idma_e3_finish()`. The fake harness currently returns simple observer/E3 values; the parent must verify whether controlled V6/bank-v2 word semantics need additional `idma_e3_finish()` gating beyond `n = 1` and bit 29.
- The host harness is a control-flow model only. It does not prove IOTLB flush behavior, hardware timestamp ordering, actual backpressure, or FPGA acceptance.
- The test harness currently exercises the listed cases at a coarse level. The parent should expand assertions for exact launcher words, cancellation accounting, no-free-until-safe, and raw error evidence once the actual V6/bank-v2 hardware captures are available.