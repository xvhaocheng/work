## Findings summary

The producer/consumer contract for `idma-composition-v1` is currently split:

- **Producer (C)** prints `result.experiment` (correct) but, after the `p4m.window` array, keeps printing `"raw":[...]` *inside* the `p4m` object and only closes `p4m` with the `]}` at the end. Because the `result` object opened at line 350 and the top‑level object opened at line 314 are then never both closed, the record is missing one `}` and is **invalid JSON**. The intended/frozen shape is:
  - `result.experiment` (top level of `result`)
  - `result.p4m` = `{aggressor_gap_cycles, sequence, observer_status, observation, window}`
  - `result.raw[128]`
  - `semantics` at the **record top level**
- **Validator (`composition_plan.py`)** reads `result.p4m.experiment`, i.e. the wrong location; and it early‑returns before checking `experiment`/`raw` when `p4m` is not an object. `case_metrics` already reads `result.raw` and `p4m.window` (same contract), so it needs no move — only verification.
- **`mac_composition.py`**: a dead `if False` line; boot_id check is only “non‑empty” (must be UUID); `validate_case`/`case_metrics` can terminate a case with an unexpected exception before `meta.json` is written. The `SerialConsole(...)`/`.start()` call and `run_shell(..., exit_var='ec')` usage are **already correct** against `mac_research2.py` signatures and `run_shell` semantics — no change.
- **`tests/test_composition_plan.py`** synthesises `p4m.experiment` and puts `raw` under `p4m`, i.e. the old nesting.
- **New** end‑to‑end contract test requested: compile the real C producer with the real UAPI and a `--wrap` mock, then feed its stdout to the real validator.

All changes below are proposals only. **Nothing has been compiled or run.** Codex must execute the build/tests to verify.

---

## 1. `out/paper-route-review-20261004/proposals/bench/idma_composition.c`

One‑line bracket fix inside `report()` (close `p4m` before `raw`, so `raw` lands at `result` level and the trailing `]}` closes `raw`+`result` while the final `}}` closes `semantics`+top level).

**old**
```c
		printf("],\"raw\":[");
```
**new**
```c
		printf("]},\"raw\":[");
```

With this single edit, the closing sequence becomes: `]}` (close window, close p4m) … raw loop … `]}` (close raw, close result) … `,"semantics":{…}}` (close semantics, close top‑level). Braces then balance: 6 opens / 6 closes. `print_experiment` already emits `experiment` at `result` level, so no other producer change is required.

---

## 2. `out/paper-route-review-20261004/proposals/scripts/composition_plan.py`

Replace the `result`‑section body of `validate_case` (current lines 286–324) so that:
- `experiment` and `raw` are read from `result` (matching the producer),
- `p4m` validation is guarded (no early `return` that skips `experiment`/`raw`/`meta`),
- malformed nested data yields problems instead of an exception.

**old**
```python
    p4m = result.get('p4m')
    if not isinstance(p4m, dict):
        problems.append('result.p4m is not an object')
        return problems

    window = p4m.get('window')
    observation = p4m.get('observation')
    raw = result.get('raw')
    window_ok = _check_words(window, 'p4m.window', problems)
    observation_ok = _check_words(observation, 'p4m.observation', problems)
    raw_ok = _check_words(raw, 'result.raw', problems)
    if window_ok and len(window) != P4M_WINDOW_WORDS:
        problems.append('p4m.window is not %d words' % P4M_WINDOW_WORDS)
        window_ok = False
    if observation_ok and len(observation) != P4M_OBS_WORDS:
        problems.append('p4m.observation is not %d words' % P4M_OBS_WORDS)
        observation_ok = False
    if raw_ok and len(raw) != RAW_WORDS:
        problems.append('result.raw is not %d words' % RAW_WORDS)
        raw_ok = False

    if p4m.get('observer_status') != P4M_OBSERVER_STATUS_OK:
        problems.append('p4m.observer_status %r' % (p4m.get('observer_status'),))
    sequence = p4m.get('sequence')
    if isinstance(sequence, bool) or not isinstance(sequence, int) or sequence <= 0:
        problems.append('p4m.sequence %r' % (sequence,))

    experiment = p4m.get('experiment')
    if not isinstance(experiment, dict):
        problems.append('result.p4m.experiment is not an object')
    else:
        _validate_experiment(case, experiment, problems)

    if window_ok and observation_ok and raw_ok:
        _validate_frozen(case, window, observation, raw, generation, problems)

    if meta is not None:
        _validate_meta(meta, problems)
    return problems
```

