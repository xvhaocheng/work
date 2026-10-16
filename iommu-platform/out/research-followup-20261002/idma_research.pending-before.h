/* SPDX-License-Identifier: GPL-2.0 */
/* Included by idma.c: all MMIO and mappings share idma_experiment_lock. */
#define IDMA_RESEARCH_MAGIC 0x49444d4152455331ULL
#define IDMA_RESEARCH_BANK 512U
#define RESEARCH_MODE 4U
#define RESEARCH_COMMAND 5U
#define RESEARCH_PHASE 6U
#define RESEARCH_ERROR 3U
#define RESEARCH_QUIET BIT_ULL(21)
#define RESEARCH_CLEAR 6U
#define RESEARCH_ABORT 5U

static u64 idma_research_read(struct idma_device *d, u32 index)
{
	writeq(IDMA_RESEARCH_BANK + index, d->reg + IDMA_OBS_INDEX_OFFSET);
	return readq(d->reg + IDMA_OBS_DATA_OFFSET);
}

static void idma_research_write(struct idma_device *d, u32 index, u64 value)
{
	writeq(IDMA_RESEARCH_BANK + index, d->reg + IDMA_OBS_INDEX_OFFSET);
	writeq(value, d->reg + IDMA_OBS_DATA_OFFSET);
}

static int idma_research_quiet(struct idma_device *d, u64 deadline)
{
	while (!(idma_research_read(d, 2) & RESEARCH_QUIET)) {
		if (ktime_get_ns() >= deadline)
			return -ETIMEDOUT;
		if (signal_pending(current))
			return -EINTR;
		cond_resched();
		cpu_relax();
	}
	return 0;
}

static int idma_research_command(struct idma_device *d, u64 command,
				 u64 expected_phase)
{
	idma_research_write(d, RESEARCH_COMMAND, command);
	if (idma_research_read(d, RESEARCH_ERROR) ||
	    idma_research_read(d, RESEARCH_PHASE) != expected_phase)
		return -EIO;
	return 0;
}

static int idma_research_begin(struct idma_device *d, struct idma_research *r)
{
	int ret;

	r->observer_magic = readq(d->reg + IDMA_OBS_MAGIC_OFFSET);
	if (r->observer_magic != IDMA_OBS_MAGIC_V8 ||
	    idma_research_read(d, 0) != IDMA_RESEARCH_MAGIC)
		return -EOPNOTSUPP;
	ret = idma_research_quiet(d, ktime_get_ns() + NSEC_PER_SEC);
	if (ret)
		return ret;
	/* Never erase an unfinished observation. CLEAR enforces this in RTL. */
	ret = idma_research_command(d, RESEARCH_CLEAR, 0);
	if (ret)
		return ret;
	r->stage = 1;
	idma_research_write(d, RESEARCH_MODE, r->mode);
	return idma_research_read(d, RESEARCH_MODE) != r->mode ||
	       idma_research_read(d, RESEARCH_ERROR) ? -EIO : 0;
}

static int idma_research_capture(struct idma_device *d,
				struct idma_research *r, int error)
{
	u64 *raw;
	u32 i;
	int ret = 0;

	if (!r->stage)
		return error ? error : -EIO;
	if (error)
		idma_research_write(d, RESEARCH_COMMAND, RESEARCH_ABORT);
	raw = kcalloc(IDMA_RESEARCH_RAW_WORDS, sizeof(*raw), GFP_KERNEL);
	if (!raw)
		return -ENOMEM;
	for (i = 0; i < IDMA_RESEARCH_RAW_WORDS; i++)
		raw[i] = idma_research_read(d, i);
	if (raw[0] != IDMA_RESEARCH_MAGIC || raw[1] != IDMA_OBS_MAGIC_V8 || raw[3])
		ret = -EIO;
	if (r->mode == IDMA_RESEARCH_E2 && raw[6] != 4)
		ret = -EIO;
	if (r->mode == IDMA_RESEARCH_COST &&
	    (raw[0xc00] != 0x49444d4152454331ULL || raw[0xc02] ||
	     raw[0xc01] != 6 || raw[0xc19] != r->p4m.window[1] ||
	     raw[0xc11] != r->transfers || raw[0xc12] != r->transfers))
		ret = -EIO;
	if (copy_to_user(u64_to_user_ptr(r->raw_user), raw,
			 IDMA_RESEARCH_RAW_WORDS * sizeof(*raw)))
		ret = -EFAULT;
	else
		r->raw_captured = IDMA_RESEARCH_RAW_WORDS;
	kfree(raw);
	return ret;
}

