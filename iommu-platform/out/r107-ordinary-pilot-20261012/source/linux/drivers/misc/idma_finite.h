/* SPDX-License-Identifier: GPL-2.0 */
/* Stage111: one source-derived finite ordinary replay, with owned DMA-API
 * pools. This is a CVA6 platform adapter, not native GPU inference.
 */
#include <linux/cpu.h>
#include <linux/iommu-finite.h>
#include <linux/sort.h>
#include <asm/csr.h>

#define FIN_SLOT 12288U
#define FIN_COMPONENT (8U * FIN_SLOT)
#define FIN_LAYER (2U * FIN_COMPONENT)
#define FIN_GUARD 0xd9

struct idma_finite_owner {
	struct device *dev;
	struct idma_device *idma;
	struct riscv_iommu_hpm_session *hpm;
	struct iommu_finite_allocation allocations[8];
	unsigned int nr, per_pool;
	u8 *seed[2];
	struct idma_finite_batch *batches;
	struct idma_finite_leaf *leaves;
	struct idma_finite_run *run;
	u64 deadline, active_id;
	bool triggered, unsafe;
};

/* A failed proof keeps the complete private ownership record until reboot.
 * No later ioctl may retry release or reuse a partly revoked reservation.
 */
static struct idma_finite_owner *idma_finite_retained;

static struct idma_finite_stamp idma_finite_now(void)
{
	struct idma_finite_stamp s;

	s.cpu_cycles = csr_read(CSR_CYCLE);
	s.time_ticks = csr_read(CSR_TIME);
	s.nominal_ns = ktime_get_ns();
	return s;
}

static void idma_finite_begin(struct idma_finite_run *r, unsigned int stage)
{
	r->stage = stage;
	r->stages[stage].reached = 1;
	r->stages[stage].status = -EINPROGRESS;
	r->stages[stage].begin = idma_finite_now();
}

static int idma_finite_end(struct idma_finite_run *r, unsigned int stage, int ret)
{
	r->stages[stage].end = idma_finite_now();
	r->stages[stage].status = ret;
	return ret;
}

/* Called under the existing experiment mutex; no ownership of busy hardware. */
static bool idma_finite_quiet(struct idma_device *d)
{
	unsigned int i;
	bool quiet = true;

	for (i = 0; i < 4; i++) {
		struct idma_device *other = idma_get_instance(i);

		if (!other)
			return false;
		if (READ_ONCE(other->quarantined) ||
		    readq(other->reg + IDMA_STATUS_REG_OFFSET) ||
		    (readq(other->reg + IDMA_BATCH_CONTROL_OFFSET) & 2))
			quiet = false;
		idma_put_instance(other);
	}
	return quiet && idma_composition_read(d, 5) == 1;
}

static int idma_finite_barrier(void *arg)
{
	struct idma_finite_owner *o = arg;

	if (!idma_finite_quiet(o->idma))
		return -EBUSY;
	/* On this single-hart CVA6, FENCE.I drains WT writes and invalidates the
	 * data cache. This is not a claim that the platform snoops DMA writes.
	 */
	local_flush_icache_all();
	return riscv_iommu_hpm_flush_s1(o->hpm);
}

static int idma_finite_release(struct idma_finite_owner *o)
{
	int ret;

	if (!o->nr)
		return 0;
	if (o->unsafe || o->triggered || !idma_finite_quiet(o->idma))
		return -EBUSY;
	ret = iommu_dma_free_finite_checked(o->dev, o->allocations, o->nr,
					  idma_finite_barrier, o);
	if (!ret) {
		memset(o->allocations, 0, sizeof(o->allocations));
		o->nr = 0;
	}
	return ret;
}