**new**
```python
    experiment = result.get('experiment')
    if not isinstance(experiment, dict):
        problems.append('result.experiment is not an object')
    else:
        _validate_experiment(case, experiment, problems)

    raw = result.get('raw')
    raw_ok = _check_words(raw, 'result.raw', problems)
    if raw_ok and len(raw) != RAW_WORDS:
        problems.append('result.raw is not %d words' % RAW_WORDS)
        raw_ok = False

    window = None
    observation = None
    window_ok = False
    observation_ok = False
    p4m = result.get('p4m')
    if not isinstance(p4m, dict):
        problems.append('result.p4m is not an object')
    else:
        window = p4m.get('window')
        observation = p4m.get('observation')
        window_ok = _check_words(window, 'p4m.window', problems)
        observation_ok = _check_words(observation, 'p4m.observation', problems)
        if window_ok and len(window) != P4M_WINDOW_WORDS:
            problems.append('p4m.window is not %d words' % P4M_WINDOW_WORDS)
            window_ok = False
        if observation_ok and len(observation) != P4M_OBS_WORDS:
            problems.append('p4m.observation is not %d words' % P4M_OBS_WORDS)
            observation_ok = False
        if p4m.get('observer_status') != P4M_OBSERVER_STATUS_OK:
            problems.append('p4m.observer_status %r' % (p4m.get('observer_status'),))
        sequence = p4m.get('sequence')
        if isinstance(sequence, bool) or not isinstance(sequence, int) or sequence <= 0:
            problems.append('p4m.sequence %r' % (sequence,))

    if window_ok and observation_ok and raw_ok:
        _validate_frozen(case, window, observation, raw, generation, problems)

    if meta is not None:
        _validate_meta(meta, problems)
    return problems
```

`case_metrics` (lines 451–476) already reads `result['raw']` and `result['p4m']['window']` — same contract, **leave unchanged**.

---

## 3. `out/paper-route-review-20261004/proposals/scripts/mac_composition.py`

### 3a. add `re` import
**old**
```python
import json
import pathlib
import secrets
```
**new**
```python
import json
import pathlib
import re
import secrets
```

### 3b. add UUID pattern (after `TOOL_SCHEMA`)
**old**
```python
TOOL_SCHEMA = 'idma-composition-tool-v1'

NEWLINE = chr(10)
```
**new**
```python
TOOL_SCHEMA = 'idma-composition-tool-v1'

BOOT_ID_RE = re.compile(
    r'^[0-9a-fA-F]{8}-[0-9a-fA-F]{4}-[0-9a-fA-F]{4}-'
    r'[0-9a-fA-F]{4}-[0-9a-fA-F]{12}$')

NEWLINE = chr(10)
```

### 3c. boot_id must be a UUID, not merely non‑empty
**old**
```python
        info['boot_id'] = text_of(result.stdout).strip()
        if not info['boot_id']:
            raise CompositionError('boot_id is empty')
```
**new**
```python
        info['boot_id'] = text_of(result.stdout).strip()
        if not BOOT_ID_RE.fullmatch(info['boot_id']):
            raise CompositionError('boot_id is not a UUID: ' + repr(info['boot_id']))
```

### 3d. delete the dead `if False` line in `run_case`
**old**
```python
        taint_before_text, _, taint_before_err = self._cat(bdir, 'taint.before') if False else (None, None, None)
        before = self.console.run_shell('cat ' + DEFAULT_TAINT_PATH,
```
**new**
```python
        before = self.console.run_shell('cat ' + DEFAULT_TAINT_PATH,
```

### 3e. malformed data must become problems + still write `meta.json` (no auto‑rerun, record already saved)
**old**
```python
        if isinstance(record, dict):
            problems.extend(self.plan.validate_case(case, record, meta={
                'exit': framed_exit,
                'taint_before': taint_before,
                'taint_after': taint_after,
            }))
        else:
            problems.append('record is not a JSON object')

        metrics = None
        if isinstance(record, dict):
            metrics = self.plan.case_metrics(case, record)
            write_json(rdir / 'metrics.json', metrics)
```
**new**
```python
        if isinstance(record, dict):
            try:
                problems.extend(self.plan.validate_case(case, record, meta={
                    'exit': framed_exit,
                    'taint_before': taint_before,
                    'taint_after': taint_after,
                }))
            except Exception as exc:
                problems.append('validate_case raised: ' + repr(exc))
        else:
            problems.append('record is not a JSON object')

        metrics = None
        if isinstance(record, dict):
            try:
                metrics = self.plan.case_metrics(case, record)
                write_json(rdir / 'metrics.json', metrics)
            except Exception as exc:
                problems.append('case_metrics raised: ' + repr(exc))
```

