// SPDX-License-Identifier: GPL-2.0-only
/*
 * Host harness for the production riscv_iommu_flush_s1_checked() path.
 *
 * The production helper bodies (riscv_iommu_cmd_inval_vma,
 * riscv_iommu_flush_s1_check_ep and riscv_iommu_flush_s1_checked) are
 * extracted from iommu.c and compiled unchanged; only their kernel
 * dependencies are mocked. This models control flow, command encoding and
 * locking, not RTL behavior.
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

#define PAGE_SHIFT 12
#define __iomem
#define BIT(n) (UINT64_C(1) << (n))
#define BIT_ULL(n) BIT(n)
#define GENMASK(h, l) ((UINT64_MAX >> (63 - (h))) & (UINT64_MAX << (l)))
#define GENMASK_ULL(h, l) GENMASK(h, l)
#define FIELD_PREP(mask, val) (((u64)(val) << __builtin_ctzll(mask)) & (mask))
#define FIELD_GET(mask, val) (((u64)(val) & (mask)) >> __builtin_ctzll(mask))

#define le64_to_cpu(v) ((u64)(v))
#define cpu_to_le64(v) ((u64)(v))
#define virt_to_pfn(p) ((unsigned long)(uintptr_t)(p) >> PAGE_SHIFT)

/*
 * Values from <linux/iommu.h> enum iommu_domain_type. They are the only
 * constants not extracted from the real headers; the harness only needs a
 * non-SVA value and the SVA value used by the production rejection.
 */
#define IOMMU_DOMAIN_DMA 3
#define IOMMU_DOMAIN_SVA 5

struct device;
struct iopf_queue;
struct iommu_device { int unused; };
struct mutex { bool held; };
struct rb_node { int unused; };
struct rb_root { struct rb_node *rb_node; };
struct list_head { struct list_head *next, *prev; };
struct io_pgtable { int unused; };
struct mmu_notifier { int unused; };
struct iommu_domain { unsigned int type; };
typedef struct { bool held; } spinlock_t;
typedef struct { long counter; } atomic64_t;

#define atomic64_read(a) ((a)->counter)
#define atomic64_inc(a) ((a)->counter++)
#define atomic64_set(a, v) ((a)->counter = (v))

static u8 s1_regs[2][0x100];

static u32 readl_relaxed(const void *addr)
{
	u32 value;

	memcpy(&value, addr, sizeof(value));
	return value;
}

static void writel_relaxed(u32 value, void *addr)
{
	memcpy(addr, &value, sizeof(value));
}

static u64 readq_relaxed(const void *addr)
{
	u64 value;

	memcpy(&value, addr, sizeof(value));
	return value;
}

static void writeq_relaxed(u64 value, void *addr)
{
	memcpy(addr, &value, sizeof(value));
}

#include "production_bits.h"
#include "production_iommu.h"

struct device { struct riscv_iommu_endpoint *s1_ep; };

/* ---- mocked dependencies ---- */

static int post_result = 1, fence_result = 1, post_bump_seq;
static unsigned int post_count, fence_count;
static struct riscv_iommu_command posted_cmd;
static struct riscv_iommu_endpoint *locked_eps[2];
static unsigned int locked_nr;

static void mutex_lock(struct mutex *lock)
{
	assert(!lock->held);
	lock->held = true;
}

static void mutex_unlock(struct mutex *lock)
{
	assert(lock->held);
	lock->held = false;
}

static bool riscv_iommu_post(struct riscv_iommu_device *iommu,
			     struct riscv_iommu_command *cmd)
{
	unsigned int i;

	for (i = 0; i < locked_nr; i++)
		assert(locked_eps[i]->lock.held);
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

	(void)iommu;
	for (i = 0; i < locked_nr; i++)
		assert(locked_eps[i]->lock.held);
	assert(post_count == 1);
	assert(fence_count == 0);
	fence_count++;
	return fence_result;
}

static struct riscv_iommu_endpoint *riscv_iommu_hpm_endpoint(struct device *dev)
{
	return dev ? dev->s1_ep : NULL;
}

#include "production_s1_flush.c"

/* ---- fixtures ---- */

static struct riscv_iommu_device iommus[2];
static struct riscv_iommu_endpoint eps[3];
static struct device devices[3];
static struct riscv_iommu_domain domains[3];
static struct riscv_iommu_dc dcs[3];
static pgd_t pgd_roots[3];
static u8 cmdq_mem[2][64];
static unsigned int cases;

