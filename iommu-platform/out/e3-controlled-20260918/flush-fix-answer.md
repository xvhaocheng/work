I could not run anything; everything below is a proposal for Codex to apply and verify. Full final contents for the same allowed files follow, with the review items applied:

- (1) CQT is read **and** compared against `cmdq.lui` under the same `cq_lock`, framed as producer consistency rather than "queue empty"; the end check still requires `CQEN|CQON` set, errors 0 and unchanged `cq_error_seq`.
- (2) second endpoint uses `mutex_lock_nested(..., SINGLE_DEPTH_NESTING)`; ascending-DID order is stable and re-validated (`dev`/`devid`/`iommu`) **under** the lock; no `domain->lock` is taken.
- (3) acquire snapshots `iommu_get_domain_for_dev(dev)` without rejecting NULL/other-consumer domains; the checked core compares the live core domain and `ep->domain` core address against the snapshot before dereferencing, requires `domain->iommu`, `ops == riscv_iommu_domain_ops`, ordinary `DMA`/`UNMANAGED` types, non-NULL `pgd_root` and Sv39.
- (4) `atomic64_set()` before queue/IRQ init; `atomic64_inc()` before the (relaxed) CQ clear with an explicit `wmb()`; `rmb()` after the final `CQCSR` read before the generation read. IRQ behavior otherwise unchanged.
- (5)/(6) test-side compile fixes and new host-model cases (barrier/atomic/spinlock stubs, non-`static` `riscv_iommu_hpm_endpoint`, real 4096-aligned `pgd` pages, `IOMMU_DOMAIN_*` extracted from `<linux/iommu.h>`, core-domain/ops stubs, nested-lock + lock-order checks, producer-after-unlock, end-CQ-disabled and post-IRQ-clear seq cases).
- (7) no API/signature change beyond the added struct/prototype; old flush/post untouched; the VMA-before-following-fence-fetch note is preserved verbatim in intent.

Unresolved/assumptions are listed at the end.

---

`linux/include/linux/riscv_iommu_hpm.h`

```c
/* SPDX-License-Identifier: GPL-2.0-only */
#ifndef _LINUX_RISCV_IOMMU_HPM_H
#define _LINUX_RISCV_IOMMU_HPM_H

#include <linux/errno.h>
#include <linux/kconfig.h>
#include <linux/string.h>
#include <linux/types.h>

struct device;
struct riscv_iommu_hpm_session;

struct riscv_iommu_hpm_sample {
	u64 cycles;
	u64 iotlb_miss[2];
	u64 s1_ptw[2];
	u64 s2_ptw[2];
};

/*
 * Sleepable API for the current platform's single-DID endpoints. The caller
 * keeps both devices bound, serializes their workloads, and calls acquire,
 * read, flush_s1 and release from the same task. Successful acquire owns
 * counters 0..5 until release; it does not own, stop or drain either DMA
 * device.
 *
 * supported checks the bound driver and live HPM capability. Acquire also
 * verifies the six selectors and exact DID filtering, and may still return
 * -EOPNOTSUPP. A busy IOMMU returns -EBUSY. A NULL aggressor selects Solo.
 *
 * Event counts start at acquire; cycles are never reset. Samples are read
 * sequentially, not atomically. Use differences around the desired window.
 * Release restores saved selectors, event counts and inhibit state. Restored
 * event counts exclude activity during the owned session; cycles retain it.
 *
 * flush_s1 issues a global host first-stage invalidation barrier for the
 * session's devices. It must be called by the task that acquired the session
 * while it still owns hpm_lock, after the caller has quiesced the relevant DMA
 * and serialized binding changes. It does not reset ATS/PR/PW or any other
 * internal IOMMU state and does not retry or reset on failure. A NULL session
 * returns -EINVAL; when no RISC-V IOMMU is reachable it returns -EOPNOTSUPP.
 */
#if IS_REACHABLE(CONFIG_RISCV_IOMMU)
bool riscv_iommu_hpm_supported(struct device *dev);
int riscv_iommu_hpm_acquire(struct device *victim, struct device *aggressor,
			   struct riscv_iommu_hpm_session **out);
int riscv_iommu_hpm_flush_s1(struct riscv_iommu_hpm_session *session);
void riscv_iommu_hpm_read(struct riscv_iommu_hpm_session *session,
			  struct riscv_iommu_hpm_sample *sample);
void riscv_iommu_hpm_release(struct riscv_iommu_hpm_session *session);
#else
static inline bool riscv_iommu_hpm_supported(struct device *dev)
{
	return false;
}

static inline int
riscv_iommu_hpm_acquire(struct device *victim, struct device *aggressor,
			struct riscv_iommu_hpm_session **out)
{
	if (out)
		*out = NULL;
	return -EOPNOTSUPP;
}

static inline int
riscv_iommu_hpm_flush_s1(struct riscv_iommu_hpm_session *session)
{
	return -EOPNOTSUPP;
}

static inline void
riscv_iommu_hpm_read(struct riscv_iommu_hpm_session *session,
		     struct riscv_iommu_hpm_sample *sample)
{
	memset(sample, 0, sizeof(*sample));
}

static inline void
riscv_iommu_hpm_release(struct riscv_iommu_hpm_session *session)
{
}
#endif

#endif /* _LINUX_RISCV_IOMMU_HPM_H */
```

---

`linux/drivers/iommu/riscv/iommu.h`

```c
/* SPDX-License-Identifier: GPL-2.0-only */
/*
 * Copyright © 2022-2023 Rivos Inc.
 * Copyright © 2023 FORTH-ICS/CARV
 *
 * RISC-V Ziommu - IOMMU Interface Specification.
 *
 * Authors
 *	Tomasz Jeznach <tjeznach@rivosinc.com>
 *	Nick Kossifidis <mick@ics.forth.gr>
 */

#ifndef _RISCV_IOMMU_H_
#define _RISCV_IOMMU_H_

#include <linux/types.h>
#include <linux/iova.h>
#include <linux/io.h>
#include <linux/idr.h>
#include <linux/mmu_notifier.h>
#include <linux/list.h>
#include <linux/iommu.h>
#include <linux/io-pgtable.h>
#include <linux/mmu_notifier.h>

#include "iommu-bits.h"

#define IOMMU_PAGE_SIZE_4K	BIT_ULL(12)
#define IOMMU_PAGE_SIZE_2M	BIT_ULL(21)
#define IOMMU_PAGE_SIZE_1G	BIT_ULL(30)
#define IOMMU_PAGE_SIZE_512G	BIT_ULL(39)

struct riscv_iommu_queue {
	dma_addr_t base_dma;
	void *base;
	size_t len;		/* single item length */
	u32 cnt;		/* items count */
	u32 lui;		/* last used index, consumer/producer share */
	unsigned qbr;		/* queue base register offset */
	unsigned qcr;		/* queue control and status register offset */
	int irq;		/* registered interrupt number */
	bool in_iomem;		/* indicates queue data are in I/O memory  */
};

enum riscv_queue_ids {
	RISCV_IOMMU_COMMAND_QUEUE	= 0,
	RISCV_IOMMU_FAULT_QUEUE		= 1,
	RISCV_IOMMU_PAGE_REQUEST_QUEUE	= 2
};

/* TODO: Readback icvec */
enum riscv_default_virqs {
	RISCV_IOMMU_INTR_CQ = 0,
	RISCV_IOMMU_INTR_FQ = 1,
	RISCV_IOMMU_INTR_PM = 2,
	RISCV_IOMMU_INTR_PQ = 3
};

struct riscv_iommu_device {
	struct iommu_device iommu;	/* iommu core interface */
	struct device *dev;		/* iommu hardware */

	/* hardware control register space */
	void __iomem *reg;
	resource_size_t reg_phys;

	/* IRQs for the various queues */
	int irq_cmdq;
	int irq_fltq;
	int irq_pm;
	int irq_priq;

	/* Queue lengths */
	int cmdq_len;
	int fltq_len;
	int priq_len;

	/* supported and enabled hardware capabilities */
	u64 cap;

	/* global lock, to be removed */
	spinlock_t cq_lock;

	/* device directory table root pointer and mode */
	unsigned long ddtp;
	unsigned ddt_mode;
	bool ddtp_in_iomem;

	/* I/O page fault queue */
	struct iopf_queue *pq_work;

	/* hardware ring buffers */
	struct riscv_iommu_queue cmdq;
	struct riscv_iommu_queue fltq;
	struct riscv_iommu_queue priq;

	/* Connected end-points */
	struct rb_root eps;
	struct mutex eps_mutex;

	/* Serializes owned HPM sessions and raw sysfs HPM writes. */
	struct mutex hpm_lock;
	bool hpm_ready;

	/*
	 * Incremented by the command-queue IRQ thread before it clears a
	 * reported CQ error, so that a checked S1 flush can notice an error
	 * that was already handled and cleared from CQCSR.
	 */
	atomic64_t cq_error_seq;
};

struct riscv_iommu_domain {
	struct iommu_domain domain;
	struct io_pgtable pgtbl;

	struct list_head endpoints;
	struct list_head notifiers;
	struct mutex lock;

	struct mmu_notifier mn;                 /* mmu_notifier handle */

	/* remove: could be a list of iommus */
	struct riscv_iommu_device *iommu;

	bool g_stage;
	struct riscv_iommu_msi_pte *msi_root;	/* INT mapping */
	struct riscv_iommu_domain *nested;	/* G-Stage protection domain if any */

	unsigned id;		/* GSCID or PSCID */
	unsigned mode;		/* RIO_ATP_MODE_* enum */
	ioasid_t pscid;		// this is a domain property

	pgd_t *pgd_root;	/* page table root pointer */
};

/* Private dev_iommu_priv object, device-domain relationship. */
struct riscv_iommu_endpoint {
	struct device *dev;			/* owned by a device $dev */
	unsigned devid;      			/* PCI bus:device:function number */
	unsigned domid;   			/* PCI domain number, segment */
	struct rb_node node;    		/* -> iommu-device lookup by devid */

	struct mutex lock;

	struct riscv_iommu_device *iommu;	/* -> parent iommu device */
	struct riscv_iommu_domain *domain;	/* -> attached domain, only one at a time, nesting via domain->domain */
	struct riscv_iommu_msi_pte *msi_root;	/* -> interrupt re-mapping */

	struct riscv_iommu_dc *dc;		/* -> device context pointer, can be tracked by iommu->dc(devid) */
	struct riscv_iommu_pc *pc;		/* -> process context root, can be tracked by iommu->dc(devid)->pc(pasid) */

	struct list_head regions;		// msi list
	struct list_head domains;		/* -> collection of endpoints attached to the same domain */

	/* end point info bits */
	unsigned pasid_bits;
	unsigned pasid_feat;
	bool pasid_enabled;
	bool ir_enabled;
};

/*
 * Acquire-time snapshot of one device targeted by an HPM session flush: the
 * bound device, the "core" iommu_domain attached to it and its device id.
 * The session holds no reference; the caller keeps the device bound and
 * serializes binding changes, and a NULL core domain is stored as-is.
 */
struct riscv_iommu_s1_flush_target {
	struct device *dev;
	struct iommu_domain *domain;
	u32 did;
};

/* Helper functions and macros */

static inline u32 riscv_iommu_readl(struct riscv_iommu_device *iommu,
				    unsigned offset)
{
	return readl_relaxed(iommu->reg + offset);
}

static inline void riscv_iommu_writel(struct riscv_iommu_device *iommu,
				      unsigned offset, u32 val)
{
	writel_relaxed(val, iommu->reg + offset);
}

static inline u64 riscv_iommu_readq(struct riscv_iommu_device *iommu,
				    unsigned offset)
{
	return readq_relaxed(iommu->reg + offset);
}

static inline void riscv_iommu_writeq(struct riscv_iommu_device *iommu,
				      unsigned offset, u64 val)
{
	writeq_relaxed(val, iommu->reg + offset);
}

int riscv_iommu_init(struct riscv_iommu_device *iommu);
void riscv_iommu_remove(struct riscv_iommu_device *iommu);

int riscv_iommu_sysfs_add(struct riscv_iommu_device *iommu);

struct riscv_iommu_endpoint *riscv_iommu_hpm_endpoint(struct device *dev);

/*
 * Checked global host first-stage invalidation barrier, implemented in
 * iommu.c. Private to the RISC-V IOMMU driver; see iommu.c for the exact
 * validation contract.
 */
int riscv_iommu_flush_s1_checked(struct riscv_iommu_device *iommu,
				 const struct riscv_iommu_s1_flush_target *targets,
				 unsigned int nr_targets);

static inline bool riscv_iommu_hpm_register(unsigned int offset)
{
	return offset >= RISCV_IOMMU_REG_IOCOUNTOVF &&
	       offset <= RISCV_IOMMU_REG_IOHPMEVT(30);
}

#endif
```

---

`linux/drivers/iommu/riscv/iommu-hpm.c`

```c
// SPDX-License-Identifier: GPL-2.0-only
/* Owned per-DID performance-counter sessions for the FPGA experiments. */

#include <linux/bitfield.h>
#include <linux/export.h>
#include <linux/riscv_iommu_hpm.h>
#include <linux/slab.h>

#include "iommu.h"

#define HPM_SESSION_COUNTERS	6
#define HPM_SESSION_INHIBIT	GENMASK(6, 1)
#define HPM_VICTIM_INHIBIT	GENMASK(3, 1)

struct riscv_iommu_hpm_session {
	struct riscv_iommu_device *iommu;
	u32 saved_inhibit;
	u64 saved_event[HPM_SESSION_COUNTERS];
	u64 saved_counter[HPM_SESSION_COUNTERS];
	unsigned int nr_targets;
	struct riscv_iommu_s1_flush_target targets[2];
};

/* The programming bridge uses 32-bit register accesses, even on RV64. */
static u64 hpm_read_counter(struct riscv_iommu_device *iommu, unsigned int reg)
{
	u32 high, low, again;

	do {
		high = readl(iommu->reg + reg + 4);
		low = readl(iommu->reg + reg);
		again = readl(iommu->reg + reg + 4);
	} while (high != again);

	return ((u64)high << 32) | low;
}

static u32 hpm_inhibit(struct riscv_iommu_device *iommu, u32 inhibit)
{
	writel(inhibit, iommu->reg + RISCV_IOMMU_REG_IOCOUNTINH);
	/* Finish the write before touching selectors or saved counter values. */
	return readl(iommu->reg + RISCV_IOMMU_REG_IOCOUNTINH);
}

bool riscv_iommu_hpm_supported(struct device *dev)
{
	struct riscv_iommu_endpoint *ep = riscv_iommu_hpm_endpoint(dev);

	return ep && READ_ONCE(ep->iommu->hpm_ready) &&
	       (readq(ep->iommu->reg + RISCV_IOMMU_REG_CAP) &
		RISCV_IOMMU_CAP_HPM);
}
EXPORT_SYMBOL_GPL(riscv_iommu_hpm_supported);

static void hpm_restore(struct riscv_iommu_hpm_session *session)
{
	struct riscv_iommu_device *iommu = session->iommu;
	unsigned int i;

	hpm_inhibit(iommu, session->saved_inhibit | HPM_SESSION_INHIBIT);
	for (i = 0; i < HPM_SESSION_COUNTERS; i++) {
		writeq(session->saved_counter[i],
		       iommu->reg + RISCV_IOMMU_REG_IOHPMCTR(i));
		writeq(session->saved_event[i],
		       iommu->reg + RISCV_IOMMU_REG_IOHPMEVT(i));
	}
	hpm_inhibit(iommu, session->saved_inhibit);
}

int riscv_iommu_hpm_acquire(struct device *victim, struct device *aggressor,
			   struct riscv_iommu_hpm_session **out)
{
	static const u64 events[] = {
		RISCV_IOMMU_EVENT_TLB_MISS,
		RISCV_IOMMU_EVENT_S1_PTW,
		RISCV_IOMMU_EVENT_S2_PTW,
	};
	struct riscv_iommu_endpoint *ep[2];
	struct riscv_iommu_hpm_session *session;
	struct riscv_iommu_device *iommu;
	u32 enable, run_inhibit;
	u64 selector;
	unsigned int i;
	int ret;

	if (!out)
		return -EINVAL;
	*out = NULL;
	ep[0] = riscv_iommu_hpm_endpoint(victim);
	ep[1] = aggressor ? riscv_iommu_hpm_endpoint(aggressor) : NULL;
	if (!ep[0] || (aggressor && !ep[1]))
		return -EOPNOTSUPP;
	iommu = ep[0]->iommu;
	if (ep[1] && ep[1]->iommu != iommu)
		return -EXDEV;
	if (ep[1] && ep[1]->devid == ep[0]->devid)
		return -EINVAL;

	session = kzalloc(sizeof(*session), GFP_KERNEL);
	if (!session)
		return -ENOMEM;
	if (!mutex_trylock(&iommu->hpm_lock)) {
		ret = -EBUSY;
		goto free;
	}
	if (!READ_ONCE(iommu->hpm_ready) ||
	    !(readq(iommu->reg + RISCV_IOMMU_REG_CAP) & RISCV_IOMMU_CAP_HPM)) {
		ret = -EOPNOTSUPP;
		goto unlock;
	}

	session->iommu = iommu;
	session->saved_inhibit = readl(iommu->reg + RISCV_IOMMU_REG_IOCOUNTINH);
	if ((hpm_inhibit(iommu, session->saved_inhibit | HPM_SESSION_INHIBIT) &
	     HPM_SESSION_INHIBIT) != HPM_SESSION_INHIBIT) {
		/* No selector or event counter has been changed yet. */
		hpm_inhibit(iommu, session->saved_inhibit);
		ret = -EOPNOTSUPP;
		goto unlock;
	}
	for (i = 0; i < HPM_SESSION_COUNTERS; i++) {
		session->saved_event[i] = readq(iommu->reg + RISCV_IOMMU_REG_IOHPMEVT(i));
		session->saved_counter[i] = hpm_read_counter(iommu,
							     RISCV_IOMMU_REG_IOHPMCTR(i));
	}

	/*
	 * This RTL exposes all 31 inhibit bits even with fewer actual counters,
	 * so their readback cannot discover the counter count. Verify the six
	 * required slots directly instead. Unimplemented registers read as zero
	 * and ignore writes on the current FPGA (regmap devmode is disabled).
	 * Selector readback checks storage/filter support, not event accuracy;
	 * cold/hot and per-DID calibration on the board is still required.
	 */
	for (i = 0; i < HPM_SESSION_COUNTERS; i++) {
		struct riscv_iommu_endpoint *selected = ep[i / 3] ?: ep[0];

		selector = RISCV_IOMMU_IOHPMEVT_OF |
			   RISCV_IOMMU_IOHPMEVT_DV_GSCV |
			   FIELD_PREP(RISCV_IOMMU_IOHPMEVT_DID_GSCID, selected->devid) |
			   FIELD_PREP(RISCV_IOMMU_IOHPMEVT_EVENT_ID, events[i % 3]);
		/* IDT=0, DMASK=0, PV=0: exact DID only; OF=1 suppresses IRQs. */
		writeq(selector, iommu->reg + RISCV_IOMMU_REG_IOHPMEVT(i));
		if (readq(iommu->reg + RISCV_IOMMU_REG_IOHPMEVT(i)) != selector) {
			ret = -EOPNOTSUPP;
			goto restore;
		}
		writeq(0, iommu->reg + RISCV_IOMMU_REG_IOHPMCTR(i));
		if (hpm_read_counter(iommu, RISCV_IOMMU_REG_IOHPMCTR(i))) {
			ret = -EIO;
			goto restore;
		}
	}

	/* Solo reserves all six slots but leaves the unused lane inhibited. */
	enable = (aggressor ? HPM_SESSION_INHIBIT : HPM_VICTIM_INHIBIT) |
		 RISCV_IOMMU_IOCOUNTINH_CY;
	run_inhibit = (session->saved_inhibit | HPM_SESSION_INHIBIT) & ~enable;
	if ((hpm_inhibit(iommu, run_inhibit) ^ run_inhibit) &
	    (HPM_SESSION_INHIBIT | RISCV_IOMMU_IOCOUNTINH_CY)) {
		ret = -EOPNOTSUPP;
		goto restore;
	}

	/*
	 * Snapshot the targets for a later riscv_iommu_hpm_flush_s1(). The
	 * "core" domain is recorded exactly as iommu_get_domain_for_dev()
	 * reports it, so a device that is not attached to a domain (NULL) or
	 * that currently uses a domain owned by another consumer is still
	 * accepted here and left for the flush to validate. No device or
	 * domain reference is taken: the caller keeps both devices bound and
	 * calls flush from the task that acquired the session.
	 */
	session->nr_targets = aggressor ? 2 : 1;
	for (i = 0; i < session->nr_targets; i++) {
		struct device *dev = i ? aggressor : victim;

		session->targets[i].dev = dev;
		session->targets[i].domain = iommu_get_domain_for_dev(dev);
		session->targets[i].did = ep[i]->devid;
	}

	*out = session;
	return 0;

restore:
	hpm_restore(session);
unlock:
	mutex_unlock(&iommu->hpm_lock);
free:
	kfree(session);
	return ret;
}
EXPORT_SYMBOL_GPL(riscv_iommu_hpm_acquire);

void riscv_iommu_hpm_read(struct riscv_iommu_hpm_session *session,
			  struct riscv_iommu_hpm_sample *sample)
{
	struct riscv_iommu_device *iommu;
	unsigned int lane;

	memset(sample, 0, sizeof(*sample));
	if (!session)
		return;
	iommu = session->iommu;
	lockdep_assert_held(&iommu->hpm_lock);
	sample->cycles = hpm_read_counter(iommu, RISCV_IOMMU_REG_IOHPMCYCLES) &
			 RISCV_IOMMU_IOHPMCYCLES_COUNTER;
	for (lane = 0; lane < 2; lane++) {
		sample->iotlb_miss[lane] = hpm_read_counter(iommu,
							    RISCV_IOMMU_REG_IOHPMCTR(lane * 3));
		sample->s1_ptw[lane] = hpm_read_counter(iommu,
							RISCV_IOMMU_REG_IOHPMCTR(lane * 3 + 1));
		sample->s2_ptw[lane] = hpm_read_counter(iommu,
							RISCV_IOMMU_REG_IOHPMCTR(lane * 3 + 2));
	}
}
EXPORT_SYMBOL_GPL(riscv_iommu_hpm_read);

void riscv_iommu_hpm_release(struct riscv_iommu_hpm_session *session)
{
	if (!session)
		return;
	lockdep_assert_held(&session->iommu->hpm_lock);
	hpm_restore(session);
	mutex_unlock(&session->iommu->hpm_lock);
	kfree(session);
}
EXPORT_SYMBOL_GPL(riscv_iommu_hpm_release);

int riscv_iommu_hpm_flush_s1(struct riscv_iommu_hpm_session *session)
{
	if (!session)
		return -EINVAL;
	lockdep_assert_held(&session->iommu->hpm_lock);
	return riscv_iommu_flush_s1_checked(session->iommu, session->targets,
					    session->nr_targets);
}
EXPORT_SYMBOL_GPL(riscv_iommu_hpm_flush_s1);
```

