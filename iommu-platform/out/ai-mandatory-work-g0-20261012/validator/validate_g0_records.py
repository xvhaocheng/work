#!/usr/bin/env python3
"""Independent finite two-rank DMA record acceptance; never imports compiler.

The frozen source shapes, slot layout, physical channels, bytes and fixture are
reconstructed here. This script reads copied/public diagnostic records only;
it never executes a DUT and never feeds observer IDs into driver admission.
"""
import argparse
import array
import csv
import hashlib
import json
import math
import shutil
import sys
from pathlib import Path


class Rejection(ValueError):
    pass


def require(condition, message):
    if not condition:
        raise Rejection(message)


def sha(data):
    return hashlib.sha256(data).hexdigest()


def digest(path):
    result = hashlib.sha256()
    with path.open('rb') as stream:
        for block in iter(lambda: stream.read(1024 * 1024), b''):
            result.update(block)
    return result.hexdigest()


SLOT_BYTES = 10 * 1024 * 1024
BACKING_BYTES = 48 * 1024 * 1024
IOVA_BASE = 0x40000000
PA_BASE = 0x84000000
COMMIT = '004ffef82b46e8dc8685c55d0cdda650bdaf1269'
EVENT_COLUMNS = ['cycle', 'event', 'job', 'role', 'channel', 'hw_id',
                 'src_iova', 'dst_iova', 'bytes', 'tensor', 'ready', 'value', 'aux']
STAT_FIELDS = ['issue', 'done', 'ar', 'aw', 'r', 'w', 'b', 'ar_bytes',
               'r_bytes', 'aw_bytes', 'w_bytes', 'errors', 'tl_accept', 'tl_resp']
JOB_EVENTS = ['ready', 'config_start', 'config_program_end', 'next_id',
              'issue', 'done', 'done_reg']


def independent_tensors():
    h = 768  # Reviewed opt_config.py:17-31,65-69, flex_opt.py:477-505.
    result, offset = [], 0
    for ti, (name, shape) in enumerate([
            ('fc1.weight', (4*h, h)), ('fc1.bias', (4*h,)),
            ('fc2.weight', (h, 4*h)), ('fc2.bias', (h,)),
            ('final_layer_norm.weight', (h,)),
            ('final_layer_norm.bias', (h,))]):
        size = math.prod(shape) * 2
        padded = (size + 4095) // 4096 * 4096
        result.append(dict(tensor=ti, name=name, shape=list(shape), bytes=size,
                           offset=offset, padded_bytes=padded))
        offset += padded
    require(sum(t['bytes'] for t in result) == 9447936, 'source useful size')
    require(offset == 9457664, 'source tensor-aligned extent')
    return result, offset


def case_contract(case_id):
    try:
        page_name, dispatch = case_id.split('-', 1)
    except ValueError as exc:
        raise Rejection('bad case identity') from exc
    require(page_name in ('4k', '2m'), 'unsupported page granularity')
    require(dispatch in ('solo0', 'solo1', 'parallel', 'single'), 'unsupported dispatcher')
    roles = [0] if dispatch == 'solo0' else [1] if dispatch == 'solo1' else [0, 1]
    return dict(id=case_id, page_bytes=4096 if page_name == '4k' else 2097152,
                dispatch=dispatch, offered_roles=roles)


def independent_jobs(case_id):
    case = case_contract(case_id)
    tensors, extent = independent_tensors()
    result = []
    for role in case['offered_roles']:
        channel = 0 if case['dispatch'] == 'single' else role
        for tensor in tensors:
            result.append(dict(id=role * 6 + tensor['tensor'], role=role,
                channel=channel, did=10 + channel,
                src_iova=IOVA_BASE + (2*role)*SLOT_BYTES + tensor['offset'],
                dst_iova=IOVA_BASE + (2*role+1)*SLOT_BYTES + tensor['offset'],
                bytes=tensor['bytes'], tensor=tensor['tensor'], ready=0))
    return case, tensors, extent, result


def independent_mapping(case):
    # Every case reserves all four slots, including inactive solo role. The
    # mapping budget is fixed across solo and pair, with distinct DIDs except
    # the trusted single-channel coordinator which owns all four slots.
    single = case['dispatch'] == 'single'
    pte_pages = (2 if single else 4) if case['page_bytes'] == 2097152 else (22 if single else 24)
    slots = []
    for slot in range(4):
        role = slot // 2
        slots.append(dict(slot=slot, role=role, kind='source' if slot % 2 == 0 else 'destination',
            iova=IOVA_BASE + slot*SLOT_BYTES, pa=PA_BASE + slot*SLOT_BYTES,
            bytes=SLOT_BYTES, did=10 if single else 10+role,
            read=True, write=bool(slot % 2), execute=False))
    return dict(page_bytes=case['page_bytes'], slot_bytes=SLOT_BYTES,
        logical_mapped_slots_bytes=4*SLOT_BYTES, actual_dense_backing_bytes=BACKING_BYTES,
        pte_pages=pte_pages, pte_reserved_bytes=pte_pages*4096,
        ddt_reserved_bytes=4096, metadata_reserved_bytes=(pte_pages+1)*4096,
        slots=slots)


