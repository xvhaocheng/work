#!/usr/bin/env python3
"""Finite parser checks based on actual-driver logs from explicitly mocked MMIO."""
import importlib.util
from pathlib import Path
import re
import unittest

HERE = Path(__file__).resolve().parent
spec = importlib.util.spec_from_file_location('e5_hist_records', HERE / 'hist_records.py')
hist = importlib.util.module_from_spec(spec)
spec.loader.exec_module(hist)
log = (HERE / 'driver-validation/runs/run02/fixture.log').read_text()
FIXTURE = log.split('BEGIN_VALID_V4_FIXTURE_LOG\n', 1)[1].split('END_VALID_V4_FIXTURE_LOG', 1)[0]


def v5_fixture():
    """Explicit protocol fixture; only INFO changes, stage buckets stay schema1."""
    return re.sub(r'idma_hist_info seq=(\d+) [^\n]+',
                  r'idma_hist_info seq=\1 valid=1 version=5 schema=2 bins=32 count_bits=32 '
                  r'time_bits=32 depth=64 overflow=0 raw=0x0040000020202002', FIXTURE)


def item_from_fixture():
    paths = {}
    for seq, role, channel, stage, raw in re.findall(r'idma_path_obs seq=(\d+) role=([va]) channel=([rw]) stage=(\d+) ([^\n]+)', FIXTURE):
        paths[role + channel + stage] = {k: hist.number(v) for k, v in hist.fields(raw).items()}
    time = re.search(r'idma_batch_obs seq=(\d+) part=time ([^\n]+)', FIXTURE)
    return {'sequence': int(time[1]), 'observer': {'time': {k: hist.number(v) for k, v in hist.fields(time[2]).items()}},
            'path': paths, 'boot_id': 'HOST-FIXTURE-NO-BOARD', 'condition': 'fixture_only'}


