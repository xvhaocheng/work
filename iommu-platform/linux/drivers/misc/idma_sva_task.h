/* SPDX-License-Identifier: GPL-2.0 */
/* Included by idma.c: one synchronous, privileged, fixed research task. */
#include <linux/cpu.h>
#include <linux/delay.h>
#include <linux/highmem.h>
#include <linux/overflow.h>
#include <linux/pgtable.h>
#include <linux/riscv_iommu_research.h>
#include <linux/sched/mm.h>
#include <uapi/linux/idma_sva.h>
#include <asm/csr.h>

static bool idma_sva_enable;
module_param_named(sva_enable, idma_sva_enable, bool, 0600);
MODULE_PARM_DESC(sva_enable,
	"Opt in only after single-hart CVA6 WT/cache, default DMA profile and all-source quiescence acceptance");

struct idma_sva_descriptor {
	u32 src, dst, bytes, lane;
};

static const struct idma_sva_descriptor idma_sva_descriptors[] = {
	{ 32, 16396, 60, 0 },
	{ 4128, 20492, 65, 1 },
	{ 8224, 20557, 503, 1 },
	{ 12320, 24588, 1011, 0 },
};

struct idma_sva_session {
	struct idma_device *lane[2];
	struct device *dev[2];
	struct iommu_domain *saved[2], *domain;
	struct mm_struct *mm;
	struct page *pages[IDMA_SVA_ARENA_PAGES];
	unsigned int pinned, mapped;
	bool claimed[2], attached[2], binding_changed;
	unsigned long base, device_base;
	struct riscv_iommu_research_certificate certificate;
};

/* This is a platform property, not the architectural FENCE.I DMA contract. */
static void idma_sva_cache_sync(void *unused)
{
	mb();
	local_flush_icache_all();
	mb();
}

/* No HW abort/PR-PW drain exists. Returning would permit mm teardown/UAF. */
static void __noreturn idma_sva_retain(struct idma_sva_session *s,
				     const char *reason)
{
	WRITE_ONCE(s->lane[0]->quarantined, true);
	WRITE_ONCE(s->lane[1]->quarantined, true);
	dev_crit(s->dev[0],
		 "SVA task unsafe to release (%s); retaining pins/mm/mmap lock/device ownership until platform recovery\n",
		 reason);
	for (;;)
		schedule_timeout_uninterruptible(HZ);
}

static bool idma_sva_profile(struct idma_device *d, struct idma_device *peer)
{
	return READ_ONCE(idma_sva_enable) && PAGE_SIZE == 4096 &&
		num_online_cpus() == 1 &&
		of_machine_is_compatible("eth,ariane-bare-dev") &&
		satp_mode == SATP_MODE_39 && d->id == 0 && d->did == 10 &&
		peer && peer->did == 11 &&
		riscv_iommu_research_supported(d->miscdev.parent,
					       peer->miscdev.parent);
}

/* FOLL_PIN and mmap_read_lock do not stop CPU PTE aging. The current walker
 * faults on A/D=0. VM_LOCKED base pages avoid the classic rmap aging path;
 * refuse alternative walkers that can clear A behind this restricted task. */
static bool idma_sva_borrowed_profile(void)
{
#if defined(CONFIG_LRU_GEN) || defined(CONFIG_IDLE_PAGE_TRACKING) || \
    defined(CONFIG_TRANSPARENT_HUGEPAGE) || defined(CONFIG_KSM) || \
    defined(CONFIG_NUMA_BALANCING) || defined(CONFIG_DAMON)
	return false;
#else
	return true;
#endif
}