def frozen_contract(root):
    selection = json.loads((root/'selection.json').read_text())
    register = json.loads((root/'g0-register.json').read_text())
    card = json.loads((root/selection['source_card']).read_text())
    require(selection['selected'] == 'distributed_two_rank_cold_MLP_operand_preparation',
            'different selected demand')
    require(selection['source_commit'] == COMMIT and selection['single_iDMA_for_both_objects_allowed'] is True,
            'source identity or ordinary consolidation changed')
    require(selection['CPU_home_not_capacity_forced'] is True and selection['no_GPU_inference_or_source_timing_claim'] is True,
            'source projection authority changed')
    require(register['stage'] == 106 and register['case_count'] == 8, 'frozen case budget')
    expected_cases = [case_contract(f'{page}-{dispatch}') for page in ('4k', '2m')
                      for dispatch in ('solo0', 'solo1', 'parallel', 'single')]
    require(register['cases'] == expected_cases, 'missing/extra/changed G0 cases')
    work = register['work']
    require(work['roles'] == 2 and work['tensor_jobs_per_role'] == 6 and
            work['useful_bytes_per_role'] == 9447936 and work['full_pair_useful_bytes'] == 18895872,
            'whole-tensor work/byte definition')
    require(work['no_page_chunk_requirement'] is True and work['ready_cycle_all_offered_jobs'] == 0,
            'hidden page chunk or postponed logical readiness')
    resources = register['resources']
    expected_resources = dict(slot_bytes=SLOT_BYTES, slot_count=4,
        logical_mapped_slots_bytes=4*SLOT_BYTES, actual_dense_backing_bytes=BACKING_BYTES,
        IOVA_base=IOVA_BASE, physical_base=PA_BASE,
        two_DID_4KiB_PTE_pages=24, two_DID_2MiB_PTE_pages=4,
        single_DID_4KiB_PTE_pages=22, single_DID_2MiB_PTE_pages=2,
        DDT_reserved_bytes=4096)
    for key, value in expected_resources.items():
        require(resources[key] == value, f'frozen resource {key}')
    config = card['configuration']
    require(config['model'] == 'facebook/opt-125m' and config['world_size'] == 2 and
            config['num_hidden_layers'] == 12 and config['hidden_size'] == 768,
            'source model or rank partition')
    require(config['partition'] == {'rank0': list(range(6)), 'rank1': list(range(6, 12))},
            'layer partition identity')
    policy = config['policy']
    require(policy['percent'] == [0, 100, 100, 0, 100, 0] and policy['pin_weight'] is True and
            policy['compress_weight'] is False and policy['sep_layer'] is True,
            'CPU-pinned full FP16 source policy')
    tensors, extent = independent_tensors()
    require(len(card['operands']) == 12, 'source not two complete packages')
    for role in (0, 1):
        own = [x for x in card['operands'] if x['rank'] == role]
        require(len(own) == 6, f'missing source rank{role} operands')
        for actual, expected in zip(own, tensors):
            require(actual['global_layer_id'] == 6*role and
                    actual['tensor_name'] == f"decoder.layers.{6*role}.{expected['name']}" and
                    actual['shape'] == expected['shape'] and actual['useful_bytes'] == expected['bytes'] and
                    actual['allocation_offset'] == expected['offset'],
                    f'source rank{role} tensor{expected["tensor"]} identity/shape/bytes')
    prepared = json.loads((root/'task-contract.json').read_text())
    require(prepared['selection_sha256'] == digest(root/'selection.json') and
            prepared['register_sha256'] == digest(root/'g0-register.json') and
            prepared['source_commit'] == COMMIT and prepared['logical_ready'] == 0,
            'prepared input is not frozen selection/register contract')
    input_hashes = {}
    for case in expected_cases:
        filename = f"commands-{case['id']}.tsv"
        keys = ['id','role','channel','src_iova','dst_iova','bytes','tensor','ready']
        lines = (root/filename).read_text().splitlines()
        require(lines and lines[0].split() == keys, f'{filename} command schema')
        actual = []
        for index,line in enumerate(lines[1:],2):
            fields = line.split()
            require(len(fields) == len(keys), f'{filename} truncated line{index}')
            actual.append(dict(zip(keys,map(int,fields))))
        expected = [{key:job[key] for key in keys} for job in independent_jobs(case['id'])[3]]
        require(actual == expected, f'{filename} differs from independent twelve whole-tensor jobs')
        input_hashes[filename] = digest(root/filename)
    source_fixtures = []
    for role in (0,1):
        filename=f'rank{role}-payload.bin'
        source_fixtures.append(validate_output(role,tensors,extent,(root/filename).read_bytes(),[0,1],guard=0x3c))
        input_hashes[filename] = digest(root/filename)
    input_hashes['task-contract.json'] = digest(root/'task-contract.json')
    return {'selection': selection, 'register': register, 'card': card,
            'case_ids': [c['id'] for c in expected_cases], 'tensors': tensors, 'extent': extent,
            'prepared_input_hashes':input_hashes,'source_fixtures':source_fixtures}


def read_events(path):
    with path.open(newline='') as stream:
        reader = csv.DictReader(stream)
        require(reader.fieldnames == EVENT_COLUMNS, 'unexpected raw CSV schema')
        result = []
        for index, row in enumerate(reader, 2):
            require(None not in row and all(v is not None for v in row.values()),
                    f'truncated event CSV row {index}')
            result.append({k: v if k == 'event' else int(v) for k, v in row.items()})
    require(result, 'empty hardware event record')
    return result


def stat_check(stat, useful, jobs, where):
    require(all(key in stat and type(stat[key]) is int and stat[key] >= 0 for key in STAT_FIELDS),
            f'{where}: missing/nonnumeric/negative counters')
    require(stat['issue'] == stat['done'] == jobs, f'{where}: real issue/DONE counts')
    require(stat['ar_bytes'] == stat['r_bytes'] == stat['aw_bytes'] == stat['w_bytes'] == useful,
            f'{where}: useful AR/R/AW/W byte conservation')
    require(stat['r']*8 == stat['w']*8 == useful, f'{where}: 64bit R/W beat counts')
    require(stat['aw'] == stat['b'] and stat['errors'] == 0, f'{where}: AW/B/error conservation')
    require(stat['tl_accept'] == stat['tl_resp'], f'{where}: translation final drain')
    if useful:
        require(stat['ar'] > 0 and stat['aw'] > 0 and stat['tl_accept'] > 0,
                f'{where}: no actual translated DMA bus work')
    else:
        require(all(stat[k] == 0 for k in STAT_FIELDS), f'{where}: inactive channel did work')


