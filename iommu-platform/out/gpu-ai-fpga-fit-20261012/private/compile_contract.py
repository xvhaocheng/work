#!/usr/bin/env python3
"""Compile a source-derived transfer contract; never supply application timing.

The source is a complete OPT125m MLP operand package. Bytes are test patterns,
not trained parameters. Two iDMAs represent fill/copy roles in a defined model;
they do not implement an SSD, CUDA, GPU arithmetic, or its measured throughput.
"""
import hashlib
import json
import struct
from pathlib import Path

OUT = Path(__file__).resolve().parent.parent
PAGE = 4096
REGION = 16 * 1024 * 1024
SOURCE, RELAY, DEST = 0x84000000, 0x85000000, 0x86000000
SRC_IOVA, RELAY_IOVA, DST_IOVA = 0x40000000, 0x50000000, 0x60000000


def align(n):
    return (n + PAGE - 1) // PAGE * PAGE


def compile_packet():
    h = 768  # fixed supported source config, not a tuned compute parameter
    specs = [('fc1.weight', (4*h, h)), ('fc1.bias', (4*h,)),
             ('fc2.weight', (h, 4*h)), ('fc2.bias', (h,)),
             ('final_layer_norm.weight', (h,)),
             ('final_layer_norm.bias', (h,))]
    tensors, chunks, offset = [], [], 0
    for ti, (name, shape) in enumerate(specs):
        elements = 1
        for dim in shape:
            elements *= dim
        size = elements * 2
        tensors.append(dict(name=name, shape=shape, dtype='FP16 bit representation',
                            bytes=size, offset=offset))
        for local in range(0, size, PAGE):
            chunks.append(dict(index=len(chunks), tensor=ti,
                               offset=offset+local, bytes=min(PAGE, size-local)))
        offset += align(size)
    assert offset <= REGION
    useful = sum(t['bytes'] for t in tensors)
    assert useful == 16*h*h + 14*h == 9447936
    jobs, groups = [], {}
    for ti in range(len(tensors)):
        subset = [c for c in chunks if c['tensor']==ti]
        group = f'tensor_{ti}_fill_complete'
        groups[group] = [2*c['index'] for c in subset]
        for c in subset:
            n, off = c['index'], c['offset']
            jobs.append(dict(id=2*n, role='fill_proxy', channel=0, did=10,
                             src=SRC_IOVA+off, dst=RELAY_IOVA+off,
                             bytes=c['bytes'], dependencies=[],
                             logical_ready='source package available at common start',
                             tensor=ti, chunk=n))
        for c in subset:
            n, off = c['index'], c['offset']
            jobs.append(dict(id=2*n+1, role='copy_proxy', channel=1, did=11,
                             src=RELAY_IOVA+off, dst=DST_IOVA+off,
                             bytes=c['bytes'], dependencies=[groups[group][-1]],
                             dependency_group=group,
                             logical_ready='all fills of this source tensor complete',
                             tensor=ti, chunk=n))
    direct = [dict(id=c['index'], role='resident_direct', channel=1, did=11,
                   src=SRC_IOVA+c['offset'], dst=DST_IOVA+c['offset'],
                   bytes=c['bytes'], dependencies=[],
                   logical_ready='source package available at common start',
                   tensor=c['tensor'], chunk=c['index']) for c in chunks]
    mappings = [dict(iova=SRC_IOVA, pa=SOURCE, reserved_bytes=REGION),
                dict(iova=RELAY_IOVA, pa=RELAY, reserved_bytes=REGION),
                dict(iova=DST_IOVA, pa=DEST, reserved_bytes=REGION)]
    result = dict(schema='ai-source-transfer-projection-v1', stage=105,
        source_commit='004ffef82b46e8dc8685c55d0cdda650bdaf1269',
        source_model='opt-125m', source_scope='one complete MLP weight package',
        source='flex_opt.py MLP.init_weight/load_weight; opt_config.py OPT125m',
        platform='Genesys2/CVA6/four iDMA/RISC-V IOMMU/Linux',
        projection_scope='two iDMA fill/copy roles; no GPU or NVMe implementation',
        physical_requester_identity='DID10/11 are platform iDMA identities; not CUDA streams',
        application_timing=None, deadlines=None, compute_delay=None,
        tensors=tensors, chunks=chunks, useful_bytes=useful,
        payload_bytes_including_holes=offset, reserved_payload_bytes=3*REGION,
        relay='whole package; distinct regions for every chunk, no shared-slot overwrite',
        mappings=mappings, jobs=jobs, barrier_groups=groups,
        resident_direct_jobs=direct,
        two_hop_logical_copy_bytes=2*useful,
        two_hop_DDR_read_plus_write_bytes=4*useful,
        direct_logical_copy_bytes=useful,
        direct_DDR_read_plus_write_bytes=2*useful,
        not_counted_as_useful=['page padding', 'relay copy', 'DDT/PTE', 'poll/config traffic'],
        allocation_and_mapping_costs='payload reserve is 48MiB plus actual PTE/DDT/CPU allocation; not free preparation',
        ordinary_rules=['FIFO tensor-order fill and dependent-copy pipeline',
                        'whole-package prefill followed by copy',
                        'resident direct one-hop when available, cost of obtaining residency declared',
                        'whole-tensor DMA supported by hardware, not excluded by seed 4KiB segmentation',
                        'legal 2MiB mappings/preparation and relevant feedback selected at G0'],
        functional_seed='serial topological execution only, not a performance baseline',
        hardware_adapter_required=True, production_ABI_modified=False,
        performance_G0_accepted=False, G1_candidate=None)
    return result