static int idma_finite_allocate(struct idma_finite_owner *o)
{
	unsigned int kind, i;
	int ret;

	for (kind = 1; kind <= 3; kind++) {
		size_t bytes = kind == 2 ? SZ_2M : SZ_4M;
		unsigned long attrs = kind == 3 ? 0 : DMA_ATTR_FORCE_CONTIGUOUS;

		o->per_pool = kind == 2 ? 2 : 1;
		for (i = 0; i < 4 * o->per_pool; i++) {
			struct iommu_finite_allocation *a = &o->allocations[i];

			a->size = bytes;
			a->cpu_addr = dma_alloc_attrs(o->dev, bytes, &a->handle,
						     GFP_KERNEL, attrs);
			if (!a->cpu_addr)
				break;
			o->nr++;
		}
		if (i == 4 * o->per_pool) {
			o->run->allocation_kind = kind;
			o->run->extent_count = o->nr;
			o->run->backing_bytes = IDMA_FINITE_BACKING_BYTES;
			return 0;
		}
		/* Successful partial allocations are not handed to legacy void
		 * unmap/free. The checked fence cost is part of stage1 fallback.
		 */
		ret = idma_finite_release(o);
		if (ret) {
			o->unsafe = true;
			return ret;
		}
	}
	return -ENOMEM;
}

static struct iommu_finite_allocation *idma_finite_extent(
	struct idma_finite_owner *o, unsigned int pool, size_t offset, size_t *within)
{
	size_t bytes = o->per_pool == 2 ? SZ_2M : SZ_4M;

	*within = offset % bytes;
	return &o->allocations[pool * o->per_pool + offset / bytes];
}

static int idma_finite_pool_copy(struct idma_finite_owner *o, unsigned int pool,
				void *buffer, bool to_pool, bool to_user)
{
	size_t off = 0, within, bytes;

	while (off < IDMA_FINITE_POOL_BYTES) {
		struct iommu_finite_allocation *a =
			idma_finite_extent(o, pool, off, &within);

		bytes = min_t(size_t, a->size - within, IDMA_FINITE_POOL_BYTES - off);
		if (to_user) {
			if (copy_to_user((void __user *)buffer + off,
					 a->cpu_addr + within, bytes))
				return -EFAULT;
		} else if (to_pool) {
			memcpy(a->cpu_addr + within, buffer + off, bytes);
		} else {
			memcpy(buffer + off, a->cpu_addr + within, bytes);
		}
		off += bytes;
	}
	return 0;
}

static int idma_finite_pa_cmp(const void *a, const void *b)
{
	u64 pa = *(const u64 *)a, pb = *(const u64 *)b;

	return (pa > pb) - (pa < pb);
}

static int idma_finite_audit(struct idma_finite_owner *o)
{
	u64 *pas;
	unsigned int i, nr_pa = 0;
	int ret = 0;

	pas = kvcalloc(IDMA_FINITE_BACKING_BYTES / PAGE_SIZE, sizeof(*pas), GFP_KERNEL);
	if (!pas)
		return -ENOMEM;
	for (i = 0; i < o->nr; i++) {
		struct iommu_finite_allocation *a = &o->allocations[i];
		size_t off = 0, n;

		while (off < a->size) {
			struct iommu_finite_leaf leaf;
			struct idma_finite_leaf *out;

			ret = riscv_iommu_finite_leaf(o->dev, a->handle + off, &leaf);
			if (ret)
				goto done;
			if (o->run->leaf_count == IDMA_FINITE_MAX_LEAVES ||
			    leaf.iova != a->handle + off || !leaf.bytes ||
			    leaf.bytes > a->size - off || !IS_ALIGNED(leaf.bytes, PAGE_SIZE) ||
			    (leaf.pte & 15) != 7) {
				ret = -EPROTO;
				goto done;
			}
			out = &o->leaves[o->run->leaf_count++];
			out->iova = leaf.iova;
			out->pa = leaf.pa;
			out->bytes = leaf.bytes;
			out->pte = leaf.pte;
			memcpy(out->table_pa, leaf.tables, sizeof(out->table_pa));
			out->table_count = leaf.table_count;
			out->pool = i / o->per_pool;
			out->extent = i % o->per_pool;
			for (n = 0; n < leaf.bytes; n += PAGE_SIZE) {
				void *addr = a->cpu_addr + off + n;
				struct page *page = idma_buffer_page(addr);

				if (!page || page_to_phys(page) != leaf.pa + n) {
					ret = -ESTALE;
					goto done;
				}
				pas[nr_pa++] = page_to_phys(page);
			}
			off += leaf.bytes;
		}
	}
	sort(pas, nr_pa, sizeof(*pas), idma_finite_pa_cmp, NULL);
	for (i = 1; i < nr_pa; i++)
		if (pas[i] == pas[i - 1]) {
			ret = -EXDEV;
			break;
		}
done:
	kvfree(pas);
	return ret;
}