---

`linux/drivers/iommu/riscv/iommu.c`

```c
// SPDX-License-Identifier: GPL-2.0-only
/*
 * IOMMU API for RISC-V architected Ziommu implementations.
 *
 * Copyright © 2022-2023 Rivos Inc.
 * Copyright © 2023 FORTH-ICS/CARV
 *
 * Authors
 *	Tomasz Jeznach <tjeznach@rivosinc.com>
 *	Nick Kossifidis <mick@ics.forth.gr>
 */

#define pr_fmt(fmt) KBUILD_MODNAME ": " fmt

#include <linux/bitfield.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/compiler.h>
#include <linux/pci.h>
#include <linux/pci-ats.h>
#include <linux/init.h>
#include <linux/atomic.h>
#include <linux/completion.h>
#include <linux/uaccess.h>
#include <linux/iommu.h>
#include <linux/irqdomain.h>
#include <linux/platform_device.h>
#include <linux/dma-map-ops.h>
#include <asm/page.h>

#include "../dma-iommu.h"
#include "../iommu-sva.h"
#include "iommu.h"

#include <asm/csr.h>
#include <asm/delay.h>

MODULE_DESCRIPTION("IOMMU driver for RISC-V architected Ziommu implementations");
MODULE_AUTHOR("Tomasz Jeznach <tjeznach@rivosinc.com>");
MODULE_AUTHOR("Nick Kossifidis <mick@ics.forth.gr>");
MODULE_ALIAS("riscv-iommu");
MODULE_LICENSE("GPL v2");

/* Global IOMMU params. */
static int ddt_mode = RISCV_IOMMU_DDTP_MODE_1LVL;
module_param(ddt_mode, int, 0644);
MODULE_PARM_DESC(ddt_mode, "Device Directory Table mode.");

static int cmdq_length = 128;
module_param(cmdq_length, int, 0644);
MODULE_PARM_DESC(cmdq_length, "Command queue length.");

static int fltq_length = 128;
module_param(fltq_length, int, 0644);
MODULE_PARM_DESC(fltq_length, "Fault queue length.");

static int priq_length = 128;
module_param(priq_length, int, 0644);
MODULE_PARM_DESC(priq_length, "Page request queue length.");

/* TODO: Enable MSI remapping */
#define RISCV_IMSIC_BASE	0x28000000

/* 1 second */
#define RISCV_IOMMU_TIMEOUT	riscv_timebase

/* RISC-V IOMMU PPN <> PHYS address conversions, PHYS <=> PPN[53:10] */
#define phys_to_ppn(va)  (((va) >> 2) & (((1ULL << 44) - 1) << 10))
#define ppn_to_phys(pn)	 (((pn) << 2) & (((1ULL << 44) - 1) << 12))

#define iommu_domain_to_riscv(iommu_domain) \
    container_of(iommu_domain, struct riscv_iommu_domain, domain)

#define iommu_device_to_riscv(iommu_device) \
    container_of(iommu_device, struct riscv_iommu, iommu)

static const struct iommu_domain_ops riscv_iommu_domain_ops;
static const struct iommu_ops riscv_iommu_ops;

/*
 * Common queue management routines
 */

/* Note: offsets are the same for all queues */
#define Q_HEAD(q) ((q)->qbr + (RISCV_IOMMU_REG_CQH - RISCV_IOMMU_REG_CQB))
#define Q_TAIL(q) ((q)->qbr + (RISCV_IOMMU_REG_CQT - RISCV_IOMMU_REG_CQB))

static unsigned riscv_iommu_queue_consume(struct riscv_iommu_device *iommu,
					  struct riscv_iommu_queue *q, unsigned *ready)
{
	u32 tail = riscv_iommu_readl(iommu, Q_TAIL(q));
	*ready = q->lui;

	BUG_ON(q->cnt <= tail);
	if (q->lui <= tail)
		return tail - q->lui;
	return q->cnt - q->lui;
}

static void riscv_iommu_queue_release(struct riscv_iommu_device *iommu,
				      struct riscv_iommu_queue *q, unsigned count)
{
	q->lui = (q->lui + count) & (q->cnt - 1);
	riscv_iommu_writel(iommu, Q_HEAD(q), q->lui);
}

static u32 riscv_iommu_queue_ctrl(struct riscv_iommu_device *iommu,
				  struct riscv_iommu_queue *q, u32 val)
{
	cycles_t end_cycles = RISCV_IOMMU_TIMEOUT + get_cycles();

	riscv_iommu_writel(iommu, q->qcr, val);
	do {
		val = riscv_iommu_readl(iommu, q->qcr);
		if (!(val & RISCV_IOMMU_QUEUE_BUSY))
			break;
		cpu_relax();
	} while (get_cycles() < end_cycles);

	return val;
}

static void riscv_iommu_queue_free(struct riscv_iommu_device *iommu,
				   struct riscv_iommu_queue *q)
{
	size_t size = q->len * q->cnt;

	riscv_iommu_queue_ctrl(iommu, q, 0);

	if (q->base) {
		if (q->in_iomem)
			iounmap(q->base);
		else
			dmam_free_coherent(iommu->dev, size, q->base, q->base_dma);
	}
	if (q->irq)
		free_irq(q->irq, q);
}

static irqreturn_t riscv_iommu_cmdq_irq_check(int irq, void *data);
static irqreturn_t riscv_iommu_cmdq_process(int irq, void *data);
static irqreturn_t riscv_iommu_fltq_irq_check(int irq, void *data);
static irqreturn_t riscv_iommu_fltq_process(int irq, void *data);
static irqreturn_t riscv_iommu_priq_irq_check(int irq, void *data);
static irqreturn_t riscv_iommu_priq_process(int irq, void *data);

static int riscv_iommu_queue_init(struct riscv_iommu_device *iommu, int queue_id)
{
	struct device *dev = iommu->dev;
	struct riscv_iommu_queue *q = NULL;
	size_t queue_size = 0;
	irq_handler_t irq_check;
	irq_handler_t irq_process;
	const char *name;
	int count = 0;
	int irq = 0;
	unsigned order = 0;
	u64 qbr_val = 0;
	u64 qbr_readback = 0;
	u64 qbr_paddr = 0;
	int ret = 0;

	switch (queue_id) {
	case RISCV_IOMMU_COMMAND_QUEUE:
		q = &iommu->cmdq;
		q->len = sizeof(struct riscv_iommu_command);
		count = iommu->cmdq_len;
		irq = iommu->irq_cmdq;
		irq_check = riscv_iommu_cmdq_irq_check;
		irq_process = riscv_iommu_cmdq_process;
		q->qbr = RISCV_IOMMU_REG_CQB;
		q->qcr = RISCV_IOMMU_REG_CQCSR;
		name = "cmdq";
		break;
	case RISCV_IOMMU_FAULT_QUEUE:
		q = &iommu->fltq;
		q->len = sizeof(struct riscv_iommu_fq_record);
		count = iommu->fltq_len;
		irq = iommu->irq_fltq;
		irq_check = riscv_iommu_fltq_irq_check;
		irq_process = riscv_iommu_fltq_process;
		q->qbr = RISCV_IOMMU_REG_FQB;
		q->qcr = RISCV_IOMMU_REG_FQCSR;
		name = "fltq";
		break;
	case RISCV_IOMMU_PAGE_REQUEST_QUEUE:
		q = &iommu->priq;
		q->len = sizeof(struct riscv_iommu_pq_record);
		count = iommu->priq_len;
		irq = iommu->irq_priq;
		irq_check = riscv_iommu_priq_irq_check;
		irq_process = riscv_iommu_priq_process;
		q->qbr = RISCV_IOMMU_REG_PQB;
		q->qcr = RISCV_IOMMU_REG_PQCSR;
		name = "priq";
		break;
	default:
		dev_err(dev, "invalid queue interrupt index in queue_init!\n");
		return -EINVAL;
	}

	/* Polling not implemented */
	if (!irq)
		return -ENODEV;

	/* Allocate queue in memory and set the base register */
	order = ilog2(count);
	do {
		queue_size = q->len * (1ULL << order);
		q->base = dmam_alloc_coherent(dev, queue_size, &q->base_dma, GFP_KERNEL);
		if (q->base || queue_size < PAGE_SIZE)
			break;

		order--;
	} while (1);

	if (!q->base) {
		dev_err(dev, "failed to allocate %s queue (cnt: %u)\n", name, count);
		return -ENOMEM;
	}

	q->cnt = 1ULL << order;

	qbr_val = phys_to_ppn(q->base_dma) |
	    FIELD_PREP(RISCV_IOMMU_QUEUE_SIZE_FIELD, order - 1);

	riscv_iommu_writeq(iommu, q->qbr, qbr_val);

	/*
	 * Queue base registers are WARL, so it's possible that whatever we wrote
	 * there was illegal/not supported by the hw in which case we need to make
	 * sure we set a supported PPN and/or queue size.
	 */
	qbr_readback = riscv_iommu_readq(iommu, q->qbr);
	if (qbr_readback == qbr_val)
		goto irq;

	dmam_free_coherent(dev, queue_size, q->base, q->base_dma);

	/* Get supported queue size */
	order = FIELD_GET(RISCV_IOMMU_QUEUE_SIZE_FIELD, qbr_readback) + 1;
	q->cnt = 1ULL << order;
	queue_size = q->len * q->cnt;

	/*
	 * In case we also failed to set PPN, it means the field is hardcoded and the
	 * queue resides in I/O memory instead, so get its physical address and
	 * ioremap it.
	 */
	qbr_paddr = ppn_to_phys(qbr_readback);
	if (qbr_paddr != q->base_dma) {
		dev_info(dev,
			 "hardcoded ppn in %s base register, using io memory for the queue\n",
			 name);
		dev_info(dev, "queue length for %s set to %i\n", name, q->cnt);
		q->in_iomem = true;
		q->base = ioremap(qbr_paddr, queue_size);
		if (!q->base) {
			dev_err(dev, "failed to map %s queue (cnt: %u)\n", name, q->cnt);
			return -ENOMEM;
		}
		q->base_dma = qbr_paddr;
	} else {
		/*
		 * We only failed to set the queue size, re-try to allocate memory with
		 * the queue size supported by the hw.
		 */
		dev_info(dev, "hardcoded queue size in %s base register\n", name);
		dev_info(dev, "retrying with queue length: %i\n", q->cnt);
		q->base = dmam_alloc_coherent(dev, queue_size, &q->base_dma, GFP_KERNEL);
		if (!q->base) {
			dev_err(dev, "failed to allocate %s queue (cnt: %u)\n",
				name, q->cnt);
			return -ENOMEM;
		}
	}

	qbr_val = phys_to_ppn(q->base_dma) |
	    FIELD_PREP(RISCV_IOMMU_QUEUE_SIZE_FIELD, order - 1);
	riscv_iommu_writeq(iommu, q->qbr, qbr_val);

	/* Final check to make sure hw accepted our write */
	qbr_readback = riscv_iommu_readq(iommu, q->qbr);
	if (qbr_readback != qbr_val) {
		dev_err(dev, "failed to set base register for %s\n", name);
		goto fail;
	}

 irq:
	if (request_threaded_irq(irq, irq_check, irq_process, IRQF_ONESHOT | IRQF_SHARED,
				 dev_name(dev), q)) {
		dev_err(dev, "fail to request irq %d for %s\n", irq, name);
		goto fail;
	}

	q->irq = irq;

	/* Note: All RIO_xQ_EN/IE fields are in the same offsets */
	ret =
	    riscv_iommu_queue_ctrl(iommu, q,
				   RISCV_IOMMU_QUEUE_ENABLE |
				   RISCV_IOMMU_QUEUE_INTR_ENABLE);
	if (ret & RISCV_IOMMU_QUEUE_BUSY) {
		dev_err(dev, "%s init timeout\n", name);
		ret = -EBUSY;
		goto fail;
	}

	return 0;

 fail:
	riscv_iommu_queue_free(iommu, q);
	return 0;
}

/*
 * I/O MMU Command queue chapter 3.1
 */

static inline void riscv_iommu_cmd_inval_vma(struct riscv_iommu_command *cmd)
{
	cmd->dword0 =
	    FIELD_PREP(RISCV_IOMMU_CMD_OPCODE,
		       RISCV_IOMMU_CMD_IOTINVAL_OPCODE) | FIELD_PREP(RISCV_IOMMU_CMD_FUNC,
								     RISCV_IOMMU_CMD_IOTINVAL_FUNC_VMA);
	cmd->dword1 = 0;
}

static void riscv_iommu_cmd_inval_gvma(struct riscv_iommu_command *cmd)
{
	cmd->dword0 =
	    FIELD_PREP(RISCV_IOMMU_CMD_OPCODE,
		       RISCV_IOMMU_CMD_IOTINVAL_OPCODE) | FIELD_PREP(RISCV_IOMMU_CMD_FUNC,
								     RISCV_IOMMU_CMD_IOTINVAL_FUNC_GVMA);
	cmd->dword1 = 0;
}

static inline void riscv_iommu_cmd_inval_set_addr(struct riscv_iommu_command *cmd,
						  u64 addr)
{
	cmd->dword0 |= RISCV_IOMMU_CMD_IOTINVAL_AV;
	cmd->dword1 = addr;
}

static inline void riscv_iommu_cmd_inval_set_pscid(struct riscv_iommu_command *cmd,
						   unsigned pscid)
{
	cmd->dword0 |= FIELD_PREP(RISCV_IOMMU_CMD_IOTINVAL_PSCID, pscid) |
	    RISCV_IOMMU_CMD_IOTINVAL_PSCV;
}

static inline void riscv_iommu_cmd_inval_set_gscid(struct riscv_iommu_command *cmd,
						   unsigned gscid)
{
	cmd->dword0 |= FIELD_PREP(RISCV_IOMMU_CMD_IOTINVAL_GSCID, gscid) |
	    RISCV_IOMMU_CMD_IOTINVAL_GV;
}

static inline void riscv_iommu_cmd_iofence(struct riscv_iommu_command *cmd)
{
	cmd->dword0 = FIELD_PREP(RISCV_IOMMU_CMD_OPCODE, RISCV_IOMMU_CMD_IOFENCE_OPCODE) |
	    FIELD_PREP(RISCV_IOMMU_CMD_FUNC, RISCV_IOMMU_CMD_IOFENCE_FUNC_C);
	cmd->dword1 = 0;
}

static inline void riscv_iommu_cmd_iofence_set_av(struct riscv_iommu_command *cmd,
						  u64 addr, u32 data)
{
	cmd->dword0 = FIELD_PREP(RISCV_IOMMU_CMD_OPCODE, RISCV_IOMMU_CMD_IOFENCE_OPCODE) |
	    FIELD_PREP(RISCV_IOMMU_CMD_FUNC, RISCV_IOMMU_CMD_IOFENCE_FUNC_C) |
	    FIELD_PREP(RISCV_IOMMU_CMD_IOFENCE_DATA, data) | RISCV_IOMMU_CMD_IOFENCE_AV;
	cmd->dword1 = (addr >> 2);
}

static inline void riscv_iommu_cmd_iodir_inval_ddt(struct riscv_iommu_command *cmd)
{
	cmd->dword0 = FIELD_PREP(RISCV_IOMMU_CMD_OPCODE, RISCV_IOMMU_CMD_IODIR_OPCODE) |
	    FIELD_PREP(RISCV_IOMMU_CMD_FUNC, RISCV_IOMMU_CMD_IODIR_FUNC_INVAL_DDT);
	cmd->dword1 = 0;
}

static inline void riscv_iommu_cmd_iodir_inval_pdt(struct riscv_iommu_command *cmd)
{
	cmd->dword0 = FIELD_PREP(RISCV_IOMMU_CMD_OPCODE, RISCV_IOMMU_CMD_IODIR_OPCODE) |
	    FIELD_PREP(RISCV_IOMMU_CMD_FUNC, RISCV_IOMMU_CMD_IODIR_FUNC_INVAL_PDT);
	cmd->dword1 = 0;
}

static inline void riscv_iommu_cmd_iodir_set_did(struct riscv_iommu_command *cmd,
						 unsigned devid)
{
	cmd->dword0 |=
	    FIELD_PREP(RISCV_IOMMU_CMD_IODIR_DID, devid) | RISCV_IOMMU_CMD_IODIR_DV;
}

static inline void riscv_iommu_cmd_iodir_set_pid(struct riscv_iommu_command *cmd,
						 unsigned pasid)
{
	cmd->dword0 |= FIELD_PREP(RISCV_IOMMU_CMD_IODIR_PID, pasid);
}

static void riscv_iommu_cmd_ats_inval(struct riscv_iommu_command *cmd)
{
	cmd->dword0 = FIELD_PREP(RISCV_IOMMU_CMD_OPCODE, RISCV_IOMMU_CMD_ATS_OPCODE) |
	    FIELD_PREP(RISCV_IOMMU_CMD_FUNC, RISCV_IOMMU_CMD_ATS_FUNC_INVAL);
	cmd->dword1 = 0;
}

static inline void riscv_iommu_cmd_ats_prgr(struct riscv_iommu_command *cmd)
{
	cmd->dword0 = FIELD_PREP(RISCV_IOMMU_CMD_OPCODE, RISCV_IOMMU_CMD_ATS_OPCODE) |
	    FIELD_PREP(RISCV_IOMMU_CMD_FUNC, RISCV_IOMMU_CMD_ATS_FUNC_PRGR);
	cmd->dword1 = 0;
}

static void riscv_iommu_cmd_ats_set_rid(struct riscv_iommu_command *cmd, u32 rid)
{
	cmd->dword0 |= FIELD_PREP(RISCV_IOMMU_CMD_ATS_RID, rid);
}

static void riscv_iommu_cmd_ats_set_pid(struct riscv_iommu_command *cmd, u32 pid)
{
	cmd->dword0 |= FIELD_PREP(RISCV_IOMMU_CMD_ATS_PID, pid) | RISCV_IOMMU_CMD_ATS_PV;
}

static void riscv_iommu_cmd_ats_set_dseg(struct riscv_iommu_command *cmd, u8 seg)
{
	cmd->dword0 |= FIELD_PREP(RISCV_IOMMU_CMD_ATS_DSEG, seg) | RISCV_IOMMU_CMD_ATS_DSV;
}

static void riscv_iommu_cmd_ats_set_payload(struct riscv_iommu_command *cmd, u64 payload)
{
	cmd->dword1 = payload;
}

/* Prepare the ATS invalidation payload */
static unsigned long riscv_iommu_ats_inval_payload(unsigned long start,
						   unsigned long end, bool global_inv)
{
	size_t len = end - start + 1;
	unsigned long payload = 0;

	/*
	 * PCI Express specification
	 * Section 10.2.3.2 Translation Range Size (S) Field
	 */
	if (len < PAGE_SIZE)
		len = PAGE_SIZE;
	else
		len = __roundup_pow_of_two(len);

	payload = (start & ~(len - 1)) | (((len - 1) >> 12) << 11);

	if (global_inv)
		payload |= RISCV_IOMMU_CMD_ATS_INVAL_G;

	return payload;
}

/* Prepare the ATS invalidation payload for all translations to be invalidated. */
static unsigned long riscv_iommu_ats_inval_all_payload(bool global_inv)
{
	unsigned long payload = GENMASK_ULL(62, 11);

	if (global_inv)
		payload |= RISCV_IOMMU_CMD_ATS_INVAL_G;

	return payload;
}

// Prepare the ATS "Page Request Group Response" payload
static unsigned long riscv_iommu_ats_prgr_payload(u16 dest_id, u8 resp_code, u16 grp_idx)
{
	return FIELD_PREP(RISCV_IOMMU_CMD_ATS_PRGR_DST_ID, dest_id) |
	    FIELD_PREP(RISCV_IOMMU_CMD_ATS_PRGR_RESP_CODE, resp_code) |
	    FIELD_PREP(RISCV_IOMMU_CMD_ATS_PRGR_PRG_INDEX, grp_idx);
}

/* TODO: Convert into lock-less MPSC implementation. */
static bool riscv_iommu_post_sync(struct riscv_iommu_device *iommu,
				  struct riscv_iommu_command *cmd, bool sync)
{
	u32 head, tail, next, last;
	unsigned long flags;

	spin_lock_irqsave(&iommu->cq_lock, flags);
	head = riscv_iommu_readl(iommu, RISCV_IOMMU_REG_CQH) & (iommu->cmdq.cnt - 1);
	tail = riscv_iommu_readl(iommu, RISCV_IOMMU_REG_CQT) & (iommu->cmdq.cnt - 1);
	last = iommu->cmdq.lui;
	if (tail != last) {
		spin_unlock_irqrestore(&iommu->cq_lock, flags);
		/* TRY AGAIN */
		dev_err(iommu->dev, "IOMMU CQT: %x != %x (1st)\n", last, tail);
		spin_lock_irqsave(&iommu->cq_lock, flags);
		tail =
		    riscv_iommu_readl(iommu, RISCV_IOMMU_REG_CQT) & (iommu->cmdq.cnt - 1);
		last = iommu->cmdq.lui;
		if (tail != last) {
			spin_unlock_irqrestore(&iommu->cq_lock, flags);
			dev_err(iommu->dev, "IOMMU CQT: %x != %x (2nd)\n", last, tail);
			spin_lock_irqsave(&iommu->cq_lock, flags);
		}
	}

	next = (last + 1) & (iommu->cmdq.cnt - 1);
	if (next != head) {
		struct riscv_iommu_command *ptr = iommu->cmdq.base;
		ptr[last] = *cmd;
		wmb();
		riscv_iommu_writel(iommu, RISCV_IOMMU_REG_CQT, next);
		iommu->cmdq.lui = next;
	}

	spin_unlock_irqrestore(&iommu->cq_lock, flags);

	if (sync && head != next) {
		cycles_t start_time = get_cycles();
		while (1) {
			last = riscv_iommu_readl(iommu, RISCV_IOMMU_REG_CQH) &
			    (iommu->cmdq.cnt - 1);
			if (head < next && last >= next)
				break;
			if (head > next && last < head && last >= next)
				break;
			if (RISCV_IOMMU_TIMEOUT < (get_cycles() - start_time)) {
				dev_err(iommu->dev, "IOFENCE TIMEOUT\n");
				return false;
			}
			cpu_relax();
		}
	}

	return next != head;
}

static bool riscv_iommu_post(struct riscv_iommu_device *iommu,
			     struct riscv_iommu_command *cmd)
{
	return riscv_iommu_post_sync(iommu, cmd, false);
}

static bool riscv_iommu_iodir_inv_devid(struct riscv_iommu_device *iommu, unsigned devid)
{
	struct riscv_iommu_command cmd;
	riscv_iommu_cmd_iodir_inval_ddt(&cmd);
	riscv_iommu_cmd_iodir_set_did(&cmd, devid);
	return riscv_iommu_post(iommu, &cmd);
}

static bool riscv_iommu_iodir_inv_pasid(struct riscv_iommu_device *iommu,
					unsigned devid, unsigned pasid)
{
	struct riscv_iommu_command cmd;
	riscv_iommu_cmd_iodir_inval_pdt(&cmd);
	riscv_iommu_cmd_iodir_set_did(&cmd, devid);
	riscv_iommu_cmd_iodir_set_pid(&cmd, pasid);
	return riscv_iommu_post(iommu, &cmd);
}

static bool riscv_iommu_iofence_sync(struct riscv_iommu_device *iommu)
{
	struct riscv_iommu_command cmd;
	riscv_iommu_cmd_iofence(&cmd);
	return riscv_iommu_post_sync(iommu, &cmd, true);
}

static void riscv_iommu_mm_invalidate(struct mmu_notifier *mn,
				      struct mm_struct *mm, unsigned long start,
				      unsigned long end)
{
	struct riscv_iommu_command cmd;
	struct riscv_iommu_endpoint *endpoint;
	struct riscv_iommu_domain *domain =
	    container_of(mn, struct riscv_iommu_domain, mn);
	unsigned long iova;
	/*
	 * The mm_types defines vm_end as the first byte after the end address,
	 * different from IOMMU subsystem using the last address of an address
	 * range. So do a simple translation here by updating what end means.
	 */
	unsigned long payload = riscv_iommu_ats_inval_payload(start, end - 1, true);

	/* TODO: cleanup GSCID/PSCID passing */
	riscv_iommu_cmd_inval_vma(&cmd);
	riscv_iommu_cmd_inval_set_gscid(&cmd, 0);
	riscv_iommu_cmd_inval_set_pscid(&cmd, domain->pscid);
	if (end > start) {
		/* Cover only the range that is needed */
		for (iova = start; iova < end; iova += PAGE_SIZE) {
			riscv_iommu_cmd_inval_set_addr(&cmd, iova);
			riscv_iommu_post(domain->iommu, &cmd);
		}
	} else {
		riscv_iommu_post(domain->iommu, &cmd);
	}

	riscv_iommu_iofence_sync(domain->iommu);

	/* ATS invalidation for every device and for specific translation range. */
	list_for_each_entry(endpoint, &domain->endpoints, domains) {
		if (!endpoint->pasid_enabled)
			continue;

		riscv_iommu_cmd_ats_inval(&cmd);
		riscv_iommu_cmd_ats_set_dseg(&cmd, endpoint->domid);
		riscv_iommu_cmd_ats_set_rid(&cmd, endpoint->devid);
		riscv_iommu_cmd_ats_set_pid(&cmd, domain->pscid);
		riscv_iommu_cmd_ats_set_payload(&cmd, payload);
		riscv_iommu_post(domain->iommu, &cmd);
	}
	riscv_iommu_iofence_sync(domain->iommu);
}

static void riscv_iommu_mm_release(struct mmu_notifier *mn, struct mm_struct *mm)
{
	/* TODO: removed from notifier, cleanup PSCID mapping, flush IOTLB */
}

static const struct mmu_notifier_ops riscv_iommu_mmuops = {
	.release = riscv_iommu_mm_release,
	.invalidate_range = riscv_iommu_mm_invalidate,
};

/* Command queue primary interrupt handler */
static irqreturn_t riscv_iommu_cmdq_irq_check(int irq, void *data)
{
	struct riscv_iommu_queue *q = (struct riscv_iommu_queue *)data;
	struct riscv_iommu_device *iommu =
	    container_of(q, struct riscv_iommu_device, cmdq);
	u32 ipsr = riscv_iommu_readl(iommu, RISCV_IOMMU_REG_IPSR);
	if (ipsr & RISCV_IOMMU_IPSR_CIP)
		return IRQ_WAKE_THREAD;
	return IRQ_NONE;
}

/* Command queue interrupt hanlder thread function */
static irqreturn_t riscv_iommu_cmdq_process(int irq, void *data)
{
	struct riscv_iommu_queue *q = (struct riscv_iommu_queue *)data;
	struct riscv_iommu_device *iommu;
	unsigned ctrl;

	iommu = container_of(q, struct riscv_iommu_device, cmdq);

	/* Error reporting, clear error reports if any. */
	ctrl = riscv_iommu_readl(iommu, RISCV_IOMMU_REG_CQCSR);
	if (ctrl & (RISCV_IOMMU_CQCSR_CQMF |
		    RISCV_IOMMU_CQCSR_CMD_TO | RISCV_IOMMU_CQCSR_CMD_ILL)) {
		/*
		 * Publish the error generation before the (relaxed) MMIO clear,
		 * so an observer that reads a clean CQCSR afterwards also sees
		 * the bumped generation.
		 */
		atomic64_inc(&iommu->cq_error_seq);
		wmb();
		riscv_iommu_queue_ctrl(iommu, &iommu->cmdq, ctrl);
		dev_warn_ratelimited(iommu->dev,
				     "Command queue error: fault: %d tout: %d err: %d\n",
				     !!(ctrl & RISCV_IOMMU_CQCSR_CQMF),
				     !!(ctrl & RISCV_IOMMU_CQCSR_CMD_TO),
				     !!(ctrl & RISCV_IOMMU_CQCSR_CMD_ILL));
	}

	/* Clear fault interrupt pending. */
	riscv_iommu_writel(iommu, RISCV_IOMMU_REG_IPSR, RISCV_IOMMU_IPSR_CIP);

	return IRQ_HANDLED;
}

/*
 * Fault/event queue, chapter 3.2
 */

static void riscv_iommu_fault_report(struct riscv_iommu_device *iommu,
				     struct riscv_iommu_fq_record *event)
{
	unsigned err, devid;
	u64 iotval, iotval2;

	err = FIELD_GET(RISCV_IOMMU_FQ_HDR_CAUSE, event->hdr);
	devid = FIELD_GET(RISCV_IOMMU_FQ_HDR_DID, event->hdr);
	iotval = event->iotval;
	iotval2 = event->iotval2;
	
	dev_warn_ratelimited(iommu->dev, "F: %d | DID: %d\n",
							err, devid);
	dev_warn_ratelimited(iommu->dev, "iotval: %llx\n",
							iotval);
	dev_warn_ratelimited(iommu->dev, "iotval2: %llx\n",
							iotval2);
}

/* Fault/event queue primary interrupt handler */
static irqreturn_t riscv_iommu_fltq_irq_check(int irq, void *data)
{
	struct riscv_iommu_queue *q = (struct riscv_iommu_queue *)data;
	struct riscv_iommu_device *iommu =
	    container_of(q, struct riscv_iommu_device, fltq);
	u32 ipsr = riscv_iommu_readl(iommu, RISCV_IOMMU_REG_IPSR);
	if (ipsr & RISCV_IOMMU_IPSR_FIP)
		return IRQ_WAKE_THREAD;
	return IRQ_NONE;
}

/* Fault queue interrupt hanlder thread function */
static irqreturn_t riscv_iommu_fltq_process(int irq, void *data)
{
	struct riscv_iommu_queue *q = (struct riscv_iommu_queue *)data;
	struct riscv_iommu_device *iommu;
	struct riscv_iommu_fq_record *events;
	unsigned cnt, len, idx, ctrl;

	iommu = container_of(q, struct riscv_iommu_device, fltq);
	events = (struct riscv_iommu_fq_record *)q->base;

	/* Error reporting, clear error reports if any. */
	ctrl = riscv_iommu_readl(iommu, RISCV_IOMMU_REG_FQCSR);
	if (ctrl & (RISCV_IOMMU_FQCSR_FQMF | RISCV_IOMMU_FQCSR_FWOF)) {
		riscv_iommu_queue_ctrl(iommu, &iommu->fltq, ctrl);
		dev_warn_ratelimited(iommu->dev,
				     "Fault queue error: fault: %d full: %d\n",
				     !!(ctrl & RISCV_IOMMU_FQCSR_FQMF),
				     !!(ctrl & RISCV_IOMMU_FQCSR_FWOF));
	}

	/* Clear fault interrupt pending. */
	riscv_iommu_writel(iommu, RISCV_IOMMU_REG_IPSR, RISCV_IOMMU_IPSR_FIP);

	/* Report fault events. */
	do {
		cnt = riscv_iommu_queue_consume(iommu, q, &idx);
		if (!cnt)
			break;
		for (len = 0; len < cnt; idx++, len++)
			riscv_iommu_fault_report(iommu, &events[idx]);
		riscv_iommu_queue_release(iommu, q, cnt);
	} while (1);

	return IRQ_HANDLED;
}

/*
 * Page request queue, chapter 3.3
 */

/*
 * Register device for IOMMU tracking.
 */
static void riscv_iommu_add_device(struct riscv_iommu_device *iommu, struct device *dev)
{
	struct riscv_iommu_endpoint *ep, *rb_ep;
	struct rb_node **new_node, *parent_node = NULL;

	mutex_lock(&iommu->eps_mutex);

	ep = dev_iommu_priv_get(dev);

	new_node = &(iommu->eps.rb_node);
	while (*new_node) {
		rb_ep = rb_entry(*new_node, struct riscv_iommu_endpoint, node);
		parent_node = *new_node;
		if (rb_ep->devid > ep->devid) {
			new_node = &((*new_node)->rb_left);
		} else if (rb_ep->devid < ep->devid) {
			new_node = &((*new_node)->rb_right);
		} else {
			dev_warn(dev, "device %u already in the tree\n", ep->devid);
			break;
		}
	}

	rb_link_node(&ep->node, parent_node, new_node);
	rb_insert_color(&ep->node, &iommu->eps);

	mutex_unlock(&iommu->eps_mutex);
}

/*
 * Get device reference based on device identifier (requester id).
 * Decrement reference count with put_device() call.
 */
static struct device *riscv_iommu_get_device(struct riscv_iommu_device *iommu,
					     unsigned devid)
{
	struct rb_node *node;
	struct riscv_iommu_endpoint *ep;
	struct device *dev = NULL;

	mutex_lock(&iommu->eps_mutex);

	node = iommu->eps.rb_node;
	while (node && !dev) {
		ep = rb_entry(node, struct riscv_iommu_endpoint, node);
		if (ep->devid < devid)
			node = node->rb_right;
		else if (ep->devid > devid)
			node = node->rb_left;
		else
			dev = get_device(ep->dev);
	}

	mutex_unlock(&iommu->eps_mutex);

	return dev;
}

static int riscv_iommu_ats_prgr(struct device *dev, struct iommu_page_response *msg)
{
	struct riscv_iommu_endpoint *ep = dev_iommu_priv_get(dev);
	struct riscv_iommu_command cmd;
	u8 resp_code;
	unsigned long payload;

	switch (msg->code) {
	case IOMMU_PAGE_RESP_SUCCESS:
		resp_code = 0b0000;
		break;
	case IOMMU_PAGE_RESP_INVALID:
		resp_code = 0b0001;
		break;
	case IOMMU_PAGE_RESP_FAILURE:
		resp_code = 0b1111;
		break;
	}
	payload = riscv_iommu_ats_prgr_payload(ep->devid, resp_code, msg->grpid);

	/* ATS Page Request Group Response */
	riscv_iommu_cmd_ats_prgr(&cmd);
	riscv_iommu_cmd_ats_set_dseg(&cmd, ep->domid);
	riscv_iommu_cmd_ats_set_rid(&cmd, ep->devid);
	if (msg->flags & IOMMU_PAGE_RESP_PASID_VALID)
		riscv_iommu_cmd_ats_set_pid(&cmd, msg->pasid);
	riscv_iommu_cmd_ats_set_payload(&cmd, payload);
	riscv_iommu_post(ep->iommu, &cmd);

	return 0;
}

static void riscv_iommu_page_request(struct riscv_iommu_device *iommu,
				     struct riscv_iommu_pq_record *req)
{
	struct iommu_fault_event event = { 0 };
	struct iommu_fault_page_request *prm = &event.fault.prm;
	int ret;
	struct device *dev;
	unsigned devid = FIELD_GET(RISCV_IOMMU_PREQ_HDR_DID, req->hdr);

	/* Ignore PGR Stop marker. */
	if ((req->payload & RISCV_IOMMU_PREQ_PAYLOAD_M) == RISCV_IOMMU_PREQ_PAYLOAD_L)
		return;

	dev = riscv_iommu_get_device(iommu, devid);
	if (!dev) {
		/* TODO: Handle invalid page request */
		return;
	}

	event.fault.type = IOMMU_FAULT_PAGE_REQ;

	if (req->payload & RISCV_IOMMU_PREQ_PAYLOAD_L)
		prm->flags |= IOMMU_FAULT_PAGE_REQUEST_LAST_PAGE;
	if (req->payload & RISCV_IOMMU_PREQ_PAYLOAD_W)
		prm->perm |= IOMMU_FAULT_PERM_WRITE;
	if (req->payload & RISCV_IOMMU_PREQ_PAYLOAD_R)
		prm->perm |= IOMMU_FAULT_PERM_READ;

	prm->grpid = FIELD_GET(RISCV_IOMMU_PREQ_PRG_INDEX, req->payload);
	prm->addr = FIELD_GET(RISCV_IOMMU_PREQ_UADDR, req->payload) << PAGE_SHIFT;

	if (req->hdr & RISCV_IOMMU_PREQ_HDR_PV) {
		prm->flags |= IOMMU_FAULT_PAGE_REQUEST_PASID_VALID;
		/* TODO: where to find this bit */
		prm->flags |= IOMMU_FAULT_PAGE_RESPONSE_NEEDS_PASID;
		prm->pasid = FIELD_GET(RISCV_IOMMU_PREQ_HDR_PID, req->hdr);
	}

	ret = iommu_report_device_fault(dev, &event);
	if (ret) {
		struct iommu_page_response resp = {
			.grpid = prm->grpid,
			.code = IOMMU_PAGE_RESP_FAILURE,
		};
		if (prm->flags & IOMMU_FAULT_PAGE_RESPONSE_NEEDS_PASID) {
			resp.flags |= IOMMU_PAGE_RESP_PASID_VALID;
			resp.pasid = prm->pasid;
		}
		riscv_iommu_ats_prgr(dev, &resp);
	}

	put_device(dev);
}

static int riscv_iommu_page_response(struct device *dev,
				     struct iommu_fault_event *evt,
				     struct iommu_page_response *msg)
{
	return riscv_iommu_ats_prgr(dev, msg);
}

/* Page request interface queue primary interrupt handler */
static irqreturn_t riscv_iommu_priq_irq_check(int irq, void *data)
{
	struct riscv_iommu_queue *q = (struct riscv_iommu_queue *)data;
	struct riscv_iommu_device *iommu =
	    container_of(q, struct riscv_iommu_device, priq);
	u32 ipsr = riscv_iommu_readl(iommu, RISCV_IOMMU_REG_IPSR);
	if (ipsr & RISCV_IOMMU_IPSR_PIP)
		return IRQ_WAKE_THREAD;
	return IRQ_NONE;
}

/* Page request interface queue interrupt hanlder thread function */
static irqreturn_t riscv_iommu_priq_process(int irq, void *data)
{
	struct riscv_iommu_queue *q = (struct riscv_iommu_queue *)data;
	struct riscv_iommu_device *iommu;
	struct riscv_iommu_pq_record *requests;
	unsigned cnt, len, idx, ctrl;

	iommu = container_of(q, struct riscv_iommu_device, priq);
	requests = (struct riscv_iommu_pq_record *)q->base;

	/* Error reporting, clear error reports if any. */
	ctrl = riscv_iommu_readl(iommu, RISCV_IOMMU_REG_PQCSR);
	if (ctrl & (RISCV_IOMMU_PQCSR_PQMF | RISCV_IOMMU_PQCSR_PQOF)) {
		riscv_iommu_queue_ctrl(iommu, &iommu->priq, ctrl);
		dev_warn_ratelimited(iommu->dev,
				     "Page request queue error: fault: %d full: %d\n",
				     !!(ctrl & RISCV_IOMMU_PQCSR_PQMF),
				     !!(ctrl & RISCV_IOMMU_PQCSR_PQOF));
	}

	/* Clear page request interrupt pending. */
	riscv_iommu_writel(iommu, RISCV_IOMMU_REG_IPSR, RISCV_IOMMU_IPSR_PIP);

	/* Process page requests. */
	do {
		cnt = riscv_iommu_queue_consume(iommu, q, &idx);
		if (!cnt)
			break;
		for (len = 0; len < cnt; idx++, len++)
			riscv_iommu_page_request(iommu, &requests[idx]);
		riscv_iommu_queue_release(iommu, q, cnt);
	} while (1);

	return IRQ_HANDLED;
}

/*
 * Endpoint management
 */

static int riscv_iommu_enable_ir(struct riscv_iommu_endpoint *ep)
{
	struct riscv_iommu_device *iommu = ep->iommu;
	struct iommu_resv_region *entry;
	struct irq_domain *msi_domain;
	u64 val;
	int i;

	/* Initialize MSI remapping */
	if (!ep->dc || !(iommu->cap & RISCV_IOMMU_CAP_MSI_FLAT))
		return 0;

	ep->msi_root = (struct riscv_iommu_msi_pte *)get_zeroed_page(GFP_KERNEL);
	if (!ep->msi_root)
		return -ENOMEM;

	for (i = 0; i < 256; i++) {
		ep->msi_root[i].pte = RISCV_IOMMU_MSI_PTE_V |
		    FIELD_PREP(RISCV_IOMMU_MSI_PTE_M, 3) |
		    phys_to_ppn(RISCV_IMSIC_BASE + i * PAGE_SIZE);
	}

	// TODO: get from irq_domain data.
	entry = iommu_alloc_resv_region(RISCV_IMSIC_BASE, PAGE_SIZE * 256, 0,
					IOMMU_RESV_SW_MSI, GFP_KERNEL);
	if (entry)
		list_add_tail(&entry->list, &ep->regions);

	val = virt_to_pfn(ep->msi_root) |
	    FIELD_PREP(RISCV_IOMMU_DC_MSIPTP_MODE, RISCV_IOMMU_DC_MSIPTP_MODE_FLAT);
	ep->dc->msiptp = cpu_to_le64(val);
	/* Single page of MSIPTP, 256 IMSIC files */
	ep->dc->msi_addr_mask = cpu_to_le64(255);
	ep->dc->msi_addr_pattern = cpu_to_le64(RISCV_IMSIC_BASE >> 12);
	wmb();

	// set msi domain for the device as isolated. hack.
	msi_domain = dev_get_msi_domain(ep->dev);
	if (msi_domain) {
		msi_domain->flags |= IRQ_DOMAIN_FLAG_ISOLATED_MSI;
	}

	dev_dbg(ep->dev, "RV-IR enabled\n");

	ep->ir_enabled = true;

	return 0;
}

static void riscv_iommu_disable_ir(struct riscv_iommu_endpoint *ep)
{
	if (!ep->ir_enabled)
		return;

	ep->dc->msi_addr_pattern = 0ULL;
	ep->dc->msi_addr_mask = 0ULL;
	ep->dc->msiptp = 0ULL;
	wmb();

	dev_dbg(ep->dev, "RV-IR disabled\n");

	free_pages((unsigned long)ep->msi_root, 0);
	ep->msi_root = NULL;
	ep->ir_enabled = false;
}

/* Endpoint features/capabilities */
static void riscv_iommu_disable_ep(struct riscv_iommu_endpoint *ep)
{
	struct pci_dev *pdev;

	if (!dev_is_pci(ep->dev))
		return;

	pdev = to_pci_dev(ep->dev);

	if (ep->pasid_enabled) {
		pci_disable_ats(pdev);
		pci_disable_pri(pdev);
		pci_disable_pasid(pdev);
		ep->pasid_enabled = false;
	}
}

static void riscv_iommu_enable_ep(struct riscv_iommu_endpoint *ep)
{
	int rc, feat, num;
	struct pci_dev *pdev;
	struct device *dev = ep->dev;

	if (!dev_is_pci(dev))
		return;

	pdev = to_pci_dev(dev);

	/* TODO: Check IOMMU Capabilities before enabling ATS/PRI/PASID for devices */

	if (!pci_ats_supported(pdev))
		return;

	if (!pci_pri_supported(pdev))
		return;

	feat = pci_pasid_features(pdev);
	if (feat < 0)
		return;

	num = pci_max_pasids(pdev);
	if (!num) {
		dev_warn(dev, "Can't enable PASID (num: %d)\n", num);
		return;
	}

	rc = pci_enable_pasid(pdev, feat);
	if (rc) {
		dev_warn(dev, "Can't enable PASID (rc: %d)\n", rc);
		return;
	}

	rc = pci_reset_pri(pdev);
	if (rc) {
		dev_warn(dev, "Can't reset PRI (rc: %d)\n", rc);
		pci_disable_pasid(pdev);
		return;
	}

	/* TODO: Get supported PRI queue length, hard-code to 32 entries */
	rc = pci_enable_pri(pdev, 32);
	if (rc) {
		dev_warn(dev, "Can't enable PRI (rc: %d)\n", rc);
		pci_disable_pasid(pdev);
		return;
	}

	rc = pci_enable_ats(pdev, PAGE_SHIFT);
	if (rc) {
		dev_warn(dev, "Can't enable ATS (rc: %d)\n", rc);
		pci_disable_pri(pdev);
		pci_disable_pasid(pdev);
		return;
	}

	ep->pasid_enabled = true;
	ep->pasid_feat = feat;
	ep->pasid_bits = ilog2(num);

	dev_dbg(ep->dev, "PASID/ATS support enabled, %d bits\n", ep->pasid_bits);
}

static int riscv_iommu_enable_sva(struct device *dev)
{
	int ret;
	struct riscv_iommu_endpoint *ep = dev_iommu_priv_get(dev);

	if (!ep || !ep->iommu || !ep->iommu->pq_work)
		return -EINVAL;

	if (!ep->pasid_enabled)
		return -ENODEV;

	ret = iopf_queue_add_device(ep->iommu->pq_work, dev);
	if (ret)
		return ret;

	return iommu_register_device_fault_handler(dev, iommu_queue_iopf, dev);
}

static int riscv_iommu_disable_sva(struct device *dev)
{
	int ret;
	struct riscv_iommu_endpoint *ep = dev_iommu_priv_get(dev);

	ret = iommu_unregister_device_fault_handler(dev);
	if (!ret)
		ret = iopf_queue_remove_device(ep->iommu->pq_work, dev);

	return ret;
}

static int riscv_iommu_enable_iopf(struct device *dev)
{
	struct riscv_iommu_endpoint *ep = dev_iommu_priv_get(dev);

	if (ep && ep->pasid_enabled)
		return 0;

	return -EINVAL;
}

static int riscv_iommu_dev_enable_feat(struct device *dev, enum iommu_dev_features feat)
{
	switch (feat) {
	case IOMMU_DEV_FEAT_IOPF:
		return riscv_iommu_enable_iopf(dev);

	case IOMMU_DEV_FEAT_SVA:
		return riscv_iommu_enable_sva(dev);

	default:
		return -ENODEV;
	}
}

static int riscv_iommu_dev_disable_feat(struct device *dev, enum iommu_dev_features feat)
{
	switch (feat) {
	case IOMMU_DEV_FEAT_IOPF:
		return 0;

	case IOMMU_DEV_FEAT_SVA:
		return riscv_iommu_disable_sva(dev);

	default:
		return -ENODEV;
	}
}

static int riscv_iommu_of_xlate(struct device *dev, struct of_phandle_args *args)
{
	return iommu_fwspec_add_ids(dev, args->args, 1);
}

static bool riscv_iommu_capable(struct device *dev, enum iommu_cap cap)
{
	switch (cap) {
	case IOMMU_CAP_CACHE_COHERENCY:
	case IOMMU_CAP_PRE_BOOT_PROTECTION:
		return true;

	default:
		break;
	}

	return false;
}

/* TODO: implement proper device context management, e.g. teardown flow */

/* Lookup or initialize device directory info structure. */
static struct riscv_iommu_dc *riscv_iommu_get_dc(struct riscv_iommu_device *iommu,
						 unsigned devid)
{
	const bool base_format = !(iommu->cap & RISCV_IOMMU_CAP_MSI_FLAT);
	unsigned depth = iommu->ddt_mode - RISCV_IOMMU_DDTP_MODE_1LVL;
	u8 ddi_bits[3] = { 0 };
	u64 *ddtp = NULL, ddt;

	if (iommu->ddt_mode == RISCV_IOMMU_DDTP_MODE_OFF ||
	    iommu->ddt_mode == RISCV_IOMMU_DDTP_MODE_BARE)
		return NULL;

	/* Make sure the mode is valid */
	if (iommu->ddt_mode > RISCV_IOMMU_DDTP_MODE_MAX)
		return NULL;

	/*
	 * Device id partitioning for base format:
	 * DDI[0]: bits 0 - 6   (1st level) (7 bits)
	 * DDI[1]: bits 7 - 15  (2nd level) (9 bits)
	 * DDI[2]: bits 16 - 23 (3rd level) (8 bits)
	 *
	 * For extended format:
	 * DDI[0]: bits 0 - 5   (1st level) (6 bits)
	 * DDI[1]: bits 6 - 14  (2nd level) (9 bits)
	 * DDI[2]: bits 15 - 23 (3rd level) (9 bits)
	 */
	if (base_format) {
		ddi_bits[0] = 7;
		ddi_bits[1] = 7 + 9;
		ddi_bits[2] = 7 + 9 + 8;
	} else {
		ddi_bits[0] = 6;
		ddi_bits[1] = 6 + 9;
		ddi_bits[2] = 6 + 9 + 9;
	}

	/* Make sure device id is within range */
	if (devid >= (1 << ddi_bits[depth]))
		return NULL;

	/* Get to the level of the non-leaf node that holds the device context */
	for (ddtp = (u64 *) iommu->ddtp; depth-- > 0;) {
		const int split = ddi_bits[depth];
		/*
		 * Each non-leaf node is 64bits wide and on each level
		 * nodes are indexed by DDI[depth].
		 */
		ddtp += (devid >> split) & 0x1FF;

	 retry:
		/*
		 * Check if this node has been populated and if not
		 * allocate a new level and populate it.
		 */
		ddt = READ_ONCE(*ddtp);
		if (ddt & RISCV_IOMMU_DDTE_VALID) {
			ddtp = __va(ppn_to_phys(ddt));
		} else {
			u64 old, new = get_zeroed_page(GFP_KERNEL);
			if (!new)
				return NULL;

			old = cmpxchg64_relaxed(ddtp, ddt,
						phys_to_ppn(__pa(new)) |
						RISCV_IOMMU_DDTE_VALID);

			if (old != ddt) {
				free_page(new);
				goto retry;
			}

			ddtp = (u64 *) new;
		}
	}

	/*
	 * Grab the node that matches DDI[depth], note that when using base
	 * format the device context is 4 * 64bits, and the extended format
	 * is 8 * 64bits, hence the (3 - base_format) below.
	 */
	ddtp += (devid & ((64 << base_format) - 1)) << (3 - base_format);
	return (struct riscv_iommu_dc *)ddtp;
}

static struct iommu_device *riscv_iommu_probe_device(struct device *dev)
{
	struct riscv_iommu_device *iommu;
	struct riscv_iommu_endpoint *ep;
	struct iommu_fwspec *fwspec;

	fwspec = dev_iommu_fwspec_get(dev);
	if (!fwspec || fwspec->ops != &riscv_iommu_ops ||
	    !fwspec->iommu_fwnode || !fwspec->iommu_fwnode->dev)
		return ERR_PTR(-ENODEV);

	iommu = dev_get_drvdata(fwspec->iommu_fwnode->dev);
	if (!iommu)
		return ERR_PTR(-ENODEV);

	if (dev_iommu_priv_get(dev))
		return &iommu->iommu;

	ep = kzalloc(sizeof(*ep), GFP_KERNEL);
	if (!ep)
		return ERR_PTR(-ENOMEM);

	mutex_init(&ep->lock);
	INIT_LIST_HEAD(&ep->regions);
	INIT_LIST_HEAD(&ep->regions);

	if (dev_is_pci(dev)) {
		ep->devid = pci_dev_id(to_pci_dev(dev));
		ep->domid = pci_domain_nr(to_pci_dev(dev)->bus);
	} else {
		/* TODO: Make this generic, for now hardcode domain id to 0 */
		ep->devid = fwspec->ids[0];
		ep->domid = 0;
	}

	ep->iommu = iommu;
	ep->dev = dev;

	/* Initial DC pointer can be NULL if IOMMU is configured in OFF or BARE mode */
	ep->dc = riscv_iommu_get_dc(iommu, ep->devid);

	dev_dbg(iommu->dev, "adding device to iommu with devid %i in domain %i\n",
		ep->devid, ep->domid);

	dev_iommu_priv_set(dev, ep);
	riscv_iommu_add_device(iommu, dev);
	riscv_iommu_enable_ep(ep);
	riscv_iommu_enable_ir(ep);

	return &iommu->iommu;
}

static void riscv_iommu_probe_finalize(struct device *dev)
{
	set_dma_ops(dev, NULL);
	iommu_setup_dma_ops(dev, 0, U64_MAX);
}

static void riscv_iommu_detach_endpoint(struct riscv_iommu_endpoint *ep)
{
	mutex_lock(&ep->lock);
	ep->domain = NULL;
	list_del(&ep->domains);
	mutex_unlock(&ep->lock);

	riscv_iommu_iodir_inv_devid(ep->iommu, ep->devid);
}

static void riscv_iommu_release_device(struct device *dev)
{
	struct riscv_iommu_endpoint *ep = dev_iommu_priv_get(dev);
	struct riscv_iommu_device *iommu = ep->iommu;

	dev_dbg(dev, "device with devid %i released\n", ep->devid);

	if (ep->domain) {
		riscv_iommu_detach_endpoint(ep);
	}

	if (ep->dc) {
		// this should be already done by domain detach.
		ep->dc->tc = 0ULL;
		wmb();
		ep->dc->fsc = 0ULL;
		ep->dc->iohgatp = 0ULL;
		wmb();
		riscv_iommu_iodir_inv_devid(iommu, ep->devid);
	}

	riscv_iommu_disable_ir(ep);
	riscv_iommu_disable_ep(ep);

	/* Remove endpoint from IOMMU tracking structures */
	mutex_lock(&iommu->eps_mutex);
	rb_erase(&ep->node, &iommu->eps);
	mutex_unlock(&iommu->eps_mutex);

	set_dma_ops(dev, NULL);
	dev_iommu_priv_set(dev, NULL);

	kfree(ep);
}

static struct iommu_group *riscv_iommu_device_group(struct device *dev)
{
	if (dev_is_pci(dev))
		return pci_device_group(dev);
	return generic_device_group(dev);
}

static void riscv_iommu_get_resv_regions(struct device *dev, struct list_head *head)
{
	struct iommu_resv_region *entry, *new_entry;
	struct riscv_iommu_endpoint *ep = dev_iommu_priv_get(dev);

	list_for_each_entry(entry, &ep->regions, list) {
		new_entry = kmemdup(entry, sizeof(*entry), GFP_KERNEL);
		if (new_entry)
			list_add_tail(&new_entry->list, head);
	}

	iommu_dma_get_resv_regions(dev, head);
}

/*
 * Domain management
 */

/*
 * TODO: move to local (private) allocators for VMID / PSCID
 *  allocate GSCID unique PSCID
 */
static ioasid_t iommu_sva_alloc_pscid(void)
{
	/* TODO: Provide anonymous pasid value */
	struct mm_struct mm = {
		.pasid = IOMMU_PASID_INVALID,
	};

	if (iommu_sva_alloc_pasid(&mm, 1, (1 << 20) - 1))
		return IOMMU_PASID_INVALID;

	return mm.pasid;
}

static struct iommu_domain *riscv_iommu_domain_alloc(unsigned type)
{
	struct riscv_iommu_domain *domain;

	if (type != IOMMU_DOMAIN_DMA &&
	    type != IOMMU_DOMAIN_DMA_FQ &&
	    type != IOMMU_DOMAIN_UNMANAGED &&
	    type != IOMMU_DOMAIN_IDENTITY &&
	    type != IOMMU_DOMAIN_BLOCKED && type != IOMMU_DOMAIN_SVA)
		return NULL;

	domain = kzalloc(sizeof(*domain), GFP_KERNEL);
	if (!domain)
		return NULL;

	mutex_init(&domain->lock);
	INIT_LIST_HEAD(&domain->endpoints);
	domain->domain.ops = &riscv_iommu_domain_ops;
	domain->mode = RISCV_IOMMU_DC_FSC_MODE_BARE;

	return &domain->domain;
}

static void riscv_iommu_domain_free_endpoints(struct riscv_iommu_domain *domain)
{
	struct riscv_iommu_endpoint *ep;

	mutex_lock(&domain->lock);

	while (!list_empty(&domain->endpoints)) {
		ep = list_first_entry(&domain->endpoints,
				      struct riscv_iommu_endpoint, domains);
		BUG_ON(!ep->domain);
		riscv_iommu_detach_endpoint(ep);
	}

	mutex_unlock(&domain->lock);
}

static void riscv_iommu_domain_free(struct iommu_domain *iommu_domain)
{
	struct riscv_iommu_domain *domain = iommu_domain_to_riscv(iommu_domain);

	riscv_iommu_domain_free_endpoints(domain);

	if (domain->mn.ops && iommu_domain->mm)
		mmu_notifier_unregister(&domain->mn, iommu_domain->mm);

	if (domain->pgtbl.cookie)
		free_io_pgtable_ops(&domain->pgtbl.ops);

	if (domain->pgd_root)
		free_pages((unsigned long)domain->pgd_root, domain->g_stage ? 2 : 0);

	kfree(domain);
}

static int riscv_iommu_domain_finalize(struct riscv_iommu_domain *domain,
				       struct riscv_iommu_device *iommu)
{
	struct iommu_domain_geometry *geometry;

	/* Domain assigned to another iommu */
	if (domain->iommu && domain->iommu != iommu)
		return -EINVAL;
	/* Domain already initialized */
	else if (domain->iommu)
		return 0;

	/*
	 * TODO: Before using VA_BITS and satp_mode here, verify they
	 * are supported by the iommu, through the capabilities register.
	 */

	geometry = &domain->domain.geometry;

	/*
	 * Note: RISC-V Privilege spec mandates that virtual addresses
	 * need to be sign-extended, so if (VA_BITS - 1) is set, all
	 * bits >= VA_BITS need to also be set or else we'll get a
	 * page fault. However the code that creates the mappings
	 * above us (e.g. iommu_dma_alloc_iova()) won't do that for us
	 * for now, so we'll end up with invalid virtual addresses
	 * to map. As a workaround until we get this sorted out
	 * limit the available virtual addresses to VA_BITS - 1.
	 */
	geometry->aperture_start = 0;
	geometry->aperture_end = DMA_BIT_MASK(VA_BITS - 1);
	geometry->force_aperture = true;

	domain->iommu = iommu;

	if (domain->domain.type == IOMMU_DOMAIN_IDENTITY)
		return 0;

	/* TODO:
	 * Implement proper GSCID and PSCID allocator based on
	 * max allowed identifier widths (16, 20 bit per spec).
	 * Optional set 1:1 mapping for GSCID:VMID and PSCID:PASID.
	 */
	domain->id = iommu_sva_alloc_pscid();

	/* TODO: Fix this for RV32 */
	domain->mode = satp_mode >> 60;
	domain->pgd_root = (pgd_t *) __get_free_pages(GFP_KERNEL | __GFP_ZERO,
						      domain->g_stage ? 2 : 0);

	if (!domain->pgd_root)
		return -ENOMEM;

	if (!alloc_io_pgtable_ops(RISCV_IOMMU, &domain->pgtbl.cfg, domain))
		return -ENOMEM;

	return 0;
}

static u64 riscv_iommu_domain_atp(struct riscv_iommu_domain *domain)
{
	u64 atp = FIELD_PREP(RISCV_IOMMU_DC_FSC_MODE, domain->mode);
	if (domain->mode != RISCV_IOMMU_DC_FSC_MODE_BARE)
		atp |= FIELD_PREP(RISCV_IOMMU_DC_FSC_PPN, virt_to_pfn(domain->pgd_root));
	if (domain->g_stage)
		atp |= FIELD_PREP(RISCV_IOMMU_DC_IOHGATP_GSCID, domain->id);
	return atp;
}

static int riscv_iommu_attach_dev(struct iommu_domain *iommu_domain, struct device *dev)
{
	struct riscv_iommu_domain *domain = iommu_domain_to_riscv(iommu_domain);
	struct riscv_iommu_endpoint *ep = dev_iommu_priv_get(dev);
	struct riscv_iommu_dc *dc = ep->dc;
	int ret;
	u64 val;

	if (!dc)
		return -ENODEV;

	mutex_lock(&domain->lock);
	mutex_lock(&ep->lock);

	/* allocate root pages, initialize io-pgtable ops, etc. */
	ret = riscv_iommu_domain_finalize(domain, ep->iommu);
	if (ret < 0) {
		dev_err(dev, "can not finalize domain: %d\n", ret);
		mutex_unlock(&ep->lock);
		mutex_unlock(&domain->lock);
		return ret;
	}

	dev_info(dev, "domain id %u %s type %d attach\n", domain->id,
		 domain->g_stage ? "G-Stage" : "S-Stage", domain->domain.type);

	/*
	 * Update device context:
	 *  G-Stage : only if domain->g_stage
	 *  S-Stage : only if domain->s_stage. rely on OS not to update S-Stage while G-Stage valid.
	 */
	if (domain->g_stage) {
		/*
		 * Enable G-Stage translation with initial pass-through mode
		 * for S-Stage. VMM is responsible for more restrictive
		 * guest VA translation scheme configuration.
		 */
		dc->iohgatp = cpu_to_le64(riscv_iommu_domain_atp(domain));
		dc->fsc = 0ULL; /* RISCV_IOMMU_DC_FSC_MODE_BARE */ ;
	} else {
		/*
		 * S-Stage translation table does not ammend G-Stage.
		 */
		val = FIELD_PREP(RISCV_IOMMU_DC_TA_PSCID, domain->id);
		dc->ta = cpu_to_le64(val);
		dc->fsc = cpu_to_le64(riscv_iommu_domain_atp(domain));
	}
	wmb();
	/*
	 * Mark device context as valid, synchronise device context cache.
	 */
	val = RISCV_IOMMU_DC_TC_V;
	if (ep->pasid_enabled)
		val |= RISCV_IOMMU_DC_TC_EN_ATS | RISCV_IOMMU_DC_TC_EN_PRI;

	/* Enable QEMU custom extension for auto-pri generation */
	if (ep->pasid_enabled)
		val |= 1ULL << 32;

	if (ep->iommu->cap & RISCV_IOMMU_CAP_AMO)
		val |= RISCV_IOMMU_DC_TC_GADE | RISCV_IOMMU_DC_TC_SADE;
	dc->tc = cpu_to_le64(val);
	wmb();
	ep->domain = domain;

	list_add_tail(&ep->domains, &domain->endpoints);
	mutex_unlock(&ep->lock);
	mutex_unlock(&domain->lock);
	riscv_iommu_iodir_inv_devid(ep->iommu, ep->devid);

	return 0;
}

static int riscv_iommu_set_dev_pasid(struct iommu_domain *iommu_domain,
				     struct device *dev, ioasid_t pasid)
{
	struct riscv_iommu_domain *domain = iommu_domain_to_riscv(iommu_domain);
	struct riscv_iommu_endpoint *ep = dev_iommu_priv_get(dev);
	struct riscv_iommu_dc *dc = ep->dc;
	struct riscv_iommu_pc *pc = ep->pc;
	struct mm_struct *mm;

	if (!iommu_domain || !iommu_domain->mm)
		return -EINVAL;
	if (!ep || !ep->pasid_enabled || !dc)
		return -ENODEV;
	if (!pc)
		pc = (struct riscv_iommu_pc *)get_zeroed_page(GFP_KERNEL);
	if (!pc)
		return -ENOMEM;

	/* register mm notifier */
	mm = iommu_domain->mm;
	domain->pscid = pasid;
	domain->iommu = ep->iommu;
	domain->mn.ops = &riscv_iommu_mmuops;

	if (mmu_notifier_register(&domain->mn, mm))
		return -ENODEV;

	mutex_lock(&domain->lock);

	/* Use PASID for PSCID tag */
	pc[pasid].ta = cpu_to_le64(FIELD_PREP(RISCV_IOMMU_PC_TA_PSCID, pasid) |
				   RISCV_IOMMU_PC_TA_V);

	/* TODO: get SXL value for the process, use 32 bit or SATP mode */
	pc[pasid].fsc = cpu_to_le64(virt_to_pfn(mm->pgd) | satp_mode);

	/* update DC with sva->mm */
	if (!(ep->dc->tc & RISCV_IOMMU_DC_TC_PDTV)) {
		/* migrate to PD, domain mappings moved to PASID:0 */
		pc[0].ta = dc->ta;
		pc[0].fsc = dc->fsc;
		dc->fsc = cpu_to_le64(virt_to_pfn(pc) |
		    FIELD_PREP(RISCV_IOMMU_DC_FSC_MODE, RISCV_IOMMU_DC_FSC_PDTP_MODE_PD8));
		dc->tc |= cpu_to_le64(RISCV_IOMMU_DC_TC_PDTV | RISCV_IOMMU_DC_TC_DPE);
		ep->pc = pc;
		wmb();

		/* TODO: transition to PD steps */
		riscv_iommu_iodir_inv_devid(ep->iommu, ep->devid);
	} else {
		wmb();
		riscv_iommu_iodir_inv_pasid(ep->iommu, ep->devid, pasid);
	}

	mutex_unlock(&domain->lock);
	riscv_iommu_iofence_sync(ep->iommu);

	return 0;
}

static void riscv_iommu_remove_dev_pasid(struct device *dev, ioasid_t pasid)
{
	struct riscv_iommu_endpoint *ep = dev_iommu_priv_get(dev);
	struct riscv_iommu_command cmd;
	unsigned long payload = riscv_iommu_ats_inval_all_payload(false);

	/* remove SVA iommu-domain */

	/* invalidate TA.V */
	ep->pc[pasid].ta = 0;
	wmb();

	/* 1. invalidate PDT entry */
	riscv_iommu_iodir_inv_pasid(ep->iommu, ep->devid, pasid);

	/* 2. invalidate all matching IOATC entries */
	riscv_iommu_cmd_inval_vma(&cmd);
	riscv_iommu_cmd_inval_set_gscid(&cmd, 0);
	riscv_iommu_cmd_inval_set_pscid(&cmd, pasid);
	riscv_iommu_post(ep->iommu, &cmd);

	/* 3. Wait IOATC flush to happen */
	riscv_iommu_iofence_sync(ep->iommu);

	/* 4. ATS invalidation */
	riscv_iommu_cmd_ats_inval(&cmd);
	riscv_iommu_cmd_ats_set_dseg(&cmd, ep->domid);
	riscv_iommu_cmd_ats_set_rid(&cmd, ep->devid);
	riscv_iommu_cmd_ats_set_pid(&cmd, pasid);
	riscv_iommu_cmd_ats_set_payload(&cmd, payload);
	riscv_iommu_post(ep->iommu, &cmd);

	/* 5. Wait DevATC flush to happen */
	riscv_iommu_iofence_sync(ep->iommu);
}

/* mark domain as second-stage translation */
static int riscv_iommu_enable_nesting(struct iommu_domain *iommu_domain)
{
	struct riscv_iommu_domain *domain = iommu_domain_to_riscv(iommu_domain);

	mutex_lock(&domain->lock);
	if (list_empty(&domain->endpoints))
		domain->g_stage = true;
	mutex_unlock(&domain->lock);

	return domain->g_stage ? 0 : -EBUSY;
}

static void riscv_iommu_flush_iotlb_range(struct iommu_domain *iommu_domain,
					  unsigned long *start, unsigned long *end,
					  size_t *pgsize)
{
	struct riscv_iommu_domain *domain = iommu_domain_to_riscv(iommu_domain);
	struct riscv_iommu_command cmd;
	struct riscv_iommu_endpoint *endpoint;
	// unsigned long iova;
	unsigned long payload;

	if (domain->mode == RISCV_IOMMU_DC_FSC_MODE_BARE)
		return;

	/* Domain not attached to an IOMMU! */
	BUG_ON(!domain->iommu);

	if (start && end) {
		payload = riscv_iommu_ats_inval_payload(*start, *end, true);
	} else {
		payload = riscv_iommu_ats_inval_all_payload(true);
	}

	if (domain->g_stage) {
		riscv_iommu_cmd_inval_gvma(&cmd);
		riscv_iommu_cmd_inval_set_gscid(&cmd, domain->id);
	} else {
		riscv_iommu_cmd_inval_vma(&cmd);
		// riscv_iommu_cmd_inval_set_pscid(&cmd, domain->id);
	}

	// if (start && end && pgsize) {
	// 	/* Cover only the range that is needed */
	// 	for (iova = *start; iova <= *end; iova += *pgsize) {
	// 		printk("Sending IOTINVAL with ADDR");
	// 		riscv_iommu_cmd_inval_set_addr(&cmd, iova);
	// 		riscv_iommu_post(domain->iommu, &cmd);
	// 	}
	// } else {
		riscv_iommu_post(domain->iommu, &cmd);
	// }
	riscv_iommu_iofence_sync(domain->iommu);

	/* ATS invalidation for every device and for every translation */
	list_for_each_entry(endpoint, &domain->endpoints, domains) {
		if (!endpoint->pasid_enabled)
			continue;

		riscv_iommu_cmd_ats_inval(&cmd);
		riscv_iommu_cmd_ats_set_dseg(&cmd, endpoint->domid);
		riscv_iommu_cmd_ats_set_rid(&cmd, endpoint->devid);
		riscv_iommu_cmd_ats_set_pid(&cmd, domain->pscid);
		riscv_iommu_cmd_ats_set_payload(&cmd, payload);
		riscv_iommu_post(domain->iommu, &cmd);
	}
	riscv_iommu_iofence_sync(domain->iommu);
}

static void riscv_iommu_flush_iotlb_all(struct iommu_domain *iommu_domain)
{
	riscv_iommu_flush_iotlb_range(iommu_domain, NULL, NULL, NULL);
}

static void riscv_iommu_iotlb_sync(struct iommu_domain *iommu_domain,
				   struct iommu_iotlb_gather *gather)
{
	riscv_iommu_flush_iotlb_range(iommu_domain, &gather->start, &gather->end,
				      &gather->pgsize);
}

static void riscv_iommu_iotlb_sync_map(struct iommu_domain *iommu_domain,
				       unsigned long iova, size_t size)
{
	unsigned long end = iova + size - 1;
	/*
	 * Given we don't know the page size used by this range, we assume the
	 * smallest page size to ensure all possible entries are flushed from
	 * the IOATC.
	 */
	size_t pgsize = PAGE_SIZE;
	riscv_iommu_flush_iotlb_range(iommu_domain, &iova, &end, &pgsize);
}

static int riscv_iommu_map_pages(struct iommu_domain *iommu_domain,
				 unsigned long iova, phys_addr_t phys,
				 size_t pgsize, size_t pgcount, int prot,
				 gfp_t gfp, size_t *mapped)
{
	struct riscv_iommu_domain *domain = iommu_domain_to_riscv(iommu_domain);

	pr_debug("Mapping IOVA %llx to SPA %llx\n", (unsigned long long)iova, phys);

	if (!domain->pgtbl.ops.map_pages)
		return -ENODEV;

	return domain->pgtbl.ops.map_pages(&domain->pgtbl.ops, iova, phys,
					   pgsize, pgcount, prot, gfp, mapped);
}

static size_t riscv_iommu_unmap_pages(struct iommu_domain *iommu_domain,
				      unsigned long iova, size_t pgsize,
				      size_t pgcount, struct iommu_iotlb_gather *gather)
{
	struct riscv_iommu_domain *domain = iommu_domain_to_riscv(iommu_domain);

	if (!domain->pgtbl.ops.unmap_pages)
		return 0;

	return domain->pgtbl.ops.unmap_pages(&domain->pgtbl.ops, iova, pgsize,
					     pgcount, gather);
}

static phys_addr_t riscv_iommu_iova_to_phys(struct iommu_domain *iommu_domain,
					    dma_addr_t iova)
{
	struct riscv_iommu_domain *domain = iommu_domain_to_riscv(iommu_domain);

	if (!domain->pgtbl.ops.iova_to_phys)
		return 0;

	return domain->pgtbl.ops.iova_to_phys(&domain->pgtbl.ops, iova);
}

/*
 * Translation mode setup
 */

static u64 riscv_iommu_get_ddtp(struct riscv_iommu_device *iommu)
{
	u64 ddtp;
	cycles_t end_cycles = RISCV_IOMMU_TIMEOUT + get_cycles();

	/* Wait for DDTP.BUSY to be cleared and return latest value */
	do {
		ddtp = riscv_iommu_readq(iommu, RISCV_IOMMU_REG_DDTP);
		if (!(ddtp & RISCV_IOMMU_DDTP_BUSY))
			break;
		cpu_relax();
	} while (get_cycles() < end_cycles);

	return ddtp;
}

static void riscv_iommu_ddt_cleanup(struct riscv_iommu_device *iommu)
{
	/* TODO: teardown whole device directory tree. */
	if (iommu->ddtp) {
		if (iommu->ddtp_in_iomem) {
			iounmap((void *)iommu->ddtp);
		} else
			free_page(iommu->ddtp);
		iommu->ddtp = 0;
	}
}

static int riscv_iommu_enable(struct riscv_iommu_device *iommu, unsigned requested_mode)
{
	struct device *dev = iommu->dev;
	u64 ddtp = 0;
	u64 ddtp_paddr = 0;
	unsigned mode = requested_mode;
	unsigned mode_readback = 0;

	ddtp = riscv_iommu_get_ddtp(iommu);
	if (ddtp & RISCV_IOMMU_DDTP_BUSY)
		return -EBUSY;

	/* Disallow state transtion from xLVL to xLVL. */
	// switch (FIELD_GET(RISCV_IOMMU_DDTP_MODE, ddtp)) {
	// case RISCV_IOMMU_DDTP_MODE_BARE:
	// case RISCV_IOMMU_DDTP_MODE_OFF:
	// 	break;
	// default:
	// 	if ((mode != RISCV_IOMMU_DDTP_MODE_BARE)
	// 	    && (mode != RISCV_IOMMU_DDTP_MODE_OFF))
	// 		return -EINVAL;
	// 	break;
	// }

 retry:
	switch (mode) {
	case RISCV_IOMMU_DDTP_MODE_BARE:
	case RISCV_IOMMU_DDTP_MODE_OFF:
		riscv_iommu_ddt_cleanup(iommu);
		ddtp = FIELD_PREP(RISCV_IOMMU_DDTP_MODE, mode);
		break;
	case RISCV_IOMMU_DDTP_MODE_1LVL:
	case RISCV_IOMMU_DDTP_MODE_2LVL:
	case RISCV_IOMMU_DDTP_MODE_3LVL:
		if (!iommu->ddtp) {
			/*
			 * We haven't initialized ddtp yet, since it's WARL make
			 * sure that we don't have a hardwired PPN field there
			 * that points to i/o memory instead.
			 */
			riscv_iommu_writeq(iommu, RISCV_IOMMU_REG_DDTP, 0);
			ddtp = riscv_iommu_get_ddtp(iommu);
			ddtp_paddr = ppn_to_phys(ddtp);
			if (ddtp_paddr) {
				dev_warn(dev, "ddtp at 0x%llx\n", ddtp_paddr);
				iommu->ddtp =
				    (unsigned long)ioremap(ddtp_paddr, PAGE_SIZE);
				iommu->ddtp_in_iomem = true;
			} else {
				iommu->ddtp = get_zeroed_page(GFP_KERNEL);
			}
		}
		if (!iommu->ddtp)
			return -ENOMEM;

		ddtp = FIELD_PREP(RISCV_IOMMU_DDTP_MODE, mode) |
		    phys_to_ppn(__pa(iommu->ddtp));

		break;
	default:
		return -EINVAL;
	}

	riscv_iommu_writeq(iommu, RISCV_IOMMU_REG_DDTP, ddtp);
	ddtp = riscv_iommu_get_ddtp(iommu);
	if (ddtp & RISCV_IOMMU_DDTP_BUSY) {
		dev_warn(dev, "timeout when setting ddtp (ddt mode: %i)\n", mode);
		return -EBUSY;
	}

	mode_readback = FIELD_GET(RISCV_IOMMU_DDTP_MODE, ddtp);
	dev_info(dev, "mode_readback: %i, mode: %i\n", mode_readback, mode);
	if (mode_readback != mode) {
		/*
		 * Mode field is WARL, an I/O MMU may support a subset of
		 * directory table levels in which case if we tried to set
		 * an unsupported number of levels we'll readback either
		 * a valid xLVL or off/bare. If we got off/bare, try again
		 * with a smaller xLVL.
		 */
		if (mode_readback < RISCV_IOMMU_DDTP_MODE_1LVL &&
		    mode > RISCV_IOMMU_DDTP_MODE_1LVL) {
			mode--;
			goto retry;
		}

		/*
		 * We tried all supported xLVL modes and still got off/bare instead,
		 * an I/O MMU must support at least one supported xLVL mode so something
		 * went very wrong.
		 */
		if (mode_readback < RISCV_IOMMU_DDTP_MODE_1LVL &&
		    mode == RISCV_IOMMU_DDTP_MODE_1LVL)
			goto fail;

		/*
		 * We tried setting off or bare and got something else back, something
		 * went very wrong since off/bare is always legal.
		 */
		if (mode < RISCV_IOMMU_DDTP_MODE_1LVL)
			goto fail;

		/*
		 * We tried setting an xLVL mode but got another xLVL mode that
		 * we don't support (e.g. a custom one).
		 */
		if (mode_readback > RISCV_IOMMU_DDTP_MODE_MAX)
			goto fail;

		/* We tried setting an xLVL mode but got another supported xLVL mode */
		mode = mode_readback;
	}

	if (mode != requested_mode)
		dev_warn(dev, "unsupported DDT mode requested (%i), using %i instead\n",
			 requested_mode, mode);

	iommu->ddt_mode = mode;
	dev_info(dev, "ddt_mode: %i\n", iommu->ddt_mode);
	return 0;

 fail:
	dev_err(dev, "failed to set DDT mode, tried: %i and got %i\n", mode,
		mode_readback);
	riscv_iommu_ddt_cleanup(iommu);
	return -EINVAL;
}

/*
 * Common I/O MMU driver probe/teardown
 */

static const struct iommu_domain_ops riscv_iommu_domain_ops = {
	.free = riscv_iommu_domain_free,
	.attach_dev = riscv_iommu_attach_dev,
	.set_dev_pasid = riscv_iommu_set_dev_pasid,
	.map_pages = riscv_iommu_map_pages,
	.unmap_pages = riscv_iommu_unmap_pages,
	.iova_to_phys = riscv_iommu_iova_to_phys,
	.iotlb_sync = riscv_iommu_iotlb_sync,
	.iotlb_sync_map = riscv_iommu_iotlb_sync_map,
	.flush_iotlb_all = riscv_iommu_flush_iotlb_all,
	.enable_nesting = riscv_iommu_enable_nesting,
};

static const struct iommu_ops riscv_iommu_ops = {
	.owner = THIS_MODULE,
	.pgsize_bitmap = SZ_4K | SZ_2M | SZ_512M,
	.capable = riscv_iommu_capable,
	.domain_alloc = riscv_iommu_domain_alloc,
	.probe_device = riscv_iommu_probe_device,
	.probe_finalize = riscv_iommu_probe_finalize,
	.release_device = riscv_iommu_release_device,
	.remove_dev_pasid = riscv_iommu_remove_dev_pasid,
	.device_group = riscv_iommu_device_group,
	.get_resv_regions = riscv_iommu_get_resv_regions,
	.of_xlate = riscv_iommu_of_xlate,
	.dev_enable_feat = riscv_iommu_dev_enable_feat,
	.dev_disable_feat = riscv_iommu_dev_disable_feat,
	.page_response = riscv_iommu_page_response,
	.default_domain_ops = &riscv_iommu_domain_ops,
};

struct riscv_iommu_endpoint *riscv_iommu_hpm_endpoint(struct device *dev)
{
	struct iommu_fwspec *fwspec;
	struct riscv_iommu_endpoint *ep;

	if (!dev)
		return NULL;
	fwspec = dev_iommu_fwspec_get(dev);
	if (!fwspec || fwspec->ops != &riscv_iommu_ops ||
	    fwspec->num_ids != 1 || dev_is_pci(dev))
		return NULL;

	ep = dev_iommu_priv_get(dev);
	if (!ep || ep->dev != dev || ep->devid != fwspec->ids[0] ||
	    ep->devid > FIELD_MAX(RISCV_IOMMU_IOHPMEVT_DID_GSCID))
		return NULL;
	return ep;
}

/*
 * Validate one acquire-time target under ep->lock. The acquire snapshot and
 * the endpoint's own domain must both still be the live "core" domain for
 * this device, the domain must belong to this IOMMU and use the ordinary
 * host S1 domain ops, and the device context must still match the domain's
 * live Sv39 root. Only plain ordinary (DMA or UNMANAGED) host first-stage
 * domains are accepted: SVA, identity, blocked, S2, nesting, PASID/PDTV and
 * the ATS/PRI/T2GPA/SXL DC bits are rejected. Called without domain->lock
 * (attach takes domain->lock before ep->lock), so domain->lock is never taken
 * here and no inversion is possible.
 */
static int riscv_iommu_flush_s1_check_ep(struct riscv_iommu_device *iommu,
					 struct riscv_iommu_endpoint *ep,
					 struct iommu_domain *snapshot)
{
	struct riscv_iommu_domain *domain;
	struct riscv_iommu_dc *dc;
	u64 tc, fsc, iohgatp;

	/*
	 * Resolve the device/domain identity before dereferencing the stored
	 * snapshot: the device must still report the same core domain and the
	 * endpoint must still point at that same domain object.
	 */
	if (!snapshot || iommu_get_domain_for_dev(ep->dev) != snapshot)
		return -ESTALE;
	if (!ep->domain || &ep->domain->domain != snapshot)
		return -ESTALE;

	domain = ep->domain;
	dc = ep->dc;

	if (domain->iommu != iommu)
		return -EXDEV;
	if (domain->domain.ops != &riscv_iommu_domain_ops)
		return -ESTALE;

	/*
	 * Ordinary host S1 only. IOMMU_DOMAIN_DMA / IOMMU_DOMAIN_UNMANAGED are
	 * the only accepted types; identity, blocked, SVA, DMA_FQ, S2, nesting
	 * and PASID are rejected as unsupported.
	 */
	if (domain->domain.type != IOMMU_DOMAIN_DMA &&
	    domain->domain.type != IOMMU_DOMAIN_UNMANAGED)
		return -EOPNOTSUPP;
	if (domain->g_stage || domain->nested || ep->pasid_enabled)
		return -EOPNOTSUPP;

	/*
	 * Sv39 requires IOSATP mode 8 with SXL == 0; mode 8 with SXL == 1
	 * would be Sv32 and is rejected through the DC check below.
	 */
	if (domain->mode != RISCV_IOMMU_DC_FSC_IOSATP_MODE_SV39)
		return -EOPNOTSUPP;
	if (!domain->pgd_root)
		return -EOPNOTSUPP;
	if (!dc)
		return -EOPNOTSUPP;

	tc = le64_to_cpu(dc->tc);
	fsc = le64_to_cpu(dc->fsc);
	iohgatp = le64_to_cpu(dc->iohgatp);

	if (!(tc & RISCV_IOMMU_DC_TC_V))
		return -EOPNOTSUPP;
	if (tc & (RISCV_IOMMU_DC_TC_EN_ATS | RISCV_IOMMU_DC_TC_EN_PRI |
		  RISCV_IOMMU_DC_TC_PDTV | RISCV_IOMMU_DC_TC_T2GPA |
		  RISCV_IOMMU_DC_TC_SXL))
		return -EOPNOTSUPP;
	if (FIELD_GET(RISCV_IOMMU_DC_IOHGATP_MODE, iohgatp) !=
	    RISCV_IOMMU_DC_IOHGATP_MODE_BARE)
		return -EOPNOTSUPP;

	/* The live device context must still describe this exact domain. */
	if (FIELD_GET(RISCV_IOMMU_DC_FSC_MODE, fsc) != domain->mode ||
	    FIELD_GET(RISCV_IOMMU_DC_FSC_PPN, fsc) !=
		    virt_to_pfn(domain->pgd_root))
		return -ESTALE;

	return 0;
}

/*
 * Global host first-stage invalidation barrier for one or two devices of the
 * same IOMMU. The caller owns the HPM session (hpm_lock), has quiesced the
 * relevant DMA and serializes binding changes. Both endpoint locks are held
 * across validation and command submission, taken in ascending device id
 * order, and no domain->lock is taken. This only issues IOTINVAL.VMA with
 * GV/AV/PSCV clear followed by a fence; it is not an ATS/PR/PW or general
 * internal-state reset, and it never retries or resets on failure.
 *
 * Note on the current RTL: CQH advances when the fetch of the last beat
 * completes, before decode. For this strict no-ATS/no-S2, quiesced
 * experiment the CQ decodes the earlier VMA (its invalidations are issued
 * before the fetch of the following IOFENCE), so CQH passing the following
 * fence is sufficient evidence that the VMA pulse was issued. This is not a
 * general fence-execution proof.
 */
int riscv_iommu_flush_s1_checked(struct riscv_iommu_device *iommu,
				 const struct riscv_iommu_s1_flush_target *targets,
				 unsigned int nr_targets)
{
	struct riscv_iommu_endpoint *ep[2] = { };
	struct riscv_iommu_command cmd;
	unsigned long flags;
	unsigned int first, second, i;
	u32 ctrl;
	u64 seq;
	int ret;

	if (!iommu || !targets || (nr_targets != 1 && nr_targets != 2))
		return -EINVAL;
	if (nr_targets == 2 &&
	    (targets[0].dev == targets[1].dev ||
	     targets[0].did == targets[1].did))
		return -EINVAL;

	for (i = 0; i < nr_targets; i++) {
		if (!targets[i].dev || !targets[i].domain)
			return -EINVAL;

		ep[i] = riscv_iommu_hpm_endpoint(targets[i].dev);
		if (!ep[i])
			return -EOPNOTSUPP;
		if (ep[i]->iommu != iommu)
			return -EXDEV;
		if (ep[i]->devid != targets[i].did)
			return -ESTALE;
	}
	if (nr_targets == 2 && ep[0] == ep[1])
		return -EINVAL;

	/* Take both endpoint mutexes, always in ascending device id order. */
	if (nr_targets == 2 && ep[0]->devid > ep[1]->devid) {
		first = 1;
		second = 0;
	} else {
		first = 0;
		second = nr_targets - 1;
	}
	mutex_lock(&ep[first]->lock);
	if (nr_targets == 2)
		mutex_lock_nested(&ep[second]->lock, SINGLE_DEPTH_NESTING);

	ret = 0;
	for (i = 0; i < nr_targets; i++) {
		/*
		 * Revalidate the endpoint identity under its lock, then let
		 * the core check compare the live domain against the stored
		 * acquire-time snapshot.
		 */
		if (ep[i]->dev != targets[i].dev ||
		    ep[i]->devid != targets[i].did ||
		    ep[i]->iommu != iommu) {
			ret = -ESTALE;
			break;
		}
		ret = riscv_iommu_flush_s1_check_ep(iommu, ep[i],
						    targets[i].domain);
		if (ret)
			break;
	}
	if (ret)
		goto unlock;

	/* Sample the error generation before looking at any CQ status. */
	seq = atomic64_read(&iommu->cq_error_seq);

	if (!iommu->cmdq.base || !iommu->cmdq.cnt ||
	    (iommu->cmdq.cnt & (iommu->cmdq.cnt - 1)) ||
	    iommu->cmdq.lui >= iommu->cmdq.cnt) {
		ret = -EIO;
		goto unlock;
	}

	ctrl = riscv_iommu_readl(iommu, RISCV_IOMMU_REG_CQCSR);
	if (!(ctrl & RISCV_IOMMU_CQCSR_CQEN) ||
	    !(ctrl & RISCV_IOMMU_CQCSR_CQON) ||
	    (ctrl & (RISCV_IOMMU_CQCSR_CQMF | RISCV_IOMMU_CQCSR_CMD_TO |
		     RISCV_IOMMU_CQCSR_CMD_ILL))) {
		ret = -EIO;
		goto unlock;
	}

	/*
	 * Producer consistency, not a queue-empty claim: under cq_lock the
	 * software write cursor must match the hardware tail. Reading and
	 * comparing under the same lock keeps a concurrent poster from
	 * invalidating the observation; cq_lock is dropped immediately after.
	 */
	spin_lock_irqsave(&iommu->cq_lock, flags);
	if ((riscv_iommu_readl(iommu, RISCV_IOMMU_REG_CQT) &
	     (iommu->cmdq.cnt - 1)) != iommu->cmdq.lui)
		ret = -EIO;
	spin_unlock_irqrestore(&iommu->cq_lock, flags);
	if (ret)
		goto unlock;

	/* IOTINVAL.VMA with GV, AV and PSCV clear: every host S1 entry. */
	riscv_iommu_cmd_inval_vma(&cmd);
	if (!riscv_iommu_post(iommu, &cmd)) {
		ret = -EIO;
		goto unlock;
	}
	if (!riscv_iommu_iofence_sync(iommu)) {
		ret = -EIO;
		goto unlock;
	}

	/*
	 * End state must still be enabled and error-free. rmb() orders the
	 * final CQCSR read before the generation read so an IRQ thread that
	 * published the error and then cleared CQCSR is observed.
	 */
	ctrl = riscv_iommu_readl(iommu, RISCV_IOMMU_REG_CQCSR);
	rmb();
	if ((ctrl & (RISCV_IOMMU_CQCSR_CQEN | RISCV_IOMMU_CQCSR_CQON)) !=
		    (RISCV_IOMMU_CQCSR_CQEN | RISCV_IOMMU_CQCSR_CQON) ||
	    (ctrl & (RISCV_IOMMU_CQCSR_CQMF | RISCV_IOMMU_CQCSR_CMD_TO |
		     RISCV_IOMMU_CQCSR_CMD_ILL)) ||
	    atomic64_read(&iommu->cq_error_seq) != seq) {
		ret = -EIO;
		goto unlock;
	}

unlock:
	if (nr_targets == 2)
		mutex_unlock(&ep[second]->lock);
	mutex_unlock(&ep[first]->lock);
	return ret;
}

void riscv_iommu_remove(struct riscv_iommu_device *iommu)
{
	/* An owner finishes restoring its HPM state before teardown proceeds. */
	mutex_lock(&iommu->hpm_lock);
	WRITE_ONCE(iommu->hpm_ready, false);
	mutex_unlock(&iommu->hpm_lock);
	iommu_device_unregister(&iommu->iommu);
	iommu_device_sysfs_remove(&iommu->iommu);
	riscv_iommu_enable(iommu, RISCV_IOMMU_DDTP_MODE_OFF);
	riscv_iommu_queue_free(iommu, &iommu->cmdq);
	riscv_iommu_queue_free(iommu, &iommu->fltq);
	riscv_iommu_queue_free(iommu, &iommu->priq);
	iopf_queue_free(iommu->pq_work);
}

int riscv_iommu_init(struct riscv_iommu_device *iommu)
{
	struct device *dev = iommu->dev;
	u32 fctl = 0;
	int ret;

	mutex_init(&iommu->hpm_lock);
	iommu->hpm_ready = false;
	iommu->eps = RB_ROOT;

	fctl = riscv_iommu_readl(iommu, RISCV_IOMMU_REG_FCTL);

#ifdef CONFIG_CPU_BIG_ENDIAN
	if (!(cap & RISCV_IOMMU_CAP_END)) {
		dev_err(dev, "IOMMU doesn't support Big Endian\n");
		return -EIO;
	} else if (!(fctl & RISCV_IOMMU_FCTL_BE)) {
		fctl |= FIELD_PREP(RISCV_IOMMU_FCTL_BE, 1);
		riscv_iommu_writel(iommu, RISCV_IOMMU_REG_FCTL, fctl);
	}
#endif

	if (iommu->cap & RISCV_IOMMU_CAP_PD20)
		iommu->iommu.max_pasids = 1u << 20;
	else if (iommu->cap & RISCV_IOMMU_CAP_PD17)
		iommu->iommu.max_pasids = 1u << 17;
	else if (iommu->cap & RISCV_IOMMU_CAP_PD8)
		iommu->iommu.max_pasids = 1u << 8;
	/*
	 * Assign queue lengths from module parameters if not already
	 * set on the device tree.
	 */
	if (!iommu->cmdq_len)
	{
		iommu->cmdq_len = cmdq_length;		
		dev_info(dev, "CQ length: %d\n", cmdq_length);
	}
	if (!iommu->fltq_len)
	{
		iommu->fltq_len = fltq_length;
		dev_info(dev, "FQ length: %d\n", fltq_length);
	}
	if (!iommu->priq_len)
		iommu->priq_len = priq_length;
	/* Clear any pending interrupt flag. */
	riscv_iommu_writel(iommu, RISCV_IOMMU_REG_IPSR,
			   RISCV_IOMMU_IPSR_CIP |
			   RISCV_IOMMU_IPSR_FIP |
			   RISCV_IOMMU_IPSR_PMIP | RISCV_IOMMU_IPSR_PIP);
	spin_lock_init(&iommu->cq_lock);
	mutex_init(&iommu->eps_mutex);
	/* Initialize the error generation before any queue IRQ is requested. */
	atomic64_set(&iommu->cq_error_seq, 0);
	ret = riscv_iommu_queue_init(iommu, RISCV_IOMMU_COMMAND_QUEUE);
	if (ret)
		goto fail;
	ret = riscv_iommu_queue_init(iommu, RISCV_IOMMU_FAULT_QUEUE);
	if (ret)
		goto fail;
	if (!(iommu->cap & RISCV_IOMMU_CAP_ATS))
		goto no_ats;

	/**** ATS enabled: Start ****/

	/* PRI functionally depends on ATS’s capabilities. */
	if (!iommu->priq_len)
	{
		iommu->priq_len = priq_length;
		dev_info(dev, "PQ length: %d\n", priq_length);
	}

	iommu->pq_work = iopf_queue_alloc(dev_name(dev));
	if (!iommu->pq_work) {
		dev_err(dev, "failed to allocate iopf queue\n");
		ret = -ENOMEM;
		goto fail;
	}

	ret = riscv_iommu_queue_init(iommu, RISCV_IOMMU_PAGE_REQUEST_QUEUE);
	if (ret)
		goto fail;

	/**** ATS enabled: End ****/

 no_ats:

	// HPM
	if (iommu->cap & RISCV_IOMMU_CAP_HPM) {
		// Program HPM counters
		// Cnt 0: Untranslated requests
		riscv_iommu_writeq(iommu, RISCV_IOMMU_REG_IOHPMEVT(0), 
								(RISCV_IOMMU_IOHPMEVT_EVENT_ID & RISCV_IOMMU_EVENT_UT_REQ) |
								RISCV_IOMMU_IOHPMEVT_OF);
		// Cnt 1: TLB Misses
		riscv_iommu_writeq(iommu, RISCV_IOMMU_REG_IOHPMEVT(1), 
								(RISCV_IOMMU_IOHPMEVT_EVENT_ID & RISCV_IOMMU_EVENT_TLB_MISS) |
								RISCV_IOMMU_IOHPMEVT_OF);
		// Cnt 2: DDT Walks
		riscv_iommu_writeq(iommu, RISCV_IOMMU_REG_IOHPMEVT(2), 
								(RISCV_IOMMU_IOHPMEVT_EVENT_ID & RISCV_IOMMU_EVENT_DDTW) |
								RISCV_IOMMU_IOHPMEVT_OF);
		// Cnt 3: S1 PT Walks
		riscv_iommu_writeq(iommu, RISCV_IOMMU_REG_IOHPMEVT(3), 
								(RISCV_IOMMU_IOHPMEVT_EVENT_ID & RISCV_IOMMU_EVENT_S1_PTW) |
								RISCV_IOMMU_IOHPMEVT_OF);
		// Cnt 4: S2 PT Walks
		riscv_iommu_writeq(iommu, RISCV_IOMMU_REG_IOHPMEVT(4), 
								(RISCV_IOMMU_IOHPMEVT_EVENT_ID & RISCV_IOMMU_EVENT_S2_PTW) |
								RISCV_IOMMU_IOHPMEVT_OF);

		// Enable counters (... 1111 1100 0001)
		riscv_iommu_writel(iommu, RISCV_IOMMU_REG_IOCOUNTINH,
											(u32)(~0x3EUL));
	}

	if (iommu_default_passthrough()) {
		dev_info(dev, "iommu set to passthrough mode\n");
		ret = riscv_iommu_enable(iommu, RISCV_IOMMU_DDTP_MODE_BARE);
	} else {
		ret = riscv_iommu_enable(iommu, ddt_mode);
	}

	if (ret) {
		dev_err(dev, "cannot enable iommu device (%d)\n", ret);
		goto fail;
	}

	ret = riscv_iommu_sysfs_add(iommu);
	if (ret) {
		dev_err(dev, "cannot register sysfs interface (%d)\n", ret);
		goto fail;
	}

	WRITE_ONCE(iommu->hpm_ready, true);
	ret = iommu_device_register(&iommu->iommu, &riscv_iommu_ops, dev);
	if (ret) {
		dev_err(dev, "cannot register iommu interface (%d)\n", ret);
		iommu_device_sysfs_remove(&iommu->iommu);
		goto fail;
	}

	return 0;
 fail:
	WRITE_ONCE(iommu->hpm_ready, false);
	riscv_iommu_enable(iommu, RISCV_IOMMU_DDTP_MODE_OFF);
	riscv_iommu_queue_free(iommu, &iommu->priq);
	riscv_iommu_queue_free(iommu, &iommu->fltq);
	riscv_iommu_queue_free(iommu, &iommu->cmdq);
	iopf_queue_free(iommu->pq_work);
	return ret;
}
```