def write_commands(path, jobs):
    with path.open('w') as f:
        f.write('id channel src_iova dst_iova bytes dep tensor chunk\n')
        for j in jobs:
            dep = j['dependencies'][0] if j['dependencies'] else -1
            f.write(f"{j['id']} {j['channel']} {j['src']} {j['dst']} {j['bytes']} {dep} {j['tensor']} {j['chunk']}\n")


def make_payload(contract):
    data = bytearray([0xA5]) * contract['payload_bytes_including_holes']
    for ti, t in enumerate(contract['tensors']):
        for wi in range(t['bytes']//8):
            # Unique tensor/word signatures validate full byte preservation.
            value = ((ti+1) << 56) | wi
            struct.pack_into('<Q', data, t['offset']+wi*8, value)
    return bytes(data)


def reference(contract, payload):
    # This verifies the DAG/data contract, not an asynchronous DMA or its cycles.
    src, relay, dst = payload, bytearray(len(payload)), bytearray(len(payload))
    completed = set()
    for j in contract['jobs']:
        deps = set(j['dependencies'])
        if 'dependency_group' in j:
            deps.update(contract['barrier_groups'][j['dependency_group']])
        if not deps <= completed:
            raise ValueError('dependent copy precedes relay fill')
        c = contract['chunks'][j['chunk']]
        o, n = c['offset'], c['bytes']
        target, source = (relay, src) if j['channel']==0 else (dst, relay)
        target[o:o+n] = source[o:o+n]
        if j['id'] in completed:
            raise ValueError('duplicate completion')
        completed.add(j['id'])
    for t in contract['tensors']:
        o, n = t['offset'], t['bytes']
        if dst[o:o+n] != src[o:o+n]:
            raise ValueError('prepared operand data mismatch')
    return dict(classification='host logical DAG/data reference only',
                jobs_completed=len(completed), useful_bytes=contract['useful_bytes'],
                source_payload_sha256=hashlib.sha256(payload).hexdigest(),
                all_six_tensors_equal=True, timing_measured=False,
                DUT_executed=False, trained_parameters=False)


if __name__ == '__main__':
    contract = compile_packet()
    (OUT/'transfer-contract.json').write_text(json.dumps(contract, indent=2)+'\n')
    write_commands(OUT/'two-hop-commands.tsv', contract['jobs'])
    write_commands(OUT/'resident-direct-commands.tsv', contract['resident_direct_jobs'])
    payload = make_payload(contract)
    (OUT/'payload.bin').write_bytes(payload)
    result = reference(contract, payload)
    (OUT/'host-reference.json').write_text(json.dumps(result, indent=2)+'\n')
    print(json.dumps(dict(useful_bytes=contract['useful_bytes'],
                         chunks=len(contract['chunks']), jobs=len(contract['jobs']),
                         host_reference=True, DUT_executed=False)))