static long idma_sva_caps_ioctl(struct idma_device *d, void __user *uarg)
{
	struct idma_sva_caps c;
	struct idma_device *peer;
	u32 index;

	if (copy_from_user(&c, uarg, sizeof(c)))
		return -EFAULT;
	index = c.peer_index;
	if (c.abi_version != IDMA_SVA_ABI || c.struct_size != sizeof(c))
		return -EINVAL;
	c.abi_version = c.struct_size = c.peer_index = 0;
	if (memchr_inv(&c, 0, sizeof(c)))
		return -EINVAL;
	peer = idma_get_instance(index);
	memset(&c, 0, sizeof(c));
	c.abi_version = IDMA_SVA_ABI;
	c.struct_size = sizeof(c);
	c.peer_index = index;
	c.primary_index = d->id;
	c.primary_did = d->did;
	c.peer_did = peer ? peer->did : U32_MAX;
	c.capabilities = IDMA_SVA_CAP_SYNCHRONOUS | IDMA_SVA_CAP_FIXED_TASK;
	cpus_read_lock();
	if (idma_sva_profile(d, peer)) {
		c.address_modes = IDMA_SVA_MODE_BIT(IDMA_SVA_MODE_IOVA) |
			IDMA_SVA_MODE_BIT(IDMA_SVA_MODE_SHADOW);
		if (idma_sva_borrowed_profile())
			c.address_modes |= IDMA_SVA_MODE_BIT(IDMA_SVA_MODE_BORROWED_MM);
		c.capabilities |= IDMA_SVA_CAP_SINGLE_HART_FENCE_I |
			IDMA_SVA_CAP_QUIESCENT_CONTEXT;
	}
	cpus_read_unlock();
	c.page_bytes = PAGE_SIZE;
	c.arena_pages = IDMA_SVA_ARENA_PAGES;
	c.data_pages = IDMA_SVA_DATA_PAGES;
	c.descriptors = IDMA_SVA_DESCRIPTORS;
	c.packets = IDMA_SVA_PACKETS;
	c.max_timeout_ms = IDMA_MAX_RUN_TIMEOUT_MS;
	idma_put_instance(peer);
	return copy_to_user(uarg, &c, sizeof(c)) ? -EFAULT : 0;
}

/* mmap_read_lock is held through all DMA, restoration and unpin. */
static int idma_sva_pin(struct idma_sva_session *s, bool borrowed_mm)
{
	struct vm_area_struct *vma;
	unsigned long addr;
	long n;
	unsigned int i;

	vma = find_vma(s->mm, s->base);
	if (!vma || vma->vm_start > s->base ||
	    vma->vm_end < s->base + IDMA_SVA_ARENA_BYTES ||
	    !vma_is_anonymous(vma) ||
	    (vma->vm_flags & (VM_SHARED | VM_SPECIAL | VM_HUGETLB)) ||
	    (vma->vm_flags & (VM_READ | VM_WRITE)) != (VM_READ | VM_WRITE))
		return -EINVAL;
	/* Userspace must mlock the SAME arena before RUN, retaining it until the
	 * safe return. No extra buffer, PTE mutation or background fault repair. */
	if (borrowed_mm && (!idma_sva_borrowed_profile() ||
			    !(vma->vm_flags & VM_LOCKED)))
		return -EOPNOTSUPP;
	n = pin_user_pages(s->base, IDMA_SVA_ARENA_PAGES,
			   FOLL_WRITE | FOLL_LONGTERM, s->pages);
	if (n > 0)
		s->pinned = n;
	if (n != IDMA_SVA_ARENA_PAGES)
		return n < 0 ? n : -EFAULT;
	for (i = 0; i < s->pinned; ++i) {
		unsigned int j;

		for (j = 0; j < i; ++j)
			if (s->pages[i] == s->pages[j])
				return -EINVAL;
	}
	/* Ordinary independent I/O page tables do not depend on CPU PTE A/D,
	 * permission encoding or a CPU 4K leaf. Do not weaken those controls
	 * by imposing the borrowed-root-only checks on them. */
	if (!borrowed_mm)
		return 0;
	/* Reject huge/non-plain/changed PTEs, rather than editing CPU A/D bits. */
	for (i = 0, addr = s->base; i < s->pinned; ++i, addr += PAGE_SIZE) {
		pgd_t *pgd = pgd_offset(s->mm, addr);
		p4d_t *p4d;
		pud_t *pud;
		pmd_t *pmd;
		pte_t *ptep, pte;
		spinlock_t *ptl;
		u64 v;

		if (pgd_none(*pgd) || pgd_bad(*pgd) ||
		    ((u64)pgd_val(*pgd) >> 54) ||
		    (pgd_val(*pgd) & (_PAGE_USER | _PAGE_ACCESSED | _PAGE_DIRTY)))
			return -EOPNOTSUPP;
		p4d = p4d_offset(pgd, addr);
		if (p4d_none(*p4d) || p4d_bad(*p4d))
			return -EOPNOTSUPP;
		pud = pud_offset(p4d, addr);
		if (pud_none(*pud) || pud_bad(*pud) || pud_leaf(*pud))
			return -EOPNOTSUPP;
		pmd = pmd_offset(pud, addr);
		if (pmd_none(*pmd) || pmd_bad(*pmd) || pmd_leaf(*pmd) ||
		    ((u64)pmd_val(*pmd) >> 54) ||
		    (pmd_val(*pmd) & (_PAGE_USER | _PAGE_ACCESSED | _PAGE_DIRTY)))
			return -EOPNOTSUPP;
		ptep = pte_offset_map_lock(s->mm, pmd, addr, &ptl);
		if (!ptep)
			return -EFAULT;
		pte = ptep_get(ptep);
		v = pte_val(pte);
		n = pte_present(pte) && pte_user(pte) && pte_write(pte) &&
			pte_young(pte) && pte_dirty(pte) &&
			(v & _PAGE_READ) && !(v >> 54) &&
			pte_pfn(pte) == page_to_pfn(s->pages[i]);
		pte_unmap_unlock(ptep, ptl);
		if (!n)
			return -EOPNOTSUPP;
	}
	return 0;
}

