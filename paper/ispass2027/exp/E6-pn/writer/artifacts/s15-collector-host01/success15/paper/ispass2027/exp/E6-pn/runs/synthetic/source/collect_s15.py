#!/usr/bin/env python3
"""Root's finite S15 collector: original CLI, no MODE/GAP writes or reloads."""
import argparse
import datetime
import importlib.util
import json
from pathlib import Path
import re
import sys

sys.dont_write_bytecode = True
if not __debug__:
    raise RuntimeError('Python -O is unsupported: original parser assertions are required')

HERE = Path(__file__).resolve().parent
BOARD = HERE.parents[2] / 'board'


def module(path, name):
    spec = importlib.util.spec_from_file_location(name, path)
    result = importlib.util.module_from_spec(spec)
    spec.loader.exec_module(result)
    return result


gate = module(HERE / 's15_gate.py', 'pn_s15_gate_for_collection')


def now():
    return datetime.datetime.now(datetime.timezone.utc).isoformat()


def save(path, value):
    path.write_text(json.dumps(value, indent=2, allow_nan=False) + '\n')


def preflight(args):
    """Read existing identities only; never imports or constructs a UART console."""
    root, out = args.source_root.resolve(), args.output.resolve()
    gate.require(not out.exists(), 'new output directory required; no resume or replacement')
    gate.require(any(out.is_relative_to(root / p) for p in (
        'paper/ispass2027/board/runs', 'paper/ispass2027/exp/E6-pn/runs')),
        'S15 output must be in the task board/experiment run directory')
    reader = gate.Inputs(root, out, False)
    mapping_path = reader.resolve(HERE.parent / 'tool-source-map.json')
    mapping_raw = mapping_path.read_bytes()
    gate.require(gate.sha(mapping_raw) == gate.MAP_SHA, 'accepted historical map changed')
    reader.observed[mapping_path] = mapping_raw
    mapping = json.loads(mapping_raw)
    _, parse, parser_identity = gate.parser(reader, mapping)
    gate.software(reader, mapping, args.software_manifest)
    _, source = reader.read(args.source_manifest)
    _, fpga = reader.read(args.fpga_manifest)
    _, loaded = reader.read(args.load_manifest)
    for value in (args.expected_cva6, args.expected_iommu):
        gate.require(bool(re.fullmatch(r'[0-9a-f]{40}', value)), 'Root must pin full commits')
    gate.require(args.expected_cva6 != mapping['baseline_identity']['cva6_commit'] and
                 args.expected_iommu != mapping['baseline_identity']['iommu_commit'],
                 'new PN source identities required')
    gate.require(fpga['status'] == 'passed' and fpga['action'] == 'fpga-ds0' and
                 fpga['variant'] == 'ds0', 'qualified new DS0 FPGA build required')
    timing = fpga['timing']
    gate.require(timing['wns_ns'] >= 0 and fpga['routing_errors'] == 0 and
                 all(timing[k] == 0 for k in ('setup_failing', 'hold_failing', 'pulse_failing')),
                 'original timing/routing acceptance required')
    gate.require(fpga['parameters']['clock_hz'] == 50000000 and
                 fpga['parameters']['ds_cycles'] == [0, 0] and fpga['direct_ds_wire'] is True,
                 '50MHz original DS0 wire required')
    gate.require(fpga['selected_cva6'] == source['selected_cva6'] == args.expected_cva6 and
                 fpga['selected_iommu'] == source['iommu'] == args.expected_iommu,
                 'Root-pinned final candidate mismatch')
    gate.require(source['linux'] == gate.LINUX and type(source['reset_mode']) is int and
                 source['reset_mode'] == 0 and type(source['v_gap_reset_cycles']) is int and
                 source['v_gap_reset_cycles'] == 0, 'original software and resetS/gap0 required')
    caps = [gate.cap_value(source[k]) for k in ('pn_cap', 'gap_cap')]
    source_path = reader.resolve(args.source_manifest)
    gate.require(reader.resolve(fpga['source_manifest']) == source_path and
                 fpga['source_manifest_sha256'] == gate.sha(reader.observed[source_path]),
                 'build must reference the actual source manifest')
    bits = [v for p, v in fpga['artifacts'].items() if p.endswith('.bit')]
    gate.require(len(bits) == 1 and bool(re.fullmatch(r'[0-9a-f]{64}', bits[0])),
                 'unique existing qualified BIT identity required')
    gate.require(loaded['status'] == 'boot_identity_verified' and
                 loaded['program_file_sha256_verified'] is True and
                 loaded['bit_sha256'] == bits[0] and loaded['kernel_release'] == gate.KERNEL,
                 'matching completed new PN load and original kernel required')
    gate.require(bool(re.fullmatch(r'[0-9a-f-]{36}', loaded['boot_id'])), 'actual boot UUID required')
    reader.unchanged()
    return dict(reader=reader, parse=parse, parser_identity=parser_identity,
                source=source, fpga=fpga, loaded=loaded, caps=caps, bit=bits[0])