**Verified, no change needed (item 3):**
- `mac_research2.SerialConsole.__init__(port, raw_log_path, events_log_path, serial_factory=None, baudrate=…)` and `start(timeout=10.0)` match the call `console_class(args.serial, out_dir/'console.raw.log', out_dir/'console.events.log')` + `console.start()`. All required methods (`start`,`close`,`run_shell`,`drain_input`,`disable_echo`) exist.
- `run_shell(..., cwd=bdir, exit_var='ec')` is real: `run_shell` seeds `ec` to a sentinel, the inner command sets `ec=$?` from the CLI, and the framed tail prints `${ec}` when it was set. `inner` ends with `cli …; ec=$?; echo $ec > exit.txt; …`, so `framed_exit == CLI exit`. Correct as written.
- `_wait_for_shell` only sends a read‑only `true` probe and re‑`disable_echo`; it never flashes and reasonably requires an already‑root shell. Correct as written.
- `record_raw` is written to `record.raw.json` before `json.loads` classification, and the run stops on failure. Correct as written.

---

## 4. `out/paper-route-review-20261004/proposals/tests/test_composition_plan.py`

### 4a. move `experiment` out of `p4m`
**old**
```python
    p4m = {
        'aggressor_gap_cycles': 0,
        'sequence': 4,
        'observer_status': plan.P4M_OBSERVER_STATUS_OK,
        'observation': observation,
        'window': window,
        'experiment': experiment,
    }
    return {
```
**new**
```python
    p4m = {
        'aggressor_gap_cycles': 0,
        'sequence': 4,
        'observer_status': plan.P4M_OBSERVER_STATUS_OK,
        'observation': observation,
        'window': window,
    }
    return {
```

### 4b. put `experiment` at `result` level
**old**
```python
        'result': {
            'abi_version': plan.COMPOSITION_ABI,
            'status': 0, 'restore_status': 0,
            'configured': 1, 'raw_captured': 1,
            'generation': generation,
            'restored_generation': generation + 1,
            'p4m': p4m,
            'raw': raw,
        },
```
**new**
```python
        'result': {
            'abi_version': plan.COMPOSITION_ABI,
            'status': 0, 'restore_status': 0,
            'configured': 1, 'raw_captured': 1,
            'generation': generation,
            'restored_generation': generation + 1,
            'experiment': experiment,
            'p4m': p4m,
            'raw': raw,
        },
```

### 4c. add contract + malformed‑data tests
**old**
```python
    def test_metrics_use_window_not_whole_round(self):
        record = base_record(self.aggr)
        metrics = plan.case_metrics(self.aggr, record)
        self.assertEqual(metrics['a_completed_jobs_window'], record['result']['p4m']['window'][3])
        self.assertEqual(metrics['v_workload_bytes'], self.aggr['transfers'] * plan.CHUNK_BYTES)
        self.assertEqual(metrics['cycle_ns'], 20)


if __name__ == '__main__':
    unittest.main()
```
**new**
```python
    def test_metrics_use_window_not_whole_round(self):
        record = base_record(self.aggr)
        metrics = plan.case_metrics(self.aggr, record)
        self.assertEqual(metrics['a_completed_jobs_window'], record['result']['p4m']['window'][3])
        self.assertEqual(metrics['v_workload_bytes'], self.aggr['transfers'] * plan.CHUNK_BYTES)
        self.assertEqual(metrics['cycle_ns'], 20)

    def test_metrics_read_the_frozen_locations(self):
        record = base_record(self.aggr)
        metrics = plan.case_metrics(self.aggr, record)
        self.assertEqual(metrics['v_window_cycles'], record['result']['p4m']['window'][1])
        self.assertEqual(metrics['v_actual_gap_cycles'],
                         record['result']['raw'][plan.RAW_V_GAP])

    def test_experiment_lives_at_result_level(self):
        record = base_record(self.solo)
        self.assertIn('experiment', record['result'])
        self.assertNotIn('experiment', record['result']['p4m'])
        self.assertNotIn('raw', record['result']['p4m'])
        self.assertEqual(plan.validate_case(self.solo, record, self.meta()), [])

    def test_malformed_result_returns_problems_not_crash(self):
        record = base_record(self.solo)
        record['result'] = ['not', 'an', 'object']
        self.assertTrue(plan.validate_case(self.solo, record, self.meta()))

    def test_malformed_p4m_returns_problems_not_crash(self):
        record = base_record(self.solo)
        record['result']['p4m'] = 'not-an-object'
        self.assertTrue(plan.validate_case(self.solo, record, self.meta()))

    def test_malformed_experiment_returns_problems_not_crash(self):
        record = base_record(self.solo)
        record['result']['experiment'] = None
        self.assertTrue(plan.validate_case(self.solo, record, self.meta()))

    def test_malformed_raw_returns_problems_not_crash(self):
        record = base_record(self.solo)
        record['result']['raw'] = {'not': 'a list'}
        self.assertTrue(plan.validate_case(self.solo, record, self.meta()))


if __name__ == '__main__':
    unittest.main()
```

