static int idma_g_read_gap_cap(struct idma_device *idma, u64 *value)
{
	u64 raw = 0;
	long ret;

	__io_br();
	ret = get_kernel_nofault(raw, (const u64 __force *)(idma->reg + IDMA_GAP_CAP_OFFSET));
	__io_ar(raw);
	if (ret)
		return -EOPNOTSUPP;
	*value = le64_to_cpu((__force __le64)raw);
	return 0;
}

static int idma_g_probe_locked(struct idma_device *victim,
		struct idma_device *aggressor, struct idma_g_caps *caps)
{
	u64 a_gap, info;
	int ret;

	if (!IS_REACHABLE(CONFIG_RISCV_IOMMU) || !IS_ENABLED(CONFIG_64BIT) ||
	    PAGE_SIZE != SZ_4K || victim->id != 0 || victim->did != 10 ||
	    (aggressor && (aggressor->id != 1 || aggressor->did != 11)) ||
	    readq(victim->reg + IDMA_BATCH_MAGIC_OFFSET) != IDMA_G_BATCH_MAGIC ||
	    (aggressor && readq(aggressor->reg + IDMA_BATCH_MAGIC_OFFSET) != IDMA_G_BATCH_MAGIC) ||
	    readq(victim->reg + IDMA_OBS_MAGIC_OFFSET) != IDMA_G_OBSERVER_MAGIC)
		return -EOPNOTSUPP;
	ret = idma_g_read_gap_cap(victim, &caps->gap_magic);
	if (!ret && aggressor)
		ret = idma_g_read_gap_cap(aggressor, &a_gap);
	if (ret)
		return ret;
	if (caps->gap_magic != IDMA_G_GAP_MAGIC || (aggressor && a_gap != IDMA_G_GAP_MAGIC))
		return -EOPNOTSUPP;
	writeq(IDMA_G_BANK_FIRST, victim->reg + IDMA_OBS_INDEX_OFFSET);
	caps->bank_magic = readq(victim->reg + IDMA_OBS_DATA_OFFSET);
	writeq(192, victim->reg + IDMA_OBS_INDEX_OFFSET);
	info = readq(victim->reg + IDMA_OBS_DATA_OFFSET);
	if (caps->bank_magic != IDMA_G_BANK_MAGIC || (info & 0xffffffffULL) != 0x20202001ULL || info >> 48)
		return -EOPNOTSUPP;
	caps->capabilities = IDMA_G_CAP_BATCH | IDMA_G_CAP_GAP | IDMA_G_CAP_WHOLE |
		IDMA_G_CAP_HIST | IDMA_G_CAP_RUN_READY;
	if (riscv_iommu_hpm_supported(victim->miscdev.parent))
		caps->capabilities |= IDMA_G_CAP_HPM;
	return 0;
}

static long idma_caps_g(struct idma_device *victim, void __user *uarg)
{
	struct idma_g_caps caps = {
		.abi_version = IDMA_G_ABI, .struct_size = sizeof(caps),
		.device_id = victim->id, .did = victim->did, .page_bytes = PAGE_SIZE,
		.victim_pages = 1, .aggressor_pages = 8,
		.chunk_bytes = IDMA_G_CHUNK_BYTES, .measured_transfers = IDMA_G_TRANSFERS,
		.warmup_transfers = IDMA_G_WARMUP_TRANSFERS,
		.max_run_timeout_ms = IDMA_G_MAX_TIMEOUT_MS,
		.batch_magic = IDMA_G_BATCH_MAGIC, .observer_magic = IDMA_G_OBSERVER_MAGIC,
		.gap_mask = IDMA_G_GAP_0 | IDMA_G_GAP_1000 | IDMA_G_GAP_2000,
		.bank_first = IDMA_G_BANK_FIRST, .bank_words = IDMA_G_BANK_WORDS,
	};
	struct idma_device *aggressor;
	long ret;

	if (!IS_REACHABLE(CONFIG_RISCV_IOMMU) || !IS_ENABLED(CONFIG_64BIT) ||
	    PAGE_SIZE != SZ_4K || victim->id != 0 || victim->did != 10)
		return -EOPNOTSUPP;
	ret = mutex_lock_interruptible(&idma_experiment_lock);
	if (ret)
		return ret;
	aggressor = idma_get_instance(1);
	ret = aggressor ? idma_g_probe_locked(victim, aggressor, &caps) : -EOPNOTSUPP;
	idma_put_instance(aggressor);
	mutex_unlock(&idma_experiment_lock);
	if (ret)
		return ret;
	return copy_to_user(uarg, &caps, sizeof(caps)) ? -EFAULT : 0;
}
