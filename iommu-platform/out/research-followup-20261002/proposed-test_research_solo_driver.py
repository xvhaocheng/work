#!/usr/bin/env python3
# SPDX-License-Identifier: GPL-2.0
"""Host harness for the iDMA research Solo adapter and its validators.

This test extracts the *real* kernel source of

  * ``idma_research_validate`` and ``idma_run_research`` from
    ``linux/drivers/misc/idma_research.h`` and
  * ``idma_validate`` and ``idma_p4m_validate`` from
    ``linux/drivers/misc/idma.c``

and compiles them, unchanged, into a host program with small stubs.  It then
drives ``idma_run_research`` the way the ioctl does and checks the argument
adaptation plus the two validators.

What this test is *not*: it never touches an FPGA, never starts a DMA engine
and never observes real hardware, so it makes no claim about the board.  The
stub for ``idma_run_session`` only runs the real ``idma_validate`` and records
the adapted experiment fields, which is exactly the contract the Solo condition
depends on (``a_pages == 0`` must still be a positive legacy page count while
``aggressor_id == NO_AGGRESSOR`` stays, so no A lane is ever allocated).  The
adapter source is taken verbatim; the only edit is a mutation that removes the
Solo page-count line to prove that these checks detect its absence.  No fix
logic is reimplemented here.  Codex must still review the real
``idma_run_session`` directly to confirm no A allocation.
"""

from __future__ import annotations

import os
import shutil
import subprocess
import tempfile
import unittest
from pathlib import Path

REPO_ROOT = Path(__file__).resolve().parents[1]
RESEARCH_H_PATH = REPO_ROOT / "linux" / "drivers" / "misc" / "idma_research.h"
IDMA_C_PATH = REPO_ROOT / "linux" / "drivers" / "misc" / "idma.c"

# The Solo page-count compatibility line the adapter is expected to carry, and
# the pre-fix line the mutation restores to prove the checks are meaningful.
FIXED_PAGE_LINE = "e->aggressor_pages = r->a_pages ? r->a_pages : 1;"
OLD_PAGE_LINE = "e->aggressor_pages = r->a_pages;"


