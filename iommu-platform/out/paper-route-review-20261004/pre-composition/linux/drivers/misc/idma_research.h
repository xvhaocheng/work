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

#define IDMA_RESEARCH2_CAP_INDEX 0xf00U
#define IDMA_RESEARCH2_CAP_VERSION 1ULL
#define IDMA_RESEARCH2_FEATURE_EXTENDED_E2 BIT_ULL(0)
#define IDMA_RESEARCH2_FEATURE_E2_TIMING BIT_ULL(1)
#define IDMA_RESEARCH2_FEATURE_REVERSE_A BIT_ULL(2)
#define IDMA_RESEARCH2_FEATURE_METHOD BIT_ULL(3)
#define IDMA_RESEARCH2_FEATURE_METHOD_MASK (IDMA_RESEARCH2_FEATURE_EXTENDED_E2 | IDMA_RESEARCH2_FEATURE_E2_TIMING | IDMA_RESEARCH2_FEATURE_REVERSE_A | IDMA_RESEARCH2_FEATURE_METHOD)
#define IDMA_RESEARCH2_REVERSE_INDEX 0xf10U
#define IDMA_RESEARCH2_METHOD_INDEX 0xe00U
#define IDMA_RESEARCH2_METHOD_E02_ERROR 0xe02U
#define IDMA_RESEARCH2_METHOD_E03_STATUS 0xe03U
#define IDMA_RESEARCH2_METHOD_E04_COMMAND 0xe04U
#define IDMA_RESEARCH2_METHOD_E05_MODE 0xe05U
#define IDMA_RESEARCH2_METHOD_E06_WINDOW 0xe06U
#define IDMA_RESEARCH2_METHOD_E07_TARGET 0xe07U
#define IDMA_RESEARCH2_METHOD_E08_DEADBAND 0xe08U
#define IDMA_RESEARCH2_METHOD_E09_INITIAL 0xe09U
#define IDMA_RESEARCH2_METHOD_E0A_MIN 0xe0aU
#define IDMA_RESEARCH2_METHOD_E0B_MAX 0xe0bU
#define IDMA_RESEARCH2_METHOD_E0C_STEP 0xe0cU
#define IDMA_RESEARCH2_METHOD_E0D_CAP 0xe0dU
#define IDMA_RESEARCH2_METHOD_E0E_HIT 0xe0eU
#define IDMA_RESEARCH2_METHOD_E0F_MISS 0xe0fU
#define IDMA_RESEARCH2_METHOD_E10_PTW 0xe10U
#define IDMA_RESEARCH2_METHOD_ARM 1ULL
#define IDMA_RESEARCH2_METHOD_ABORT 2ULL
#define IDMA_RESEARCH2_METHOD_FREEZE 3ULL
#define IDMA_RESEARCH2_METHOD_CLEAR 4ULL
#define IDMA_RESEARCH2_METHOD_STATUS_ARMED BIT_ULL(0)
#define IDMA_RESEARCH2_METHOD_STATUS_FROZEN BIT_ULL(1)
#define IDMA_RESEARCH2_METHOD_STATUS_ABORT BIT_ULL(2)
#define IDMA_RESEARCH2_E2_TIMING_INDEX 0xd80U

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
	/* A zero magic keeps old BITs usable; never accept a corrupt new bank. */
	if (r->mode == IDMA_RESEARCH_COST && raw[0xd00] &&
	    (raw[0xd00] != 0x49444d4141474731ULL || raw[0xd01] != 1 ||
	     raw[0xd02] != 6 || raw[0xd03] != 16 || raw[0xd04] ||
	     raw[0xd05] != raw[0xc14]))
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

static int idma_research2_set_reverse(struct idma_device *d, u32 reverse)
{
	idma_research_write(d, IDMA_RESEARCH2_REVERSE_INDEX, reverse);
	return idma_research_read(d, IDMA_RESEARCH2_REVERSE_INDEX) == reverse ? 0 : -EIO;
}

