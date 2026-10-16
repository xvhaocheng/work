"""New LR1 delivery/source/document integrity only. No model/test replay."""
import hashlib
import json
from pathlib import Path

O=Path(__file__).resolve().parents[1]
P=O.parents[1]
W=P.parent
checks=[]


def read(path):
    return json.loads((O/path).read_text())


def sha(path):
    return hashlib.sha256(path.read_bytes()).hexdigest()


def verify(ok,name):
    checks.append(dict(name=name,passed=bool(ok)))
    assert ok,name


def run():
    acc=read('root-result-acceptance.json')
    verify(acc['goal_created_at']==1791990424 and not acc['paper_complete'], 'actual LR1 scope/paper')
    verify(acc['performance_conditions']==0 and not acc['old_experiments_replayed'], 'conditional performance gate/no replay')
    verify(acc['native_RTL_FPGA_runs']==0 and acc['physical_metrics'] is None, 'native/physical unmeasured')
    g=read('root/goal-registration.json')
    verify(g['goal']['createdAt']==1791990424 and g['goal']['status']=='active', 'actual no-budget goal registration')
    verify(g['remainingTokens'] is None, 'no extra token budget')
    f=read('prototype/functional-results.json')
    verify(f['status']=='pass' and f['directed_checks']==122 and len(f['checks'])==122,
           'actual122 directed checks')
    verify(all(c['passed'] for c in f['checks']), 'all actual checks passed')
    verify(len(f['group_names'])==len(f['traces'])==8, 'eight groups/eight complete traces')
    verify(f['performance_conditions']==0 and not f['native_cache_PTW_RTL_DDR_run'], 'reference not native/performance')
    verify(f['independent_policy_sha256']==sha(O/'controls/ordinary_policy.py'), 'integrated independent code unchanged')
    for i,t in enumerate(f['traces']):
        verify(t['quiet'] and not t['native_execution'] and t['timing_cycles'] is None, f'trace{i}:quiet/scope')
        c=t['counts'];es=t['events']
        verify([e['index'] for e in es]==list(range(len(es))),f'trace{i}:ordered event ledger')
        verify(c['r_bytes']==c['w_bytes']==8192 and c['r_beats']==c['w_beats']==1024,f'trace{i}:whole transport payload')
        verify(c['ar']==c['aw']==c['b']==4 and c['consumer']==2,f'trace{i}:all RWB/consumer')
        rs=[e for e in es if e['kind']=='r_accept'];ws=[e for e in es if e['kind']=='w_accept']
        verify(len(rs)==len(ws)==1024,f'trace{i}:actual accepted beats')
        for source in t['roles']:
            rdata=''.join(e['data'] for e in rs if e['source']==source)
            wdata=''.join(e['data'] for e in ws if e['source']==source)
            verify(rdata==wdata and len(bytes.fromhex(rdata))==4096,f'trace{i}:source{source}oldbytes ownership')
            verify(sum(e['last'] for e in rs if e['source']==source)==2,
                   f'trace{i}:source{source}two complete RLAST')
        acks=[e for e in es if e['kind']=='lease_ack']
        if t['error']:
            verify(not acks and t['source_quarantined'] and not t['source_released'],f'trace{i}:fault no release')
            verify(c['source_reuse_bytes']==0 and c['w_strobed_bytes']==4096,f'trace{i}:error sink distinct useful bytes')
            verify(c['errors']==1 and c['b_errors']==0,f'trace{i}:actual R/B error statuses')
        else:
            verify(len(acks)==1 and t['source_released'] and c['source_reuse_bytes']==4096,f'trace{i}:once ACK and new owner work')
            verify(c['w_strobed_bytes']==8192 and c['errors']==0,f'trace{i}:full successful payload')
            payload=bytes((n*17+3)%256 for n in range(4096))
            verify(t['target_data_sha256']==hashlib.sha256(payload).hexdigest(),f'trace{i}:old dest value hash')
            for e in acks:
                verify(e['global_IOFENCE_complete'] is False and e['old_obligations']==0,f'trace{i}:scoped ACK/old zero')
        if t['policy'] in ('candidate_step','phased','targeted_drain') and not t['error']:
            verify(t['ack_before_target_B'] and t['unrelated_inflight_at_phase_ack'],f'trace{i}:phase action beyond wholecopy')
    for ordinary in ('phased','targeted_drain'):
        for t in (x for x in f['traces'] if x['policy']==ordinary):
            c=next(x for x in f['traces'] if x['policy']=='candidate_step' and x['roles']==t['roles'] and x['error']==t['error'])
            verify(t['events']==c['events'] and t['counts']==c['counts'],f'full ordinary {ordinary}:{t["roles"]}/{t["error"]}')
    q=read('controls/policy-checks.json')
    verify(q['result']=='pass' and q['exhaustive_snapshots']==73728 and q['exhaustive_policy_calls']==221184,
           'independent new checks retained/no rerun')
    for rel,expected in q['policy_files_sha256'].items():
        verify(sha(O/'controls'/rel)==expected,'actual checked ordinary artifact:'+rel)
    sources=read('source/source-evidence.json')
    verify(sources['file_count']==len(sources['files'])==35 and sources['range_count']==118,'native35 files118ranges')
    for rec in sources['files']:
        path=Path(rec['absolute_path']);verify(sha(path)==rec['sha256'],'native source unchanged:'+rec['path'])
        lines=path.read_text().splitlines()
        for x in rec['ranges']:
            expected='\n'.join(lines[x['start_line']-1:x['end_line']])
            verify(expected==x['text'].rstrip('\n'),'native original range:'+x['id'])
    root=read('root/critical-source-evidence.json')
    verify(len(root['files'])==11 and sum(len(x['excerpts']) for x in root['files'])==22,'root11 files22 original excerpts')
    for rec in root['files']:
        verify(sha(Path(rec['path']))==rec['sha256'],'root critical original unchanged:'+Path(rec['path']).name)
    prior=read('controls/critical-sources.json')
    for rel,expected in prior['artifact_hashes'].items():
        verify(sha(O/'controls'/rel)==expected,'actual official/critical artifact:'+rel)
    cost=read('root/full-cost.json')
    verify(cost['paired_logical_delta']==0 and cost['aggregate_new_bits'] is None,'constructive paired bill vs unknown area')
    verify(cost['fixed']['buffer_bytes_total']==8192 and cost['fixed']['additional_payload_buffer_bytes']==0,'no extra payload')
    verify(cost['fixed']['PTW']==1 and cost['fixed']['IOTLB']==16 and cost['fixed']['TC_W_total_beats']==32,'fixed native resources')
    sync=read('root/state-sync.json');old=read('before/snapshots.json')
    verify(sync['five_pages_synced_before_complete'] and len(sync['files'])==5,'five pages sync before complete')
    for path,rec in old.items():
        verify(sha(O/rec['snapshot'])==rec['sha256'],'exact five-page archive:'+path)
        verify(sha(W/path)==sync['files'][path]['sha256'],'current synchronized:'+path)
        verify('1791990424' in (W/path).read_text(),'real goal navigation:'+path)
    for path in ('HANDOFF.md','iommu-platform/docs/validation.md'):
        verify((W/path).read_bytes().endswith((O/old[path]['snapshot']).read_bytes()),'whole prior historical body preserved:'+path)
    text=(O/'before/CURRENT_STATUS.md').read_text()
    verify((W/'CURRENT_STATUS.md').read_text().endswith(text[text.index('- 独立Linux贡献'):]),'independent maintenance suffix unchanged')
    runtime=read('root/runtime-root.json');workers=read('root/runtime-workers.json')
    verify(len(workers)==2,'two fresh exclusive execution seats')
    for who in [runtime]+workers:
        verify(who['latest_turn']['model']=='gpt-6.1-sol' and who['latest_turn']['effort']=='ultra',
               'actual model:'+who.get('agent','root'))
        verify(who['latest_window']['effective_context_window']==475000 and not who['overrides_changed'],
               'native window/no manual override:'+who.get('agent','root'))
    ledger=read('root/execution-ledger.json')
    for key in ('performance_conditions','old_experiment_model_validator_trace_build_runs','native_RTL_FPGA_runs',
                'production_mutations','pressure_capacity_walker_increase','restored_closed_branches_or_old_seats','non_Codex_calls'):
        verify(ledger[key]==0,'execution bound:'+key)
    manifest={str(p.relative_to(O)):sha(p) for p in sorted(O.rglob('*')) if p.is_file() and
              '__pycache__' not in str(p) and p.name not in ('delivery-verification.json','delivery-artifact-manifest.json')}
    (O/'root/delivery-artifact-manifest.json').write_text(json.dumps(manifest,indent=2)+'\n')
    result=dict(status='pass',checks=len(checks),details=checks,artifact_hashes=len(manifest),
                scope='new LR1 delivery/source/doc checks only; no protocol or policy/model replay',before_actual_complete=True)
    (O/'root/delivery-verification.json').write_text(json.dumps(result,indent=2)+'\n')
    print(json.dumps({k:result[k] for k in ('status','checks','artifact_hashes')}))


if __name__=='__main__':
    run()
