"""New SDH1 artifact/source/state integrity only; does not replay any model."""
import hashlib
import json
from pathlib import Path

O = Path(__file__).resolve().parents[1]
P = O.parents[1]
W = P.parent
checks = []


def verify(condition, label):
    checks.append(dict(name=label, passed=bool(condition)))
    if not condition:
        raise AssertionError(label)


def read(path):
    return json.loads((O / path).read_text())


def sha(path):
    return hashlib.sha256(path.read_bytes()).hexdigest()


def run():
    a = read('root-result-acceptance.json')
    verify(a['goal_created_at'] == 1791986539 and not a['paper_complete'], 'stable goal/paper scope')
    verify(a['new_performance_conditions'] == 0 and not a['old_results_replayed'], 'performance gate respected')
    verify(a['area'] is None and a['Fmax'] is None and a['main_completion_time'] is None,
           'physical performance unmeasured')
    g = read('root/goal-registration.json')
    verify(g['goal']['createdAt'] == 1791986539 and g['goal']['status'] == 'active', 'actual registration receipt')
    verify(g.get('remainingTokens') is None, 'no extra token budget')
    f = read('prototype/functional-results.json')
    verify(f['status'] == 'pass' and len(f['groups']) == 6, 'six new directed groups pass')
    verify(all(x['passed'] for x in f['groups']), 'all groups passed')
    verify(not f['native_rtl_run'] and not f['real_cache_or_ptw_run'], 'reference scope')
    verify(f['performance_conditions_run'] == 0 and f['old_experiments_run'] == 0, 'no forbidden replay')
    for i, t in enumerate(f['traces']):
        verify(t['quiet'], f'trace{i}: final quiet')
        events = t['events']
        verify([e['index'] for e in events] == list(range(len(events))), f'trace{i}: event indexing')
        if 'roles' not in t:
            verify(t['counts']['r_bytes'] == t['counts']['w_bytes'] == 8192, 'swap payload bytes')
            verify(t['counts']['b'] == 4 and t['counts']['consumer'] == 2, 'swap B/consumer')
            verify(not t['native_gate_and_frontend_execution'], 'swap conditional scope')
            verify(sum(x['r_bytes'] for x in t['scratch']) == 12288, 'scratch full read bytes')
            verify(sum(x['w_bytes'] for x in t['scratch']) == 12288, 'scratch full write bytes')
            continue
        verify(t['counts']['r_bytes'] == t['counts']['w_bytes'] == 48, f'trace{i}: full bytes')
        verify(t['counts']['ar'] == t['counts']['aw'] == t['counts']['b'] == 2, f'trace{i}: RWB transactions')
        verify(t['counts']['consumer'] == 2, f'trace{i}: consumer receipts')
        verify(t['counts']['errors'] == int(t['error']), f'trace{i}: error accounting')
        rs = [e for e in events if e['kind'] == 'r_accept']
        ws = [e for e in events if e['kind'] == 'w_accept']
        verify(len(rs) == len(ws) == 6, f'trace{i}: accepted beats')
        for source in t['roles']:
            rdata = sum([e['data'] for e in rs if e['source'] == source], [])
            wdata = sum([e['data'] for e in ws if e['source'] == source], [])
            verify(rdata == wdata and len(rdata) == 24, f'trace{i}: source{source} data ownership')
        held = [e for e in events if e['kind'] == 'physical_ar_valid']
        verify([e['ready'] for e in held[:3]] == [False, False, True], f'trace{i}: held offer check')
        for k in ('tag', 'pa', 'iova', 'bytes', 'epoch', 'path'):
            verify(held[0][k] == held[1][k] == held[2][k], f'trace{i}: stable {k}')
    e = read('prototype/ordinary-equivalence.json')
    verify(e['status'] == 'pass' and e['well_formed_snapshot_pairs'] == 32768, 'independent ordinary32768')
    verify(e['independent_ordinary_sha256'] == sha(O/'controls/ordinary_policy.py'), 'checked ordinary code unchanged')
    verify(len(e['ordinary_integration']) == 3 and all(x['same_events'] and x['quiet'] for x in e['ordinary_integration']),
           'three ordinary full event chains identical')
    for t in e['ordinary_integration']:
        c = next(c for c in f['traces'] if c.get('roles') == t['roles'] and c.get('error') == t['error'])
        verify(c['events'] == t['events'] and c['counts'] == t['counts'], f'ordinary {t["roles"]}/{t["error"]}: raw equality')
    src = read('root/source-evidence.json')
    verify(len(src['files']) == 16 and sum(len(s['excerpts']) for s in src['files']) == 36, '16sources36excerpts')
    for s in src['files']:
        verify(sha(Path(s['path'])) == s['sha256'], f'source unchanged:{Path(s["path"]).name}')
        lines = Path(s['path']).read_text().splitlines()
        for x in s['excerpts']:
            expected = '\n'.join(f'{n}: {lines[n-1]}' for n in range(x['start'], x['end']+1))
            verify(expected == x['text'], f'original excerpt:{Path(s["path"]).name}:{x["start"]}')
    cost = read('root/full-cost.json')
    verify(cost['candidate_minus_ordinary_logical_cost'] == 0 and cost['aggregate_incremental_bits'] is None,
           'paired cost vs unknown aggregate separated')
    verify(cost['original_resources']['DMA_buffer_beats'] == [512,512,3,3], 'actual buffer capacities')
    verify(cost['original_resources']['PTW'] == 1 and cost['original_resources']['IOTLB'] == 16, 'original walker/cache')
    snap = read('before/snapshots.json')
    sync = read('root/state-sync.json')
    verify(sync['before_goal_complete'] and len(sync['files']) == 5, 'five page sync before complete')
    for path, old in snap.items():
        verify(sha(O/old['snapshot']) == old['sha256'], f'archive exact:{path}')
        verify(sha(W/path) == sync['files'][path]['sha256'], f'synchronized:{path}')
        verify('1791986539' in (W/path).read_text(), f'goal navigation:{path}')
    for path in ['HANDOFF.md','iommu-platform/docs/validation.md']:
        verify((W/path).read_bytes().endswith((O/snap[path]['snapshot']).read_bytes()), f'whole historical body:{path}')
    old_current = (O/'before/CURRENT_STATUS.md').read_text()
    verify((W/'CURRENT_STATUS.md').read_text().endswith(old_current[old_current.index('- 独立Linux贡献'):]),
           'independent maintenance suffix unchanged')
    root = read('root/runtime-root.json')
    verify(root['latest_turn']['model'] == 'gpt-6.1-sol' and root['latest_turn']['effort'] == 'ultra', 'actual root model')
    verify(root['latest_window']['model_context_window'] == 475000 and not root['configuration_overrides_changed'], 'root native window')
    workers = read('root/runtime-workers.json')
    verify(len(workers) == 2, 'two fresh exclusive seats')
    for t in workers:
        verify(t['latest_turn']['model'] == 'gpt-6.1-sol' and t['latest_turn']['effort'] == 'ultra', 'actual worker:'+t['agent'])
        verify(t['effective_context_window'] == 475000 and not t['overrides_changed'], 'worker native window:'+t['agent'])
    ledger = read('root/execution-ledger.json')
    for k in ['performance_conditions','native_RTL_build_runs','FPGA_runs','old_experiment_model_trace_validator_build_runs',
              'production_mutations','new_pressure_capacity_walker_phase_warmup_search','closed_branch_or_old_seat_restores','external_non_Codex_models']:
        verify(ledger[k] == 0, 'execution zero:'+k)
    manifest = {str(p.relative_to(O)):sha(p) for p in sorted(O.rglob('*'))
                if p.is_file() and '__pycache__' not in str(p) and p.name not in
                ['delivery-verification.json','delivery-artifact-manifest.json']}
    (O/'root/delivery-artifact-manifest.json').write_text(json.dumps(manifest,indent=2)+'\n')
    result = dict(status='pass', checks=len(checks), details=checks, scope='new delivery only, no model replay',
                  five_page_sync_before_actual_complete=True, checked_file_hashes=len(manifest))
    (O/'root/delivery-verification.json').write_text(json.dumps(result,indent=2)+'\n')
    print(json.dumps({k:result[k] for k in ['status','checks','checked_file_hashes']}))


if __name__ == '__main__':
    run()