static void idma_finite_hpm(struct idma_finite_owner *o, u64 *raw)
{
	struct riscv_iommu_hpm_sample sample;

	riscv_iommu_hpm_read(o->hpm, &sample);
	raw[0] = sample.cycles;
	raw[1] = sample.iotlb_miss[0]; raw[2] = sample.iotlb_miss[1];
	raw[3] = sample.s1_ptw[0]; raw[4] = sample.s1_ptw[1];
	raw[5] = sample.s2_ptw[0]; raw[6] = sample.s2_ptw[1];
}

static int idma_finite_capture(struct idma_finite_owner *o,
			       struct idma_observation *obs, unsigned int phase, int error)
{
	u64 *v = o->run->observer[phase], *c = o->run->composition[phase];
	u64 status;
	unsigned int i;
	int ret = error;

	status = readq(o->idma->reg + IDMA_OBS_CONTROL_OFFSET);
	if (status & 3) {
		writeq(2, o->idma->reg + IDMA_OBS_CONTROL_OFFSET);
		status = readq(o->idma->reg + IDMA_OBS_CONTROL_OFFSET);
	}
	o->run->observer_status[phase] = status;
	for (i = 0; i < 35; i++)
		v[i] = idma_observer_indexed(obs, i);
	for (i = 0; i < IDMA_COMPOSITION_WORDS; i++)
		c[i] = idma_composition_read(o->idma, i);
	writeq(0, o->idma->reg + IDMA_OBS_INDEX_OFFSET);
	if (ret)
		return ret;
	/* Word9 is the final BATCH_READ poll, not a count of software batches. */
	if (status != 4 || v[7] != 432 || v[8] != 432 || v[9] != 1 || v[2] ||
	    v[0] != v[1] + v[2] + v[3] ||
	    v[0] != v[10] + v[11] + v[12] + v[13] || v[32] || v[33])
		return -EIO;
	for (i = 0; i < 2; i++)
		if ((v[30 + i] >> 16) + v[14 + 6 * i] !=
		    (v[32 + i] >> 16) + v[16 + 6 * i] ||
		    (v[30 + i] & 0xffff) + v[15 + 6 * i] !=
		    (v[32 + i] & 0xffff) + v[17 + 6 * i])
			return -EIO;
	if (c[0] != IDMA_COMPOSITION_MAGIC || c[1] != 1 || c[2] != 0xa00 ||
	    c[3] || c[4] || c[5] != 1 || c[6] != 4 || !c[7] || c[9] ||
	    c[10] != 0xa00 || c[11] || c[34] != 3 || c[35])
		return -EIO;
	for (i = 12; i <= 18; i += 3)
		if (c[i] != c[i + 1] + c[i + 2])
			return -EIO;
	for (i = 40; i < 64; i++)
		if (c[i])
			return -EPROTO;
	return 0;
}

