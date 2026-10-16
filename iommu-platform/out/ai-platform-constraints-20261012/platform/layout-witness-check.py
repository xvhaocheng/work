#!/usr/bin/env python3
"""One-off static check of the stage106 dispersed-extent geometry witness."""
import csv
import hashlib
import json
import re
from pathlib import Path


def check():
    here = Path(__file__).resolve().parent
    witness = json.loads((here / 'layout-witness.json').read_text())
    root = Path(witness['workspace_root'])
    for source in witness['inputs']:
        raw = (root / source['path']).read_bytes()
        assert hashlib.sha256(raw).hexdigest() == source['sha256'], source['path']
    contract = json.loads((root / witness['original_contract']).read_text())
    assert witness['address_status'] == 'illustrative_not_allocated_or_programmed'
    assert witness['tensors'] == contract['tensors']
    leaf_bytes = 2 * 1024 * 1024
    slot_bytes = 10 * 1024 * 1024
    slots, leaves = witness['slots'], witness['leaves']
    assert len(slots) == 4 and len(leaves) == 20
    assert witness['mapped_payload_bytes'] == 4 * slot_bytes
    assert witness['original_host_backing_bytes'] == contract['resources']['actual_dense_backing_bytes']
    assert sum(t['bytes'] for t in witness['tensors']) == 9447936
    assert len({s['slot_id'] for s in slots}) == 4
    ram_lo, ram_hi = witness['ram']['start'], witness['ram']['end_exclusive']
    dts = (root / witness['ram']['source_path']).read_text()
    memory = re.search(r'memory@80200000\s*\{[^}]*reg\s*=\s*<([^>]+)>', dts, re.S)
    assert memory is not None
    cells = [int(cell, 0) for cell in memory.group(1).split()]
    start = (cells[0] << 32) | cells[1]
    size = (cells[2] << 32) | cells[3]
    assert (ram_lo, ram_hi) == (start, start + size)
    pa_ranges, io_ranges = [], []
    for slot in slots:
        role = contract['roles'][slot['role']]
        base = role['source_iova' if slot['kind'] == 'source' else 'destination_iova']
        assert slot['iova_start'] == base and slot['bytes'] == role['slot_bytes'] == slot_bytes
        assert base % leaf_bytes == 0
        io_ranges.append((base, base + slot_bytes))
        owned = sorted((l for l in leaves if l['slot_id'] == slot['slot_id']), key=lambda l: l['index'])
        assert [l['index'] for l in owned] == list(range(5))
        for index, leaf in enumerate(owned):
            assert leaf['bytes'] == leaf_bytes
            assert leaf['iova_start'] == base + index * leaf_bytes
            pa = leaf['pa_start']
            assert pa % leaf_bytes == 0 and ram_lo <= pa < pa + leaf_bytes <= ram_hi
            assert leaf['owner'] == slot['owner']
            assert leaf['ownership_range'] == {'pa_start': pa, 'bytes': leaf_bytes, 'owner': slot['owner']}
            assert leaf['permission_request'] == ('R,A;noW/X' if slot['kind'] == 'source' else 'R,W,A,D;noX')
            assert leaf['authorized_dids_by_dispatch'] == {'parallel': [10 + slot['role']], 'single': [10]}
            pa_ranges.append((pa, pa + leaf_bytes))
    for ranges in (pa_ranges, io_ranges):
        ordered = sorted(ranges)
        assert all(a[1] <= b[0] for a, b in zip(ordered, ordered[1:]))
    assert all(a[1] < b[0] for a, b in zip(sorted(pa_ranges), sorted(pa_ranges)[1:]))
    assert len(witness['copy_objects']) == 12
    coverage_ranges = 0
    cross_extent_objects = 0
    useful = 0
    seen = set()
    for obj in witness['copy_objects']:
        role, tensor = obj['role'], obj['tensor']
        assert (role, tensor) not in seen
        seen.add((role, tensor))
        original = contract['tensors'][tensor]
        assert obj['offset'] == original['offset'] and obj['bytes'] == original['bytes']
        assert obj['tensor_name'] == original['name']
        useful += obj['bytes']
        src_phys, dst_phys = [], []
        for kind in ('source', 'destination'):
            slot = next(s for s in slots if s['role'] == role and s['kind'] == kind)
            start, end = slot['iova_start'] + obj['offset'], slot['iova_start'] + obj['offset'] + obj['bytes']
            assert slot['iova_start'] <= start < end <= slot['iova_start'] + slot_bytes
            assert obj[kind + '_iova'] == start
            parts = obj[kind + '_coverage']
            cursor = start
            for part in parts:
                leaf = next(l for l in leaves if l['leaf_id'] == part['leaf_id'])
                assert leaf['slot_id'] == slot['slot_id']
                assert part['iova_start'] == cursor
                assert part['bytes'] > 0 and cursor + part['bytes'] <= leaf['iova_start'] + leaf_bytes
                assert leaf['iova_start'] <= cursor
                assert part['pa_start'] == leaf['pa_start'] + cursor - leaf['iova_start']
                cursor += part['bytes']
                (src_phys if kind == 'source' else dst_phys).append((part['pa_start'], part['pa_start'] + part['bytes']))
                coverage_ranges += 1
            assert cursor == end
        assert all(a[1] <= b[0] or b[1] <= a[0] for a in src_phys for b in dst_phys)
        cross_extent_objects += len(obj['source_coverage']) > 1
    assert seen == {(r, t) for r in (0, 1) for t in range(6)}
    assert useful == witness['full_pair_useful_bytes'] == 18895872
    for mode in ('parallel', 'single'):
        case = next(c for c in contract['cases'] if c['id'] == '2m-' + mode)
        original_rows = list(csv.DictReader((root / witness['original_batch'] / case['commands']).open(), delimiter='\t'))
        rows = [{key: int(value) for key, value in row.items()} for row in original_rows]
        assert rows == case['jobs'] == witness['descriptors_by_dispatch'][mode]
        assert len(rows) == 12
        for row in rows:
            obj = next(o for o in witness['copy_objects'] if o['role'] == row['role'] and o['tensor'] == row['tensor'])
            assert row['src_iova'] == obj['source_iova'] and row['dst_iova'] == obj['destination_iova']
            assert row['bytes'] == obj['bytes'] and row['ready'] == 0
    assert all(value is None for value in witness['unmeasured_costs'].values())
    return {
        'result': 'PASS', 'scope': 'static geometry and original-task coverage only',
        'leaf_count': 20, 'slot_count': 4, 'full_copy_objects': 12,
        'useful_transfer_bytes': useful, 'source_and_destination_valid_bytes': 2 * useful,
        'source_and_destination_interval_parts': coverage_ranges,
        'whole_tensor_objects_crossing_noncontiguous_extents': cross_extent_objects,
        'descriptors_per_dispatch': 12, 'actual_allocations': 0,
        'dut_runs': 0, 'board_runs': 0,
    }


if __name__ == '__main__':
    print(json.dumps(check(), ensure_ascii=False, indent=2))
