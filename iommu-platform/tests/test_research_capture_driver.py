#!/usr/bin/env python3
# SPDX-License-Identifier: GPL-2.0
"""Host harness for the *real* ``idma_research_capture`` observer-bank reader.

This test extracts the real kernel source of

  * ``idma_research_capture`` from ``linux/drivers/misc/idma_research.h`` and

and compiles it, unchanged, into a host program with small stubs.  It then
drives the capture helper the way the E2 / COST drain path does and checks the
document by ``docs/research-fullstats-v1.md`` (0xC00 / 0xD00 raw words).

Nothing is hand-copied from the UAPI.  The real
``linux/include/uapi/linux/idma.h`` is copied verbatim into a temporary
``include/linux/idma.h`` and reached with ``-I``, so every struct, field layout
and constant (``IDMA_RESEARCH_RAW_WORDS`` == 4096, ``IDMA_RESEARCH_E2``,
``IDMA_RESEARCH_COST``) comes from the checked-in header.  The private
object-like macros of ``idma.c`` and ``idma_research.h`` (``IDMA_RESEARCH_MAGIC``,
``IDMA_OBS_MAGIC_V8``, ``RESEARCH_COMMAND``, ``RESEARCH_ABORT``, currently used
raw indices) are extracted from those files as well, so no private constant is
invented here.  Error values come from the host ``<errno.h>``.

The extracted function is pasted verbatim; the only host-side replacements are
the hardware observation primitives (``idma_research_read`` /
``idma_research_write``), the allocation primitive (``kcalloc`` / ``kfree``) and
the userspace copy primitive (``copy_to_user`` / ``u64_to_user_ptr``).  No
validation logic is reimplemented: the checks below either observe the real
function's return value or observe the real raw bytes it hands to
``copy_to_user``.

What this test is *not*: it never touches an FPGA, never starts a DMA engine and
never observes real hardware, so it makes no claim about the board.  It also
cannot validate the RTL that produces the observation bank; it only checks that
the software reader honours the documented contract (accept old zero-magic
banks, reject a corrupt new bank, always capture all raw words).  Codex must
still review the real drain path (``idma_research_e2``) and the RTL directly.
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
MISC_DIR = REPO_ROOT / "linux" / "drivers" / "misc"

# The one function under test.  Only this text is pasted into the harness.
CAPTURE_SIGNATURE = "static int idma_research_capture("

# Function-like macros never match (no whitespace after the name); continued
# lines are skipped below so the generated C stays well formed.
DEFINE_RE = re.compile(
    r"^#define[ \t]+([A-Za-z_][A-Za-z0-9_]*)[ \t]+(.+?)[ \t]*$", re.MULTILINE)


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


def collect_defines(paths, prefixes):
    """Object-like ``#define`` macros of *paths* whose name starts with a prefix.

    Returns a name -> replacement-text mapping.  A name defined differently in
    two files is a hard error: the harness must not silently pick one.
    """
    found = {}
    for path in paths:
        text = path.read_text(encoding="utf-8")
        for match in DEFINE_RE.finditer(text):
            name = match.group(1)
            value = match.group(2)
            if value.endswith("\\"):
                # Multi-line macro; not usable as a single generated line.
                continue
            if not any(name.startswith(prefix) for prefix in prefixes):
                continue
            previous = found.get(name)
            if previous is not None and previous != value:
                raise AssertionError(
                    "%s is defined differently in %s: %r vs %r"
                    % (name, path, previous, value))
            found[name] = value
    return found


PREAMBLE = r'''#include <errno.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Verbatim UAPI header, reached through -I; nothing is transcribed. */
#include <linux/idma.h>

/* Host translations of kernel annotations and platform constants. */
typedef uint64_t u64;
typedef uint32_t u32;

#define __user
#define BIT(n) (1UL << (n))
#define BIT_ULL(n) (1ULL << (n))
#define PAGE_SIZE 4096UL
#define SZ_4K 4096UL
'''

DEFINES_HEADER = (
    "/* Private object-like macros copied verbatim from idma.c / "
    "idma_research.h. */")

STUBS = r'''
/* Small host stubs; none of them reimplements capture validation. */
struct idma_device {
    int unused;
};

static u64 g_hw[IDMA_RESEARCH_RAW_WORDS];
static u64 g_copy_dst[IDMA_RESEARCH_RAW_WORDS];
static int g_read_calls;
static int g_write_calls;
static u32 g_last_write_index;
static u64 g_last_write_value;
static int g_kcalloc_fail;
static int g_copy_fail;
static int g_copy_calls;
static int g_failures;

/* Hardware observation stub: the fake bank is fully controlled by the test. */
static u64 idma_research_read(struct idma_device *d, u32 index)
{
    (void)d;
    g_read_calls++;
    return g_hw[index];
}

/* Hardware observation stub: records what the capture helper writes. */
static void idma_research_write(struct idma_device *d, u32 index, u64 value)
{
    (void)d;
    g_write_calls++;
    g_last_write_index = index;
    g_last_write_value = value;
}

static void *x_kcalloc(size_t count, size_t size, int flags)
{
    (void)flags;
    if (g_kcalloc_fail)
        return NULL;
    return calloc(count, size);
}
#define kcalloc(count, size, flags) x_kcalloc((count), (size), (flags))
#define kfree(ptr) free(ptr)
#define GFP_KERNEL 0

/* Userspace copy stub: can be forced to fail, and records the attempt. */
static int x_copy_to_user(void *dst, const void *src, size_t size)
{
    g_copy_calls++;
    if (g_copy_fail)
        return 1;
    memcpy(dst, src, size);
    return 0;
}
#define copy_to_user(dst, src, size) \
    x_copy_to_user((void *)(dst), (const void *)(src), (size))
#define u64_to_user_ptr(value) ((void *)(uintptr_t)(value))
'''

HARNESS_MAIN = r'''
static void check(int condition, const char *message)
{
    if (!condition) {
        printf("FAIL: %s\n", message);
        g_failures++;
    }
}

static void reset(void)
{
    memset(g_hw, 0, sizeof(g_hw));
    memset(g_copy_dst, 0xAA, sizeof(g_copy_dst));
    g_read_calls = 0;
    g_write_calls = 0;
    g_last_write_index = 0;
    g_last_write_value = 0;
    g_kcalloc_fail = 0;
    g_copy_fail = 0;
    g_copy_calls = 0;
}

/* 1 only when the byte stream handed to userspace equals the raw bank. */
static int raw_copied(void)
{
    u32 i;

    for (i = 0; i < IDMA_RESEARCH_RAW_WORDS; i++)
        if (g_copy_dst[i] != g_hw[i])
            return 0;
    return 1;
}

static void make_request(struct idma_research *r, __u32 mode)
{
    memset(r, 0, sizeof(*r));
    r->abi_version = IDMA_RESEARCH_ABI;
    r->struct_size = sizeof(*r);
    r->mode = mode;
    r->stage = 1;
    r->transfers = 64;
    r->raw_words = IDMA_RESEARCH_RAW_WORDS;
    r->raw_user = (__u64)(uintptr_t)g_copy_dst;
}

/* Observer words common to every valid bank. */
static void good_common(void)
{
    g_hw[0] = IDMA_RESEARCH_MAGIC;
    g_hw[1] = IDMA_OBS_MAGIC_V8;
    g_hw[3] = 0;
}

/* Valid COST bank: shared hardware window plus the new 0xD00 aggregation. */
static void good_cost(struct idma_research *r)
{
    r->p4m.window[1] = 0x2222222222222222ULL;
    g_hw[0xc00] = 0x49444d4152454331ULL;
    g_hw[0xc01] = 6;
    g_hw[0xc02] = 0;
    g_hw[0xc11] = r->transfers;
    g_hw[0xc12] = r->transfers;
    g_hw[0xc14] = 0x0000000000005555ULL;
    g_hw[0xc19] = r->p4m.window[1];
    g_hw[0xd00] = 0x49444d4141474731ULL;
    g_hw[0xd01] = 1;
    g_hw[0xd02] = 6;
    g_hw[0xd03] = 16;
    g_hw[0xd04] = 0;
    g_hw[0xd05] = g_hw[0xc14];
}

/* Each COST corruption must be refused while the raw bank is still copied. */
#define EXPECT_COST_EIO(desc, mutation)                                    \
    do {                                                                   \
        reset();                                                           \
        make_request(&r, IDMA_RESEARCH_COST);                              \
        good_common();                                                     \
        good_cost(&r);                                                     \
        mutation;                                                          \
        ret = idma_research_capture(&d, &r, 0);                            \
        check(ret == -EIO, desc "; COST corruption must return -EIO");     \
        check(r.raw_captured == IDMA_RESEARCH_RAW_WORDS,                   \
              desc "; corrupt COST must still report all raw words");      \
        check(raw_copied(),                                                \
              desc "; corrupt COST must still retain the raw bank");       \
    } while (0)

int main(void)
{
    struct idma_device d;
    struct idma_research r;
    int ret;

    memset(&d, 0, sizeof(d));

    check(IDMA_RESEARCH_RAW_WORDS == 4096U,
          "the UAPI raw word count must be 4096");

    /* 1. No stage: propagate the caller error, touch nothing else. */
    reset();
    make_request(&r, IDMA_RESEARCH_COST);
    r.stage = 0;
    ret = idma_research_capture(&d, &r, 0);
    check(ret == -EIO, "missing stage with error 0 must return -EIO");
    check(g_read_calls == 0 && g_write_calls == 0 && g_copy_calls == 0,
          "missing stage must not read, write or copy");
    check(r.raw_captured == 0, "missing stage must not set raw_captured");

    reset();
    make_request(&r, IDMA_RESEARCH_COST);
    r.stage = 0;
    ret = idma_research_capture(&d, &r, -EINTR);
    check(ret == -EINTR, "missing stage must propagate the caller error verbatim");
    check(g_write_calls == 0, "missing stage must not issue an abort write");
    check(r.raw_captured == 0, "missing stage (error case) must not set raw_captured");

    /* 2. Allocation failure: -ENOMEM, no read, no copy, no raw_captured. */
    reset();
    make_request(&r, IDMA_RESEARCH_COST);
    good_common();
    good_cost(&r);
    g_kcalloc_fail = 1;
    ret = idma_research_capture(&d, &r, 0);
    check(ret == -ENOMEM, "kcalloc failure must return -ENOMEM");
    check(g_read_calls == 0, "kcalloc failure must not read the observer bank");
    check(g_copy_calls == 0, "kcalloc failure must not copy to user");
    check(r.raw_captured == 0, "kcalloc failure must not set raw_captured");

    /* 3. E2: normal bank, raw[6] == 4, no COST aggregation header needed. */
    reset();
    make_request(&r, IDMA_RESEARCH_E2);
    good_common();
    g_hw[6] = 4;
    ret = idma_research_capture(&d, &r, 0);
    check(ret == 0, "valid E2 bank must be accepted");
    check(r.raw_captured == IDMA_RESEARCH_RAW_WORDS, "E2 must report all raw words");
    check(g_read_calls == IDMA_RESEARCH_RAW_WORDS,
          "capture must read every raw word");
    check(raw_copied(), "E2 must copy the whole raw bank");

    reset();
    make_request(&r, IDMA_RESEARCH_E2);
    good_common();
    g_hw[6] = 4;
    g_hw[0xc00] = 0xdeadbeefULL;
    g_hw[0xc01] = 99;
    g_hw[0xc02] = 7;
    g_hw[0xd00] = 0x11223344ULL;
    g_hw[0xd01] = 5;
    g_hw[0xd02] = 5;
    g_hw[0xd03] = 5;
    g_hw[0xd04] = 0xff;
    g_hw[0xd05] = 0x1234;
    ret = idma_research_capture(&d, &r, 0);
    check(ret == 0, "E2 must not require a COST aggregation header");
    check(r.raw_captured == IDMA_RESEARCH_RAW_WORDS,
          "E2 without a COST header must still report all raw words");

    reset();
    make_request(&r, IDMA_RESEARCH_E2);
    good_common();
    g_hw[6] = 3;
    ret = idma_research_capture(&d, &r, 0);
    check(ret == -EIO, "E2 with raw[6] != 4 must return -EIO");
    check(r.raw_captured == IDMA_RESEARCH_RAW_WORDS,
          "E2 corrupt phase must still report all raw words");
    check(raw_copied(), "E2 corrupt phase must still retain the raw bank");

    reset();
    make_request(&r, IDMA_RESEARCH_E2);
    good_common();
    g_hw[6] = 4;
    g_hw[1] = 0;
    ret = idma_research_capture(&d, &r, 0);
    check(ret == -EIO, "E2 with a wrong observer magic must return -EIO");
    check(r.raw_captured == IDMA_RESEARCH_RAW_WORDS,
          "E2 wrong magic must still retain the raw bank");

    /* 4. COST with the new 0xD00 aggregation bank is accepted. */
    reset();
    make_request(&r, IDMA_RESEARCH_COST);
    good_common();
    good_cost(&r);
    ret = idma_research_capture(&d, &r, 0);
    check(ret == 0, "valid COST bank (new aggregation words) must be accepted");
    check(r.raw_captured == IDMA_RESEARCH_RAW_WORDS, "COST must report all raw words");
    check(g_read_calls == IDMA_RESEARCH_RAW_WORDS, "COST must read every raw word");
    check(g_write_calls == 0, "a clean COST capture must not issue an abort write");
    check(raw_copied(), "COST must copy the whole raw bank");

    /* COST does not apply the E2 raw[6] phase rule. */
    reset();
    make_request(&r, IDMA_RESEARCH_COST);
    good_common();
    good_cost(&r);
    g_hw[6] = 12345;
    ret = idma_research_capture(&d, &r, 0);
    check(ret == 0, "COST must not apply the E2 raw[6] phase rule");

    /* 5. Old BIT compatibility: a zero aggregation magic keeps 0xD01.. usable. */
    reset();
    make_request(&r, IDMA_RESEARCH_COST);
    good_common();
    good_cost(&r);
    g_hw[0xd00] = 0;
    g_hw[0xd01] = 0xdeadbeefULL;
    g_hw[0xd02] = 0x99ULL;
    g_hw[0xd03] = 1ULL;
    g_hw[0xd04] = 0xffULL;
    g_hw[0xd05] = 0x1234ULL;
    ret = idma_research_capture(&d, &r, 0);
    check(ret == 0, "COST with a zero aggregation magic must keep the old BITs usable");
    check(r.raw_captured == IDMA_RESEARCH_RAW_WORDS,
          "old COST bank must still report all raw words");
    check(raw_copied(), "old COST bank must still copy the raw bank");

    /* 6. Every corrupt field is refused, yet the raw bank is still retained. */
    EXPECT_COST_EIO("COST bank magic", g_hw[0xc00] = 0);
    EXPECT_COST_EIO("COST bank version", g_hw[0xc01] = 5);
    EXPECT_COST_EIO("COST bank error word", g_hw[0xc02] = 1);
    EXPECT_COST_EIO("COST bank window[1]", g_hw[0xc19] = 0x3333333333333333ULL);
    EXPECT_COST_EIO("COST bank transfer count", g_hw[0xc11] = 63);
    EXPECT_COST_EIO("COST bank second transfer count", g_hw[0xc12] = 65);
    EXPECT_COST_EIO("observer magic", g_hw[0] = 0);
    EXPECT_COST_EIO("observer version", g_hw[1] = 0);
    EXPECT_COST_EIO("observer error word", g_hw[3] = 1);
    EXPECT_COST_EIO("aggregation magic", g_hw[0xd00] = 0x49444d4141474732ULL);
    EXPECT_COST_EIO("aggregation version", g_hw[0xd01] = 2);
    EXPECT_COST_EIO("aggregation group count", g_hw[0xd02] = 5);
    EXPECT_COST_EIO("aggregation stride", g_hw[0xd03] = 8);
    EXPECT_COST_EIO("aggregation error word", g_hw[0xd04] = 1);
    EXPECT_COST_EIO("aggregation total", g_hw[0xd05] = 0x9999ULL);

    /* 7. copy_to_user failure: -EFAULT and no raw_captured claim. */
    reset();
    make_request(&r, IDMA_RESEARCH_COST);
    good_common();
    good_cost(&r);
    g_copy_fail = 1;
    ret = idma_research_capture(&d, &r, 0);
    check(ret == -EFAULT, "copy_to_user failure must return -EFAULT");
    check(r.raw_captured == 0, "copy_to_user failure must not claim captured words");
    check(g_copy_calls == 1, "copy_to_user must be attempted exactly once");
    check(g_copy_dst[0] == 0xAAAAAAAAAAAAAAAAULL,
          "copy_to_user failure must leave the user buffer untouched");

    /* 8. Caller error aborts the observation but the raw bank is still read. */
    reset();
    make_request(&r, IDMA_RESEARCH_COST);
    good_common();
    good_cost(&r);
    ret = idma_research_capture(&d, &r, -EIO);
    check(ret == 0, "a caller error with a valid bank must not invent a new error");
    check(g_write_calls == 1, "a caller error must issue exactly one abort write");
    check(g_last_write_index == RESEARCH_COMMAND &&
          g_last_write_value == RESEARCH_ABORT,
          "the caller error must abort the observation");
    check(r.raw_captured == IDMA_RESEARCH_RAW_WORDS,
          "an aborted drain must still capture all raw words");
    check(raw_copied(), "an aborted drain must still retain the raw bank");

    reset();
    make_request(&r, IDMA_RESEARCH_COST);
    good_common();
    good_cost(&r);
    g_hw[0xc00] = 0;
    ret = idma_research_capture(&d, &r, -EINTR);
    check(ret == -EIO, "an aborted corrupt drain must report the capture error");
    check(g_write_calls == 1, "an aborted corrupt drain must still abort once");
    check(raw_copied(), "an aborted corrupt drain must still retain the raw bank");

    if (g_failures) {
        printf("%d check(s) failed\n", g_failures);
        return 1;
    }
    printf("OK: idma_research_capture honoured the raw-bank contract\n");
    return 0;
}
'''


def assemble_source(defines, capture):
    """Preamble + extracted macros + stubs + verbatim capture + harness main."""
    defines_block = "\n".join(
        [DEFINES_HEADER] + ["#define %s %s" % (name, value)
                            for name, value in sorted(defines.items())])
    return "\n".join([PREAMBLE, defines_block, STUBS, capture, HARNESS_MAIN]) + "\n"


class ResearchCaptureDriverTests(unittest.TestCase):
    """Compile the real capture helper into a host harness.

    This exercises the observer-bank reader only.  It is not a test of real DMA
    or of any FPGA behaviour and must never be reported as one.
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

    def collect_available_defines(self):
        """Fallback scan when ``IDMA_OBS_MAGIC_V8`` is not in idma.c."""
        paths = [IDMA_C_PATH, RESEARCH_H_PATH]
        paths += [p for p in sorted(MISC_DIR.glob("*.h")) if p != RESEARCH_H_PATH]
        return collect_defines(paths, ("IDMA_", "RESEARCH_"))

    def build_source(self):
        if not RESEARCH_H_PATH.is_file() or not IDMA_C_PATH.is_file():
            self.skipTest("driver sources are not available")
        header = RESEARCH_H_PATH.read_text(encoding="utf-8")
        capture = extract_function(header, CAPTURE_SIGNATURE, "idma_research.h")
        for needle in ("return -ENOMEM",
                       "raw[0xc00]",
                       "raw[0xd00] &&",
                       "raw_captured = IDMA_RESEARCH_RAW_WORDS",
                       "RESEARCH_ABORT",
                       "copy_to_user"):
            self.assertIn(needle, capture,
                          "the extracted idma_research_capture is missing %r" % needle)
        defines = collect_defines([IDMA_C_PATH, RESEARCH_H_PATH],
                                  ("IDMA_", "RESEARCH_"))
        if "IDMA_OBS_MAGIC_V8" not in defines:
            defines = self.collect_available_defines()
        for needed in ("IDMA_OBS_MAGIC_V8", "IDMA_RESEARCH_MAGIC",
                       "RESEARCH_COMMAND", "RESEARCH_ABORT"):
            self.assertIn(needed, defines,
                          "%s must be taken from the real driver sources, "
                          "not invented here" % needed)
        return assemble_source(defines, capture)

    def run_harness(self):
        source = self.build_source()
        c_path = self.base / "capture_harness.c"
        exe = self.base / "capture_harness"
        c_path.write_text(source, encoding="utf-8")
        build = subprocess.run(
            [self.cc, "-std=gnu11", "-O0", "-w", "-I", str(self.include_dir),
             "-o", str(exe), str(c_path)],
            capture_output=True, text=True)
        if build.returncode != 0:
            self.fail("compilation failed:\n%s" % (build.stdout + build.stderr))
        return subprocess.run([str(exe)], capture_output=True, text=True)

    def test_real_capture_contract(self):
        result = self.run_harness()
        self.assertEqual(result.returncode, 0, result.stdout + result.stderr)
        self.assertIn("OK", result.stdout)


if __name__ == "__main__":
    unittest.main()
