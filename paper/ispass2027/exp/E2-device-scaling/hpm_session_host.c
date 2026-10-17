// SPDX-License-Identifier: GPL-2.0-only
/* Stub control registers, never a model of actual HPM event behavior. */
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

struct task_struct { unsigned int id; };
static struct task_struct tasks[2] = {{1}, {2}}, *current = &tasks[0];
static unsigned int warnings;
#define WARN_ON_ONCE(condition) ({ bool warn = !!(condition); warnings += warn; warn; })
struct mutex { bool held; };
struct riscv_iommu_device {
	unsigned char *reg;
	struct mutex hpm_lock;
	bool hpm_ready;
};
struct riscv_iommu_endpoint { struct riscv_iommu_device *iommu; u32 devid; };
struct device { struct riscv_iommu_endpoint *ep; };
static struct riscv_iommu_endpoint *riscv_iommu_hpm_endpoint(struct device *dev)
{
	return dev ? dev->ep : NULL;
}
static int mutex_trylock(struct mutex *lock)
{
	if (lock->held) return 0;
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
	if (allocation_fail) return NULL;
	allocation = calloc(1, size);
	assert(allocation);
	allocated++;
	return allocation;
}
static void kfree(void *allocation)
{
	if (allocation) { allocated--; free(allocation); }
}

enum fault { NONE, MISSING_SLOT, DROPPED_DID_FILTER, COUNTER_CLEAR_IGNORED,
	INHIBIT_FREEZE_IGNORED, INHIBIT_ENABLE_IGNORED };
struct mmio {
	unsigned char regs[0x400];
	enum fault fault;
	unsigned int slot, writes, cycle_writes, counter_writes[31], event_writes[31];
	unsigned int rollover_reg, rollover_step;
	bool rollover;
};
static struct mmio banks[2];
static struct riscv_iommu_device iommus[2];
static struct riscv_iommu_endpoint endpoints[5];
static struct device devices[6];
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
		uintptr_t base = (uintptr_t)banks[i].regs, addr = (uintptr_t)address;
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
		(reg == RISCV_IOMMU_REG_IOHPMCTR(bank->slot) ||
		 reg == RISCV_IOMMU_REG_IOHPMEVT(bank->slot));
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
		if (bank->rollover_step == 6) bank->rollover = false;
		return value;
	}
	if (missing(bank, reg & ~7U)) return 0;
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
	    (bank->fault == INHIBIT_ENABLE_IGNORED && value == 0x8000e000U)) return;
	memcpy(bank->regs + reg, &value, sizeof(value));
}
static void writeq(u64 value, void *address)
{
	unsigned int reg, i;
	struct mmio *bank = bank_of(address, &reg);
	bank->writes++;
	if (reg == RISCV_IOMMU_REG_IOHPMCYCLES) bank->cycle_writes++;
	for (i = 0; i < 31; i++) {
		if (reg == RISCV_IOMMU_REG_IOHPMCTR(i) || reg == RISCV_IOMMU_REG_IOHPMEVT(i))
			assert(readl(bank->regs + RISCV_IOMMU_REG_IOCOUNTINH) & (1U << (i + 1)));
		if (reg == RISCV_IOMMU_REG_IOHPMCTR(i)) bank->counter_writes[i]++;
		if (reg == RISCV_IOMMU_REG_IOHPMEVT(i)) bank->event_writes[i]++;
	}
	if (missing(bank, reg)) return;
	if (bank->fault == DROPPED_DID_FILTER && reg == RISCV_IOMMU_REG_IOHPMEVT(bank->slot))
		value &= ~RISCV_IOMMU_IOHPMEVT_DV_GSCV;
	if (bank->fault == COUNTER_CLEAR_IGNORED && value == 0 &&
	    reg == RISCV_IOMMU_REG_IOHPMCTR(bank->slot)) return;
	raw_write(bank, reg, value);
}

/* Exact production function bodies; only #include lines were removed. */
#include "production_hpm.c"

static void setup(void)
{
	unsigned int bank, i;
	assert(allocated == 0);
	memset(banks, 0, sizeof(banks));
	memset(iommus, 0, sizeof(iommus));
	memset(devices, 0, sizeof(devices));
	current = &tasks[0];
	warnings = 0;
	allocation_fail = 0;
	for (bank = 0; bank < 2; bank++) {
		iommus[bank].reg = banks[bank].regs;
		iommus[bank].hpm_ready = true;
		raw_write(&banks[bank], RISCV_IOMMU_REG_CAP, RISCV_IOMMU_CAP_HPM);
		raw_write(&banks[bank], RISCV_IOMMU_REG_IOCOUNTINH, 0x8000e129U);
		raw_write(&banks[bank], RISCV_IOMMU_REG_IOHPMCYCLES, UINT64_C(0x123456789));
		for (i = 0; i < 31; i++) {
			raw_write(&banks[bank], RISCV_IOMMU_REG_IOHPMEVT(i), 0x101 + i);
			raw_write(&banks[bank], RISCV_IOMMU_REG_IOHPMCTR(i), 0x100000010ULL + i);
		}
	}
	for (i = 0; i < 5; i++) {
		endpoints[i] = (struct riscv_iommu_endpoint){&iommus[i == 4], 10 + i};
		devices[i].ep = &endpoints[i];
	}
}

