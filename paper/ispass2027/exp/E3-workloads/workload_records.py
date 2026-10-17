#!/usr/bin/env python3
"""Strict ABI5 records; pure validation, no hardware access or inferred measurements."""
from collections import defaultdict
import importlib.util
import json
from pathlib import Path
import re
import model_full_coverage as model
from p0_records import require, backend_evidence as p0_backend_evidence
CLOCK_HZ=50_000_000
MAGIC=0x49444d4142544303
OBS_MAGIC=0x49444d414f425305
HIST_PATH=Path(__file__).resolve().parent/'source/hist_records.v5.as-reused.py'
spec=importlib.util.spec_from_file_location('e3_hist_v5',HIST_PATH)
HIST=importlib.util.module_from_spec(spec);spec.loader.exec_module(HIST)


def backend_evidence(p0, long, packet):
    base=p0_backend_evidence(p0)
    l,p=(json.loads(Path(x).read_text()) for x in (long,packet))
    require(all(x['status']=='passed' and not x.get('fixture_only',False) for x in (l,p)), 'ABI5 backend evidence failed/fixture')
    require(l['case_count']==4 and p['case_count']==3, 'ABI5 backend case coverage')
    for c,length,bursts in [('long_16384',16384,8),('long_65536',65536,32),('long_65536_offset64',65536,33)]:
        value=next(x for x in l['cases'] if x['case']==c)
        require(value['ar']==value['aw']==bursts and value['read_bytes']==value['write_bytes']==length,'ABI5 long backend mismatch')
    for c,length,offset,bursts in [('packet_256_offset0',256,0,1),('packet_1536_offset0',1536,0,1),('packet_1536_offset1536',1536,1536,2)]:
        v=p['by_case'][c]
        require(v['length']==length and v['offset']==offset and v['ar']==v['aw']==bursts and v['read_bytes']==v['write_bytes']==length,'ABI5 packet backend mismatch')
    return {'qualified':True,'p0':base,'long':l['case_count'],'packet':p['case_count']}


def geometry(g,shape,size):
    require(shape in ('ring','nic','storage','tile') and (size in model.SIZES or (shape=='nic' and size==1536)), 'ABI5 shape/operation size')
    require(set(g)=={'geometry','row_bytes','rows_per_operation','row_pitch_bytes','ring_pages','matrix_rows','columns','reserved0','reserved'},'ABI5 geometry fields')
    require(g['reserved0']==0 and g['reserved']==[0]*4,'ABI5 geometry reserved')
    expected=model.geometry('p1_minimal' if shape=='ring' else shape,size)
    pages=g['ring_pages']
    require(type(pages) is int and 0<pages<=256 and not pages&(pages-1),'ABI5 allocation bound')
    if shape=='tile':require(pages==256,'ABI5 tile allocation')
    else:expected['mapped_pages']=pages
    mapping={'geometry':{'linear':0,'page_packed':1,'tile_rows':2}[expected['geometry']],
             'row_bytes':expected['row_bytes'],'rows_per_operation':expected['rows_per_operation'],
             'row_pitch_bytes':expected['pitch_bytes'],'matrix_rows':expected['matrix_rows'],'columns':expected['columns']}
    require(all(type(g[k]) is int and g[k]==v for k,v in mapping.items()),'ABI5 geometry palette mismatch')
    if expected['geometry']=='linear':require(pages*4096%size==0,'ABI5 linear span')
    return expected


def coverage(actual,g,n,w):
    measured,warmup,union=(model.prefix(g,k) for k in (n,w,max(n,w)))
    require(actual=={'measured_distinct_bytes':measured['distinct_accessed_bytes'],
                    'measured_distinct_pages':measured['distinct_accessed_pages'],
                    'warmup_distinct_pages':warmup['distinct_accessed_pages'],
                    'verified_union_bytes':union['distinct_accessed_bytes'],'reserved':0},'ABI5 coverage mismatch')


