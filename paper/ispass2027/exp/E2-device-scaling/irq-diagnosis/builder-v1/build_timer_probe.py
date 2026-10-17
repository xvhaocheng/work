#!/usr/bin/env python3
"""Prepare unchanged full-E3 inputs, then build one opt-in E2 probe payload.

Preparation does not build a temporary kernel. Finalization uses explicit
commits and a fresh source archive, preserves original CLI/timeout/updater,
and creates matching SPI recovery modules. No SD/UART/JTAG operation exists.
"""
import argparse
import importlib.util
import json
from pathlib import Path
import shutil

HERE = Path(__file__).resolve().parent
PLATFORM = Path('/home/xvhaocheng/work/iommu-platform')
BASE_SW = Path('/home/xvhaocheng/work-a2-workloads/paper/ispass2027/exp/E3-workloads/artifacts/full-e3-software01')
BASE_PLATFORM = 'ec99648e3c9a1e48c1e9721e3662bb632eb439bd'
BASE_LINUX = '65b427946152e3aca27babab0a437821729805d4'
M = '1facb309758e3d41622355de71dbc033cd0fac97'
PLATFORM_COMMIT = '5baab2217eb4877da29d5c38f12749a80c5cdf4f'
CVA6 = '1d587125eee690e623e6ffae13e70f9bbe989adc'
SBI = 'e8d21bfdc2dd753314627968e496c3a1188f6dfb'
REFERENCE_MANIFEST_SHA = '7415dcc4d076fa434bc22294f4309582af84086964f1b47bb05c695555f0bcc4'
HELPER_SHA = '5a7f410db31e7efa6c3cf1898b62d2cb26864d863f06b594d7f526726a5de19f'
RELEASE_SUFFIX = '-ispass2027-e2-timer-probe-v1'
RELEASE = '6.5.0-rc2' + RELEASE_SUFFIX
MAX_PAYLOAD = 33554432
BINS = ['idma_experiment', 'idma_p0', 'idma_pages', 'idma_scaling',
        'idma_workload', 'platform-sd-update', 'platform-timeout']


def require(condition, message):
    if not condition:
        raise ValueError(message)


def library():
    spec = importlib.util.spec_from_file_location('p0_software_helpers', HERE/'source/build_baseline.as-reused.py')
    module = importlib.util.module_from_spec(spec); spec.loader.exec_module(module)
    module.WORK = PLATFORM.parent; module.PLATFORM = PLATFORM
    return module


def validate_tuple(base, platform_commit, linux_commit):
    require(platform_commit == PLATFORM_COMMIT, 'frozen E2 probe platform only')
    require(linux_commit == M, 'frozen E2 probe Linux only')
    changed = base.git(PLATFORM, 'diff', '--name-only', BASE_PLATFORM, platform_commit).decode().splitlines()
    require(set(changed) == {'linux', 'sources.lock.json'}, 'platform scope changed')
    old_lock = json.loads(base.show(PLATFORM, BASE_PLATFORM, 'sources.lock.json'))
    lock = json.loads(base.show(PLATFORM, platform_commit, 'sources.lock.json'))
    components = {entry['path']: entry['commit'] for entry in lock['components']}
    require(len(components) == len(lock['components']), 'duplicate source lock path')
    for entry in old_lock['components']:
        if entry['path'] == 'linux':
            require(entry['commit'] == BASE_LINUX, 'unexpected base Linux')
            entry['commit'] = M
    require(lock == old_lock, 'source lock changed beyond Linux')
    for name, commit in [('linux', M), ('cva6', CVA6), ('opensbi', SBI)]:
        require(components[name] == commit, 'source lock tuple mismatch: ' + name)
        actual = base.git(PLATFORM, 'rev-parse', platform_commit + ':' + name).decode().strip()
        require(actual == commit, 'gitlink tuple mismatch: ' + name)
    changed = base.git(PLATFORM/'linux', 'diff', '--name-only', BASE_LINUX, M).decode().splitlines()
    require(changed == ['drivers/clocksource/timer-riscv.c'], 'Linux changed beyond timer probe')
    # The existing archive/parser helpers must be the exact frozen platform code.
    require(base.sha(HERE/'source/build_baseline.as-reused.py') == HELPER_SHA, 'helper changed')
    require(base.SBI == SBI, 'helper OpenSBI changed')
    for name in ['scripts/platform.py', 'scripts/rootfs.py']:
        require((PLATFORM/name).read_bytes() == base.show(PLATFORM, platform_commit, name),
                'working archive/parser helper changed: ' + name)