static void trial(unsigned int nr, bool legacy)
{
	struct device *list[] = {&devices[0], &devices[1], &devices[2], &devices[3]};
	struct riscv_iommu_hpm_session *session = NULL, *busy = NULL;
	struct riscv_iommu_hpm_sample pair;
	struct riscv_iommu_hpm_multi_sample multi;
	unsigned char saved[sizeof(banks[0].regs)];
	unsigned int i, count = legacy ? 6 : 3 * nr;
	static const unsigned int events[] = {4, 7, 8};
	u64 cycles = UINT64_C(0x80000001234567ec);
	u32 inhibit;
	setup();
	memcpy(saved, banks[0].regs, sizeof(saved));
	assert((legacy ? riscv_iommu_hpm_acquire(list[0], nr == 2 ? list[1] : NULL, &session) :
		riscv_iommu_hpm_acquire_multi(list, nr, &session)) == 0);
	assert(session && allocated == 1 && iommus[0].hpm_lock.held);
	inhibit = (0x8000e129U | (u32)GENMASK(count, 1)) &
		~((u32)GENMASK(nr * 3, 1) | 1U);
	assert(readl(banks[0].regs + RISCV_IOMMU_REG_IOCOUNTINH) == inhibit);
	for (i = 0; i < count; i++) {
		u64 did = legacy && nr == 1 ? 10 : 10 + i / 3;
		u64 selector = BIT_ULL(63) | BIT_ULL(61) | (did << 36) | events[i % 3];
		assert(raw_read(&banks[0], RISCV_IOMMU_REG_IOHPMEVT(i)) == selector);
		assert(raw_read(&banks[0], RISCV_IOMMU_REG_IOHPMCTR(i)) == 0);
		if (!legacy || nr == 2 || i < 3)
			raw_write(&banks[0], RISCV_IOMMU_REG_IOHPMCTR(i), 0x8000000000000020ULL + i);
	}
	assert(riscv_iommu_hpm_acquire_multi(list, 4, &busy) == -EBUSY && !busy && allocated == 1);
	raw_write(&banks[0], RISCV_IOMMU_REG_IOHPMCYCLES, cycles);
	memcpy(saved + RISCV_IOMMU_REG_IOHPMCYCLES, &cycles, sizeof(cycles));
	if (legacy) {
		riscv_iommu_hpm_read(session, &pair);
		assert(pair.cycles == (cycles & UINT64_C(0x7fffffffffffffff)));
		for (i = 0; i < 2; i++) {
			assert(pair.iotlb_miss[i] == (i < nr ? 0x8000000000000020ULL + 3 * i : 0));
			assert(pair.s1_ptw[i] == (i < nr ? 0x8000000000000021ULL + 3 * i : 0));
			assert(pair.s2_ptw[i] == (i < nr ? 0x8000000000000022ULL + 3 * i : 0));
		}
	} else {
		memset(&multi, 0xff, sizeof(multi));
		riscv_iommu_hpm_read_multi(session, &multi);
		assert(multi.cycles == (cycles & UINT64_C(0x7fffffffffffffff)));
		for (i = 0; i < 4; i++) {
			assert(multi.iotlb_miss[i] == (i < nr ? 0x8000000000000020ULL + 3 * i : 0));
			assert(multi.s1_ptw[i] == (i < nr ? 0x8000000000000021ULL + 3 * i : 0));
			assert(multi.s2_ptw[i] == (i < nr ? 0x8000000000000022ULL + 3 * i : 0));
		}
	}
	riscv_iommu_hpm_release(session);
	assert(allocated == 0 && !iommus[0].hpm_lock.held && !banks[0].cycle_writes);
	assert(!memcmp(saved, banks[0].regs, sizeof(saved)));
	for (i = count; i < 31; i++) assert(!banks[0].counter_writes[i] && !banks[0].event_writes[i]);
	printf("PASS %s nr=%u slots=%u read/zero-unused/restore/busy/cycles/unowned\n", legacy ? "legacy" : "multi", nr, count);
	cases++;
}

static void faults(void)
{
	struct device *list[] = {&devices[0], &devices[1], &devices[2], &devices[3]};
	struct riscv_iommu_hpm_session *session;
	unsigned char saved[sizeof(banks[0].regs)];
	unsigned int fault, slot, i;
	for (fault = MISSING_SLOT; fault <= INHIBIT_ENABLE_IGNORED; fault++) {
		for (slot = 0; slot < 12; slot++) {
			setup();
			banks[0].fault = fault;
			banks[0].slot = slot;
			memcpy(saved, banks[0].regs, sizeof(saved));
			session = (void *)1;
			assert(riscv_iommu_hpm_acquire_multi(list, 4, &session) ==
			       (fault == COUNTER_CLEAR_IGNORED ? -EIO : -EOPNOTSUPP));
			assert(!session && !allocated && !iommus[0].hpm_lock.held);
			assert(!memcmp(saved, banks[0].regs, sizeof(saved)));
			assert(!banks[0].cycle_writes);
			for (i = 12; i < 31; i++) assert(!banks[0].counter_writes[i] && !banks[0].event_writes[i]);
			if (fault != INHIBIT_FREEZE_IGNORED)
				for (i = 0; i < 12; i++) assert(banks[0].counter_writes[i] && banks[0].event_writes[i]);
			cases++;
		}
		printf("PASS fault=%u across slots0..11 full restore and no unowned writes\n", fault);
	}
}

