"""Generate bounded synthetic traces, validate independently, retain negative witnesses."""
from copy import deepcopy
from datetime import datetime, timezone
from hashlib import sha256
import json
from pathlib import Path
import sys

from prototype import enumerate_traces
from replay import InvalidTrace, validate

HERE = Path(__file__).resolve().parent


def save(name, data):
    (HERE / name).write_text(json.dumps(data, indent=2) + '\n')


def replenishment(trace):
    return [e['task'] for e in trace['events'] if e['kind'] == 'submit'][trace['Q']:]


def main():
    traces = enumerate_traces(4, 2)
    results = [validate(t) for t in traces]
    choices = {tuple(e['task'] for e in t['events'] if e['kind'] == 'complete') for t in traces}
    # Two branching replenishment callbacks, then both final active tasks retire.
    assert len(traces) == len(choices) == 2 ** (4 - 2) * 2
    assert all(r['io_submitted'] == r['io_completed'] == r['successful_completed'] == 4
               and r['released_buffers'] == 2 and r['peak_queue_depth'] <= 2 for r in results)
    save('synthetic-success-traces.json', traces)
    save('synthetic-success-example.json', traces[0])
    witness = next(t for t in traces if replenishment(t) != replenishment(traces[0]))
    save('synthetic-order-witness.json', {'synthetic': True, 'left': traces[0], 'right': witness,
                                        'left_replenishment': replenishment(traces[0]),
                                        'right_replenishment': replenishment(witness)})
    negatives = {}
    early = deepcopy(traces[0])
    extra = deepcopy(early['events'][0])
    extra.update(generation=2, submit_number=3)
    early['events'].insert(2, extra)
    negatives['early-reuse'] = early
    post = deepcopy(traces[0])
    nth = next(i for i, e in enumerate(post['events'])
               if e['kind'] == 'submit' and e['submit_number'] == 4)
    extra = deepcopy(post['events'][nth])
    extra.update(generation=extra['generation'] + 1, submit_number=5)
    post['events'].insert(nth + 1, extra)
    negatives['post-drain-submit'] = post
    error_results = {}
    for label, raw in [('SC', 3), ('SCT', 513)]:
        error = deepcopy(traces[0])
        final = next(e for e in reversed(error['events']) if e['kind'] == 'complete')
        final.update(status_raw=raw, success=False)
        error['claim_success'] = False
        result = validate(error)
        assert result['io_completed'] == 4 and result['successful_completed'] == 3
        assert result['successful_run_evidence'] is False
        error_results[label] = result
        save(f'synthetic-error-{label}.json', error)
        mislabeled = deepcopy(error)
        next(e for e in reversed(mislabeled['events']) if e['kind'] == 'complete')['success'] = True
        mislabeled['claim_success'] = True
        negatives[f'{label}-error-mislabeled-success'] = mislabeled
        false_claim = deepcopy(error)
        false_claim['claim_success'] = True
        negatives[f'{label}-error-false-episode-claim'] = false_claim
    rejected = {}
    for name, trace in negatives.items():
        save(f'synthetic-negative-{name}.json', trace)
        try:
            validate(trace)
        except InvalidTrace as exc:
            rejected[name] = str(exc)
        else:
            raise AssertionError(f'negative mutation accepted: {name}')
    # Additional boundaries, not workload cases/calibration.
    boundary = {}
    for n, q in [(1, 1), (2, 2), (5, 1), (5, 3)]:
        cases = enumerate_traces(n, q)
        for trace in cases:
            validate(trace)
        boundary[f'N{n}-Q{q}'] = len(cases)
    save('verification.json', {
        'schema': 'clockless-spdk-verification-v1', 'passed': True, 'synthetic': True,
        'N': 4, 'Q': 2, 'constants_role': 'verification only; not workload calibration',
        'source_interface_callback_orders': len(traces),
        'hardware_order_reachability_claim': False,
        'success_results': results, 'error_results': error_results,
        'phase_one_raw_one_success': True, 'negative_mutations_rejected': rejected,
        'order_changes_replenishment': replenishment(traces[0]) != replenishment(witness),
        'boundary_verification_case_counts': boundary,
        'performance_evidence': False, 'actual_spdk_execution': False,
    })
    inputs = ['prototype.py', 'replay.py', 'verify.py', '../root/semantic-prototype-contract.json']
    save('run-receipt.json', {
        'utc': datetime.now(timezone.utc).isoformat(), 'execution': 'Codex local Python only',
        'command': 'python3 iommu-platform/out/d-spdk-contract-20261015/model/verify.py',
        'cwd': str(HERE.parents[3]), 'python': sys.version, 'result': 'PASS',
        'inputs_sha256': {name: sha256((HERE / name).read_bytes()).hexdigest() for name in inputs},
        'requested_configuration': {'model': 'gpt-6.1-sol', 'reasoning_effort': 'medium'},
        'actual_model_metadata': 'unknown; requested configuration is not runtime proof',
        'source_changes_build_device_network': False,
    })
    print(json.dumps({'passed': True, 'success_orders': len(traces),
                      'negative_rejections': len(rejected), 'error_cases': len(error_results)}))


if __name__ == '__main__':
    main()
