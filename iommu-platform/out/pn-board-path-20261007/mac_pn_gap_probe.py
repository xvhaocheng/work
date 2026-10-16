#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""mac_pn_gap_probe.py - P/PN Vgap supply probe (explore-only, 18 cases).

Thin wrapper around scripts/mac_composition.py living in the same directory.
It imports the real runner from this directory and replaces ONLY the module
returned by that runner's load_plan_module() hook with a frozen 18-case plan.
Everything else - main(), the UART/SSH transports, the safety gates, the
fail-stop and the plan/manifest/summary/record/metrics saving - stays the
original code path.

Scope (no SCI conclusion)
    This run perturbs only the V-side supply of the P/PN shadow path: the
    original explore cases keep their mode / a_pages / v_gap_cycles but the
    victim transfer count drops from 4096 to 64.  It is NOT a strict
    DDR-priority counterfactual and nothing here establishes a mechanism,
    a benefit or any other scientific claim.

Selection (deterministic filter of the original 172-case plan, order kept):
    phase == 'explore' and a_pages == 8 and mode in (P=2, PN=3)
    and v_gap_cycles in (0, 1000, 4000) and block in (1, 2, 3)      -> 18

Per selected case (shallow copy): transfers 4096 -> 64, warmup 256,
timeout_ms copied unchanged.  ids get the 'pn-gap64-' prefix so records from
the old 4096-transfer runs stay distinguishable.

