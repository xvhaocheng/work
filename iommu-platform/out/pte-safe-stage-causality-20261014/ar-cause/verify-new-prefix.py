#!/usr/bin/env python3
"""One offline pass over the new passive CSV. Never executes the DUT or a build."""
import csv
import hashlib
import json
from collections import Counter, defaultdict, deque
from pathlib import Path

BASE = Path(__file__).resolve().parents[1]
MODEL = BASE / 'model'
OUT = BASE / 'ar-cause'
rows = [{k: int(v) for k, v in row.items()}
        for row in csv.DictReader((MODEL / 'prefix.csv').open())]
issues = []
def check(ok, msg):
    if not ok:
        issues.append(msg)

def fire(row, channel):
    return bool(row[channel + '_valid'] and row[channel + '_ready'])

pte_data = {
    0x20000: 0x10001, 0x40400: 0x14001,
    0x50000: 0x200000df, 0x50008: 0x200004df,
    0x30000: 0x18001, 0x60800: 0x1c001,
    0x70000: 0x240000df, 0x70008: 0x240400df,
}
owner_keys = ['owner_bank', 'owner_did', 'owner_iova', 'owner_type']
ptes, reads, writes = [], [], []
read_queues = defaultdict(deque)
write_queues = deque()
finished_writes = defaultdict(deque)
prev = None
active_pte = None
held_by_id = Counter()
counts = Counter()
row_lookup = {r['cycle']: r for r in rows}

def match_pte(addr, field):
    candidates = [p for p in ptes if p['pte_addr'] == addr and field not in p]
    check(len(candidates) == 1,
          f'PTE phase match {field} addr={addr:#x}: {len(candidates)} candidates')
    return candidates[0] if candidates else None

