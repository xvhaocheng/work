#!/usr/bin/env python3
"""Complete SPI recovery modules without rebuilding the frozen P0 Image/payload.

The initial failed build manifest/log stay unchanged. A new repair receipt and
delivery manifest bind the exact original binaries and the normal Kbuild fix.
"""
import argparse
import copy
import datetime
import hashlib
import json
from pathlib import Path
import re
import subprocess


def sha(path):
    return hashlib.sha256(Path(path).read_bytes()).hexdigest()


def now():
    return datetime.datetime.now(datetime.timezone.utc).isoformat()


def save(path, value):
    path.write_text(json.dumps(value, indent=2)+'\n')


def main():
    p = argparse.ArgumentParser(description=__doc__)
    p.add_argument('--out', required=True, type=Path)
    p.add_argument('--jobs', type=int, default=16)
    a = p.parse_args()
    assert a.jobs > 0
    out = a.out.resolve()
    original = out/'build-manifest.json'
    initial = json.loads(original.read_text())
    assert initial['status'] == 'failed'
    assert initial['selected_linux'] == 'acece3a3ab7d72ba1f936c1984b82a4a55f28d46'
    assert initial['selected_platform'] == '52c5fdd028bbf3b07deaf1d0f7e13e3a271b29d0'
    linux, kernel, modules = out/'src/linux', out/'linux-build', out/'spi-recovery'
    release = (kernel/'include/config/kernel.release').read_text().strip()
    assert release == '6.5.0-rc2-ispass2027-p0-multi-v1'
    assert not (kernel/'scripts/module.lds').exists(), 'repair is for the observed missing module linker script'
    for name in ('spi-bitbang.c', 'spi-xilinx.c'):
        assert (modules/name).read_bytes() == (linux/'drivers/spi'/name).read_bytes()
    frozen = [original, out/'build.log', out/'fw_payload.bin', kernel/'arch/riscv/boot/Image',
              out/'platform.dtb', out/'rootfs.cpio', kernel/'.config', kernel/'include/generated/utsrelease.h',
              *list((out/'bin').iterdir()), *[linux/name for name in initial['kernel_critical_sources']]]
    before = {str(x.relative_to(out)): sha(x) for x in frozen}
    assert (out/'fw_payload.bin').stat().st_size <= 33554432
    repair = out/'recovery-repair01'
    repair.mkdir(exist_ok=False)
    receipt_path = repair/'manifest.json'
    receipt = {'schema': 'ispass2027-p0-software-repair-v1', 'status': 'running', 'start': now(),
               'script_sha256': sha(__file__), 'initial_failed_manifest': str(original),
               'initial_failed_manifest_sha256': before['build-manifest.json'],
               'initial_failure': initial['error'], 'kernel_release': release,
               'diagnosis': 'Image target did not create scripts/module.lds; Makefile.modfinal:60 requires it. '
                            'Run the existing board recovery modules_prepare step before external modules.',
               'kernel_source_changed': False, 'Image_or_payload_rebuilt': False, 'board_operations': False,
               'frozen_before': before, 'commands': []}
    save(receipt_path, receipt)

    def run(argv, log):
        command = {'argv': list(map(str, argv)), 'start': now()}
        receipt['commands'].append(command); save(receipt_path, receipt)
        log.write(('\n+ '+' '.join(command['argv'])+'\n').encode()); log.flush()
        result = subprocess.run(command['argv'], stdout=log, stderr=log)
        command.update(end=now(), exit_code=result.returncode); save(receipt_path, receipt)
        result.check_returncode()

    try:
        with (repair/'build.log').open('xb') as log:
            common = ['make', '-C', linux, 'ARCH=riscv', 'O='+str(kernel),
                      'CROSS_COMPILE=riscv64-linux-gnu-', 'KERNELRELEASE='+release, '-j'+str(a.jobs)]
            run(common+['modules_prepare'], log)
            assert (kernel/'scripts/module.lds').is_file()
            assert (kernel/'Module.symvers').read_bytes() == (kernel/'vmlinux.symvers').read_bytes()
            run(common+['M='+str(modules), 'modules'], log)
        receipt['frozen_after'] = {str(x.relative_to(out)): sha(x) for x in frozen}
        assert receipt['frozen_after'] == before, 'frozen binary/config/source changed during module-only repair'
        receipt['module_info'] = {}
        for name in ('spi-bitbang.ko', 'spi-xilinx.ko'):
            text = subprocess.check_output(['riscv64-linux-gnu-readelf', '-p', '.modinfo', modules/name], text=True)
            (repair/(name+'.modinfo.txt')).write_text(text)
            magic = re.search(r'vermagic=([^\n]+)', text)
            assert magic and magic[1].startswith(release+' '), 'module vermagic differs from frozen kernel'
            receipt['module_info'][name] = {'vermagic': magic[1], 'sha256': sha(modules/name),
                                           'bytes': (modules/name).stat().st_size}
        receipt.update(status='passed', end=now())
        save(receipt_path, receipt)
        delivery = copy.deepcopy(initial)
        delivery.pop('error', None)
        delivery.update(status='passed', kernel_release=release, end=now(),
                        completion_method='original kernel/payload build plus normal module-only continuation',
                        initial_failed_build={'path': str(original), 'sha256': before['build-manifest.json'],
                                              'status': 'failed', 'error': initial['error']},
                        recovery_repair={'path': str(receipt_path), 'sha256': sha(receipt_path)},
                        payload_bytes=(out/'fw_payload.bin').stat().st_size,
                        payload_headroom_bytes=33554432-(out/'fw_payload.bin').stat().st_size,
                        recovery={'kernel_release': release, 'load_order': ['spi-bitbang.ko', 'spi-xilinx.ko'],
                                  'policy': 'Only for software update; not loaded during formal measurements'},
                        update_from_pathv3_modules='existing baseline100 package spi-recovery-pathv3; original boot only')
        artifacts = [out/'fw_payload.bin', kernel/'arch/riscv/boot/Image', out/'platform.dtb', out/'rootfs.cpio',
                     *list((out/'bin').iterdir()), modules/'spi-bitbang.ko', modules/'spi-xilinx.ko']
        delivery['artifacts'] = {str(x.relative_to(out)): sha(x) for x in artifacts}
        destination = out/'delivery-manifest.json'
        with destination.open('x') as stream:
            json.dump(delivery, stream, indent=2); stream.write('\n')
    except BaseException as error:
        receipt.update(status='failed', error=repr(error), end=now()); save(receipt_path, receipt)
        raise
    print(json.dumps({'status': 'passed', 'delivery_manifest': str(destination),
                      'repair_receipt': str(receipt_path), 'payload_bytes': delivery['payload_bytes']}))


if __name__ == '__main__':
    main()