static int idma_sva_barrier(struct idma_sva_session *s)
{
	return riscv_iommu_research_barrier(s->dev[0], s->dev[1],
		idma_sva_cache_sync, NULL, &s->certificate);
}

static int idma_sva_preflight(struct idma_sva_session *s)
{
	struct idma_device *d;
	u64 status;
	unsigned int i;

	if (idma_any_quarantined())
		return -EBUSY;
	/* Public launcher exposes COUNT/HOLD/WAIT for the selected two lanes. */
	if (readq(s->lane[0]->reg + IDMA_LAUNCHER_MAGIC_OFFSET) != IDMA_LAUNCHER_MAGIC)
		return -EOPNOTSUPP;
	status = readq(s->lane[0]->reg + IDMA_LAUNCHER_CONTROL_OFFSET);
	if (status & (IDMA_LAUNCHER_STATUS_PREP | IDMA_LAUNCHER_STATUS_RUNNING |
		      IDMA_LAUNCHER_HELD_MASK | IDMA_LAUNCHER_ACTIVE_MASK |
		      IDMA_LAUNCHER_ERROR_MASK))
		return -EBUSY;
	/* Existing APIs are under idma_experiment_lock. No external /dev/mem writer
	 * or untracked manager is permitted by the explicit research profile. */
	for (i = 0; i < 4; i++) {
		d = idma_get_instance(i);
		if (!d)
			return -ENODEV;
		status = readq(d->reg + IDMA_STATUS_REG_OFFSET) |
			(readq(d->reg + IDMA_BATCH_CONTROL_OFFSET) & 3);
		idma_put_instance(d);
		if (status)
			return -EBUSY;
	}
	return 0;
}

static int idma_sva_execute(struct idma_sva_session *s,
			    struct idma_sva_run *r)
{
	unsigned int j;
	int ret = 0;

