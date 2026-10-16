"""Independent ordinary choice and new protocol integration; no performance."""
import copy
import hashlib
import importlib.util
import itertools
import json
from pathlib import Path
from protocol import candidate_choose
from check_functional import fixture, full_chain

HERE = Path(__file__).resolve().parent
spec = importlib.util.spec_from_file_location('independent_ordinary', HERE.parent / 'controls/ordinary_policy.py')
ordinary = importlib.util.module_from_spec(spec)
spec.loader.exec_module(ordinary)


def check():
    fields = ['ptw_busy', 'result_free', 'priority_pending', 'grant_used',
              'aw_accepted', 'preceding_independent', 'hit', 'permissions_ok',
              'native_guard_ok', 'needed_for_head', 'oldest', 'job_match', 'active_is_owner']
    count = 0
    digest = hashlib.sha256()
    for roles in ((0, 8), (8, 0), (4, 12), (12, 4)):
        p, a, b = fixture(roles=roles)
        base = p.state()
        for values in itertools.product((False, True), repeat=len(fields)):
            v = dict(zip(fields, values)); s = copy.deepcopy(base)
            for k in ('ptw_busy', 'result_free', 'grant_used'):
                s[k] = v[k]
            s['normal_or_invalidation_pending'] = v['priority_pending']
            for k in ('aw_accepted', 'preceding_independent'):
                s['drain'][k] = v[k]
            h = next(h for h in s['heads'] if h['source'] == b.source)
            for k in ('hit', 'permissions_ok', 'native_guard_ok', 'needed_for_head', 'oldest'):
                h[k] = v[k]
            if not v['job_match']:
                h['job'] = 999
            if v['active_is_owner']:
                s['active_source'] = b.source
            c, o = candidate_choose(s), ordinary.choose(s)
            assert c == o, (roles, v, c, o)
            digest.update(json.dumps([roles, values, c], separators=(',', ':')).encode())
            count += 1
    previous = json.loads((HERE / 'functional-results.json').read_text())
    integration = []
    for roles, error in (((0, 8), False), ((8, 0), False), ((0, 8), True)):
        p = full_chain(choose=ordinary.choose, roles=roles, error=error)
        c = next(t for t in previous['traces']
                 if t.get('roles') == list(roles) and t.get('error') == error)
        normalized_events = json.loads(json.dumps(p.events))
        assert normalized_events == c['events'] and p.counts == c['counts'] and p.quiet()
        integration.append(dict(roles=list(roles), error=error, same_events=True,
                                counts=p.counts, events=p.events, quiet=p.quiet()))
    result = dict(status='pass', well_formed_snapshot_pairs=count, dimensions=fields,
                  roles=[[0,8],[8,0],[4,12],[12,4]], enumeration_sha256=digest.hexdigest(),
                  independent_ordinary_sha256=hashlib.sha256(Path(spec.origin).read_bytes()).hexdigest(),
                  ordinary_integration=integration, conditional_structural_proof=True,
                  native_source_generation_proven=False, performance_conditions=0,
                  cycles=None, area=None, fmax=None)
    (HERE / 'ordinary-equivalence.json').write_text(json.dumps(result, indent=2)+'\n')
    print(json.dumps({k: result[k] for k in ('status', 'well_formed_snapshot_pairs', 'performance_conditions')}))


if __name__ == '__main__':
    check()
