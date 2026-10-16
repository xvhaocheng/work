#!/usr/bin/env python3
'''Host-only checks for the explicit resume35 continuation mode.

No board, device, DMA, HPM or FPGA access is performed. The tests extract the
real sequence and preflight shell functions from tests/board-p4m-formal60.sh and
run them under stubbed commands. They never write to /tmp/p4m-formal60-215406 or
any other board path.
'''

from pathlib import Path
import subprocess
import tempfile
import unittest


ROOT = Path(__file__).resolve().parents[1]
SCRIPT = ROOT / 'tests' / 'board-p4m-formal60.sh'
EXACT_KERNEL = '6.5.0-rc2-00089-gefd9461d6459'
ANCHOR_A_BEGIN_NS = '3600085892000'

EXPECTED_RESUME_SEQUENCE = [
    ('formal-b09-g0', 'P8g0'),
    ('formal-b09-g1000', 'P8g1000'),
    ('formal-b10-g0', 'P8g0'),
    ('formal-b10-g2000', 'P8g2000'),
    ('formal-b10-g1000', 'P8g1000'),
    ('formal-b11-g2000', 'P8g2000'),
    ('formal-b11-g1000', 'P8g1000'),
    ('formal-b11-g0', 'P8g0'),
    ('formal-b12-g1000', 'P8g1000'),
    ('formal-b12-g0', 'P8g0'),
    ('formal-b12-g2000', 'P8g2000'),
    ('formal-b13-g0', 'P8g0'),
    ('formal-b13-g1000', 'P8g1000'),
    ('formal-b13-g2000', 'P8g2000'),
    ('formal-b14-g1000', 'P8g1000'),
    ('formal-b14-g2000', 'P8g2000'),
    ('formal-b14-g0', 'P8g0'),
    ('formal-b15-g2000', 'P8g2000'),
    ('formal-b15-g0', 'P8g0'),
    ('formal-b15-g1000', 'P8g1000'),
    ('formal-b16-g0', 'P8g0'),
    ('formal-b16-g2000', 'P8g2000'),
    ('formal-b16-g1000', 'P8g1000'),
    ('formal-b17-g2000', 'P8g2000'),
    ('formal-b17-g1000', 'P8g1000'),
    ('formal-b17-g0', 'P8g0'),
    ('formal-b18-g1000', 'P8g1000'),
    ('formal-b18-g0', 'P8g0'),
    ('formal-b18-g2000', 'P8g2000'),
    ('formal-b19-g0', 'P8g0'),
    ('formal-b19-g1000', 'P8g1000'),
    ('formal-b19-g2000', 'P8g2000'),
    ('formal-b20-g2000', 'P8g2000'),
    ('formal-b20-g1000', 'P8g1000'),
    ('formal-b20-g0', 'P8g0'),
]


def _extract_shell_function(text, name):
    lines = text.splitlines()
    start = None
    for index, line in enumerate(lines):
        if line == name + '() {':
            start = index
            break
    if start is None:
        raise AssertionError('production function not found: ' + name)
    end = start
    while end < len(lines) and lines[end] != '}':
        end += 1
    if end >= len(lines):
        raise AssertionError('production function unterminated: ' + name)
    return '\n'.join(lines[start:end + 1]) + '\n'


