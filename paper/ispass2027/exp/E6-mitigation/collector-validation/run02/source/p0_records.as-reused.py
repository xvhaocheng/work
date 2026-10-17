#!/usr/bin/env python3
"""Pure P0 record/observer checks. No hardware access or inferred measurements.

Private AXI and shared AXI are distinct boundaries. Shared counts match PATH0
leave, not the sum of private accepts. See board/observer-shared-counts.md.
"""
from collections import defaultdict
import json
from pathlib import Path
import re

MAGIC = 0x49444d4142544302
CLOCK_HZ = 50_000_000


def require(condition, message):
    if not condition:
        raise ValueError(message)


def backend_evidence(path):
    value = json.loads(Path(path).read_text())
    require(not value.get('fixture_only', False), 'protocol fixtures are not backend evidence')
    require(value['status'] == 'passed', 'backend finite evidence did not pass')
    bursts = {int(k): v for k, v in value['burst_per_descriptor'].items()}
    require(set(bursts) == {64, 1024, 4096}, 'backend evidence length set mismatch')
    require(all(isinstance(v, int) and v > 0 for v in bursts.values()), 'invalid burst evidence')
    return bursts


def validate(item, bursts):
    record = item['record']
    require(record['schema'] == 'idma.batch.p0.v2' and record['success']
            and record['errno'] == 0 and record['stage'] == 'run' and record['run_attempted'], 'P0 CLI failure')
    p, r, c = record['input'], record['result'], record['capabilities']
    n, length = p['victim_transfers'], p['victim_chunk_bytes']
    require(p['abi_version'] == r['abi_version'] == 2 and p['struct_size'] == r['struct_size'] == 328,
            'P0 ABI/layout mismatch')
    require(p['device'] == '/dev/idma0' and not p['caps_only'] and p['flags'] == 0
            and p['reserved'] == [0]*6 and p['geometry'] == 1 and p['victim_pages'] == 1,
            'P0 victim geometry mismatch')
    require(n == 4096 and p['warmup_transfers'] == 256 and length in bursts
            and p['aggressor_chunk_bytes'] == 4096 and 0 < p['run_timeout_ms'] <= 300000,
            'P0 work/length mismatch')
    has_a = p['aggressor_id'] is not None
    require((p['aggressor_id'] == 1 and p['aggressor_pages'] in (1, 8)) if has_a
            else p['aggressor_pages'] == 1, 'P0 aggressor geometry mismatch')
    require(c['abi_version'] == 2 and c['struct_size'] == 64 and c['flags'] & 5 == 5
            and c['device_id'] == 0 and c['did'] == 10 and c['batch_magic'] == MAGIC
            and c['page_bytes'] == 4096 and c['max_pages'] >= 8 and c['reserved'] == [0]*3
            and c['victim_length_mask'] == 7 and c['aggressor_length_mask'] == 4
            and c['geometry_mask'] == 1 and c['max_run_timeout_ms'] >= p['run_timeout_ms'], 'P0 capability mismatch')
    require(r['status'] == 0 and r['hpm_configured'] == r['measurement_started'] == r['measurement_complete'] == 1
            and r['victim_elapsed_ns'] > 0 and r['victim_did'] == 10
            and r['aggressor_did'] == (11 if has_a else None), 'P0 incomplete result/role mismatch')
    require((r['aggressor'] is not None) == has_a, 'P0 aggressor presence mismatch')
    for role in ('victim', 'aggressor'):
        lane = r[role]
        if lane is None:
            continue
        actual_length = length if role == 'victim' else 4096
        require(lane['data_errors'] == lane['quarantined'] == 0, 'P0 data error/quarantine')
        require(0 < lane['submitted'] == lane['completed'] <= 1 << 32, 'P0 undrained/invalid lane')
        require(lane['completed_bytes'] == lane['completed']*actual_length, 'P0 byte accounting mismatch')
        require(all(isinstance(lane[k], int) and lane[k] >= 0 for k in ('iotlb_miss', 's1_ptw', 's2_ptw')),
                'P0 missing HPM counters')
    require(r['victim']['completed'] == n, 'P0 victim count mismatch')
    obs, paths = item['observer'], item['path']
    require(set(obs) == {'time', 'axi_v', 'axi_a'}, 'P0 incomplete observer parts')
    t, v, a = obs['time'], obs['axi_v'], obs['axi_a']
    require(t['valid'] == 1 and t['status'] == 4 and t['batch_polled'] == 1
            and t['issued'] == t['done'] == n and t['cycles'] > 0
            and t['cycles'] == t['h'] + t['gap'] and t['gap'] == n-1,
            'P0 invalid hardware window/task accounting')
    require(t['v_did'] == 10 and t['a_did'] == (11 if has_a else 0xffffffff), 'P0 observer role mismatch')
    require(sum(v[k] for k in ('neither', 'v_only', 'a_only', 'both')) == t['cycles'], 'P0 activity partition mismatch')
    transactions = n*bursts[length]
    require(v['ar'] == v['aw'] == v['rlast'] == v['b'] == transactions
            and v['start'] == v['end'] == 0, 'P0 victim AXI count mismatch')
    for lane in (v, a):
        require((lane['start'] >> 16) + lane['ar'] == lane['rlast'] + (lane['end'] >> 16)
                and (lane['start'] & 65535) + lane['aw'] == lane['b'] + (lane['end'] & 65535),
                'P0 private AXI stock/flow mismatch')
    require((a['ar'] > 0 and a['aw'] > 0 and v['both'] > 0) if has_a else
            v['both'] == v['a_only'] == a['ar'] == a['aw'] == 0, 'P0 aggressor activity mismatch')
    expected = {role+channel+str(stage) for role in 'va' for channel in 'rw' for stage in range(4)}
    require(set(paths) == expected, 'P0 requires 16 matched PATH records')
    for key, path in paths.items():
        require(all(isinstance(x, int) and x >= 0 for x in path.values()), 'P0 invalid PATH counter')
        require(path['valid'] == 1 and path['start'] + path['entered'] == path['full'] + path['carry'] + path['end'],
                'P0 PATH stock/flow mismatch')
        require(path['carry'] <= path['start'] and path['full'] <= path['entered']
                and path['max'] <= path['sum'] <= path['full']*path['max'] and path['area'] >= path['sum'],
                'P0 PATH latency accounting mismatch')
        if int(key[-1]) < 3:
            require(path['full'] + path['carry'] == paths[key[:-1]+str(int(key[-1])+1)]['entered'],
                    'P0 PATH stage continuity mismatch')
        if key.startswith('v'):
            require(path['start'] == path['end'] == path['carry'] == 0
                    and path['entered'] == path['full'] == transactions and path['area'] == path['sum'],
                    'P0 victim PATH window mismatch')
        elif not has_a:
            require(all(path[k] == 0 for k in ('entered', 'full', 'carry', 'sum', 'max', 'start', 'end', 'area')),
                    'P0 Solo aggressor PATH nonempty')
    # Stage0 leave is exactly the shared AR/AW handshake event. PATH0 enter is
    # first offer, not private accept: do not equate their opening/closing stock.
    for channel, shared in (('r', 'shared_ar'), ('w', 'shared_aw')):
        leave = sum(paths[role+channel+'0']['full'] + paths[role+channel+'0']['carry'] for role in 'va')
        require(a[shared] == leave, 'P0 shared/PATH0 leave mismatch')
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


