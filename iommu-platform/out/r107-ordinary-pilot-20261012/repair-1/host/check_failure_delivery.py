#!/usr/bin/env python3
"""Exercise CLI serializer with host link wrappers; real ioctl is unreachable."""
import argparse
import importlib.util
import json
import subprocess
import sys
import time
from pathlib import Path

sys.dont_write_bytecode = True
ROOT = Path(__file__).resolve().parents[1]
spec = importlib.util.spec_from_file_location('finite_v', ROOT/'host/validate_finite_pilot.py')
v = importlib.util.module_from_spec(spec);spec.loader.exec_module(v)


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--ledger-file', type=Path, default=ROOT/'validation/host-check-ledger.json')
    args = parser.parse_args()
    stamp = str(time.time_ns())
    out = ROOT/'validation'/('cli-failed-ioctl-host-mock-'+stamp)
    commands = [
        ['gcc', '-std=c11', '-O2', '-Wall', '-Wextra', '-Werror', '-D__riscv=1',
         '-D__riscv_xlen=64', 'host/idma_finite_cli.c', 'host/mock_failed_ioctl.c',
         '-Wl,--wrap=open', '-Wl,--wrap=ioctl', '-o', 'host/idma_finite_cli.host_mock'],
        ['host/idma_finite_cli.host_mock', '--run', '--device', '/synthetic/idma0',
         '--data-dir', 'data', '--output-dir', str(out.relative_to(ROOT))],
    ]
    records = []
    for i, command in enumerate(commands):
        begin = time.monotonic_ns();r=subprocess.run(command, cwd=ROOT, text=True, capture_output=True)
        record = dict(command=command, returncode=r.returncode, wall_ns=time.monotonic_ns()-begin,
                      stdout=r.stdout, stderr=r.stderr)
        records.append(record)
        assert r.returncode == (0 if i == 0 else 1), record
    receipt = json.loads((out/'receipt.json').read_text())
    assert receipt['ioctl']['return'] == -1 and receipt['ioctl']['errno'] == 5
    assert receipt['timing_semantics']['physical_rate_uncalibrated'] is True
    assert receipt['timing_semantics']['independent_dell_reference'] is False
    assert 'userspace_cost_nominal_ns' in receipt and 'host_cost_ns' not in receipt
    assert receipt['run']['status'] == -5 and receipt['run']['release_status'] == -115
    assert receipt['run']['quarantined'] == 1 and receipt['run']['stage'] == 3
    assert len(receipt['run']['stages']) == 12 and len(receipt['run']['layers']) == 24
    assert receipt['run']['batches'][0]['first_id'] == 37
    assert receipt['run']['batches'][0]['observed_done'] == 36
    assert receipt['run']['batches'][0]['issued'] == 9 and receipt['run']['batches'][0]['completed'] == 8
    assert receipt['run']['hpm_before'][0][0] == 123 and receipt['run']['hpm_after'][0][0] == 456
    assert len(receipt['run']['observer'][0]) == 35 and len(receipt['run']['composition'][0]) == 128
    assert receipt['run']['observer_status'][0] == 4
    assert all((out/p['file']).stat().st_size == v.POOL for p in receipt['pools'])
    assert (out/receipt['pools'][0]['file']).read_bytes()[0] == 0x5a
    assert all(p['memcmp_equal'] is False for p in receipt['pools'])
    v.original_buffers(receipt, out)
    try:
        v.validate(receipt, out, ROOT/'data')
        raise AssertionError('failed ioctl receipt accepted')
    except v.Rejection as exc:
        rejection = str(exc)
    summary = dict(scope='host linker mock of failure delivery; no device open or real ioctl',
                   hardware_test=False, real_ioctl_calls=0, commands=records,
                   output_dir=str(out), errno_preserved=5, partial_outputs_preserved=True,
                   original_buffers_match_json=True, failure_acceptance=False,
                   board_userspace_clock_is_uncalibrated_nominal=True,
                   independent_dell_reference_claim=False,
                   validator_rejection=rejection)
    (ROOT/'validation/cli-failure-delivery-host-mock.json').write_text(json.dumps(summary, indent=2)+'\n')
    ledger = json.loads(args.ledger_file.read_text())
    ledger['additional_failure_delivery_checks'] = summary
    ledger['files']['host/mock_failed_ioctl.c'] = v.digest(ROOT/'host/mock_failed_ioctl.c')
    ledger['files']['host/check_failure_delivery.py'] = v.digest(ROOT/'host/check_failure_delivery.py')
    args.ledger_file.write_text(json.dumps(ledger, indent=2)+'\n')
    print(json.dumps({k: val for k, val in summary.items() if k != 'commands'}, indent=2))


if __name__ == '__main__':
    main()