static int idma_research2_method_status_read(struct idma_device *d,
					     const struct idma_research2_config *cfg,
					     u64 *status)
{
	if (!cfg || cfg->required_role != IDMA_RESEARCH2_METHOD_ROLE)
		return -EINVAL;
	if (idma_research_read(d, IDMA_RESEARCH2_METHOD_INDEX) !=
	    IDMA_RESEARCH2_POLICY_MAGIC ||
	    idma_research_read(d, IDMA_RESEARCH2_METHOD_INDEX + 1) != 1ULL)
		return -EOPNOTSUPP;
	*status = idma_research_read(d, IDMA_RESEARCH2_METHOD_E03_STATUS);
	return 0;
}

static int idma_research2_method_configure(struct idma_device *d,
					   const struct idma_research2_config *cfg)
{
	const u32 values[] = {
		cfg->policy_mode, cfg->window_cycles, cfg->target_bytes,
		cfg->deadband_bytes, cfg->initial_budget, cfg->min_budget,
		cfg->max_budget, cfg->budget_step, cfg->credit_cap,
		cfg->hit_weight, cfg->miss_weight, cfg->ptw_weight,
	};
	u64 status;
	u32 i;

	if (idma_research_read(d, IDMA_RESEARCH2_METHOD_INDEX) !=
	    IDMA_RESEARCH2_POLICY_MAGIC ||
	    idma_research_read(d, IDMA_RESEARCH2_METHOD_INDEX + 1) != 1ULL)
		return -EOPNOTSUPP;
	status = idma_research_read(d, IDMA_RESEARCH2_METHOD_E03_STATUS);
	if (status & IDMA_RESEARCH2_METHOD_STATUS_ARMED)
		return -EBUSY;
	idma_research_write(d, IDMA_RESEARCH2_METHOD_E04_COMMAND,
			    IDMA_RESEARCH2_METHOD_CLEAR);
	if (idma_research_read(d, IDMA_RESEARCH2_METHOD_E02_ERROR))
		return -EIO;
	status = idma_research_read(d, IDMA_RESEARCH2_METHOD_E03_STATUS);
	if (status & (IDMA_RESEARCH2_METHOD_STATUS_ARMED |
		      IDMA_RESEARCH2_METHOD_STATUS_FROZEN |
		      IDMA_RESEARCH2_METHOD_STATUS_ABORT))
		return -EIO;

	for (i = 0; i < sizeof(values) / sizeof(values[0]); i++) {
		idma_research_write(d, IDMA_RESEARCH2_METHOD_E05_MODE + i,
				    values[i]);
		if (idma_research_read(d, IDMA_RESEARCH2_METHOD_E05_MODE + i) !=
		    values[i])
			return -EIO;
	}
	if (idma_research_read(d, IDMA_RESEARCH2_METHOD_E02_ERROR))
		return -EIO;
	idma_research_write(d, IDMA_RESEARCH2_METHOD_E04_COMMAND,
			    IDMA_RESEARCH2_METHOD_ARM);
	if (idma_research_read(d, IDMA_RESEARCH2_METHOD_E02_ERROR))
		return -EIO;
	status = idma_research_read(d, IDMA_RESEARCH2_METHOD_E03_STATUS);
	if (!(status & IDMA_RESEARCH2_METHOD_STATUS_ARMED) ||
	    (status & IDMA_RESEARCH2_METHOD_STATUS_FROZEN) ||
	    (status & IDMA_RESEARCH2_METHOD_STATUS_ABORT))
		return -EIO;
	return 0;
}

static void idma_research2_method_abort(struct idma_device *d,
					const struct idma_research2_config *cfg,
					int error)
{
	u64 status;
	int ret;

	if (!cfg || cfg->required_role != IDMA_RESEARCH2_METHOD_ROLE || !error)
		return;
	ret = idma_research2_method_status_read(d, cfg, &status);
	if (ret || !(status & IDMA_RESEARCH2_METHOD_STATUS_ARMED))
		return;
	idma_research_write(d, IDMA_RESEARCH2_METHOD_E04_COMMAND,
			    IDMA_RESEARCH2_METHOD_ABORT);
}