static int idma_finite_batch(struct idma_finite_owner *o, unsigned int phase,
	unsigned int worker, unsigned int layer, unsigned int component,
	size_t src_offset, size_t dst_offset, size_t bytes)
{
	struct idma_device *d = o->idma;
	unsigned int src_pool = 2 * worker + (phase == 0), dst_pool = 2 * worker + (phase == 1);
	size_t src_within, dst_within, count;
	struct iommu_finite_allocation *src, *dst;
	struct idma_finite_batch *b;
	u64 control, spins = 0;
	int ret = 0;

	while (bytes) {
		if (o->run->batch_count == IDMA_FINITE_MAX_BATCHES)
			return -E2BIG;
		src = idma_finite_extent(o, src_pool, src_offset, &src_within);
		dst = idma_finite_extent(o, dst_pool, dst_offset, &dst_within);
		count = min3(bytes, src->size - src_within, dst->size - dst_within);
		if (!count || !IS_ALIGNED(count, PAGE_SIZE))
			return -EINVAL;
		b = &o->batches[o->run->batch_count++];
		b->phase = phase; b->worker = worker; b->layer = layer; b->component = component;
		b->source_offset = src_offset; b->destination_offset = dst_offset; b->bytes = count;
		b->begin = idma_finite_now();
		if (!idma_finite_quiet(d)) {
			ret = -EBUSY;
			goto record;
		}
		writeq(0, d->reg + IDMA_BATCH_CONTROL_OFFSET);
		writeq(count / PAGE_SIZE, d->reg + IDMA_BATCH_TARGET_OFFSET);
		writeq(count / PAGE_SIZE, d->reg + IDMA_BATCH_PAGES_OFFSET);
		writeq(0, d->reg + IDMA_BATCH_GAP_OFFSET);
		writeq(1, d->reg + IDMA_BATCH_CONTROL_OFFSET);
		if (readq(d->reg + IDMA_BATCH_CONTROL_OFFSET) != 1) {
			ret = -EIO;
			goto record;
		}
		writeq(src->handle + src_within, d->reg + IDMA_SRC_ADDR_REG_OFFSET);
		writeq(dst->handle + dst_within, d->reg + IDMA_DST_ADDR_REG_OFFSET);
		writeq(PAGE_SIZE, d->reg + IDMA_NUM_BYTES_REG_OFFSET);
		writeq(0, d->reg + IDMA_CONF_REG_OFFSET);
		/* Reading NEXT is the side-effecting trigger. Invalid returned ID
		 * leaves acceptance uncertain; never match a previous ID or free.
		 */
		o->triggered = true;
		b->first_id = readq(d->reg + IDMA_NEXT_ID_REG_OFFSET);
		o->active_id = b->first_id;
		if (!b->first_id) {
			o->unsafe = true;
			ret = -EIO;
			goto stop;
		}
		for (;;) {
			b->observed_done = readq(d->reg + IDMA_DONE_REG_OFFSET);
			control = readq(d->reg + IDMA_BATCH_CONTROL_OFFSET);
			if (b->observed_done == b->first_id && !(control & 2) &&
			    idma_finite_quiet(d))
				break;
			if (ktime_get_ns() >= o->deadline) {
				ret = -ETIMEDOUT;
				goto stop;
			}
			if (!(++spins & 1023))
				cond_resched();
			cpu_relax();
		}
		o->triggered = false;
		b->issued = readq(d->reg + IDMA_BATCH_ISSUED_OFFSET);
		b->completed = readq(d->reg + IDMA_BATCH_COMPLETED_OFFSET);
		b->quiet = 1;
		if ((control >> 32) || b->issued != count / PAGE_SIZE || b->completed != b->issued)
			ret = -EIO;
		writeq(0, d->reg + IDMA_BATCH_CONTROL_OFFSET);
		goto record;
stop:
		/* STOP only stops future tasks. It does not cancel accepted AXI.
		 * One bounded drain may prove the current whole-batch ID retired.
		 */
		writeq(2, d->reg + IDMA_BATCH_CONTROL_OFFSET);
		if (b->first_id && !o->unsafe) {
			u64 drain_until = ktime_get_ns() + 2 * NSEC_PER_SEC;

			while (ktime_get_ns() < drain_until) {
				b->observed_done = readq(d->reg + IDMA_DONE_REG_OFFSET);
				if (b->observed_done == b->first_id && idma_finite_quiet(d)) {
					o->triggered = false;
					b->quiet = 1;
					break;
				}
				cond_resched();
			}
		}
		b->issued = readq(d->reg + IDMA_BATCH_ISSUED_OFFSET);
		b->completed = readq(d->reg + IDMA_BATCH_COMPLETED_OFFSET);
		if (o->triggered)
			o->unsafe = true;
		else
			writeq(0, d->reg + IDMA_BATCH_CONTROL_OFFSET);
record:
		b->control = readq(d->reg + IDMA_BATCH_CONTROL_OFFSET);
		b->status = ret;
		b->end = idma_finite_now();
		if (ret)
			return ret;
		if (phase == 0) {
			o->run->swapout_bytes += count;
			o->run->swapout_tasks += b->completed;
		} else {
			o->run->restore_bytes += count;
			o->run->restore_tasks += b->completed;
		}
		src_offset += count; dst_offset += count; bytes -= count;
	}
	if (phase == 0)
		o->run->swapout_logical_runs++;
	else
		o->run->restore_logical_runs++;
	return 0;
}

