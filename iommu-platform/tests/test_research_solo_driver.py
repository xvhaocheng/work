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

Nothing is hand-copied from the UAPI.  The real
``linux/include/uapi/linux/idma.h`` is copied verbatim into a temporary
``include/linux/idma.h`` and reached with ``-I``, so every struct, field layout
and constant comes from the checked-in header; the host ``<linux/types.h>``,
``<linux/ioctl.h>`` and ``<errno.h>`` supply the rest.  The private ``IDMA_*``
object-like macros of ``idma.c`` (the page / transfer / timeout ceilings among
them) are extracted from that file as well, so no private limit is invented
here.  Error values come from the host ``<errno.h>``; ``EINPROGRESS`` is the
positive errno and is only ever negated by the driver.

What this test is *not*: it never touches an FPGA, never starts a DMA engine and
never observes real hardware, so it makes no claim about the board.  The stub
for ``idma_run_session`` only runs the real ``idma_validate`` and records the
adapted experiment fields, which is exactly the contract the Solo condition
depends on (``a_pages == 0`` must still be a positive legacy page count while
``aggressor_id == NO_AGGRESSOR`` stays, so no A lane is ever allocated).  The
adapter source is taken verbatim; the only edit is a mutation that removes the
Solo page-count line to prove that these checks detect its absence.  No fix
logic is reimplemented here.  Codex must still review the real
``idma_run_session`` directly to confirm that no A allocation happens.
"""

from __future__ import annotations

import os
import re
import shutil
import subprocess
import tempfile
import unittest
from pathlib import Path

REPO_ROOT = Path(__file__).resolve().parents[1]
RESEARCH_H_PATH = REPO_ROOT / "linux" / "drivers" / "misc" / "idma_research.h"
IDMA_C_PATH = REPO_ROOT / "linux" / "drivers" / "misc" / "idma.c"
UAPI_H_PATH = REPO_ROOT / "linux" / "include" / "uapi" / "linux" / "idma.h"

# The Solo page-count compatibility line the adapter is expected to carry and the
# pre-fix line the mutation restores, to prove the checks below are meaningful.
FIXED_PAGE_LINE = "e->aggressor_pages = r->a_pages ? r->a_pages : 1;"
OLD_PAGE_LINE = "e->aggressor_pages = r->a_pages;"

PRIVATE_DEFINE_RE = re.compile(
    r"^#define[ \t]+(IDMA_[A-Z0-9_]+)[ \t]+(.+?)[ \t]*$", re.MULTILINE)


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


def extract_private_defines(text):
    """Every object-like ``#define`` of *text* whose name starts with IDMA_."""
    return ["#define %s %s" % (match.group(1), match.group(2))
            for match in PRIVATE_DEFINE_RE.finditer(text)]


PREAMBLE_HEAD = r'''#include <errno.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Exact copy of linux/include/uapi/linux/idma.h, reached through -I. */
#include <linux/idma.h>

/* Host translations of kernel annotations and platform constants. */
#define __user
#define BIT_ULL(n) (1ULL << (n))
#define PAGE_SIZE 4096UL
#define SZ_4K 4096UL
'''

PREAMBLE_STUBS = r'''
/* Small host stubs; none of them reimplements driver logic. */
struct idma_device {
    int id;
    __u32 did;
};

struct idma_e3_pair;
struct idma_e3_diag_round;

static int idma_validate(const struct idma_experiment *e);
static long idma_run_session(struct idma_device *victim,
                             struct idma_experiment *request, bool batch,
                             struct idma_e3_pair *pair,
                             struct idma_e3_diag_round *diagnostic,
                             bool queue_pair, struct idma_p4m *p4m,
                             struct idma_research *research);

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


def build_preamble(private_defines):
    return "\n".join([
        PREAMBLE_HEAD,
        "/* Private object-like macros extracted verbatim from idma.c. */",
        "\n".join(private_defines),
        PREAMBLE_STUBS,
    ]) + "\n"


HARNESS_MAIN = r'''
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

    /* The driver only negates EINPROGRESS; the host errno must be positive. */
    check(EINPROGRESS > 0, "errno.h must supply the positive EINPROGRESS");

    memset(&victim, 0, sizeof(victim));
    victim.id = 0;
    victim.did = 10;

    /* Solo, short parameters: positive legacy count, NO_AGGRESSOR preserved. */
    init_cost(&r, 0, 64, 8);
    g_session_calls = 0;
    ret = idma_run_research(&victim, (void *)&r);
    check(ret == 0, "solo short: idma_run_research must succeed");
    check(g_session_calls == 1, "solo short: session must be reached once");
    check(g_aggressor_id == IDMA_EXPERIMENT_NO_AGGRESSOR,
          "solo short: NO_AGGRESSOR must be preserved");
    check(g_aggressor_pages == 1, "solo short: legacy page count must be 1");
    check(g_victim_pages == 1, "solo short: victim pages must be 1");
    check(g_victim_transfers == 64, "solo short: transfers preserved");
    check(g_warmup_transfers == 8, "solo short: warmup preserved");
    check(g_chunk_bytes == PAGE_SIZE, "solo short: chunk bytes preserved");
    check(g_run_timeout_ms == 10000, "solo short: timeout preserved");
    check(r.stage == 5, "solo short: stage must be the completed stage");
    check(r.flush_status == -EINPROGRESS,
          "solo short: flush_status keeps the negated errno");

    /* Solo, main parameters. */
    init_cost(&r, 0, 4096, 256);
    g_session_calls = 0;
    ret = idma_run_research(&victim, (void *)&r);
    check(ret == 0, "solo main: idma_run_research must succeed");
    check(g_session_calls == 1, "solo main: session must be reached once");
    check(g_aggressor_id == IDMA_EXPERIMENT_NO_AGGRESSOR,
          "solo main: NO_AGGRESSOR must be preserved");
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

    /* E2 solo reaches the E2 adapter path. */
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


