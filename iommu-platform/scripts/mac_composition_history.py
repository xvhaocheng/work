#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""mac_composition_history.py - history-isolation diagnostic runner (22 cases).

A thin wrapper around scripts/mac_composition.py.  It imports the real
runner from this directory and replaces ONLY the module returned by that
runner's load_plan_module() hook with a frozen 22-case diagnostic plan.
Everything else - main(), the UART/SSH transports, the safety gates, the
fail-stop, the plan/manifest/summary/record/metrics saving - stays the
original code path.

The 22 cases are a deterministic filter of the original 172-case plan:

  short  : mode P / PN with a_pages in {0, 8}                 ->  4
  explore: v_gap_cycles == 0 and
             (a_pages == 8, any of the 4 modes) or
             (a_pages == 1, mode P / PN)                      -> 18
                                                              total 22

Original execution order, ids and per-case parameters are preserved.  There is
no condition/selection CLI parameter: --plan always freezes the full 22-case
list.  A technical failure stops the run; there is no resume and no rerun.
This wrapper never reads, prints or logs a credential.
"""
from __future__ import annotations

import importlib.util
import pathlib
import sys
import types

HISTORY_PLAN_SCHEMA = 'idma-composition-plan-history-v1'
HISTORY_PLAN_GENERATION = 'history-isolation-diagnostic22'
HISTORY_PLAN_GENERATION_NOTE = 'history-isolation diagnostic22'

SOURCE_PLAN_SCHEMA = 'idma-composition-plan-v1'
SOURCE_PLAN_COUNT = 172

MODE_P = 2
MODE_PN = 3
HISTORY_SHORT_MODES = frozenset((MODE_P, MODE_PN))
HISTORY_SHORT_A_PAGES = frozenset((0, 8))
HISTORY_EXPLORE_ALL_MODE_A_PAGES = frozenset((8,))
HISTORY_EXPLORE_P_ONLY_A_PAGES = frozenset((1,))

EXPECTED_COUNT = 22
EXPECTED_PHASES = {'short': 4, 'noise': 0, 'explore': 18}


def _load_sibling(name):
    """Load a .py file that sits next to this wrapper, no package needed."""
    here = pathlib.Path(__file__).resolve().parent
    path = here / (name + '.py')
    if not path.is_file():
        raise SystemExit('cannot find ' + name + '.py next to this wrapper')
    spec = importlib.util.spec_from_file_location('history_wrapper_' + name, path)
    if spec is None or spec.loader is None:
        raise SystemExit('cannot load ' + str(path))
    module = importlib.util.module_from_spec(spec)
    sys.modules['history_wrapper_' + name] = module
    spec.loader.exec_module(module)
    return module


def select_source_cases(cases):
    """Deterministic filter of the original plan; original order is kept.

    short  : mode P / PN and a_pages in {0, 8}
    explore: v_gap_cycles == 0 and
             (a_pages == 8 any mode) or (a_pages == 1 mode P / PN)
    """
    selected = []
    for case in cases:
        phase = case['phase']
        mode = int(case['mode'])
        a_pages = int(case['a_pages'])
        gap = int(case['v_gap_cycles'])
        if phase == 'short':
            if mode in HISTORY_SHORT_MODES and a_pages in HISTORY_SHORT_A_PAGES:
                selected.append(case)
        elif phase == 'explore':
            if gap != 0:
                continue
            if a_pages in HISTORY_EXPLORE_ALL_MODE_A_PAGES:
                selected.append(case)
            elif a_pages in HISTORY_EXPLORE_P_ONLY_A_PAGES and mode in HISTORY_SHORT_MODES:
                selected.append(case)
    return selected


def build_history_plan(source_plan):
    cases = select_source_cases(source_plan.get('cases') or [])
    phases = {}
    for name in ('short', 'noise', 'explore'):
        phases[name] = sum(1 for case in cases if case['phase'] == name)
    return {
        'schema': HISTORY_PLAN_SCHEMA,
        'plan_generation': HISTORY_PLAN_GENERATION,
        'plan_generation_note': HISTORY_PLAN_GENERATION_NOTE,
        'source_plan_schema': source_plan.get('schema'),
        'source_plan_generation': source_plan.get('plan_generation'),
        'source_case_count': source_plan.get('count'),
        'auto_rerun': False,
        'resume': False,
        'calibration': source_plan.get('calibration', 'unverified'),
        'selection': (
            'short: mode P/PN a_pages in {0,8}; '
            'explore: v_gap 0 and (a_pages 8 any mode, a_pages 1 mode P/PN)'),
        'count': len(cases),
        'phases': phases,
        'cases': cases,
    }


def make_history_plan_module(real_plan):
    """Build a plan-module facade: same API, but build_plan() is the frozen 22."""
    source_plan = real_plan.build_plan()
    if (source_plan.get('schema') != SOURCE_PLAN_SCHEMA
            or source_plan.get('count') != SOURCE_PLAN_COUNT):
        raise SystemExit(
            'source plan is not the expected %s/%d: schema=%r count=%r'
            % (SOURCE_PLAN_SCHEMA, SOURCE_PLAN_COUNT,
               source_plan.get('schema'), source_plan.get('count')))
    history_plan = build_history_plan(source_plan)
    if history_plan['count'] != EXPECTED_COUNT or history_plan['phases'] != EXPECTED_PHASES:
        raise SystemExit(
            'history selection drifted from the frozen 22: count=%r phases=%r'
            % (history_plan['count'], history_plan['phases']))
    module = types.ModuleType('composition_plan_history')
    for name in dir(real_plan):
        if name.startswith('_'):
            continue
        setattr(module, name, getattr(real_plan, name))
    # Only this is replaced; case_argv/validate_case/case_metrics/
    # cases_through_phase and all constants stay the original ones.
    module.build_plan = lambda: history_plan
    return module


def main(argv=None):
    mac_module = _load_sibling('mac_composition')
    real_plan = _load_sibling('composition_plan')
    history_plan_module = make_history_plan_module(real_plan)
    # mac_composition.main() looks up load_plan_module() at call time, so this
    # single rebind is enough; no other runner code is touched.
    mac_module.load_plan_module = lambda: history_plan_module
    return mac_module.main(sys.argv[1:] if argv is None else argv)


if __name__ == '__main__':
    sys.exit(main())