class HistTests(unittest.TestCase):
    def test_actual_driver_fixture_and_rows(self):
        item = item_from_fixture()
        parsed = hist.parse_lines(FIXTURE, item)
        rows = list(hist.bin_rows(item, parsed))
        self.assertEqual(len(rows), 512)
        self.assertEqual(sum(r['count'] for r in rows), 16)
        self.assertEqual(parsed['duplicate_equal_lines'], 0)
        self.assertTrue(all(r['condition'] == 'fixture_only' for r in rows))

    def test_cycle_bounds(self):
        self.assertEqual(hist.bucket_bounds(0), (0, 0))
        self.assertEqual(hist.bucket_bounds(1), (1, 1))
        self.assertEqual(hist.bucket_bounds(2), (2, 3))
        self.assertEqual(hist.bucket_bounds(31), (1 << 30, (1 << 32) - 1))

    def test_v5_explicit_info_and_unchanged_stage_schema(self):
        item = item_from_fixture()
        text = v5_fixture()
        v4 = hist.parse_lines(FIXTURE, item)
        v5 = hist.parse_lines(text, item)
        self.assertNotIn('depth', v4['info'])
        self.assertEqual(v5['info']['depth'], 64)
        self.assertEqual(v5['info']['schema'], 2)
        self.assertEqual(v5['info']['raw'], 0x0040000020202002)
        self.assertEqual(v4['stages'], v5['stages'])
        self.assertTrue(all(stage['schema'] == 1 for stage in v5['stages'].values()))
        self.assertEqual(list(hist.bin_rows(item, v4)), list(hist.bin_rows(item, v5)))
        full = '\nCASE_EXIT=0\n' + text + '\nTAINT_AFTER=0\n'
        self.assertEqual(hist.attach_from_output(full, item), v5)

    def test_v5_rejects_wrong_version_depth_format_and_overflow(self):
        cases = [
            ('unknown_version', 'version=5', 'version=6'),
            ('wrong_depth', 'depth=64', 'depth=4'),
            ('missing_depth', ' depth=64', ''),
            ('wrong_info_schema', 'version=5 schema=2', 'version=5 schema=1'),
            ('wrong_bins', 'schema=2 bins=32', 'schema=2 bins=16'),
            ('wrong_count_bits', 'count_bits=32', 'count_bits=16'),
            ('wrong_time_bits', 'time_bits=32', 'time_bits=64'),
            ('overflow_flag', 'overflow=0 raw=', 'overflow=1 raw='),
            ('raw_depth_mismatch', 'raw=0x0040000020202002', 'raw=0x0004000020202002'),
            ('raw_reserved_high', 'raw=0x0040000020202002', 'raw=0x0140000020202002'),
            ('raw_reserved_middle', 'raw=0x0040000020202002', 'raw=0x0040000120202002'),
            ('raw_overflow', 'raw=0x0040000020202002', 'raw=0x0040001020202002'),
            ('raw_schema_mismatch', 'raw=0x0040000020202002', 'raw=0x0040000020202001'),
            ('stage_schema_changed', 'stage=0 valid=1 schema=1', 'stage=0 valid=1 schema=2'),
        ]
        for name, old, new in cases:
            with self.subTest(name=name):
                text = v5_fixture().replace(old, new, 1)
                self.assertNotEqual(text, v5_fixture())
                with self.assertRaises(ValueError) as error:
                    hist.parse_lines(text, item_from_fixture())
                print(f'REJECTED {name}: {error.exception}')

    def test_v4_field_set_and_same_sequence_version_consistency(self):
        for name, text in (
                ('v4_extra_depth', FIXTURE.replace('version=4', 'depth=4 version=4')),
                ('v4_wrong_schema', FIXTURE.replace('version=4 schema=1', 'version=4 schema=2')),
                ('mixed_info_versions', FIXTURE + re.search(r'idma_hist_info[^\n]+', v5_fixture())[0] + '\n')):
            with self.subTest(name=name), self.assertRaises(ValueError) as error:
                hist.parse_lines(text, item_from_fixture())
            print(f'REJECTED {name}: {error.exception}')

    def test_missing_stage(self):
        text = re.sub(r'idma_path_hist[^\n]+\n', '', FIXTURE, count=1)
        with self.assertRaisesRegex(ValueError, '16 complete'):
            hist.parse_lines(text, item_from_fixture())

    def test_bad_count(self):
        text = FIXTURE.replace('counts=0,0,2,', 'counts=0,0,1,', 1)
        with self.assertRaisesRegex(ValueError, 'conservation'):
            hist.parse_lines(text, item_from_fixture())

    def test_bad_version_overflow_or_truncation(self):
        for text in (FIXTURE.replace('version=4', 'version=3'),
                     FIXTURE.replace('overflow=0 raw=', 'overflow=0x1 raw=', 1),
                     FIXTURE.replace('counts=0,0,2,', 'counts=0,2,', 1)):
            with self.subTest(text=text[:80]), self.assertRaises(ValueError):
                hist.parse_lines(text, item_from_fixture())

    def test_wrong_sequence(self):
        seq = item_from_fixture()['sequence']
        text = FIXTURE.replace(f'idma_path_hist seq={seq}', f'idma_path_hist seq={seq+1}', 1)
        with self.assertRaises(ValueError):
            hist.parse_lines(text, item_from_fixture())

    def test_duplicates_count_once_and_inconsistent_rejected(self):
        line = re.search(r'idma_path_hist[^\n]+', FIXTURE)[0]
        result = hist.parse_lines(FIXTURE + line + '\n', item_from_fixture())
        self.assertEqual(len(result['stages']), 16)
        self.assertEqual(result['duplicate_equal_lines'], 1)
        with self.assertRaises(ValueError):
            hist.parse_lines(FIXTURE + line.replace('full=2', 'full=3') + '\n', item_from_fixture())

    def test_final_validity_gates_earlier_hist_lines(self):
        for part in ('batch', 'PATH'):
            item = item_from_fixture()
            if part == 'batch': item['observer']['time']['valid'] = 0
            else: item['path']['aw3']['valid'] = 0
            with self.subTest(part=part), self.assertRaisesRegex(ValueError, 'invalid'):
                hist.parse_lines(FIXTURE, item)

    def test_time_bounds(self):
        for field, value in [('sum', 100), ('max', 4)]:
            item = item_from_fixture()
            item['path']['vr0'][field] = value
            with self.assertRaises(ValueError):
                hist.parse_lines(FIXTURE, item)

    def test_complete_console_dump(self):
        item = item_from_fixture()
        full = '\nCASE_EXIT=0\n' + FIXTURE + '\nTAINT_AFTER=0\n'
        self.assertEqual(len(hist.attach_from_output(full, item)['stages']), 16)
        for text in (full.replace('CASE_EXIT=0', 'CASE_EXIT=1'), full.replace('TAINT_AFTER=0', 'TAINT_AFTER=1'),
                     full.split('\nTAINT_AFTER=')[0]):
            with self.assertRaises(ValueError): hist.attach_from_output(text, item)


if __name__ == '__main__':
    unittest.main()