PREAMBLE = r'''
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef uint32_t __u32;
typedef uint64_t __u64;
typedef int32_t __s32;
typedef __u64 __aligned_u64;

#define __user
#define PAGE_SIZE 4096UL
#define SZ_4K 4096UL
#define EINPROGRESS (-115)

#define IDMA_EXPERIMENT_ABI 1U
#define IDMA_EXPERIMENT_MAX_TRANSFERS (1ULL << 32)
#define IDMA_EXPERIMENT_NO_AGGRESSOR ((__u32)~0U)
#define IDMA_BATCH_MAX_TRANSFERS (1ULL << 20)
#define IDMA_RESEARCH_ABI 1U
#define IDMA_RESEARCH_E2 1U
#define IDMA_RESEARCH_COST 2U
#define IDMA_RESEARCH_RAW_WORDS 4096U
#define IDMA_MAX_PAGES 4096U
#define IDMA_MAX_RUN_TIMEOUT_MS 300000U

struct idma_experiment_stats {
	__u64 submitted;
	__u64 completed;
	__u64 completed_bytes;
	__u64 active_cycles;
	__u64 iotlb_miss;
	__u64 s1_ptw;
	__u64 s2_ptw;
	__u64 refill_gap_ns_sum;
	__u64 refill_gap_ns_max;
	__u64 refill_gap_samples;
	__u32 in_flight_at_start;
	__u32 in_flight_at_end;
	__u32 data_errors;
	__u32 quarantined;
};

struct idma_experiment {
	__u32 abi_version;
	__u32 struct_size;
	__u32 flags;
	__u32 aggressor_id;
	__u32 victim_pages;
	__u32 aggressor_pages;
	__u32 chunk_bytes;
	__u32 run_timeout_ms;
	__u64 victim_transfers;
	__u64 warmup_transfers;
	__u32 reserved[8];
	__s32 status;
	__u32 victim_did;
	__u32 aggressor_did;
	__u32 hpm_configured;
	__u32 measurement_started;
	__u32 measurement_complete;
	__u64 victim_elapsed_ns;
	__u64 snapshot_before_ns;
	__u64 snapshot_after_ns;
	__u64 drain_ns;
	struct idma_experiment_stats victim;
	struct idma_experiment_stats aggressor;
};

struct idma_p4m {
	struct idma_experiment experiment;
	__u32 aggressor_gap_cycles;
	__u32 reserved[7];
	__u64 sequence;
	__u64 observer_status;
	__u64 observation[35];
	__u64 window[16];
};

struct idma_research {
	__u32 abi_version, struct_size;
	__u32 mode, a_pages, a_transfers, window_cycles;
	__u32 warmup, transfers, gap_cycles, timeout_ms;
	__u32 raw_words, reserved[5];
	__aligned_u64 raw_user;
	__s32 status, flush_status;
	__u32 stage, raw_captured;
	__u64 observer_magic;
	__u64 mapping[4];
	struct idma_experiment_stats phase_stats[3];
	struct idma_p4m p4m;
};

struct idma_device {
	int id;
	__u32 did;
};

struct idma_e3_pair;
struct idma_e3_diag_round;

static int idma_validate(const struct idma_experiment *e);
static int idma_p4m_validate(struct idma_device *victim, const struct idma_p4m *p);
static int idma_research_validate(struct idma_device *v, const struct idma_research *r);
static long idma_run_research(struct idma_device *victim, void *uarg);

static void *xmemchr_inv(const void *start, int c, size_t bytes)
{
	const unsigned char *p = start;
	size_t i;

	for (i = 0; i < bytes; i++)
		if (p[i] != (unsigned char)c)
			return (void *)(p + i);
	return NULL;
}
#define memchr_inv xmemchr_inv
#define access_ok(addr, size) (1)
#define u64_to_user_ptr(x) ((void *)(uintptr_t)(x))
#define IS_ERR(p) (0)
#define PTR_ERR(p) (0)
#define kfree(p) free(p)

static void *xmemdup_user(const void *src, size_t size)
{
	void *p = malloc(size);

	if (p)
		memcpy(p, src, size);
	return p;
}
#define memdup_user xmemdup_user

static int xcopy_to_user(void *dst, const void *src, size_t size)
{
	memcpy(dst, src, size);
	return 0;
}
#define copy_to_user(dst, src, size) xcopy_to_user((void *)(dst), (const void *)(src), (size))

static __u64 g_raw[IDMA_RESEARCH_RAW_WORDS];
static int g_failures;
static int g_session_calls;
static int g_e2_calls;
static __u32 g_aggressor_id;
static __u32 g_aggressor_pages;
static __u32 g_victim_pages;
static __u64 g_victim_transfers;
static __u64 g_warmup_transfers;
static __u32 g_chunk_bytes;
static __u32 g_run_timeout_ms;

static int idma_research_e2(struct idma_device *victim, struct idma_research *r)
{
	(void)victim;
	(void)r;
	g_e2_calls++;
	return 0;
}

/*
 * Stub for the real idma_run_session.  It records the adapted experiment and then
 * runs the real idma_validate, which is the first thing the real session does.
 */
static long idma_run_session(struct idma_device *victim,
			     struct idma_experiment *request, bool batch,
			     struct idma_e3_pair *pair,
			     struct idma_e3_diag_round *diagnostic,
			     bool queue_pair, struct idma_p4m *p4m,
			     struct idma_research *research)
{
	(void)victim;
	(void)batch;
	(void)pair;
	(void)diagnostic;
	(void)queue_pair;
	(void)p4m;
	(void)research;
	g_session_calls++;
	g_aggressor_id = request->aggressor_id;
	g_aggressor_pages = request->aggressor_pages;
	g_victim_pages = request->victim_pages;
	g_victim_transfers = request->victim_transfers;
	g_warmup_transfers = request->warmup_transfers;
	g_chunk_bytes = request->chunk_bytes;
	g_run_timeout_ms = request->run_timeout_ms;
	return idma_validate(request);
}
'''