	for (j = 0; j < IDMA_SVA_DESCRIPTORS; ++j) {
		const struct idma_sva_descriptor *d = &idma_sva_descriptors[j];
		void __iomem *reg = s->lane[d->lane]->reg;
		u64 id, done, deadline;
		bool late = false;

		/* No early live-register overwrite, HOLD, extra permit or pending. */
		writeq(0, reg + IDMA_BATCH_CONTROL_OFFSET);
		writeq(0, reg + IDMA_CONF_REG_OFFSET);
		writeq(s->device_base + d->src, reg + IDMA_SRC_ADDR_REG_OFFSET);
		writeq(s->device_base + d->dst, reg + IDMA_DST_ADDR_REG_OFFSET);
		writeq(d->bytes, reg + IDMA_NUM_BYTES_REG_OFFSET);
		wmb();
		id = readq(reg + IDMA_NEXT_ID_REG_OFFSET);
		/* A zero/uncertain receipt is NOT proof that NEXT did not publish. */
		if (!id)
			idma_sva_retain(s, "uncertain NEXT receipt");
		r->transfer_id[j] = id;
		r->submitted++;
		r->device_src[j] = s->device_base + d->src;
		r->device_dst[j] = s->device_base + d->dst;
		deadline = ktime_get_ns() + (u64)r->timeout_ms * NSEC_PER_MSEC;
		for (;;) {
			done = readq(reg + IDMA_DONE_REG_OFFSET);
			if (done == id)
				break;
			if (!late && ktime_get_ns() >= deadline) {
				late = true;
				ret = -ETIMEDOUT;
				dev_err(s->dev[d->lane],
					"SVA deadline passed; waiting uninterruptibly for real DONE before release\n");
			}
			usleep_range(50, 100);
		}
		r->completed++;
		if (ret)
			break; /* Drained this job; no new publication after timeout. */
	}
	idma_sva_cache_sync(NULL);
	if (!ret) {
		/* The CPU comparison does not require another payload buffer. */
		for (j = 0; j < IDMA_SVA_DESCRIPTORS; ++j) {
			const struct idma_sva_descriptor *d = &idma_sva_descriptors[j];
			void *src = kmap_local_page(s->pages[d->src / PAGE_SIZE]);
			void *dst = kmap_local_page(s->pages[d->dst / PAGE_SIZE]);

			if (memcmp(src + offset_in_page(d->src),
				   dst + offset_in_page(d->dst), d->bytes))
				ret = -EILSEQ;
			kunmap_local(dst);
			kunmap_local(src);
		}
	}
	return ret;
}