def validate(item,backend):
    require(backend.get('qualified') is True,'ABI5 qualified backend evidence required')
    record=item['record'];p,r,c=record['input'],record['result'],record['capabilities']
    require(record['schema']=='idma.batch.workload.v5' and record['success'] and record['errno']==0 and record['stage']=='run' and record['run_attempted'],'ABI5 CLI failure')
    require(p['abi_version']==r['abi_version']==5 and p['struct_size']==r['struct_size']==544,'ABI5 layout')
    require(p['device']=='/dev/idma0' and not p['caps_only'] and p['flags']==p['reserved0']==p['reserved1']==0 and p['reserved']==[0,0] and p['supply']==1 and 0<p['run_timeout_ms']<=60000,'ABI5 input contract')
    vg=geometry(p['victim_geometry'],p['victim_shape'],p['victim_size'])
    n=p['victim_operations']*vg['rows_per_operation'];warm=p['victim_warmup_operations']*vg['rows_per_operation']
    require(type(n) is int and 0<n<=262144 and type(warm) is int and 0<=warm<=16384,'ABI5 work bound')
    require(p['protocol'] in ('equal16MiB_fullVring_v1','explicit_counts_v1'),'ABI5 protocol')
    if p['protocol']=='equal16MiB_fullVring_v1':
        require(n*vg['row_bytes']==16*1048576 and warm==model.period(vg),'ABI5 default effective work/full-ring warmup')
    has_a=p['aggressor_id'] is not None
    if has_a:
        require(p['aggressor_id']==1,'ABI5 A role');ag=geometry(p['aggressor_geometry'],p['aggressor_shape'],p['aggressor_size'])
    else:
        require(p['aggressor_shape'] is None and p['aggressor_size'] is None and p['aggressor_geometry']==dict.fromkeys(['geometry','row_bytes','rows_per_operation','row_pitch_bytes','ring_pages','matrix_rows','columns','reserved0'],0)|{'reserved':[0]*4},'ABI5 Solo A geometry')
    require(c=={'abi_version':5,'struct_size':64,'flags':5,'device_id':0,'did':10,'max_pages':256,
                'max_rows_per_op':64,'max_operation_bytes':65536,'batch_magic':MAGIC,'observer_magic':OBS_MAGIC,
                'geometry_mask':7,'row_length_mask':127,'path_queue_depth':64,'max_run_timeout_ms':60000},'ABI5 capabilities')
    require(r['status']==0 and r['hpm_configured']==r['measurement_started']==r['measurement_complete']==1 and r['victim_elapsed_ns']>0 and r['victim_did']==10 and r['aggressor_did']==(11 if has_a else None),'ABI5 result/roles')
    require((r['aggressor'] is not None)==has_a,'ABI5 A presence')
    for role,g in [('victim',vg)]+([('aggressor',ag)] if has_a else []):
        s=r[role];w=r[role+'_warmup_descriptors']
        require(s['data_errors']==s['quarantined']==0 and 0<s['submitted']==s['completed']<=1<<32,'ABI5 data/quiet/count')
        require(s['completed_bytes']==s['completed']*g['row_bytes'] and type(w) is int and w>=0 and r[role+'_warmup_bytes']==w*g['row_bytes'],'ABI5 byte accounting')
        require(s['in_flight_at_start']==0 and s['in_flight_at_end']==(role=='aggressor'),'ABI5 batch endpoint state')
        require(all(type(s[k]) is int and s[k]>=0 for k in ('iotlb_miss','s1_ptw','s2_ptw')),'ABI5 missing HPM')
        coverage(r[role+'_coverage'],g,s['completed'],w)
    require(r['victim']['completed']==n and r['victim_warmup_descriptors']==warm,'ABI5 V count/warmup')
    if not has_a:
        require(r['aggressor_warmup_descriptors']==r['aggressor_warmup_bytes']==0 and r['aggressor_coverage']=={'measured_distinct_bytes':0,'measured_distinct_pages':0,'warmup_distinct_pages':0,'verified_union_bytes':0,'reserved':0},'ABI5 Solo A output')
    obs, paths = item['observer'], item['path']
    require(set(obs) == {'time', 'axi_v', 'axi_a'}, 'ABI5 incomplete observer parts')
    t, v, a = obs['time'], obs['axi_v'], obs['axi_a']
    require(t['valid'] == 1 and t['status'] == 4 and t['batch_polled'] == 1
            and t['issued'] == t['done'] == n and t['cycles'] > 0
            and t['cycles'] == t['h'] + t['gap'] and t['gap'] == n-1,
            'ABI5 invalid hardware window/task accounting')
    require(t['v_did'] == 10 and t['a_did'] == (11 if has_a else 0xffffffff), 'ABI5 observer role mismatch')
    require(sum(v[k] for k in ('neither', 'v_only', 'a_only', 'both')) == t['cycles'], 'ABI5 activity partition mismatch')
    transactions = model.prefix(vg, n)['predicted_AXI_bursts_source_model']
    require(v['ar'] == v['aw'] == v['rlast'] == v['b'] == transactions
            and v['start'] == v['end'] == 0, 'ABI5 victim AXI count mismatch')
    for lane in (v, a):
        require((lane['start'] >> 16) + lane['ar'] == lane['rlast'] + (lane['end'] >> 16)
                and (lane['start'] & 65535) + lane['aw'] == lane['b'] + (lane['end'] & 65535),
                'ABI5 private AXI stock/flow mismatch')
    require((a['ar'] > 0 and a['aw'] > 0 and v['both'] > 0) if has_a else
            v['both'] == v['a_only'] == a['ar'] == a['aw'] == 0, 'ABI5 aggressor activity mismatch')
    expected = {role+channel+str(stage) for role in 'va' for channel in 'rw' for stage in range(4)}
    require(set(paths) == expected, 'ABI5 requires 16 matched PATH records')
    for key, path in paths.items():
        require(all(isinstance(x, int) and x >= 0 for x in path.values()), 'ABI5 invalid PATH counter')
        require(path['valid'] == 1 and path['start'] + path['entered'] == path['full'] + path['carry'] + path['end'],
                'ABI5 PATH stock/flow mismatch')
        require(path['carry'] <= path['start'] and path['full'] <= path['entered']
                and path['max'] <= path['sum'] <= path['full']*path['max'] and path['area'] >= path['sum'],
                'ABI5 PATH latency accounting mismatch')
        if int(key[-1]) < 3:
            require(path['full'] + path['carry'] == paths[key[:-1]+str(int(key[-1])+1)]['entered'],
                    'ABI5 PATH stage continuity mismatch')
        if key.startswith('v'):
            require(path['start'] == path['end'] == path['carry'] == 0
                    and path['entered'] == path['full'] == transactions and path['area'] == path['sum'],
                    'ABI5 victim PATH window mismatch')
        elif not has_a:
            require(all(path[k] == 0 for k in ('entered', 'full', 'carry', 'sum', 'max', 'start', 'end', 'area')),
                    'ABI5 Solo aggressor PATH nonempty')
    # Stage0 leave is exactly the shared AR/AW handshake event. PATH0 enter is
    # first offer, not private accept: do not equate their opening/closing stock.
    for channel, shared in (('r', 'shared_ar'), ('w', 'shared_aw')):
        leave = sum(paths[role+channel+'0']['full'] + paths[role+channel+'0']['carry'] for role in 'va')
        require(a[shared] == leave, 'ABI5 shared/PATH0 leave mismatch')
    metrics = {'hardware_ms': t['cycles']/CLOCK_HZ*1000,
               'hardware_MiB_s': r['victim']['completed_bytes']/1048576/(t['cycles']/CLOCK_HZ),
               'hardware_cycles': t['cycles'], 'software_ms': r['victim_elapsed_ns']/1e6,
               'overlap_percent': v['both']*100/t['cycles'],
               'shared_minus_private_ar': a['shared_ar']-v['ar']-a['ar'],
               'shared_minus_private_aw': a['shared_aw']-v['aw']-a['aw']}
    for role in ('victim', 'aggressor'):
        for name in ('iotlb_miss', 's1_ptw', 's2_ptw'):
            metrics[role+'_'+name] = r[role][name] if r[role] is not None else None
    for key, path in paths.items():
        metrics['path_'+key+'_mean_cycles'] = path['sum']/path['full'] if path['full'] else None
        metrics['path_'+key+'_max_cycles'] = path['max'] if path['full'] else None
    return metrics


