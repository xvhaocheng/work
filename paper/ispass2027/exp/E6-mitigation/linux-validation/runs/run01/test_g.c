/* Fixture-only host checks of the actual G helpers/capability functions. */
#include <assert.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <linux/types.h>
#include <linux/errno.h>
#include <linux/idma_g.h>
#include "idma-g.h"

#define __iomem
#define __user
#define __force
#define CONFIG_RISCV_IOMMU 1
#define CONFIG_64BIT 1
#define IS_REACHABLE(x) (x)
#define IS_ENABLED(x) (x)
#define PAGE_SIZE 4096U
#define SZ_4K 4096U
#define IDMA_BATCH_MAGIC_OFFSET 0x80
#define IDMA_OBS_MAGIC_OFFSET 0x58
#define IDMA_OBS_INDEX_OFFSET 0x70
#define IDMA_OBS_DATA_OFFSET 0x78
#define IDMA_GAP_CAP_OFFSET 0xe8

static unsigned int checks;
#define CHECK(x) do { ++checks; if (!(x)) { fprintf(stderr, "line %u: %s\n", __LINE__, #x); assert(x); } } while (0)
struct idma_device { int id; u32 did; void *reg; struct { void *parent; } miscdev; };
static struct idma_device devices[2];
static u64 registers[2][32], index_value[2], bank_magic, hist_info;
static bool locked, missing_a, hpm, copy_fault;
static int lock_error, nofault_lane;
static unsigned int writes, probes, gets, puts_count, br_count, ar_count;
static int idma_experiment_lock;

static int lane_offset(const void *address, unsigned int *offset)
{
	unsigned int lane;
	for (lane = 0; lane < 2; lane++) {
		uintptr_t p = (uintptr_t)address, base = (uintptr_t)registers[lane];
		if (p >= base && p < base + sizeof(registers[lane])) {
			*offset = p - base;
			return lane;
		}
	}
	assert(false);
	return -1;
}
static u64 readq(const void *address)
{
	unsigned int offset;
	int lane = lane_offset(address, &offset);
	CHECK(locked);
	if (offset == IDMA_OBS_DATA_OFFSET)
		return index_value[lane] == 768 ? bank_magic : hist_info;
	return registers[lane][offset / 8];
}
static void writeq(u64 value, void *address)
{
	unsigned int offset;
	int lane = lane_offset(address, &offset);
	CHECK(locked && lane == 0 && offset == IDMA_OBS_INDEX_OFFSET);
	CHECK(value == 768 || value == 192);
	index_value[lane] = value;
	writes++;
}
static int mock_nofault(u64 *value, const u64 *address)
{
	unsigned int offset;
	int lane = lane_offset(address, &offset);
	CHECK(locked && offset == IDMA_GAP_CAP_OFFSET);
	CHECK(((uintptr_t)address & 7) == 0);
	probes++;
	if (lane == nofault_lane)
		return -EFAULT;
	*value = readq(address);
	return 0;
}
#define get_kernel_nofault(value, address) mock_nofault(&(value), (address))
#define __io_br() do { br_count++; } while (0)
#define __io_ar(value) do { (void)(value); ar_count++; } while (0)
#define le64_to_cpu(value) (value)
static int mutex_lock_interruptible(int *mutex)
{
	(void)mutex;
	CHECK(!locked);
	if (lock_error)
		return lock_error;
	locked = true;
	return 0;
}
static void mutex_unlock(int *mutex) { (void)mutex; CHECK(locked); locked = false; }
static struct idma_device *idma_get_instance(u32 id)
{
	CHECK(locked && id == 1);
	if (missing_a)
		return NULL;
	gets++;
	return &devices[1];
}
static void idma_put_instance(struct idma_device *device) { if (device) puts_count++; }
static bool riscv_iommu_hpm_supported(void *parent) { (void)parent; return hpm; }
static int copy_to_user(void *target, const void *source, size_t size)
{
	CHECK(!locked);
	if (copy_fault)
		return 1;
	memcpy(target, source, size);
	return 0;
}

#include "actual-g-functions.c"