static void reg_write(unsigned int bank, unsigned int off, u32 value)
{
	assert(off + sizeof(value) <= sizeof(s1_regs[bank]));
	memcpy(s1_regs[bank] + off, &value, sizeof(value));
}

static void setup(void)
{
	unsigned int i;

	memset(s1_regs, 0, sizeof(s1_regs));
	memset(iommus, 0, sizeof(iommus));
	memset(eps, 0, sizeof(eps));
	memset(devices, 0, sizeof(devices));
	memset(domains, 0, sizeof(domains));
	memset(dcs, 0, sizeof(dcs));

	post_count = fence_count = 0;
	post_result = fence_result = 1;
	post_bump_seq = 0;
	locked_nr = 0;

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
		eps[i].iommu = &iommus[i == 2 ? 1 : 0];
		eps[i].devid = 10 + i;
		eps[i].domain = &domains[i];
		eps[i].dc = &dcs[i];
		eps[i].pasid_enabled = false;

		domains[i].domain.type = IOMMU_DOMAIN_DMA;
		domains[i].mode = RISCV_IOMMU_DC_FSC_IOSATP_MODE_SV39;
		domains[i].pgd_root = &pgd_roots[i];
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
	int ret;

	post_count = fence_count = 0;
	ret = riscv_iommu_flush_s1_checked(&iommus[iommu], targets, nr_targets);
	assert(!eps[0].lock.held && !eps[1].lock.held && !eps[2].lock.held);
	cases++;
	return ret;
}

/* Rejection that must never reach command submission. */
static void reject(unsigned int iommu,
		   const struct riscv_iommu_s1_flush_target *targets,
		   unsigned int nr_targets, int expected)
{
	assert(run(iommu, targets, nr_targets) == expected);
	assert(!post_count && !fence_count);
}