---

`tests/test_iommu_hpm_session.c`

```c
// SPDX-License-Identifier: GPL-2.0-only
/* Host MMIO model for production HPM session control logic, not event behavior. */
#include <assert.h>
#include <errno.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef uint32_t u32;
typedef uint64_t u64;
#define BIT(n) (UINT64_C(1) << (n))
#define BIT_ULL(n) BIT(n)
#define GENMASK(h, l) ((UINT64_MAX >> (63 - (h))) & (UINT64_MAX << (l)))
#define FIELD_PREP(mask, val) (((u64)(val) << __builtin_ctzll(mask)) & (mask))
#define READ_ONCE(x) (x)
#define GFP_KERNEL 0
#define EXPORT_SYMBOL_GPL(name)
#define IS_REACHABLE(option) 1
#include "production_hpm_bits.h"
#include "production_hpm_api.h"

struct mutex { bool held; };
struct riscv_iommu_device {
	unsigned char *reg;
	struct mutex hpm_lock;
	bool hpm_ready;
};
struct iommu_domain { unsigned int type; };
struct riscv_iommu_domain { struct iommu_domain domain; };
struct riscv_iommu_endpoint {
	struct riscv_iommu_device *iommu;
	u32 devid;
	struct riscv_iommu_domain *domain;
};
struct device {
	struct riscv_iommu_endpoint *ep;
	struct iommu_domain *live;
};
struct riscv_iommu_s1_flush_target {
	struct device *dev;
	struct iommu_domain *domain;
	u32 did;
};

static struct riscv_iommu_endpoint *riscv_iommu_hpm_endpoint(struct device *dev)
{
	return dev ? dev->ep : NULL;
}

/* The core domain reported for the device at acquire time. */
static struct iommu_domain *iommu_get_domain_for_dev(struct device *dev)
{
	return dev ? dev->live : NULL;
}

/* Captures what the production wrapper forwards to the private helper. */
static struct riscv_iommu_device *flushed_iommu;
static struct riscv_iommu_s1_flush_target flushed_targets[2];
static unsigned int flushed_nr;
static int flush_result;

static int riscv_iommu_flush_s1_checked(struct riscv_iommu_device *iommu,
					const struct riscv_iommu_s1_flush_target *targets,
					unsigned int nr_targets)
{
	unsigned int i;

	assert(iommu->hpm_lock.held);
	flushed_iommu = iommu;
	flushed_nr = nr_targets;
	for (i = 0; i < nr_targets && i < 2; i++)
		flushed_targets[i] = targets[i];
	return flush_result;
}

static int mutex_trylock(struct mutex *lock)
{
	if (lock->held)
		return 0;
	lock->held = true;
	return 1;
}

static void mutex_unlock(struct mutex *lock)
{
	assert(lock->held);
	lock->held = false;
}

#define lockdep_assert_held(lock) assert((lock)->held)
static int allocation_fail, allocated;

static void *kzalloc(size_t size, int flags)
{
	void *allocation;

	(void)flags;
	if (allocation_fail)
		return NULL;
	allocation = calloc(1, size);
	assert(allocation);
	allocated++;
	return allocation;
}

static void kfree(void *allocation)
{
	if (allocation) {
		allocated--;
		free(allocation);
	}
}

enum fault {
	NONE, MISSING_SLOT, DROPPED_DID_FILTER, COUNTER_CLEAR_IGNORED,
	INHIBIT_FREEZE_IGNORED, INHIBIT_ENABLE_IGNORED,
};
struct mmio {
	unsigned char regs[0x400];
	enum fault fault;
	unsigned int fault_slot;
	unsigned int writes, cycle_writes, high_reads, low_reads;
	unsigned int rollover_reg, rollover_step;
	bool rollover;
};
static struct mmio banks[2];
static struct riscv_iommu_device iommus[2];
static struct riscv_iommu_endpoint endpoints[3];
static struct device devices[4];
static struct riscv_iommu_domain domains[3];
static unsigned int cases;

static u64 raw_read(struct mmio *bank, unsigned int reg)
{
	u64 value;

	assert(reg + sizeof(value) <= sizeof(bank->regs));
	memcpy(&value, bank->regs + reg, sizeof(value));
	return value;
}

static void raw_write(struct mmio *bank, unsigned int reg, u64 value)
{
	assert(reg + sizeof(value) <= sizeof(bank->regs));
	memcpy(bank->regs + reg, &value, sizeof(value));
}

static struct mmio *bank_of(const void *address, unsigned int *offset)
{
	unsigned int i;

	for (i = 0; i < 2; i++) {
		uintptr_t base = (uintptr_t)banks[i].regs;
		uintptr_t addr = (uintptr_t)address;

		if (addr >= base && addr < base + sizeof(banks[i].regs)) {
			*offset = (unsigned int)(addr - base);
			return &banks[i];
		}
	}
	abort();
}

static bool missing(const struct mmio *bank, unsigned int reg)
{
	return bank->fault == MISSING_SLOT &&
	       (reg == RISCV_IOMMU_REG_IOHPMCTR(bank->fault_slot) ||
		reg == RISCV_IOMMU_REG_IOHPMEVT(bank->fault_slot));
}

static u32 readl(const void *address)
{
	unsigned int reg;
	struct mmio *bank = bank_of(address, &reg);
	u32 value;

	assert(!(reg & 3U));
	if (bank->rollover && (reg == bank->rollover_reg || reg == bank->rollover_reg + 4)) {
		static const unsigned int offsets[] = {4, 0, 4, 4, 0, 4};
		static const u32 values[] = {5, UINT32_MAX, 6, 6, 9, 6};

		assert(bank->rollover_step < 6);
		assert(reg - bank->rollover_reg == offsets[bank->rollover_step]);
		value = values[bank->rollover_step++];
		if (reg == bank->rollover_reg) bank->low_reads++;
		else bank->high_reads++;
		if (bank->rollover_step == 6)
			bank->rollover = false;
		return value;
	}
	if (missing(bank, reg & ~7U))
		return 0;
	memcpy(&value, bank->regs + reg, sizeof(value));
	return value;
}

static u64 readq(const void *address)
{
	unsigned int reg;
	struct mmio *bank = bank_of(address, &reg);

	return missing(bank, reg) ? 0 : raw_read(bank, reg);
}

static void writel(u32 value, void *address)
{
	unsigned int reg;
	struct mmio *bank = bank_of(address, &reg);

	bank->writes++;
	assert(reg == RISCV_IOMMU_REG_IOCOUNTINH);
	if (bank->fault == INHIBIT_FREEZE_IGNORED ||
	    (bank->fault == INHIBIT_ENABLE_IGNORED && value == 0x80000000U))
		return;
	memcpy(bank->regs + reg, &value, sizeof(value));
}

static void writeq(u64 value, void *address)
{
	unsigned int reg;
	struct mmio *bank = bank_of(address, &reg);
	unsigned int i;

	bank->writes++;
	if (reg == RISCV_IOMMU_REG_IOHPMCYCLES)
		bank->cycle_writes++;
	for (i = 0; i < 6; i++)
		if (reg == RISCV_IOMMU_REG_IOHPMCTR(i) || reg == RISCV_IOMMU_REG_IOHPMEVT(i))
			assert(readl(bank->regs + RISCV_IOMMU_REG_IOCOUNTINH) & (1U << (i + 1)));
	if (missing(bank, reg))
		return;
	if (bank->fault == DROPPED_DID_FILTER && reg == RISCV_IOMMU_REG_IOHPMEVT(bank->fault_slot))
		value &= ~RISCV_IOMMU_IOHPMEVT_DV_GSCV;
	if (bank->fault == COUNTER_CLEAR_IGNORED && value == 0 &&
	    reg == RISCV_IOMMU_REG_IOHPMCTR(bank->fault_slot))
		return;
	raw_write(bank, reg, value);
}

/* This file is copied from the production source with only includes removed. */
#include "production_hpm.c"

static void setup(void)
{
	unsigned int i, lane;

	assert(allocated == 0);
	memset(banks, 0, sizeof(banks));
	memset(iommus, 0, sizeof(iommus));
	memset(devices, 0, sizeof(devices));
	memset(domains, 0, sizeof(domains));
	allocation_fail = 0;
	flush_result = 0;
	flushed_iommu = NULL;
	flushed_nr = 0;
	for (lane = 0; lane < 2; lane++) {
		iommus[lane].reg = banks[lane].regs;
		iommus[lane].hpm_ready = true;
		raw_write(&banks[lane], RISCV_IOMMU_REG_CAP, RISCV_IOMMU_CAP_HPM);
		raw_write(&banks[lane], RISCV_IOMMU_REG_IOCOUNTINH, 0x80000029U);
		raw_write(&banks[lane], RISCV_IOMMU_REG_IOHPMCYCLES, UINT64_C(0x123456789));
		for (i = 0; i < 7; i++) {
			raw_write(&banks[lane], RISCV_IOMMU_REG_IOHPMEVT(i), 0x101 + i);
			raw_write(&banks[lane], RISCV_IOMMU_REG_IOHPMCTR(i), 0x100000010ULL + i);
		}
	}
	endpoints[0] = (struct riscv_iommu_endpoint){ .iommu = &iommus[0], .devid = 10, .domain = &domains[0] };
	endpoints[1] = (struct riscv_iommu_endpoint){ .iommu = &iommus[0], .devid = 11, .domain = &domains[1] };
	endpoints[2] = (struct riscv_iommu_endpoint){ .iommu = &iommus[1], .devid = 12, .domain = &domains[2] };
	for (i = 0; i < 3; i++) {
		devices[i].ep = &endpoints[i];
		devices[i].live = &domains[i].domain;
	}
	devices[3].ep = NULL;
	devices[3].live = NULL;
}

static void check_trial(bool solo)
{
	struct riscv_iommu_hpm_session *session = NULL, *other = NULL;
	struct riscv_iommu_hpm_sample sample;
	unsigned char saved[sizeof(banks[0].regs)];
	static const unsigned int event_ids[] = {4, 7, 8};
	u64 cycles;
	unsigned int i;

	setup();
	memcpy(saved, banks[0].regs, sizeof(saved));
	assert(riscv_iommu_hpm_acquire(&devices[0], solo ? NULL : &devices[1], &session) == 0);
	assert(session && allocated == 1 && iommus[0].hpm_lock.held);
	/* Unrelated inhibit bits survive; cycle counter is enabled without reset. */
	assert(readl(banks[0].regs + RISCV_IOMMU_REG_IOCOUNTINH) == (solo ? 0x80000070U : 0x80000000U));
	for (i = 0; i < 6; i++) {
		u64 did = i < 3 || solo ? 10 : 11;
		u64 expected = (UINT64_C(1) << 63) | (UINT64_C(1) << 61) |
			       (did << 36) | event_ids[i % 3];

		assert(raw_read(&banks[0], RISCV_IOMMU_REG_IOHPMEVT(i)) == expected);
		assert(raw_read(&banks[0], RISCV_IOMMU_REG_IOHPMCTR(i)) == 0);
		if (!solo || i < 3)
			raw_write(&banks[0], RISCV_IOMMU_REG_IOHPMCTR(i), 0x8000000000000020ULL + i);
	}
	assert(riscv_iommu_hpm_acquire(&devices[0], NULL, &other) == -EBUSY);
	assert(!other && allocated == 1 && iommus[0].hpm_lock.held);
	cycles = raw_read(&banks[0], RISCV_IOMMU_REG_IOHPMCYCLES) + 99;
	cycles |= UINT64_C(1) << 63; /* Overflow flag is excluded only from sample.cycles. */
	raw_write(&banks[0], RISCV_IOMMU_REG_IOHPMCYCLES, cycles);
	/* Preserve synthetic cycle advancement in the expected post-release image. */
	memcpy(saved + RISCV_IOMMU_REG_IOHPMCYCLES, &cycles, sizeof(cycles));
	riscv_iommu_hpm_read(session, &sample);
	assert(sample.cycles == (cycles & UINT64_C(0x7fffffffffffffff)));
	assert(sample.iotlb_miss[0] == 0x8000000000000020ULL);
	assert(sample.s1_ptw[0] == 0x8000000000000021ULL);
	assert(sample.s2_ptw[0] == 0x8000000000000022ULL);
	assert(sample.iotlb_miss[1] == (solo ? 0 : 0x8000000000000023ULL));
	assert(sample.s1_ptw[1] == (solo ? 0 : 0x8000000000000024ULL));
	assert(sample.s2_ptw[1] == (solo ? 0 : 0x8000000000000025ULL));

	/* The wrapper forwards the acquire-time snapshot to the helper. */
	assert(riscv_iommu_hpm_flush_s1(session) == 0);
	assert(flushed_iommu == &iommus[0]);
	assert(flushed_nr == (solo ? 1U : 2U));
	assert(flushed_targets[0].dev == &devices[0]);
	assert(flushed_targets[0].domain == &domains[0].domain);
	assert(flushed_targets[0].did == 10);
	if (!solo) {
		assert(flushed_targets[1].dev == &devices[1]);
		assert(flushed_targets[1].domain == &domains[1].domain);
		assert(flushed_targets[1].did == 11);
	}
	flush_result = -EIO;
	assert(riscv_iommu_hpm_flush_s1(session) == -EIO);
	flush_result = 0;

	riscv_iommu_hpm_release(session);
	assert(!memcmp(saved, banks[0].regs, sizeof(saved)));
	assert(!banks[0].cycle_writes && !iommus[0].hpm_lock.held && !allocated);
	cases++;
}

/* An unbound device, or a domain owned by another consumer, is not rejected. */
static void check_snapshots(void)
{
	struct riscv_iommu_hpm_session *session = NULL;

	/* No core domain at all: acquire still succeeds and flush sees NULL. */
	setup();
	devices[0].live = NULL;
	assert(riscv_iommu_hpm_acquire(&devices[0], NULL, &session) == 0);
	assert(session && allocated == 1 && iommus[0].hpm_lock.held);
	assert(riscv_iommu_hpm_flush_s1(session) == 0);
	assert(flushed_nr == 1 && flushed_targets[0].dev == &devices[0]);
	assert(flushed_targets[0].domain == NULL);
	riscv_iommu_hpm_release(session);
	assert(!allocated && !banks[0].cycle_writes);

	/* A core domain that differs from the endpoint's own record is stored. */
	setup();
	devices[0].live = &domains[2].domain;
	endpoints[0].domain = &domains[1];
	assert(riscv_iommu_hpm_acquire(&devices[0], NULL, &session) == 0);
	assert(session && allocated == 1);
	assert(riscv_iommu_hpm_flush_s1(session) == 0);
	assert(flushed_targets[0].domain == &domains[2].domain);
	assert(flushed_targets[0].did == 10);
	riscv_iommu_hpm_release(session);
	assert(!allocated && !banks[0].cycle_writes);
	cases++;
}

static void check_failed_programming(enum fault fault, unsigned int slot, int expected)
{
	struct riscv_iommu_hpm_session *session = (void *)(uintptr_t)1;
	unsigned char saved[sizeof(banks[0].regs)];

	setup();
	banks[0].fault = fault;
	banks[0].fault_slot = slot;
	if (fault == MISSING_SLOT) {
		raw_write(&banks[0], RISCV_IOMMU_REG_IOHPMEVT(slot), 0);
		raw_write(&banks[0], RISCV_IOMMU_REG_IOHPMCTR(slot), 0);
	}
	memcpy(saved, banks[0].regs, sizeof(saved));
	assert(riscv_iommu_hpm_acquire(&devices[0], &devices[1], &session) == expected);
	assert(!session && !allocated && !iommus[0].hpm_lock.held);
	assert(!memcmp(saved, banks[0].regs, sizeof(saved)));
	assert(!banks[0].cycle_writes);
	if (fault == INHIBIT_FREEZE_IGNORED)
		assert(banks[0].writes == 2); /* Only freeze and original inhibit attempted. */
	cases++;
}

static void check_rejections(void)
{
	struct riscv_iommu_hpm_session *session;
	struct riscv_iommu_hpm_sample sample, zero = {0};

	setup();
	assert(riscv_iommu_hpm_supported(&devices[0]));
	assert(!riscv_iommu_hpm_supported(&devices[3]));
	assert(riscv_iommu_hpm_acquire(&devices[0], NULL, NULL) == -EINVAL);
	assert(riscv_iommu_hpm_flush_s1(NULL) == -EINVAL);
	assert(riscv_iommu_hpm_acquire(&devices[3], NULL, &session) == -EOPNOTSUPP && !session);
	assert(riscv_iommu_hpm_acquire(&devices[0], &devices[3], &session) == -EOPNOTSUPP && !session);
	assert(riscv_iommu_hpm_acquire(&devices[0], &devices[2], &session) == -EXDEV && !session);
	endpoints[1].devid = endpoints[0].devid;
	assert(riscv_iommu_hpm_acquire(&devices[0], &devices[1], &session) == -EINVAL && !session);
	iommus[0].hpm_ready = false;
	assert(!riscv_iommu_hpm_supported(&devices[0]));
	assert(riscv_iommu_hpm_acquire(&devices[0], NULL, &session) == -EOPNOTSUPP && !session);
	iommus[0].hpm_ready = true;
	raw_write(&banks[0], RISCV_IOMMU_REG_CAP, 0);
	assert(!riscv_iommu_hpm_supported(&devices[0]));
	assert(riscv_iommu_hpm_acquire(&devices[0], NULL, &session) == -EOPNOTSUPP && !session);
	raw_write(&banks[0], RISCV_IOMMU_REG_CAP, RISCV_IOMMU_CAP_HPM);
	allocation_fail = 1;
	assert(riscv_iommu_hpm_acquire(&devices[0], NULL, &session) == -ENOMEM && !session);
	assert(!iommus[0].hpm_lock.held && !allocated && !banks[0].writes);
	memset(&sample, 0xff, sizeof(sample));
	riscv_iommu_hpm_read(NULL, &sample);
	assert(!memcmp(&sample, &zero, sizeof(sample)));
	riscv_iommu_hpm_release(NULL);
	cases++;
}

static void check_rollover(unsigned int reg)
{
	struct riscv_iommu_hpm_session *session;
	struct riscv_iommu_hpm_sample sample;

	setup();
	assert(riscv_iommu_hpm_acquire(&devices[0], NULL, &session) == 0);
	banks[0].rollover_reg = reg;
	banks[0].rollover = true;
	raw_write(&banks[0], reg, (UINT64_C(6) << 32) | 9);
	riscv_iommu_hpm_read(session, &sample);
	assert(banks[0].rollover_step == 6 && banks[0].high_reads == 4 && banks[0].low_reads == 2);
	assert((reg == RISCV_IOMMU_REG_IOHPMCYCLES ? sample.cycles : sample.iotlb_miss[0]) ==
	       ((UINT64_C(6) << 32) | 9));
	riscv_iommu_hpm_release(session);
	assert(!allocated && !banks[0].cycle_writes);
	cases++;
}

int main(void)
{
	unsigned int slot;

	check_trial(false);
	check_trial(true);
	check_snapshots();
	for (slot = 0; slot < 6; slot++) {
		check_failed_programming(MISSING_SLOT, slot, -EOPNOTSUPP);
		check_failed_programming(DROPPED_DID_FILTER, slot, -EOPNOTSUPP);
		check_failed_programming(COUNTER_CLEAR_IGNORED, slot, -EIO);
	}
	check_rejections();
	check_failed_programming(INHIBIT_FREEZE_IGNORED, 0, -EOPNOTSUPP);
	check_failed_programming(INHIBIT_ENABLE_IGNORED, 0, -EOPNOTSUPP);
	check_rollover(RISCV_IOMMU_REG_IOHPMCYCLES);
	check_rollover(RISCV_IOMMU_REG_IOHPMCTR(0));
	printf("PASS: %u production HPM control cases (host MMIO/lock model)\n", cases);
	return 0;
}
```

