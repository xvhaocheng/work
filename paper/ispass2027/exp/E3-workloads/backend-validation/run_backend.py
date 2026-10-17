#!/usr/bin/env python3
"""One bounded build/run of the unchanged fixed-source iDMA AXI backend."""
import argparse
import datetime
import hashlib
import json
from pathlib import Path
import shutil
import subprocess
import sys
import time

HERE = Path(__file__).resolve().parent
ROOT = HERE.parents[4]
DEFAULT_CVA6 = Path('/home/xvhaocheng/work/iommu-platform/out/ispass2027-20261017-ds0-01/src/cva6')
CC = 'vendor/pulp-platform/common_cells'
AXI = 'vendor/pulp-platform/axi'
IDMA = 'corev_apu/iDMA'
IOMMU = 'corev_apu/riscv-iommu'
SOURCE_NAMES = [
    CC+'/src/cf_math_pkg.sv', AXI+'/src/axi_pkg.sv', IDMA+'/src/idma_pkg.sv',
    CC+'/src/fifo_v3.sv', CC+'/src/deprecated/fifo_v2.sv', CC+'/src/stream_fifo.sv',
    CC+'/src/spill_register_flushable.sv', CC+'/src/spill_register.sv',
    CC+'/src/fall_through_register.sv', CC+'/src/counter.sv', CC+'/src/delta_counter.sv',
    IDMA+'/src/idma_stream_fifo.sv', IDMA+'/src/idma_buffer.sv',
    IDMA+'/src/idma_error_handler.sv', IDMA+'/src/idma_channel_coupler.sv',
    IDMA+'/src/idma_axi_transport_layer.sv', IDMA+'/src/idma_axi_lite_transport_layer.sv',
    IDMA+'/src/idma_obi_transport_layer.sv', IDMA+'/src/idma_legalizer.sv', IDMA+'/src/idma_backend.sv',
]
HEADERS = [CC+'/include/common_cells/assertions.svh', CC+'/include/common_cells/registers.svh',
           AXI+'/include/axi/typedef.svh', IOMMU+'/include/axi-iommu/typedef.svh',
           IDMA+'/src/include/idma/typedef.svh', IDMA+'/src/include/idma/guard.svh']


def now():
    return datetime.datetime.now(datetime.timezone.utc).isoformat()


def digest(data):
    return hashlib.sha256(data).hexdigest()


def save(path, value):
    path.write_text(json.dumps(value, indent=2, sort_keys=True)+'\n')


