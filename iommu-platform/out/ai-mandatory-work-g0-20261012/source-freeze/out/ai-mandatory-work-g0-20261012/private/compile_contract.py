#!/usr/bin/env python3
"""Compile the frozen stage106 transfer contract; no timing or policy search."""
import argparse
import array
import hashlib
import json
import sys
from pathlib import Path


def digest(path):
    return hashlib.sha256(path.read_bytes()).hexdigest()


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument('--batch', type=Path, default=Path(__file__).resolve().parents[1])
    args = parser.parse_args()
    batch = args.batch.resolve()
    register = json.loads((batch / 'g0-register.json').read_text())
    selection = json.loads((batch / 'selection.json').read_text())
    slot = 10 * 1024 * 1024
    io, pa = 0x40000000, 0x84000000
    offsets = [0, 4718592, 4726784, 9445376, 9449472, 9453568]
    sizes = [4718592, 6144, 4718592, 1536, 1536, 1536]
    names = ['fc1.weight', 'fc1.bias', 'fc2.weight', 'fc2.bias',
             'final_layer_norm.weight', 'final_layer_norm.bias']
    shapes = [[3072, 768], [3072], [768, 3072], [768], [768], [768]]
    extent = (offsets[-1] + sizes[-1] + 4095) // 4096 * 4096
    assert sum(sizes) == register['work']['useful_bytes_per_role'] == 9447936
    assert slot == register['resources']['slot_bytes'] and extent == 9457664
    assert register['case_count'] == len(register['cases']) == 8
    assert register['work']['ready_cycle_all_offered_jobs'] == 0
    tensors = [dict(index=t, name=names[t], shape=shapes[t], dtype='float16',
                    offset=offsets[t], bytes=sizes[t]) for t in range(6)]
    roles = []
    generated = []
    for role, layer in [(0, 0), (1, 6)]:
        payload = bytearray([0x3c]) * extent
        for t in range(6):
            words = array.array('Q', (((role + 1) << 60) ^ (t << 52) ^ k
                                     for k in range(sizes[t] // 8)))
            assert words.itemsize == 8
            if sys.byteorder != 'little':
                words.byteswap()
            payload[offsets[t]:offsets[t] + sizes[t]] = words.tobytes()
        payload_path = batch / f'rank{role}-payload.bin'
        payload_path.write_bytes(payload)
        generated.append(payload_path)
        roles.append(dict(role=role, native_rank=role, decoder_layer=layer,
                          source_object_prefix=f'decoder.layers.{layer}',
                          source_iova=io + 2 * role * slot,
                          destination_iova=io + (2 * role + 1) * slot,
                          source_pa=pa + 2 * role * slot,
                          destination_pa=pa + (2 * role + 1) * slot,
                          slot_bytes=slot, payload_extent_bytes=extent,
                          useful_bytes=sum(sizes), payload=payload_path.name,
                          payload_sha256=digest(payload_path)))
    cases = []
    for case in register['cases']:
        rows = []
        for role in case['offered_roles']:
            for t in range(6):
                rows.append(dict(id=role * 6 + t, role=role,
                                 channel=0 if case['dispatch'] == 'single' else role,
                                 src_iova=roles[role]['source_iova'] + offsets[t],
                                 dst_iova=roles[role]['destination_iova'] + offsets[t],
                                 bytes=sizes[t], tensor=t, ready=0))
        path = batch / f"commands-{case['id']}.tsv"
        columns = ['id', 'role', 'channel', 'src_iova', 'dst_iova', 'bytes', 'tensor', 'ready']
        with path.open('w') as f:
            f.write('\t'.join(columns) + '\n')
            for row in rows:
                f.write('\t'.join(str(row[k]) for k in columns) + '\n')
        generated.append(path)
        cases.append(dict(**case, commands=path.name, jobs=rows,
                          offered_useful_bytes=sum(r['bytes'] for r in rows),
                          command_sha256=digest(path)))
    contract = dict(stage=106, schema=1, status='frozen_before_build',
                    selection_sha256=digest(batch / 'selection.json'),
                    register_sha256=digest(batch / 'g0-register.json'),
                    source_commit=selection['source_commit'],
                    source_card=selection['source_card'],
                    logical_ready=0, physical_execution='existing actual iDMA RTL model',
                    scopes=selection, roles=roles, tensors=tensors, cases=cases,
                    resources=register['resources'],
                    fixture=dict(word_order='little-endian uint64',
                                 word_formula='((role+1)<<60) ^ (tensor<<52) ^ local_word_index',
                                 trained_weights=False, source_and_holes_byte=0x3c,
                                 empty_destination_byte=0xd9, unmapped_backing_byte=0xa7),
                    controller=register['ordinary_rules'],
                    costs=register['costs'], watchdogs=register['watchdogs'],
                    mapping=dict(source_permissions='R,A; no W/X',
                                 destination_permissions='R,W,A,D; no X',
                                 dual_did=[10, 11], single_did=10,
                                 single_did11_valid=False),
                    measurement=register['outputs'], interpretation=register['interpretation'])
    contract_path = batch / 'task-contract.json'
    contract_path.write_text(json.dumps(contract, indent=2) + '\n')
    generated.append(contract_path)
    manifest = dict(stage=106, generated=[dict(path=str(p.relative_to(batch)),
                                             bytes=p.stat().st_size, sha256=digest(p))
                                        for p in generated])
    (batch / 'fixture-manifest.json').write_text(json.dumps(manifest, indent=2) + '\n')
    print(json.dumps(dict(case_count=len(cases), jobs_per_pair=12,
                          useful_bytes_per_role=sum(sizes), extent=extent,
                          files=len(generated))))


if __name__ == '__main__':
    main()