---

`tests/test_iommu_hpm_session.py`

```python
#!/usr/bin/env python3
"""Run production HPM session functions against host-controlled MMIO/locks.

Only kernel includes are removed; the production function bodies are compiled
unchanged. This checks driver control logic, not RTL behavior or board counters.
"""

import hashlib
import os
from pathlib import Path
import re
import resource
import subprocess
import tempfile


ROOT = Path(__file__).resolve().parents[1]


def without_includes(text):
    return re.sub(r"^\s*#include[^\n]*", "", text, flags=re.MULTILINE)


def main():
    resource.setrlimit(resource.RLIMIT_CORE, (0, 0))
    source = ROOT / "linux/drivers/iommu/riscv/iommu-hpm.c"
    contents = source.read_text()
    with tempfile.TemporaryDirectory(prefix="iommu-hpm-session-") as temporary:
        temp = Path(temporary)
        (temp / "production_hpm.c").write_text(without_includes(contents))
        (temp / "production_hpm_api.h").write_text(without_includes(
            (ROOT / "linux/include/linux/riscv_iommu_hpm.h").read_text()))
        # Use the driver's register definitions without unrelated queue structs.
        definitions = []
        for line in (ROOT / "linux/drivers/iommu/riscv/iommu-bits.h").read_text().splitlines():
            if re.match(r"#define\s+RISCV_IOMMU_", line) and (
                    "HPM" in line or "IOCOUNT" in line or
                    re.match(r"#define\s+RISCV_IOMMU_REG_CAP\s", line) or
                    re.match(r"#define\s+RISCV_IOMMU_EVENT_(TLB_MISS|S1_PTW|S2_PTW)\s", line)):
                assert not line.endswith("\\"), "update extraction for continued register macro"
                definitions.append(line)
        (temp / "production_hpm_bits.h").write_text("\n".join(definitions) + "\n")
        binary = temp / "hpm-session-test"
        subprocess.run([
            os.environ.get("HOSTCC", "cc"), "-std=gnu11", "-O2", "-g", "-Wall", "-Wextra",
            "-Werror", "-I", str(temp), str(ROOT / "tests/test_iommu_hpm_session.c"),
            "-o", str(binary),
        ], check=True)
        subprocess.run([str(binary)], check=True)
    print(f"Production iommu-hpm.c SHA256: {hashlib.sha256(contents.encode()).hexdigest()}")
    print("Host control-logic checks only; no RTL/HPM event/FPGA calibration result.")


if __name__ == "__main__":
    main()
```