static int idma_research2_method_freeze(struct idma_device *d,
					const struct idma_research2_config *cfg,
					int error)
{
	u64 status;
	int ret;

	if (!cfg || cfg->required_role != IDMA_RESEARCH2_METHOD_ROLE)
		return 0;
	ret = idma_research2_method_status_read(d, cfg, &status);
	if (ret || !(status & IDMA_RESEARCH2_METHOD_STATUS_ARMED))
		return 0;
	if (error)
		idma_research_write(d, IDMA_RESEARCH2_METHOD_E04_COMMAND,
				    IDMA_RESEARCH2_METHOD_ABORT);
	idma_research_write(d, IDMA_RESEARCH2_METHOD_E04_COMMAND,
			    IDMA_RESEARCH2_METHOD_FREEZE);
	if (error)
		return 0;
	status = idma_research_read(d, IDMA_RESEARCH2_METHOD_E03_STATUS);
	if (idma_research_read(d, IDMA_RESEARCH2_METHOD_E02_ERROR) ||
	    !(status & IDMA_RESEARCH2_METHOD_STATUS_FROZEN) ||
	    (status & IDMA_RESEARCH2_METHOD_STATUS_ARMED) ||
	    (status & IDMA_RESEARCH2_METHOD_STATUS_ABORT))
		return -EIO;
	return 0;
}

static int idma_research2_method_readback_ok(struct idma_device *d,
					     const struct idma_research2_config *cfg)
{
	const u32 expected[] = {
		cfg->policy_mode, cfg->window_cycles, cfg->target_bytes,
		cfg->deadband_bytes, cfg->initial_budget, cfg->min_budget,
		cfg->max_budget, cfg->budget_step, cfg->credit_cap,
		cfg->hit_weight, cfg->miss_weight, cfg->ptw_weight,
	};
	u64 status;
	u32 i;

	if (idma_research_read(d, IDMA_RESEARCH2_METHOD_INDEX) !=
	    IDMA_RESEARCH2_POLICY_MAGIC ||
	    idma_research_read(d, IDMA_RESEARCH2_METHOD_INDEX + 1) != 1ULL ||
	    idma_research_read(d, IDMA_RESEARCH2_METHOD_E02_ERROR))
		return -EIO;
	status = idma_research_read(d, IDMA_RESEARCH2_METHOD_E03_STATUS);
	if (!(status & IDMA_RESEARCH2_METHOD_STATUS_FROZEN) ||
	    (status & IDMA_RESEARCH2_METHOD_STATUS_ARMED) ||
	    (status & IDMA_RESEARCH2_METHOD_STATUS_ABORT))
		return -EIO;
	for (i = 0; i < sizeof(expected) / sizeof(expected[0]); i++) {
		if (idma_research_read(d, IDMA_RESEARCH2_METHOD_E05_MODE + i) !=
		    expected[i])
			return -EIO;
	}
	return 0;
}

static int idma_research2_pre_arm(struct idma_device *d,
				  const struct idma_research2_config *cfg)
{
	int ret;

	if (!cfg)
		return 0;
	if (idma_research_read(d, IDMA_RESEARCH2_METHOD_INDEX) !=
	    IDMA_RESEARCH2_POLICY_MAGIC ||
	    idma_research_read(d, IDMA_RESEARCH2_METHOD_INDEX + 1) != 1ULL)
		return -EOPNOTSUPP;
	ret = idma_research2_set_reverse(d, cfg->reverse);
	if (ret)
		return ret;
	if (cfg->required_role == IDMA_RESEARCH2_METHOD_ROLE)
		return idma_research2_method_configure(d, cfg);
	return 0;
}

