#!/usr/bin/env python3
"""Build the two existing upstream SPI modules for a recorded running kernel.

No board operation, driver-source change, forced loading, or kernel rebuild.
"""
import argparse
import datetime
import json
from pathlib import Path
import shutil
import subprocess

WORK = Path(__file__).resolve().parents[4]
BOARD = Path(__file__).resolve().parent.parent
OUT = WORK / 'iommu-platform/out/ispass2027-spi-recovery-20261017'
LINUX = WORK / 'iommu-platform/linux'
SOFTWARE = WORK / 'iommu-platform/out/ispass2027-20261017-software01'
PINS = {
    'g1': ('b7979a1a06acd8a966ee33558ab98dd93f31c98d',
           '6.5.0-rc2-00082-gb7979a1a06ac', 'g1-20261017', 'spi-recovery-00082'),
    'pathv3': ('45bf76b15e36cf608dff95013d91f8bedb7b1b8e',
               '6.5.0-rc2-ispass2027-pathv3', 'baseline100-20261017', 'spi-recovery-pathv3'),
}


def now():
    return datetime.datetime.now(datetime.timezone.utc).isoformat()


def main():
    p = argparse.ArgumentParser(description=__doc__)
    p.add_argument('--group', required=True, choices=PINS)
    p.add_argument('--jobs', type=int, default=16)
    a = p.parse_args()
    if a.jobs <= 0:
        p.error('--jobs must be positive')
    commit, release, package, module_directory = PINS[a.group]
    out = OUT / a.group
    out.mkdir(parents=True, exist_ok=False)
    manifest_path = out / 'manifest.json'
    manifest = {'group': a.group, 'status': 'running', 'started_utc': now(),
                'linux_commit': commit, 'kernel_release': release,
                'driver_files': ['drivers/spi/spi-bitbang.c', 'drivers/spi/spi-xilinx.c'],
                'driver_source_modified': False, 'runtime_config_modified': False,
                'kernel_release_method': 'KERNELRELEASE explicitly supplied from the recorded actual release; source commit is pinned and archive has no Git metadata',
                'core_symbol_inventory': str(SOFTWARE / 'linux-build/vmlinux.symvers'),
                'core_symbol_inventory_note': 'Existing completed 45bf kernel build; zero CRC with CONFIG_MODVERSIONS=n. b797..45bf changes only drivers/misc/idma.c, without exported symbols; SPI/core/headers are identical. Used for normal modpost, without suppressing errors.',
                'board_operations': False, 'commands': []}

    def save():
        manifest_path.write_text(json.dumps(manifest, indent=2) + '\n')

    def run(argv, log, stdout=None):
        record = {'argv': list(map(str, argv)), 'started_utc': now()}
        manifest['commands'].append(record)
        save()
        result = subprocess.run(list(map(str, argv)), stdout=stdout or log,
                                stderr=log, cwd=out)
        record.update(finished_utc=now(), returncode=result.returncode)
        save()
        result.check_returncode()

    save()
    try:
        with (out / 'build.log').open('xb') as log:
            if a.group == 'g1':
                source = out / 'src/linux'
                source.mkdir(parents=True)
                command = ['git', '-C', str(LINUX), 'archive', '--format=tar', commit]
                record = {'argv': command, 'started_utc': now()}
                manifest['commands'].append(record)
                save()
                archive = subprocess.Popen(command, stdout=subprocess.PIPE, stderr=log)
                unpack = subprocess.run(['tar', '-xf', '-', '-C', str(source)],
                                        stdin=archive.stdout, stdout=log, stderr=log)
                archive.stdout.close()
                archive_status = archive.wait()
                record.update(finished_utc=now(), returncode=archive_status,
                              unpack_returncode=unpack.returncode)
                save()
                unpack.check_returncode()
                if archive_status:
                    raise subprocess.CalledProcessError(archive_status, command)
                payload = WORK / 'iommu-platform/out/e4-batch-20260916/delivery/fw_payload.bin'
                with (out / 'runtime.config').open('xb') as config:
                    run(['sh', source / 'scripts/extract-ikconfig', payload], log, stdout=config)
                manifest['runtime_config_source'] = str(payload) + ' embedded IKCONFIG'
            else:
                source = SOFTWARE / 'src/linux'
                runtime_config = SOFTWARE / 'linux-build/.config'
                shutil.copyfile(runtime_config, out / 'runtime.config')
                manifest['runtime_config_source'] = str(runtime_config)
            manifest['source_directory'] = str(source)
            build = out / 'linux-build'
            build.mkdir()
            shutil.copyfile(out / 'runtime.config', build / '.config')
            modules = out / 'modules'
            modules.mkdir()
            for name in ['spi-bitbang.c', 'spi-xilinx.c']:
                (modules / name).write_bytes(subprocess.check_output(
                    ['git', '-C', str(LINUX), 'show', commit + ':drivers/spi/' + name]))
            (modules / 'Makefile').write_text('obj-m += spi-bitbang.o\nobj-m += spi-xilinx.o\n')
            manifest['module_method'] = 'External obj-m builds the original files; the runtime .config is retained without enabling new built-in options.'
            common = ['make', '-C', source, 'O=' + str(build), 'ARCH=riscv',
                      'CROSS_COMPILE=riscv64-linux-gnu-', 'KERNELRELEASE=' + release]
            run(common + ['-j' + str(a.jobs), 'modules_prepare'], log)
            shutil.copyfile(SOFTWARE / 'linux-build/vmlinux.symvers', build / 'Module.symvers')
            run(common + ['-j' + str(a.jobs), 'M=' + str(modules), 'modules'], log)
            destination = BOARD / 'packages' / package / 'artifacts' / module_directory
            destination.mkdir(parents=True, exist_ok=False)
            artifacts = []
            for name in ['spi-bitbang.ko', 'spi-xilinx.ko']:
                shutil.copyfile(modules / name, destination / name)
                artifacts.append({'path': str(modules / name), 'package_copy': str(destination / name),
                                  'bytes': (modules / name).stat().st_size})
            manifest.update(status='built', artifacts=artifacts,
                            load_order=['spi-bitbang.ko', 'spi-xilinx.ko'],
                            load_policy='Root owns insmod and SD writes. Do not load pathv3 recovery modules during the formal matrix; external modules taint the kernel O.')
    except BaseException as error:
        manifest.update(status='failed', error=type(error).__name__ + ': ' + str(error))
        raise
    finally:
        manifest['finished_utc'] = now()
        save()
        shutil.copyfile(manifest_path, BOARD / 'recovery' / (a.group + '-modules.json'))
    print(json.dumps({'status': manifest['status'], 'manifest': str(manifest_path),
                      'artifacts': manifest.get('artifacts')}, indent=2))


if __name__ == '__main__':
    main()