---

`newtests/test_iommu_s1_flush.c`

```c
// SPDX-License-Identifier: GPL-2.0-only
/*
 * Host harness for the production riscv_iommu_flush_s1_checked() path.
 *
 * The production function bodies (riscv_iommu_cmd_inval_vma,
 * riscv_iommu_flush_s1_check_ep and riscv_iommu_flush_s1_checked) are
 * extracted verbatim from iommu.c and compiled unchanged; only their kernel
 * dependencies are mocked. This models driver control flow, command encoding
 * and locking, not RTL behavior.
 */
#include <assert.h>
#include <errno.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

typedef uint8_t u8;
typedef uint32_t u32;
typedef uint64_t u64;
typedef unsigned long pgd_t;
typedef unsigned long resource_size_t;
typedef unsigned long dma_addr_t;
typedef u32 ioasid_t;

#define PAGE_SHIFT	12
#define PAGE_SIZE	(1UL << PAGE_SHIFT)

#define __iomem
#define BIT(n)		(UINT64_C(1) << (n))
#define BIT_ULL(n)	BIT(n)
#define GENMASK(h, l)	((UINT64_MAX >> (63 - (h))) & (UINT64_MAX << (l)))
#define GENMASK_ULL(h, l) GENMASK(h, l)
#define FIELD_PREP(mask, val) (((u64)(val) << __builtin_ctzll(mask)) & (mask))
#define FIELD_GET(mask, val) (((u64)(val) & (mask)) >> __builtin_ctzll(mask))

#define le64_to_cpu(v)	((u64)(v))
#define cpu_to_le64(v)	((u64)(v))
#define virt_to_pfn(p)	((unsigned long)(uintptr_t)(p) >> PAGE_SHIFT)

/* Barriers are no-ops in this single-threaded host model. */
#define wmb()	do { } while (0)
#define rmb()	do { } while (0)

#define SINGLE_DEPTH_NESTING	1

struct device;
struct iopf_queue;
struct iommu_device { int unused; };
struct iommu_domain_ops { int unused; };
struct iommu_domain {
	unsigned int type;
	const struct iommu_domain_ops *ops;
};
struct mutex { bool held; };
struct rb_node { int unused; };
struct rb_root { struct rb_node *rb_node; };
struct list_head { struct list_head *next, *prev; };
struct io_pgtable { int unused; };
struct mmu_notifier { int unused; };
typedef struct { bool held; } spinlock_t;
typedef struct { long counter; } atomic64_t;

#define atomic64_read(a)	((a)->counter)
#define atomic64_inc(a)		((a)->counter++)
#define atomic64_set(a, v)	((a)->counter = (v))

static u32 readl_relaxed(const void *address)
{
	u32 value;

	memcpy(&value, address, sizeof(value));
	return value;
}

static void writel_relaxed(u32 value, void *address)
{
	memcpy(address, &value, sizeof(value));
}

static u64 readq_relaxed(const void *address)
{
	u64 value;

	memcpy(&value, address, sizeof(value));
	return value;
}

static void writeq_relaxed(u64 value, void *address)
{
	memcpy(address, &value, sizeof(value));
}

#include "production_bits.h"
#include "production_iommu_types.h"
#include "production_iommu.h"

struct device {
	struct riscv_iommu_endpoint *s1_ep;
	struct iommu_domain *live;
};

/* The production domain ops object lives in iommu.c and is not extracted. */
static const struct iommu_domain_ops riscv_iommu_domain_ops = { 0 };

/* ---- mocked production dependencies ---- */

static int post_result = 1;
static int fence_result = 1;
static int post_bump_seq;
static int fence_clear_cqen;
static unsigned int post_count;
static unsigned int fence_count;
static struct riscv_iommu_command posted_cmd;

static struct riscv_iommu_endpoint *post_watch[2];
static unsigned int post_watch_nr;

/* Lock ordering / nesting observations. */
static struct mutex *lock_seq[8];
static unsigned int lock_seq_nr;
static struct mutex *lock_stack[4];
static unsigned int lock_stack_nr;
static bool nested_lock_seen;

/*
 * Host model of a producer that posts to the command queue immediately after
 * cq_lock is released. It must not invalidate a comparison that was already
 * made under the lock.
 */
static struct riscv_iommu_device *producer_after_unlock;
static u32 producer_lui;

static void mutex_lock(struct mutex *lock)
{
	assert(!lock->held);
	assert(lock_seq_nr < 8 && lock_stack_nr < 4);
	lock_seq[lock_seq_nr++] = lock;
	lock_stack[lock_stack_nr++] = lock;
	lock->held = true;
}

static void mutex_lock_nested(struct mutex *lock, unsigned int subclass)
{
	assert(subclass == SINGLE_DEPTH_NESTING);
	assert(!nested_lock_seen);
	nested_lock_seen = true;
	mutex_lock(lock);
}

static void mutex_unlock(struct mutex *lock)
{
	assert(lock->held);
	assert(lock_stack_nr > 0 && lock_stack[lock_stack_nr - 1] == lock);
	lock_stack_nr--;
	lock->held = false;
}

static void spin_lock_irqsave(spinlock_t *lock, unsigned long flags)
{
	(void)flags;
	assert(!lock->held);
	lock->held = true;
}

static void spin_unlock_irqrestore(spinlock_t *lock, unsigned long flags)
{
	(void)flags;
	assert(lock->held);
	lock->held = false;
	if (producer_after_unlock) {
		producer_after_unlock->cmdq.lui = producer_lui;
		producer_after_unlock = NULL;
	}
}

static bool riscv_iommu_post(struct riscv_iommu_device *iommu,
			     struct riscv_iommu_command *cmd)
{
	unsigned int i;

	for (i = 0; i < post_watch_nr; i++)
		assert(post_watch[i]->lock.held);
	assert(post_count == 0);
	posted_cmd = *cmd;
	post_count++;
	if (post_bump_seq)
		atomic64_inc(&iommu->cq_error_seq);
	return post_result;
}

static bool riscv_iommu_iofence_sync(struct riscv_iommu_device *iommu)
{
	unsigned int i;

	for (i = 0; i < post_watch_nr; i++)
		assert(post_watch[i]->lock.held);
	assert(post_count == 1);
	assert(fence_count == 0);
	fence_count++;
	if (fence_clear_cqen)
		riscv_iommu_writel(iommu, RISCV_IOMMU_REG_CQCSR,
				   RISCV_IOMMU_CQCSR_CQON);
	return fence_result;
}

struct riscv_iommu_endpoint *riscv_iommu_hpm_endpoint(struct device *dev)
{
	return dev ? dev->s1_ep : NULL;
}

struct iommu_domain *iommu_get_domain_for_dev(struct device *dev)
{
	return dev ? dev->live : NULL;
}

#include "production_s1_flush.c"

/* ---- fixtures ---- */

static struct riscv_iommu_device iommus[2];
static struct riscv_iommu_endpoint eps[3];
static struct device devices[3];
static struct riscv_iommu_domain domains[3];
static struct riscv_iommu_dc dcs[3];
/* Distinct 4096-byte-aligned roots so the FSC PPN comparison is meaningful. */
static u8 pgd_pages[3][PAGE_SIZE] __attribute__((aligned(PAGE_SIZE)));
static u8 s1_regs[2][0x100];
static u8 cmdq_mem[2][8 * sizeof(struct riscv_iommu_command)];
static unsigned int cases;

static void reg_write(unsigned int bank, unsigned int offset, u32 value)
{
	assert(offset + sizeof(value) <= sizeof(s1_regs[bank]));
	memcpy(s1_regs[bank] + offset, &value, sizeof(value));
}

static unsigned int ep_lock_devid(const struct mutex *lock)
{
	unsigned int i;

	for (i = 0; i < 3; i++)
		if (lock == &eps[i].lock)
			return eps[i].devid;
	abort();
}

static void setup(void)
{
	unsigned int i;

	memset(iommus, 0, sizeof(iommus));
	memset(eps, 0, sizeof(eps));
	memset(devices, 0, sizeof(devices));
	memset(domains, 0, sizeof(domains));
	memset(dcs, 0, sizeof(dcs));
	memset(s1_regs, 0, sizeof(s1_regs));

	post_count = fence_count = 0;
	post_result = fence_result = 1;
	post_bump_seq = fence_clear_cqen = 0;
	post_watch_nr = 0;
	lock_seq_nr = lock_stack_nr = 0;
	nested_lock_seen = false;
	producer_after_unlock = NULL;
	producer_lui = 0;

	for (i = 0; i < 2; i++) {
		iommus[i].reg = s1_regs[i];
		iommus[i].cmdq.base = cmdq_mem[i];
		iommus[i].cmdq.cnt = 8;
		iommus[i].cmdq.lui = 0;
		reg_write(i, RISCV_IOMMU_REG_CQCSR,
			  RISCV_IOMMU_CQCSR_CQEN | RISCV_IOMMU_CQCSR_CQON);
		reg_write(i, RISCV_IOMMU_REG_CQT, 0);
	}

	for (i = 0; i < 3; i++) {
		devices[i].s1_ep = &eps[i];
		devices[i].live = &domains[i].domain;
		eps[i].dev = &devices[i];
		eps[i].iommu = &iommus[i == 2 ? 1 : 0];
		eps[i].devid = 10 + i;
		eps[i].domain = &domains[i];
		eps[i].dc = &dcs[i];
		eps[i].pasid_enabled = false;

		domains[i].domain.type = IOMMU_DOMAIN_DMA;
		domains[i].domain.ops = &riscv_iommu_domain_ops;
		domains[i].iommu = &iommus[i == 2 ? 1 : 0];
		domains[i].mode = RISCV_IOMMU_DC_FSC_IOSATP_MODE_SV39;
		domains[i].pgd_root = (pgd_t *)pgd_pages[i];
		domains[i].g_stage = false;
		domains[i].nested = NULL;

		dcs[i].tc = cpu_to_le64(RISCV_IOMMU_DC_TC_V);
		dcs[i].iohgatp = 0;
		dcs[i].fsc = cpu_to_le64(
				FIELD_PREP(RISCV_IOMMU_DC_FSC_MODE,
					   RISCV_IOMMU_DC_FSC_IOSATP_MODE_SV39) |
				FIELD_PREP(RISCV_IOMMU_DC_FSC_PPN,
					   virt_to_pfn(domains[i].pgd_root)));
	}
}

static struct riscv_iommu_s1_flush_target mk(unsigned int idx)
{
	struct riscv_iommu_s1_flush_target target;

	target.dev = &devices[idx];
	target.domain = &domains[idx].domain;
	target.did = 10 + idx;
	return target;
}

static int run(unsigned int iommu,
	       const struct riscv_iommu_s1_flush_target *targets,
	       unsigned int nr_targets)
{
	unsigned int i;
	int ret;

	post_count = fence_count = 0;
	post_watch_nr = 0;
	nested_lock_seen = false;

	if (targets && (nr_targets == 1 || nr_targets == 2)) {
		for (i = 0; i < nr_targets; i++) {
			struct riscv_iommu_endpoint *ep = targets[i].dev ?
				 targets[i].dev->s1_ep : NULL;

			if (ep)
				post_watch[post_watch_nr++] = ep;
		}
	}

	ret = riscv_iommu_flush_s1_checked(&iommus[iommu], targets,
					   nr_targets);
	assert(!eps[0].lock.held && !eps[1].lock.held && !eps[2].lock.held);
	assert(lock_stack_nr == 0);
	cases++;
	return ret;
}

/* Rejection that must never reach command submission or take an ep lock. */
static void reject(unsigned int iommu,
		   const struct riscv_iommu_s1_flush_target *targets,
		   unsigned int nr_targets, int expected)
{
	unsigned int base = lock_seq_nr;

	assert(run(iommu, targets, nr_targets) == expected);
	assert(!post_count && !fence_count);
	assert(lock_seq_nr == base);
	assert(!nested_lock_seen);
}

int main(void)
{
	struct riscv_iommu_s1_flush_target t[2];
	unsigned int base;
	u64 expected;

	setup();

	/* ---- success: exactly one global VMA command, then a fence ---- */
	t[0] = mk(0);
	base = lock_seq_nr;
	assert(run(0, t, 1) == 0);
	assert(post_count == 1 && fence_count == 1);
	assert(lock_seq_nr == base + 1 && !nested_lock_seen);
	expected = FIELD_PREP(RISCV_IOMMU_CMD_OPCODE,
			      RISCV_IOMMU_CMD_IOTINVAL_OPCODE) |
		   FIELD_PREP(RISCV_IOMMU_CMD_FUNC,
			      RISCV_IOMMU_CMD_IOTINVAL_FUNC_VMA);
	assert(posted_cmd.dword0 == expected);
	assert(posted_cmd.dword1 == 0);
	assert(!(posted_cmd.dword0 & (RISCV_IOMMU_CMD_IOTINVAL_AV |
				      RISCV_IOMMU_CMD_IOTINVAL_PSCV |
				      RISCV_IOMMU_CMD_IOTINVAL_GV)));

	/* Pair success, both target orders, always ascending devid + nested. */
	t[0] = mk(0);
	t[1] = mk(1);
	base = lock_seq_nr;
	assert(run(0, t, 2) == 0);
	assert(post_count == 1 && fence_count == 1);
	assert(lock_seq_nr == base + 2);
	assert(ep_lock_devid(lock_seq[base]) == 10);
	assert(ep_lock_devid(lock_seq[base + 1]) == 11);
	assert(nested_lock_seen);

	t[0] = mk(1);
	t[1] = mk(0);
	base = lock_seq_nr;
	assert(run(0, t, 2) == 0);
	assert(post_count == 1 && fence_count == 1);
	assert(lock_seq_nr == base + 2);
	assert(ep_lock_devid(lock_seq[base]) == 10);
	assert(ep_lock_devid(lock_seq[base + 1]) == 11);
	assert(nested_lock_seen);

	/* ---- parameter rejection ---- */
	t[0] = mk(0);
	assert(run(0, t, 0) == -EINVAL);
	assert(run(0, t, 3) == -EINVAL);
	assert(run(0, NULL, 1) == -EINVAL);
	t[0] = mk(0);
	t[0].dev = NULL;
	reject(0, t, 1, -EINVAL);
	t[0] = mk(0);
	t[0].domain = NULL;
	reject(0, t, 1, -EINVAL);
	t[0] = mk(0);
	t[1] = mk(0);
	reject(0, t, 2, -EINVAL);
	t[0] = mk(0);
	t[1] = mk(1);
	t[1].did = t[0].did;
	reject(0, t, 2, -EINVAL);

	/* ---- identity / snapshot ---- */
	t[0] = mk(0);
	devices[0].s1_ep = NULL;
	reject(0, t, 1, -EOPNOTSUPP);
	devices[0].s1_ep = &eps[0];

	reject(1, t, 1, -EXDEV);

	t[0] = mk(0);
	t[0].did = eps[0].devid + 1;
	reject(0, t, 1, -ESTALE);

	t[0] = mk(0);
	eps[0].domain = &domains[1];
	reject(0, t, 1, -ESTALE);

	eps[0].domain = NULL;
	reject(0, t, 1, -ESTALE);
	eps[0].domain = &domains[0];

	devices[0].live = &domains[1].domain;
	reject(0, t, 1, -ESTALE);
	devices[0].live = NULL;
	reject(0, t, 1, -ESTALE);
	devices[0].live = &domains[0].domain;

	/* ---- domain shape ---- */
	t[0] = mk(0);
	domains[0].domain.type = IOMMU_DOMAIN_IDENTITY;
	reject(0, t, 1, -EOPNOTSUPP);
	domains[0].domain.type = IOMMU_DOMAIN_BLOCKED;
	reject(0, t, 1, -EOPNOTSUPP);
	domains[0].domain.type = IOMMU_DOMAIN_SVA;
	reject(0, t, 1, -EOPNOTSUPP);
	domains[0].domain.type = IOMMU_DOMAIN_DMA;

	domains[0].domain.ops = NULL;
	reject(0, t, 1, -ESTALE);
	domains[0].domain.ops = &riscv_iommu_domain_ops;

	domains[0].iommu = &iommus[1];
	reject(0, t, 1, -EXDEV);
	domains[0].iommu = &iommus[0];

	domains[0].g_stage = true;
	reject(0, t, 1, -EOPNOTSUPP);
	domains[0].g_stage = false;
	domains[0].nested = &domains[1];
	reject(0, t, 1, -EOPNOTSUPP);
	domains[0].nested = NULL;
	eps[0].pasid_enabled = true;
	reject(0, t, 1, -EOPNOTSUPP);
	eps[0].pasid_enabled = false;

	domains[0].mode = RISCV_IOMMU_DC_FSC_MODE_BARE;
	reject(0, t, 1, -EOPNOTSUPP);
	domains[0].mode = RISCV_IOMMU_DC_FSC_IOSATP_MODE_SV48;
	reject(0, t, 1, -EOPNOTSUPP);
	domains[0].mode = RISCV_IOMMU_DC_FSC_IOSATP_MODE_SV39;

	domains[0].pgd_root = NULL;
	reject(0, t, 1, -EOPNOTSUPP);
	domains[0].pgd_root = (pgd_t *)pgd_pages[0];

	/* ---- device context ---- */
	eps[0].dc = NULL;
	reject(0, t, 1, -EOPNOTSUPP);
	eps[0].dc = &dcs[0];

	dcs[0].tc = cpu_to_le64(0);
	reject(0, t, 1, -EOPNOTSUPP);
	dcs[0].tc = cpu_to_le64(RISCV_IOMMU_DC_TC_V | RISCV_IOMMU_DC_TC_EN_ATS);
	reject(0, t, 1, -EOPNOTSUPP);
	dcs[0].tc = cpu_to_le64(RISCV_IOMMU_DC_TC_V | RISCV_IOMMU_DC_TC_EN_PRI);
	reject(0, t, 1, -EOPNOTSUPP);
	dcs[0].tc = cpu_to_le64(RISCV_IOMMU_DC_TC_V | RISCV_IOMMU_DC_TC_PDTV);
	reject(0, t, 1, -EOPNOTSUPP);
	dcs[0].tc = cpu_to_le64(RISCV_IOMMU_DC_TC_V | RISCV_IOMMU_DC_TC_T2GPA);
	reject(0, t, 1, -EOPNOTSUPP);
	dcs[0].tc = cpu_to_le64(RISCV_IOMMU_DC_TC_V | RISCV_IOMMU_DC_TC_SXL);
	reject(0, t, 1, -EOPNOTSUPP);
	dcs[0].tc = cpu_to_le64(RISCV_IOMMU_DC_TC_V);

	dcs[0].iohgatp = cpu_to_le64(
			FIELD_PREP(RISCV_IOMMU_DC_IOHGATP_MODE,
				   RISCV_IOMMU_DC_IOHGATP_MODE_SV39X4));
	reject(0, t, 1, -EOPNOTSUPP);
	dcs[0].iohgatp = 0;

	dcs[0].fsc = cpu_to_le64(
			FIELD_PREP(RISCV_IOMMU_DC_FSC_MODE,
				   RISCV_IOMMU_DC_FSC_MODE_BARE) |
			FIELD_PREP(RISCV_IOMMU_DC_FSC_PPN,
				   virt_to_pfn(domains[0].pgd_root)));
	reject(0, t, 1, -ESTALE);

	dcs[0].fsc = cpu_to_le64(
			FIELD_PREP(RISCV_IOMMU_DC_FSC_MODE,
				   RISCV_IOMMU_DC_FSC_IOSATP_MODE_SV39) |
			FIELD_PREP(RISCV_IOMMU_DC_FSC_PPN,
				   virt_to_pfn(pgd_pages[1])));
	reject(0, t, 1, -ESTALE);

	dcs[0].fsc = cpu_to_le64(
			FIELD_PREP(RISCV_IOMMU_DC_FSC_MODE,
				   RISCV_IOMMU_DC_FSC_IOSATP_MODE_SV39) |
			FIELD_PREP(RISCV_IOMMU_DC_FSC_PPN,
				   virt_to_pfn(domains[0].pgd_root)));

	/* ---- command queue gating ---- */
	t[0] = mk(0);

	reg_write(0, RISCV_IOMMU_REG_CQCSR, RISCV_IOMMU_CQCSR_CQON);
	reject(0, t, 1, -EIO);
	reg_write(0, RISCV_IOMMU_REG_CQCSR, RISCV_IOMMU_CQCSR_CQEN);
	reject(0, t, 1, -EIO);
	reg_write(0, RISCV_IOMMU_REG_CQCSR,
		  RISCV_IOMMU_CQCSR_CQEN | RISCV_IOMMU_CQCSR_CQON |
		  RISCV_IOMMU_CQCSR_CQMF);
	reject(0, t, 1, -EIO);
	reg_write(0, RISCV_IOMMU_REG_CQCSR,
		  RISCV_IOMMU_CQCSR_CQEN | RISCV_IOMMU_CQCSR_CQON |
		  RISCV_IOMMU_CQCSR_CMD_ILL);
	reject(0, t, 1, -EIO);
	reg_write(0, RISCV_IOMMU_REG_CQCSR,
		  RISCV_IOMMU_CQCSR_CQEN | RISCV_IOMMU_CQCSR_CQON |
		  RISCV_IOMMU_CQCSR_CMD_TO);
	reject(0, t, 1, -EIO);
	reg_write(0, RISCV_IOMMU_REG_CQCSR,
		  RISCV_IOMMU_CQCSR_CQEN | RISCV_IOMMU_CQCSR_CQON);

	reg_write(0, RISCV_IOMMU_REG_CQT, 1);
	reject(0, t, 1, -EIO);
	reg_write(0, RISCV_IOMMU_REG_CQT, 0);

	iommus[0].cmdq.base = NULL;
	reject(0, t, 1, -EIO);
	iommus[0].cmdq.base = cmdq_mem[0];
	iommus[0].cmdq.cnt = 0;
	reject(0, t, 1, -EIO);
	iommus[0].cmdq.cnt = 6;
	reject(0, t, 1, -EIO);
	iommus[0].cmdq.cnt = 8;
	iommus[0].cmdq.lui = 8;
	reject(0, t, 1, -EIO);
	iommus[0].cmdq.lui = 0;

	/*
	 * Producer consistency: a poster that updates cmdq.lui right after
	 * cq_lock is released must not invalidate the earlier comparison that
	 * was made while holding the lock.
	 */
	producer_after_unlock = &iommus[0];
	producer_lui = 3;
	assert(run(0, t, 1) == 0);
	assert(post_count == 1 && fence_count == 1);
	assert(iommus[0].cmdq.lui == 3);
	iommus[0].cmdq.lui = 0;

	/* ---- submission outcomes ---- */
	t[0] = mk(0);

	post_result = 0;
	assert(run(0, t, 1) == -EIO && post_count == 1 && fence_count == 0);
	post_result = 1;

	fence_result = 0;
	assert(run(0, t, 1) == -EIO && post_count == 1 && fence_count == 1);
	fence_result = 1;

	/*
	 * Model of the CQ IRQ thread publishing an error and then clearing
	 * CQCSR: CQCSR is clean here, only cq_error_seq moved. This does not
	 * execute the real IRQ path; the production bookkeeping in
	 * riscv_iommu_cmdq_process() is left to kernel review/board test.
	 */
	post_bump_seq = 1;
	assert(run(0, t, 1) == -EIO && post_count == 1 && fence_count == 1);
	post_bump_seq = 0;
	assert(!(riscv_iommu_readl(&iommus[0], RISCV_IOMMU_REG_CQCSR) &
		 (RISCV_IOMMU_CQCSR_CQMF | RISCV_IOMMU_CQCSR_CMD_TO |
		  RISCV_IOMMU_CQCSR_CMD_ILL)));

	/* CQ disabled again before the final check. */
	fence_clear_cqen = 1;
	assert(run(0, t, 1) == -EIO && post_count == 1 && fence_count == 1);
	fence_clear_cqen = 0;
	reg_write(0, RISCV_IOMMU_REG_CQCSR,
		  RISCV_IOMMU_CQCSR_CQEN | RISCV_IOMMU_CQCSR_CQON);

	/* Everything restored still flushes once. */
	assert(run(0, t, 1) == 0);
	assert(post_count == 1 && fence_count == 1);

	printf("PASS: %u production S1 flush cases (host MMIO/lock/command model)\n",
	       cases);
	return 0;
}
```