static int idma_finite_phase(struct idma_finite_owner *o, unsigned int phase)
{
	struct idma_observation obs = { .batch = true, .p4m = true };
	unsigned int layer, worker, component;
	int ret;

	ret = idma_observation_prepare(&obs, true);
	if (ret)
		goto done;
	idma_finite_hpm(o, o->run->hpm_before[phase]);
	ret = idma_observation_arm(&obs, o->idma, NULL, 432);
	if (ret)
		goto capture;
	for (layer = 0; layer < 12 && !ret; layer++)
		for (worker = 0; worker < 2 && !ret; worker++)
			for (component = 0; component < 2 && !ret; component++) {
				size_t base = layer * FIN_LAYER + component * FIN_COMPONENT;

				if (!phase) {
					ret = idma_finite_batch(o, phase, worker, layer, component,
						base + 5 * FIN_SLOT, base + 5 * FIN_SLOT, 3 * FIN_SLOT);
				} else {
					ret = idma_finite_batch(o, phase, worker, layer, component,
						base + 5 * FIN_SLOT, base + 6 * FIN_SLOT, 2 * FIN_SLOT);
					if (!ret)
						ret = idma_finite_batch(o, phase, worker, layer, component,
							base + 7 * FIN_SLOT, base + 5 * FIN_SLOT, FIN_SLOT);
				}
			}
capture:
	idma_finite_hpm(o, o->run->hpm_after[phase]);
	if (obs.armed)
		ret = idma_finite_capture(o, &obs, phase, ret);
done:
	if (obs.control)
		idma_put_instance(obs.control);
	return ret;
}

static u8 idma_finite_byte(struct idma_finite_owner *o, unsigned int pool, size_t off)
{
	size_t within;
	struct iommu_finite_allocation *a = idma_finite_extent(o, pool, off, &within);

	return READ_ONCE(((u8 *)a->cpu_addr)[within]);
}