static int idma_research_validate(struct idma_device *v,
				 const struct idma_research *r)
{
	if (v->id != 0 || v->did != 10 || PAGE_SIZE != 4096)
		return -EINVAL;
	if (r->abi_version != IDMA_RESEARCH_ABI || r->struct_size != sizeof(*r))
		return -EPROTO;
	if (memchr_inv(r->reserved, 0, sizeof(r->reserved)) ||
	    r->raw_words != IDMA_RESEARCH_RAW_WORDS || !r->raw_user ||
	    !access_ok(u64_to_user_ptr(r->raw_user), r->raw_words * sizeof(u64)))
		return -EINVAL;
	if (r->mode == IDMA_RESEARCH_E2) {
		if (r->a_pages != 0 && r->a_pages != 1 && r->a_pages != 4 &&
		    r->a_pages != 7 && r->a_pages != 8 && r->a_pages != 16)
			return -EINVAL;
		if (r->a_transfers != (r->a_pages ? 112 : 0) ||
		    r->warmup != 2 || r->transfers != 1 || r->gap_cycles ||
		    r->window_cycles != 5000000 || r->timeout_ms != 2000)
			return -EINVAL;
	} else if (r->mode == IDMA_RESEARCH_COST) {
		if (r->a_pages != 0 && r->a_pages != 1 &&
		    r->a_pages != 8 && r->a_pages != 16)
			return -EINVAL;
		if (r->gap_cycles && (r->a_pages != 8 ||
		    (r->gap_cycles != 1000 && r->gap_cycles != 2000)))
			return -EINVAL;
		if (r->a_transfers || r->window_cycles || r->timeout_ms != 10000 ||
		    !((r->transfers == 64 && r->warmup == 8) ||
		      (r->transfers == 4096 && r->warmup == 256)))
			return -EINVAL;
	} else {
		return -EINVAL;
	}
	return 0;
}

static int idma_research_e2(struct idma_device *victim, struct idma_research *r)
{
	struct idma_stream v = {}, a = {};
	struct idma_device *aggressor = NULL;
	struct riscv_iommu_hpm_session *hpm = NULL;
	struct riscv_iommu_hpm_sample before, after;
	struct idma_batch_start batch_start = {};
	u64 active_before[2], active_after[2], deadline, end = 0;
	int ret, tmp;
	bool prepared = false;

	ret = mutex_lock_interruptible(&idma_experiment_lock);
	if (ret)
		return ret;
	if (idma_any_quarantined()) {
		ret = -EIO;
		goto unlock;
	}
	/* Legacy observer may contain a frozen result, but must not be active. */
	if (readq(victim->reg + IDMA_OBS_CONTROL_OFFSET) & 3) {
		ret = -EBUSY;
		goto unlock;
	}
	if (r->a_pages) {
		aggressor = idma_get_instance(1);
		if (!aggressor || aggressor->did != 11) {
			ret = -ENODEV;
			goto cleanup;
		}
	}
	ret = idma_stream_prepare(&v, victim, 1, PAGE_SIZE, &r->phase_stats[0]);
	if (ret)
		goto cleanup;
	if (aggressor) {
		ret = idma_stream_prepare(&a, aggressor, r->a_pages, PAGE_SIZE,
					  &r->phase_stats[1]);
		if (ret)
			goto cleanup;
	}
	r->mapping[0] = v.src_iova;
	r->mapping[1] = v.dst_iova;
	r->mapping[2] = a.src_iova;
	r->mapping[3] = a.dst_iova;
	ret = riscv_iommu_hpm_acquire(v.dev, a.dev, &hpm);
	if (ret)
		goto cleanup;
	r->flush_status = riscv_iommu_hpm_flush_s1(hpm);
	if (r->flush_status) {
		ret = r->flush_status;
		goto drain;
	}
	/* The sole platform cache workaround, before any DMA touches buffers. */
	local_flush_icache_all();
	ret = idma_research_begin(victim, r);
	prepared = r->stage != 0;
	if (ret)
		goto drain;
	idma_research_write(victim, 0x30, v.src_iova);
	idma_research_write(victim, 0x31, v.dst_iova);
	ret = idma_research_command(victim, 1, 1);
	if (ret)
		goto drain;
	r->stage = 2;
	deadline = ktime_get_ns() + (u64)r->timeout_ms * NSEC_PER_MSEC;
	idma_snapshot(hpm, &v, a.dev ? &a : NULL, &before, active_before);
	ret = idma_run_window(&v, NULL, r->warmup, deadline, true, &end);
	idma_snapshot(hpm, &v, a.dev ? &a : NULL, &after, active_after);
	idma_snapshot_delta(&r->phase_stats[0], 0, &before, &after,
			    active_before[0], active_after[0]);
	if (ret)
		goto drain;
	if (a.dev) {
		ret = idma_batch_configure(&a, r->a_transfers);
		if (!ret)
			ret = idma_gap_program_readback(&a, 0);
		if (ret)
			goto drain;
	}
	ret = idma_research_quiet(victim, deadline);
	if (!ret)
		ret = idma_research_command(victim, 2, 2);
	if (ret)
		goto drain;
	r->stage = 3;
	if (a.dev) {
		idma_snapshot(hpm, &v, &a, &before, active_before);
		ret = idma_batch_window(&a, NULL, deadline, &end, &batch_start);
		idma_snapshot(hpm, &v, &a, &after, active_after);
		idma_snapshot_delta(&r->phase_stats[1], 1, &before, &after,
				    active_before[1], active_after[1]);
		if (!ret)
			ret = idma_batch_collect(&a);
		if (!ret && r->phase_stats[1].completed != r->a_transfers)
			ret = -EIO;
		if (ret)
			goto drain;
	}
	while (idma_research_read(victim, RESEARCH_PHASE) != 6) {
		if (idma_research_read(victim, RESEARCH_ERROR)) {
			ret = -EIO;
			goto drain;
		}
		if (ktime_get_ns() >= deadline || signal_pending(current)) {
			ret = signal_pending(current) ? -EINTR : -ETIMEDOUT;
			goto drain;
		}
		cond_resched();
		cpu_relax();
	}
	ret = idma_research_command(victim, 3, 3);
	if (ret)
		goto drain;
	r->stage = 4;
	v.stats = &r->phase_stats[2];
	v.next_offset = 0;
	idma_snapshot(hpm, &v, a.dev ? &a : NULL, &before, active_before);
	ret = idma_run_window(&v, NULL, 1, deadline, true, &end);
	idma_snapshot(hpm, &v, a.dev ? &a : NULL, &after, active_after);
	idma_snapshot_delta(&r->phase_stats[2], 0, &before, &after,
			    active_before[0], active_after[0]);
	if (!ret)
		ret = idma_research_quiet(victim, deadline);
	if (!ret)
		ret = idma_research_command(victim, 4, 4);
drain:
	idma_batch_stop(&a);
	tmp = idma_drain(&v);
	if (!ret)
		ret = tmp;
	if (a.dev) {
		tmp = idma_drain(&a);
		if (!ret)
			ret = tmp;
		tmp = idma_batch_collect(&a);
		if (!ret)
			ret = tmp;
	}
	/* Snapshot before release/unmap can issue any IOMMU invalidation. */
	if (prepared) {
		tmp = idma_research_quiet(victim, ktime_get_ns() + NSEC_PER_SEC);
		if (tmp) {
			if (!v.retain)
				idma_quarantine(&v);
			if (a.dev && !a.retain)
				idma_quarantine(&a);
			if (!ret)
				ret = tmp;
		}
		tmp = idma_research_capture(victim, r, ret);
		if (!ret)
			ret = tmp;
	}
	riscv_iommu_hpm_release(hpm);
	if (!ret) {
		local_flush_icache_all();
		ret = idma_verify(&v);
		if (a.dev) {
			tmp = idma_verify(&a);
			if (!ret)
				ret = tmp;
		}
	}
	if (!ret)
		r->stage = 5;
cleanup:
	idma_batch_disable(&a);
	idma_stream_cleanup(&a);
	idma_stream_cleanup(&v);
	idma_put_instance(aggressor);
unlock:
	mutex_unlock(&idma_experiment_lock);
	return ret;
}