def collect(args, *, _console_factory=None, _prepared=None):
    # Private injection is for deterministic host tests only; no public fixture flag.
    prepared = preflight(args) if _prepared is None else _prepared
    fixture = _console_factory is not None or _prepared is not None
    reader, parse = prepared['reader'], prepared['parse']
    loaded, out = prepared['loaded'], args.output.resolve()
    gate.require(not out.exists(), 'new output required')
    reader.unchanged()
    out.mkdir(parents=True, exist_ok=False)
    snapshots = out / 'source'
    snapshots.mkdir()
    script_raw = Path(__file__).read_bytes()
    (snapshots / 'collect_s15.py').write_bytes(script_raw)
    (snapshots / 's15_gate.py').write_bytes((HERE / 's15_gate.py').read_bytes())
    uart_path = BOARD / 'uart.py'
    (snapshots / 'uart.py').write_bytes(uart_path.read_bytes())
    manifest = dict(phase='e6-pn-s15', status='running', ds='0/0', started_utc=now(),
                    kernel_release=gate.KERNEL, boot_id=loaded['boot_id'],
                    bit_sha256=prepared['bit'], planned_cases=gate.plan(), control_actions=[],
                    script_source=str(snapshots / 'collect_s15.py'),
                    script_sha256=gate.sha(script_raw), recorded_cases=0,
                    execution_script=str(Path(__file__).resolve()), execution_argv=sys.argv,
                    parser=prepared['parser_identity'], automatic_retries=0)
    if fixture:
        manifest.update(fixture_only=True, source_board_cases=0)
    for field in ('source_manifest', 'fpga_manifest', 'load_manifest', 'software_manifest'):
        path = reader.resolve(getattr(args, field))
        manifest[field] = str(path)
        manifest[field + '_sha256'] = gate.sha(reader.observed[path])
    manifest['dependency_sources'] = [dict(path=str(p), sha256=gate.sha(p.read_bytes()))
        for p in (snapshots / 's15_gate.py', snapshots / 'uart.py')]
    records, console = [], None
    save(out / 'records.json', records)
    save(out / 'manifest.json', manifest)
    try:
        factory = _console_factory
        if factory is None:
            uart = module(uart_path, 'pn_s15_uart')
            factory = uart.Console
        console = factory(args.port, out / 'uart')
        raw = console.command(gate.READ_COMMAND, 60)
        pre = gate.readback(reader, raw['raw_path'], loaded['boot_id'], prepared['caps'])
        manifest['pre_readback'] = pre['path']
        save(out / 'manifest.json', manifest)
        previous = pre['observer_watermark']
        for index, (block, condition) in enumerate(gate.plan()):
            reader.unchanged()
            raw = console.command(gate.command(condition), 180)
            gate.require(raw['exit_code'] == 0, 'CLI wrapper failed; no retry or later point')
            item = parse(raw['stdout'])
            gate.require(not gate.markers(item) or fixture, 'synthetic CLI cannot qualify')
            gate.require(item['boot_id'] == loaded['boot_id'] and
                         item['sequence'] == previous + 1, 'boot/observer sequence changed')
            pages = 1 if condition == 'Solo' else int(condition[1:])
            gate.require(item['record']['input']['aggressor_pages'] == pages and
                         (item['record']['result']['aggressor'] is None) == (condition == 'Solo'),
                         'actual condition differs from plan')
            previous = item['sequence']
            item.update(case=index + 1, block=block, condition=condition, ds='0/0',
                        raw_command_path=raw['raw_path'])
            if fixture:
                item.update(fixture_only=True, source_board_cases=0)
            records.append(item)
            manifest['recorded_cases'] = len(records)
            save(out / 'records.json', records)
            save(out / 'manifest.json', manifest)
            print(json.dumps(dict(case=index + 1, condition=condition,
                                 hardware_cycles=item['observer']['time']['cycles'])), flush=True)
        raw = console.command(gate.READ_COMMAND, 60)
        post = gate.readback(reader, raw['raw_path'], loaded['boot_id'], prepared['caps'])
        gate.require(post['observer_watermark'] == previous, 'post-read does not bracket15 points')
        manifest['post_readback'] = post['path']
        reader.unchanged()
        manifest['status'] = 'passed'
    except BaseException as error:
        manifest.update(status='failed', error=type(error).__name__ + ': ' + str(error))
        raise
    finally:
        try:
            if console is not None:
                console.close()
        except BaseException as error:
            manifest.update(status='failed', cleanup_error=type(error).__name__ + ': ' + str(error))
            manifest.setdefault('error', manifest['cleanup_error'])
            raise
        finally:
            manifest.update(finished_utc=now(), recorded_cases=len(records))
            save(out / 'manifest.json', manifest)
    return manifest


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    for name in ('source-root', 'output', 'source-manifest', 'fpga-manifest',
                 'load-manifest', 'software-manifest'):
        parser.add_argument('--' + name, type=Path, required=True)
    parser.add_argument('--expected-cva6', required=True)
    parser.add_argument('--expected-iommu', required=True)
    parser.add_argument('--port', default='/dev/serial/by-id/usb-FTDI_FT232R_USB_UART_AV0KRCHT-if00-port0')
    parser.add_argument('--check-only', action='store_true')
    args = parser.parse_args()
    if args.check_only:
        preflight(args)
        print(json.dumps(dict(status='manifest_checks_passed', hardware_access=False,
                              planned_cases=15, source_board_cases=0)))
        return
    collect(args)


if __name__ == '__main__':
    main()