---

`newtests/test_iommu_s1_flush.py`

```python
#!/usr/bin/env python3
"""Run the production host S1 flush helper against host-controlled MMIO/locks.

Only kernel includes are removed, and the production helper bodies are
extracted verbatim from iommu.c and compiled unchanged. This checks driver
control flow, command encoding and locking, not RTL behavior or board counters.
"""

import hashlib
import os
from pathlib import Path
import re
import resource
import subprocess
import tempfile


ROOT = Path(__file__).resolve().parents[1]

HELPERS = (
    "riscv_iommu_cmd_inval_vma",
    "riscv_iommu_flush_s1_check_ep",
    "riscv_iommu_flush_s1_checked",
)


def without_includes(text):
    return re.sub(r"^\s*#include[^\n]*", "", text, flags=re.MULTILINE)


def extract_function(text, name):
    match = re.search(r"^[A-Za-z_][^\n]*\b" + re.escape(name) + r"\s*\(",
                      text, flags=re.MULTILINE)
    assert match, "function not found: " + name
    start = match.start()
    open_brace = text.index("{", match.end())
    depth = 0
    for index in range(open_brace, len(text)):
        if text[index] == "{":
            depth += 1
        elif text[index] == "}":
            depth -= 1
            if depth == 0:
                return text[start:index + 1]
    raise AssertionError("unterminated function: " + name)


def domain_type_definitions(text):
    """Copy the real IOMMU_DOMAIN_* constants out of <linux/iommu.h>.

    Re-joins backslash continuations so the extracted block is valid C for
    hosts that cannot include <linux/iommu.h> directly.
    """
    lines = text.splitlines()
    definitions = []

    for index, line in enumerate(lines):
        if not re.match(r"#define\s+_*IOMMU_DOMAIN_", line):
            continue
        block = [line]
        while block[-1].rstrip().endswith("\\"):
            index += 1
            block.append(lines[index])
        definitions.append("\n".join(block))

    assert any("IOMMU_DOMAIN_SVA" in entry for entry in definitions), \
        "IOMMU_DOMAIN_SVA not found in <linux/iommu.h>"
    assert any("IOMMU_DOMAIN_DMA" in entry for entry in definitions), \
        "IOMMU_DOMAIN_DMA not found in <linux/iommu.h>"
    return "\n".join(definitions) + "\n"


def main():
    resource.setrlimit(resource.RLIMIT_CORE, (0, 0))
    source = ROOT / "linux/drivers/iommu/riscv/iommu.c"
    contents = source.read_text()
    helpers = "\n\n".join(extract_function(contents, name)
                          for name in HELPERS)
    with tempfile.TemporaryDirectory(prefix="iommu-s1-flush-") as temporary:
        temp = Path(temporary)
        (temp / "production_bits.h").write_text(without_includes(
            (ROOT / "linux/drivers/iommu/riscv/iommu-bits.h").read_text()))
        (temp / "production_iommu_types.h").write_text(domain_type_definitions(
            (ROOT / "linux/include/linux/iommu.h").read_text()))
        (temp / "production_iommu.h").write_text(without_includes(
            (ROOT / "linux/drivers/iommu/riscv/iommu.h").read_text()))
        (temp / "production_s1_flush.c").write_text(helpers + "\n")
        binary = temp / "s1-flush-test"
        subprocess.run([
            os.environ.get("HOSTCC", "cc"), "-std=gnu11", "-O2", "-g",
            "-Wall", "-Wextra", "-Werror", "-I", str(temp),
            str(ROOT / "newtests/test_iommu_s1_flush.c"), "-o", str(binary),
        ], check=True)
        subprocess.run([str(binary)], check=True)
    print(f"Production iommu.c (S1 helpers) SHA256: "
          f"{hashlib.sha256(contents.encode()).hexdigest()}")
    print("Host control/command/lock checks only; no RTL or board result.")


if __name__ == "__main__":
    main()
```

