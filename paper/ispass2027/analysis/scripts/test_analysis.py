"""Acceptance checks for evidence boundaries, not plot implementation mirrors."""
import csv
import hashlib
import json
import tempfile
import unittest
from pathlib import Path

from data_adapter import (load_experiment_table, reject_duplicate_ids,
                          require_one_base, sample_values)
from statistics_common import bootstrap_median_ci, quantile, relative_slowdown, summarize


class StatisticsAcceptance(unittest.TestCase):
    def test_known_quartiles(self):
        summary=summarize([1,2,3,4])
        self.assertEqual((summary['n'],summary['median'],summary['q1'],summary['q3'],summary['iqr']),(4,2.5,1.75,3.25,1.5))
        self.assertEqual(bootstrap_median_ci([1,2,3,4]),bootstrap_median_ci([1,2,3,4]))
        self.assertEqual((summary['ci_low'],summary['ci_high']),(1,4))

    def test_fixed_boot_strata(self):
        # Each boot is internally constant; fixed-stratum bootstrap cannot
        # pretend it observed additional boots or change their sample weights.
        low,high=bootstrap_median_ci([1,1,100,100],strata=['a','a','b','b'])
        self.assertEqual((low,high),(50.5,50.5))

    def test_ratio_definition(self):
        self.assertEqual(relative_slowdown([2,4],[1,2])['value'],1)
        with self.assertRaises(ValueError):relative_slowdown([1,2],[1],paired=True)
        with self.assertRaises(ValueError):relative_slowdown([1],[0])

    def test_missing_is_not_zero(self):
        result=summarize([])
        self.assertEqual(result['n'],0)
        self.assertIsNone(result['median'])
        self.assertEqual(bootstrap_median_ci([1]),(None,None))
        self.assertIsNone(relative_slowdown([],[])['value'])


class EvidenceAcceptance(unittest.TestCase):
    def test_mixed_base_rejected(self):
        with self.assertRaisesRegex(ValueError,'mixed'):
            require_one_base([{'base_id':'a'},{'base_id':'b'}],context='paired figure')

    def test_duplicate_ids_rejected(self):
        with self.assertRaisesRegex(ValueError,'duplicate'):
            reject_duplicate_ids([{'row_id':'same'},{'row_id':'same'}])

    def test_scalar_summary_not_sample(self):
        with self.assertRaisesRegex(ValueError,'summary scalar'):
            sample_values([dict(base_id='a',statistic='median',n='10',value='7',value_status='observed')])

    def test_pending_and_hash_mismatch_rejected(self):
        with tempfile.TemporaryDirectory() as directory:
            p=Path(directory)/'results.csv';p.write_text('row_id,value\nx,1\n')
            rows,_,decision=load_experiment_table(p)
            self.assertEqual(rows,[]);self.assertEqual(decision['status'],'not_imported')
            receipt=dict(status='passed',complete=True,results_sha256='wrong',base_id='fixed',source_manifest='source.json',run_manifest='run.json')
            fields=['row_id','experiment','condition','metric','statistic','value','unit','n','raw_path','source_locator','base_id','evidence_level','boot_id','value_status','bitstream_id','kernel_id','identity_status','sample_set']
            with p.open('w',newline='') as f:
                writer=csv.DictWriter(f,fieldnames=fields);writer.writeheader();writer.writerow(dict(zip(fields,['x','e','mode=A1','elapsed','sample','1','cycles','1','raw.json','/0','fixed','fpga_board','boot1','observed','bit1','kernel1','recorded','formal'])))
            p.with_name('analysis-import.json').write_text(json.dumps(receipt))
            with self.assertRaisesRegex(ValueError,'hash'):load_experiment_table(p)
            receipt['results_sha256']=hashlib.sha256(p.read_bytes()).hexdigest()
            p.with_name('analysis-import.json').write_text(json.dumps(receipt))
            rows,_,decision=load_experiment_table(p)
            self.assertEqual((len(rows),decision['status']),(1,'imported'))


if __name__=='__main__':unittest.main()