static int idma_research2_config_validate(const struct idma_research2_config *cfg,
					  u32 role)
{
	if (!cfg || cfg->abi_version != IDMA_RESEARCH2_ABI ||
	    cfg->struct_size != sizeof(*cfg) ||
	    memchr_inv(cfg->reserved, 0, sizeof(cfg->reserved)))
		return -EINVAL;
	if (cfg->required_role != role || cfg->reverse > 1)
		return -EINVAL;
	if (role == IDMA_RESEARCH2_E2_ROLE) {
		if (cfg->policy_mode || cfg->window_cycles || cfg->target_bytes ||
		    cfg->deadband_bytes || cfg->initial_budget ||
		    cfg->min_budget || cfg->max_budget || cfg->budget_step ||
		    cfg->credit_cap || cfg->hit_weight || cfg->miss_weight ||
		    cfg->ptw_weight)
			return -EINVAL;
		return 0;
	}
	if (role == IDMA_RESEARCH2_METHOD_ROLE) {
		u32 mode = cfg->policy_mode;
		u32 min = cfg->min_budget;
		u32 init = cfg->initial_budget;
		u32 max = cfg->max_budget;

		if (mode > 7)
			return -EINVAL;
		if (cfg->window_cycles < 1024 || cfg->window_cycles > 1048576)
			return -EINVAL;
		if (!cfg->target_bytes || cfg->target_bytes > 0x1000000)
			return -EINVAL;
		if (cfg->deadband_bytes > cfg->target_bytes)
			return -EINVAL;
		if (min > init || init > max || max > 0x1000000)
			return -EINVAL;
		if (!cfg->budget_step || cfg->budget_step > max)
			return -EINVAL;
		if (cfg->credit_cap < max || cfg->credit_cap > 0x4000000)
			return -EINVAL;
		if (((mode >= 1 && mode <= 5) || mode == 7) && min < 1)
			return -EINVAL;
		if (mode == 6 && max > 200000)
			return -EINVAL;
		if (mode == 4 && !cfg->hit_weight && !cfg->miss_weight &&
		    !cfg->ptw_weight)
			return -EINVAL;
		if (cfg->hit_weight > 65535 || cfg->miss_weight > 65535 ||
		    cfg->ptw_weight > 65535)
			return -EINVAL;
		return 0;
	}
	return -EINVAL;
}