for line, r in enumerate(rows, 2):
    t = r['cycle']
    r['_csv_line'] = line
    if prev is not None:
        check(t == prev['cycle'] + 1, f'CSV cycle discontinuity at {t}')
        check(r['prior_mheld'] == int(prev['mvalid'] and not prev['mready']),
              f'prior_mheld mismatch at {t}')
    check(sum(len(q) for q in read_queues.values()) == r['memq'],
          f'DDR outstanding queue count mismatch at {t}')
    check(r['ddr_ar_ready'] == int(r['memq'] < 16), f'DDR credit-ready mismatch at {t}')
    counts['both_valid'] += r['pair'] == 3
    fresh = r['pair'] == 3 and not r['prior_mheld']
    counts['any_fresh'] += fresh
    counts['safe_fresh'] += bool(fresh and r['read0'] and r['read1'] and r['capacity'] == 3)
    counts['shared_mux_comp_ds_covalid'] += (r['mux_reqs'] & 12) == 12
    counts['mux_held_ticks'] += bool(r['mux_valid'] and not r['mux_ready'])
    counts['ddr_ar_stall_ticks'] += bool(r['ddr_ar_valid'] and not r['ddr_ar_ready'])
    counts['pte_internal_ar_stall_ticks'] += bool(r['pte_valid'] and not r['pte_ready'])
    counts['delayed_ds_ar_stall_ticks'] += bool(r['ds_delayed_valid'] and not r['ds_delayed_ready'])
    counts['ds_spill_output_ar_stall_ticks'] += bool(r['ds_valid'] and not r['ds_ready'])
    if r['ddr_r_valid'] and not r['ddr_r_ready']:
        held_by_id[r['ddr_r_id']] += 1
    if r['pte_valid'] and (prev is None or not prev['pte_valid'] or fire(prev, 'pte')):
        check(active_pte is None, f'new PTE with prior unconsumed at {t}')
        active_pte = {
            'ordinal': len(ptes) + 1, 'full_ddr_id': 48,
            'local_ptw_id_from_compiled_source': 0,
            'birth': t, 'birth_csv_line': line, 'pte_addr': r['pte_addr'],
            'pte_addr_hex': hex(r['pte_addr']), 'pte_stage': r['pte_stage'],
            'pte_level': r['pte_level'],
            **{k: r[k] for k in owner_keys},
            'older_ddr_obligations_at_birth': [
                {'full_id': q['id'], 'addr': q['addr'], 'accepted': q['accepted'],
                 'remaining_beats': q['len'] + 1 - q['returned_beats']}
                for queue in read_queues.values() for q in queue],
        }
        ptes.append(active_pte)
    if fire(r, 'pte'):
        check(active_pte is not None, f'PTE accept without birth at {t}')
        if active_pte is not None:
            check('internal_accept' not in active_pte, f'double PTE internal accept at {t}')
            active_pte['internal_accept'] = t
    for prefix, id_key, addr_key, first_field, accept_field in [
        ('ds', 'ds_id', 'ds_addr', 'ds_spill_first_valid', 'ds_spill_to_delayer_accept'),
        ('ds_delayed', 'ds_delayed_id', 'ds_delayed_addr', 'delayed_first_valid', 'xbar_slave3_accept'),
        ('mux', 'mux_id', 'mux_addr', 'mux_first_valid', 'mux_to_output_spill_accept')
    ]:
        expected_id = 48 if prefix == 'mux' else 0
        if r[prefix + '_valid'] and r[id_key] == expected_id:
            is_new = prev is None or not prev[prefix + '_valid'] or fire(prev, prefix)
            if is_new:
                p = match_pte(r[addr_key], first_field)
                if p is not None:
                    p[first_field] = t
            if fire(r, prefix):
                p = match_pte(r[addr_key], accept_field)
                if p is not None:
                    p[accept_field] = t
    if fire(r, 'ddr_ar'):
        q = {'id': r['ddr_ar_id'], 'addr': r['ddr_ar_addr'],
             'len': r['ddr_ar_len'], 'accepted': t, 'accepted_csv_line': line,
             'returned_beats': 0, 'held_ticks': 0}
        reads.append(q)
        read_queues[q['id']].append(q)
        if q['id'] == 48:
            p = match_pte(q['addr'], 'ddr_accept')
            if p is not None:
                p['ddr_accept'] = t
                p['ddr_ar_len'] = q['len']
                p['ddr_read_ordinal'] = len(reads)
                q['pte_ordinal'] = p['ordinal']
    if r['ddr_r_valid']:
        queue = read_queues[r['ddr_r_id']]
        check(bool(queue), f'RVALID without accepted same-fullID AR at {t}')
        if queue:
            q = queue[0]
            if 'first_rvalid' not in q:
                q['first_rvalid'] = t
            check(bool(r['ddr_r_last']) == (q['returned_beats'] == q['len']),
                  f'R LAST length mismatch at {t}')
            check(r['ddr_r_resp'] == 0, f'DDR R error at {t}')
            if not r['ddr_r_ready']:
                q['held_ticks'] += 1
            if fire(r, 'ddr_r'):
                if 'first_rhandshake' not in q:
                    q['first_rhandshake'] = t
                q['returned_beats'] += 1
                if r['ddr_r_last']:
                    q['last_rhandshake'] = t
                    queue.popleft()
    if r['pte_rvalid']:
        check(active_pte is not None, f'PTW RVALID without active PTE at {t}')
        if active_pte is not None:
            active_pte.setdefault('ptw_first_rvalid', t)
            if r['pte_rready']:
                active_pte['ptw_consume'] = t
                active_pte['consume_csv_line'] = line
                active_pte['pte_rdata'] = r['pte_rdata']
                active_pte['pte_rdata_hex'] = hex(r['pte_rdata'])
                expected = pte_data.get(active_pte['pte_addr'])
                active_pte['pte_data_matches_setup_memory'] = r['pte_rdata'] == expected
                check(r['pte_rdata'] == expected, f'PTE value mismatch at {t}')
                check(all(r[k] == active_pte[k] for k in owner_keys),
                      f'PTE consume owner mismatch at {t}')
                active_pte = None
    if fire(r, 'ddr_aw'):
        w = {'id': r['ddr_aw_id'], 'len': r['ddr_aw_len'],
             'accepted': t, 'w_beats': 0}
        writes.append(w)
        write_queues.append(w)
    if fire(r, 'ddr_w'):
        check(bool(write_queues), f'W without accepted AW at {t}')
        if write_queues:
            w = write_queues[0]
            check(bool(r['ddr_w_last']) == (w['w_beats'] == w['len']),
                  f'W LAST mismatch at {t}')
            w['w_beats'] += 1
            if r['ddr_w_last']:
                w['last_w'] = t
                write_queues.popleft()
                finished_writes[w['id']].append(w)
    if fire(r, 'ddr_b'):
        queue = finished_writes[r['ddr_b_id']]
        check(bool(queue), f'B without completed same-fullID W at {t}')
        check(r['ddr_b_resp'] == 0, f'B error at {t}')
        if queue:
            queue.popleft()['b_handshake'] = t
    prev = r