class Resume35SequenceTest(unittest.TestCase):
    def setUp(self):
        self._temporary = tempfile.TemporaryDirectory(prefix='idma-p4m-resume35-')
        self.temp = Path(self._temporary.name)
        self.call_log = self.temp / 'calls.log'

    def tearDown(self):
        self._temporary.cleanup()

    def _harness(self, fail_at=0):
        text = SCRIPT.read_text()
        parts = [
            '#!/bin/sh\n',
            'LONG_TRANSFERS=4096\n',
            'LONG_WARMUP=256\n',
            'CALL_LOG="%s"\n' % self.call_log,
            'count=0\n',
            'run_case() {\n',
            '    count=$((count + 1))\n',
            "    printf '%s|%s|%s|%s\\n' \"$1\" \"$2\" \"$3\" \"$4\" >> \"$CALL_LOG\"\n",
        ]
        if fail_at:
            parts.append('    if [ "$count" = "%d" ]; then return 1; fi\n' % fail_at)
        parts.extend([
            '    return 0\n',
            '}\n',
        ])
        for name in ('run_formal_triplet', 'run_formal_resume'):
            parts.append(_extract_shell_function(text, name))
        parts.append('run_formal_resume\n')
        parts.append('echo rc=$?\n')
        harness = self.temp / 'sequence-harness.sh'
        harness.write_text(''.join(parts))
        return harness

    def _run(self, fail_at=0):
        harness = self._harness(fail_at=fail_at)
        result = subprocess.run(['sh', str(harness)], text=True,
                                capture_output=True, check=False)
        lines = []
        if self.call_log.exists():
            lines = [line for line in self.call_log.read_text().splitlines() if line]
        return result, lines

    def test_shell_syntax(self):
        result = subprocess.run(['sh', '-n', str(SCRIPT)], text=True,
                                capture_output=True, check=False)
        self.assertEqual(result.returncode, 0, result.stderr)

    def test_usage_documents_resume35(self):
        result = subprocess.run(['sh', str(SCRIPT)], text=True,
                                capture_output=True, check=False)
        self.assertEqual(result.returncode, 2, result.stderr)
        self.assertIn('resume35', result.stderr)
        self.assertIn('/tmp/p4m-formal60-resume-01', result.stderr)

    def test_resume35_rejects_existing_output_directory(self):
        out = self.temp / 'existing'
        out.mkdir()
        result = subprocess.run(['sh', str(SCRIPT), 'resume35', str(out)],
                                text=True, capture_output=True, check=False)
        self.assertEqual(result.returncode, 2, result.stderr)
        self.assertIn('already exists', result.stderr)

    def test_resume_sequence_is_exact_remaining_35_cases(self):
        result, lines = self._run()
        self.assertIn('rc=0', result.stdout)
        expected = ['%s|%s|4096|256' % item for item in EXPECTED_RESUME_SEQUENCE]
        self.assertEqual(lines, expected)

    def test_resume_sequence_never_replays_cases_1_to_25_or_earlier_blocks(self):
        result, lines = self._run()
        self.assertIn('rc=0', result.stdout)
        ids = [line.split('|', 1)[0] for line in lines]
        self.assertNotIn('formal-b09-g2000', ids)
        for case_id in ids:
            self.assertFalse(case_id.startswith('formal-b01-'))
            self.assertFalse(case_id.startswith('formal-b02-'))
            self.assertFalse(case_id.startswith('formal-b03-'))
            self.assertFalse(case_id.startswith('formal-b04-'))
            self.assertFalse(case_id.startswith('formal-b05-'))
            self.assertFalse(case_id.startswith('formal-b06-'))
            self.assertFalse(case_id.startswith('formal-b07-'))
            self.assertFalse(case_id.startswith('formal-b08-'))
            self.assertFalse(case_id.startswith('start-'))
        self.assertEqual(len(ids), 35)

    def test_resume_sequence_fail_stop_on_first_and_second_case(self):
        for fail_at in (1, 2):
            with self.subTest(fail_at=fail_at):
                if self.call_log.exists():
                    self.call_log.unlink()
                result, lines = self._run(fail_at=fail_at)
                self.assertIn('rc=1', result.stdout)
                self.assertEqual(len(lines), fail_at)

    def test_no_short_or_compare_warmup_calls(self):
        result, lines = self._run()
        self.assertIn('rc=0', result.stdout)
        for line in lines:
            self.assertTrue(line.endswith('|4096|256'), line)

    def test_source_has_only_explicit_resume35_mode(self):
        text = SCRIPT.read_text()
        self.assertIn('resume35)', text)
        self.assertIn('run_formal_resume', text)
        self.assertNotIn('--skip', text)
        self.assertNotIn('--resume', text)
        self.assertNotIn('--start-at', text)