def validate_job_events(jobs, events):
    expected = {j['id']: j for j in jobs}
    per_job = {j['id']: {} for j in jobs}
    for event in events:
        require(event['cycle'] >= 0, 'negative event time')
        if event['event'] not in JOB_EVENTS:
            continue
        jid, kind = event['job'], event['event']
        require(jid in expected, f'unknown job{jid} event {kind}')
        require(kind not in per_job[jid], f'duplicate hardware {kind} job{jid}')
        for field in ('role', 'channel', 'src_iova', 'dst_iova', 'bytes', 'tensor', 'ready'):
            require(event[field] == expected[jid][field], f'job{jid} event {field} identity')
        per_job[jid][kind] = event
    for jid, records in per_job.items():
        require(set(records) == set(JOB_EVENTS),
                f'job{jid} missing events {sorted(set(JOB_EVENTS)-set(records))}')
    used_ids = [set(), set()]
    for job in jobs:
        records = per_job[job['id']]
        require(records['ready']['cycle'] == 0, f'job{job["id"]} hidden logical-ready wait')
        time = {key: records[key]['cycle'] for key in JOB_EVENTS}
        require(0 <= time['config_start'] <= time['config_program_end'] <= time['next_id'] <= time['done_reg'],
                f'job{job["id"]} config/NEXT/DONE-register order')
        require(time['config_start'] <= time['issue'] <= time['done'] <= time['done_reg'],
                f'job{job["id"]} real ISSUE/DONE order')
        hwid = records['next_id']['hw_id']
        channel = job['channel']
        require(hwid > 0 and hwid not in used_ids[channel], f'job{job["id"]} invalid/reused actual NEXT_ID')
        used_ids[channel].add(hwid)
        require(records['next_id']['value'] == hwid and records['done_reg']['hw_id'] == hwid and
                records['done_reg']['value'] == hwid, f'job{job["id"]} actual NEXT/DONE binding')
        for kind in ('issue', 'done'):
            observed = records[kind]['hw_id']
            require(observed == hwid or (observed == -1 and time[kind] <= time['next_id']),
                    f'job{job["id"]} observer-to-actual-ID binding')
    active = [None, None]
    for event in sorted((e for e in events if e['event'] in ('issue', 'done')),
                        key=lambda e: (e['cycle'], 0 if e['event'] == 'done' else 1)):
        channel, jid = event['channel'], event['job']
        require(channel in (0, 1), 'unknown physical channel')
        if event['event'] == 'issue':
            require(active[channel] is None, f'channel{channel} overlapping whole jobs')
            active[channel] = jid
        else:
            require(active[channel] == jid, f'job{jid} DONE without matching actual ISSUE')
            active[channel] = None
    require(active == [None, None], 'terminal outstanding whole jobs')
    # Frontend accepts one active whole job; refill requires actual polled DONE,
    # not merely a debug completion pulse or postponed ready timestamp.
    for channel in (0, 1):
        previous_done_reg = 0
        for index,job in enumerate(j for j in jobs if j['channel'] == channel):
            records = per_job[job['id']]
            require(records['next_id']['hw_id'] == index+2,
                    f'channel{channel} real NEXT/DONE does not follow reset DONE1/NEXT2')
            require(records['config_start']['cycle'] >= previous_done_reg,
                    f'job{job["id"]} refill before actual polled DONE')
            previous_done_reg = records['done_reg']['cycle']
    return per_job