---

## Changes applied (map to review items)

1. `riscv_iommu_flush_s1_checked()`: `seq` sampled before any CQ status read; CQT read **and** compared to `cmdq.lui` inside one `spin_lock_irqsave()` region (“producer consistency”, no queue-empty claim); final check requires `CQEN|CQON` set, no `CQMF|CMD_TO|CMD_ILL`, and unchanged `cq_error_seq`.
2. Second endpoint uses `mutex_lock_nested(..., SINGLE_DEPTH_NESTING)`; first/second chosen by ascending `devid` (stable, tie impossible after the duplicate-DID reject); `dev`/`devid`/`iommu` are re-validated under the lock; no `domain->lock` is ever taken (attach order `domain->lock -> ep->lock` is respected).
3. `riscv_iommu_hpm_acquire()` snapshots `iommu_get_domain_for_dev(dev)` and does **not** reject NULL/other-consumer domains. `riscv_iommu_flush_s1_check_ep()` compares both the live core domain and `&ep->domain->domain` against the snapshot before dereferencing, then checks `domain->iommu`, `ops == riscv_iommu_domain_ops`, `IOMMU_DOMAIN_DMA`/`UNMANAGED` only, Sv39 mode, non-NULL `pgd_root`, TC (V set, no ATS/PRI/PDTV/T2GPA/SXL), IOHGATP BARE, and FSC mode+PPN equality (`-ESTALE` for changed identity, `-EOPNOTSUPP` for unsupported shape/mode, `-EXDEV` for a distinct IOMMU).
4. `atomic64_set()` before `riscv_iommu_queue_init()`; `atomic64_inc()` before the relaxed `riscv_iommu_queue_ctrl()` clear with an explicit `wmb()`; `rmb()` after the final `CQCSR` read before the generation read. IRQ error-clear behaviour is otherwise unchanged (no reset, no extra wait). Wrapper asserts `hpm_lock` (via `lockdep_assert_held`) and returns `-EINVAL` for NULL.
5. Test compile fixes: added `spin_lock_irqsave`/`spin_unlock_irqrestore`/`mutex_lock_nested`/`SINGLE_DEPTH_NESTING` and barrier/atomic stubs; `riscv_iommu_hpm_endpoint` is defined **non-static** in the extracted harness (matches `iommu.h`); `pgd_pages[3][PAGE_SIZE]` with `__attribute__((aligned(PAGE_SIZE)))` gives three genuinely distinct PFNs; `IOMMU_DOMAIN_*` are extracted from the real `linux/include/linux/iommu.h` (DMA=3, UNMANAGED=1, SVA=16), not mirrored.
6. Added core-domain / domain-ops stubs (`iommu_get_domain_for_dev`, `riscv_iommu_domain_ops`), lock-order and nested-class checks, producer-after-unlock case, NULL-root / other-IOMMU / core-vs-EP mismatch / end-CQ-disabled / post-IRQ-clear-seq cases. The IRQ case is explicitly a model of the bookkeeping; the production `riscv_iommu_cmdq_process()` ordering is left to root review/board test. Old acquire without a domain still succeeds; the wrapper passes the NULL snapshot and the checked helper rejects NULL.
7. No public API or existing struct signature changed; `riscv_iommu_post`, `riscv_iommu_post_sync`, `riscv_iommu_flush_iotlb_*` and the queue-IRQ clear path keep their behaviour; the restricted VMA-before-following-fence-fetch note is retained.