class Resume35GuardTest(unittest.TestCase):
    GUARD_FUNCTIONS = (
        'startup_max_seq',
        'dmget',
        'int_of',
        'extract_resume_seq37',
        'resume_check_seq37_record',
        'resume_preflight',
    )

    def setUp(self):
        self._temporary = tempfile.TemporaryDirectory(prefix='idma-p4m-resume35-guard-')
        self.temp = Path(self._temporary.name)
        self.out = self.temp / 'out'
        self.out.mkdir()
        self.current_dmesg = self.temp / 'current.dmesg'
        self.old_dir = self.temp / 'old-run'
        self.old_dir.mkdir()
        (self.old_dir / 'params.txt').write_text('kernel_release=%s\n' % EXACT_KERNEL)
        (self.old_dir / 'formal-b09-g2000.json').write_text('{}\n')
        self._write_seq37(self.current_dmesg, ANCHOR_A_BEGIN_NS, seq=37)
        self._write_seq37(self.old_dir / 'formal-b09-g2000.dmesg.raw',
                          ANCHOR_A_BEGIN_NS, seq=37)

    def tearDown(self):
        self._temporary.cleanup()

    def _write_seq37(self, path, a_begin_ns, seq=37):
        path.write_text(
            '[ 1.000000] idma_batch_start version=1 seq=%d method=irq_pair'
            ' v_did=10 a_did=11 attempted=1 has_a=1 a_triggered=1 v_triggered=1'
            ' a_id=7 v_id=9 a_begin_ns=%s a_end_ns=%s v_begin_ns=%s v_end_ns=%s'
            ' irq_span_ns=100 a_to_v_ns=50 error=0\n'
            '[ 1.000001] idma_batch_obs seq=%d part=time valid=1 status=0x4'
            ' v_did=10 a_did=11 issued=4096 done=4096 batch_polled=1\n'
            % (seq, a_begin_ns, int(a_begin_ns) + 1, int(a_begin_ns) + 2,
               int(a_begin_ns) + 3, seq))

    def _harness(self, old_dir=None, kernel_release=EXACT_KERNEL,
                 current_dmesg=None):
        text = SCRIPT.read_text()
        old = old_dir if old_dir is not None else self.old_dir
        current = current_dmesg if current_dmesg is not None else self.current_dmesg
        parts = [
            '#!/bin/sh\n',
            'set -u\n',
            'OUT="%s"\n' % self.out,
            'RESUME_ORIGINAL="%s"\n' % old,
            'RESUME_ORIGINAL_ID=p4m-formal60-215406\n',
            'RESUME_ORIGINAL_CASE=formal-b09-g2000\n',
            'RESUME_ANCHOR_SEQ=37\n',
            'RESUME_ANCHOR_A_BEGIN_NS=%s\n' % ANCHOR_A_BEGIN_NS,
            'KERNEL_RESUME_MATCH=00089\n',
            'KERNEL_RESUME_EXACT=%s\n' % EXACT_KERNEL,
            'LONG_TRANSFERS=4096\n',
            'A_DID=11\n',
            'kernel_release=%s\n' % kernel_release,
            'RESUME_TEST_DMESG="%s"\n' % current,
            'dmesg() { cat "$RESUME_TEST_DMESG"; }\n',
            'verify_startup() { return 0; }\n',
            'verify_batch_time() { return 0; }\n',
            'check_case_json() { return 0; }\n',
        ]
        for name in self.GUARD_FUNCTIONS:
            parts.append(_extract_shell_function(text, name))
        parts.append('resume_preflight\n')
        parts.append('echo preflight_rc=$?\n')
        harness = self.temp / 'guard-harness.sh'
        harness.write_text(''.join(parts))
        return harness

    def _run(self, **kwargs):
        harness = self._harness(**kwargs)
        result = subprocess.run(['sh', str(harness)], text=True,
                                capture_output=True, check=False)
        return result

    def test_normal_boundary_gate_passes_without_any_dma_command(self):
        result = self._run()
        self.assertEqual(result.returncode, 0, result.stderr)
        self.assertIn('preflight_rc=0', result.stdout)
        self.assertTrue((self.out / 'resume35-preflight.txt').is_file())

    def test_seq_above_37_fails_closed(self):
        self._write_seq37(self.current_dmesg, ANCHOR_A_BEGIN_NS, seq=38)
        result = self._run()
        self.assertIn('preflight_rc=5', result.stdout)
        self.assertIn('startup_max_seq', result.stderr)

    def test_missing_original_directory_fails_closed(self):
        result = self._run(old_dir=self.temp / 'does-not-exist')
        self.assertIn('preflight_rc=5', result.stdout)
        self.assertIn('missing_original_dir', result.stderr)

    def test_wrong_kernel_fails_closed(self):
        result = self._run(kernel_release='6.5.0-rc2-00088-g9f8da9e7a6a0')
        self.assertIn('preflight_rc=5', result.stdout)
        self.assertIn('kernel_exact', result.stderr)

    def test_wrong_anchor_fails_closed(self):
        self._write_seq37(self.current_dmesg, '1', seq=37)
        result = self._run()
        self.assertIn('preflight_rc=5', result.stdout)
        self.assertIn('anchor_a_begin_ns', result.stderr)

    def test_guard_does_not_touch_real_board_path(self):
        harness = self._harness()
        harness_text = harness.read_text()
        self.assertNotIn('idma_experiment', harness_text)
        self.assertNotIn('platform-timeout', harness_text)
        self.assertNotIn('/tmp/p4m-formal60-215406', harness_text)


if __name__ == '__main__':
    unittest.main()
