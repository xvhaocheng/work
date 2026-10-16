#!/usr/bin/env python3
"""Accept the new R107 finite ioctl receipt; never execute old replay/matrix.

Only shared Rejection/require/sha/digest helpers are loaded from stage106.
Every acceptance claim is tied to raw current receipt fields or whole binaries.
"""
import argparse
import ctypes as C
import importlib.util
import json
import sys
from pathlib import Path

sys.dont_write_bytecode = True
ROOT = Path(__file__).resolve().parents[1]
PLATFORM = ROOT.parents[1]
OLD = PLATFORM / 'out/ai-mandatory-work-g0-20261012/validator/validate_g0_records.py'
spec = importlib.util.spec_from_file_location('stage106_shared_validation_helpers', OLD)
shared = importlib.util.module_from_spec(spec)
spec.loader.exec_module(shared)
Rejection, require, sha, digest = shared.Rejection, shared.require, shared.sha, shared.digest

SCHEMA = 'r107-ordinary-finite-pilot-receipt-v1'
POOL = 2359296
BACKING = 16777216
PHASE_BYTES = 1769472
GUARD_COVERAGE = 13238272
HASHES = {
    'worker0-initial_gpu.bin': '4e65c7d3200cc915171578c958a1dc0215c58cc0eaaad59a2ec83dae8a024b6d',
    'worker0-cpu_source.bin': '4e65c7d3200cc915171578c958a1dc0215c58cc0eaaad59a2ec83dae8a024b6d',
    'worker0-restored_gpu.bin': 'ac113ba4522ae30e6b189135b5b34cfbb1fe4183708ec2bb8db7ceed7792f97b',
    'worker1-initial_gpu.bin': 'e36600660f944b7c353025f6f9481e97045910d9b02e113ee0218ebf6a86ceea',
    'worker1-cpu_source.bin': 'e36600660f944b7c353025f6f9481e97045910d9b02e113ee0218ebf6a86ceea',
    'worker1-restored_gpu.bin': 'f58651864a5311a5c69c7e471b61adbb726e5c164cff9036f152d4ac2e12a00d',
}
STAGES = [
    'immutable_seed_input', 'dma_api_allocation_mapping',
    'actual_leaf_audit_checked_prepare_fence_and_cpu_to_dma', 'prior_swapout',
    'swapout_cpu_reacquire_and_full_verify', 'restore_cpu_to_dma_handoff',
    'restore', 'final_cpu_reacquire_full_pools_and_backing_guard_verify',
    'copy_raw_output_to_user', 'actual_leaf_revoke_checked_fence_and_release',
    'final_bookkeeping', 'whole_ioctl',
]


class RawStamp(C.Structure):
    _fields_ = [('cpu_cycles', C.c_uint64), ('time_ticks', C.c_uint64), ('nominal_ns', C.c_uint64)]


class RawStage(C.Structure):
    _fields_ = [('begin', RawStamp), ('end', RawStamp), ('status', C.c_int32), ('reached', C.c_uint32)]


class RawLayer(C.Structure):
    _fields_ = [('worker', C.c_uint32), ('layer', C.c_uint32), ('ready', RawStamp)]


class RawBatch(C.Structure):
    _fields_ = [(k, C.c_uint32) for k in ('phase', 'worker', 'layer', 'component')] + [
        (k, C.c_uint64) for k in ('source_offset', 'destination_offset', 'bytes', 'first_id',
                                'observed_done', 'issued', 'completed', 'control')] + [
        ('begin', RawStamp), ('end', RawStamp), ('status', C.c_int32), ('quiet', C.c_uint32)]


class RawLeaf(C.Structure):
    _fields_ = [(k, C.c_uint64) for k in ('iova', 'pa', 'bytes', 'pte')] + [
        ('table_pa', C.c_uint64*5)] + [(k, C.c_uint32) for k in ('pool', 'extent', 'table_count', 'reserved')]