MAIN = r'''
static void check(int condition, const char *message)
{
	if (!condition) {
		printf("FAIL: %s\n", message);
		g_failures++;
	}
}

static void init_cost(struct idma_research *r, __u32 a_pages, __u32 transfers, __u32 warmup)
{
	memset(r, 0, sizeof(*r));
	r->abi_version = IDMA_RESEARCH_ABI;
	r->struct_size = sizeof(*r);
	r->mode = IDMA_RESEARCH_COST;
	r->a_pages = a_pages;
	r->a_transfers = 0;
	r->window_cycles = 0;
	r->warmup = warmup;
	r->transfers = transfers;
	r->gap_cycles = 0;
	r->timeout_ms = 10000;
	r->raw_words = IDMA_RESEARCH_RAW_WORDS;
	r->raw_user = (__u64)(uintptr_t)g_raw;
}

static void init_e2(struct idma_research *r, __u32 a_pages, __u32 a_transfers)
{
	memset(r, 0, sizeof(*r));
	r->abi_version = IDMA_RESEARCH_ABI;
	r->struct_size = sizeof(*r);
	r->mode = IDMA_RESEARCH_E2;
	r->a_pages = a_pages;
	r->a_transfers = a_transfers;
	r->window_cycles = 5000000;
	r->warmup = 2;
	r->transfers = 1;
	r->gap_cycles = 0;
	r->timeout_ms = 2000;
	r->raw_words = IDMA_RESEARCH_RAW_WORDS;
	r->raw_user = (__u64)(uintptr_t)g_raw;
}

int main(void)
{
	struct idma_device victim;
	struct idma_research r;
	long ret;

	memset(&victim, 0, sizeof(victim));
	victim.id = 0;
	victim.did = 10;

	/* Solo, short parameters: positive legacy count, NO_AGGRESSOR preserved. */
	init_cost(&r, 0, 64, 8);
	g_session_calls = 0;
	ret = idma_run_research(&victim, (void *)&r);
	check(ret == 0, "solo short: idma_run_research must succeed");
	check(g_session_calls == 1, "solo short: session must be reached once");
	check(g_aggressor_id == IDMA_EXPERIMENT_NO_AGGRESSOR, "solo short: NO_AGGRESSOR must be preserved");
	check(g_aggressor_pages == 1, "solo short: legacy page count must be 1");
	check(g_victim_pages == 1, "solo short: victim pages must be 1");
	check(g_victim_transfers == 64, "solo short: transfers preserved");
	check(g_warmup_transfers == 8, "solo short: warmup preserved");
	check(g_chunk_bytes == PAGE_SIZE, "solo short: chunk bytes preserved");
	check(g_run_timeout_ms == 10000, "solo short: timeout preserved");
	check(r.stage == 5, "solo short: stage must be the completed stage");

	/* Solo, main parameters. */
	init_cost(&r, 0, 4096, 256);
	g_session_calls = 0;
	ret = idma_run_research(&victim, (void *)&r);
	check(ret == 0, "solo main: idma_run_research must succeed");
	check(g_session_calls == 1, "solo main: session must be reached once");
	check(g_aggressor_id == IDMA_EXPERIMENT_NO_AGGRESSOR, "solo main: NO_AGGRESSOR must be preserved");
	check(g_aggressor_pages == 1, "solo main: legacy page count must be 1");
	check(r.stage == 5, "solo main: stage must be the completed stage");

	/* Non-solo page counts are unchanged. */
	init_cost(&r, 1, 64, 8);
	g_session_calls = 0;
	ret = idma_run_research(&victim, (void *)&r);
	check(ret == 0, "a1: must succeed");
	check(g_aggressor_id == 1, "a1: aggressor id must be 1");
	check(g_aggressor_pages == 1, "a1: page count must stay 1");

	init_cost(&r, 8, 64, 8);
	g_session_calls = 0;
	ret = idma_run_research(&victim, (void *)&r);
	check(ret == 0, "a8: must succeed");
	check(g_aggressor_pages == 8, "a8: page count must stay 8");

	init_cost(&r, 16, 64, 8);
	g_session_calls = 0;
	ret = idma_run_research(&victim, (void *)&r);
	check(ret == 0, "a16: must succeed");
	check(g_aggressor_pages == 16, "a16: page count must stay 16");

	/* Illegal COST inputs are refused before the session is reached. */
	init_cost(&r, 2, 64, 8);
	g_session_calls = 0;
	ret = idma_run_research(&victim, (void *)&r);
	check(ret != 0, "cost a_pages=2 must be rejected");
	check(g_session_calls == 0, "cost a_pages=2 must not reach the session");

	init_cost(&r, 8, 100, 8);
	g_session_calls = 0;
	ret = idma_run_research(&victim, (void *)&r);
	check(ret != 0, "cost transfers=100 must be rejected");
	check(g_session_calls == 0, "cost transfers=100 must not reach the session");

	init_cost(&r, 0, 64, 8);
	r.gap_cycles = 1000;
	g_session_calls = 0;
	ret = idma_run_research(&victim, (void *)&r);
	check(ret != 0, "cost gap without a8 must be rejected");
	check(g_session_calls == 0, "cost gap without a8 must not reach the session");

	init_cost(&r, 8, 64, 8);
	r.gap_cycles = 1000;
	g_session_calls = 0;
	ret = idma_run_research(&victim, (void *)&r);
	check(ret == 0, "cost a8g1000 must succeed");
	check(g_aggressor_pages == 8, "cost a8g1000: page count must stay 8");

	/* E2 solo short parameters reach the E2 adapter path. */
	init_e2(&r, 0, 0);
	g_e2_calls = 0;
	ret = idma_run_research(&victim, (void *)&r);
	check(ret == 0, "e2 solo short must succeed");
	check(g_e2_calls == 1, "e2 solo short must use the E2 path");

	/* E2 illegal page counts stay refused. */
	init_e2(&r, 2, 112);
	g_e2_calls = 0;
	ret = idma_run_research(&victim, (void *)&r);
	check(ret != 0, "e2 a_pages=2 must be rejected");
	check(g_e2_calls == 0, "e2 a_pages=2 must not use the E2 path");

	if (g_failures) {
		printf("%d check(s) failed\n", g_failures);
		return 1;
	}
	printf("OK: solo adapter and validators honoured the contract\n");
	return 0;
}
'''


