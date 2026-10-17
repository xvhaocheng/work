#!/usr/bin/env python3
"""Root-only fixed 40-point G collector; no retry, refill, reload or SD action."""
import argparse
import datetime
import hashlib
import json
from pathlib import Path
import sys
import time
from g_records import BACKEND, CONDITIONS, RELEASE, parse, require, synthetic_markers

HERE = Path(__file__).resolve().parent
LINUX = '69d8b9d4549c473477bcdc1df7c09ee1bb55528c'
PLATFORM = '6491dfd76fc92dc020d17166b327da93605f58b1'
CVA6 = 'cfb9a0a8bb70b32be6036bc112a5189d6e2aa0ab'
DRIVER = 'dfdad8be6cc6ff1db7232c32694a57be30d7cbcdd4e7e794d2170bfcbae39158'


def sha(path): return hashlib.sha256(Path(path).read_bytes()).hexdigest()
def save(path, obj): path.write_text(json.dumps(obj, indent=2)+'\n')
def now(): return datetime.datetime.now(datetime.timezone.utc).isoformat()


def plan():
    return [{'case': block*4+index+1, 'block': block, 'condition': condition}
            for block in range(10)
            for index, condition in enumerate(CONDITIONS[block % 4:]+CONDITIONS[:block % 4])]


def command(point):
    require(point in plan(), 'G command must be a frozen planned point')
    return ('printf "KERNEL="; uname -r; printf "BOOT="; cat /proc/sys/kernel/random/boot_id; '
        'printf "TAINT="; cat /proc/sys/kernel/tainted; '
        "pre_seq=$(dmesg | sed -n 's/.*idma_batch_obs seq=\\([0-9]*\\).*/\\1/p' | tail -n 1); "
        'printf "PRE_SEQ=%s\\n" "${pre_seq:-0}"; '
        'platform-timeout 90 idma_g --device /dev/idma0 --condition '+point['condition']+' --timeout-ms 60000; '
        'case_exit=$?; printf "CASE_EXIT=%s\\n" "$case_exit"; dmesg; '
        'printf "TAINT_AFTER="; cat /proc/sys/kernel/tainted; test "$case_exit" -eq 0')


def bindings(paths, fixture=False):
    loaded, fpga, software = (json.loads(paths[k].read_text()) for k in ('load', 'fpga', 'software'))
    require(fixture or not any(synthetic_markers(json.loads(p.read_text())) for p in paths.values()), 'G synthetic source forbidden')
    require(loaded['status'] == 'boot_identity_verified' and loaded['kernel_release'] == software['kernel_release'] == RELEASE, 'G verified load/kernel required')
    require(software['status'] == 'passed' and software['selected_linux'] == LINUX and software['selected_platform'] == PLATFORM and software['selected_cva6'] == CVA6 and software['selected_cva6_reference_only'] is True, 'G frozen software tuple')
    require(software['kernel_critical_sources']['drivers/misc/idma.c'] == DRIVER, 'G accepted driver source')
    require(fpga['status'] == 'passed' and fpga['direct_ds_wire'] is True and fpga['selected_cva6'] == CVA6, 'G passed DS0 FPGA required')
    q = fpga['parameters']
    for key, expected in dict(clock_hz=50000000, ds_cycles=[0, 0], batch_version=2,
        observer_version=4, PathDepth=4, histogram_stages=16, histogram_bins=32,
        histogram_counter_bits=32, histogram_scheme=1, top_generic={'IOMMUGapEnable':1}).items():
        require(q[key] == expected, 'G FPGA '+key)
    require(fpga['actual_vivado_top_generic']['IOMMUGapEnable'] == 1, 'G actual Vivado generic')
    s = fpga['synthesized_configuration']
    require(s['passed'] is True and s['G_observer_instances'] == s['OBS_observer_instances'] == 1 and s['PATH_stage_instances'] == 16 and s['delay_instances'] == s['forbidden_research_instances'] == 0 and s['source_bound_observer_version'] == 4 and s['cpu_period_ns'] == 20, 'G actual synthesized hierarchy')
    require(len(s['stages']) == 16 and all(x == dict(head_register_bits=2, tail_register_bits=2, stamp_register_bits=128, stamp_slots=4, histogram_register_bits=1024, histogram_bins=32) for x in s['stages']), 'G synthesized Depth4 histogram')
    require([v for k, v in fpga['artifacts'].items() if k.endswith('.bit')] == [loaded['bit_sha256']], 'G actual loaded BIT identity')
    return loaded, BACKEND.backend_evidence(paths['backend'])