class RawRun(C.Structure):
    _fields_ = [(k, C.c_uint32) for k in ('abi_version', 'struct_size', 'flags', 'timeout_ms',
                                        'batch_capacity', 'leaf_capacity')] + [
        ('reserved', C.c_uint32*2), ('gpu_seed_user', C.c_uint64*2), ('pool_user', C.c_uint64*4),
        ('batches_user', C.c_uint64), ('leaves_user', C.c_uint64),
        ('status', C.c_int32), ('release_status', C.c_int32)] + [
        (k, C.c_uint32) for k in ('stage', 'quarantined', 'allocation_kind', 'extent_count',
                                'batch_count', 'leaf_count', 'worker_layers', 'joint_layers')] + [
        (k, C.c_uint64) for k in ('backing_bytes', 'reference_bytes', 'source_bytes_verified',
                                'destination_bytes_verified', 'guard_bytes_verified', 'data_errors',
                                'swapout_bytes', 'restore_bytes', 'swapout_tasks', 'restore_tasks',
                                'swapout_logical_runs', 'restore_logical_runs')] + [
        ('stages', RawStage*12), ('layers', RawLayer*24),
        ('hpm_before', (C.c_uint64*7)*2), ('hpm_after', (C.c_uint64*7)*2),
        ('observer', (C.c_uint64*35)*2), ('observer_status', C.c_uint64*2),
        ('composition', (C.c_uint64*128)*2)]


def raw_value(obj):
    if isinstance(obj, C.Structure):
        return {name: raw_value(getattr(obj, name)) for name, *_ in obj._fields_}
    if isinstance(obj, C.Array):
        return [raw_value(v) for v in obj]
    return obj


def without_index(row):
    return {k: v for k, v in row.items() if k != 'index'}


def original_buffers(receipt, receipt_dir):
    require(C.sizeof(RawRun) == 4520 and C.sizeof(RawBatch) == 136 and C.sizeof(RawLeaf) == 88,
            'host decoder matches frozen RV64 UAPI sizes')
    decoded = {}
    for file, typ in [('uapi-run.bin', RawRun), ('uapi-batches.bin', RawBatch*192),
                      ('uapi-leaves.bin', RawLeaf*4096)]:
        raw = (receipt_dir / file).read_bytes()
        require(len(raw) == C.sizeof(typ), f'{file}: full original UAPI buffer')
        decoded[file] = raw_value(typ.from_buffer_copy(raw))
    run_raw = decoded['uapi-run.bin']
    for k, v in receipt['request'].items():
        require(k in run_raw and run_raw[k] == v, f'original UAPI request field {k}')
    for k, v in receipt['run'].items():
        if k in ('batches', 'leaves'):
            continue
        expected = [without_index(x) for x in v] if k == 'stages' else v
        require(k in run_raw and run_raw[k] == expected, f'original UAPI output field {k}')
    for name, file, count in [('batches', 'uapi-batches.bin', receipt['run']['batch_count']),
                              ('leaves', 'uapi-leaves.bin', receipt['run']['leaf_count'])]:
        require(decoded[file][:count] == [without_index(x) for x in receipt['run'][name]],
                f'original full {name} record buffer matches JSON')


def number(value, what):
    require(type(value) is int and 0 <= value < 1 << 64, f'{what}: unsigned raw integer')
    return value


def stamp(value, what, nonzero=True):
    require(set(value) == {'cpu_cycles', 'time_ticks', 'nominal_ns'}, f'{what}: exact clock fields')
    for k, v in value.items():
        number(v, f'{what}.{k}')
        if nonzero:
            require(v != 0, f'{what}.{k}: absent timestamp')
    return value


def before(a, b, what):
    for clock in ('cpu_cycles', 'time_ticks', 'nominal_ns'):
        require(a[clock] <= b[clock], f'{what}: {clock} dependency order')


def exact_asset(path, expected_hash):
    require(path.is_file(), f'missing asset {path.name}')
    require(path.stat().st_size == POOL, f'{path.name}: complete pool including guards/padding')
    require(digest(path) == expected_hash, f'{path.name}: frozen full-pool sha256')


def frozen_assets(data_dir):
    manifest = json.loads((data_dir / 'raw-manifest.json').read_text())
    require(manifest['schema'] == 'r107-ordinary-frozen-data-v1', 'data manifest schema')
    entries = manifest['files']
    require(len(entries) == 6 and {x['file'] for x in entries} == set(HASHES), 'exact six frozen assets')
    for x in entries:
        require(x['bytes'] == POOL and x['sha256'] == HASHES[x['file']], 'data manifest identity')
        exact_asset(data_dir / x['file'], HASHES[x['file']])