There is no selection/condition CLI parameter: a run always freezes this full
18-case list, and --through-phase is left exactly as the runner defines it.
A technical failure stops the run; there is no resume and no rerun.  This
wrapper never reads, prints or logs a credential.
"""
from __future__ import annotations

import importlib.util
import pathlib
import sys
import types

PN_PLAN_SCHEMA = 'idma-composition-plan-pn-gap64-v1'
PN_PLAN_GENERATION = 'pn-gap64-20261007'
PN_PLAN_GENERATION_NOTE = 'pn-gap64 Vgap supply probe'
PN_ID_PREFIX = 'pn-gap64-'

SOURCE_PLAN_SCHEMA = 'idma-composition-plan-v1'
SOURCE_PLAN_COUNT = 172

SOURCE_PHASES = ('short', 'noise', 'explore')

MODE_P = 2
MODE_PN = 3
PN_MODES = frozenset((MODE_P, MODE_PN))

TARGET_PHASE = 'explore'
TARGET_A_PAGES = 8
TARGET_GAPS = frozenset((0, 1000, 4000))
TARGET_BLOCKS = frozenset((1, 2, 3))

PN_TRANSFERS = 64
PN_WARMUP = 256

EXPECTED_COUNT = 18
EXPECTED_PHASES = {'short': 0, 'noise': 0, 'explore': 18}

PN_SELECTION_NOTE = (
    "explore only: a_pages 8, mode P/PN, v_gap in {0,1000,4000}, "
    "block in {1,2,3}; per case transfers 4096->64, warmup 256, "
    "timeout_ms unchanged; id prefix 'pn-gap64-'")
PN_SCOPE_NOTE = (
    "V-side supply perturbation of the P/PN path only; not a strict "
    "DDR-priority counterfactual; no SCI conclusion is drawn from this run.")


def _load_sibling(name):
    """Load a .py file that sits next to this wrapper, no package needed."""
    here = pathlib.Path(__file__).resolve().parent
    path = here / (name + '.py')
    if not path.is_file():
        raise SystemExit('cannot find ' + name + '.py next to this wrapper')
    spec = importlib.util.spec_from_file_location('pn_gap64_wrapper_' + name, path)
    if spec is None or spec.loader is None:
        raise SystemExit('cannot load ' + str(path))
    module = importlib.util.module_from_spec(spec)
    sys.modules['pn_gap64_wrapper_' + name] = module
    spec.loader.exec_module(module)
    return module


def select_source_cases(cases):
    """Deterministic filter of the original plan; original order is kept.

    Keeps explore cases with a_pages == 8, mode P/PN, v_gap in (0,1000,4000)
    and block in (1,2,3).  Each kept case is shallow-copied, its id gets the
    'pn-gap64-' prefix and its transfers/warmup are set to 64/256.
    """
    selected = []
    for case in cases:
        if case['phase'] != TARGET_PHASE:
            continue
        if int(case['a_pages']) != TARGET_A_PAGES:
            continue
        if int(case['mode']) not in PN_MODES:
            continue
        if int(case['v_gap_cycles']) not in TARGET_GAPS:
            continue
        if int(case['block']) not in TARGET_BLOCKS:
            continue
        probe = dict(case)
        probe['id'] = PN_ID_PREFIX + case['id']
        probe['transfers'] = PN_TRANSFERS
        probe['warmup'] = PN_WARMUP
        # timeout_ms is intentionally left at the source value.
        selected.append(probe)
    return selected


def build_pn_plan(source_plan):
    cases = select_source_cases(source_plan.get('cases') or [])
    phases = {name: 0 for name in SOURCE_PHASES}
    for case in cases:
        phases[case['phase']] = phases.get(case['phase'], 0) + 1
    return {
        'schema': PN_PLAN_SCHEMA,
        'plan_generation': PN_PLAN_GENERATION,
        'plan_generation_note': PN_PLAN_GENERATION_NOTE,
        'source_plan_schema': source_plan.get('schema'),
        'source_plan_generation': source_plan.get('plan_generation'),
        'source_case_count': source_plan.get('count'),
        'auto_rerun': False,
        'resume': False,
        'calibration': source_plan.get('calibration', 'unverified'),
        'selection': PN_SELECTION_NOTE,
        'scope': PN_SCOPE_NOTE,
        'count': len(cases),
        'phases': phases,
        'cases': cases,
    }


def _verify_pn_plan(pn_plan, expected_timeout_ms):
    if pn_plan['count'] != EXPECTED_COUNT or pn_plan['phases'] != EXPECTED_PHASES:
        raise SystemExit('pn-gap64 selection drifted: count=%r phases=%r'
                         % (pn_plan['count'], pn_plan['phases']))
    seen = set()
    for case in pn_plan['cases']:
        cid = case['id']
        if not cid.startswith(PN_ID_PREFIX) or cid in seen:
            raise SystemExit('pn-gap64 id is not prefixed/unique: %r' % (cid,))
        seen.add(cid)
        got = (case['phase'], int(case['a_pages']), int(case['mode']),
               int(case['v_gap_cycles']), int(case['block']),
               int(case['transfers']), int(case['warmup']),
               int(case['timeout_ms']))
        ok = (got[0] == TARGET_PHASE
              and got[1] == TARGET_A_PAGES
              and got[2] in PN_MODES
              and got[3] in TARGET_GAPS
              and got[4] in TARGET_BLOCKS
              and got[5] == PN_TRANSFERS
              and got[6] == PN_WARMUP
              and got[7] == expected_timeout_ms)
        if not ok:
            raise SystemExit('pn-gap64 case drifted: %s -> %r' % (cid, got))


def make_pn_plan_module(real_plan):
    """Plan-module facade: same API, but build_plan() is the frozen 18."""
    source_plan = real_plan.build_plan()
    if (source_plan.get('schema') != SOURCE_PLAN_SCHEMA
            or source_plan.get('count') != SOURCE_PLAN_COUNT):
        raise SystemExit(
            'source plan is not the expected %s/%d: schema=%r count=%r'
            % (SOURCE_PLAN_SCHEMA, SOURCE_PLAN_COUNT,
               source_plan.get('schema'), source_plan.get('count')))
    pn_plan = build_pn_plan(source_plan)
    _verify_pn_plan(pn_plan, int(real_plan.DEFAULT_TIMEOUT_MS))
    module = types.ModuleType('composition_plan_pn_gap64')
    for name in dir(real_plan):
        if name.startswith('__') and name.endswith('__'):
            continue
        setattr(module, name, getattr(real_plan, name))
    # Only build_plan() is replaced.  case_argv / validate_case / case_metrics /
    # cases_through_phase and all constants stay the original objects.
    module.build_plan = lambda: pn_plan
    return module


def main(argv=None):
    mac_module = _load_sibling('mac_composition')
    real_plan = _load_sibling('composition_plan')
    plan_module = make_pn_plan_module(real_plan)
    # mac_composition.main() looks load_plan_module() up at call time, so this
    # single rebind is enough; no other runner code is touched.
    mac_module.load_plan_module = lambda: plan_module
    return mac_module.main(sys.argv[1:] if argv is None else argv)


if __name__ == '__main__':
    sys.exit(main())
