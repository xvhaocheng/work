#!/usr/bin/env python3
"""Reuse the prior accepted private login helper; never log credential bytes."""
import sys,importlib.util
from pathlib import Path
sys.dont_write_bytecode=True
REFERENCE=Path('/home/xvhaocheng/work/iommu-platform/out/r107-execution-gate-20261012/route')
sys.path.insert(0,str(REFERENCE))
spec=importlib.util.spec_from_file_location('r110_private_board_login',REFERENCE/'login_current_board.py')
m=importlib.util.module_from_spec(spec);sys.modules[spec.name]=m;spec.loader.exec_module(m)
m.HERE=Path(__file__).resolve().parent
if __name__=='__main__':raise SystemExit(m.main())