def parse(output, expected_kernel, bursts):
    text = output.replace('\r', '')
    require(re.search(r'^KERNEL='+re.escape(expected_kernel)+r'$', text, re.M), 'P0 kernel identity')
    boot = re.search(r'^BOOT=([0-9a-f-]{36})$', text, re.M)
    require(boot is not None, 'P0 boot identity')
    require(re.findall(r'^TAINT(?:_AFTER)?=(\d+)$', text, re.M) == ['0', '0'], 'P0 tainted kernel')
    require(re.findall(r'^CASE_EXIT=(\d+)$', text, re.M) == ['0'], 'P0 command exit')
    previous = re.findall(r'^PRE_SEQ=(\d+)$', text, re.M)
    require(len(previous) == 1, 'P0 fresh observer watermark')
    sequence = int(previous[0])+1
    observation_text = text.split('\nCASE_EXIT=0\n', 1)[1].split('\nTAINT_AFTER=', 1)[0]
    decoder = json.JSONDecoder()
    records = [decoder.raw_decode(text[match.start():])[0]
               for match in re.finditer(r'\{\s*"schema"\s*:\s*"idma\.batch\.p0\.v2"', text)]
    require(len(records) == 1, 'P0 exactly one CLI JSON')
    observations, paths = defaultdict(dict), {}

    def numbers(fields):
        return {k: int(v, 16) if v.startswith('0x') else int(v)
                for k, v in re.findall(r'(\w+)=(0x[0-9a-fA-F]+|\d+)', fields)}

    for seq, part, fields in re.findall(r'idma_batch_obs seq=(\d+) part=(\w+) ([^\n]+)', observation_text):
        values, seq = numbers(fields), int(seq)
        require(part not in observations[seq] or observations[seq][part] == values, 'P0 inconsistent duplicate observer')
        observations[seq][part] = values
    require(observations and sequence in observations and max(observations) == sequence, 'P0 observer sequence mismatch')
    for seq, role, channel, stage, fields in re.findall(
            r'idma_path_obs seq=(\d+) role=([va]) channel=([rw]) stage=(\d+) ([^\n]+)', observation_text):
        if int(seq) == sequence:
            key, value = role+channel+stage, numbers(fields)
            require(key not in paths or paths[key] == value, 'P0 inconsistent duplicate PATH')
            paths[key] = value
    item = {'record': records[0], 'observer': observations[sequence], 'path': paths,
            'sequence': sequence, 'boot_id': boot[1]}
    item['metrics'] = validate(item, bursts)
    return item