static int idma_finite_verify(struct idma_finite_owner *o, bool restored)
{
	unsigned int worker, layer, component, pool, i;
	size_t off;
	struct idma_finite_run *r = o->run;

	for (layer = 0; layer < 12; layer++) {
		for (worker = 0; worker < 2; worker++) {
			for (component = 0; component < 2; component++) {
				size_t base = layer * FIN_LAYER + component * FIN_COMPONENT;

				for (off = 0; off < FIN_COMPONENT; off++) {
					size_t seed_off = base + off;
					u8 expected_cpu = o->seed[worker][seed_off];
					u8 expected_gpu;

					if (restored && off / FIN_SLOT >= 5) {
						unsigned int slot = off / FIN_SLOT;
						unsigned int src = slot == 5 ? 7 : slot - 1;

						seed_off = base + src * FIN_SLOT + off % FIN_SLOT;
					}
					expected_gpu = o->seed[worker][seed_off];
					if (idma_finite_byte(o, 2 * worker, base + off) != expected_cpu ||
					    idma_finite_byte(o, 2 * worker + 1, base + off) != expected_gpu) {
						r->data_errors++;
						return -EILSEQ;
					}
				}
			}
			if (restored) {
				struct idma_finite_layer *l = &r->layers[r->worker_layers++];

				l->worker = worker; l->layer = layer;
				l->ready = idma_finite_now();
			}
		}
		if (restored)
			r->joint_layers++;
		cond_resched();
	}
	/* Every reserved byte beyond logical tensor geometry remains untouched. */
	for (pool = 0; pool < 4; pool++)
		for (i = 0; i < o->per_pool; i++) {
			struct iommu_finite_allocation *a = &o->allocations[pool * o->per_pool + i];
			size_t start = min_t(size_t, a->size,
				i * a->size < IDMA_FINITE_POOL_BYTES ? IDMA_FINITE_POOL_BYTES - i * a->size : 0);

			for (off = start; off < a->size; off++)
				if (READ_ONCE(((u8 *)a->cpu_addr)[off]) != FIN_GUARD) {
					r->data_errors++;
					return -EILSEQ;
				}
		}
	if (restored) {
		r->source_bytes_verified = 2ULL * IDMA_FINITE_POOL_BYTES;
		r->destination_bytes_verified = 2ULL * IDMA_FINITE_POOL_BYTES;
		/* Coverage overlaps source/destination checks; not extra unique bytes. */
		r->guard_bytes_verified = 13238272ULL;
	}
	return 0;
}

static long idma_finite_caps_ioctl(struct idma_device *d, void __user *arg)
{
	struct idma_finite_caps input, caps = {};
	int ret;

	if (!capable(CAP_SYS_RAWIO))
		return -EPERM;
	if (copy_from_user(&input, arg, sizeof(input)))
		return -EFAULT;
	if (input.abi_version != IDMA_FINITE_ABI || input.struct_size != sizeof(input))
		return -EINVAL;
	ret = mutex_lock_interruptible(&idma_experiment_lock);
	if (ret)
		return ret;
	caps.abi_version = IDMA_FINITE_ABI; caps.struct_size = sizeof(caps);
	caps.magic = IDMA_FINITE_MAGIC;
	caps.device_id = d->id; caps.did = d->did;
	caps.quarantined = READ_ONCE(d->quarantined);
	caps.online_cpus = num_online_cpus();
	caps.observer_magic = readq(d->reg + IDMA_OBS_MAGIC_OFFSET);
	caps.batch_magic = readq(d->reg + IDMA_BATCH_MAGIC_OFFSET);
	if (!d->id && caps.observer_magic == IDMA_OBS_MAGIC_V9) {
		caps.composition_magic = idma_composition_read(d, 0);
		caps.composition_abi = idma_composition_read(d, 1);
		caps.composition_config = idma_composition_read(d, 2);
		caps.composition_idle = idma_composition_read(d, 5);
		writeq(0, d->reg + IDMA_OBS_INDEX_OFFSET);
	}
	caps.sample = idma_finite_now();
	mutex_unlock(&idma_experiment_lock);
	return copy_to_user(arg, &caps, sizeof(caps)) ? -EFAULT : 0;
}

