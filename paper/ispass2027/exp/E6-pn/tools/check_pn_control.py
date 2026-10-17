#!/usr/bin/env python3
"""Finite host-only actual-function/JSON checks and RV64 compile evidence."""
import argparse
import hashlib
import importlib.util
import json
from pathlib import Path
import re
import shutil
import subprocess
import sys
import tempfile

sys.dont_write_bytecode = True
HERE = Path(__file__).resolve().parent


def sha(path):
    return hashlib.sha256(Path(path).read_bytes()).hexdigest()


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--output', type=Path, required=True)
    parser.add_argument('--software-manifest', type=Path, required=True)
    args = parser.parse_args()
    out = args.output.resolve()
    out.mkdir(parents=True, exist_ok=False)
    commands = []
    checks = []

    def run(argv, log_name, env=None):
        result = subprocess.run([str(x) for x in argv], text=True,
                                stdout=subprocess.PIPE, stderr=subprocess.PIPE,
                                env=env)
        entry = {'argv': [str(x) for x in argv], 'exit_code': result.returncode,
                 'stdout': result.stdout, 'stderr': result.stderr}
        commands.append(entry)
        (out / log_name).write_text(json.dumps(entry, indent=2) + '\n')
        if result.returncode:
            raise RuntimeError(f'{log_name}: exit {result.returncode}; original output retained')
        return result.stdout

    def check(name, condition):
        if not condition:
            raise AssertionError(name)
        checks.append(name)

    receipt = {'schema': 'ispass2027.e6_pn.pn_control.host.v1',
               'fixture_only': True, 'synthetic': True, 'source_board_cases': 0,
               'board_executed': False, 'devmem_opened': False,
               'status': 'running', 'commands': commands, 'checks': checks}
    try:
        source_map = HERE.parent / 'tool-source-map.json'
        mapping = json.loads(source_map.read_text())
        expected = next(x['sha256'] for x in mapping['existing_evidence']
                        if x['path'].endswith('/manifests/software-build.json'))
        check('original software manifest bytes', sha(args.software_manifest) == expected)
        manifest = json.loads(args.software_manifest.read_text())
        original = [x['argv'] for x in manifest['commands']
                    if x['argv'][0] == 'riscv64-linux-gnu-gcc'
                    and any(str(y).endswith('/idma_experiment.c') for y in x['argv'])]
        check('unique original static compiler/profile', len(original) == 1 and
              original[0][1:5] == ['-O2', '-static', '-Wall', '-Wextra'])
        receipt['original_software'] = {'path': str(args.software_manifest.resolve()),
                                       'sha256': expected, 'cli_argv': original[0]}
        receipt['source_sha256'] = {p.name: sha(p) for p in (
            HERE / 'pn_control.c', HERE / 'check_pn_control.c',
            Path(__file__).resolve(), HERE / 's15_gate.py', source_map)}
        frozen = out / 'source-as-executed'
        frozen.mkdir()
        for name in receipt['source_sha256']:
            source = source_map if name == 'tool-source-map.json' else HERE / name
            shutil.copyfile(source, frozen / name)
        check('S15 source map retains original locked bytes',
              sha(source_map) == 'd9669346c23343117e2aa7408542c8c1ff7d6009abc6500b2fb9242dff2417fe')
        cc = shutil.which(original[0][0])
        check('original compiler name available', cc is not None)
        receipt['compiler'] = {'path': cc, 'sha256': sha(cc)}
        run([cc, '--version'], 'compiler-version.json')
        run([cc, '-dumpmachine'], 'compiler-target.json')
        run([cc, '-Q', '--help=target'], 'compiler-defaults.json')
        host = out / 'check-pn-control'
        run(['cc', '-O2', '-Wall', '-Wextra', '-Werror',
             HERE / 'check_pn_control.c', '-o', host], 'host-compile.json')
        text = run([host], 'host-execution.json')
        rows = [json.loads(line) for line in text.splitlines()]
        check('71 uniquely labeled finite C scenarios', len(rows) == 71 and
              len({x['test_case'] for x in rows}) == 71)
        by_name = {x['test_case']: x for x in rows}
        for row in rows:
            result = row.get('result', row)
            check('explicit fixture ' + row['test_case'],
                  result['fixture_only'] is True and result['source_board_cases'] == 0)
            if 'result' not in row:
                continue
            check('JSON protocol ' + row['test_case'],
                  result['schema'] == 'idma.pn_control.v1' and
                  result['read_only'] is True and result['word_bits'] == 64 and
                  result['mmio_base'] == 0x50000000 and
                  set(result['raw']) == {str(i) for i in range(192, 200)} and
                  result['success'] == (result['errno'] == 0))
            word = result['raw']['193']
            status = result['raw']['194']
            check('lossless mode/history decode ' + row['test_case'],
                  result['mode_word'] == word and
                  result['mode'] == (None if word is None else word & 3) and
                  result['history_shared'] == (None if word is None else bool(word & 4)) and
                  result['history_control_supported'] ==
                  (None if status is None else bool(status & (1 << 63))) and
                  result['partition_history'] ==
                  ({0: 'not_partitioned', 1: 'not_partitioned',
                    2: 'isolated', 3: 'isolated', 6: 'shared'}.get(word)))
        valid = by_name['valid-mode-0']['result']
        check('full-width raw194/199 survive uninterpreted',
              valid['raw']['194'] == 2**64 - 1 and
              valid['raw']['199'] == 0x8000000000001234)
        shared = by_name['valid-mode-6']['result']
        check('P_shared keeps word6 and exposes mode2/shared history',
              shared['success'] and shared['raw']['193'] == 6 and
              shared['mode_word'] == 6 and shared['mode'] == 2 and
              shared['history_shared'] and shared['history_control_supported'] and
              shared['partition_history'] == 'shared')
        unsupported = by_name['shared-without-history-support']['result']
        check('unsupported shared is failure with original evidence retained',
              not unsupported['success'] and unsupported['errno'] == 71 and
              unsupported['stage'] == 'history_support' and
              unsupported['raw']['193'] == 6 and unsupported['mode'] == 2 and
              unsupported['history_shared'] and
              not unsupported['history_control_supported'])
        for name in ('invalid-mode', 'invalid-mode-word-5', 'invalid-mode-word-7',
                     'invalid-mode-word-8', 'invalid-mode-word-4294967296',
                     'invalid-mode-word-9223372036854775814'):
            item = by_name[name]['result']
            check('illegal full word refused ' + name,
                  item['success'] is False and item['errno'] == 71 and
                  item['stage'] == 'mode' and item['raw']['193'] == item['mode_word'])
        bad_capture = by_name['invalid-capture-status-readable']['result']
        check('capture invalid flags remain readable and do not qualify data',
              bad_capture['success'] and bad_capture['raw']['199'] == 255 and
              bad_capture['raw']['194'] == 2**64 - 1)
        check('unknown operation does no MMIO',
              by_name['invalid-operation-no-io']['result']['index_selector_write_attempts'] == 0)
        first_fault = by_name['access-error-at-4']['result']
        check('failed first DATA is null, not fabricated zero',
              first_fault['raw']['192'] is None and first_fault['cap'] is None)
        late_fault = by_name['access-error-at-28']['result']
        check('late error retains all acquired bank/gap words',
              all(v is not None for v in late_fault['raw'].values()) and
              late_fault['gap_cap'] == 0x49444d4147415001 and
              late_fault['observer_after'] is None)
        sanitized = out / 'check-pn-control-sanitized'
        run(['cc', '-O1', '-g', '-Wall', '-Wextra', '-Werror',
             '-fsanitize=address,undefined', '-fno-omit-frame-pointer',
             '-fno-pie', '-no-pie', HERE / 'check_pn_control.c', '-o', sanitized],
            'host-sanitized-compile.json')
        sanitized_text = run([sanitized], 'host-sanitized-execution.json')
        check('ASan+UBSan same actual-function results', sanitized_text == text)

        # Exercise accepted S15 readback against actual C JSON; fixtures never
        # have their tags stripped, even for the private positive protocol test.
        spec = importlib.util.spec_from_file_location('pn_s15_gate', HERE / 's15_gate.py')
        gate = importlib.util.module_from_spec(spec)
        spec.loader.exec_module(gate)
        with tempfile.TemporaryDirectory(prefix='pn-readback-fixture-') as tmp:
            root = Path(tmp)
            saved = root / 'command-read.json'
            boot = '00000000-0000-4000-8000-000000000015'

            def raw(result):
                return {'command': gate.READ_COMMAND, 'exit_code': 0,
                        'stdout': f'KERNEL={gate.KERNEL}\nBOOT={boot}\nPRE_SEQ=15\n' +
                        json.dumps(result, separators=(',', ':')) + '\n'}

            saved.write_text(json.dumps(raw(valid)))
            item = gate.readback(gate.Inputs(root, root, True), saved, boot,
                                 [0x49444d41504e0001, 0x49444d4147415001])
            check('actual C resetS/gap0 JSON private S15 protocol',
                  item['record'] == valid and item['observer_watermark'] == 15)
            try:
                gate.readback(gate.Inputs(root, root, False), saved, boot,
                              [0x49444d41504e0001, 0x49444d4147415001])
            except ValueError as error:
                check('public S15 recursively rejects fake control JSON',
                      'synthetic' in str(error))
            else:
                raise AssertionError('fixture qualified for public S15')
            for name in ('capabilities-full-snapshot', 'valid-mode-1', 'valid-mode-6',
                         'shared-without-history-support', 'valid-gap-1000',
                         'pn-cap-mismatch-no-fallback', 'mmio-fault-reported-eio'):
                saved.write_text(json.dumps(raw(by_name[name]['result'])))
                try:
                    gate.readback(gate.Inputs(root, root, True), saved, boot,
                                  [0x49444d41504e0001, 0x49444d4147415001])
                except ValueError:
                    checks.append('S15 refuses unsuitable control snapshot ' + name)
                else:
                    raise AssertionError(name + ' wrongly qualified for resetS')
        check('no UART import', 'serial' not in sys.modules)

        binary = out / 'pn-control'
        # -I is unnecessary: this helper uses no Linux/UAPI header. The compiler
        # executable and -O2/-static/-Wall/-Wextra profile are the original ones.
        run([cc, *original[0][1:5], HERE / 'pn_control.c', '-o', binary],
            'rv64-static-compile.json')
        elf = run(['riscv64-linux-gnu-readelf', '-h', '-l', '-A', binary],
                  'rv64-readelf.json')
        check('RV64 static LP64D ELF, no interpreter', 'ELF64' in elf and
              'RISC-V' in elf and 'double-float ABI' in elf and 'INTERP' not in elf)
        assembly = {}
        for function in ('pn_mmio_load64', 'pn_mmio_select_index'):
            assembly[function] = run(['riscv64-linux-gnu-objdump', '-d',
                                     '--disassemble=' + function, binary], function + '.json')
            check(function + ' explicit full I/O fences',
                  assembly[function].count('0ff0000f') == 2)
        check('single MMIO store uses aligned 64bit sd at INDEX112',
              len(re.findall(r'\bsd\s+[^\n]+,112\(', assembly['pn_mmio_select_index'])) == 1)
        check('MMIO read uses 64bit ld',
              re.search(r'\bld\s+a4,0\(a4\)', assembly['pn_mmio_load64']) is not None)
        receipt['binary'] = {'path': str(binary), 'sha256': sha(binary),
                             'bytes': binary.stat().st_size, 'board_executed': False}
        check('source bytes unchanged during validation',
              all(sha(HERE / name if name != 'tool-source-map.json' else source_map) == digest
                  for name, digest in receipt['source_sha256'].items()))
        receipt['finite_C_scenarios'] = [x['test_case'] for x in rows]
        receipt['status'] = 'passed'
        receipt['limits'] = [
            'fake MMIO actual snapshot/JSON; production /dev/mem/sigaction/mmap not run',
            'RV64 binary compiled and disassembled only; no emulator/board execution',
            'OBS busy detection is not an atomic lock; external sole-owner serialization required',
            'only194 bit63 is decoded; all other194/199 bits and capture validity stay raw']
        print(json.dumps({'status': receipt['status'], 'checks': len(checks),
                          'finite_C_scenarios': len(rows), 'source_board_cases': 0,
                          'binary_sha256': receipt['binary']['sha256']}))
    except Exception as error:
        receipt['status'] = 'failed'
        receipt['error'] = str(error)
        raise
    finally:
        receipt['check_count'] = len(checks)
        (out / 'validation.json').write_text(json.dumps(receipt, indent=2) + '\n')


if __name__ == '__main__':
    main()
