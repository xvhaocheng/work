## Summary

Both files are corrected for review points **A–G** only. Production (`iommu.c`, `iommu.h`, `iommu-bits.h`) is untouched; the s64 snapshot correction already present at `iommu.c:2173` is matched by a host `s64` typedef.

---

## `tests/test_iommu_s1_flush.py`

Only `domain_type_definitions()` (lines 48–70) changes: it now walks lines with an explicit index, keeps backslash continuations **and** treats an unterminated `/* … */` as a continuation, so `__IOMMU_DOMAIN_DMA_API` (whose comment closes on the next physical line at `linux/include/linux/iommu.h:61-62`) no longer swallows `#define __IOMMU_DOMAIN_PT`. Everything else (helper extraction, `without_includes`, compile flags `-Wall -Wextra -Werror`, sha256 print, run of the binary) is unchanged.

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

    A macro definition is continued while its last physical line ends with a
    backslash and while a trailing /* ... */ block comment it opened is still
    unterminated. Copying such a multi-line comment whole keeps the extracted
    block valid C: a definition whose comment closes on the next line would
    otherwise swallow the following #define.
    """
    lines = text.splitlines()
    definitions = []
    index = 0
    total = len(lines)

    while index < total:
        line = lines[index]
        if not re.match(r"#define\s+_*IOMMU_DOMAIN_", line):
            index += 1
            continue

        block = [line]
        index += 1
        while True:
            joined = "\n".join(block)
            continued = block[-1].rstrip().endswith("\\")
            comment_open = joined.count("/*") > joined.count("*/")
            if not continued and not comment_open:
                break
            assert index < total, "unterminated IOMMU_DOMAIN_* definition"
            block.append(lines[index])
            index += 1

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
            str(ROOT / "tests/test_iommu_s1_flush.c"), "-o", str(binary),
        ], check=True)
        subprocess.run([str(binary)], check=True)
    print(f"Production iommu.c (S1 helpers) SHA256: "
          f"{hashlib.sha256(contents.encode()).hexdigest()}")
    print("Host control/command/lock checks only; no RTL or board result.")


if __name__ == "__main__":
    main()
```

Trace over `linux/include/linux/iommu.h`: l60 single (`/* … */` balanced), l61–62 joined as one block (comment closed on l62), l63 single, l64 single, l66 single, l68 single, l85–87 single, l88–89 joined by backslash, l90–92 joined, l93 single. `__IOMMU_DOMAIN_PT` is therefore emitted and `IOMMU_DOMAIN_IDENTITY` resolves.

---

## `tests/test_iommu_s1_flush.c`

Changes by review point:

- **B** — `#include <stdlib.h>` added (for `abort()`).
- **C** — `typedef int64_t s64;` added so the production `s64 seq` (`iommu.c:2173`) matches the `long` host `atomic64_read()`; `atomic64_t` stays `long`.
- **D** — `reject()` split into `reject_unlocked()` (no lock may be taken) and `reject_locked(..., locks)` (locks must have been taken/released, nothing posted). All `-EINVAL`/`-EOPNOTSUPP`/`-EXDEV`/`-ESTALE` sites at/above pre-lock validation use the former; every check-ep/DC/CQ `-ESTALE`/`-EOPNOTSUPP`/`-EXDEV`/`-EIO` site uses the latter with `1`.
- **E** — lock trace is now per-call: `run()` resets `lock_seq_nr`/`lock_stack_nr`, arrays bounded by `LOCK_TRACE_MAX 4`, and the `base = lock_seq_nr` arithmetic is replaced with absolute per-call counts; ascending-DID (`lock_seq[0]==10`, `lock_seq[1]==11`) and `nested_lock_seen` assertions preserved for both target orders.
- **F** — `spin_lock_irqsave`/`spin_unlock_irqrestore` are now macros that assign and check the caller's `flags` (via `host_irq_save()`/`host_irq_restore()`), matching the kernel macro that writes `flags` before it is read; production body untouched.
- **G** — the producer hook now advances both `cmdq.lui` and hardware `CQT` under the lock; the test resets both afterwards.

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
#include <stdlib.h>
#include <string.h>

typedef uint8_t u8;
typedef uint32_t u32;
typedef uint64_t u64;
typedef int64_t s64;
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

/*
 * Lock ordering / nesting observations. run() resets the trace before every
 * production call, so it is bounded by the maximum nesting of a single call
 * (two endpoint mutexes) instead of accumulating over the whole test.
 */
#define LOCK_TRACE_MAX 4
static struct mutex *lock_seq[LOCK_TRACE_MAX];
static unsigned int lock_seq_nr;
static struct mutex *lock_stack[LOCK_TRACE_MAX];
static unsigned int lock_stack_nr;
static bool nested_lock_seen;

/*
 * Host model of a legitimate concurrent producer that takes cq_lock
 * immediately after it is released. It must not invalidate a comparison that
 * was already made while the lock was held.
 */
static struct riscv_iommu_device *producer_after_unlock;
static u32 producer_lui;

static void mutex_lock(struct mutex *lock)
{
	assert(!lock->held);
	assert(lock_seq_nr < LOCK_TRACE_MAX && lock_stack_nr < LOCK_TRACE_MAX);
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

/*
 * Interrupt-state halves of spin_lock_irqsave()/spin_unlock_irqrestore().
 * The host model always runs with interrupts enabled, so the saved value is a
 * constant the restore half checks.
 */
static unsigned long host_irq_save(void)
{
	return 0UL;
}

static void host_irq_restore(unsigned long flags)
{
	assert(flags == 0UL);
}

static void host_cq_lock_acquire(spinlock_t *lock)
{
	assert(!lock->held);
	lock->held = true;
}

static void host_cq_lock_release(spinlock_t *lock)
{
	assert(lock->held);
	lock->held = false;

	if (producer_after_unlock) {
		/*
		 * A legitimate producer holding cq_lock advances the software
		 * write cursor and the hardware tail together.
		 */
		producer_after_unlock->cmdq.lui = producer_lui;
		riscv_iommu_writel(producer_after_unlock, RISCV_IOMMU_REG_CQT,
				   producer_lui);
		producer_after_unlock = NULL;
	}
}

/*
 * spin_lock_irqsave()/spin_unlock_irqrestore() are kernel macros: the lock
 * helper *assigns* the saved interrupt state into the caller's flags variable.
 * Model them with macros so the production local flags is written before it is
 * ever read, instead of passing an uninitialized value as a function argument.
 */
#define spin_lock_irqsave(lock, flags)		\
	do {					\
		(flags) = host_irq_save();	\
		host_cq_lock_acquire(lock);	\
	} while (0)

#define spin_unlock_irqrestore(lock, flags)	\
	do {					\
		host_irq_restore(flags);	\
		host_cq_lock_release(lock);	\
	} while (0)

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
		dcs[i].iohgatm = 0;
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
	lock_seq_nr = lock_stack_nr = 0;
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

/*
 * Rejection decided before any endpoint lock is taken: argument checks and the
 * cheap identity validation (endpoint lookup, IOMMU and device-id match). No
 * lock may be acquired and nothing may be posted.
 */
static void reject_unlocked(unsigned int iommu,
			    const struct riscv_iommu_s1_flush_target *targets,
			    unsigned int nr_targets, int expected)
{
	assert(run(iommu, targets, nr_targets) == expected);
	assert(!post_count && !fence_count);
	assert(lock_seq_nr == 0 && !nested_lock_seen);
}

/*
 * Rejection decided while holding the endpoint lock(s): live-domain, device
 * context and command-queue validation deliberately run under the locks. The
 * expected number of locks must have been taken and (checked by run()) all
 * released again, with no command or fence issued.
 */
static void reject_locked(unsigned int iommu,
			  const struct riscv_iommu_s1_flush_target *targets,
			  unsigned int nr_targets, int expected,
			  unsigned int locks)
{
	assert(run(iommu, targets, nr_targets) == expected);
	assert(!post_count && !fence_count);
	assert(lock_seq_nr == locks);
}

int main(void)
{
	struct riscv_iommu_s1_flush_target t[2];
	u64 expected;

	setup();

	/* ---- success: exactly one global VMA command, then a fence ---- */
	t[0] = mk(0);
	assert(run(0, t, 1) == 0);
	assert(post_count == 1 && fence_count == 1);
	assert(lock_seq_nr == 1 && !nested_lock_seen);
	assert(ep_lock_devid(lock_seq[0]) == 10);
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
	assert(run(0, t, 2) == 0);
	assert(post_count == 1 && fence_count == 1);
	assert(lock_seq_nr == 2);
	assert(ep_lock_devid(lock_seq[0]) == 10);
	assert(ep_lock_devid(lock_seq[1]) == 11);
	assert(nested_lock_seen);

	t[0] = mk(1);
	t[1] = mk(0);
	assert(run(0, t, 2) == 0);
	assert(post_count == 1 && fence_count == 1);
	assert(lock_seq_nr == 2);
	assert(ep_lock_devid(lock_seq[0]) == 10);
	assert(ep_lock_devid(lock_seq[1]) == 11);
	assert(nested_lock_seen);

	/* ---- parameter rejection (decided before any lock) ---- */
	t[0] = mk(0);
	assert(run(0, t, 0) == -EINVAL);
	assert(lock_seq_nr == 0);
	assert(run(0, t, 3) == -EINVAL);
	assert(run(0, NULL, 1) == -EINVAL);
	t[0] = mk(0);
	t[0].dev = NULL;
	reject_unlocked(0, t, 1, -EINVAL);
	t[0] = mk(0);
	t[0].domain = NULL;
	reject_unlocked(0, t, 1, -EINVAL);
	t[0] = mk(0);
	t[1] = mk(0);
	reject_unlocked(0, t, 2, -EINVAL);
	t[0] = mk(0);
	t[1] = mk(1);
	t[1].did = t[0].did;
	reject_unlocked(0, t, 2, -EINVAL);

	/* ---- identity / snapshot ---- */
	t[0] = mk(0);
	devices[0].s1_ep = NULL;
	reject_unlocked(0, t, 1, -EOPNOTSUPP);
	devices[0].s1_ep = &eps[0];

	reject_unlocked(1, t, 1, -EXDEV);

	t[0] = mk(0);
	t[0].did = eps[0].devid + 1;
	reject_unlocked(0, t, 1, -ESTALE);

	/* Under the endpoint lock: the live domain must match the snapshot. */
	t[0] = mk(0);
	eps[0].domain = &domains[1];
	reject_locked(0, t, 1, -ESTALE, 1);

	eps[0].domain = NULL;
	reject_locked(0, t, 1, -ESTALE, 1);
	eps[0].domain = &domains[0];

	devices[0].live = &domains[1].domain;
	reject_locked(0, t, 1, -ESTALE, 1);
	devices[0].live = NULL;
	reject_locked(0, t, 1, -ESTALE, 1);
	devices[0].live = &domains[0].domain;

	/* ---- domain shape (decided while holding ep->lock) ---- */
	t[0] = mk(0);
	domains[0].domain.type = IOMMU_DOMAIN_IDENTITY;
	reject_locked(0, t, 1, -EOPNOTSUPP, 1);
	domains[0].domain.type = IOMMU_DOMAIN_BLOCKED;
	reject_locked(0, t, 1, -EOPNOTSUPP, 1);
	domains[0].domain.type = IOMMU_DOMAIN_SVA;
	reject_locked(0, t, 1, -EOPNOTSUPP, 1);
	domains[0].domain.type = IOMMU_DOMAIN_DMA;

	domains[0].domain.ops = NULL;
	reject_locked(0, t, 1, -ESTALE, 1);
	domains[0].domain.ops = &riscv_iommu_domain_ops;

	domains[0].iommu = &iommus[1];
	reject_locked(0, t, 1, -EXDEV, 1);
	domains[0].iommu = &iommus[0];

	domains[0].g_stage = true;
	reject_locked(0, t, 1, -EOPNOTSUPP, 1);
	domains[0].g_stage = false;
	domains[0].nested = &domains[1];
	reject_locked(0, t, 1, -EOPNOTSUPP, 1);
	domains[0].nested = NULL;
	eps[0].pasid_enabled = true;
	reject_locked(0, t, 1, -EOPNOTSUPP, 1);
	eps[0].pasid_enabled = false;

	domains[0].mode = RISCV_IOMMU_DC_FSC_MODE_BARE;
	reject_locked(0, t, 1, -EOPNOTSUPP, 1);
	domains[0].mode = RISCV_IOMMU_DC_FSC_IOSATP_MODE_SV48;
	reject_locked(0, t, 1, -EOPNOTSUPP, 1);
	domains[0].mode = RISCV_IOMMU_DC_FSC_IOSATP_MODE_SV39;

	domains[0].pgd_root = NULL;
	reject_locked(0, t, 1, -EOPNOTSUPP, 1);
	domains[0].pgd_root = (pgd_t *)pgd_pages[0];

	/* ---- device context ---- */
	eps[0].dc = NULL;
	reject_locked(0, t, 1, -EOPNOTSUPP, 1);
	eps[0].dc = &dcs[0];

	dcs[0].tc = cpu_to_le64(0);
	reject_locked(0, t, 1, -EOPNOTSUPP, 1);
	dcs[0].tc = cpu_to_le64(RISCV_IOMMU_DC_TC_V | RISCV_IOMMU_DC_TC_EN_ATS);
	reject_locked(0, t, 1, -EOPNOTSUPP, 1);
	dcs[0].tc = cpu_to_le64(RISCV_IOMMU_DC_TC_V | RISCV_IOMMU_DC_TC_EN_PRI);
	reject_locked(0, t, 1, -EOPNOTSUPP, 1);
	dcs[0].tc = cpu_to_le64(RISCV_IOMMU_DC_TC_V | RISCV_IOMMU_DC_TC_PDTV);
	reject_locked(0, t, 1, -EOPNOTSUPP, 1);
	dcs[0].tc = cpu_to_le64(RISCV_IOMMU_DC_TC_V | RISCV_IOMMU_DC_TC_T2GPA);
	reject_locked(0, t, 1, -EOPNOTSUPP, 1);
	dcs[0].tc = cpu_to_le64(RISCV_IOMMU_DC_TC_V | RISCV_IOMMU_DC_TC_SXL);
	reject_locked(0, t, 1, -EOPNOTSUPP, 1);
	dcs[0].tc = cpu_to_le64(RISCV_IOMMU_DC_TC_V);

	dcs[0].iohgatp = cpu_to_le64(
			FIELD_PREP(RISCV_IOMMU_DC_IOHGATP_MODE,
				   RISCV_IOMMU_DC_IOHGATP_MODE_SV39X4));
	reject_locked(0, t, 1, -EOPNOTSUPP, 1);
	dcs[0].iohgatp = 0;

	dcs[0].fsc = cpu_to_le64(
			FIELD_PREP(RISCV_IOMMU_DC_FSC_MODE,
				   RISCV_IOMMU_DC_FSC_MODE_BARE) |
			FIELD_PREP(RISCV_IOMMU_DC_FSC_PPN,
				   virt_to_pfn(domains[0].pgd_root)));
	reject_locked(0, t, 1, -ESTALE, 1);

	dcs[0].fsc = cpu_to_le64(
			FIELD_PREP(RISCV_IOMMU_DC_FSC_MODE,
				   RISCV_IOMMU_DC_FSC_IOSATP_MODE_SV39) |
			FIELD_PREP(RISCV_IOMMU_DC_FSC_PPN,
				   virt_to_pfn(pgd_pages[1])));
	reject_locked(0, t, 1, -ESTALE, 1);

	dcs[0].fsc = cpu_to_le64(
			FIELD_PREP(RISCV_IOMMU_DC_FSC_MODE,
				   RISCV_IOMMU_DC_FSC_IOSATP_MODE_SV39) |
			FIELD_PREP(RISCV_IOMMU_DC_FSC_PPN,
				   virt_to_pfn(domains[0].pgd_root)));

	/* ---- command queue gating (under the endpoint lock) ---- */
	t[0] = mk(0);

	reg_write(0, RISCV_IOMMU_REG_CQCSR, RISCV_IOMMU_CQCSR_CQON);
	reject_locked(0, t, 1, -EIO, 1);
	reg_write(0, RISCV_IOMMU_REG_CQCSR, RISCV_IOMMU_CQCSR_CQEN);
	reject_locked(0, t, 1, -EIO, 1);
	reg_write(0, RISCV_IOMMU_REG_CQCSR,
		  RISCV_IOMMU_CQCSR_CQEN | RISCV_IOMMU_CQCSR_CQON |
		  RISCV_IOMMU_CQCSR_CQMF);
	reject_locked(0, t, 1, -EIO, 1);
	reg_write(0, RISCV_IOMMU_REG_CQCSR,
		  RISCV_IOMMU_CQCSR_CQEN | RISCV_IOMMU_CQCSR_CQON |
		  RISCV_IOMMU_CQCSR_CMD_ILL);
	reject_locked(0, t, 1, -EIO, 1);
	reg_write(0, RISCV_IOMMU_REG_CQCSR,
		  RISCV_IOMMU_CQCSR_CQEN | RISCV_IOMMU_CQCSR_CQON |
		  RISCV_IOMMU_CQCSR_CMD_TO);
	reject_locked(0, t, 1, -EIO, 1);
	reg_write(0, RISCV_IOMMU_REG_CQCSR,
		  RISCV_IOMMU_CQCSR_CQEN | RISCV_IOMMU_CQCSR_CQON);

	reg_write(0, RISCV_IOMMU_REG_CQT, 1);
	reject_locked(0, t, 1, -EIO, 1);
	reg_write(0, RISCV_IOMMU_REG_CQT, 0);

	iommus[0].cmdq.base = NULL;
	reject_locked(0, t, 1, -EIO, 1);
	iommus[0].cmdq.base = cmdq_mem[0];
	iommus[0].cmdq.cnt = 0;
	reject_locked(0, t, 1, -EIO, 1);
	iommus[0].cmdq.cnt = 6;
	reject_locked(0, t, 1, -EIO, 1);
	iommus[0].cmdq.cnt = 8;
	iommus[0].cmdq.lui = 8;
	reject_locked(0, t, 1, -EIO, 1);
	iommus[0].cmdq.lui = 0;

	/*
	 * Producer consistency: a legitimate producer that takes cq_lock right
	 * after it is released advances both the software cursor and the
	 * hardware tail, and must not invalidate the comparison that was
	 * already made while holding the lock.
	 */
	producer_after_unlock = &iommus[0];
	producer_lui = 3;
	assert(run(0, t, 1) == 0);
	assert(post_count == 1 && fence_count == 1);
	assert(iommus[0].cmdq.lui == 3);
	assert(riscv_iommu_readl(&iommus[0], RISCV_IOMMU_REG_CQT) == 3);
	iommus[0].cmdq.lui = 0;
	reg_write(0, RISCV_IOMMU_REG_CQT, 0);

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

> Correction to the block above: the line `dcs[i].iohgatm = 0;` in `setup()` is a typo and must **not** be present; `setup()` should contain only `dcs[i].tc = …; dcs[i].iohgatp = 0; dcs[i].fsc = …;`. Remove that stray line.

---

## Assumptions

- `s64 seq` at `linux/drivers/iommu/riscv/iommu.c:2173` is the Codex-applied production correction; on the LP64 host `long` and `int64_t` are the same type, so `atomic64_read()` (still `long`) compares cleanly.
- All rejection sites after `riscv_iommu_flush_s1_checked()`'s pre-lock block (lines 2176–2196) are single-target, so exactly one `ep->lock` is taken before the `-ESTALE`/`-EXDEV`/`-EOPNOTSUPP`/`-EIO` returns; no two-target locked-rejection call site exists in the suite.
- `tests/test_iommu_s1_flush.c` is compiled with asserts enabled (no `-DNDEBUG`), which the harness depends on.

## Suggested acceptance checks (not executed here)

1. `python3 tests/test_iommu_s1_flush.py` — compiles clean under `-Wall -Wextra -Werror` and the binary prints `PASS: …` (the earlier `-Werror=comment`, `-Werror=sign-compare`, and `abort`/`builtin-declaration-mismatch` diagnostics are gone).
2. Grep the generated temp header (or re-run `domain_type_definitions` on `linux/include/linux/iommu.h`) and confirm both `__IOMMU_DOMAIN_PT` and `IOMMU_DOMAIN_IDENTITY` are present.
3. Confirm `linux/drivers/iommu/riscv/iommu.c`, `iommu.h`, `iommu-bits.h` are unmodified relative to the applied production diff.
4. Confirm no test asserts a lock count of `0` for a `-EIO`/late-`-ESTALE`/`-EOPNOTSUPP` case (locked rejections always require one lock).

## Unresolved / for Codex

- Whether the emitter should also drop the incidental `IOMMU_DOMAIN_ALLOC_FLAGS` definition (harmless, but not used by the harness). I kept it as a faithful copy of the header.
- Whether the two-target locked-rejection path (currently unexercised) should get explicit coverage; not added, as it is outside the A–G review points.