def expected_batches():
    result = {}
    for phase in (0, 1):
        for worker in (0, 1):
            for layer in range(12):
                for component in (0, 1):
                    base = layer * 196608 + component * 98304
                    runs = [(5, 5, 36864)] if phase == 0 else [(5, 6, 24576), (7, 5, 12288)]
                    for src, dst, size in runs:
                        key = (phase, worker, layer, component, base + src * 12288,
                               base + dst * 12288, size)
                        result[key] = size // 4096
    require(len(result) == 144, 'validator internal finite enumeration')
    return result


def raw_counters(r):
    for name, width in [('hpm_before', 7), ('hpm_after', 7), ('observer', 35),
                        ('composition', 128)]:
        require(len(r[name]) == 2 and all(len(x) == width for x in r[name]), f'{name}: complete raw phase arrays')
        for p, row in enumerate(r[name]):
            for i, v in enumerate(row):
                number(v, f'{name}[{p}][{i}]')
    require(len(r['observer_status']) == 2, 'raw observer status length')
    for phase in (0, 1):
        o, c = r['observer'][phase], r['composition'][phase]
        require(o[0] == sum(o[1:4]) and o[0] == sum(o[10:14]), f'phase{phase}: observer cycle decomposition')
        require(o[7] == 432 and o[8] == 432, f'phase{phase}: actual observer hardware issue/done conservation')
        require(o[9] == 1, f'phase{phase}: final BATCH_READ successful completion poll')
        # Opening and closing are 16-bit R/W outstanding counts, not per-beat records.
        for lane, opened, closed, ar, aw, rlast, bresp in [
                ('V', o[30], o[32], o[14], o[15], o[16], o[17]),
                ('A', o[31], o[33], o[20], o[21], o[22], o[23])]:
            require(opened < 1 << 32 and closed < 1 << 32, f'phase{phase}/{lane}: raw outstanding width')
            require((opened >> 16) + ar == (closed >> 16) + rlast,
                    f'phase{phase}/{lane}: read accept/return conservation')
            require((opened & 65535) + aw == (closed & 65535) + bresp,
                    f'phase{phase}/{lane}: write accept/return conservation')
            require(closed == 0, f'phase{phase}/{lane}: nonzero closing outstanding')
        require(r['observer_status'][phase] == 0, f'phase{phase}: observer status error')
        require(c[0] == 0x49444d4143504e01 and c[1] == 1, f'phase{phase}: composition identity')
        require(c[2] == 0xA00 and c[3] == 0 and c[4] == 0, f'phase{phase}: live S0 identity/status')
        require(c[5] == 1 and c[6] == 4 and c[9] == 0, f'phase{phase}: FROZEN/quiet/error')
        require(c[10] == 0xA00 and c[11] == 0, f'phase{phase}: window S0 identity')
        for total, one, two in [(12, 13, 14), (15, 16, 17), (18, 19, 20)]:
            require(c[total] == c[one] + c[two], f'phase{phase}: composition raw decomposition {total}')
        require(c[34] == 3 and c[35] == 0 and all(v == 0 for v in c[40:64]),
                f'phase{phase}: composition capabilities/reserved')
    return {'observer_counters': 'batch-independent raw issue/done and outstanding conservation',
            'hpm_counters': 'seven complete sequential raw snapshots; no atomic-window claim'}