The 172‑case plan, ordering/Latin rotation and all existing rules are untouched.

---

## 5. NEW FILE: `out/paper-route-review-20261004/proposals/tests/test_composition_cli_contract.py`

```python
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
UAPI_INCLUDE = REPO / 'linux' / 'include' / 'uapi'

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
@unittest.skipUnless(UAPI_INCLUDE.is_dir(), 'linux/include/uapi is missing')
class CompositionCliContractTests(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        cls._tmp = tempfile.TemporaryDirectory(prefix='idma-composition-cli-')
        cls.tmp = pathlib.Path(cls._tmp.name)
        mock_path = cls.tmp / 'mock_idma.c'
        mock_path.write_text(MOCK_SOURCE, encoding='utf-8')
        cls.binary = cls.tmp / 'idma_composition'
        cmd = [
            'gcc', '-Wall', '-Wextra', '-Werror', '-std=gnu99',
            '-I', str(UAPI_INCLUDE),
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
```

Mock fills only UAPI‑derived values that make `validate_case` pass (Solo and A8, all four modes, all three V‑gap values), asserts real `raw[0]` magic, protected DID 10, completion (`window[13]/[14]`, `observation[7]/[8]` = transfers), and full timing/field presence. `--help`/`--abi`, duplicate/unknown args, and ioctl failure are covered. **Not run.**

---

## Assumptions

- Proposal tree layout is `proposals/{bench/idma_composition.c, scripts/composition_plan.py, scripts/mac_composition.py, tests/*, linux/include/uapi/linux/idma.h}`, matching the supplied paths.
- Host is Linux with a system gcc and glibc; other UAPI headers (`<linux/ioctl.h>`, `<linux/types.h>`) resolve from the system include path while `linux/idma.h` comes from `-I …/linux/include/uapi`.
- `composition_plan.py` `case_metrics` metrics keys used by `mac_composition.py` (`v_window_cycles`, `a_completed_jobs_window`) are unchanged.
- The producer is expected to compile warning‑free with `-Wall -Wextra -Werror`; if the vendor gcc flags a warning this must be reported, not masked.

## Unresolved / for Codex

- The exact raw indices (`RAW_LIVE_CONFIG=2`, `RAW_WINDOW_CYCLES=8`, `RAW_ARM_CONFIG=10`, `RAW_V_GAP=24`, `RAW_FAST_N_DISABLED=33`, etc.) are taken from the current `composition_plan.py`; the mock uses the same indices. If those indices are wrong relative to the kernel, the schema test passes but is not a hardware‑truth test — that is out of scope here.
- `--wrap=ioctl`/`--wrap=open` behavior is platform/libc dependent; if a platform redirects `ioctl`→`__ioctl` or `open`→`open64`, wrapping may need adjustment. Wrapping both `open` and `open64` is already included.
- No verification was performed. Codex must run `gcc` build and `python -m unittest` for both test modules.

## Suggested acceptance checks

1. Compile the producer with the mock exactly as in the new test; confirm `gcc` exits 0 and the CLI prints one line that `json.loads` parses.
2. Run the new `tests/test_composition_cli_contract.py`; all sub‑tests pass, especially `test_solo_short`, `test_aggressor_a8_pn`, `test_all_mode_a8_vgap_combinations`, `test_ioctl_failure_still_emits_valid_json`, and the no‑open tests.
3. Run `tests/test_composition_plan.py`; the original 172‑count/order tests still pass and the new nesting/malformed‑data tests pass.
4. Confirm the C record now contains `result.experiment`, `result.p4m{aggressor_gap_cycles,sequence,observer_status,observation,window}`, `result.raw[128]`, and top‑level `semantics`, and that `json.loads` succeeds for both a successful and a failing run.
5. Confirm `mac_composition.py` no longer contains the `if False` line, rejects a non‑UUID `boot_id`, and still writes `meta.json` when `validate_case` is handed malformed data.