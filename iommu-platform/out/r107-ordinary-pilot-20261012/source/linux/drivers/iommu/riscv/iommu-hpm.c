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