for p in ptes:
    required = ['internal_accept', 'ds_spill_first_valid', 'ds_spill_to_delayer_accept',
                'delayed_first_valid', 'xbar_slave3_accept', 'mux_first_valid',
                'mux_to_output_spill_accept', 'ddr_accept', 'ptw_consume']
    check(all(k in p for k in required), f'incomplete phase PTE {p["ordinal"]}')
    q = next((q for q in reads if q.get('pte_ordinal') == p['ordinal']), None)
    if q is not None:
        p['ddr_first_rvalid'] = q.get('first_rvalid')
        p['ddr_first_rhandshake'] = q.get('first_rhandshake')
        p['ddr_last_rhandshake'] = q.get('last_rhandshake')
        p['ddr_held_ticks_for_matched_pte'] = q['held_ticks']
        p['matched_ddr_r_beats'] = q['returned_beats']
        check(q['returned_beats'] == 1 and q['len'] == 0,
              f'PTE DDR length mismatch {p["ordinal"]}')
    window = [r for r in rows if p['birth'] <= r['cycle'] <= p.get('ptw_consume', p['birth'])]
    p['all_interval_owner_stable'] = all(all(r[k] == p[k] for k in owner_keys) for r in window)
    p['all_interval_stage_level_stable'] = all(r['pte_stage'] == p['pte_stage'] and r['pte_level'] == p['pte_level'] for r in window)
    p['interval_ddr_held_by_full_id'] = dict(Counter(r['ddr_r_id'] for r in window if r['ddr_r_valid'] and not r['ddr_r_ready']))
    p['interval_other_ds_ddr_ar'] = [r['cycle'] for r in window if fire(r, 'ddr_ar') and r['ddr_ar_id'] == 49]
    p['phase_gaps'] = {f'{a}_to_{b}': p[b] - p[a] for a, b in [
        ('birth', 'internal_accept'), ('birth', 'ds_spill_first_valid'),
        ('ds_spill_first_valid', 'delayed_first_valid'),
        ('birth', 'delayed_first_valid'), ('delayed_first_valid', 'mux_first_valid'),
        ('mux_first_valid', 'ddr_accept'), ('birth', 'ddr_accept'),
        ('ddr_accept', 'ddr_first_rvalid'), ('ddr_first_rvalid', 'ddr_first_rhandshake'),
        ('ddr_accept', 'ptw_consume'), ('birth', 'ptw_consume')]
        if a in p and b in p and p[a] is not None and p[b] is not None}
    previous = ptes[p['ordinal'] - 2] if p['ordinal'] > 1 else None
    p['previous_pte_consume_to_birth'] = p['birth'] - previous['ptw_consume'] if previous else None
    p['same_owner_as_previous_pte'] = all(p[k] == previous[k] for k in owner_keys) if previous else None

for q in reads:
    check(q['returned_beats'] == q['len'] + 1 and 'last_rhandshake' in q,
          f'incomplete DDR read id={q["id"]} accepted={q["accepted"]}')
check(active_pte is None, 'unconsumed final PTE')
check(not any(read_queues.values()), 'outstanding final DDR reads')
check(not write_queues and not any(finished_writes.values()), 'outstanding final W/B')
by_source = {}
for ch in [0, 1]:
    rr = [q for q in reads if q['id'] >> 4 == 2 and (q['id'] >> 2 & 1) == ch]
    ww = [w for w in writes if w['id'] >> 4 == 2 and (w['id'] >> 2 & 1) == ch]
    by_source[ch] = {'AR': len(rr), 'R_beats': sum(q['returned_beats'] for q in rr),
                     'R_bytes_64bit': sum(q['returned_beats'] * 8 for q in rr),
                     'AW': len(ww), 'W_beats': sum(w['w_beats'] for w in ww),
                     'W_bytes_64bit': sum(w['w_beats'] * 8 for w in ww),
                     'B': sum('b_handshake' in w for w in ww)}

manifest = json.loads((MODEL / 'build-manifest.json').read_text())
run = json.loads((MODEL / 'run-result.json').read_text())
selected_names = ['safe_pte_stage_top.sv', 'axi_mux.sv', 'axi_demux.sv',
                  'axi_demux_simple.sv', 'axi_xbar.sv', 'axi_xbar_unmuxed.sv',
                  'rv_iommu_ds_if.sv', 'rv_iommu_ptw.sv', 'rv_iommu_ddtw.sv']
source_hashes = []
for rec in manifest['sources']:
    if Path(rec['path']).name in selected_names:
        digest = hashlib.sha256(Path(rec['path']).read_bytes()).hexdigest()
        check(digest == rec['sha256'], f'compiled source manifest mismatch {rec["path"]}')
        source_hashes.append({**rec, 'current_hash_matches_manifest': digest == rec['sha256']})
binary = Path(run['argv'][0])
binary_hash = hashlib.sha256(binary.read_bytes()).hexdigest()
check(binary_hash == run['binary_sha256'], 'current binary hash differs from actual run record')
artefacts = []
for name in ['prefix.csv', 'run.log', 'run-result.json', 'build-manifest.json',
             'build-result.json', 'safe_pte_stage_top.sv', 'safe_pte_stage_probe.cpp']:
    path = MODEL / name
    artefacts.append({'path': str(path), 'sha256': hashlib.sha256(path.read_bytes()).hexdigest()})