def reference_manifest(base):
    path = BASE_SW/'build-manifest.json'
    require(base.sha(path) == REFERENCE_MANIFEST_SHA, 'passed reference manifest changed')
    reference = json.loads(path.read_text())
    for key, value in [('status', 'passed'), ('selected_linux', BASE_LINUX),
                       ('selected_platform', BASE_PLATFORM), ('selected_cva6', CVA6),
                       ('selected_opensbi', SBI)]:
        require(reference.get(key) == value, 'reference identity mismatch: ' + key)
    require(reference['rootfs_binaries'] == BINS, 'reference CLI list changed')
    return reference


def validate_inputs(base, root, reference):
    require(base.sha(root/'rootfs.cpio') == reference['rootfs_sha256'], 'rootfs bytes changed')
    require((root/'rootfs.cpio').stat().st_size == reference['rootfs_bytes'], 'rootfs size changed')
    require(base.tree_hashes(root/'inputs') == reference['handwritten_inputs'], 'CLI/config/DTS inputs changed')
    require(base.tree_hashes(root/'uapi') == base.tree_hashes(BASE_SW/'uapi'), 'preserved UAPI changed')
    require(sorted(path.name for path in (root/'bin').iterdir()) == BINS, 'CLI binary set changed')
    parser = base.load_module(PLATFORM/'scripts/rootfs.py', 'e2_probe_preserved_rootfs')
    entries = list(parser.read_newc((root/'rootfs.cpio').read_bytes()))
    paths = [str(Path(name)) for name, _, _ in entries]
    require(len(paths) == len(set(paths)), 'duplicate rootfs path')
    packed = {str(Path(name)): (fields, body) for name, fields, body in entries}
    for name in BINS:
        body = (root/'bin'/name).read_bytes()
        require(base.sha(root/'bin'/name) == reference['artifacts']['bin/'+name], 'CLI bytes changed: ' + name)
        fields, archived = packed['usr/bin/'+name]
        require(archived == body and fields[1] == 0o100755, 'rootfs CLI body/mode changed: ' + name)
        require(sum(Path(path).name == name for path in paths) == 1, 'duplicate rootfs CLI: ' + name)
    for name in reference['handwritten_inputs']:
        # Config/DTS were inherited baseline inputs in the passed full-E3 build;
        # their manifest bytes, not the platform's different templates, apply.
        if name.startswith('bench/'):
            require((root/'inputs'/name).read_bytes() == base.show(PLATFORM, PLATFORM_COMMIT, name),
                    'preserved CLI source differs from frozen platform: ' + name)
    for name in base.tree_hashes(root/'uapi'):
        require((root/'uapi'/name).read_bytes() == base.show(PLATFORM/'linux', M, 'include/uapi/'+name),
                'preserved UAPI differs from probe Linux: ' + name)
    config = root/'linux-build/.config' if root == BASE_SW else root/'reference.config'
    require(base.sha(config) == reference['kernel_config_sha256'], 'reference kernel config changed')


def validate_prepared(base, out, manifest):
    require(manifest['status'] == 'prepared', 'finalize only a prepared package; no retry/overwrite')
    require(manifest['script_sha256'] == base.sha(__file__), 'builder changed after preparation')
    require(manifest['selected_platform'] == PLATFORM_COMMIT and manifest['selected_linux'] == M,
            'prepared tuple changed')
    validate_inputs(base, out, reference_manifest(base))


def validate_kernel_config(config, reference):
    def options(path):
        return {line.split('=', 1)[0]: line for line in path.read_text().splitlines()
                if line.startswith('CONFIG_') or line.startswith('# CONFIG_')}
    actual, original = options(config), options(reference)
    for name in ['CONFIG_INITRAMFS_SOURCE', 'CONFIG_LOCALVERSION']:
        actual.pop(name, None)
        original.pop(name, None)
    require(actual == original, 'generated kernel config changed beyond rootfs path/release')


