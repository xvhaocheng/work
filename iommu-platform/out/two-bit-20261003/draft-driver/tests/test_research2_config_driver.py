#!/usr/bin/env python3
# SPDX-License-Identifier: GPL-2.0
'''Host harness for the real research2 config/base validators and old-path regression.

This test extracts the real kernel source of

  * idma_research2_config_validate and idma_research2_base_validate from
    linux/drivers/misc/idma_research.h,
  * idma_p4m_validate and idma_p4m_validate_cfg from linux/drivers/misc/idma.c,
  * idma_research_validate from linux/drivers/misc/idma_research.h.

It compiles them unchanged into a host program with small stubs.  Nothing is
hand-copied from the UAPI: the real UAPI header is copied verbatim into a
temporary include/linux/idma.h.  This only checks software validation and does
not touch an FPGA.
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
RESEARCH_H_PATH = REPO_ROOT / 'linux' / 'drivers' / 'misc' / 'idma_research.h'
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


PREAMBLE = r'''#include <errno.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <linux/idma.h>
#define __user
#define BIT_ULL(n) (1ULL << (n))
#define PAGE_SIZE 4096UL
#define SZ_4K 4096UL
struct idma_device { int id; __u32 did; };
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

static void init_cfg(struct idma_research2_config *c, __u32 role)
{
    memset(c, 0, sizeof(*c));
    c->abi_version = IDMA_RESEARCH2_ABI;
    c->struct_size = sizeof(*c);
    c->required_role = role;
}

static void init_research(struct idma_research *r, __u32 mode)
{
    memset(r, 0, sizeof(*r));
    r->abi_version = IDMA_RESEARCH_ABI;
    r->struct_size = sizeof(*r);
    r->mode = mode;
    r->raw_words = IDMA_RESEARCH_RAW_WORDS;
    r->raw_user = 0x1000;
}

static void init_p4m(struct idma_p4m *p, __u32 gap)
{
    memset(p, 0, sizeof(*p));
    p->experiment.abi_version = IDMA_EXPERIMENT_ABI;
    p->experiment.struct_size = sizeof(p->experiment);
    p->experiment.aggressor_id = 1;
    p->experiment.victim_pages = 1;
    p->experiment.aggressor_pages = 8;
    p->experiment.chunk_bytes = PAGE_SIZE;
    p->experiment.victim_transfers = 64;
    p->experiment.warmup_transfers = 8;
    p->experiment.run_timeout_ms = 10000;
    p->aggressor_gap_cycles = gap;
}