def parse(output, expected_kernel, backend):
    text = output.replace('\r', '')
    require(re.search(r'^KERNEL='+re.escape(expected_kernel)+r'$', text, re.M), 'ABI5 kernel identity')
    boot = re.search(r'^BOOT=([0-9a-f-]{36})$', text, re.M)
    require(boot is not None, 'ABI5 boot identity')
    require(re.findall(r'^TAINT(?:_AFTER)?=(\d+)$', text, re.M) == ['0', '0'], 'ABI5 tainted kernel')
    require(re.findall(r'^CASE_EXIT=(\d+)$', text, re.M) == ['0'], 'ABI5 command exit')
    previous = re.findall(r'^PRE_SEQ=(\d+)$', text, re.M)
    require(len(previous) == 1, 'ABI5 fresh observer watermark')
    sequence = int(previous[0])+1
    observation_text = text.split('\nCASE_EXIT=0\n', 1)[1].split('\nTAINT_AFTER=', 1)[0]
    decoder = json.JSONDecoder()
    records = [decoder.raw_decode(text[match.start():])[0]
               for match in re.finditer(r'\{\s*"schema"\s*:\s*"idma\.batch\.workload\.v5"', text)]
    require(len(records) == 1, 'ABI5 exactly one CLI JSON')
    observations, paths = defaultdict(dict), {}

    def numbers(fields):
        return {k: int(v, 16) if v.startswith('0x') else int(v)
                for k, v in re.findall(r'(\w+)=(0x[0-9a-fA-F]+|\d+)', fields)}

    for seq, part, fields in re.findall(r'idma_batch_obs seq=(\d+) part=(\w+) ([^\n]+)', observation_text):
        values, seq = numbers(fields), int(seq)
        require(part not in observations[seq] or observations[seq][part] == values, 'ABI5 inconsistent duplicate observer')
        observations[seq][part] = values
    require(observations and sequence in observations and max(observations) == sequence, 'ABI5 observer sequence mismatch')
    for seq, role, channel, stage, fields in re.findall(
            r'idma_path_obs seq=(\d+) role=([va]) channel=([rw]) stage=(\d+) ([^\n]+)', observation_text):
        if int(seq) == sequence:
            key, value = role+channel+stage, numbers(fields)
            require(key not in paths or paths[key] == value, 'ABI5 inconsistent duplicate PATH')
            paths[key] = value
    item = {'record': records[0], 'observer': observations[sequence], 'path': paths,
            'sequence': sequence, 'boot_id': boot[1]}
    require(item['record']['result']['observer_sequence'] == sequence, 'ABI5 returned/fresh sequence mismatch')
    item['metrics'] = validate(item, backend)
    item['histogram'] = HIST.attach_from_output(output, item)
    require(item['histogram']['info']['version'] == 5 and item['histogram']['info']['depth'] == 64, 'ABI5 OBS5/Depth64 histogram required')
    return item
