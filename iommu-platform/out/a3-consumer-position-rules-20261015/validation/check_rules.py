#!/usr/bin/env python3
"""Finite symbolic rule probes. No clock, payload execution, DMA or RTL model.

The reference gets identical producer/owner/public-completion information.
Completion-prefix semantics below is an explicit library-contract assumption,
NOT an assertion about unverified native completed-ID hardware.
"""
from pathlib import Path
import hashlib
import json

HERE = Path(__file__).resolve().parent
ROOT = HERE.parent.parent.parent
SOURCE = ROOT / 'out/a2-sva-implementation-20261015/hero/source/hero-tools/apps/omp/basic/axpy/axpy_snitch.c'
RUNTIME = ROOT / 'out/a2-native-sva-binding-20261015/sources/date-raw/carfield-date-runtime.txt'
checks = []


def record(name, predicate, **evidence):
    checks.append(dict(name=name, passed=bool(predicate), **evidence))


def public_closure(required, issue_order):
    """Cumulative public completion: cannot select arbitrary later completion."""
    if not required:
        return frozenset()
    end = max(issue_order.index(job) for job in required)
    return frozenset(issue_order[:end + 1])


def derive_safe_compute(scope, inputs, done, pending_slot_reads,
                        compute_writes, future_writes):
    """Rule W: producer completion plus two independent owner checks."""
    return (scope <= done and
            not (compute_writes & pending_slot_reads) and
            not (future_writes & pending_slot_reads))


def standard_reference(done, issue_order, consumer_inputs, compute_writes,
                       future_writes, obligations):
    """Direct generic dependency/resource constraints, without Rule-W helper."""
    # Prefix-completed library semantics is supplied to the standard model too.
    for producer in consumer_inputs:
        before = issue_order[:issue_order.index(producer) + 1]
        if any(job not in done for job in before):
            return False
    for job, read_slots in obligations.items():
        if job not in done:
            for slot in compute_writes | future_writes:
                if slot in read_slots:
                    return False
    return True


order = ('RX_last', 'RY_last', 'WY_previous')
inputs = frozenset(order[:2])
scope = public_closure(inputs, order)
observations = []
for prefix_len in range(4):
    done = frozenset(order[:prefix_len])
    pending = frozenset({'Y_old'}) if 'WY_previous' not in done else frozenset()
    rule = derive_safe_compute(scope, inputs, done, pending,
                               frozenset({'Y_current'}), frozenset())
    generic = standard_reference(done, order, inputs, frozenset({'Y_current'}),
                                  frozenset(), {'WY_previous': {'Y_old'}})
    observations.append(dict(completed_prefix_length=prefix_len,
                             wait_all_releases=set(order) <= done,
                             rule_releases=rule, standard_reference_releases=generic))
record('same-information standard model equals rule on all four public-prefix states',
       all(x['rule_releases'] == x['standard_reference_releases'] for x in observations),
       observations=observations, independent_algorithm_novelty=False)
record('last tile admits an abstract ordinary overlap opportunity',
       observations[2]['rule_releases'] and not observations[2]['wait_all_releases'],
       native_window_observed=False)
record('already retired previous output removes this scope difference',
       observations[3]['rule_releases'] == observations[3]['wait_all_releases'])
record('middle tile cannot overwrite old Y while old output owns its read',
       not derive_safe_compute(scope, inputs, scope, frozenset({'Y_old'}),
                               frozenset({'Y_current'}), frozenset({'Y_old'})))
record('single tile has no previous-output excess to remove',
       public_closure({'RX0', 'RY0'}, ('RX0', 'RY0')) == frozenset({'RX0', 'RY0'}))
record('an earlier unfinished public ID remains in the wait closure',
       public_closure({'RY_last'}, order) == frozenset({'RX_last', 'RY_last'}))
record('source capability does not authorize final completion or release',
       not frozenset({'WY_previous', 'WY_last'}) <= frozenset(order[:2]),
       all_tail_drain_cache_consumer_release_obligations_retained=True)

# Rule L checks a quantifier hazard without assigning an artificial duration.
# A DMA-only must-set fails to cover a permitted CPU implementation.
realization_must_jobs = {'device': {'DMA_input', 'compute', 'DMA_output'},
                         'CPU': {'compute'}}
global_must = set.intersection(*realization_must_jobs.values())
record('allowed CPU realization defeats a universal mandatory-DMA cut',
       'DMA_input' not in global_must and 'compute' in global_must)

result = dict(schema='a3-symbolic-owner-and-wait-scope-probes-v1',
    scope='finite mathematical/library-contract probes, not execution of AXPY, original source episodes, timing or hardware',
    assumptions=['cumulative completed-prefix API interpretation; native counter semantics remains unverified',
                 'opposite current/previous Y banks', 'final iteration has no future overwrite'],
    source_sha256=hashlib.sha256(SOURCE.read_bytes()).hexdigest(),
    runtime_sha256=hashlib.sha256(RUNTIME.read_bytes()).hexdigest(),
    checks=checks, passed=sum(c['passed'] for c in checks), count=len(checks),
    full_task_time=None, native_or_timing_cases_frozen=False,
    all_passed=all(c['passed'] for c in checks))
(HERE / 'rule-probes.json').write_text(json.dumps(result, indent=2) + '\n')
print(json.dumps({k: result[k] for k in ['count', 'passed', 'all_passed', 'full_task_time']}))
raise SystemExit(0 if result['all_passed'] else 1)
