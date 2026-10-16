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