def relative(path):
    return Path(path).resolve().relative_to(ROOT).as_posix()


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--cva6', type=Path, default=DEFAULT_CVA6)
    parser.add_argument('--out', type=Path, required=True)
    parser.add_argument('--verilator', default='verilator')
    parser.add_argument('--negative-evidence', type=Path,
                        help='Preserved failed run manifest for the separately excluded zero-length probe')
    args = parser.parse_args()
    out, cva6 = args.out.resolve(), args.cva6.resolve()
    out.relative_to(HERE)
    out.mkdir(parents=True, exist_ok=False)
    original_manifest = cva6.parents[1]/'build-manifest.json'
    original = json.loads(original_manifest.read_text())
    if original['selected_cva6'] != '8aac3fcd345af7686a55af1867783fd6b91a0069':
        raise ValueError('Only the fixed baseline CVA6 source is in scope')
    manifest = {'status': 'preparing', 'started_utc': now(),
                'source_cva6': str(cva6), 'original_build_manifest': str(original_manifest),
                'selected_cva6': original['selected_cva6'], 'selected_iommu': original['selected_iommu'],
                'source_files': [], 'commands': [], 'compile_timeout_seconds': 300,
                'simulation_timeout_seconds': 30, 'simulation_watchdog_cycles_per_case': 50000,
                'scope': 'P0 legal lengths 64/1024/4096 only, including boundary and SLVERR/drain cases; no global backend pass',
                'run_reason': 'Separate the authorized 13-case legal input domain from the preserved extra zero-length negative failure; do not modify RTL or erase the failure',
                'parameters': {'DataWidth':64, 'AddrWidth':64, 'AxiIdWidth':2, 'NumAxInFlight':8,
                    'BufferDepth':3, 'TFLenWidth':64, 'RAWCouplingAvail':False, 'MaskInvalidData':False,
                    'HardwareLegalizer':True, 'ErrorCap':'NO_ERROR_HANDLING', 'FPGA_EN':0}}
    manifest_path = out/'manifest.json'
    save(manifest_path,manifest)
    start = time.monotonic()
    try:
        frozen = out/'frozen/cva6'
        for name in SOURCE_NAMES+HEADERS+['core/include/cv64a6_imafdc_sv39_config_pkg.sv',
                                         IDMA+'/src/systems/cva6_reg/dma_core_wrap.sv',
                                         'corev_apu/fpga/src/ariane_peripherals_xilinx.sv']:
            source = cva6/name
            data = source.read_bytes()
            sha = digest(data)
            if original['sources']['cva6/'+name] != sha:
                raise ValueError('Source differs from actual fixed build input: '+name)
            dest = frozen/name
            dest.parent.mkdir(parents=True,exist_ok=True)
            dest.write_bytes(data)
            manifest['source_files'].append({'actual_path':str(source), 'frozen_path':relative(dest),
                                              'sha256':sha, 'matches_fixed_build_manifest':True})
        for name in ['ariane_config_stub.sv','backend_top.sv','backend_test.cpp','run_backend.py']:
            data = (HERE/name).read_bytes()
            dest = out/'frozen'/name
            dest.write_bytes(data)
            manifest['source_files'].append({'actual_path':relative(HERE/name), 'frozen_path':relative(dest),
                                              'sha256':digest(data), 'kind':'test_adapter_or_runner'})
        version = subprocess.run([args.verilator,'--version'],check=True,capture_output=True,text=True,timeout=10)
        manifest['verilator_version'] = version.stdout.strip()
        obj = out/'obj'
        command = [args.verilator,'--cc','--exe','--build','-j','8','-Wno-fatal',
                   '+define+ASSERTS_OFF','--top-module','backend_top','--Mdir',str(obj),
                   '-CFLAGS','-std=c++17 -O1']
        for name in [CC+'/include',AXI+'/include',IOMMU+'/include',IDMA+'/src/include']:
            command.append('-I'+str(frozen/name))
        command.append(str(out/'frozen/ariane_config_stub.sv'))
        command.extend(str(frozen/name) for name in SOURCE_NAMES)
        command.extend([str(out/'frozen/backend_top.sv'),str(out/'frozen/backend_test.cpp')])
        manifest.update(status='compiling')
        manifest['commands'].append({'argv':command,'cwd':str(HERE),'started_utc':now()})
        save(manifest_path,manifest)
        with (out/'build.log').open('wb') as log:
            built = subprocess.run(command,cwd=HERE,stdout=log,stderr=subprocess.STDOUT,timeout=300)
        manifest['commands'][-1].update(returncode=built.returncode,finished_utc=now())
        save(manifest_path,manifest)
        built.check_returncode()
        binary = obj/'Vbackend_top'
        manifest.update(status='running')
        manifest['commands'].append({'argv':[str(binary)],'cwd':str(HERE),'started_utc':now()})
        save(manifest_path,manifest)
        with (out/'events.jsonl').open('wb') as stdout, (out/'stderr.log').open('wb') as stderr:
            ran = subprocess.run([str(binary)],cwd=HERE,stdout=stdout,stderr=stderr,timeout=30)
        manifest['commands'][-1].update(returncode=ran.returncode,finished_utc=now())
        save(manifest_path,manifest)
        ran.check_returncode()
        summaries, final, bursts_observed = [], None, {}
        for line_number, line in enumerate((out/'events.jsonl').read_text().splitlines(),1):
            if not line.startswith('{'):
                continue
            event = json.loads(line)
            if event['event']=='case_summary': summaries.append(dict(event,raw_summary_line=line_number))
            if event['event'] in {'AR','AW'}:
                bursts_observed.setdefault((event['case'],event['event']),[]).append(dict(event,raw_event_line=line_number))
            if event['event']=='all_passed': final=event
        if len(summaries)!=13 or final!={'event':'all_passed','cases':13}:
            raise ValueError('Missing actual test completion evidence')
        bursts = {}
        by_length = {}
        for length in [64,1024,4096]:
            observed = next(c for c in summaries if c['case']=='aligned_'+str(length))
            descriptor = observed['descriptors'][0]
            if descriptor['ar']!=descriptor['aw'] or descriptor['read_bytes']!=length or descriptor['write_bytes']!=length:
                raise ValueError('Actual aligned backend counts/bytes mismatch')
            bursts[str(length)] = descriptor['ar']
            by_length[str(length)] = {k:observed[k] for k in ['ar','aw','rlast','b','r_beats','w_beats','read_bytes','write_bytes','submitted','completed','drained']}
            by_length[str(length)].update(ar_requests=bursts_observed[(observed['case'],'AR')],
                                          aw_requests=bursts_observed[(observed['case'],'AW')])
        if bursts!={'64':1,'1024':1,'4096':2}:
            raise ValueError('Actual burst counts differ from the proposed P0 collection contract')
        negative = None
        if args.negative_evidence is not None:
            path = args.negative_evidence.resolve()
            prior = json.loads(path.read_text())
            if prior['status'] != 'failed':
                raise ValueError('Negative control reference is not an actual failed run')
            negative = {'status':'failed','manifest':relative(path),
                        'stderr':relative(path.parent/'stderr.log'),'events':relative(path.parent/'events.jsonl'),
                        'finding':'zero-length request accepted while response stalled; combinational error response disappears on next legal input',
                        'source':'idma_backend.sv gen_reject_zero_transfers; unchanged fixed baseline',
                        'separate_from_p0_legal_domain':True}
        summary = {'status':'passed','scope':manifest['scope'],
                   'burst_per_descriptor':bursts,'by_length':by_length,
                   'actual_backend_cases':summaries,'case_count':len(summaries),
                   'raw_event_log':relative(out/'events.jsonl'),'build_manifest':relative(manifest_path),
                   'source_commits':{'cva6':manifest['selected_cva6'],'iommu':manifest['selected_iommu']},
                   'source_hashes':manifest['source_files'], 'commands':manifest['commands'],
                   'known_exclusions':{'zero_length':negative or 'outside legal input domain; not certified',
                       'other_lengths':'outside the P0 64/1024/4096 contract',
                       'upstream_requirement':'CLI/driver/batch RTL must reject zero and unsupported lengths; that check is not provided by this backend test'},
                   'limitations':['Actual production configuration has NO_ERROR_HANDLING: R/B SLVERR drains but is not propagated via idma_rsp.error.',
                                  'Backend has no STOP input: stop case only ceases new descriptor supply and drains accepted descriptors; frontend/driver STOP remains separate.',
                                  'AXI slave is a bounded software memory model with stalls; this is functional RTL evidence, not FPGA timing or performance.',
                                  'Only the selected FPGA_EN=0 configuration constant is stubbed; compiled backend and FIFO/datapath sources are unmodified frozen bytes.',
                                  'Vendor internal SVA is disabled as in the finite harness; external handshakes, stalled payload stability, byte contents, guards and drain are checked by the C++ driver.']}
        save(out/'summary.json',summary)
        manifest['status']='passed'
        print(json.dumps({'status':'passed','cases':len(summaries),'burst_per_descriptor':bursts,'summary':relative(out/'summary.json')}))
    except BaseException as error:
        manifest.update(status='failed',error=type(error).__name__+': '+str(error))
        raise
    finally:
        manifest.update(finished_utc=now(),elapsed_seconds=time.monotonic()-start)
        save(manifest_path,manifest)


if __name__=='__main__':
    main()