def mapping(r):
    kind, extents = r['allocation_kind'], r['extent_count']
    require(kind in (1, 2, 3), 'missing DMA-API allocation kind')
    require(extents == (8 if kind == 2 else 4), 'actual allocation extent count')
    leaves = r['leaves']
    require(0 < r['leaf_count'] <= 4096 and len(leaves) == r['leaf_count'], 'complete actual leaf records')
    seen, groups, iova_ranges, pa_ranges, tables = set(), {}, [], [], set()
    for i, leaf in enumerate(leaves):
        require(leaf['index'] == i, 'actual leaf array order')
        pool, extent = leaf['pool'], leaf['extent']
        require(pool in range(4) and extent in range(extents), 'actual leaf pool/extent ownership')
        n, va, pa, pte = [number(leaf[k], f'leaf{i}.{k}') for k in ('bytes', 'iova', 'pa', 'pte')]
        require(n in (4096, 2097152), 'actual Sv39 leaf must fit whole owner allocation')
        require(va != 0 and pa != 0 and va % n == 0 and pa % n == 0, 'actual leaf base/alignment')
        require((pte & 7) == 7 and (pte & 8) == 0, 'DMA-API BIDIRECTIONAL leaf V/R/W, no execute')
        require(((pte >> 10) & ((1 << 44) - 1)) * 4096 == pa, 'actual PTE-to-PA binding')
        require(leaf['reserved'] == 0 and leaf['table_count'] == (2 if n == 2097152 else 3), 'actual Sv39 table depth/reserved')
        require(len(leaf['table_pa']) == 5, 'actual table page record capacity')
        for j, table in enumerate(leaf['table_pa']):
            number(table, 'actual table PA')
            if j < leaf['table_count']:
                require(table != 0 and table % 4096 == 0, 'actual table PA aligned/nonzero')
                tables.add(table)
            else:
                require(table == 0, 'unused actual table slots must be zero')
        require((va, n) not in seen, 'duplicate actual leaf/extra DMA alias')
        seen.add((va, n));iova_ranges.append((va, va+n));pa_ranges.append((pa, pa+n))
        groups.setdefault((pool, extent), []).append((va, va+n))
    for what, ranges in [('IOVA', iova_ranges), ('PA', pa_ranges)]:
        ordered = sorted(ranges)
        require(all(a[1] <= b[0] for a, b in zip(ordered, ordered[1:])), f'whole owner {what} nonalias/nonoverlap')
    require(sum(b-a for a, b in iova_ranges) == BACKING, 'actual full 16MiB mapped owner coverage')
    require(len(groups) == extents, 'all actual DMA-API extents accounted')
    expected_extent = 2097152 if kind == 2 else 4194304
    for pool in range(4):
        own = [spans for (p, _), spans in groups.items() if p == pool]
        require(len(own) == (2 if kind == 2 else 1), f'pool{pool}: whole allocation ownership')
        require(sum(b-a for spans in own for a, b in spans) == 4194304, f'pool{pool}: physical backing/slack accounting')
        for spans in own:
            ordered = sorted(spans)
            require(sum(b-a for a, b in spans) == expected_extent and
                    all(a[1] == b[0] for a, b in zip(ordered, ordered[1:])),
                    f'pool{pool}: actual contiguous IOVA extent coverage')
    require(all(not any(a <= table < b for a, b in pa_ranges) for table in tables),
            'page table backing must not alias payload backing')
    return {'allocation_kind': kind, 'actual_leaf_count': len(leaves),
            'actual_table_pages': len(tables), 'actual_mapped_bytes': BACKING,
            'hardware_source_RO_claim': False}


