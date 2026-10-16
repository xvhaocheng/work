#!/usr/bin/env python3
# SPDX-License-Identifier: GPL-2.0
'''Host harness for reverse batch collect and idma_verify prefix UNION tail coverage.

It extracts idma_batch_collect, idma_verify and idma_pattern from the real idma.c and
compiles them with fake MMIO.  No validation logic is rewritten in the test.
'''

from __future__ import annotations

import os
import re
import shutil
import subprocess
import tempfile
import unittest
from pathlib import Path

REPO_ROOT = Path(__file__).resolve().parents[1]
IDMA_C_PATH = REPO_ROOT / 'linux' / 'drivers' / 'misc' / 'idma.c'
UAPI_H_PATH = REPO_ROOT / 'linux' / 'include' / 'uapi' / 'linux' / 'idma.h'

PRIVATE_DEFINE_RE = re.compile(r'^#define[ \t]+(IDMA_[A-Z0-9_]+)[ \t]+(.+?)[ \t]*$', re.MULTILINE)


def extract_function(text, signature, source):
    start = text.find(signature)
    if start < 0:
        raise AssertionError('signature %r not found in %s' % (signature, source))
    brace = text.find('{', start)
    if brace < 0:
        raise AssertionError('no opening brace after %r in %s' % (signature, source))
    depth = 0
    for offset in range(brace, len(text)):
        char = text[offset]
        if char == '{':
            depth += 1
        elif char == '}':
            depth -= 1
            if depth == 0:
                return text[start:offset + 1]
    raise AssertionError('unbalanced braces after %r in %s' % (signature, source))


def extract_private_defines(text):
    return ['#define %s %s' % (m.group(1), m.group(2))
            for m in PRIVATE_DEFINE_RE.finditer(text)]


def dedupe_defines(defines):
    seen = {}
    for line in defines:
        name = line.split()[1]
        seen[name] = line
    return list(seen.values())


PREAMBLE = r'''#include <errno.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <linux/idma.h>
typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;
typedef int32_t s32;
typedef int64_t s64;
#define __user
#define __iomem
#define BIT_ULL(n) (1ULL << (n))
#define PAGE_SIZE 4096UL
#define min(a, b) ((a) < (b) ? (a) : (b))
#define max(a, b) ((a) > (b) ? (a) : (b))
typedef uint64_t dma_addr_t;
typedef uint64_t phys_addr_t;
struct device;
struct iommu_domain;
struct idma_device { int id; __u32 did; void *reg; };
struct idma_stream {
    struct idma_device *idma;
    struct device *dev;
    struct iommu_domain *domain;
    void *src, *dst;
    dma_addr_t src_dma, dst_dma;
    unsigned long src_iova, dst_iova;
    size_t size, offset, next_offset, touched_end, touched_tail;
    u32 chunk;
    u64 transfer_id, deadline_ns, last_complete_ns;
    bool mapped_src, mapped_dst, in_flight, retain;
    bool batch_configured, batch_started, reverse;
    struct idma_experiment_stats *stats;
    void *diagnostic;
};
static u64 fake_regs[0x2000];
static inline void fake_writeq(u64 val, void *addr) { fake_regs[(unsigned long)addr / 8] = val; }
static inline u64 fake_readq(void *addr) { return fake_regs[(unsigned long)addr / 8]; }
#define writeq fake_writeq
#define readq fake_readq
#define READ_ONCE(x) (x)
static inline void local_flush_icache_all(void) { }
static inline unsigned long idma_buffer_page(void *ptr) { (void)ptr; return 0; }
static inline phys_addr_t page_to_phys(unsigned long page) { (void)page; return 0; }
static inline unsigned long offset_in_page(const void *p) { (void)p; return 0; }
static inline phys_addr_t iommu_iova_to_phys(struct iommu_domain *d, unsigned long iova) { (void)d; (void)iova; return 0; }
#define dev_err(dev, fmt, ...) do { } while (0)
static void idma_quarantine(struct idma_stream *s) { s->retain = true; }
'''