_Static_assert(sizeof(struct idma_g_caps) == 96, "G caps layout");
_Static_assert(offsetof(struct idma_g_experiment, status) == 96, "G input boundary");
_Static_assert(offsetof(struct idma_g_experiment, words) == 232, "G raw bank offset");
_Static_assert(sizeof(struct idma_g_experiment) == 680, "G request layout");
_Static_assert(_IOC_NR(IDMA_IOC_CAPS_G) == 0x11, "G CAPS number");
_Static_assert(_IOC_NR(IDMA_IOC_RUN_G) == 0x12, "G RUN number");

static struct idma_g_experiment input(bool has_a)
{
	struct idma_g_experiment p = {
		.abi_version = IDMA_G_ABI, .struct_size = sizeof(p),
		.aggressor_id = has_a ? 1 : IDMA_EXPERIMENT_NO_AGGRESSOR,
		.victim_pages = 1, .aggressor_pages = has_a ? 8 : 0,
		.chunk_bytes = 4096, .run_timeout_ms = 300000,
		.victim_transfers = 4096, .aggressor_transfers = has_a ? 4096 : 0,
		.warmup_transfers = 256,
	};
	return p;
}
static void test_input(void)
{
	struct idma_g_experiment p = input(true), q;
	unsigned int i;
	CHECK(idma_g_validate_input(&p) == 0);
	for (i = 0; i < 3; i++) {
		p.aggressor_gap_cycles = i * 1000;
		CHECK(idma_g_validate_input(&p) == 0);
	}
	q = p; q.abi_version = 5; CHECK(idma_g_validate_input(&q) == -EPROTO);
	q = p; q.struct_size--; CHECK(idma_g_validate_input(&q) == -EPROTO);
#define BAD(field, value) do { q = p; q.field = (value); CHECK(idma_g_validate_input(&q) == -EINVAL); } while (0)
	BAD(flags, 1); BAD(victim_pages, 2); BAD(aggressor_pages, 1);
	BAD(chunk_bytes, 0); BAD(chunk_bytes, 1024); BAD(victim_transfers, 4095);
	BAD(aggressor_transfers, 4097); BAD(warmup_transfers, 255);
	BAD(victim_gap_cycles, 1000); BAD(aggressor_gap_cycles, 999);
	BAD(run_timeout_ms, 0); BAD(run_timeout_ms, 300001); BAD(aggressor_id, 2);
	for (i = 0; i < 8; i++) { q = p; q.reserved[i] = 1; CHECK(idma_g_validate_input(&q) == -EINVAL); }
	p = input(false); CHECK(idma_g_validate_input(&p) == 0);
	BAD(aggressor_pages, 1); BAD(aggressor_transfers, 4096); BAD(aggressor_gap_cycles, 1000);
#undef BAD
}
static void snapshot(u64 *w, bool has_a)
{
	memset(w, 0, 32 * sizeof(*w));
	w[0] = IDMA_G_BANK_MAGIC; w[2] = has_a ? 0x3c : 0x0c;
	w[4] = w[8] = w[9] = 4096; w[10] = w[16] = 20000;
	w[27] = 20000; w[28] = 40000;
	if (has_a) {
		w[5] = w[11] = w[12] = 4096; w[13] = 40000;
		w[17] = w[18] = 2048; w[19] = w[20] = w[21] = 1;
		w[23] = 1024; w[24] = 3072; w[25] = 1024; w[26] = 3071;
		w[29] = 10000; w[30] = 50000;
	}
}
static void test_snapshot(void)
{
	u64 w[32], q[32];
	unsigned int i;
	snapshot(w, true); CHECK(idma_g_validate_snapshot(w, true, 0, 20000) == 0);
#define BAD(slot, value, mask) do { memcpy(q, w, sizeof(q)); q[slot] = (value); CHECK(idma_g_validate_snapshot(q, true, 0, 20000) & (mask)); } while (0)
	BAD(0, 0, IDMA_G_BAD_CAP); BAD(1, 1, IDMA_G_BAD_RESERVED); BAD(31, 1, IDMA_G_BAD_RESERVED);
	BAD(2, 0x2c, IDMA_G_BAD_FLAGS); BAD(2, 0x7c, IDMA_G_BAD_FLAGS);
	for (i = 0; i < 10; i++) BAD(3, 1ULL << i, IDMA_G_HARDWARE_ERROR);
	BAD(4, 0, IDMA_G_BAD_CONFIG); BAD(5, 4095, IDMA_G_BAD_CONFIG);
	BAD(6, 1, IDMA_G_BAD_CONFIG); BAD(7, 1000, IDMA_G_BAD_CONFIG);
	BAD(8, 4095, IDMA_G_BAD_BUDGET); BAD(9, 4097, IDMA_G_BAD_BUDGET);
	BAD(11, 4097, IDMA_G_BAD_BUDGET); BAD(12, 4095, IDMA_G_BAD_BUDGET);
	BAD(10, 0, IDMA_G_BAD_TIME); BAD(13, 0, IDMA_G_BAD_TIME);
	BAD(14, 1, IDMA_G_BAD_TIME); BAD(15, 1, IDMA_G_BAD_TIME);
	BAD(28, 19999, IDMA_G_BAD_TIME); BAD(30, 9999, IDMA_G_BAD_TIME);
	BAD(16, 19999, IDMA_G_BAD_TIME); BAD(17, ~0ULL, IDMA_G_BAD_WINDOW);
	BAD(19, 2, IDMA_G_BAD_WINDOW); BAD(20, 0, IDMA_G_BAD_WINDOW);
	BAD(21, 0, IDMA_G_BAD_IDS); BAD(22, 1, IDMA_G_BAD_IDS);
	BAD(23, 1025, IDMA_G_BAD_IDS); BAD(24, 3073, IDMA_G_BAD_IDS);
	BAD(25, 0, IDMA_G_BAD_IDS); BAD(26, 4097, IDMA_G_BAD_IDS);
#undef BAD
	/* A completes before V opens: zero A window work is a valid observation. */
	memcpy(q, w, sizeof(q)); memset(q + 17, 0, 10 * sizeof(u64));
	q[13] = 9000; q[29] = 1000; q[30] = 10000;
	CHECK(idma_g_validate_snapshot(q, true, 0, 20000) == 0);
	/* A starts/finishes inside V; strict carry and open-edge issue are distinct. */
	q[13] = 10000; q[29] = 25000; q[30] = 35000;
	q[17] = q[18] = 4096; q[25] = 1; q[26] = 4096;
	CHECK(idma_g_validate_snapshot(q, true, 0, 20000) == 0);
	q[17] = 4095; q[19] = 1; q[22] = 1; q[23] = 1;
	CHECK(idma_g_validate_snapshot(q, true, 0, 20000) == 0);
	q[21] = 1; CHECK(idma_g_validate_snapshot(q, true, 0, 20000) & IDMA_G_BAD_IDS);
	/* Fixed natural completion contains exactly4095 actual inter-task gaps. */
	for (i = 1; i <= 2; i++) {
		memcpy(q, w, sizeof(q)); q[7] = i * 1000;
		q[15] = 4095 * q[7]; q[13] = q[15] + 40000; q[30] = q[29] + q[13];
		CHECK(idma_g_validate_snapshot(q, true, i * 1000, 20000) == 0);
		q[15]--; CHECK(idma_g_validate_snapshot(q, true, i * 1000, 20000) & IDMA_G_BAD_TIME);
	}
	snapshot(w, false); CHECK(idma_g_validate_snapshot(w, false, 0, 20000) == 0);
	CHECK(idma_g_validate_snapshot(w, false, 1000, 20000) & IDMA_G_BAD_CONFIG);
	CHECK(idma_g_validate_snapshot(w, false, 0, 19999) & IDMA_G_BAD_TIME);
	for (i = 0; i < 32; i++) {
		static const unsigned int a_fields[] = {5,7,11,12,13,15,17,18,19,20,21,22,23,24,25,26,29,30};
		unsigned int j;
		for (j = 0; j < sizeof(a_fields)/sizeof(a_fields[0]); j++)
			if (i == a_fields[j]) { memcpy(q, w, sizeof(q)); q[i] = 1; CHECK(idma_g_validate_snapshot(q, false, 0, 20000) & IDMA_G_BAD_SOLO); }
	}
}
static void reset_caps(void)
{
	unsigned int lane;
	memset(registers, 0, sizeof(registers));
	for (lane = 0; lane < 2; lane++) {
		devices[lane].id = lane; devices[lane].did = 10 + lane;
		devices[lane].reg = registers[lane];
		registers[lane][0x80 / 8] = IDMA_G_BATCH_MAGIC;
		registers[lane][0x58 / 8] = IDMA_G_OBSERVER_MAGIC;
		registers[lane][0xe8 / 8] = IDMA_G_GAP_MAGIC;
	}
	bank_magic = IDMA_G_BANK_MAGIC; hist_info = 0x20202001;
	locked = missing_a = copy_fault = false; hpm = true;
	lock_error = 0; nofault_lane = -1;
	writes = probes = gets = puts_count = br_count = ar_count = 0;
}
static void test_caps(void)
{
	struct idma_g_caps caps;
	unsigned int lane;
	reset_caps(); CHECK(idma_caps_g(&devices[0], &caps) == 0);
	CHECK(caps.abi_version == 6 && caps.struct_size == 96 && caps.capabilities == 0x1f);
	CHECK(!(caps.capabilities & IDMA_G_CAP_RUN_READY));
	CHECK(caps.batch_magic == IDMA_G_BATCH_MAGIC && caps.observer_magic == IDMA_G_OBSERVER_MAGIC);
	CHECK(caps.gap_magic == IDMA_G_GAP_MAGIC && caps.bank_magic == IDMA_G_BANK_MAGIC);
	CHECK(caps.bank_first == 768 && caps.bank_words == 32 && caps.reserved == 0);
	CHECK(writes == 2 && probes == 2 && gets == puts_count && br_count == 2 && ar_count == 2 && !locked);
	reset_caps(); hpm = false; CHECK(idma_caps_g(&devices[0], &caps) == 0); CHECK(caps.capabilities == 0x1e);
	reset_caps(); CHECK(idma_caps_g(&devices[1], &caps) == -EOPNOTSUPP); CHECK(writes == 0 && gets == 0);
	reset_caps(); lock_error = -EINTR; CHECK(idma_caps_g(&devices[0], &caps) == -EINTR); CHECK(writes == 0 && gets == 0);
	reset_caps(); missing_a = true; CHECK(idma_caps_g(&devices[0], &caps) == -EOPNOTSUPP); CHECK(!locked);
	for (lane = 0; lane < 2; lane++) {
		reset_caps(); registers[lane][0x80 / 8]++;
		CHECK(idma_caps_g(&devices[0], &caps) == -EOPNOTSUPP); CHECK(writes == 0 && probes == 0 && gets == puts_count);
		reset_caps(); nofault_lane = lane;
		CHECK(idma_caps_g(&devices[0], &caps) == -EOPNOTSUPP); CHECK(writes == 0 && gets == puts_count && br_count == ar_count);
		reset_caps(); registers[lane][0xe8 / 8] = 0;
		CHECK(idma_caps_g(&devices[0], &caps) == -EOPNOTSUPP); CHECK(writes == 0 && gets == puts_count);
	}
	reset_caps(); registers[0][0x58 / 8]++;
	CHECK(idma_caps_g(&devices[0], &caps) == -EOPNOTSUPP); CHECK(writes == 0 && probes == 0);
	reset_caps(); bank_magic = 0;
	CHECK(idma_caps_g(&devices[0], &caps) == -EOPNOTSUPP); CHECK(writes == 2 && gets == puts_count);
	reset_caps(); hist_info = 0x20202002;
	CHECK(idma_caps_g(&devices[0], &caps) == -EOPNOTSUPP);
	reset_caps(); hist_info |= 1ULL << 48;
	CHECK(idma_caps_g(&devices[0], &caps) == -EOPNOTSUPP);
	reset_caps(); hist_info |= 0xffffULL << 32;
	CHECK(idma_caps_g(&devices[0], &caps) == 0); /* Sticky prior overflow is not a capability change. */
	reset_caps(); copy_fault = true;
	CHECK(idma_caps_g(&devices[0], &caps) == -EFAULT); CHECK(!locked && gets == puts_count);
}
int main(void)
{
	test_input(); test_snapshot(); test_caps();
	printf("fixture_only: %u checks passed; no DMA, board or performance data\n", checks);
	return 0;
}