int main(void)
{
	struct riscv_iommu_s1_flush_target t[2];
	u64 expected;

	setup();
	locked_eps[0] = &eps[0];
	locked_eps[1] = &eps[1];
	locked_nr = 1;

	/* Solo success: exactly one global VMA command, then a fence. */
	t[0] = mk(0);
	assert(run(0, t, 1) == 0);
	assert(post_count == 1 && fence_count == 1);
	expected = FIELD_PREP(RISCV_IOMMU_CMD_OPCODE,
			      RISCV_IOMMU_CMD_IOTINVAL_OPCODE) |
		   FIELD_PREP(RISCV_IOMMU_CMD_FUNC,
			      RISCV_IOMMU_CMD_IOTINVAL_FUNC_VMA);
	assert(posted_cmd.dword0 == expected);
	assert(posted_cmd.dword1 == 0);
	assert(!(posted_cmd.dword0 & (RISCV_IOMMU_CMD_IOTINVAL_AV |
				      RISCV_IOMMU_CMD_IOTINVAL_PSCV |
				      RISCV_IOMMU_CMD_IOTINVAL_GV)));

	/* Pair success, exercising both lock orders. */
	t[0] = mk(0);
	t[1] = mk(1);
	locked_nr = 2;
	assert(run(0, t, 2) == 0);
	assert(post_count == 1 && fence_count == 1);
	t[0] = mk(1);
	t[1] = mk(0);
	assert(run(0, t, 2) == 0);
	assert(post_count == 1 && fence_count == 1);
	locked_nr = 1;

	/* Parameter and duplicate rejection. */
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

	/* Identity: unbound device, other IOMMU, changed DID, changed domain. */
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

	/* Domain shape: BARE, S2, nesting, SVA/PASID. */
	t[0] = mk(0);
	domains[0].mode = RISCV_IOMMU_DC_FSC_MODE_BARE;
	reject(0, t, 1, -EOPNOTSUPP);
	domains[0].mode = RISCV_IOMMU_DC_FSC_IOSATP_MODE_SV39;
	domains[0].g_stage = true;
	reject(0, t, 1, -EOPNOTSUPP);
	domains[0].g_stage = false;
	domains[0].nested = &domains[1];
	reject(0, t, 1, -EOPNOTSUPP);
	domains[0].nested = NULL;
	domains[0].domain.type = IOMMU_DOMAIN_SVA;
	reject(0, t, 1, -EOPNOTSUPP);
	domains[0].domain.type = IOMMU_DOMAIN_DMA;
	eps[0].pasid_enabled = true;
	reject(0, t, 1, -EOPNOTSUPP);
	eps[0].pasid_enabled = false;

	/* Device context bits. */
	t[0] = mk(0);
	dcs[0].tc = cpu_to_le64(0);
	reject(0, t, 1, -EOPNOTSUPP);
	dcs[0].tc = cpu_to_le64(RISCV_IOMMU_DC_TC_V |
				RISCV_IOMMU_DC_TC_EN_ATS);
	reject(0, t, 1, -EOPNOTSUPP);
	dcs[0].tc = cpu_to_le64(RISCV_IOMMU_DC_TC_V |
				RISCV_IOMMU_DC_TC_EN_PRI);
	reject(0, t, 1, -EOPNOTSUPP);
	dcs[0].tc = cpu_to_le64(RISCV_IOMMU_DC_TC_V |
				RISCV_IOMMU_DC_TC_PDTV);
	reject(0, t, 1, -EOPNOTSUPP);
	dcs[0].tc = cpu_to_le64(RISCV_IOMMU_DC_TC_V |
				RISCV_IOMMU_DC_TC_T2GPA);
	reject(0, t, 1, -EOPNOTSUPP);
	dcs[0].tc = cpu_to_le64(RISCV_IOMMU_DC_TC_V |
				RISCV_IOMMU_DC_TC_SXL);
	reject(0, t, 1, -EOPNOTSUPP);
	dcs[0].tc = cpu_to_le64(RISCV_IOMMU_DC_TC_V);
	dcs[0].iohgatp = cpu_to_le64(
		FIELD_PREP(RISCV_IOMMU_DC_IOHGATP_MODE,
			   RISCV_IOMMU_DC_IOHGATP_MODE_SV39X4));
	reject(0, t, 1, -EOPNOTSUPP);
	dcs[0].iohgatp = 0;
	eps[0].dc = NULL;
	reject(0, t, 1, -EOPNOTSUPP);
	eps[0].dc = &dcs[0];

	/* Non-Sv39 mode, then FSC/domain disagreement. */
	domains[0].mode = RISCV_IOMMU_DC_FSC_IOSATP_MODE_SV48;
	dcs[0].fsc = cpu_to_le64(
		FIELD_PREP(RISCV_IOMMU_DC_FSC_MODE,
			   RISCV_IOMMU_DC_FSC_IOSATP_MODE_SV48) |
		FIELD_PREP(RISCV_IOMMU_DC_FSC_PPN,
			   virt_to_pfn(domains[0].pgd_root)));
	reject(0, t, 1, -EOPNOTSUPP);
	domains[0].mode = RISCV_IOMMU_DC_FSC_IOSATP_MODE_SV39;
	dcs[0].fsc = cpu_to_le64(
		FIELD_PREP(RISCV_IOMMU_DC_FSC_MODE,
			   RISCV_IOMMU_DC_FSC_IOSATP_MODE_SV39) |
		FIELD_PREP(RISCV_IOMMU_DC_FSC_PPN, virt_to_pfn(&pgd_roots[1])));
	reject(0, t, 1, -ESTALE);
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
			   virt_to_pfn(domains[0].pgd_root)));

	/* Command queue gating. */
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

	/* Submission outcomes. */
	post_result = 0;
	assert(run(0, t, 1) == -EIO && post_count == 1 && fence_count == 0);
	post_result = 1;

	fence_result = 0;
	assert(run(0, t, 1) == -EIO && post_count == 1 && fence_count == 1);
	fence_result = 1;

	/* IRQ thread observed a CQ error even though CQCSR is now clean. */
	post_bump_seq = 1;
	assert(run(0, t, 1) == -EIO && post_count == 1 && fence_count == 1);
	post_bump_seq = 0;
	assert(!(riscv_iommu_readl(&iommus[0], RISCV_IOMMU_REG_CQCSR) &
		 (RISCV_IOMMU_CQCSR_CQMF | RISCV_IOMMU_CQCSR_CMD_TO |
		  RISCV_IOMMU_CQCSR_CMD_ILL)));

	/* Everything restored still flushes once. */
	assert(run(0, t, 1) == 0);
	assert(post_count == 1 && fence_count == 1);

	printf("PASS: %u production S1 flush cases (host MMIO/lock/command model)\n",
	       cases);
	return 0;
}