def extract_function(text, signature, source):
    """Return the exact text of the function starting at *signature*."""
    start = text.find(signature)
    if start < 0:
        raise AssertionError("signature %r not found in %s" % (signature, source))
    brace = text.find("{", start)
    if brace < 0:
        raise AssertionError("no opening brace after %r in %s" % (signature, source))
    depth = 0
    for offset in range(brace, len(text)):
        char = text[offset]
        if char == "{":
            depth += 1
        elif char == "}":
            depth -= 1
            if depth == 0:
                return text[start:offset + 1]
    raise AssertionError("unbalanced braces after %r in %s" % (signature, source))


class SoloDriverHarnessTests(unittest.TestCase):
    """Compile the real adapter/validators into a host harness.

    This exercises argument adaptation and validation only.  It is not a test of
    real DMA or of any FPGA behaviour and must never be reported as one.
    """

    def setUp(self):
        self.cc = os.environ.get("CC", "cc")
        if shutil.which(self.cc) is None:
            self.skipTest("no C compiler (%s) available" % self.cc)
        self.tmp = tempfile.TemporaryDirectory()
        self.addCleanup(self.tmp.cleanup)
        self.base = Path(self.tmp.name)

    def build_source(self, fix):
        if not RESEARCH_H_PATH.is_file() or not IDMA_C_PATH.is_file():
            self.skipTest("driver sources are not available")
        header = RESEARCH_H_PATH.read_text(encoding="utf-8")
        driver = IDMA_C_PATH.read_text(encoding="utf-8")
        validate = extract_function(driver, "static int idma_validate(", "idma.c")
        p4m = extract_function(driver, "static int idma_p4m_validate(", "idma.c")
        research_validate = extract_function(
            header, "static int idma_research_validate(", "idma_research.h")
        adapter = extract_function(header, "static long idma_run_research(", "idma_research.h")
        self.assertIn(FIXED_PAGE_LINE, adapter,
                      "the Solo page-count compatibility line is missing from the adapter")
        if not fix:
            adapter = adapter.replace(FIXED_PAGE_LINE, OLD_PAGE_LINE)
            self.assertNotIn(FIXED_PAGE_LINE, adapter)
        return (PREAMBLE + validate + "\n\n" + p4m + "\n\n" + research_validate +
                "\n\n" + adapter + "\n" + MAIN)

    def run_harness(self, fix):
        source = self.build_source(fix)
        stem = "harness_fix" if fix else "harness_old"
        c_path = self.base / (stem + ".c")
        exe = self.base / stem
        c_path.write_text(source, encoding="utf-8")
        compile_run = subprocess.run(
            [self.cc, "-std=gnu11", "-O0", "-w", "-o", str(exe), str(c_path)],
            capture_output=True, text=True)
        if compile_run.returncode != 0:
            self.fail("compilation failed:\n%s" % (compile_run.stdout + compile_run.stderr))
        return subprocess.run([str(exe)], capture_output=True, text=True)

    def test_real_adapter_passes_and_the_old_one_liner_fails(self):
        fixed = self.run_harness(fix=True)
        self.assertEqual(fixed.returncode, 0, fixed.stdout + fixed.stderr)
        self.assertIn("OK", fixed.stdout)
        old = self.run_harness(fix=False)
        self.assertNotEqual(
            old.returncode, 0,
            "the pre-fix one-liner must be detected (mutation test): " +
            old.stdout + old.stderr)


if __name__ == "__main__":
    unittest.main()