static void invalid(void)
{
	struct device *list[] = {&devices[0], &devices[1], &devices[2], &devices[3]};
	struct riscv_iommu_hpm_session *session = (void *)1;
	setup();
	assert(riscv_iommu_hpm_acquire_multi(list, 0, &session) == -EINVAL && !session);
	assert(riscv_iommu_hpm_acquire_multi(list, 5, &session) == -EINVAL && !session);
	assert(riscv_iommu_hpm_acquire_multi(NULL, 1, &session) == -EINVAL && !session);
	assert(riscv_iommu_hpm_acquire_multi(list, 1, NULL) == -EINVAL);
	endpoints[3].devid = 10;
	assert(riscv_iommu_hpm_acquire_multi(list, 4, &session) == -EINVAL && !session);
	setup();
	list[3] = &devices[4];
	assert(riscv_iommu_hpm_acquire_multi(list, 4, &session) == -EXDEV && !session);
	list[3] = &devices[5];
	assert(riscv_iommu_hpm_acquire_multi(list, 4, &session) == -EOPNOTSUPP && !session);
	list[3] = NULL;
	assert(riscv_iommu_hpm_acquire_multi(list, 4, &session) == -EOPNOTSUPP && !session);
	assert(!allocated && !banks[0].writes && !banks[1].writes);
	setup();
	list[3] = &devices[3];
	allocation_fail = 1;
	assert(riscv_iommu_hpm_acquire_multi(list, 4, &session) == -ENOMEM && !session);
	allocation_fail = 0;
	iommus[0].hpm_ready = false;
	assert(riscv_iommu_hpm_acquire_multi(list, 4, &session) == -EOPNOTSUPP && !session);
	iommus[0].hpm_ready = true;
	raw_write(&banks[0], RISCV_IOMMU_REG_CAP, 0);
	assert(riscv_iommu_hpm_acquire_multi(list, 4, &session) == -EOPNOTSUPP && !session);
	assert(!allocated && !iommus[0].hpm_lock.held && !banks[0].writes);
	printf("PASS invalid count/list/out/device/duplicate/cross-IOMMU/allocation/not-ready/no-HPM\n");
	cases++;
}

static void owner_and_rollover(void)
{
	struct device *list[] = {&devices[3], &devices[1], &devices[0], &devices[2]};
	struct riscv_iommu_hpm_session *session = NULL;
	struct riscv_iommu_hpm_multi_sample sample, zero = {};
	struct riscv_iommu_hpm_sample pair, pair_zero = {};
	unsigned int writes;
	setup();
	assert(riscv_iommu_hpm_acquire_multi(list, 4, &session) == 0);
	assert(((raw_read(&banks[0], RISCV_IOMMU_REG_IOHPMEVT(0)) >> 36) & 0xffffff) == 13);
	writes = banks[0].writes;
	current = &tasks[1];
	memset(&sample, 0xff, sizeof(sample));
	riscv_iommu_hpm_read_multi(session, &sample);
	assert(!memcmp(&sample, &zero, sizeof(zero)));
	riscv_iommu_hpm_release(session);
	assert(warnings == 2 && allocated == 1 && iommus[0].hpm_lock.held && banks[0].writes == writes);
	current = &tasks[0];
	riscv_iommu_hpm_read(session, &pair);
	assert(!memcmp(&pair, &pair_zero, sizeof(pair)) && warnings == 3);
	banks[0].rollover = true;
	banks[0].rollover_reg = RISCV_IOMMU_REG_IOHPMCTR(11);
	riscv_iommu_hpm_read_multi(session, &sample);
	assert(sample.s2_ptw[3] == ((UINT64_C(6) << 32) | 9));
	assert(banks[0].rollover_step == 6);
	riscv_iommu_hpm_release(session);
	assert(!allocated && !iommus[0].hpm_lock.held);
	memset(&sample, 0xff, sizeof(sample));
	riscv_iommu_hpm_read_multi(NULL, &sample);
	assert(!memcmp(&sample, &zero, sizeof(zero)));
	riscv_iommu_hpm_read(NULL, &pair);
	assert(!memcmp(&pair, &pair_zero, sizeof(pair)));
	riscv_iommu_hpm_release(NULL);
	printf("PASS reordered DIDs/same-task owner/mode guards/high-low-high/NULL read-release\n");
	cases++;
}

int main(void)
{
	unsigned int nr;
	trial(1, true);
	trial(2, true);
	for (nr = 1; nr <= 4; nr++) trial(nr, false);
	faults();
	invalid();
	owner_and_rollover();
	printf("PASS total scenario iterations=%u; no board/RTL claims\n", cases);
	return 0;
}
