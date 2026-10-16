"""Independent event consumer: deliberately does not import the trace generator."""
import argparse
import json


class InvalidTrace(ValueError):
    pass


def validate(trace):
    def need(condition, why):
        if not condition:
            raise InvalidTrace(why)

    need(trace.get('synthetic') is True, 'synthetic provenance missing')
    n, q = trace['N'], trace['Q']
    need(type(n) is int and type(q) is int and 1 <= q <= n, 'require N >= Q >= 1')
    active, generations, buffers, released = {}, {}, {}, set()
    submitted = completed = successful = peak = 0
    draining = False
    expected = None
    for index, event in enumerate(trace['events']):
        kind = event['kind']
        task, buffer, generation = event['task'], event['buffer'], event['generation']
        need(type(task) is int and 0 <= task < q, f'{index}: invalid task')
        need(type(generation) is int and generation >= 1, f'{index}: invalid generation')
        need(task not in released, f'{index}: released task reused')
        need(buffer == f'b{task}', f'{index}: stable buffer identity broken')
        if kind == 'submit':
            need(not draining, f'{index}: post-drain submission')
            need(task not in active, f'{index}: early reuse before completion')
        if submitted < q:
            need(kind == 'submit' and task == submitted, f'{index}: initial Q submissions required')
        elif expected is not None:
            need((kind, task, generation) == expected, f'{index}: callback follow-up mismatch')
        else:
            need(kind == 'complete', f'{index}: no independent arrival/free allowed')
        if kind == 'submit':
            need(generation == generations.get(task, 0) + 1, f'{index}: stale generation')
            need(event['submit_number'] == submitted + 1, f'{index}: submission counter mismatch')
            need(buffer not in buffers.values() or buffers.get(task) == buffer,
                 f'{index}: buffer aliases another task')
            buffers[task], generations[task], active[task] = buffer, generation, generation
            submitted += 1
            need(submitted <= n and len(active) <= q, f'{index}: submission/depth bound')
            peak = max(peak, len(active))
            draining = submitted == n
            expected = None
        elif kind == 'complete':
            need(active.get(task) == generation, f'{index}: completion lacks matching pending instance')
            raw = event['status_raw']
            need(type(raw) is int and 0 <= raw <= 65535, f'{index}: invalid status word')
            # NVMe layout: P[0], SC[8:1], SCT[11:9]. P alone is not an error.
            success = ((raw >> 1) & 255) == 0 and ((raw >> 9) & 7) == 0
            need(event['success'] is success, f'{index}: status mislabeled success/error')
            # Generic invalid namespace triggers a distinct source early-drain path.
            # That path is outside this finite-count projection, not silently simulated.
            need(not (((raw >> 9) & 7) == 0 and ((raw >> 1) & 255) == 0x0B),
                 f'{index}: invalid-namespace early-drain path outside projection')
            del active[task]
            completed += 1
            successful += int(success)
            expected = ('free', task, generation) if draining else ('submit', task, generation + 1)
        elif kind == 'free':
            need(draining and task not in active, f'{index}: free while pending/non-draining')
            need(generations.get(task) == generation, f'{index}: free stale generation')
            released.add(task)
            expected = None
        else:
            raise InvalidTrace(f'{index}: unknown event kind')
        need(len(active) == submitted - completed, f'{index}: queue accounting mismatch')
    need(expected is None, 'unfinished callback follow-up')
    need(submitted == completed == n, 'finite-count episode incomplete')
    need(not active and len(released) == q and draining, 'final buffers/requests not released')
    evidence = successful == n
    need(type(trace['claim_success']) is bool and trace['claim_success'] == evidence,
         'episode success claim disagrees with SC/SCT completion evidence')
    return {'io_submitted': submitted, 'io_completed': completed,
            'successful_completed': successful, 'successful_run_evidence': evidence,
            'peak_queue_depth': peak, 'released_buffers': len(released)}


if __name__ == '__main__':
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('trace')
    args = parser.parse_args()
    try:
        print(json.dumps({'accepted': True, **validate(json.load(open(args.trace)))}, indent=2))
    except (InvalidTrace, KeyError, TypeError, ValueError) as exc:
        print(json.dumps({'accepted': False, 'reason': str(exc)}, indent=2))
        raise SystemExit(1)
