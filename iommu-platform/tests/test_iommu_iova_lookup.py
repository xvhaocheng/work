#!/usr/bin/env python3
"""Exercise production page-table walk/lookup with Sv39 host page tables.

IOMMU_PGTABLE_SOURCE can select the saved pre-fix C file for regression evidence.
This tests the software lookup, not an FPGA walker or IOTLB.
"""
import os
from pathlib import Path
import re
import subprocess
import tempfile
import unittest

ROOT = Path(__file__).resolve().parents[1]


class IovaLookupTest(unittest.TestCase):
    def test_production_lookup(self):
        source = Path(os.environ.get('IOMMU_PGTABLE_SOURCE',
                      ROOT / 'linux/drivers/iommu/riscv/io_pgtable.c')).read_text()
        functions = []
        for name in ('riscv_iommu_pt_walk_fetch', 'riscv_iommu_iova_to_phys'):
            matches = re.findall(rf'^static [^\n]*\b{name}\(.*?^\}}', source,
                                 re.MULTILINE | re.DOTALL)
            self.assertEqual(len(matches), 1, name)
            functions.append(matches[0])
        harness = (ROOT / 'tests/iommu_iova_lookup_harness.c').read_text()
        harness = harness.replace('/* PRODUCTION_FUNCTIONS */', '\n'.join(functions))
        with tempfile.TemporaryDirectory() as temporary:
            path = Path(temporary)
            (path/'lookup.c').write_text(harness)
            subprocess.run([os.environ.get('HOSTCC', 'cc'), '-std=gnu11', '-O2',
                            '-Wall', '-Wextra', '-Werror', str(path/'lookup.c'),
                            '-o', str(path/'lookup')], check=True)
            result = subprocess.run([str(path/'lookup')], text=True,
                                    capture_output=True, timeout=5)
            self.assertEqual(result.returncode, 0, result.stdout + result.stderr)
            print(result.stdout.strip())


if __name__ == '__main__':
    unittest.main()