def validate_payload_size(size):
    require(0 < size <= MAX_PAYLOAD, '32MiB SD payload budget exceeded; stop without changing configuration')


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('action', choices=['prepare', 'finalize'])
    parser.add_argument('--out', required=True, type=Path)
    parser.add_argument('--platform-commit', required=True)
    parser.add_argument('--linux-commit', required=True)
    # CLI sources are fixed by the selected platform commit, not mutable external paths.
    parser.add_argument('--jobs', type=int, default=16)
    a = parser.parse_args()
    require(a.jobs > 0, 'jobs must be positive')
    base, out = library(), a.out.resolve()
    output_root = (HERE/'artifacts').resolve()
    require(out != output_root and out.is_relative_to(output_root), 'out must stay inside this E2 task artifacts/')
    a.platform_commit = base.git(PLATFORM, 'rev-parse', a.platform_commit+'^{commit}').decode().strip()
    if a.linux_commit:
        a.linux_commit = base.git(PLATFORM/'linux', 'rev-parse', a.linux_commit+'^{commit}').decode().strip()
    validate_tuple(base, a.platform_commit, a.linux_commit)
    reference = reference_manifest(base)
    validate_inputs(base, BASE_SW, reference)
    path = out/'build-manifest.json'
    if a.action == 'prepare':
        out.mkdir(parents=True, exist_ok=False)
        manifest = {'schema': 'ispass2027-e2-timer-probe-software-v1', 'status': 'preparing', 'start': base.now(),
                    'script_sha256': base.sha(__file__), 'commands': [],
                    'baseline_software_manifest': str(BASE_SW/'build-manifest.json'),
                    'baseline_rootfs': str(BASE_SW/'rootfs.cpio'),
                    'prepared_platform_cli_commit': BASE_PLATFORM,
                    'selected_platform': a.platform_commit, 'selected_linux': a.linux_commit,
                    'preserved_software_manifest_sha256': REFERENCE_MANIFEST_SHA,
                    'preserved_kernel_release': reference['kernel_release'],
                    'new_kernel_release_suffix': RELEASE_SUFFIX, 'board_operations': False}
        base.save(path, manifest)
        for name in ['inputs', 'uapi', 'bin']:
            shutil.copytree(BASE_SW/name, out/name)
        shutil.copyfile(BASE_SW/'rootfs.cpio', out/'rootfs.cpio')
        shutil.copyfile(BASE_SW/'linux-build/.config', out/'reference.config')
        (out/'build.log').touch(exist_ok=False)
        validate_inputs(base, out, reference)
        manifest.update(status='prepared', prepared_utc=base.now(),
                        rootfs_sha256=reference['rootfs_sha256'], rootfs_bytes=reference['rootfs_bytes'],
                        rootfs_binaries=BINS, payload_limit_bytes=MAX_PAYLOAD,
                        baseline_payload_bytes=(BASE_SW/'fw_payload.bin').stat().st_size,
                        baseline_payload_headroom_bytes=MAX_PAYLOAD-(BASE_SW/'fw_payload.bin').stat().st_size,
                        prepared_added_ELF_bytes=0,
                        note='Final payload size is checked after Image/lloader/OpenSBI; no temporary kernel or SD write.')
        base.save(path, manifest)
        print(json.dumps({'status': 'prepared', 'manifest': str(path), 'rootfs_bytes': manifest['rootfs_bytes']}))
        return
    manifest = json.loads(path.read_text())
    validate_prepared(base, out, manifest)
    base.git(PLATFORM/'linux', 'merge-base', '--is-ancestor', BASE_LINUX, a.linux_commit)
    manifest.update(status='building', final_build_started=base.now(), selected_linux=a.linux_commit,
                    selected_platform=a.platform_commit, selected_cva6=CVA6, selected_opensbi=base.SBI,
                    selected_cva6_reference_only=True,
                    selected_cva6_role='source_reference_not_loaded_bit',
                    finalized_builder_sha256=base.sha(__file__))
    base.save(path, manifest)
    try:
        with (out/'build.log').open('ab') as log:
            linux, sbi = out/'src/linux', out/'src/opensbi'
            base.archive(PLATFORM/'linux', a.linux_commit, linux)
            base.archive(PLATFORM/'opensbi', base.SBI, sbi)
            for name in base.tree_hashes(out/'uapi'):
                require((linux/'include/uapi'/name).read_bytes() == (out/'uapi'/name).read_bytes(),
                        'exported UAPI changed after preparation: ' + name)
            kernel = out/'linux-build'; kernel.mkdir()
            config = kernel/'.config'; shutil.copyfile(out/'inputs/configs/linux_defconfig', config)
            base.run([linux/'scripts/config', '--file', config, '--set-str', 'INITRAMFS_SOURCE', out/'rootfs.cpio',
                      '--set-str', 'LOCALVERSION', RELEASE_SUFFIX, '--disable', 'LOCALVERSION_AUTO'], out, log, manifest, path)
            common = ['make', '-C', linux, 'ARCH=riscv', 'O='+str(kernel), 'CROSS_COMPILE=riscv64-linux-gnu-']
            base.run(common+['olddefconfig'], out, log, manifest, path)
            validate_kernel_config(config, out/'reference.config')
            critical = ['drivers/misc/idma.c', 'include/uapi/linux/idma.h', 'include/uapi/linux/idma_p0.h',
                        'drivers/iommu/riscv/iommu-hpm.c', 'include/linux/riscv_iommu_hpm.h',
                        'drivers/iommu/riscv/io_pgtable.c', 'drivers/iommu/riscv/iommu.c']
            critical += [str(h.relative_to(linux)) for h in (linux/'include/uapi/linux').glob('idma_multi*.h')]
            critical += ['include/uapi/linux/idma_pages.h', 'include/linux/riscv_iommu_leaf.h',
                         'include/uapi/linux/idma_workload.h', 'drivers/misc/idma-workload.h',
                         'drivers/clocksource/timer-riscv.c']
            manifest.update(kernel_config_sha256=base.sha(config),
                            handwritten_inputs=base.tree_hashes(out/'inputs'),
                            kernel_critical_sources={name: base.sha(linux/name) for name in critical})
            base.save(path, manifest)
            base.run(common+['-j'+str(a.jobs), 'Image'], out, log, manifest, path)
            image = kernel/'arch/riscv/boot/Image'; dtb = out/'platform.dtb'
            base.run(['dtc', '-I', 'dts', '-O', 'dtb', '-o', dtb, out/'inputs/configs/platform.dts'], out, log, manifest, path)
            loader = linux/'tools/lloader'
            base.run(['make', 'CROSS_COMPILE=riscv64-unknown-elf-', 'ARCH=rv64', 'IMAGE='+str(image),
                      'DTB='+str(dtb), 'TARGET=linux-rv64-cva6'], loader, log, manifest, path)
            base.run(['make', '-C', sbi, 'O='+str(out/'opensbi-build'), 'CROSS_COMPILE=riscv64-linux-gnu-',
                      'PLATFORM=fpga/ariane', 'FW_PAYLOAD=y', 'FW_PAYLOAD_PATH='+str(loader/'linux-rv64-cva6.bin'),
                      '-j'+str(a.jobs)], out, log, manifest, path)
            shutil.copyfile(out/'opensbi-build/platform/fpga/ariane/firmware/fw_payload.bin', out/'fw_payload.bin')
            size = (out/'fw_payload.bin').stat().st_size
            validate_payload_size(size)
            release = (kernel/'include/config/kernel.release').read_text().strip()
            require(release == RELEASE, 'unexpected diagnostic kernel release')
            # Match the actual final kernel exports/release, using unmodified SPI sources.
            modules = out/'spi-recovery'; modules.mkdir()
            for name in ['spi-bitbang.c', 'spi-xilinx.c']:
                shutil.copyfile(linux/'drivers/spi'/name, modules/name)
            (modules/'Makefile').write_text('obj-m += spi-bitbang.o\nobj-m += spi-xilinx.o\n')
            base.run(common+['KERNELRELEASE='+release, '-j'+str(a.jobs), 'modules_prepare'], out, log, manifest, path)
            require((kernel/'scripts/module.lds').is_file(), 'modules_prepare did not generate module.lds')
            shutil.copyfile(kernel/'vmlinux.symvers', kernel/'Module.symvers')
            base.run(common+['KERNELRELEASE='+release, '-j'+str(a.jobs), 'M='+str(modules), 'modules'], out, log, manifest, path)
            artifacts = [out/'fw_payload.bin', image, dtb, out/'rootfs.cpio', *list((out/'bin').iterdir()),
                         modules/'spi-bitbang.ko', modules/'spi-xilinx.ko']
            manifest.update(status='passed', kernel_release=release, payload_bytes=size,
                            payload_headroom_bytes=MAX_PAYLOAD-size,
                            artifacts={str(x.relative_to(out)): base.sha(x) for x in artifacts},
                            recovery={'kernel_release': release, 'load_order': ['spi-bitbang.ko', 'spi-xilinx.ko'],
                                      'policy': 'Only for software update; not loaded during formal measurements'},
                            update_from_pathv3_modules='existing baseline100 package spi-recovery-pathv3; original boot only')
    except BaseException as error:
        manifest.update(status='failed', error=repr(error)); raise
    finally:
        manifest['end'] = base.now(); base.save(path, manifest)
    print(json.dumps({'status': manifest['status'], 'kernel_release': release, 'payload_bytes': size, 'manifest': str(path)}))


if __name__ == '__main__':
    main()