def validate_output(role, tensors, extent, data, offered, guard=0xd9):
    require(len(data) == extent, f'rank{role} raw output extent')
    if role not in offered:
        require(data == bytes([guard])*extent, f'inactive rank{role} raw guard modified')
        return {'role': role, 'offered': False, 'useful_bytes': 0,
                'full_inactive_extent_guard': True, 'output_sha256': sha(data)}
    cursor, hashes = 0, []
    useful = hashlib.sha256()
    for tensor in tensors:
        offset, size = tensor['offset'], tensor['bytes']
        require(data[cursor:offset] == bytes([guard])*(offset-cursor),
                f'rank{role} tensor{tensor["tensor"]} preceding hole changed')
        words = array.array('Q', (((role+1) << 60) ^ (tensor['tensor'] << 52) ^ index
                                for index in range(size//8)))
        if sys.byteorder != 'little':
            words.byteswap()
        expected = words.tobytes()
        actual = data[offset:offset+size]
        require(actual == expected, f'rank{role} complete tensor{tensor["tensor"]} payload differs')
        useful.update(actual)
        hashes.append({'tensor': tensor['tensor'], 'name': tensor['name'], 'bytes': size,
                       'sha256': sha(actual)})
        cursor = offset + size
    require(data[cursor:] == bytes([guard])*(extent-cursor), f'rank{role} final hole changed')
    return {'role': role, 'offered': True, 'useful_bytes': 9447936,
            'all_six_complete_tensors': True, 'all_extent_holes_preserved': True,
            'useful_sha256': useful.hexdigest(), 'output_sha256': sha(data), 'tensors': hashes}


def write_events(path, events):
    with path.open('w', newline='') as stream:
        writer = csv.DictWriter(stream, fieldnames=EVENT_COLUMNS)
        writer.writeheader()
        writer.writerows(events)


def metadata_pages(mapping, blob):
    require(len(blob) == mapping['metadata_reserved_bytes'], 'raw DDT/PT metadata file extent')
    pages, offsets = {}, set()
    descriptors = mapping['metadata_pages']
    require(len(descriptors)*4096 == len(blob), 'metadata page record count')
    for descriptor in descriptors:
        pa, offset = descriptor['pa'], descriptor['file_offset']
        require(type(pa) is int and pa % 4096 == 0 and pa not in pages,
                'duplicate/unaligned metadata PA')
        require(descriptor['bytes'] == 4096 and offset % 4096 == 0 and
                0 <= offset < len(blob) and offset not in offsets, 'metadata file page gap/overlap')
        require(descriptor['kind'] in ('ddt', 'root', 'l1', 'l0'), 'unknown metadata page kind')
        offsets.add(offset)
        chunk = array.array('Q')
        chunk.frombytes(blob[offset:offset+4096])
        if sys.byteorder != 'little':
            chunk.byteswap()
        pages[pa] = {'words': chunk, 'kind': descriptor['kind'], 'role': descriptor['role']}
    require(offsets == set(range(0, len(blob), 4096)), 'metadata file page coverage')
    return pages


def walk_slot_pages(pages, root_pa, iova, leaf_level):
    current = root_pa
    visited, path = set(), []
    for level in (2, 1, 0):
        require(current in pages and pages[current]['kind'] == {2:'root', 1:'l1', 0:'l0'}[level],
                'Sv39 page-walk page ownership/level')
        require(current not in visited, 'cyclic translation page table')
        visited.add(current)
        index = (iova >> (12 + level*9)) & 511
        word = pages[current]['words'][index]
        path.append((current, index))
        if word & 0xe:
            require(level == leaf_level, 'unexpected superpage/leaf level')
            return word, path
        require(level > leaf_level and word & 0x3ff == 1, 'missing/invalid nonleaf PTE')
        current = (word >> 10) << 12
    raise Rejection('no leaf PTE')


def validate_mapping(case, mapping, metadata):
    expected = independent_mapping(case)
    require(mapping['page_bytes'] == expected['page_bytes'] and
            mapping['pte_pages'] == expected['pte_pages'] and
            mapping['metadata_reserved_bytes'] == expected['metadata_reserved_bytes'] and
            mapping['logical_mapped_bytes'] == 4*SLOT_BYTES and
            mapping['payload_reserved_bytes'] == BACKING_BYTES and
            mapping['pte_reserved_bytes'] == expected['pte_reserved_bytes'] and
            mapping['ddt_reserved_bytes'] == 4096 and mapping['ddt_base'] == 0x10000 and
            mapping['diagnostic_metadata_only'] is True,
            'frozen mapping/page-table footprint')
    require(len(mapping['slots']) == 4, 'missing source/destination slot permissions')
    for actual, wanted in zip(mapping['slots'], expected['slots']):
        for key in ('slot', 'role', 'iova', 'pa', 'bytes', 'did'):
            require(actual[key] == wanted[key], f'slot{wanted["slot"]} mapping {key}')
        require(actual['kind'] == wanted['kind'] and actual['pte_flags'] ==
                (0xd3 if wanted['kind'] == 'source' else 0xd7),
                f'slot{wanted["slot"]} read/write/no-exec permission')
    pages = metadata_pages(mapping, metadata)
    require(0x10000 in pages and pages[0x10000]['kind'] == 'ddt', 'DDT physical base')
    require(sum(p['kind'] == 'ddt' for p in pages.values()) == 1 and
            sum(p['kind'] != 'ddt' for p in pages.values()) == expected['pte_pages'],
            'raw DDT/PTE page counts')
    roots = {0:0x20000, 1:0x30000}
    single = case['dispatch'] == 'single'
    require({pa for pa,p in pages.items() if p['kind'] == 'root'} ==
            ({roots[0]} if single else set(roots.values())), 'raw PT root physical allocations')
    reached, all_leaves = set(), {}
    step = case['page_bytes']
    leaf_level = 1 if step == 2097152 else 0
    for slot in expected['slots']:
        owner = 0 if single else slot['role']
        root_pa = roots[owner]
        for offset in range(0, SLOT_BYTES, step):
            iova, pa = slot['iova']+offset, slot['pa']+offset
            require(iova % step == 0 and pa % step == 0, 'actual leaf alignment')
            word, path = walk_slot_pages(pages, root_pa, iova, leaf_level)
            flag = 0xd3 if slot['kind'] == 'source' else 0xd7
            require(word == ((pa >> 12) << 10) | flag,
                    f'slot{slot["slot"]} raw leaf PA/permissions at offset{offset}')
            for location in path:
                reached.add(location)
            all_leaves[path[-1]] = word
    # Every nonzero PTE must be a required path/leaf. This rejects hidden extra
    # aliases, executable data, foreign role access and invalid guard mappings.
    nonzero = {(pa,index) for pa,page in pages.items() if page['kind'] != 'ddt'
               for index,word in enumerate(page['words']) if word}
    require(nonzero == reached, 'extra/missing raw translation mappings or unauthorized role aliases')
    def inventory(pa, level, prefix, ancestors):
        require(pa not in ancestors and pa in pages and
                pages[pa]['kind'] == {2:'root',1:'l1',0:'l0'}[level],
                'invalid/cyclic raw per-DID page-table inventory')
        result = {}
        for index,word in enumerate(pages[pa]['words']):
            if not word:
                continue
            address = prefix | (index << (12+level*9))
            if word & 0xe:
                require(level == leaf_level, 'unexpected per-DID leaf level')
                require(address not in result, 'duplicate per-DID virtual leaf')
                result[address] = word
            else:
                require(level > leaf_level and word & 0x3ff == 1, 'invalid per-DID nonleaf PTE')
                child = inventory((word >> 10) << 12,level-1,address,ancestors | {pa})
                require(not (result.keys() & child.keys()), 'per-DID alias path')
                result.update(child)
        return result
    for owner in ([0] if single else [0,1]):
        permitted = {}
        for slot in expected['slots']:
            if single or slot['role'] == owner:
                for offset in range(0,SLOT_BYTES,step):
                    flag = 0xd3 if slot['kind'] == 'source' else 0xd7
                    permitted[slot['iova']+offset] = (((slot['pa']+offset) >> 12) << 10) | flag
        require(inventory(roots[owner],2,0,set()) == permitted,
                f'DID{10+owner} raw reachable leaves include foreign role, alias or guard mapping')
    leaves, nonleaves = len(all_leaves), len(reached)-len(all_leaves)
    require(mapping['leaf_pte_count'] == leaves == 4*SLOT_BYTES//step,
            'leaf count differs from complete mapped slots')
    expected_nonleaf = (1 if single else 2) if leaf_level == 1 else (21 if single else 22)
    require(mapping['nonleaf_pte_count'] == nonleaves == expected_nonleaf,
            'nonleaf count differs from independently walked tables')
    require(mapping['pte_init_write64_count'] == expected['pte_pages']*512+leaves+nonleaves,
            'page table setup writes not counted')
    ddt = pages[0x10000]['words']
    required_ddt = [0]*512
    for channel in ([0] if single else [0, 1]):
        index = (10+channel)*8  # Current extended 64-byte device context ABI.
        required_ddt[index] = 1
        required_ddt[index+2] = (10+channel) << 12
        required_ddt[index+3] = (8 << 60) | (roots[channel] >> 12)
    require(list(ddt) == required_ddt, 'actual DDT DID/PSCID/root binding or extra valid context')
    require(mapping['ddt_init_write64_count'] == 512 + (3 if single else 6),
            'DDT zeroing/context setup writes not counted')
    require(mapping['did11_valid'] is (not single) and
            mapping['init_write64_count'] == mapping['pte_init_write64_count']+mapping['ddt_init_write64_count'] and
            mapping['metadata_initialized_unique_bytes'] == len(metadata), 'metadata setup/ownership accounting')
    return {'raw_metadata_sha256':sha(metadata), 'raw_leaf_PA_permissions_checked':True,
            'actual_DID_PSCID_root_binding_checked':True,
            'unexpected_aliases_or_extra_guard_mappings':False, 'page_bytes':step,
            'leaf_count':leaves, 'nonleaf_count':nonleaves,
            'pte_pages':expected['pte_pages'], 'metadata_reserved_bytes':len(metadata),
            'single_DID_consolidation':single}


def cfg_sum(records):
    keys = ('writes', 'reads', 'write_cycles', 'read_cycles', 'next_reads', 'done_reads')
    result = {key:0 for key in keys}
    for stat in records:
        require(all(type(stat.get(key)) is int and stat[key] >= 0 for key in keys),
                'missing/nonnumeric per-job actual configuration counters')
        require(stat['next_reads'] == 1 and stat['done_reads'] >= 1 and
                stat['reads'] == stat['next_reads']+stat['done_reads'] and
                stat['writes'] == 4 and stat['write_cycles'] > 0 and stat['read_cycles'] > 0,
                'per-job configuration/NEXT/DONE polling accounting')
        for key in keys:
            result[key] += stat[key]
    return result


def validate_bus(jobs, per_job, summary_jobs, events, metadata_page_info):
    expected = {j['id']:j for j in jobs}
    raw = {j['id']:{kind:[] for kind in ('ddr_ar', 'ddr_aw', 'ddr_b', 'tl_accept', 'tl_resp')}
           for j in jobs}
    metadata_reads = []
    for event in events:
        kind = event['event']
        if kind == 'metadata_ar':
            require(event['value'] > 0 and event['value']%8 == 0,
                    'nonpositive/unaligned metadata DDR burst')
            pa = event['aux']
            require(any(page['pa'] <= pa and pa+event['value'] <= page['pa']+4096
                        for page in metadata_page_info), 'DDR metadata fetch outside actual DDT/PT pages')
            metadata_reads.append(event)
            continue
        if kind not in ('ddr_ar', 'ddr_aw', 'ddr_b', 'tl_accept', 'tl_resp'):
            continue
        jid = event['job']
        require(jid in expected, f'unknown job{jid} raw {kind}')
        for field in ('role', 'channel', 'src_iova', 'dst_iova', 'bytes', 'tensor', 'ready'):
            require(event[field] == expected[jid][field], f'job{jid} raw {kind} {field}')
        require(per_job[jid]['issue']['cycle'] <= event['cycle'] <= per_job[jid]['done']['cycle'],
                f'job{jid} raw {kind} outside actual ISSUE/DONE')
        raw[jid][kind].append(event)
    for job in jobs:
        jid, channel = job['id'], job['channel']
        bus = summary_jobs[jid]['bus']
        stat_check(bus, job['bytes'], 1, f'job{jid}')
        for kind, stat_name, field in [('ddr_ar', 'ar', 'src_iova'), ('ddr_aw', 'aw', 'dst_iova')]:
            start = PA_BASE + job[field]-IOVA_BASE
            cursor = start
            headers = sorted(raw[jid][kind], key=lambda e:e['aux'])
            require(len(headers) == bus[stat_name], f'job{jid} raw {kind} count')
            for header in headers:
                require(header['aux'] == cursor and header['value'] > 0 and header['value']%8 == 0,
                        f'job{jid} {kind} physical coverage gap/overlap')
                cursor += header['value']
            require(cursor == start+job['bytes'], f'job{jid} {kind} incomplete useful byte coverage')
        replies = raw[jid]['ddr_b']
        require(len(replies) == bus['b'] and all(e['value'] == 0 for e in replies),
                f'job{jid} real DDR B count/errors')
        require(all(((e['aux'] >> 4) & 3) == 2 and ((e['aux'] >> 2) & 1) == channel for e in replies),
                f'job{jid} actual DDR B physical requester identity')
        for kind in ('tl_accept', 'tl_resp'):
            points = raw[jid][kind]
            require(len(points) == bus[kind] and [e['value'] for e in points] == list(range(1, len(points)+1)),
                    f'job{jid} raw translation handshake count/sequence')
            require(all(e['aux'] == channel for e in points), f'job{jid} raw TL physical bank identity')
    return raw, metadata_reads


def validate_case_objects(root, run, context, summary, events, outputs, metadata):
    case_id = summary['case_id']
    require(case_id in context['case_ids'] and run.name == case_id,
            'case record identity does not match frozen case or directory')
    case, tensors, extent, jobs = independent_jobs(case_id)
    require(summary['schema'] == 'ai-mandatory-transfer-g0-v1' and summary['pass'] is True,
            'not a completed actual bounded G0 record')
    for field in ('page_bytes', 'dispatch', 'offered_roles'):
        require(summary[field] == case[field], f'case summary {field}')
    require(summary['useful_bytes'] == sum(j['bytes'] for j in jobs) and
            summary['payload_extent_bytes'] == extent, 'summary useful bytes or padded extent')
    require(summary['cpu_enable'] == summary['policy_enable'] == summary['nonblock_enable'] == 0,
            'unregistered CPU/priority/nonblock policy active')
    require(summary['mapping']['diagnostic_metadata_only'] is True and summary['ptw_latency'] == 1,
            'frozen diagnostic memory/translation settings changed')
    require(type(summary['common_t0_absolute_cycle']) is int and summary['common_t0_absolute_cycle'] >= 0,
            'missing absolute common release')
    require(summary['scope'] == 'cold_independent_operand_transfer_stage_FPGA_projection_only' and
            summary['priority'] is False and summary['read_admit'] == 0 and summary['warmup'] == 0 and
            summary['STOP_used'] is False and summary['board_costs_measured'] is False and
            summary['trained_weights'] is False and summary['GPU_inference_run'] is False,
            'unregistered mechanism, initial state or claim authority')
    kinds = set(JOB_EVENTS + ['common_start', 'role_complete', 'other_role_completed',
        'all_hardware_done', 'natural_quiet', 'validation_end', 'ddr_ar', 'ddr_aw',
        'ddr_b', 'metadata_ar', 'tl_accept', 'tl_resp'])
    require(all(e['event'] in kinds for e in events), 'unknown hardware event kind')
    common = [e for e in events if e['event'] == 'common_start']
    require(len(common) == 1 and common[0]['cycle'] == 0 and
            common[0]['value'] == sum(j['bytes'] for j in jobs) and common[0]['aux'] == len(jobs),
            'missing/duplicate/nonzero common release or offered-work metadata')
    per_job = validate_job_events(jobs, events)
    expected_jobs = {job['id']:job for job in jobs}
    for event in events:
        if event['event'] in ('role_complete','other_role_completed'):
            require(event['job'] in expected_jobs and event['role'] in case['offered_roles'],
                    'role progress marker belongs to inactive/unknown work')
            job = expected_jobs[event['job']]
            for field in ('role','channel','src_iova','dst_iova','bytes','tensor','ready'):
                require(event[field] == job[field], f'role progress marker mismatched {field}')
    require(len(summary['jobs']) == len(jobs) and [j['id'] for j in summary['jobs']] == [j['id'] for j in jobs],
            'summary whole-tensor job identity/count/order')
    summaries = {j['id']:j for j in summary['jobs']}
    channel_done_reg = [0, 0]
    for expected in jobs:
        jid, channel = expected['id'], expected['channel']
        actual, raw = summaries[jid], per_job[jid]
        for field, value in expected.items():
            require(actual[field] == value, f'job{jid} summary {field}')
        for field, kind in [('ready', 'ready'), ('config_start', 'config_start'),
                            ('config_program_end', 'config_program_end'), ('next_cycle', 'next_id'),
                            ('issue', 'issue'), ('done', 'done'), ('done_reg_cycle', 'done_reg')]:
            require(actual[field] == raw[kind]['cycle'], f'job{jid} summary/raw {field}')
        require(actual['next_id'] == raw['next_id']['hw_id'] == actual['done_id'] == raw['done_reg']['hw_id'],
                f'job{jid} summary/raw reserved NEXT/DONE ID')
        require(actual['channel_ready'] == channel_done_reg[channel], f'job{jid} physical channel-ready hidden delay')
        channel_done_reg[channel] = raw['done_reg']['cycle']
    mapping_result = validate_mapping(case, summary['mapping'], metadata)
    raw_bus, metadata_reads = validate_bus(jobs, per_job, summaries, events, summary['mapping']['metadata_pages'])
    accumulated = [{key:0 for key in STAT_FIELDS} for _ in range(2)]
    for job in jobs:
        for key in STAT_FIELDS:
            accumulated[job['channel']][key] += summaries[job['id']]['bus'][key]
    require(len(summary['channels']) == 2 and len(summary['roles']) == 2,
            'missing physical channels/logical roles')
    for channel in (0, 1):
        actual = summary['channels'][channel]
        # channels[] owns bus counters; logical roles stay separate in single.
        counter = actual['bus'] if 'bus' in actual else actual
        nbytes = sum(j['bytes'] for j in jobs if j['channel'] == channel)
        nissue = sum(j['channel'] == channel for j in jobs)
        stat_check(counter, nbytes, nissue, f'physical channel{channel}')
        require(all(counter[k] == accumulated[channel][k] for k in STAT_FIELDS),
                f'physical channel{channel} aggregate differs from actual jobs')
        expected_cfg = cfg_sum([summaries[j['id']]['cfg'] for j in jobs if j['channel'] == channel])
        require(actual['channel'] == channel and actual['did'] == 10+channel,
                f'physical channel{channel} DID identity')
        for key,value in expected_cfg.items():
            if key == 'writes':
                require(actual['cfg'][key] == value+1, f'physical channel{channel} disable-write accounting')
            elif key == 'write_cycles':
                require(actual['cfg'][key] > value, f'physical channel{channel} disable-write cycle cost omitted')
            else:
                require(actual['cfg'][key] == value, f'physical channel{channel} configuration {key} cost')
    cfg_total = cfg_sum([j['cfg'] for j in summary['jobs']])
    require(summary['disable_writes'] == 2 and summary['cfg']['writes'] == cfg_total['writes']+2 and
            summary['cfg']['write_cycles'] > cfg_total['write_cycles'], 'total disable-write setup cost omitted')
    require(all(summary['cfg'][key] == sum(channel['cfg'][key] for channel in summary['channels'])
                for key in cfg_total), 'total configuration cost differs from both physical channels')
    role_results = []
    for role in (0, 1):
        own = [j for j in jobs if j['role'] == role]
        offered = bool(own)
        result = summary['roles'][role]
        require(result['role'] == role and result['offered'] is offered and
                result['completed_tensors'] == len(own) and result['completed_useful_bytes'] == sum(j['bytes'] for j in own),
                f'rank{role} complete useful work accounting')
        role_bus = {key:sum(summaries[j['id']]['bus'][key] for j in own) for key in STAT_FIELDS}
        require(all(result['bus'][key] == value for key,value in role_bus.items()),
                f'rank{role} logical bus != owned jobs')
        done = max((per_job[j['id']]['done']['cycle'] for j in own), default=-1)
        polled = max((per_job[j['id']]['done_reg']['cycle'] for j in own), default=-1)
        require(result['hardware_done_cycle'] == done and result['polled_done_cycle'] == polled,
                f'rank{role} completion time from raw DONE')
        role_events = [e for e in events if e['event'] == 'role_complete' and e['role'] == role]
        progress_events = [e for e in events if e['event'] == 'other_role_completed' and e['role'] == role]
        require(len(result['tensors']) == 6, f'rank{role} incomplete tensor specification')
        for actual,wanted in zip(result['tensors'],tensors):
            for key in ('tensor','name','offset','bytes'):
                require(actual[key] == wanted[key], f'rank{role} tensor{wanted["tensor"]} metadata {key}')
        if offered:
            require(len(result['tensors']) == 6, f'rank{role} missing tensor completion')
            for ti, job in enumerate(own):
                tensor = result['tensors'][ti]
                require(tensor['bytes'] == job['bytes'] and tensor['done'] == per_job[job['id']]['done']['cycle'] and
                        tensor['done_reg_cycle'] == per_job[job['id']]['done_reg']['cycle'],
                        f'rank{role} tensor{ti} complete bytes/raw DONE')
            require(len(role_events) == len(progress_events) == 1 and role_events[0]['cycle'] == done and
                    role_events[0]['value'] == 9447936 and role_events[0]['aux'] == 6,
                    f'rank{role} unique raw full-role completion')
            other = 1-role
            other_progress = sum(j['bytes'] for j in jobs if j['role'] == other and
                                 per_job[j['id']]['done']['cycle'] <= done)
            require(progress_events[0]['cycle'] == done and progress_events[0]['value'] == other_progress and
                    progress_events[0]['aux'] == other and
                    result['other_role_completed_useful_bytes_at_hardware_done'] == other_progress,
                    f'rank{role} other-role useful progress hides same-start work')
        else:
            require(not role_events and not progress_events and len(result['tensors']) == 6 and
                    all(t['done'] == t['done_reg_cycle'] == -1 for t in result['tensors']) and
                    result['other_role_completed_useful_bytes_at_hardware_done'] == 0,
                    f'inactive rank{role} claimed completion')
        output_result = validate_output(role, tensors, extent, outputs[role], case['offered_roles'])
        output_result.update(hardware_done_cycle=done, polled_done_cycle=polled)
        role_results.append(output_result)
    joint_cycle = max(per_job[j['id']]['done']['cycle'] for j in jobs)
    join = [e for e in events if e['event'] == 'all_hardware_done']
    quiet = [e for e in events if e['event'] == 'natural_quiet']
    ended = [e for e in events if e['event'] == 'validation_end']
    offered_bytes = sum(j['bytes'] for j in jobs)
    require(len(join) == 1 and join[0]['cycle'] == joint_cycle and
            join[0]['value'] == offered_bytes and join[0]['aux'] == len(jobs),
            'missing/early/duplicate full offered-work DONE join')
    require(len(quiet) == 1 and quiet[0]['cycle'] >= joint_cycle and
            quiet[0]['value'] == 3 and quiet[0]['aux'] == 0,
            'not naturally quiet after all actual hardware work complete')
    require(len(ended) == 1 and ended[0]['cycle'] >= max(quiet[0]['cycle'],max(channel_done_reg)) and
            ended[0]['value'] == 64 and ended[0]['aux'] == 0,
            'validation ended before natural drain/actual polled completion or quiet confirmation absent')
    require(summary['quiet_cycle'] == quiet[0]['cycle'], 'summary/raw natural quiet time')
    require(summary['natural_quiet_cycle'] == quiet[0]['cycle'] and
            summary['all_hardware_done_cycle'] == joint_cycle and summary['validation_end_cycle'] == ended[0]['cycle'],
            'summary/raw hardware join/quiet/validation time')
    first_role = min(case['offered_roles'],key=lambda role:(summary['roles'][role]['hardware_done_cycle'],role))
    require(summary['first_complete_role'] == first_role and
            summary['other_role_completed_useful_bytes_at_first_complete'] ==
            summary['roles'][first_role]['other_role_completed_useful_bytes_at_hardware_done'],
            'first-complete other-role useful progress')
    terminal = {'dma_job_quiet':3, 'tl_in_flight':0, 'cpu_quiet':1, 'cpu_wr_quiet':1,
        'cpu_issued':0, 'cpu_completed':0, 'memory_r_queue':0, 'memory_aw_queue':0,
        'memory_b_queue':0, 'observer_r_queue':0, 'observer_aw_queue':0,
        'observer_b_queue':0, 'ddr_valid':0}
    require(summary['terminal'] == terminal, 'DMA/TL/DDR/observer queues not finally drained')
    require(summary['launch_errors'] == 0 and summary['stability_samples'] > 0,
            'actual launch error or missing protocol stability check')
    for key in ('metadata_unchanged', 'source_match', 'destination_match', 'padding_preserved',
                'inactive_destination_preserved', 'slot_tail_guard_preserved', 'backing_guard_preserved'):
        require(summary[key] is True, f'driver full initialized-memory invariant {key}')
    require(summary['guards_checked_bytes'] == BACKING_BYTES and summary['output_includes_extent_holes'] is True,
            'full allocated guard cost/output coverage not declared')
    global_bus = summary['all_ddr']
    require(all(type(global_bus.get(k)) is int and global_bus[k] >= 0 for k in STAT_FIELDS),
            'global DDR counter schema')
    require(global_bus['ar_bytes'] == global_bus['r_bytes'] == global_bus['r']*8 and
            global_bus['aw_bytes'] == global_bus['w_bytes'] == global_bus['w']*8 and
            global_bus['aw'] == global_bus['b'] and global_bus['errors'] == 0,
            'global DDR byte/request/response/error conservation')
    for key in ('aw', 'w', 'b', 'aw_bytes', 'w_bytes'):
        require(global_bus[key] == sum(a[key] for a in accumulated), f'unexpected global payload write {key}')
    require(global_bus['ar_bytes'] == offered_bytes+sum(e['value'] for e in metadata_reads),
            'global raw payload+metadata AR bytes')
    require(global_bus['ar'] == sum(a['ar'] for a in accumulated)+len(metadata_reads),
            'global raw payload+metadata AR count')
    require(summary['metadata_traffic'] == {'ar':len(metadata_reads),
            'ar_bytes':sum(e['value'] for e in metadata_reads),'r_bytes':sum(e['value'] for e in metadata_reads)},
            'summary/raw actual metadata traffic')
    decompositions = []
    for job in jobs:
        records = per_job[job['id']]
        decompositions.append({'job':job['id'], 'role':job['role'], 'channel':job['channel'],
            'ready':0, 'ready_to_issue':records['issue']['cycle'],
            'issue_to_hardware_done':records['done']['cycle']-records['issue']['cycle'],
            'hardware_done_to_polled_DONE':records['done_reg']['cycle']-records['done']['cycle'],
            'ready_to_polled_DONE':records['done_reg']['cycle']})
    return {'schema':'independent-ai-mandatory-g0-case-acceptance-v1', 'accepted':True,
        'case_id':case_id, 'classification':'actual-iDMA RTL finite transfer-stage G0; no GPU/board inference result',
        'offered_roles':case['offered_roles'], 'full_pair':case['offered_roles'] == [0,1],
        'dispatch':case['dispatch'], 'page_bytes':case['page_bytes'], 'whole_tensor_jobs':len(jobs),
        'offered_useful_bytes':offered_bytes, 'all_offered_jobs_logically_ready_at_zero':True,
        'real_issue_DONE_and_actual_polled_ID_binding':True, 'natural_drain_cycle':quiet[0]['cycle'],
        'all_offered_hardware_done_cycle':joint_cycle, 'roles':role_results,
        'job_time_decomposition':decompositions, 'mapping':mapping_result,
        'metadata_read_bytes':sum(e['value'] for e in metadata_reads), 'configuration_cost':summary['cfg'],
        'bus_check_boundary':'independent raw AR/AW/B and TL handshake checks; per-beat R/W totals use actual runtime observer counters',
        'guard_check_boundary':'raw full rank extent including inactive role/hole bytes checked independently; remaining 10MiB slot tails,8MiB backing guard,source preservation use driver full-memory assertions',
        'authority':'new mapped direct two-object projection and ordinary baselines only; no native shared-GPU-IOMMU, source timing, throughput, PTW-only attribution or net board claim',
        'G1_candidate_accepted':False,
        'input_hashes':{name:digest(run/name) for name in ('events.csv','summary.json',
            'rank0-output.bin','rank1-output.bin','metadata.bin')}}


def hardware_report(root, run, context=None):
    if context is None:
        context = frozen_contract(root)
    summary = json.loads((run/'summary.json').read_text())
    return validate_case_objects(root, run, context, summary, read_events(run/'events.csv'),
        [(run/f'rank{role}-output.bin').read_bytes() for role in (0,1)], (run/'metadata.bin').read_bytes())


def static_report(root):
    context = frozen_contract(root)
    return {'schema':'independent-ai-mandatory-g0-static-acceptance-v1', 'accepted':True,
        'classification':'independent source/job/role/layout derivation only; no actual case accepted',
        'source_commit':COMMIT, 'full_pair_jobs':12, 'full_pair_useful_bytes':18895872,
        'extent_per_rank':context['extent'], 'slot_bytes':SLOT_BYTES,
        'mapped_payload_bytes':4*SLOT_BYTES, 'dense_backing_bytes':BACKING_BYTES,
        'tensors':context['tensors'],
        'all_eight_command_files_match_independent_jobs':True,
        'both_source_fixture_bytes_and_holes_checked':True,
        'prepared_input_hashes':context['prepared_input_hashes'],
        'cases':[{'case':case_contract(c), 'jobs':independent_jobs(c)[3],
                  'mapping':independent_mapping(case_contract(c))} for c in context['case_ids']],
        'actual_RTL_cases_accepted':False,
        'input_hashes':{name:digest(root/name) for name in ('selection.json','g0-register.json',
                     context['selection']['source_card'])}}


def negative_report(root, run, output):
    context = frozen_contract(root)
    baseline = hardware_report(root, run, context)
    inputs = ['events.csv','summary.json','rank0-output.bin','rank1-output.bin','metadata.bin']
    original_hashes = {name:digest(run/name) for name in inputs}
    first_role = baseline['offered_roles'][0]
    first_job = 6*first_role
    variants = []
    for name in ('missing_done','duplicate_done','wrong_role','wrong_channel','payload_corruption'):
        folder = output/'negative-records'/name/run.name
        folder.mkdir(parents=True,exist_ok=True)
        for filename in inputs:
            shutil.copyfile(run/filename,folder/filename)
        events = read_events(folder/'events.csv')
        if name == 'missing_done':
            events = [e for e in events if not (e['event']=='done' and e['job']==first_job)]
        elif name == 'duplicate_done':
            events.append(dict(next(e for e in events if e['event']=='done' and e['job']==first_job)))
        elif name in ('wrong_role','wrong_channel'):
            event = next(e for e in events if e['event']=='issue' and e['job']==first_job)
            field = 'role' if name == 'wrong_role' else 'channel'
            event[field] = 1-event[field]
        else:
            target = folder/f'rank{first_role}-output.bin'
            with target.open('r+b') as stream:
                byte=stream.read(1);stream.seek(0);stream.write(bytes([byte[0]^1]))
        write_events(folder/'events.csv',events)
        try:
            hardware_report(root,folder,context)
        except (Rejection,KeyError,ValueError,OSError) as exc:
            variants.append({'name':name,'rejected':True,'reason':str(exc),'record_copy':str(folder)})
        else:
            variants.append({'name':name,'rejected':False,'reason':'damaged record accepted','record_copy':str(folder)})
    require({name:digest(run/name) for name in inputs} == original_hashes, 'negative generator changed originals')
    require(all(v['rejected'] for v in variants), 'negative checker accepted damaged records')
    return {'schema':'independent-ai-mandatory-g0-negative-acceptance-v1','accepted':True,
        'classification':'necessary record/byte mutations only; no new DUT run',
        'baseline_case':baseline['case_id'],'variants':variants,'originals_unchanged':True,
        'original_hashes':original_hashes}


def suite_report(root, runs):
    context = frozen_contract(root)
    result = [hardware_report(root,runs/case_id,context) for case_id in context['case_ids']]
    return {'schema':'independent-ai-mandatory-g0-suite-acceptance-v1','accepted':True,
        'classification':'eight finite actual-iDMA G0 records; four full-pair and four isolated cases',
        'case_count':8,'full_pair_cases':[x['case_id'] for x in result if x['full_pair']],
        'isolated_cases':[x['case_id'] for x in result if not x['full_pair']], 'cases':result,
        'solo_is_never_full_pair':True,'G1_candidate_accepted':False}


def main():
    parser=argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--root',type=Path,default=Path(__file__).resolve().parent.parent)
    parser.add_argument('--static',action='store_true')
    parser.add_argument('--run',type=Path)
    parser.add_argument('--suite',type=Path)
    parser.add_argument('--negative-cases',action='store_true')
    parser.add_argument('--report',type=Path,required=True)
    args=parser.parse_args()
    try:
        require(sum([args.static,bool(args.run),bool(args.suite)])==1, 'select --static, --run or --suite')
        require(not args.negative_cases or args.run, 'negative copies require actual case')
        require(args.report.resolve().is_relative_to(Path(__file__).resolve().parent),
                'validator writes confined to exclusive validator directory')
        if args.static:
            report=static_report(args.root)
        elif args.negative_cases:
            report=negative_report(args.root,args.run,args.report.parent)
        elif args.suite:
            report=suite_report(args.root,args.suite)
        else:
            report=hardware_report(args.root,args.run)
    except (Rejection,KeyError,ValueError,OSError) as exc:
        report={'accepted':False,'reason':str(exc)}
    args.report.parent.mkdir(parents=True,exist_ok=True)
    args.report.write_text(json.dumps(report,ensure_ascii=False,indent=2)+'\n')
    print(json.dumps({k:report[k] for k in ('accepted','classification','reason') if k in report},ensure_ascii=False))
    return 0 if report['accepted'] else 1


if __name__=='__main__':
    raise SystemExit(main())
