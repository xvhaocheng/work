#!/usr/bin/env python3
# SPDX-License-Identifier: GPL-2.0
'''Host harness for the real research2 config/base validators, preflight and method CLEAR/ARM.

It extracts the real functions from the kernel sources and compiles them with fake MMIO.
No hand-copied validation logic is used.
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


def dedupe_defines(defines):
    seen = {}
    for line in defines:
        name = line.split()[1]
        seen[name] = line
    return list(seen.values())


BASE_PREAMBLE = r'''#include <errno.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
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
#define SZ_4K 4096UL
typedef uint64_t phys_addr_t;
struct idma_device { int id; __u32 did; void *reg; };
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

FAKE_MMIO_PREAMBLE = r'''
static u64 fake_regs[0x4000];
static u64 fake_index;
static inline void fake_writeq(u64 val, void *addr)
{
    unsigned long off = (unsigned long)addr;
    if (off == IDMA_OBS_INDEX_OFFSET) {
        fake_index = val;
    } else if (off == IDMA_OBS_DATA_OFFSET) {
        if (fake_index == IDMA_RESEARCH_BANK + IDMA_RESEARCH2_METHOD_E04_COMMAND) {
            if (val == IDMA_RESEARCH2_METHOD_CLEAR) {
                fake_regs[IDMA_RESEARCH_BANK + IDMA_RESEARCH2_METHOD_E03_STATUS] = 0;
                fake_regs[IDMA_RESEARCH_BANK + IDMA_RESEARCH2_METHOD_E02_ERROR] = 0;
            } else if (val == IDMA_RESEARCH2_METHOD_ARM) {
                fake_regs[IDMA_RESEARCH_BANK + IDMA_RESEARCH2_METHOD_E03_STATUS] =
                    IDMA_RESEARCH2_METHOD_STATUS_ARMED;
                fake_regs[IDMA_RESEARCH_BANK + IDMA_RESEARCH2_METHOD_E02_ERROR] = 0;
            } else if (val == IDMA_RESEARCH2_METHOD_FREEZE) {
                fake_regs[IDMA_RESEARCH_BANK + IDMA_RESEARCH2_METHOD_E03_STATUS] &=
                    ~IDMA_RESEARCH2_METHOD_STATUS_ARMED;
                fake_regs[IDMA_RESEARCH_BANK + IDMA_RESEARCH2_METHOD_E03_STATUS] |=
                    IDMA_RESEARCH2_METHOD_STATUS_FROZEN;
            } else if (val == IDMA_RESEARCH2_METHOD_ABORT) {
                fake_regs[IDMA_RESEARCH_BANK + IDMA_RESEARCH2_METHOD_E03_STATUS] |=
                    IDMA_RESEARCH2_METHOD_STATUS_ABORT;
            }
        }
        fake_regs[fake_index] = val;
    } else {
        fake_regs[off / 8] = val;
    }
}
static inline u64 fake_readq(void *addr)
{
    unsigned long off = (unsigned long)addr;
    if (off == IDMA_OBS_DATA_OFFSET)
        return fake_regs[fake_index];
    return fake_regs[off / 8];
}
#define writeq fake_writeq
#define readq fake_readq
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

static void set_caps(struct idma_device *d, u64 magic, u64 version, u64 role, u64 features)
{
    idma_research_write(d, IDMA_RESEARCH2_CAP_INDEX, magic);
    idma_research_write(d, IDMA_RESEARCH2_CAP_INDEX + 1, version);
    idma_research_write(d, IDMA_RESEARCH2_CAP_INDEX + 2, role);
    idma_research_write(d, IDMA_RESEARCH2_CAP_INDEX + 3, features);
}

static void set_method_status(u64 status, u64 error)
{
    fake_regs[IDMA_RESEARCH_BANK + IDMA_RESEARCH2_METHOD_E03_STATUS] = status;
    fake_regs[IDMA_RESEARCH_BANK + IDMA_RESEARCH2_METHOD_E02_ERROR] = error;
}

int main(void)
{
    struct idma_device victim = { .id = 0, .did = 10, .reg = (void *)0 };
    struct idma_research2_config cfg;
    struct idma_research r;
    u32 i;

    fake_regs[IDMA_OBS_MAGIC_OFFSET / 8] = IDMA_OBS_MAGIC_V8;

    init_cfg(&cfg, IDMA_RESEARCH2_E2_ROLE);
    cfg.reverse = 1;
    check(idma_research2_config_validate(&cfg, IDMA_RESEARCH2_E2_ROLE) == 0,
          "E2 cfg abi/size/role/reverse must pass");
    cfg.policy_mode = 1;
    check(idma_research2_config_validate(&cfg, IDMA_RESEARCH2_E2_ROLE) != 0,
          "E2 cfg non-zero policy must fail");

    init_cfg(&cfg, IDMA_RESEARCH2_METHOD_ROLE);
    cfg.policy_mode = 0;
    cfg.window_cycles = 16384;
    cfg.target_bytes = 100000;
    cfg.deadband_bytes = 4096;
    cfg.initial_budget = 100;
    cfg.min_budget = 1;
    cfg.max_budget = 1000;
    cfg.budget_step = 1;
    cfg.credit_cap = 2000;
    cfg.hit_weight = 1;
    cfg.miss_weight = 2;
    cfg.ptw_weight = 3;
    check(idma_research2_config_validate(&cfg, IDMA_RESEARCH2_METHOD_ROLE) == 0,
          "M cfg bounded values must pass");
    cfg.min_budget = 0;
    cfg.policy_mode = 1;
    check(idma_research2_config_validate(&cfg, IDMA_RESEARCH2_METHOD_ROLE) != 0,
          "billing mode with min 0 must fail");
    cfg.min_budget = 0;
    cfg.policy_mode = 6;
    cfg.max_budget = 200000;
    cfg.credit_cap = 200000;
    check(idma_research2_config_validate(&cfg, IDMA_RESEARCH2_METHOD_ROLE) == 0,
          "mode6 min0 max200000 must pass");
    cfg.min_budget = 50;
    check(idma_research2_config_validate(&cfg, IDMA_RESEARCH2_METHOD_ROLE) == 0,
          "mode6 min>0 must pass");
    cfg.max_budget = 200001;
    check(idma_research2_config_validate(&cfg, IDMA_RESEARCH2_METHOD_ROLE) != 0,
          "mode6 max above 200000 must fail");

    init_cfg(&cfg, IDMA_RESEARCH2_METHOD_ROLE);
    cfg.policy_mode = 4;
    cfg.window_cycles = 16384;
    cfg.target_bytes = 100000;
    cfg.deadband_bytes = 4096;
    cfg.initial_budget = 100;
    cfg.min_budget = 1;
    cfg.max_budget = 1000;
    cfg.budget_step = 1;
    cfg.credit_cap = 2000;
    cfg.hit_weight = 0;
    cfg.miss_weight = 0;
    cfg.ptw_weight = 0;
    check(idma_research2_config_validate(&cfg, IDMA_RESEARCH2_METHOD_ROLE) != 0,
          "mode4 all-zero weights must fail early");

    init_research(&r, IDMA_RESEARCH_E2);
    r.a_pages = 8;
    r.a_transfers = 16;
    r.warmup = 2;
    r.transfers = 1;
    r.window_cycles = 5000000;
    r.timeout_ms = 2000;
    init_cfg(&cfg, IDMA_RESEARCH2_E2_ROLE);
    cfg.reverse = 1;
    check(idma_research2_base_validate(&victim, &r, &cfg) == 0,
          "E2 a8/16 reverse must pass");
    r.a_pages = 2;
    check(idma_research2_base_validate(&victim, &r, &cfg) != 0,
          "E2 a_pages=2 must fail");

    init_research(&r, IDMA_RESEARCH_COST);
    r.a_pages = 8;
    r.a_transfers = 0;
    r.transfers = 64;
    r.warmup = 8;
    r.timeout_ms = 10000;
    r.gap_cycles = 500;
    init_cfg(&cfg, IDMA_RESEARCH2_METHOD_ROLE);
    cfg.policy_mode = 0;
    check(idma_research2_base_validate(&victim, &r, &cfg) == 0,
          "M cost gap500 policy0 must pass");
    cfg.policy_mode = 1;
    check(idma_research2_base_validate(&victim, &r, &cfg) != 0,
          "M gap500 with policy!=0 must fail");
    cfg.policy_mode = 0;
    r.a_pages = 0;
    check(idma_research2_base_validate(&victim, &r, &cfg) != 0,
          "Solo with non-zero gap must fail");

    init_cfg(&cfg, IDMA_RESEARCH2_E2_ROLE);
    cfg.reverse = 1;
    set_caps(&victim, IDMA_RESEARCH2_CAP_MAGIC, IDMA_RESEARCH2_CAP_VERSION,
             IDMA_RESEARCH2_E2_ROLE,
             IDMA_RESEARCH2_FEATURE_EXTENDED_E2 |
             IDMA_RESEARCH2_FEATURE_E2_TIMING |
             IDMA_RESEARCH2_FEATURE_REVERSE_A);
    check(idma_research2_preflight(&victim, &cfg) == 0,
          "E2 preflight correct caps must pass");
    set_caps(&victim, IDMA_RESEARCH2_CAP_MAGIC, IDMA_RESEARCH2_CAP_VERSION,
             IDMA_RESEARCH2_E2_ROLE,
             IDMA_RESEARCH2_FEATURE_EXTENDED_E2 |
             IDMA_RESEARCH2_FEATURE_E2_TIMING);
    check(idma_research2_preflight(&victim, &cfg) == -EOPNOTSUPP,
          "E2 preflight missing reverse feature must reject");
    set_caps(&victim, IDMA_RESEARCH2_CAP_MAGIC, 0,
             IDMA_RESEARCH2_E2_ROLE,
             IDMA_RESEARCH2_FEATURE_EXTENDED_E2 |
             IDMA_RESEARCH2_FEATURE_E2_TIMING |
             IDMA_RESEARCH2_FEATURE_REVERSE_A);
    check(idma_research2_preflight(&victim, &cfg) == -EOPNOTSUPP,
          "E2 preflight wrong version must reject");
    set_caps(&victim, 0, IDMA_RESEARCH2_CAP_VERSION, IDMA_RESEARCH2_E2_ROLE,
             IDMA_RESEARCH2_FEATURE_EXTENDED_E2 |
             IDMA_RESEARCH2_FEATURE_E2_TIMING |
             IDMA_RESEARCH2_FEATURE_REVERSE_A);
    check(idma_research2_preflight(&victim, &cfg) == -EOPNOTSUPP,
          "wrong F00 magic must reject before DMA");
    set_caps(&victim, IDMA_RESEARCH2_CAP_MAGIC, IDMA_RESEARCH2_CAP_VERSION,
             IDMA_RESEARCH2_METHOD_ROLE,
             IDMA_RESEARCH2_FEATURE_EXTENDED_E2 |
             IDMA_RESEARCH2_FEATURE_E2_TIMING |
             IDMA_RESEARCH2_FEATURE_REVERSE_A);
    check(idma_research2_preflight(&victim, &cfg) == -EOPNOTSUPP,
          "wrong F02 role must reject before DMA");
    fake_regs[IDMA_OBS_MAGIC_OFFSET / 8] = 0;
    check(idma_research2_preflight(&victim, &cfg) == -EOPNOTSUPP,
          "wrong observer magic must reject before DMA");
    fake_regs[IDMA_OBS_MAGIC_OFFSET / 8] = IDMA_OBS_MAGIC_V8;
    victim.id = 1;
    check(idma_research2_preflight(&victim, &cfg) == -EINVAL,
          "wrong id must reject before DMA");
    victim.id = 0;
    victim.did = 11;
    check(idma_research2_preflight(&victim, &cfg) == -EINVAL,
          "wrong DID must reject before DMA");
    victim.did = 10;

    init_cfg(&cfg, IDMA_RESEARCH2_METHOD_ROLE);
    set_caps(&victim, IDMA_RESEARCH2_CAP_MAGIC, IDMA_RESEARCH2_CAP_VERSION,
             IDMA_RESEARCH2_METHOD_ROLE,
             IDMA_RESEARCH2_FEATURE_EXTENDED_E2 |
             IDMA_RESEARCH2_FEATURE_E2_TIMING |
             IDMA_RESEARCH2_FEATURE_REVERSE_A |
             IDMA_RESEARCH2_FEATURE_METHOD);
    check(idma_research2_preflight(&victim, &cfg) == 0,
          "method preflight full feature mask must pass");
    set_method_status(IDMA_RESEARCH2_METHOD_STATUS_ARMED, 0);
    check(idma_research2_preflight(&victim, &cfg) == -EBUSY,
          "unfinished policy rejected before warmup");
    set_method_status(IDMA_RESEARCH2_METHOD_STATUS_FROZEN, 0);
    check(idma_research2_preflight(&victim, &cfg) == 0,
          "frozen previous result permits a new run");
    set_method_status(0, 0);
    set_caps(&victim, IDMA_RESEARCH2_CAP_MAGIC, IDMA_RESEARCH2_CAP_VERSION,
             IDMA_RESEARCH2_METHOD_ROLE,
             IDMA_RESEARCH2_FEATURE_METHOD);
    check(idma_research2_preflight(&victim, &cfg) == -EOPNOTSUPP,
          "method preflight missing feature bits must reject");

    init_cfg(&cfg, IDMA_RESEARCH2_METHOD_ROLE);
    cfg.policy_mode = 0;
    cfg.window_cycles = 16384;
    cfg.target_bytes = 100000;
    cfg.deadband_bytes = 4096;
    cfg.initial_budget = 100;
    cfg.min_budget = 1;
    cfg.max_budget = 1000;
    cfg.budget_step = 1;
    cfg.credit_cap = 2000;
    cfg.hit_weight = 1;
    cfg.miss_weight = 2;
    cfg.ptw_weight = 3;
    idma_research_write(&victim, IDMA_RESEARCH2_METHOD_INDEX,
                        IDMA_RESEARCH2_POLICY_MAGIC);
    idma_research_write(&victim, IDMA_RESEARCH2_METHOD_INDEX + 1, 1ULL);
    set_method_status(IDMA_RESEARCH2_METHOD_STATUS_ARMED, 0);
    check(idma_research2_method_configure(&victim, &cfg) == -EBUSY,
          "already armed policy must reject configure");
    set_method_status(IDMA_RESEARCH2_METHOD_STATUS_FROZEN |
                      IDMA_RESEARCH2_METHOD_STATUS_ABORT, 0);
    check(idma_research2_method_configure(&victim, &cfg) == 0,
          "frozen/abort policy legal CLEAR+configure+ARM");
    check((idma_research_read(&victim, IDMA_RESEARCH2_METHOD_E03_STATUS) &
           IDMA_RESEARCH2_METHOD_STATUS_ARMED) &&
          !(idma_research_read(&victim, IDMA_RESEARCH2_METHOD_E03_STATUS) &
            (IDMA_RESEARCH2_METHOD_STATUS_FROZEN |
             IDMA_RESEARCH2_METHOD_STATUS_ABORT)),
          "ARM readback requires armed and no frozen/abort");
    check(idma_research_read(&victim, IDMA_RESEARCH2_METHOD_E02_ERROR) == 0,
          "E02 zero after ARM");
    for (i = 0; i < 12; i++)
        check(idma_research_read(&victim, IDMA_RESEARCH2_METHOD_E05_MODE + i) ==
              (i == 0 ? cfg.policy_mode : i == 1 ? cfg.window_cycles :
               i == 2 ? cfg.target_bytes : i == 3 ? cfg.deadband_bytes :
               i == 4 ? cfg.initial_budget : i == 5 ? cfg.min_budget :
               i == 6 ? cfg.max_budget : i == 7 ? cfg.budget_step :
               i == 8 ? cfg.credit_cap : i == 9 ? cfg.hit_weight :
               i == 10 ? cfg.miss_weight : cfg.ptw_weight),
              "configured register readback");
    check(idma_research2_method_configure(&victim, &cfg) == -EBUSY,
          "second run must reject already armed");

    if (failures) {
        printf("%d check(s) failed\n", failures);
        return 1;
    }
    printf("OK: research2 config/base/preflight/method-clear honoured the contract\n");
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
        private = dedupe_defines(extract_private_defines(driver) + extract_private_defines(header))
        pieces = [BASE_PREAMBLE, '\n'.join(private), FAKE_MMIO_PREAMBLE]
        for signature, source, path in [
            ('static u64 idma_research_read(', header, 'idma_research.h'),
            ('static void idma_research_write(', header, 'idma_research.h'),
            ('static int idma_research2_set_reverse(', header, 'idma_research.h'),
            ('static int idma_research2_method_status_read(', header, 'idma_research.h'),
            ('static int idma_research2_method_configure(', header, 'idma_research.h'),
            ('static void idma_research2_method_abort(', header, 'idma_research.h'),
            ('static int idma_research2_method_freeze(', header, 'idma_research.h'),
            ('static int idma_research2_method_readback_ok(', header, 'idma_research.h'),
            ('static int idma_research2_pre_arm(', header, 'idma_research.h'),
            ('static int idma_research2_config_validate(', header, 'idma_research.h'),
            ('static int idma_research2_base_validate(', header, 'idma_research.h'),
            ('static int idma_research2_extra_validate(', header, 'idma_research.h'),
            ('static int idma_research2_preflight(', header, 'idma_research.h'),
            ('static int idma_research_validate(', header, 'idma_research.h'),
            ('static int idma_p4m_validate(', driver, 'idma.c'),
            ('static int idma_p4m_validate_cfg(', driver, 'idma.c'),
        ]:
            pieces.append(extract_function(source, signature, path))
        pieces.append(HARNESS_MAIN)
        return '\n'.join(pieces) + '\n'

    def run_harness(self):
        src = self.base / 'research2_config.c'
        exe = self.base / 'research2_config'
        src.write_text(self.build_source(), encoding='utf-8')
        build = subprocess.run([self.cc, '-std=gnu11', '-O0', '-I', str(self.include_dir), '-o', str(exe), str(src)], capture_output=True, text=True)
        if build.returncode != 0:
            self.fail('compilation failed:\n%s' % (build.stdout + build.stderr))
        return subprocess.run([str(exe)], capture_output=True, text=True)

    def test_real_research2_config_preflight_and_method_clear(self):
        result = self.run_harness()
        self.assertEqual(result.returncode, 0, result.stdout + result.stderr)
        self.assertIn('OK', result.stdout)


if __name__ == '__main__':
    unittest.main()