static int idma_research2_base_validate(struct idma_device *v,
					const struct idma_research *r,
					const struct idma_research2_config *cfg)
{
	if (v->id != 0 || v->did != 10 || PAGE_SIZE != 4096)
		return -EINVAL;
	if (r->abi_version != IDMA_RESEARCH_ABI || r->struct_size != sizeof(*r))
		return -EPROTO;
	if (memchr_inv(r->reserved, 0, sizeof(r->reserved)) ||
	    r->raw_words != IDMA_RESEARCH_RAW_WORDS || !r->raw_user ||
	    !access_ok(u64_to_user_ptr(r->raw_user), r->raw_words * sizeof(u64)))
		return -EINVAL;
	if (cfg->required_role == IDMA_RESEARCH2_E2_ROLE) {
		if (r->mode != IDMA_RESEARCH_E2)
			return -EINVAL;
		if (r->a_pages != 0 && r->a_pages != 1 && r->a_pages != 4 &&
		    r->a_pages != 7 && r->a_pages != 8 && r->a_pages != 16)
			return -EINVAL;
		if (r->a_pages == 0) {
			if (r->a_transfers)
				return -EINVAL;
		} else if (r->a_transfers != 1 && r->a_transfers != 8 &&
			   r->a_transfers != 16 && r->a_transfers != 112) {
			return -EINVAL;
		}
		if (r->warmup != 2 || r->transfers != 1 || r->gap_cycles ||
		    r->window_cycles != 5000000 || r->timeout_ms != 2000)
			return -EINVAL;
	} else if (cfg->required_role == IDMA_RESEARCH2_METHOD_ROLE) {
		if (r->mode != IDMA_RESEARCH_COST)
			return -EINVAL;
		if (r->a_pages != 0 && r->a_pages != 1 &&
		    r->a_pages != 8 && r->a_pages != 16)
			return -EINVAL;
		if (r->gap_cycles && r->gap_cycles != 500 &&
		    r->gap_cycles != 1000 && r->gap_cycles != 2000 &&
		    r->gap_cycles != 4000)
			return -EINVAL;
		if (r->gap_cycles && cfg->policy_mode != 0)
			return -EINVAL;
		if (r->a_pages == 0 &&
		    (cfg->policy_mode || r->gap_cycles || cfg->reverse))
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

static int idma_research2_extra_validate(struct idma_device *d,
					 const struct idma_research2_config *cfg)
{
	u64 cap_magic, cap_version, cap_role, cap_features;

	if (!cfg)
		return 0;
	cap_magic = idma_research_read(d, IDMA_RESEARCH2_CAP_INDEX);
	cap_version = idma_research_read(d, IDMA_RESEARCH2_CAP_INDEX + 1);
	cap_role = idma_research_read(d, IDMA_RESEARCH2_CAP_INDEX + 2);
	cap_features = idma_research_read(d, IDMA_RESEARCH2_CAP_INDEX + 3);
	if (cap_magic != IDMA_RESEARCH2_CAP_MAGIC ||
	    cap_version != IDMA_RESEARCH2_CAP_VERSION ||
	    cap_role != cfg->required_role)
		return -EOPNOTSUPP;
	if (cfg->required_role == IDMA_RESEARCH2_E2_ROLE) {
		if (!(cap_features & IDMA_RESEARCH2_FEATURE_EXTENDED_E2) ||
		    !(cap_features & IDMA_RESEARCH2_FEATURE_E2_TIMING) ||
		    !(cap_features & IDMA_RESEARCH2_FEATURE_REVERSE_A))
			return -EOPNOTSUPP;
		if (idma_research_read(d, IDMA_RESEARCH2_E2_TIMING_INDEX) !=
		    IDMA_RESEARCH2_E2_TIMING_MAGIC ||
		    idma_research_read(d, IDMA_RESEARCH2_E2_TIMING_INDEX + 1) != 1ULL ||
		    idma_research_read(d, IDMA_RESEARCH2_E2_TIMING_INDEX + 2) ||
		    idma_research_read(d, IDMA_RESEARCH2_E2_TIMING_INDEX + 3) != 7ULL)
			return -EIO;
		return 0;
	}
	if (cfg->required_role == IDMA_RESEARCH2_METHOD_ROLE) {
		if ((cap_features & IDMA_RESEARCH2_FEATURE_METHOD_MASK) !=
		    IDMA_RESEARCH2_FEATURE_METHOD_MASK)
			return -EOPNOTSUPP;
		return idma_research2_method_readback_ok(d, cfg);
	}
	return -EINVAL;
}

static int idma_research2_preflight(struct idma_device *d,
				    const struct idma_research2_config *cfg)
{
	u64 magic, version, role, features;

	if (!cfg)
		return 0;
	if (d->id != 0 || d->did != 10)
		return -EINVAL;
	if (readq(d->reg + IDMA_OBS_MAGIC_OFFSET) != IDMA_OBS_MAGIC_V8)
		return -EOPNOTSUPP;
	magic = idma_research_read(d, IDMA_RESEARCH2_CAP_INDEX);
	version = idma_research_read(d, IDMA_RESEARCH2_CAP_INDEX + 1);
	role = idma_research_read(d, IDMA_RESEARCH2_CAP_INDEX + 2);
	features = idma_research_read(d, IDMA_RESEARCH2_CAP_INDEX + 3);
	if (magic != IDMA_RESEARCH2_CAP_MAGIC ||
	    version != IDMA_RESEARCH2_CAP_VERSION ||
	    role != cfg->required_role)
		return -EOPNOTSUPP;
	if (cfg->required_role == IDMA_RESEARCH2_E2_ROLE) {
		if (!(features & IDMA_RESEARCH2_FEATURE_EXTENDED_E2) ||
		    !(features & IDMA_RESEARCH2_FEATURE_E2_TIMING) ||
		    !(features & IDMA_RESEARCH2_FEATURE_REVERSE_A))
			return -EOPNOTSUPP;
	} else if (cfg->required_role == IDMA_RESEARCH2_METHOD_ROLE) {
		if ((features & IDMA_RESEARCH2_FEATURE_METHOD_MASK) !=
		    IDMA_RESEARCH2_FEATURE_METHOD_MASK)
			return -EOPNOTSUPP;
		/* Preserve an unfinished policy session before any warmup DMA. */
		if (idma_research_read(d, IDMA_RESEARCH2_METHOD_E03_STATUS) &
		    IDMA_RESEARCH2_METHOD_STATUS_ARMED)
			return -EBUSY;
	} else {
		return -EINVAL;
	}
	return 0;
}

static int idma_research_capture_ext(struct idma_device *d,
				     struct idma_research *r,
				     const struct idma_research2_config *cfg,
				     int error)
{
	int extra = idma_research2_extra_validate(d, cfg);
	int ret = idma_research_capture(d, r, error);

	if (extra && !ret)
		ret = extra;
	return ret;
}

static int idma_research_e2_cfg(struct idma_device *victim,
				struct idma_research *r,
				const struct idma_research2_config *cfg)
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
	if (cfg) {
		ret = idma_research2_preflight(victim, cfg);
		if (ret)
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
		a.reverse = cfg && cfg->reverse;
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
	if (cfg) {
		ret = idma_research2_set_reverse(victim, cfg->reverse);
		if (ret)
			goto drain;
	}
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
		tmp = idma_research_capture_ext(victim, r, cfg, ret);
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

static int idma_research_e2(struct idma_device *victim, struct idma_research *r)
{
	return idma_research_e2_cfg(victim, r, NULL);
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

static long idma_run_research2(struct idma_device *victim, void __user *uarg)
{
	struct idma_research2 *r2;
	struct idma_research *r;
	struct idma_research2_config *cfg;
	struct idma_experiment *e;
	long ret;

	r2 = memdup_user(uarg, sizeof(*r2));
	if (IS_ERR(r2))
		return PTR_ERR(r2);
	r = &r2->base;
	cfg = &r2->config;
	memset(&r->status, 0, sizeof(*r) - offsetof(struct idma_research, status));
	r->flush_status = -EINPROGRESS;
	ret = idma_research2_config_validate(cfg, cfg->required_role);
	if (ret)
		goto copy;
	ret = idma_research2_base_validate(victim, r, cfg);
	if (ret)
		goto copy;
	if (cfg->required_role == IDMA_RESEARCH2_E2_ROLE) {
		ret = idma_research_e2_cfg(victim, r, cfg);
	} else if (cfg->required_role == IDMA_RESEARCH2_METHOD_ROLE) {
		e = &r->p4m.experiment;
		e->abi_version = IDMA_EXPERIMENT_ABI;
		e->struct_size = sizeof(*e);
		e->aggressor_id = r->a_pages ? 1 : IDMA_EXPERIMENT_NO_AGGRESSOR;
		e->victim_pages = 1;
		e->aggressor_pages = r->a_pages ? r->a_pages : 1;
		e->chunk_bytes = PAGE_SIZE;
		e->run_timeout_ms = r->timeout_ms;
		e->victim_transfers = r->transfers;
		e->warmup_transfers = r->warmup;
		r->p4m.aggressor_gap_cycles = r->gap_cycles;
		ret = idma_p4m_validate_cfg(victim, &r->p4m, cfg);
		if (!ret)
			ret = idma_run_session2(victim, e, true, NULL, NULL, false,
						&r->p4m, r, cfg);
		if (!ret)
			r->stage = 5;
	} else {
		ret = -EINVAL;
	}
copy:
	r->status = ret;
	if (copy_to_user(uarg, r2, sizeof(*r2)))
		ret = -EFAULT;
	kfree(r2);
	return ret;
}

static long idma_research2_caps_ioctl(struct idma_device *idma, void __user *uarg)
{
	struct idma_research2_caps caps;
	long ret;

	if (idma->id != 0 || idma->did != 10)
		return -EINVAL;
	if (copy_from_user(&caps, uarg, sizeof(caps)))
		return -EFAULT;
	if (caps.abi_version != IDMA_RESEARCH2_ABI ||
	    caps.struct_size != sizeof(caps))
		return -EPROTO;
	ret = mutex_lock_interruptible(&idma_experiment_lock);
	if (ret)
		return ret;
	caps.magic = idma_research_read(idma, IDMA_RESEARCH2_CAP_INDEX);
	caps.version = idma_research_read(idma, IDMA_RESEARCH2_CAP_INDEX + 1);
	caps.role = idma_research_read(idma, IDMA_RESEARCH2_CAP_INDEX + 2);
	caps.features = idma_research_read(idma, IDMA_RESEARCH2_CAP_INDEX + 3);
	caps.observer_magic = readq(idma->reg + IDMA_OBS_MAGIC_OFFSET);
	if (caps.magic != IDMA_RESEARCH2_CAP_MAGIC ||
	    caps.version != IDMA_RESEARCH2_CAP_VERSION ||
	    (caps.role != IDMA_RESEARCH2_E2_ROLE &&
	     caps.role != IDMA_RESEARCH2_METHOD_ROLE) ||
	    caps.observer_magic != IDMA_OBS_MAGIC_V8) {
		ret = -EOPNOTSUPP;
	} else {
		ret = 0;
	}
	mutex_unlock(&idma_experiment_lock);
	if (ret)
		return ret;
	if (copy_to_user(uarg, &caps, sizeof(caps)))
		return -EFAULT;
	return 0;
}