## Limitations / assumptions for Codex

- Nothing was compiled or run; all "PASS" strings are test content, not evidence.
- `atomic64_t` is taken to come from `<linux/types.h>` for the struct field; `<linux/atomic.h>` is added to `iommu.c` for the operations. If this tree’s `types.h` differs, add the include to the driver `iommu.h` too.
- `rmb()`/`wmb()` availability is assumed via existing barrier users in `iommu.c`.
- Accepted domain types are strictly `IOMMU_DOMAIN_DMA` and `IOMMU_DOMAIN_UNMANAGED`; `IOMMU_DOMAIN_DMA_FQ` is deliberately rejected. Confirm that matches the intended experiment set.
- `riscv_iommu_flush_s1_check_ep()`/`riscv_iommu_flush_s1_checked()` are placed after `riscv_iommu_hpm_endpoint()`; the `.py` extractor relies on kernel brace/indentation style and on `riscv_iommu_flush_s1_checked`/`riscv_iommu_flush_s1_check_ep` first appearing as their definitions in `iommu.c`.
- The new harness relies on host `void *` pointer arithmetic (`iommu->reg + offset`) being accepted by `-std=gnu11`, same as the existing HPM harness.
- Lockdep semantics (`mutex_lock_nested`, `SINGLE_DEPTH_NESTING`) are modelled by asserts only; a real lockdep run still belongs on the kernel build.