int main(void)
{
    struct idma_device victim = { .id = 0, .did = 10 };
    struct idma_research2_config c;
    struct idma_research r;
    struct idma_p4m p;

    init_cfg(&c, IDMA_RESEARCH2_E2_ROLE);
    c.reverse = 1;
    check(idma_research2_config_validate(&c, IDMA_RESEARCH2_E2_ROLE) == 0,
          "E2 cfg with only abi/size/role/reverse must pass");
    c.policy_mode = 1;
    check(idma_research2_config_validate(&c, IDMA_RESEARCH2_E2_ROLE) != 0,
          "E2 cfg non-zero policy must fail");

    init_cfg(&c, IDMA_RESEARCH2_METHOD_ROLE);
    c.policy_mode = 0;
    c.window_cycles = 16384;
    c.target_bytes = 100000;
    c.deadband_bytes = 4096;
    c.initial_budget = 100;
    c.min_budget = 1;
    c.max_budget = 1000;
    c.budget_step = 1;
    c.credit_cap = 2000;
    c.hit_weight = 1;
    c.miss_weight = 2;
    c.ptw_weight = 3;
    check(idma_research2_config_validate(&c, IDMA_RESEARCH2_METHOD_ROLE) == 0,
          "M cfg bounded values must pass");
    c.min_budget = 0;
    c.policy_mode = 1;
    check(idma_research2_config_validate(&c, IDMA_RESEARCH2_METHOD_ROLE) != 0,
          "billing mode with min 0 must fail");
    c.min_budget = 0;
    c.policy_mode = 6;
    c.max_budget = 200000;
    check(idma_research2_config_validate(&c, IDMA_RESEARCH2_METHOD_ROLE) == 0,
          "mode6 min0 max200000 must pass");
    c.max_budget = 200001;
    check(idma_research2_config_validate(&c, IDMA_RESEARCH2_METHOD_ROLE) != 0,
          "mode6 max above 200000 must fail");

    init_research(&r, IDMA_RESEARCH_E2);
    r.a_pages = 8;
    r.a_transfers = 16;
    r.warmup = 2;
    r.transfers = 1;
    r.window_cycles = 5000000;
    r.timeout_ms = 2000;
    init_cfg(&c, IDMA_RESEARCH2_E2_ROLE);
    c.reverse = 1;
    check(idma_research2_base_validate(&victim, &r, &c) == 0,
          "E2 a8/16 reverse must pass");
    r.a_pages = 2;
    check(idma_research2_base_validate(&victim, &r, &c) != 0,
          "E2 a_pages=2 must fail");

    init_research(&r, IDMA_RESEARCH_COST);
    r.a_pages = 8;
    r.a_transfers = 0;
    r.transfers = 64;
    r.warmup = 8;
    r.timeout_ms = 10000;
    r.gap_cycles = 500;
    init_cfg(&c, IDMA_RESEARCH2_METHOD_ROLE);
    c.policy_mode = 0;
    check(idma_research2_base_validate(&victim, &r, &c) == 0,
          "M cost gap500 policy0 must pass");
    c.policy_mode = 1;
    check(idma_research2_base_validate(&victim, &r, &c) != 0,
          "M gap500 with policy!=0 must fail");
    c.policy_mode = 0;
    r.a_pages = 0;
    check(idma_research2_base_validate(&victim, &r, &c) != 0,
          "Solo with non-zero gap must fail");

    init_p4m(&p, 500);
    init_cfg(&c, IDMA_RESEARCH2_METHOD_ROLE);
    c.policy_mode = 0;
    check(idma_p4m_validate_cfg(&victim, &p, &c) == 0,
          "new cfg path must allow bounded gap500 policy0");
    c.policy_mode = 1;
    check(idma_p4m_validate_cfg(&victim, &p, &c) != 0,
          "new cfg path must reject gap500 policy1");
    check(idma_p4m_validate(&victim, &p) != 0,
          "old p4m_validate must still reject gap500");

    init_research(&r, IDMA_RESEARCH_COST);
    r.a_pages = 8;
    r.transfers = 64;
    r.warmup = 8;
    r.timeout_ms = 10000;
    r.gap_cycles = 500;
    check(idma_research_validate(&victim, &r) != 0,
          "old research validate must not be weakened");

    if (failures) {
        printf("%d check(s) failed\n", failures);
        return 1;
    }
    printf("OK: research2 config/base and old-path regression honoured the contract\n");
    return 0;
}
'''


class Research2ConfigDriverTests(unittest.TestCase):
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
        if not RESEARCH_H_PATH.is_file() or not IDMA_C_PATH.is_file():
            self.skipTest('driver sources missing')
        header = RESEARCH_H_PATH.read_text(encoding='utf-8')
        driver = IDMA_C_PATH.read_text(encoding='utf-8')
        private = extract_private_defines(driver)
        p4m = extract_function(driver, 'static int idma_p4m_validate(', 'idma.c')
        p4m_cfg = extract_function(driver, 'static int idma_p4m_validate_cfg(', 'idma.c')
        old_rv = extract_function(header, 'static int idma_research_validate(', 'idma_research.h')
        cfgv = extract_function(header, 'static int idma_research2_config_validate(', 'idma_research.h')
        basev = extract_function(header, 'static int idma_research2_base_validate(', 'idma_research.h')
        pieces = [PREAMBLE, '\n'.join(private), p4m, p4m_cfg, old_rv, cfgv, basev, HARNESS_MAIN]
        return '\n'.join(pieces) + '\n'

    def run_harness(self):
        src = self.base / 'research2_config.c'
        exe = self.base / 'research2_config'
        src.write_text(self.build_source(), encoding='utf-8')
        build = subprocess.run([self.cc, '-std=gnu11', '-O0', '-w', '-I', str(self.include_dir), '-o', str(exe), str(src)], capture_output=True, text=True)
        if build.returncode != 0:
            self.fail('compilation failed:\n%s' % (build.stdout + build.stderr))
        return subprocess.run([str(exe)], capture_output=True, text=True)

    def test_real_research2_config_and_old_path(self):
        result = self.run_harness()
        self.assertEqual(result.returncode, 0, result.stdout + result.stderr)
        self.assertIn('OK', result.stdout)


if __name__ == '__main__':
    unittest.main()