HARNESS_MAIN = r'''
static int failures;
static void check(int condition, const char *message)
{
    if (!condition) {
        printf("FAIL: %s\n", message);
        failures++;
    }
}

int main(void)
{
    struct idma_device dev = { .id = 3, .did = 13, .reg = (void *)0 };
    struct idma_experiment_stats stats;
    struct idma_stream s;
    u8 src[2 * PAGE_SIZE];
    u8 dst[2 * PAGE_SIZE];
    size_t i;

    memset(&stats, 0, sizeof(stats));
    memset(&s, 0, sizeof(s));
    s.idma = &dev;
    s.src = src;
    s.dst = dst;
    s.size = sizeof(src);
    s.chunk = PAGE_SIZE;
    s.stats = &stats;

    s.touched_end = 0;
    s.touched_tail = PAGE_SIZE;
    memset(src, 0, sizeof(src));
    memset(dst, 0, sizeof(dst));
    for (i = 0; i < sizeof(src); i++) {
        u64 word = idma_pattern(&s, i / sizeof(u64));
        u8 value = ((u8 *)&word)[i % sizeof(u64)];
        src[i] = value;
        dst[i] = (i < s.touched_end || i >= s.size - s.touched_tail) ? value : (u8)~value;
    }
    check(idma_verify(&s) == 0, "reverse tail union must pass");
    dst[PAGE_SIZE + 17] = (u8)~dst[PAGE_SIZE + 17];
    check(idma_verify(&s) == -EILSEQ, "corrupt reverse tail must fail");
    dst[PAGE_SIZE + 17] = (u8)~dst[PAGE_SIZE + 17];
    src[PAGE_SIZE + 17] = (u8)~src[PAGE_SIZE + 17];
    check(idma_verify(&s) == -EILSEQ, "corrupt reverse source must fail");
    src[PAGE_SIZE + 17] = (u8)~src[PAGE_SIZE + 17];

    s.touched_end = PAGE_SIZE;
    s.touched_tail = PAGE_SIZE;
    memset(dst, 0, sizeof(dst));
    for (i = 0; i < sizeof(src); i++) {
        u64 word = idma_pattern(&s, i / sizeof(u64));
        u8 value = ((u8 *)&word)[i % sizeof(u64)];
        src[i] = value;
        dst[i] = value;
    }
    check(idma_verify(&s) == 0, "overlap full union must pass");

    memset(&s, 0, sizeof(s));
    s.idma = &dev;
    s.stats = &stats;
    s.size = 4 * PAGE_SIZE;
    s.batch_configured = true;
    s.batch_started = true;
    s.reverse = true;
    fake_regs[IDMA_BATCH_CONTROL_OFFSET / 8] = 0;
    fake_regs[IDMA_BATCH_ISSUED_OFFSET / 8] = 2;
    fake_regs[IDMA_BATCH_COMPLETED_OFFSET / 8] = 2;
    check(idma_batch_collect(&s) == 0, "reverse batch collect succeeds");
    check(s.touched_tail == 2 * PAGE_SIZE, "reverse collect updates tail");
    check(s.touched_end == 0, "reverse collect must not fake forward prefix");

    s.reverse = false;
    s.touched_end = 0;
    s.touched_tail = 0;
    check(idma_batch_collect(&s) == 0, "forward batch collect succeeds");
    check(s.touched_end == 2 * PAGE_SIZE, "forward collect updates prefix");
    check(s.touched_tail == 0, "forward collect must not update tail");

    if (failures) {
        printf("%d check(s) failed\n", failures);
        return 1;
    }
    printf("OK: reverse prefix UNION tail and batch collect coverage honoured the contract\n");
    return 0;
}
'''


class Research2ReverseDriverTests(unittest.TestCase):
    def setUp(self):
        self.cc = os.environ.get('CC', 'cc')
        if shutil.which(self.cc) is None:
            self.skipTest('no C compiler available')
        if not UAPI_H_PATH.is_file():
            self.skipTest('UAPI header missing')
        self.tmp = tempfile.TemporaryDirectory()
        self.addCleanup(self.tmp.cleanup)
        self.base = Path(self.tmp.name)
        self.include_dir = self.base / 'include'
        (self.include_dir / 'linux').mkdir(parents=True)
        shutil.copyfile(UAPI_H_PATH, self.include_dir / 'linux' / 'idma.h')

    def build_source(self):
        if not IDMA_C_PATH.is_file():
            self.skipTest('driver source missing')
        driver = IDMA_C_PATH.read_text(encoding='utf-8')
        private = dedupe_defines(extract_private_defines(driver))
        pieces = [PREAMBLE, '\n'.join(private)]
        for signature in [
            'static u64 idma_pattern(',
            'static int idma_batch_collect(',
            'static int idma_verify(',
        ]:
            pieces.append(extract_function(driver, signature, 'idma.c'))
        pieces.append(HARNESS_MAIN)
        return '\n'.join(pieces) + '\n'

    def run_harness(self):
        src = self.base / 'research2_reverse.c'
        exe = self.base / 'research2_reverse'
        src.write_text(self.build_source(), encoding='utf-8')
        build = subprocess.run([self.cc, '-std=gnu11', '-O0', '-I', str(self.include_dir), '-o', str(exe), str(src)], capture_output=True, text=True)
        if build.returncode != 0:
            self.fail('compilation failed:\n%s' % (build.stdout + build.stderr))
        return subprocess.run([str(exe)], capture_output=True, text=True)

    def test_real_reverse_collect_and_verify_union(self):
        result = self.run_harness()
        self.assertEqual(result.returncode, 0, result.stdout + result.stderr)
        self.assertIn('OK', result.stdout)


if __name__ == '__main__':
    unittest.main()