static long idma_run_research(struct idma_device *victim, void __user *uarg)
{
	struct idma_research *r;
	struct idma_experiment *e;
	long ret;

	r = memdup_user(uarg, sizeof(*r));
	if (IS_ERR(r))
		return PTR_ERR(r);
	memset(&r->status, 0, sizeof(*r) - offsetof(struct idma_research, status));
	r->flush_status = -EINPROGRESS;
	ret = idma_research_validate(victim, r);
	if (ret)
		goto copy;
	if (r->mode == IDMA_RESEARCH_E2) {
		ret = idma_research_e2(victim, r);
	} else {
		e = &r->p4m.experiment;
		e->abi_version = IDMA_EXPERIMENT_ABI;
		e->struct_size = sizeof(*e);
		e->aggressor_id = r->a_pages ? 1 : IDMA_EXPERIMENT_NO_AGGRESSOR;
		e->victim_pages = 1;
		/* Legacy validation needs a positive page count even for Solo.
		 * NO_AGGRESSOR still prevents any A allocation or submission;
		 * the research request keeps a_pages == 0 for the Solo condition.
		 */
		e->aggressor_pages = r->a_pages ? r->a_pages : 1;
		e->chunk_bytes = PAGE_SIZE;
		e->run_timeout_ms = r->timeout_ms;
		e->victim_transfers = r->transfers;
		e->warmup_transfers = r->warmup;
		r->p4m.aggressor_gap_cycles = r->gap_cycles;
		ret = idma_p4m_validate(victim, &r->p4m);
		if (!ret)
			ret = idma_run_session(victim, e, true, NULL, NULL, false,
					       &r->p4m, r);
		if (!ret)
			r->stage = 5;
	}
copy:
	r->status = ret;
	if (copy_to_user(uarg, r, sizeof(*r)))
		ret = -EFAULT;
	kfree(r);
	return ret;
}
