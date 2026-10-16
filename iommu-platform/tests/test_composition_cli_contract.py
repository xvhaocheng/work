#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""Producer -> validator contract test for the composition CLI.

End-to-end at process level: it compiles the real bench/idma_composition.c with
the real Linux UAPI header and a small C mock that interposes open/open64/
ioctl/close with the linker --wrap option.  The mock only fills the struct the
way the frozen UAPI describes (no hardware is simulated), so the JSON printed
by the actual C producer is handed, unchanged, to the actual
composition_plan.validate_case.

Schema/contract only: it never touches /dev/idma0.  gcc -Wall -Wextra -Werror
is required.  This file does not claim the test has run.
"""
from __future__ import annotations

import importlib.util
import json
import os
import pathlib
import shutil
import subprocess
import sys
import tempfile
import unittest

HERE = pathlib.Path(__file__).resolve()
REPO = HERE.parent.parent
PLAN_SCRIPT = REPO / 'scripts' / 'composition_plan.py'
CLI_SOURCE = REPO / 'bench' / 'idma_composition.c'
UAPI_HEADER = REPO / 'linux' / 'include' / 'uapi' / 'linux' / 'idma.h'

MOCK_SOURCE = r'''
/* Test-only mock: interposes open/open64/ioctl/close via --wrap. */
#define _POSIX_C_SOURCE 200809L
#include <errno.h>
#include <fcntl.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <linux/idma.h>

#ifndef IDMA_COMPOSITION_ABI
#error "the real UAPI header must define IDMA_COMPOSITION_ABI"
#endif

static void mock_log_open(void)
{
	const char *path = getenv("IDMA_MOCK_OPEN_LOG");
	FILE *handle;

	if (!path || !*path)
		return;
	handle = fopen(path, "a");
	if (!handle)
		return;
	fputs("open" "\n", handle);
	fclose(handle);
}

int __wrap_open(const char *path, int flags, ...)
{
	(void)path;
	(void)flags;
	mock_log_open();
	return 42;
}

int __wrap_open64(const char *path, int flags, ...)
{
	(void)path;
	(void)flags;
	mock_log_open();
	return 42;
}

int __wrap_close(int fd)
{
	(void)fd;
	return 0;
}

static int mock_fill_result(struct idma_composition *r)
{
	unsigned int mode;
	unsigned int v_gap;
	unsigned long long config;
	unsigned long long generation = 7;
	unsigned long long cycles = 2000;
	unsigned long long transfers;
	int has_aggressor;

	mode = r->mode & 0x3u;
	v_gap = r->victim_gap_cycles;
	config = (unsigned long long)(mode & 0x3u) |
		 ((unsigned long long)(r->protected_did & 0xFFFFFFu) << 8);
	transfers = (unsigned long long)r->p4m.experiment.victim_transfers;
	has_aggressor = r->p4m.experiment.aggressor_id != IDMA_EXPERIMENT_NO_AGGRESSOR;

	r->abi_version = IDMA_COMPOSITION_ABI;
	r->struct_size = (__u32)sizeof(*r);
	r->status = 0;
	r->restore_status = 0;
	r->configured = 1;
	r->raw_captured = 1;
	r->generation = generation;
	r->restored_generation = generation + 1;

	r->p4m.experiment.status = 0;
	r->p4m.experiment.victim_did = r->protected_did;
	memset(&r->p4m.experiment.victim, 0, sizeof(r->p4m.experiment.victim));
	memset(&r->p4m.experiment.aggressor, 0, sizeof(r->p4m.experiment.aggressor));
	if (has_aggressor)
		r->p4m.experiment.aggressor_did = 11;
	else
		r->p4m.experiment.aggressor_did = 0;

	r->p4m.aggressor_gap_cycles = 0;
	r->p4m.sequence = 4;
	r->p4m.observer_status = 4;   /* P4M observer status OK */

	memset(r->p4m.observation, 0, sizeof(r->p4m.observation));
	r->p4m.observation[0] = cycles;
	r->p4m.observation[7] = transfers;
	r->p4m.observation[8] = transfers;

	memset(r->p4m.window, 0, sizeof(r->p4m.window));
	r->p4m.window[0] = 0x49444D4150344D01ULL;   /* P4M bank magic */
	r->p4m.window[1] = cycles;
	r->p4m.window[2] = transfers;
	r->p4m.window[3] = transfers;
	r->p4m.window[13] = transfers;
	r->p4m.window[14] = transfers;
	r->p4m.window[15] = has_aggressor ? 0x3u : 0x2u;

	memset(r->raw, 0, sizeof(r->raw));
	r->raw[0] = IDMA_COMPOSITION_MAGIC;
	r->raw[1] = IDMA_COMPOSITION_ABI;
	r->raw[2] = config;
	r->raw[3] = generation;
	r->raw[6] = 0x4u;   /* WINDOW_FLAG_FROZEN */
	r->raw[7] = 4;
	r->raw[8] = cycles;
	r->raw[9] = 0;
	r->raw[10] = config;
	r->raw[11] = generation;
	r->raw[12] = 100; r->raw[13] = 70; r->raw[14] = 30;
	if (has_aggressor) { r->raw[15] = 50; r->raw[16] = 20; r->raw[17] = 30; }
	r->raw[18] = 12; r->raw[19] = 5; r->raw[20] = 7;
	r->raw[21] = 0; r->raw[22] = 0; r->raw[23] = 0;
	r->raw[24] = v_gap;
	r->raw[25] = 0;
	r->raw[33] = 0;
	return 0;
}

int __wrap_ioctl(int fd, unsigned long request, ...)
{
	va_list ap;
	void *arg;

	(void)fd;
	va_start(ap, request);
	arg = va_arg(ap, void *);
	va_end(ap);
	if (request != (unsigned long)IDMA_IOC_RUN_COMPOSITION) {
		errno = ENOTTY;
		return -1;
	}
	if (getenv("IDMA_MOCK_IOCTL_FAIL")) {
		errno = EIO;
		return -1;
	}
	return mock_fill_result((struct idma_composition *)arg);
}
'''


def load_plan_module():
    spec = importlib.util.spec_from_file_location(
        'composition_plan_cli_contract', PLAN_SCRIPT)
    if spec is None or spec.loader is None:
        raise RuntimeError('cannot load ' + str(PLAN_SCRIPT))
    module = importlib.util.module_from_spec(spec)
    sys.modules['composition_plan_cli_contract'] = module
    spec.loader.exec_module(module)
    return module


plan = load_plan_module()


@unittest.skipUnless(shutil.which('gcc'), 'gcc is required for the producer contract test')
@unittest.skipUnless(CLI_SOURCE.is_file(), 'bench/idma_composition.c is missing')
@unittest.skipUnless(UAPI_HEADER.is_file(), 'linux/include/uapi/linux/idma.h is missing')
class CompositionCliContractTests(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        cls._tmp = tempfile.TemporaryDirectory(prefix='idma-composition-cli-')
        cls.tmp = pathlib.Path(cls._tmp.name)
        # Stage only the repository UAPI header under test.  Putting all of
        # linux/include/uapi on the include path shadows the host's exported
        # linux/types.h with the kernel-internal one, which then warns
        # "Attempt to use kernel headers from user space" and needs
        # linux/compiler_types.h; -Werror turns that into a hard failure.
        # bench() in scripts/platform.py stages the same single header for the
        # board cross build.
        cls.include = cls.tmp / 'include'
        (cls.include / 'linux').mkdir(parents=True, exist_ok=True)
        shutil.copy2(UAPI_HEADER, cls.include / 'linux' / 'idma.h')
        mock_path = cls.tmp / 'mock_idma.c'
        mock_path.write_text(MOCK_SOURCE, encoding='utf-8')
        cls.binary = cls.tmp / 'idma_composition'
        cmd = [
            'gcc', '-Wall', '-Wextra', '-Werror', '-std=gnu99',
            '-I', str(cls.include),
            str(CLI_SOURCE), str(mock_path),
            '-Wl,--wrap=open', '-Wl,--wrap=open64',
            '-Wl,--wrap=close', '-Wl,--wrap=ioctl',
            '-o', str(cls.binary),
        ]
        proc = subprocess.run(cmd, stdout=subprocess.PIPE, stderr=subprocess.PIPE)
        if proc.returncode != 0:
            raise AssertionError(
                'gcc failed for the producer contract build:\n'
                + proc.stderr.decode('utf-8', errors='replace'))
        cls.open_log = cls.tmp / 'open.log'

    @classmethod
    def tearDownClass(cls):
        cls._tmp.cleanup()

    def setUp(self):
        if self.open_log.exists():
            self.open_log.unlink()

    def run_cli(self, arguments, extra_env=None):
        env = dict(os.environ)
        env['IDMA_MOCK_OPEN_LOG'] = str(self.open_log)
        env.pop('IDMA_MOCK_IOCTL_FAIL', None)
        if extra_env:
            env.update(extra_env)
        return subprocess.run([str(self.binary)] + list(arguments),
                              stdout=subprocess.PIPE, stderr=subprocess.PIPE, env=env)

    @staticmethod
    def case_for(mode, a_pages, v_gap, transfers, warmup):
        return {
            'id': 'cli-m%d-a%d-g%d' % (mode, a_pages, v_gap),
            'phase': 'short',
            'block': None,
            'pair': None,
            'slot': None,
            'mode': mode,
            'a_pages': a_pages,
            'v_gap_cycles': v_gap,
            'transfers': transfers,
            'warmup': warmup,
        }

    @staticmethod
    def success_argv(mode, a_pages, v_gap, transfers, warmup):
        return [
            '--device', '/dev/idma0',
            '--mode', str(mode),
            '--a-pages', str(a_pages),
            '--v-gap', str(v_gap),
            '--transfers', str(transfers),
            '--warmup', str(warmup),
            '--timeout-ms', '15000',
        ]

    def record_of(self, proc):
        text = proc.stdout.decode('utf-8', errors='replace')
        self.assertTrue(text.strip(), 'the CLI printed nothing on stdout')
        return json.loads(text)

    def validate(self, mode, a_pages, v_gap, transfers, warmup):
        proc = self.run_cli(self.success_argv(mode, a_pages, v_gap, transfers, warmup))
        self.assertTrue(self.open_log.exists(), 'open() was never called')
        record = self.record_of(proc)
        self.assertIs(record.get('success'), True,
                      'producer reported failure: ' + repr(record.get('error')))
        case = self.case_for(mode, a_pages, v_gap, transfers, warmup)
        problems = plan.validate_case(case, record, {
            'exit': proc.returncode,
            'taint_before': '0',
            'taint_after': '0',
        })
        self.assertEqual(problems, [])
        return record

    def test_solo_short(self):
        record = self.validate(plan.MODE_S, 0, 0, 16, 16)
        result = record['result']
        self.assertEqual(result['raw'][plan.RAW_MAGIC], plan.COMPOSITION_MAGIC)
        self.assertEqual(len(result['raw']), plan.RAW_WORDS)
        self.assertEqual(len(result['p4m']['window']), plan.P4M_WINDOW_WORDS)
        self.assertEqual(len(result['p4m']['observation']), plan.P4M_OBS_WORDS)
        self.assertIsNone(result['experiment']['aggressor'])
        self.assertEqual(record['request']['protected_did'], plan.PROTECTED_DID)
        self.assertEqual(result['experiment']['victim_did'], plan.PROTECTED_DID)

    def test_aggressor_a8_pn(self):
        record = self.validate(plan.MODE_PN, 8, 4000, 16, 16)
        result = record['result']
        self.assertEqual(result['raw'][plan.RAW_MAGIC], plan.COMPOSITION_MAGIC)
        self.assertEqual(result['experiment']['victim_did'], plan.PROTECTED_DID)
        self.assertNotEqual(result['experiment']['aggressor_did'], plan.PROTECTED_DID)
        self.assertIsInstance(result['experiment']['aggressor'], dict)
        self.assertEqual(result['raw'][plan.RAW_V_GAP], 4000)

    def test_all_mode_a8_vgap_combinations(self):
        for mode in plan.MODE_ORDER:
            for a_pages in (0, 8):
                for v_gap in plan.V_GAP_CHOICES:
                    with self.subTest(mode=mode, a_pages=a_pages, v_gap=v_gap):
                        self.validate(mode, a_pages, v_gap, 16, 16)

    def test_producer_reports_complete_program(self):
        record = self.validate(plan.MODE_N, 8, 1000, 32, 16)
        result = record['result']
        experiment = result['experiment']
        for key in ('victim_elapsed_ns', 'snapshot_before_ns', 'snapshot_after_ns',
                    'drain_ns', 'victim', 'aggressor',
                    'measurement_complete', 'measurement_started'):
            self.assertIn(key, experiment)
        self.assertIn('p4m', result)
        self.assertIn('raw', result)
        self.assertNotIn('experiment', result['p4m'])
        self.assertNotIn('raw', result['p4m'])

    def test_help_does_not_open_the_device(self):
        proc = self.run_cli(['--help'])
        self.assertEqual(proc.returncode, 0)
        self.assertIn(b'Usage', proc.stdout)
        self.assertFalse(self.open_log.exists(), '--help opened the device')

    def test_abi_does_not_open_the_device(self):
        proc = self.run_cli(['--abi'])
        self.assertEqual(proc.returncode, 0)
        self.assertFalse(self.open_log.exists(), '--abi opened the device')
        abi = json.loads(proc.stdout.decode('utf-8', errors='replace'))
        self.assertEqual(abi['schema'], 'idma-composition-tool-v1')
        self.assertEqual(abi['record_schema'], plan.RECORD_SCHEMA)
        self.assertEqual(abi['composition_magic'], plan.COMPOSITION_MAGIC)
        self.assertEqual(abi['protected_did'], plan.PROTECTED_DID)

    def test_duplicate_option_is_json_error_without_open(self):
        argv = self.success_argv(plan.MODE_S, 0, 0, 16, 16) + ['--mode', '0']
        proc = self.run_cli(argv)
        self.assertNotEqual(proc.returncode, 0)
        self.assertFalse(self.open_log.exists(), 'a rejected argv opened the device')
        record = self.record_of(proc)
        self.assertIs(record['success'], False)
        self.assertEqual(record['stage'], 'arguments')
        self.assertIs(record['run_attempted'], False)
        self.assertIsNone(record['result'])

    def test_unknown_option_is_json_error_without_open(self):
        proc = self.run_cli(['--not-a-real-option'])
        self.assertNotEqual(proc.returncode, 0)
        self.assertFalse(self.open_log.exists(), 'a rejected argv opened the device')
        record = self.record_of(proc)
        self.assertIs(record['success'], False)
        self.assertEqual(record['stage'], 'arguments')
        self.assertIsNone(record['result'])

    def test_ioctl_failure_still_emits_valid_json(self):
        proc = self.run_cli(self.success_argv(plan.MODE_S, 0, 0, 16, 16),
                            extra_env={'IDMA_MOCK_IOCTL_FAIL': '1'})
        self.assertTrue(self.open_log.exists())
        self.assertNotEqual(proc.returncode, 0)
        record = self.record_of(proc)
        self.assertIs(record['success'], False)
        self.assertIsNone(record['result'])
        self.assertTrue(isinstance(record['error'], str) and record['error'])


if __name__ == '__main__':
    unittest.main()