static long idma_run_finite(struct idma_device *d, void __user *arg)
{
	struct idma_finite_run *r;
	struct idma_finite_stamp entry = idma_finite_now();
	struct idma_finite_owner *o = NULL;
	unsigned int i, worker;
	int ret = 0, release_ret = 0;
	bool locked = false;

	if (!capable(CAP_SYS_RAWIO))
		return -EPERM;
	r = memdup_user(arg, sizeof(*r));
	if (IS_ERR(r))
		return PTR_ERR(r);
	memset(&r->status, 0, sizeof(*r) - offsetof(struct idma_finite_run, status));
	r->release_status = -EINPROGRESS;
	idma_finite_begin(r, 11);
	r->stages[11].begin = entry;
	idma_finite_begin(r, 0);
	if (r->abi_version != IDMA_FINITE_ABI || r->struct_size != sizeof(*r) ||
	    r->flags || r->reserved[0] || r->reserved[1] ||
	    !r->timeout_ms || r->timeout_ms > IDMA_MAX_RUN_TIMEOUT_MS ||
	    r->batch_capacity != IDMA_FINITE_MAX_BATCHES ||
	    r->leaf_capacity != IDMA_FINITE_MAX_LEAVES || !r->batches_user || !r->leaves_user) {
		ret = -EINVAL;
		goto input_end;
	}
	for (i = 0; i < 4; i++)
		if (!r->pool_user[i]) {
			ret = -EINVAL;
			goto input_end;
		}
	for (i = 0; i < 2; i++)
		if (!r->gpu_seed_user[i]) {
			ret = -EINVAL;
			goto input_end;
		}
	o = kzalloc(sizeof(*o), GFP_KERNEL);
	if (!o) {
		ret = -ENOMEM;
		goto input_end;
	}
	o->run = r; o->idma = d; o->dev = d->miscdev.parent;
	get_device(o->dev);
	o->batches = kvcalloc(IDMA_FINITE_MAX_BATCHES, sizeof(*o->batches), GFP_KERNEL);
	o->leaves = kvcalloc(IDMA_FINITE_MAX_LEAVES, sizeof(*o->leaves), GFP_KERNEL);
	if (!o->batches || !o->leaves) {
		ret = -ENOMEM;
		goto input_end;
	}
	for (worker = 0; worker < 2; worker++) {
		o->seed[worker] = kvmalloc(IDMA_FINITE_POOL_BYTES, GFP_KERNEL);
		if (!o->seed[worker]) {
			ret = -ENOMEM;
			goto input_end;
		}
		if (copy_from_user(o->seed[worker], u64_to_user_ptr(r->gpu_seed_user[worker]),
				   IDMA_FINITE_POOL_BYTES)) {
			ret = -EFAULT;
			goto input_end;
		}
		r->reference_bytes += IDMA_FINITE_POOL_BYTES;
		/* Inactive logical slots are literal guards in both frozen inputs. */
		for (i = 0; i < 24; i++)
			if (memchr_inv(o->seed[worker] + i * FIN_COMPONENT,
				       FIN_GUARD, 5 * FIN_SLOT)) {
				ret = -EILSEQ;
				goto input_end;
			}
	}
input_end:
	idma_finite_end(r, 0, ret);
	if (ret)
		goto cleanup;
	ret = mutex_lock_interruptible(&idma_experiment_lock);
	if (ret)
		goto cleanup;
	locked = true;
	if (d->id || d->did != 10 || READ_ONCE(d->quarantined) || idma_finite_retained ||
	    num_online_cpus() != 1 || readq(d->reg + IDMA_OBS_MAGIC_OFFSET) != IDMA_OBS_MAGIC_V9 ||
	    readq(d->reg + IDMA_BATCH_MAGIC_OFFSET) != IDMA_BATCH_MAGIC ||
	    idma_composition_read(d, 0) != IDMA_COMPOSITION_MAGIC ||
	    idma_composition_read(d, 1) != 1 || idma_composition_read(d, 2) != 0xa00 ||
	    idma_composition_read(d, 3) || idma_composition_read(d, 4) || !idma_finite_quiet(d)) {
		ret = -EOPNOTSUPP;
		goto cleanup;
	}
	idma_finite_begin(r, 1);
	ret = riscv_iommu_hpm_acquire(o->dev, NULL, &o->hpm);
	if (!ret)
		ret = idma_finite_allocate(o);
	if (!ret) {
		for (i = 0; i < o->nr; i++)
			memset(o->allocations[i].cpu_addr, FIN_GUARD, o->allocations[i].size);
		for (worker = 0; worker < 2; worker++)
			idma_finite_pool_copy(o, 2 * worker + 1, o->seed[worker], true, false);
	}
	idma_finite_end(r, 1, ret);
	if (ret)
		goto cleanup;
	idma_finite_begin(r, 2);
	ret = idma_finite_audit(o);
	if (!ret) {
		ret = idma_finite_barrier(o);
		if (ret)
			o->unsafe = true;
	}
	idma_finite_end(r, 2, ret);
	if (ret)
		goto cleanup;
	/* Fault guard starts at first phase; no application deadline is asserted. */
	o->deadline = ktime_get_ns() + (u64)r->timeout_ms * NSEC_PER_MSEC;
	idma_finite_begin(r, 3);
	ret = idma_finite_phase(o, 0);
	idma_finite_end(r, 3, ret);
	if (ret)
		goto cleanup;
	idma_finite_begin(r, 4);
	local_flush_icache_all();
	ret = idma_finite_verify(o, false);
	idma_finite_end(r, 4, ret);
	if (ret)
		goto cleanup;
	idma_finite_begin(r, 5);
	/* Preserve all pool maps and IOTLB residency. No between-phase remap or
	 * translation flush: only the CPU-to-device cache ownership handoff.
	 */
	local_flush_icache_all();
	idma_finite_end(r, 5, 0);
	idma_finite_begin(r, 6);
	ret = idma_finite_phase(o, 1);
	idma_finite_end(r, 6, ret);
	if (ret)
		goto cleanup;
	idma_finite_begin(r, 7);
	local_flush_icache_all();
	ret = idma_finite_verify(o, true);
	idma_finite_end(r, 7, ret);
	if (ret)
		goto cleanup;
cleanup:
	/* Records are copied even after a technical failure; unsafe DMA buffers
	 * are never read by the CPU. Final run struct follows checked teardown.
	 */
	if (o && o->batches && o->leaves) {
		int copy_ret = 0;

		idma_finite_begin(r, 8);
		if (o->nr && !o->unsafe && !o->triggered && locked && idma_finite_quiet(d)) {
			local_flush_icache_all();
			for (i = 0; i < 4 && !copy_ret; i++)
				copy_ret = idma_finite_pool_copy(o, i, u64_to_user_ptr(r->pool_user[i]), false, true);
		}
		if (copy_to_user(u64_to_user_ptr(r->batches_user), o->batches,
				 r->batch_count * sizeof(*o->batches)) ||
		    copy_to_user(u64_to_user_ptr(r->leaves_user), o->leaves,
				 r->leaf_count * sizeof(*o->leaves)))
			copy_ret = -EFAULT;
		idma_finite_end(r, 8, copy_ret);
		if (!ret)
			ret = copy_ret;
	}
	idma_finite_begin(r, 9);
	if (o && o->nr)
		release_ret = idma_finite_release(o);
	r->release_status = release_ret;
	idma_finite_end(r, 9, release_ret);
	if (release_ret) {
		o->unsafe = true;
		if (!ret)
			ret = release_ret;
	}
	idma_finite_begin(r, 10);
	if (o) {
		if (o->hpm)
			riscv_iommu_hpm_release(o->hpm);
		o->hpm = NULL;
		if (o->unsafe || o->triggered) {
			r->quarantined = 1;
			WRITE_ONCE(d->quarantined, true);
			idma_finite_retained = o;
			dev_crit(o->dev, "finite ordinary proof failed; private resources quarantined until reboot\n");
		} else {
			kvfree(o->seed[0]); kvfree(o->seed[1]);
			kvfree(o->batches); kvfree(o->leaves);
			put_device(o->dev);
			kfree(o);
		}
	}
	if (locked) {
		writeq(0, d->reg + IDMA_OBS_INDEX_OFFSET);
		mutex_unlock(&idma_experiment_lock);
	}
	idma_finite_end(r, 10, 0);
	r->status = ret;
	r->stage = 11;
	idma_finite_end(r, 11, ret);
	if (copy_to_user(arg, r, sizeof(*r)))
		ret = -EFAULT;
	/* Quarantine record keeps its own immutable final status receipt. */
	if (!r->quarantined)
		kfree(r);
	return ret;
}
