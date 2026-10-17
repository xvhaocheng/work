#!/usr/bin/env python3
"""Explicit internal fixtures: no generated result is a board measurement."""
import copy
import csv
import hashlib
import json
from pathlib import Path
import re
import tempfile
from types import SimpleNamespace
import unittest

import analyze_hist as analysis
from test_hist_records import FIXTURE, item_from_fixture

HERE = Path(__file__).resolve().parent
KERNEL = 'HOST-FIXTURE-NO-KERNEL'


def fixture():
    item = item_from_fixture()
    item['fixture_only'] = True
    item['mapping_page_bytes'] = 4096
    item['case'] = item['block'] = 1
    item['record'] = {'schema': 'idma.pages.run.v1', 'success': True, 'errno': 0,
                      'input': {'mapping_page_bytes': 4096}, 'result': {'status': 0}}
    item['observer'] = {}
    for seq, part, fields in re.findall(r'idma_batch_obs seq=(\d+) part=(\w+) ([^\n]+)', FIXTURE):
        item['observer'][part] = analysis.numbers(fields)
    item['histogram'] = analysis.hist.parse_lines(FIXTURE, item)
    text = f'\nKERNEL={KERNEL}\nBOOT={item["boot_id"]}\nTAINT=0\nPRE_SEQ={item["sequence"]-1}\n'
    text += json.dumps(item['record']) + '\nCASE_EXIT=0\n' + FIXTURE + '\nTAINT_AFTER=0\n'
    return {'exit_code': 0, 'stdout': text, 'fixture_only': True}, item


