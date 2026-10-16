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
