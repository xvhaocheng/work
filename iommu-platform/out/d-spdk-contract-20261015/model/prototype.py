"""Clockless synthetic application-callback projection; no device execution."""
from copy import deepcopy


def enumerate_traces(n=4, q=2):
    if type(n) is not int or type(q) is not int or not 1 <= q <= n:
        raise ValueError('require integer N >= Q >= 1')
    initial = {'submitted': 0, 'draining': False, 'active': {},
               'generation': {t: 0 for t in range(q)}, 'events': []}

    def submit(s, task):
        s['generation'][task] += 1
        generation = s['generation'][task]
        s['active'][task] = generation
        s['submitted'] += 1
        s['draining'] = s['submitted'] == n
        s['events'].append({'kind': 'submit', 'task': task, 'buffer': f'b{task}',
                            'generation': generation, 'submit_number': s['submitted']})

    for task in range(q):
        submit(initial, task)

    def visit(s):
        if not s['active']:
            yield {'synthetic': True, 'schema': 'clockless-spdk-v1', 'N': n, 'Q': q,
                   'events': s['events'], 'claim_success': True}
            return
        # Each branch recomputes the callback-driven replenishment. No fixed arrivals.
        for task in sorted(s['active']):
            branch = deepcopy(s)
            generation = branch['active'].pop(task)
            branch['events'].append({'kind': 'complete', 'task': task,
                                     'buffer': f'b{task}', 'generation': generation,
                                     'status_raw': 1, 'success': True})
            if branch['draining']:
                branch['events'].append({'kind': 'free', 'task': task,
                                         'buffer': f'b{task}', 'generation': generation})
            else:
                submit(branch, task)
            yield from visit(branch)

    return list(visit(initial))