def collect(output, paths, board_tools, port=None, _factory=None, _fixture=False):
    loaded, bursts = bindings(paths, _fixture)
    require(bursts[4096] == 2, 'G COPY4096 backend must have two AR/AW transactions')
    output.mkdir(parents=True, exist_ok=False)
    manifest = {'schema':'ispass2027-g-collection-v1', 'status':'running', 'started_utc':now(),
        'fixture_only':_fixture, 'evidence_level':'synthetic_protocol_fixture' if _fixture else 'fpga_board',
        'protocol':'G_independent_W256_each_then_4096_each_natural_v1', 'planned_cases':plan(),
        'kernel_release':RELEASE, 'boot_id':loaded['boot_id'], 'bit_sha256':loaded['bit_sha256'],
        'ds':'0/0', 'clock_hz':50000000, 'recorded_cases':0,
        'script_sha256':sha(__file__), 'parser_sha256':sha(HERE/'g_records.py'),
        'helpers':{str(p.relative_to(HERE)):sha(p) for p in sorted((HERE/'collector-source').glob('*.py'))},
        'sources':{k:{'path':str(v.resolve()),'sha256':sha(v)} for k, v in paths.items()}}
    records = []; console = None; start = time.monotonic()
    save(output/'manifest.json', manifest)
    try:
        for point in plan():
            require(time.monotonic()-start < 5400-130, 'G total wall budget exhausted; stop without retry')
            if console is None:
                if _factory: console = _factory(output/'uart')
                else:
                    sys.path.insert(0, str(board_tools.resolve()))
                    from uart import Console, DEFAULT_PORT
                    console = Console(port or DEFAULT_PORT, output/'uart')
            manifest['attempting_case'] = point; save(output/'manifest.json', manifest)
            raw = console.command(command(point), 120)
            raw_path = output/f'raw-{point["case"]:02d}.json'
            save(raw_path, raw)  # Preserve every returned command before validation.
            require(raw['exit_code'] == 0 and raw['command'] == command(point), 'G command failure/mismatch; no retry')
            item = parse(raw['stdout'], point['condition'], bursts)
            require(_fixture or not synthetic_markers(item), 'G synthetic trial forbidden')
            require(item['boot_id'] == loaded['boot_id'], 'G boot changed')
            require(not records or item['sequence'] == records[-1]['sequence']+1, 'G duplicate/gapped sequence')
            item.update(point, raw_command_path=str(raw_path.resolve()), fixture_only=_fixture,
                        evidence_level=manifest['evidence_level'], ds='0/0')
            records.append(item); save(output/'records.json', records)
            manifest['recorded_cases'] = len(records); save(output/'manifest.json', manifest)
            print(f'{len(records)}/40 block{point["block"]} {point["condition"]} validated', flush=True)
        require(len(records) == 40, 'G full 40-point matrix required')
        manifest['status'] = 'passed'
    except BaseException as error:
        manifest.update(status='failed', error=repr(error)); raise
    finally:
        if console: console.close()
        manifest.update(finished_utc=now(), wall_seconds=time.monotonic()-start)
        save(output/'manifest.json', manifest)
    return manifest


def main():
    p = argparse.ArgumentParser(description=__doc__)
    for name in ['output', 'load-manifest', 'fpga-manifest', 'software-manifest', 'backend-evidence', 'board-tools']:
        p.add_argument('--'+name, type=Path, required=True)
    p.add_argument('--port')
    a = p.parse_args()
    collect(a.output.resolve(), {'load':a.load_manifest, 'fpga':a.fpga_manifest,
        'software':a.software_manifest, 'backend':a.backend_evidence}, a.board_tools, a.port)


if __name__ == '__main__': main()