def validate(receipt, receipt_dir, data_dir):
    require(receipt['schema'] == SCHEMA and receipt['kind'] == 'run', 'new finite pilot receipt schema/kind')
    require(receipt['timing_semantics']['physical_calibration_claim'] is False, 'raw nominal clock must not claim physical calibration')
    io, q, r = receipt['ioctl'], receipt['request'], receipt['run']
    require(io['attempted'] is True and io['run_calls'] == 1 and io['return'] == 0 and io['errno'] == 0, 'finite RUN syscall failure/no single actual ioctl')
    require(q['abi_version'] == 1 and q['struct_size'] == 4520 and q['flags'] == 0 and q['reserved'] == [0, 0], 'fixed UAPI ABI/size/zero flags')
    require(q['timeout_ms'] == 300000 and q['batch_capacity'] == 192 and q['leaf_capacity'] == 4096, 'frozen fault guard and capacities')
    require(len(q['gpu_seed_user']) == 2 and len(q['pool_user']) == 4 and all(q['gpu_seed_user'] + q['pool_user']) and q['batches_user'] and q['leaves_user'], 'complete userspace buffer pointers')
    require(r['status'] == 0 and r['release_status'] == 0 and r['quarantined'] == 0, 'kernel failure/quarantine/unchecked release')
    require(r['stage'] == 11, 'whole finite transaction not complete')
    for k, n in [('backing_bytes', BACKING), ('reference_bytes', 2*POOL),
                 ('source_bytes_verified', 2*POOL), ('destination_bytes_verified', 2*POOL),
                 ('guard_bytes_verified', GUARD_COVERAGE), ('data_errors', 0),
                 ('swapout_bytes', PHASE_BYTES), ('restore_bytes', PHASE_BYTES),
                 ('swapout_tasks', 432), ('restore_tasks', 432),
                 ('swapout_logical_runs', 48), ('restore_logical_runs', 96),
                 ('worker_layers', 24), ('joint_layers', 12)]:
        require(r[k] == n, f'finite exact {k}')
    stages = r['stages'];require(len(stages) == 12, 'all finite stage records')
    for i, s in enumerate(stages):
        require(s['index'] == i and s['reached'] == 1 and s['status'] == 0, f'stage{i}/{STAGES[i]} not successful')
        stamp(s['begin'], f'stage{i}.begin');stamp(s['end'], f'stage{i}.end')
        before(s['begin'], s['end'], f'stage{i} interval')
        if 0 < i < 11:
            before(stages[i-1]['end'], s['begin'], f'stage{i-1}->stage{i}')
    before(stages[11]['begin'], stages[0]['begin'], 'whole ioctl starts before input')
    before(stages[10]['end'], stages[11]['end'], 'whole ioctl completes after bookkeeping')
    expected = expected_batches();coverage = {k: [] for k in expected}
    require(144 <= r['batch_count'] <= 192 and len(r['batches']) == r['batch_count'], 'all actual whole-batch receipts/capacity')
    layer_completions = {}
    for i, b in enumerate(r['batches']):
        key = tuple(b[k] for k in ('phase', 'worker', 'layer', 'component', 'source_offset', 'destination_offset', 'bytes'))
        matched = [k for k in expected if k[:4] == key[:4] and
                   key[4] >= k[4] and key[4]+key[6] <= k[4]+k[6] and
                   key[5]-k[5] == key[4]-k[4]]
        require(len(matched) == 1 and key[6] > 0 and key[6] % 4096 == 0 and
                key[4] % 4096 == 0 and key[5] % 4096 == 0,
                f'batch{i}: foreign shard/run or invalid legal extent split')
        coverage[matched[0]].append((key[4], key[4]+key[6]))
        tasks = key[6] // 4096
        require(b['index'] == i and b['status'] == 0 and b['quiet'] == 1, f'batch{i}: failed/nonquiet completion')
        require(b['issued'] == tasks and b['completed'] == tasks and b['control'] == 0, f'batch{i}: hardware task count/control')
        require(number(b['first_id'], 'whole batch NEXT ID') > 0 and
                number(b['observed_done'], 'whole batch DONE ID') == b['first_id'], f'batch{i}: whole-batch NEXT/DONE exact binding')
        stamp(b['begin'], f'batch{i}.begin');stamp(b['end'], f'batch{i}.end')
        before(b['begin'], b['end'], f'batch{i} interval')
        phase_stage = stages[3 if b['phase'] == 0 else 6]
        before(phase_stage['begin'], b['begin'], f'batch{i} starts within phase')
        before(b['end'], phase_stage['end'], f'batch{i} ends within phase')
        if i:
            before(r['batches'][i-1]['end'], b['begin'], f'batch{i-1}->batch{i}')
        if b['phase'] == 1:
            layer_completions.setdefault((b['worker'], b['layer']), []).append(b['end'])
    for key, spans in coverage.items():
        ordered = sorted(spans)
        require(ordered and ordered[0][0] == key[4] and ordered[-1][1] == key[4]+key[6] and
                all(a[1] == b[0] for a, b in zip(ordered, ordered[1:])),
                'missing/duplicate byte coverage in finite logical run')
    require(len(r['layers']) == 24, 'all 24 worker-layer data-ready records')
    layers = set()
    for row in r['layers']:
        key = (row['worker'], row['layer'])
        require(key not in layers and key in layer_completions and len(layer_completions[key]) >= 4, 'missing/duplicate/foreign worker-layer record')
        layers.add(key);stamp(row['ready'], f'layer{key}.ready')
        for done in layer_completions[key]:
            before(done, row['ready'], f'layer{key} complete before consumer proxy')
        before(stages[7]['begin'], row['ready'], f'layer{key} CPU reacquire before proxy')
        before(row['ready'], stages[7]['end'], f'layer{key} ready within full verification')
    require(layers == {(w, l) for w in range(2) for l in range(12)}, 'complete layers/shards')
    map_result = mapping(r);counter_result = raw_counters(r)
    frozen_assets(data_dir)
    assets = receipt['input_assets']
    require(len(assets) == 6 and {a['file'] for a in assets} == set(HASHES), 'all frozen input/reference receipt assets')
    for a in assets:
        require(a['bytes'] == POOL and a['sha256'] == HASHES[a['file']], 'receipt input/reference identity')
    require(len(receipt['pools']) == 4, 'four complete raw returned pools')
    for i, pool in enumerate(receipt['pools']):
        ref = f'worker{i//2}-' + ('restored_gpu.bin' if i % 2 else 'cpu_source.bin')
        require(pool['pool'] == i and pool['worker'] == i//2 and pool['expected_asset'] == ref, 'returned pool/shard ownership')
        require(pool['bytes'] == POOL and pool['memcmp_bytes'] == POOL and
                pool['memcmp_equal'] is True and pool['first_mismatch'] is None and
                pool['memcmp_errno'] == 0 and pool['write_errno'] == 0, 'complete full-pool memcmp including padding/guards')
        require(pool['sha256'] == HASHES[ref] and pool['expected_sha256'] == HASHES[ref], 'returned complete pool frozen sha256')
        filename = Path(pool['file']);require(filename.name == pool['file'], 'pool file must be local receipt artifact')
        exact_asset(receipt_dir / filename, HASHES[ref])
        require((receipt_dir / filename).read_bytes() == (data_dir / ref).read_bytes(), 'host independent full byte comparison')
    require(receipt['raw_write_errno'] == [0, 0, 0], 'all original UAPI raw buffer writes')
    for file, size in [('uapi-run.bin', 4520), ('uapi-batches.bin', 192*136), ('uapi-leaves.bin', 4096*88)]:
        require((receipt_dir / file).is_file() and (receipt_dir / file).stat().st_size == size, f'{file}: raw UAPI buffer full capacity')
    original_buffers(receipt, receipt_dir)
    memory = receipt['userspace_memory']
    require(memory == {'seed_reference_bytes': 2*POOL, 'output_pool_bytes': 4*POOL,
                       'batch_record_bytes': 192*136, 'leaf_record_bytes': 4096*88}, 'userspace reference/output memory accounting')
    costs = receipt['host_cost_ns']
    for k in ['data_load_and_allocation', 'device_open_ioctl_close', 'hash_and_full_memcmp',
              'raw_output_write', 'total_before_receipt']:
        number(costs[k], f'host phase cost {k}')
    require(costs['total_before_receipt'] >= sum(v for k, v in costs.items() if k != 'total_before_receipt'), 'host phase cost conservation')
    return {'accepted': True, 'schema': SCHEMA, 'bytes_each_phase': PHASE_BYTES,
            'logical_runs': [48, 96], 'hardware_tasks_each_phase': 432,
            'whole_batches': r['batch_count'], 'worker_layer_proxy_ready': 24, 'joint_layer_bookkeeping': 12,
            'mapping': map_result, 'counters': counter_result,
            'checked_prepare_stage': 2, 'checked_revoke_release_stage': 9,
            'guard_coverage_overlap_with_pool_verification': True,
            'physical_calibration_claim': False,
            'limitations': ['no exposed per-beat trace or per-task independent NEXT acceptance',
                            'layer ready is CPU consumer proxy, not attention/token completion',
                            'raw batch address registers and post-revoke PTE snapshots are not separate UAPI fields; '
                            'their checked validation is represented by kernel stage2/stage9 and release_status']}


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('receipt', type=Path)
    parser.add_argument('--data-dir', type=Path, default=ROOT/'data')
    parser.add_argument('--result', type=Path)
    args = parser.parse_args()
    try:
        receipt = json.loads(args.receipt.read_text())
        result = validate(receipt, args.receipt.parent, args.data_dir)
        result['receipt_sha256'] = digest(args.receipt)
        code = 0
    except (Rejection, KeyError, TypeError, ValueError, OSError) as exc:
        result = {'accepted': False, 'schema': SCHEMA, 'reason': str(exc)}
        code = 1
    rendered = json.dumps(result, indent=2, ensure_ascii=False) + '\n'
    if args.result:
        args.result.write_text(rendered)
    print(rendered, end='')
    return code


if __name__ == '__main__':
    raise SystemExit(main())
