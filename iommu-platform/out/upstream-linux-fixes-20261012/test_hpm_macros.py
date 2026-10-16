#!/usr/bin/env python3
"""Compile actual HPM macros from a selected header; no hardware claim."""
import os
from pathlib import Path
import re
import subprocess
import sys
import tempfile

header = Path(sys.argv[1]).read_text()
names = ('RISCV_IOMMU_REG_IOHPMCTR', 'RISCV_IOMMU_REG_IOHPMEVT')
defines = []
checks = []
for name in names:
    for symbol in (name + '_BASE', name):
        matches = re.findall(r'^#define\s+' + symbol + r'(?:\(_n\))?\s+[^\n]+',
                             header, re.MULTILINE)
        assert len(matches) == 1, symbol
        defines.append(matches[0])
    checks += [f'cases++; failures += {name}(lane * 3 + 1) != '
               f'{name}_BASE + ((lane * 3 + 1) * 8);',
               f'cases++; failures += {name}(lane) != {name}_BASE + lane * 8;']
source = '\n'.join(defines) + '\n#include <stdio.h>\nint main(void) {\n'
source += 'int cases = 0, failures = 0;\nfor (int lane = 0; lane < 8; lane++) {\n'
source += '\n'.join(checks) + '\n}\n'
source += 'printf("%d macro cases, %d failures\\n", cases, failures);\n'
source += 'return failures != 0;\n}\n'
with tempfile.TemporaryDirectory() as temporary:
    path = Path(temporary)
    (path / 'test.c').write_text(source)
    subprocess.run([os.environ.get('HOSTCC', 'cc'), '-std=gnu11', '-O2',
                    '-Wall', '-Wextra', '-Werror', str(path / 'test.c'),
                    '-o', str(path / 'test')], check=True)
    sys.exit(subprocess.run([str(path / 'test')], timeout=5).returncode)