class SoloDriverHarnessTests(unittest.TestCase):
    """Compile the real adapter/validators into a host harness.

    This exercises argument adaptation and validation only.  It is not a test of
    real DMA or of any FPGA behaviour and must never be reported as one.
    """

    def setUp(self):
        self.cc = os.environ.get("CC", "cc")
        if shutil.which(self.cc) is None:
            self.skipTest("no C compiler (%s) available" % self.cc)
        if not UAPI_H_PATH.is_file():
            self.skipTest("linux/include/uapi/linux/idma.h is not available")
        self.tmp = tempfile.TemporaryDirectory()
        self.addCleanup(self.tmp.cleanup)
        self.base = Path(self.tmp.name)
        self.include_dir = self.base / "include"
        (self.include_dir / "linux").mkdir(parents=True)
        # Verbatim copy of the checked-in UAPI header; nothing is transcribed.
        shutil.copyfile(UAPI_H_PATH, self.include_dir / "linux" / "idma.h")
        if not self.uapi_headers_usable():
            self.skipTest("host <linux/types.h>/<linux/ioctl.h> cannot be used "
                          "with the UAPI header")

    def uapi_headers_usable(self):
        probe = self.base / "probe.c"
        probe.write_text("#include <linux/idma.h>\nint main(void) { return 0; }\n",
                         encoding="utf-8")
        build = subprocess.run(
            [self.cc, "-std=gnu11", "-I", str(self.include_dir),
             "-o", str(self.base / "probe"), str(probe)],
            capture_output=True, text=True)
        return build.returncode == 0

    def build_source(self, fix):
        if not RESEARCH_H_PATH.is_file() or not IDMA_C_PATH.is_file():
            self.skipTest("driver sources are not available")
        header = RESEARCH_H_PATH.read_text(encoding="utf-8")
        driver = IDMA_C_PATH.read_text(encoding="utf-8")
        private_defines = extract_private_defines(driver)
        for needed in ("IDMA_MAX_PAGES", "IDMA_MAX_RUN_TIMEOUT_MS"):
            self.assertTrue(
                any(line.startswith("#define %s " % needed) for line in private_defines),
                "%s must be taken from idma.c, not invented here" % needed)
        validate = extract_function(driver, "static int idma_validate(", "idma.c")
        p4m = extract_function(driver, "static int idma_p4m_validate(", "idma.c")
        research_validate = extract_function(
            header, "static int idma_research_validate(", "idma_research.h")
        adapter = extract_function(header, "static long idma_run_research(",
                                   "idma_research.h")
        self.assertIn(FIXED_PAGE_LINE, adapter,
                      "the Solo page-count compatibility line is missing from the adapter")
        if not fix:
            adapter = adapter.replace(FIXED_PAGE_LINE, OLD_PAGE_LINE)
            self.assertNotIn(FIXED_PAGE_LINE, adapter)
        return (build_preamble(private_defines) + validate + "\n\n" + p4m + "\n\n" +
                research_validate + "\n\n" + adapter + "\n" + HARNESS_MAIN)

    def run_harness(self, fix):
        source = self.build_source(fix)
        stem = "harness_fix" if fix else "harness_old"
        c_path = self.base / (stem + ".c")
        exe = self.base / stem
        c_path.write_text(source, encoding="utf-8")
        build = subprocess.run(
            [self.cc, "-std=gnu11", "-O0", "-w", "-I", str(self.include_dir),
             "-o", str(exe), str(c_path)],
            capture_output=True, text=True)
        if build.returncode != 0:
            self.fail("compilation failed:\n%s" % (build.stdout + build.stderr))
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