static long idma_sva_run_ioctl(struct idma_device *primary, void __user *uarg)
{
	struct idma_sva_run r;
	struct idma_sva_session *s;
	unsigned long end;
	int ret, cleanup = 0, i;
	bool mmap_locked = false;

	if (copy_from_user(&r, uarg, sizeof(r)))
		return -EFAULT;
	memset(&r.status, 0, sizeof(r) - offsetof(struct idma_sva_run, status));
	ret = -EINVAL;
	if (r.abi_version != IDMA_SVA_ABI || r.struct_size != sizeof(r) ||
	    r.flags || memchr_inv(r.reserved0, 0, sizeof(r.reserved0)) ||
	    memchr_inv(r.reserved, 0, sizeof(r.reserved)) ||
	    r.address_mode < IDMA_SVA_MODE_IOVA ||
	    r.address_mode > IDMA_SVA_MODE_BORROWED_MM ||
	    r.arena_bytes != IDMA_SVA_ARENA_BYTES ||
	    !IS_ALIGNED(r.arena_user, PAGE_SIZE) ||
	    check_add_overflow((unsigned long)r.arena_user,
			       (unsigned long)r.arena_bytes, &end) ||
	    end > TASK_SIZE || !r.timeout_ms || r.timeout_ms > IDMA_MAX_RUN_TIMEOUT_MS)
		goto copy_result;
	ret = -EPERM;
	if (!capable(CAP_SYS_RAWIO) || !current->mm ||
	    get_nr_threads(current) != 1 ||
	    atomic_read(&current->mm->mm_users) != 1)
		goto copy_result;
	s = kzalloc(sizeof(*s), GFP_KERNEL);
	if (!s) {
		ret = -ENOMEM;
		goto copy_result;
	}
	s->lane[0] = idma_get_instance(primary->id);
	s->lane[1] = idma_get_instance(r.peer_index);
	ret = -EOPNOTSUPP;
	if (!s->lane[0] || !s->lane[1])
		goto free_session;
	s->dev[0] = s->lane[0]->miscdev.parent;
	s->dev[1] = s->lane[1]->miscdev.parent;
	s->base = r.arena_user;
	s->device_base = r.address_mode == IDMA_SVA_MODE_IOVA ?
		0x10000000UL : s->base;
	s->mm = current->mm;
	mmget(s->mm);
	ret = mutex_lock_interruptible(&idma_experiment_lock);
	if (ret)
		goto put_mm;
	cpus_read_lock();
	if (!idma_sva_profile(primary, s->lane[1])) {
		ret = -EOPNOTSUPP;
		goto unlock_experiment;
	}
	ret = idma_sva_preflight(s);
	if (ret)
		goto unlock_experiment;
	mmap_read_lock(s->mm);
	mmap_locked = true;
	ret = idma_sva_pin(s, r.address_mode == IDMA_SVA_MODE_BORROWED_MM);
	if (ret)
		goto release_pins;
	r.pinned_pages = s->pinned;
	r.primary_did = primary->did;
	r.peer_did = s->lane[1]->did;
	r.domain_pt_pages = U32_MAX; /* Unknown includes no fabricated allocation fee. */
	s->domain = riscv_iommu_research_domain_alloc(s->dev[0],
		r.address_mode == IDMA_SVA_MODE_BORROWED_MM ? s->mm : NULL);
	if (IS_ERR(s->domain)) {
		ret = PTR_ERR(s->domain);
		s->domain = NULL;
		goto release_pins;
	}
	if (r.address_mode == IDMA_SVA_MODE_BORROWED_MM)
		r.domain_pt_pages = 0;
	else {
		for (i = 0; i < IDMA_SVA_DATA_PAGES; ++i) {
			ret = iommu_map(s->domain, s->device_base + i * PAGE_SIZE,
				page_to_phys(s->pages[i]), PAGE_SIZE,
				IOMMU_READ | IOMMU_WRITE, GFP_KERNEL);
			if (ret)
				goto restore;
			s->mapped++;
		}
		r.mapped_pages = s->mapped;
	}
	/* Preserve backing/default domains and claim both before attaching either. */
	for (i = 0; i < 2; ++i) {
		s->saved[i] = iommu_get_domain_for_dev(s->dev[i]);
		ret = iommu_device_claim_dma_owner(s->dev[i], s);
		if (ret)
			goto restore;
		s->claimed[i] = true;
		s->binding_changed = true;
	}
	for (i = 0; i < 2; ++i) {
		ret = iommu_attach_device(s->domain, s->dev[i]);
		if (ret)
			goto restore;
		s->attached[i] = true;
	}
	ret = idma_sva_barrier(s);
	if (ret)
		idma_sva_retain(s, "initial context barrier failed");
	idma_sva_cache_sync(NULL);
	ret = idma_sva_execute(s, &r);
restore:
	/* No CPU/page/context release until every published job actually retired. */
	for (i = 1; i >= 0; --i) {
		if (s->attached[i])
			iommu_detach_device(s->domain, s->dev[i]);
		if (s->claimed[i]) {
			iommu_device_release_dma_owner(s->dev[i]);
			if (iommu_get_domain_for_dev(s->dev[i]) != s->saved[i])
				idma_sva_retain(s, "default domain restore failed");
		}
	}
	if (s->binding_changed) {
		cleanup = idma_sva_barrier(s);
		if (cleanup)
			idma_sva_retain(s, "restored context barrier failed");
	}
	while (s->mapped) {
		--s->mapped;
		if (iommu_unmap(s->domain,
			s->device_base + s->mapped * PAGE_SIZE, PAGE_SIZE) != PAGE_SIZE)
			idma_sva_retain(s, "short unmap");
	}
	if (s->domain) {
		cleanup = riscv_iommu_research_domain_release(s->domain);
		if (cleanup)
			idma_sva_retain(s, "domain release not certified");
	}
	idma_sva_cache_sync(NULL);
	r.result_flags |= IDMA_SVA_RESULT_CONTEXT_RESTORED |
		IDMA_SVA_RESULT_CPU_OWNED;
release_pins:
	if (s->pinned)
		unpin_user_pages_dirty_lock(s->pages, s->pinned, true);
	r.result_flags |= IDMA_SVA_RESULT_PINS_RELEASED;
	if (mmap_locked)
		mmap_read_unlock(s->mm);
unlock_experiment:
	cpus_read_unlock();
	mutex_unlock(&idma_experiment_lock);
put_mm:
	mmput(s->mm);
free_session:
	idma_put_instance(s->lane[1]);
	idma_put_instance(s->lane[0]);
	kfree(s);
copy_result:
	r.status = ret;
	r.restore_status = cleanup;
	if (copy_to_user(uarg, &r, sizeof(r)))
		return -EFAULT;
	return ret;
}
