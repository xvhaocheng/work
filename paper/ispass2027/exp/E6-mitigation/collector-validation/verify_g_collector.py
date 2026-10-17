#!/usr/bin/env python3
"""Bounded synthetic protocol fixtures for the actual G collector/parser."""
import argparse
from contextlib import redirect_stdout
from copy import deepcopy
import hashlib
import io
import json
from pathlib import Path
import sys

sys.dont_write_bytecode = True
HERE = Path(__file__).resolve().parent
sys.path.insert(0, str(HERE.parent))
import collect_g as collector
import g_records as records

BOOT = '12345678-1234-1234-1234-123456789abc'


def fixture(cli, sequence=42):
    row = deepcopy(cli)
    row['fixture_only'] = True
    row['result_buffer']['observer_sequence'] = sequence
    row['result_buffer']['old_observer_status'] = '0x0000000000000004'
    w = [int(x, 16) for x in row['result_buffer']['words_hex']]
    has_a = row['condition'] != 'solo'
    vcount, acount = 8192, w[18]*2
    active = 100 if acount else 0
    time = dict(valid=1, status=4, v_did=10, a_did=11 if has_a else 0xffffffff,
        cycles=w[10], h=w[10]-4095, gap=4095, hmax=1000, gapmax=1,
        issued=4096, done=4096, batch_polled=1, done_to_read=10)
    axi_v = dict(neither=0, v_only=w[10]-active, a_only=0, both=active,
        ar=vcount, aw=vcount, rlast=vcount, b=vcount, ar_wait=0, aw_wait=0, start=0, end=0)
    axi_a = dict(ar=acount, aw=acount, rlast=acount, b=acount, ar_wait=0, aw_wait=0,
        start=0, end=0, shared_ar=vcount+acount, shared_aw=vcount+acount, shared_ar_wait=0, shared_aw_wait=0)
    lines = [f'idma_hist_info seq={sequence} valid=1 version=4 schema=1 bins=32 count_bits=32 time_bits=32 overflow=0 raw=0x20202001']
    paths = []
    for role in 'va':
        for channel in 'rw':
            for stage in range(4):
                full = vcount if role == 'v' else acount
                path = dict(valid=1, entered=full, full=full, carry=0, sum=full, max=1 if full else 0, start=0, end=0, area=full)
                prefix = f'seq={sequence} role={role} channel={channel} stage={stage}'
                counts = [0, full]+[0]*30
                lines.append('idma_path_hist '+prefix+f' valid=1 schema=1 bins=32 full={full} total={full} overflow=0 counts='+','.join(map(str, counts)))
                paths.append('idma_path_obs '+prefix+' '+' '.join(f'{k}={v}' for k, v in path.items()))
    for part, values in [('time', time), ('axi_v', axi_v), ('axi_a', axi_a)]:
        lines.append(f'idma_batch_obs seq={sequence} part={part} '+' '.join(f'{k}={v}' for k, v in values.items()))
    lines += paths
    lines.append(f'idma_g_obs seq={sequence} valid=1 status=0x4 validation_errors=0 hw_errors=0')
    lines += [f'idma_g_word seq={sequence} index={768+i} raw={x:#x}' for i, x in enumerate(w)]
    return '\n'.join(['KERNEL='+records.RELEASE, 'BOOT='+BOOT, 'TAINT=0', f'PRE_SEQ={sequence-1}',
        json.dumps(row), 'CASE_EXIT=0', *lines, 'TAINT_AFTER=0', ''])