result = {
    'schema': 'new-passive-raw-phase-verification-v1',
    'authorship': 'Codex; one independent offline parse; root built/executed the new passive DUT run',
    'scope': 'this single CPUoff two-DMA native prefix only; no all-domain reachability claim',
    'execution': {'csv_passes': 1, 'DUT_runs_by_this_seat': 0, 'builds_by_this_seat': 0,
                  'old_validator_runs': 0, 'root_model_or_public_docs_modified': False},
    'cycle_coverage': {'first': rows[0]['cycle'], 'last': rows[-1]['cycle'], 'rows': len(rows),
                       'continuous': all(rows[i]['cycle'] + 1 == rows[i+1]['cycle'] for i in range(len(rows)-1))},
    'tap_mapping': {
        'pte_ready': 'PTW->rv_iommu_ds_if input arbitration/spill acceptance; top:1499',
        'ds_ready': 'DS_if ARspill output->axi_delayer acceptance; top:1505',
        'ds_delayed_ready': 'delayer output->xbar slave3 front ARspill; top:1509',
        'mux_ready': 'shared AR chooser->existing output ARspill; top:1514',
        'ptw_local_id': '0 from compiled rv_iommu_ptw.sv:276; not explicitly printed as an original PTE field',
        'ddtw_local_id': '1 from compiled rv_iommu_ddtw.sv:180',
        'sampling': 'probe_emit pre-edge after drive_ddr and eval, CPP:829-838,906-919; FIFO update after edge'
    },
    'counts': dict(counts), 'memq_peak': max(r['memq'] for r in rows),
    'held_DDR_R_by_full_id': dict(held_by_id),
    'mux_req_mask_histogram': dict(Counter(r['mux_reqs'] for r in rows)),
    'bytes_transactions_reconstructed_from_raw_handshakes': by_source,
    'PTE_count': len(ptes), 'PTE_events': ptes,
    'all_DDR_reads_including_old_DDTW_and_payload': reads,
    'all_DDR_writes': writes,
    'compiled_source_hash_checks': source_hashes,
    'binary_hash_matches_actual_run': binary_hash == run['binary_sha256'],
    'input_artifact_hashes': artefacts,
    'missing_fields_and_limits': [
        'Functional NEXT_ID/DONE numeric values not printed; main passes captured vID/aID into wait_done and checks byte copies, but the independent CSV cannot reproduce numeric functional IDs.',
        'DDR R data and W data/strb are absent; independent parse proves beat/byte volume and fullID/len/LAST conservation. PTE_R data is present and is checked against setup_memory table words; payload content equality is only the executed CPP bytewise assertion.',
        'No direct ar_id_cnt_full/lookup-select/in_flight counter or per-spill occupancy taps; zero observed wait is not a measurement of every credit register. Manifest-confirmed source/DDR channels, queue size and phase timing exclude an induced wait in this trace.',
        'No DDTW consumer interface, DS return ID, or complete raw DDR queue-entry dump; all accepted DDR obligations and every held DDR R are reconstructed by fullID FIFO. DDTW consumer-side final retirement relies on the root source proof and check_quiet, not a new printed ID.',
        'Build manifest argv includes the actual CPP and success log compiles it, but manifest.sources omits a CPP hash. Current CPP hash is recorded here; binary hash binds the actual run. This record does not invent a missing build-time CPP hash.',
        'No two-safe fresh origin in this prefix means no ABBA/alternative-action comparison and no phase invariance or unreachability theorem for other legal prefixes.'
    ],
    'issues': issues,
    'passed_critical_raw_checks': not issues,
}
(OUT / 'raw-phase-verification.json').write_text(json.dumps(result, ensure_ascii=False, indent=2) + '\n')
print(json.dumps({k: result[k] for k in ['cycle_coverage','counts','memq_peak','held_DDR_R_by_full_id','PTE_count','bytes_transactions_reconstructed_from_raw_handshakes','passed_critical_raw_checks','issues']}, ensure_ascii=False, indent=2))
for p in ptes:
    print(json.dumps({k:p[k] for k in ['ordinal','owner_bank','owner_did','owner_iova','owner_type','pte_stage','pte_level','pte_addr_hex','birth','internal_accept','ddr_accept','ddr_first_rvalid','ddr_first_rhandshake','ptw_consume','pte_data_matches_setup_memory','previous_pte_consume_to_birth','same_owner_as_previous_pte','phase_gaps']},ensure_ascii=False))
