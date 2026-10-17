#!/usr/bin/env python3
"""E6 task-local entry for the common pure, raw-bound paper converter."""
import importlib.util
import sys

sys.dont_write_bytecode = True
from pathlib import Path

path = Path(__file__).resolve().parent.parent/'E2-device-scaling/export_paper.py'
spec = importlib.util.spec_from_file_location('e2_e6_paper_export', path)
converter = importlib.util.module_from_spec(spec)
spec.loader.exec_module(converter)

if __name__ == '__main__':
    converter.main('E6')