def main():
    p = argparse.ArgumentParser(description=__doc__)
    p.add_argument('--cli-report', required=True, type=Path)
    p.add_argument('--output', required=True, type=Path)
    a = p.parse_args(); a.output.mkdir(parents=True, exist_ok=False)
    cli = json.loads(a.cli_report.read_text())
    conditions = {c['record']['condition']:c['record'] for c in cli['cases']
        if 'record' in c and c.get('mode') == '' and c['record']['success'] and c['record']['run_attempted']}
    require = records.require; checks = []
    bursts = {64:1, 1024:1, 4096:2}

    def accept(name, text):
        row = records.parse(text, name, bursts)
        checks.append({'case':name, 'result':'accepted'})
        return row

    def reject(label, text, condition='a8-gap1000'):
        try: records.parse(text, condition, bursts)
        except (ValueError, KeyError, TypeError, IndexError, json.JSONDecodeError) as e:
            checks.append({'case':label, 'result':'rejected', 'reason':str(e)})
        else: raise AssertionError('accepted '+label)

    for name in records.CONDITIONS: accept(name, fixture(conditions[name]))
    for mode in ['A-before', 'A-inside']:
        row = next(c['record'] for c in cli['cases'] if c.get('mode') == mode)
        accept('a8-gap0', fixture(row))
    base = fixture(conditions['a8-gap1000'])
    rejected = {
        'taint-before':base.replace('TAINT=0', 'TAINT=1'),
        'taint-after':base.replace('TAINT_AFTER=0', 'TAINT_AFTER=1'),
        'wrong-kernel':base.replace(records.RELEASE, 'wrong-kernel'),
        'duplicate-boot':base.replace('BOOT='+BOOT, 'BOOT='+BOOT+'\nBOOT='+BOOT),
        'bad-exit':base.replace('CASE_EXIT=0', 'CASE_EXIT=1'),
        'stale-watermark':base.replace('PRE_SEQ=41', 'PRE_SEQ=42'),
        'duplicate-CLI':base.replace('CASE_EXIT=0', json.dumps(conditions['a8-gap1000'])+'\nCASE_EXIT=0'),
        'final-invalid':base.replace('idma_g_obs seq=42 valid=1', 'idma_g_obs seq=42 valid=0'),
        'final-error':base.replace('validation_errors=0 hw_errors=0', 'validation_errors=1 hw_errors=0'),
        'hw-error':base.replace('validation_errors=0 hw_errors=0', 'validation_errors=0 hw_errors=1'),
        'old-final-invalid':base.replace('part=time valid=1', 'part=time valid=0'),
        'old-final-abort':base.replace('part=time valid=1 status=4', 'part=time valid=1 status=12'),
        'missing-word':base.replace('idma_g_word seq=42 index=799 raw=0x0\n', ''),
        'word-mismatch':base.replace('index=768 raw=0x49444d4150344d02', 'index=768 raw=0x1'),
        'duplicate-word-conflict':base.replace('TAINT_AFTER=0', 'idma_g_word seq=42 index=799 raw=1\nTAINT_AFTER=0'),
        'word-65bit':base.replace('index=799 raw=0x0', 'index=799 raw=0x10000000000000000'),
        'newer-G':base.replace('TAINT_AFTER=0', 'idma_g_obs seq=43 valid=1\nTAINT_AFTER=0'),
        'no-hist-info':base.replace('idma_hist_info seq=42', 'other_hist_info seq=42'),
        'hist-overflow':base.replace('overflow=0 counts=', 'overflow=1 counts=', 1),
        'hist-total':base.replace('total=8192', 'total=8191', 1),
        'path-invalid':base.replace('stage=0 valid=1 entered=', 'stage=0 valid=0 entered=', 1),
        'path-stock':base.replace('entered=8192 full=8192', 'entered=8191 full=8192', 1),
        'missing-boundary':base.replace('TAINT_AFTER=0', ''),
        'duplicate-watermark':base.replace('PRE_SEQ=41', 'PRE_SEQ=41\nPRE_SEQ=41'),
    }
    for name, text in rejected.items(): reject(name, text)
    # Mutate actual ABI inputs/results while preserving dmesg. Decode must reject
    # software failures, count truncation, unsafe lanes and raw bank corruption.
    original = conditions['a8-gap1000']
    mutations = [('success', False), ('run_return', -1), ('run_errno', 4)]
    for key, value in mutations:
        row = deepcopy(original); row[key] = value; reject('CLI-'+key, fixture(row))
    for key in ['status', 'observer_validation_errors', 'hpm_configured', 'measurement_complete',
                'prepared_lane_mask', 'quiet_lane_mask', 'retained_lane_mask', 'verified_lane_mask', 'reserved_output']:
        row = deepcopy(original); row['result_buffer'][key] ^= 1; reject('result-'+key, fixture(row))
    for role in ['victim', 'aggressor']:
        for key in ['submitted', 'completed', 'completed_bytes', 'data_errors', 'quarantined']:
            row = deepcopy(original); row['result_buffer'][role][key] ^= 1; reject(role+'-'+key, fixture(row))
    for key in ['warmup_completed', 'warmup_bytes']:
        for lane in range(2):
            row = deepcopy(original); row['result_buffer'][key][lane] ^= 1; reject(key+str(lane), fixture(row))
    for i in range(32):
        if i in [10, 13, 16, 27, 28, 29, 30]: continue
        row = deepcopy(original); n = int(row['result_buffer']['words_hex'][i], 16) ^ (1 << 63)
        row['result_buffer']['words_hex'][i] = f'0x{n:016x}'; reject('raw'+str(i), fixture(row))
    for key in ['active_cycles', 'iotlb_miss', 's1_ptw', 's2_ptw', 'in_flight_at_end']:
        row = deepcopy(original); del row['result_buffer']['aggressor'][key]
        reject('missing A stat '+key, fixture(row))
    for label, change in [
        ('boolean input', lambda x: x['input'].__setitem__('victim_pages', True)),
        ('boolean cap', lambda x: x['caps_buffer'].__setitem__('victim_pages', True)),
        ('fractional software time', lambda x: x['result_buffer'].__setitem__('snapshot_before_ns', 0.5)),
        ('oversized u32 stat', lambda x: x['result_buffer']['aggressor'].__setitem__('in_flight_at_end', 1 << 32))]:
        row = deepcopy(original); change(row); reject(label, fixture(row))
    require(len(collector.plan()) == 40 and all({x['condition'] for x in collector.plan() if x['block']==b} == set(records.CONDITIONS) for b in range(10)), 'fixed40 plan')
    for point in collector.plan():
        cmd = collector.command(point)
        require('idma_g --device /dev/idma0 --condition '+point['condition'] in cmd and '--timeout-ms 60000' in cmd, 'planned command')
    checks.append({'case':'40 fixed command bindings', 'result':'passed'})
    paths = {}; inputs = a.output/'inputs'; inputs.mkdir()
    sw = json.loads((HERE.parent/'software-source/build01/build-manifest.json').read_text())
    synth = json.loads((HERE.parent/'build/synth-stage01/e6-g-synth-evidence.json').read_text())
    fpga = dict(status='passed', fixture_only=True, direct_ds_wire=True, selected_cva6=collector.CVA6,
        parameters=dict(clock_hz=50000000, ds_cycles=[0,0], batch_version=2, observer_version=4, PathDepth=4,
                        histogram_stages=16, histogram_bins=32, histogram_counter_bits=32, histogram_scheme=1,
                        top_generic={'IOMMUGapEnable':1}), actual_vivado_top_generic={'IOMMUGapEnable':1},
        synthesized_configuration=synth, artifacts={'synthetic.bit':'0'*64})
    bound = dict(load=dict(status='boot_identity_verified', fixture_only=True, kernel_release=records.RELEASE, boot_id=BOOT, bit_sha256='0'*64),
        software=sw, fpga=fpga, backend=dict(status='passed', burst_per_descriptor=bursts))
    for name, value in bound.items():
        path=inputs/(name+'.json'); path.write_text(json.dumps(value)+'\n'); paths[name]=path
    class Console:
        def __init__(self, fault=None): self.calls=0; self.closed=False; self.fault=fault
        def command(self, cmd, timeout):
            point=collector.plan()[self.calls]; self.calls+=1
            require(timeout==120 and cmd==collector.command(point), 'one bounded command')
            text=fixture(conditions[point['condition']], self.calls)
            if self.calls==2 and self.fault=='boot': text=text.replace(BOOT, '87654321-4321-4321-4321-cba987654321')
            if self.calls==2 and self.fault=='sequence': text=text.replace('PRE_SEQ=1','PRE_SEQ=2').replace('seq=2','seq=3').replace('"observer_sequence": 2','"observer_sequence": 3')
            if self.calls==2 and self.fault=='command': cmd+=' changed'
            if self.calls==2 and self.fault=='exit': return dict(command=cmd, stdout=text, exit_code=1)
            return dict(command=cmd, stdout=text, exit_code=0)
        def close(self): self.closed=True
    console=Console()
    with redirect_stdout(io.StringIO()):
        m=collector.collect(a.output/'full40', paths, Path('/NO_HARDWARE_IMPORT'), _factory=lambda p:console, _fixture=True)
    require(console.calls==40 and console.closed and m['status']=='passed' and m['fixture_only'] is True, 'finite full40')
    checks.append({'case':'actual collector full40 fake console', 'result':'passed'})
    for fault in ['boot','sequence','command','exit']:
        console=Console(fault)
        try:
            with redirect_stdout(io.StringIO()): collector.collect(a.output/('failure-'+fault), paths, Path('/NO_HARDWARE_IMPORT'), _factory=lambda p:console, _fixture=True)
        except ValueError: pass
        else: raise AssertionError('collector accepted '+fault)
        require(console.calls==2 and console.closed, 'failed trial must stop without retry')
        m=json.loads((a.output/('failure-'+fault)/'manifest.json').read_text())
        require(m['status']=='failed' and m['recorded_cases']==1 and (a.output/('failure-'+fault)/'raw-02.json').exists(), 'raw before failure')
        checks.append({'case':'collector stops '+fault, 'result':'passed'})
    try: collector.bindings(paths)
    except ValueError: checks.append({'case':'actual collector refuses synthetic input before hardware import', 'result':'passed'})
    else: raise AssertionError('synthetic accepted')
    for name, value in [('load', 'bad-kernel'), ('software','bad-driver'), ('fpga','running')]:
        old=deepcopy(bound[name]); bad=deepcopy(old)
        if name=='load':bad['kernel_release']=value
        if name=='software':bad['kernel_critical_sources']['drivers/misc/idma.c']=value
        if name=='fpga':bad['status']=value
        paths[name].write_text(json.dumps(bad)+'\n')
        try: collector.bindings(paths, True)
        except ValueError: checks.append({'case':'binding refuses '+value, 'result':'passed'})
        else:raise AssertionError(value)
        paths[name].write_text(json.dumps(old)+'\n')
    report=dict(status='passed', fixture_only=True, evidence_level='synthetic_protocol_fixture',
        scope='actual parser and collector with finite synthetic logs/Console; no board', checks=checks, check_count=len(checks),
        source_sha256={p.name:hashlib.sha256(p.read_bytes()).hexdigest() for p in [HERE.parent/'g_records.py',HERE.parent/'collect_g.py',Path(__file__)]})
    (a.output/'report.json').write_text(json.dumps(report,indent=2)+'\n')
    print(json.dumps({'status':'passed','checks':len(checks),'report':str(a.output/'report.json')}))


if __name__=='__main__': main()
