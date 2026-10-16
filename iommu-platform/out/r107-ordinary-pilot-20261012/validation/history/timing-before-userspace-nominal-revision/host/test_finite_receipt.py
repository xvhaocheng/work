#!/usr/bin/env python3
"""Bounded current-receipt rejection checks, using explicitly synthetic records.

No old shape/matrix/replay is executed. No ioctl/board/kernel claim is made.
"""
import copy
import ctypes as C
import importlib.util
import json
import shutil
import sys
from pathlib import Path

sys.dont_write_bytecode = True
ROOT = Path(__file__).resolve().parents[1]
spec = importlib.util.spec_from_file_location('finite_receipt_validation', ROOT/'host/validate_finite_pilot.py')
v = importlib.util.module_from_spec(spec)
spec.loader.exec_module(v)


def tick(n):
    return {'cpu_cycles': n, 'time_ticks': n, 'nominal_ns': n}


def make_fixture():
    q = dict(abi_version=1, struct_size=4520, flags=0, timeout_ms=300000,
             batch_capacity=192, leaf_capacity=4096, reserved=[0, 0],
             gpu_seed_user=[0x1000, 0x2000], pool_user=[0x3000, 0x4000, 0x5000, 0x6000],
             batches_user=0x7000, leaves_user=0x8000)
    stages = [dict(index=i, reached=1, status=0, begin=tick(10000*i+100),
                   end=tick(10000*i+9900)) for i in range(12)]
    stages[11]['begin'], stages[11]['end'] = tick(1), tick(110000)
    batches = []
    phase_indices = [0, 0]
    for key, tasks in v.expected_batches().items():
        phase, worker, layer, component, src, dst, size = key
        t = (30000 if phase == 0 else 60000)+200+phase_indices[phase]*50
        b = dict(index=len(batches), phase=phase, worker=worker, layer=layer,
                 component=component, source_offset=src, destination_offset=dst, bytes=size,
                 first_id=len(batches)+2, observed_done=len(batches)+2, issued=tasks,
                 completed=tasks, control=0, begin=tick(t), end=tick(t+30), status=0, quiet=1)
        batches.append(b);phase_indices[phase] += 1
    leaves = []
    for pool in range(4):
        for half in range(2):
            pa = 0x84000000+pool*4194304+half*2097152
            leaves.append(dict(index=len(leaves), pool=pool, extent=pool,
                               iova=0x40000000+pool*4194304+half*2097152, pa=pa,
                               bytes=2097152, pte=(pa//4096 << 10)|0xC7,
                               table_count=2, table_pa=[0x90000000, 0x90001000, 0, 0, 0], reserved=0))
    observers, comps = [], []
    for _ in range(2):
        o = [0]*35;o[0]=100;o[1]=100;o[10]=100;o[7]=432;o[8]=432;o[9]=1
        o[14]=864;o[15]=864;o[16]=864;o[17]=864
        observers.append(o)
        c = [0]*128;c[0]=0x49444d4143504e01;c[1]=1;c[2]=0xA00;c[5]=1;c[6]=4;c[10]=0xA00;c[34]=3
        comps.append(c)
    r = dict(status=0, release_status=0, stage=11, quarantined=0, allocation_kind=1,
             extent_count=4, batch_count=144, leaf_count=8, worker_layers=24, joint_layers=12,
             backing_bytes=v.BACKING, reference_bytes=2*v.POOL,
             source_bytes_verified=2*v.POOL, destination_bytes_verified=2*v.POOL,
             guard_bytes_verified=v.GUARD_COVERAGE, data_errors=0,
             swapout_bytes=v.PHASE_BYTES, restore_bytes=v.PHASE_BYTES,
             swapout_tasks=432, restore_tasks=432, swapout_logical_runs=48, restore_logical_runs=96,
             stages=stages, batches=batches, leaves=leaves,
             layers=[dict(worker=w, layer=l, ready=tick(70200+(w*12+l)*100))
                     for w in range(2) for l in range(12)],
             hpm_before=[[0]*7 for _ in range(2)], hpm_after=[[100]*7 for _ in range(2)],
             observer=observers, observer_status=[0, 0], composition=comps)
    pools = []
    for p in range(4):
        ref = f'worker{p//2}-'+('restored_gpu.bin' if p%2 else 'cpu_source.bin')
        pools.append(dict(pool=p, worker=p//2, file=f'pool{p}.bin', sha256=v.HASHES[ref],
                          expected_asset=ref, expected_sha256=v.HASHES[ref], bytes=v.POOL,
                          memcmp_bytes=v.POOL, memcmp_equal=True, first_mismatch=None,
                          memcmp_errno=0, write_errno=0))
    return dict(schema=v.SCHEMA, kind='run', device='/dev/idma0',
                timing_semantics={'physical_calibration_claim': False},
                ioctl=dict(attempted=True, run_calls=1, return_=0, errno=0), request=q, run=r,
                input_assets=[dict(file=f, bytes=v.POOL, sha256=h) for f, h in v.HASHES.items()],
                pools=pools, raw_write_errno=[0, 0, 0],
                userspace_memory=dict(seed_reference_bytes=2*v.POOL, output_pool_bytes=4*v.POOL,
                                      batch_record_bytes=192*136, leaf_record_bytes=4096*88),
                host_cost_ns=dict(data_load_and_allocation=100, device_open_ioctl_close=100,
                                  hash_and_full_memcmp=100, raw_output_write=100, total_before_receipt=400))


def set_raw(obj, values):
    if isinstance(obj, C.Structure):
        for name, *_ in obj._fields_:
            value = values[name]
            field = getattr(obj, name)
            if isinstance(field, (C.Array, C.Structure)):
                set_raw(field, value)
            else:
                setattr(obj, name, value)
    elif isinstance(obj, C.Array):
        for i, value in enumerate(values):
            if isinstance(obj[i], (C.Array, C.Structure)):
                set_raw(obj[i], value)
            else:
                obj[i] = value


def write_raw(record, dest):
    q, r = record['request'], record['run']
    fields = {**q, **r}
    fields['stages'] = [v.without_index(x) for x in r['stages']]
    run = v.RawRun();set_raw(run, fields)
    batches = (v.RawBatch*192)();set_raw(batches, [v.without_index(x) for x in r['batches']])
    leaves = (v.RawLeaf*4096)();set_raw(leaves, [v.without_index(x) for x in r['leaves']])
    for file, obj in [('uapi-run.bin', run), ('uapi-batches.bin', batches), ('uapi-leaves.bin', leaves)]:
        (dest/file).write_bytes(bytes(obj))


def main():
    dest = ROOT/'validation/receipt-validator-tests'
    dest.mkdir(parents=True, exist_ok=True)
    baseline = make_fixture();baseline['ioctl']['return'] = baseline['ioctl'].pop('return_')
    for pool in baseline['pools']:
        shutil.copyfile(ROOT/'data'/pool['expected_asset'], dest/pool['file'])
    write_raw(baseline, dest)
    result = v.validate(baseline, dest, ROOT/'data')
    checks = [dict(name='complete_synthetic_current_receipt', accepted=True)]
    cases = [
        ('missing_last_layer', lambda x: x['run']['layers'].pop()),
        ('cross_shard_pool_identity', lambda x: x['pools'][0].update(expected_asset='worker1-cpu_source.bin')),
        ('wrong_dma_bytes', lambda x: x['run'].update(restore_bytes=v.PHASE_BYTES-4096)),
        ('nonquiet_batch', lambda x: x['run']['batches'][-1].update(quiet=0)),
        ('checked_release_fence_failure', lambda x: x['run']['stages'][9].update(status=-5)),
        ('quarantined_owner', lambda x: x['run'].update(quarantined=1)),
        ('next_id_missing', lambda x: x['run']['batches'][0].update(first_id=0)),
        ('done_id_wrong_batch', lambda x: x['run']['batches'][0].update(observed_done=3)),
        ('composition_not_quiet', lambda x: x['run']['composition'][1].__setitem__(5, 0)),
        ('observer_closing_outstanding', lambda x: x['run']['observer'][1].__setitem__(32, 1)),
        ('source_destination_pa_alias', lambda x: x['run']['leaves'][2].update(pa=x['run']['leaves'][0]['pa'], pte=x['run']['leaves'][0]['pte'])),
        ('duplicate_logical_byte_coverage', lambda x: x['run']['batches'][1].update(source_offset=x['run']['batches'][0]['source_offset'], destination_offset=x['run']['batches'][0]['destination_offset'], component=x['run']['batches'][0]['component'])),
        ('premature_layer_ready', lambda x: x['run']['layers'][0].update(ready=tick(60000))),
        ('ioctl_errno_preserved', lambda x: x['ioctl'].update(errno=5, **{'return': -1})),
    ]
    for name, mutation in cases:
        record = copy.deepcopy(baseline);mutation(record)
        try:
            v.validate(record, dest, ROOT/'data')
            raise AssertionError(f'{name}: invalid current receipt accepted')
        except v.Rejection as exc:
            checks.append(dict(name=name, accepted=False, reason=str(exc)))
    # Damage a real copied tail-padding byte, leaving every semantic field intact.
    pool_path = dest/baseline['pools'][1]['file']
    with pool_path.open('r+b') as f:
        f.seek(6*12288+16);byte=f.read(1);f.seek(6*12288+16);f.write(bytes([byte[0]^1]))
    try:
        v.validate(baseline, dest, ROOT/'data')
        raise AssertionError('tail-padding omission accepted')
    except v.Rejection as exc:
        checks.append(dict(name='damaged_tail_padding_actual_binary', accepted=False, reason=str(exc)))
    shutil.copyfile(ROOT/'data'/baseline['pools'][1]['expected_asset'], pool_path)
    with (dest/'uapi-run.bin').open('r+b') as f:
        f.seek(v.RawRun.restore_bytes.offset);f.write((v.PHASE_BYTES-4096).to_bytes(8, 'little'))
    try:
        v.validate(baseline, dest, ROOT/'data')
        raise AssertionError('raw-vs-JSON disagreement accepted')
    except v.Rejection as exc:
        checks.append(dict(name='raw_uapi_json_disagreement', accepted=False, reason=str(exc)))
    write_raw(baseline, dest)
    (dest/'receipt-synthetic.json').write_text(json.dumps(baseline, indent=2)+'\n')
    summary = {'scope': 'synthetic current receipt-validator unit checks; no board/ioctl/replay',
               'old_stage106_matrix_executed': False, 'board_executed': False,
               'positive_checks': 1, 'negative_checks': len(checks)-1, 'checks': checks,
               'synthetic_acceptance_result': result}
    (ROOT/'validation/receipt-validator-tests.json').write_text(json.dumps(summary, indent=2)+'\n')
    print(json.dumps(summary, indent=2))


if __name__ == '__main__':
    main()