class AnalysisTests(unittest.TestCase):
    def test_raw_rebind_and_fixture_publication_gate(self):
        raw, item = fixture()
        with self.assertRaisesRegex(ValueError, 'fixture'):
            analysis.rebind_trial(raw, item, KERNEL)
        rebound = analysis.rebind_trial(raw, item, KERNEL, _internal_fixture=True)
        self.assertEqual(rebound['histogram'], item['histogram'])
        self.assertTrue(rebound['fixture_only'])

    def test_raw_or_cache_tampering_rejected(self):
        for mode in ('CLI', 'PATH', 'bins', 'exit', 'kernel', 'watermark', 'final_invalid'):
            raw, item = fixture()
            if mode == 'CLI': item['record']['result']['status'] = 1
            if mode == 'PATH': item['path']['vr0']['sum'] += 1
            if mode == 'bins': item['histogram']['stages']['vr0']['counts'][2] = 1
            if mode == 'exit': raw['exit_code'] = 1
            if mode == 'kernel': raw['stdout'] = raw['stdout'].replace(KERNEL, 'wrong')
            if mode == 'watermark': raw['stdout'] = raw['stdout'].replace('PRE_SEQ=3', 'PRE_SEQ=2')
            if mode == 'final_invalid': raw['stdout'] = raw['stdout'].replace('part=time valid=1', 'part=time valid=0')
            with self.subTest(mode=mode), self.assertRaises(ValueError):
                analysis.rebind_trial(raw, item, KERNEL, _internal_fixture=True)

    def test_nearest_rank_uses_original_intervals(self):
        counts = [0] * 32
        counts[0], counts[1], counts[2], counts[31] = 1, 49, 45, 5
        self.assertEqual(analysis.interval(counts, 50), {'rank': 50, 'bin': 1, 'lo_cycles': 1, 'hi_cycles': 1})
        self.assertEqual(analysis.interval(counts, 95), {'rank': 95, 'bin': 2, 'lo_cycles': 2, 'hi_cycles': 3})
        self.assertEqual(analysis.interval(counts, 99), {'rank': 99, 'bin': 31, 'lo_cycles': 1 << 30, 'hi_cycles': (1 << 32) - 1})

    def test_empty_full_exact_mean_and_carry(self):
        _, item = fixture()
        item['path']['ar0'].update(carry=1, start=1, end=2, entered=2)
        rows = analysis.trial_intervals(item, item['histogram'], {'mapping_page_bytes': 4096}, _internal_fixture=True)
        victim = next(r for r in rows if (r['role'], r['channel'], r['stage']) == ('v', 'r', 0))
        aggressor = next(r for r in rows if (r['role'], r['channel'], r['stage']) == ('a', 'r', 0))
        self.assertEqual((victim['mean_numerator'], victim['mean_denominator'], victim['mean_cycles'], victim['max_cycles']), (4, 2, 2, 2))
        self.assertEqual((aggressor['n_valid'], aggressor['full'], aggressor['carry'], aggressor['opening'], aggressor['closing']), (0, 0, 1, 1, 2))
        self.assertIsNone(aggressor['p99_lo_cycles'])
        self.assertIsNone(aggressor['mean_cycles'])
        self.assertEqual(aggressor['max_cycles'], 0)

    def test_equal_trial_type7_endpoints_and_n_not_request_count(self):
        _, item = fixture()
        rows = analysis.trial_intervals(item, item['histogram'], {'mapping_page_bytes': 4096}, _internal_fixture=True)
        selected = [r for r in rows if r['channel'] == 'r' and r['stage'] == 0]
        second = copy.deepcopy(selected)
        for row in second:
            row['sequence'] += 1
            if row['role'] == 'v':
                for p in (50, 95, 99): row[f'p{p}_lo_cycles'], row[f'p{p}_hi_cycles'] = 4, 7
                row.update(sum_cycles=10, mean_numerator=10, mean_cycles=5, max_cycles=5)
        groups = analysis.describe_trials(selected + second, ['mapping_page_bytes'])
        victim = next(g for g in groups if g['group']['role'] == 'v')
        aggressor = next(g for g in groups if g['group']['role'] == 'a')
        self.assertEqual((victim['n_trials'], victim['n_full_trials'], victim['total_full']), (2, 2, 4))
        self.assertEqual(victim['percentiles']['99']['lo']['median_type7'], 3)
        self.assertEqual(victim['percentiles']['99']['hi']['q1_type7'], 4)
        self.assertEqual((aggressor['n_trials'], aggressor['n_full_trials'], aggressor['total_full']), (2, 0, 0))
        self.assertEqual(aggressor['percentiles']['99']['lo']['n'], 0)
        with self.assertRaises(ValueError): analysis.describe_trials(selected + selected, ['mapping_page_bytes'])
        with self.assertRaises(ValueError): analysis.describe_trials(selected, [])

    def test_internal_packet_labels_fixture_and_public_default_rejects(self):
        with tempfile.TemporaryDirectory(dir=HERE, prefix='.analysis-fixture-') as tmp:
            root = Path(tmp)
            raw, item = fixture()
            item['raw_command_path'] = 'command.json'
            load = {'status': 'boot_identity_verified', 'kernel_release': KERNEL,
                    'boot_id': item['boot_id'], 'bit_sha256': 'a' * 64, 'fixture_only': True}
            load_bytes = (json.dumps(load) + '\n').encode()
            (root / 'load.json').write_bytes(load_bytes)
            manifest = {'status': 'passed', 'recorded_cases': 1, 'planned_cases': [[1, 'fixture_only']],
                        'kernel_release': KERNEL, 'boot_id': item['boot_id'], 'bit_sha256': 'a' * 64,
                        'load_manifest': 'load.json', 'load_manifest_sha256': hashlib.sha256(load_bytes).hexdigest(),
                        'fixture_only': True}
            for name, value in [('manifest.json', manifest), ('records.json', [item]), ('command.json', raw)]:
                (root / name).write_text(json.dumps(value) + '\n')
            args = SimpleNamespace(records=root / 'records.json', manifest=root / 'manifest.json',
                                   source_root=root, output=root / 'output', group_by=['mapping_page_bytes'], allow_partial=False)
            with self.assertRaisesRegex(ValueError, 'fixture'): analysis.run_packet(args)
            self.assertFalse(args.output.exists())
            analysis.run_packet(args, _internal_fixture=True)
            report = json.loads((args.output / 'summary.json').read_text())
            self.assertTrue(report['fixture_only'])
            self.assertEqual(report['n_trials'], 1)
            with (args.output / 'trial-stage-intervals.csv').open() as stream:
                self.assertEqual(len(list(csv.DictReader(stream))), 16)
            with (args.output / 'histogram-bins.csv').open() as stream:
                self.assertEqual(len(list(csv.DictReader(stream))), 512)
            with self.assertRaises(ValueError): analysis.run_packet(args, _internal_fixture=True)


if __name__ == '__main__':
    unittest.main()
