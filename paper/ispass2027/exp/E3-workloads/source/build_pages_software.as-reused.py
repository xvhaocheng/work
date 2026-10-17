#!/usr/bin/env python3
"""Prepare rootfs, then build one frozen common E4/E5 kernel/payload.

Preparation does not build a temporary kernel. Finalization uses explicit
commits and a fresh source archive, preserves original CLI/timeout/updater,
and creates matching SPI recovery modules. No SD/UART/JTAG operation exists.
"""
import argparse
import importlib.util
import json
from pathlib import Path
import re
import shutil
import stat

HERE = Path(__file__).resolve().parent
PLATFORM = Path('/home/xvhaocheng/work/iommu-platform')
BASE_SW = PLATFORM / 'out/ispass2027-20261017-software01'
M = 'f81c82c24a8c6833c14a1d5c6fa315a8b26e1345'
CVA6 = '6c5c666d45979d3a03fb98cff89ba7ad095f6125'
RELEASE_SUFFIX = '-ispass2027-e4-e5-v1'
MAX_PAYLOAD = 33554432


def library():
    spec = importlib.util.spec_from_file_location('p0_software_helpers', HERE/'source/build_baseline.as-reused.py')
    module = importlib.util.module_from_spec(spec); spec.loader.exec_module(module)
    module.WORK = PLATFORM.parent; module.PLATFORM = PLATFORM
    return module


def rootfs(base, out, manifest):
    module = base.load_module(PLATFORM/'scripts/rootfs.py', 'p0_rootfs')
    entries = list(module.read_newc((BASE_SW/'rootfs.cpio').read_bytes()))
    bins = {path.name: path.read_bytes() for path in (out/'bin').iterdir() if path.is_file()}
    packed = bytearray()
    for name, fields, body in entries:
        if name not in {'usr/bin/'+key for key in bins}:
            module.write_entry(packed, name, fields, body)
    ino = max(fields[0] for _, fields, _ in entries)+1
    for name, body in bins.items():
        module.write_entry(packed, 'usr/bin/'+name,
            [ino, stat.S_IFREG|0o755, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0], body)
        ino += 1
    module.write_entry(packed, 'TRAILER!!!', [0]*13, b'')
    packed.extend(b'\0'*((-len(packed)) % 512))
    (out/'rootfs.cpio').write_bytes(packed)
    reread = {name: body for name, _, body in module.read_newc(packed)}
    assert all(reread['usr/bin/'+name] == body for name, body in bins.items())
    manifest.update(rootfs_sha256=base.sha(out/'rootfs.cpio'),
                    rootfs_bytes=len(packed), rootfs_binaries=sorted(bins),
                    payload_limit_bytes=MAX_PAYLOAD)


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('action', choices=['prepare', 'finalize'])
    parser.add_argument('--out', required=True, type=Path)
    parser.add_argument('--platform-commit', required=True)
    parser.add_argument('--linux-commit')
    parser.add_argument('--extra-program', action='append', default=[], metavar='NAME=SOURCE')
    parser.add_argument('--jobs', type=int, default=16)
    a = parser.parse_args()
    assert a.jobs > 0
    base, out = library(), a.out.resolve()
    a.platform_commit = base.git(PLATFORM, 'rev-parse', a.platform_commit+'^{commit}').decode().strip()
    if a.linux_commit:
        a.linux_commit = base.git(PLATFORM/'linux', 'rev-parse', a.linux_commit+'^{commit}').decode().strip()
    path = out/'build-manifest.json'
    if a.action == 'prepare':
        out.mkdir(parents=True, exist_ok=False)
        manifest = {'schema': 'ispass2027-e4-e5-software-v1', 'status': 'preparing', 'start': base.now(),
                    'script_sha256': base.sha(__file__), 'commands': [],
                    'baseline_software_manifest': str(BASE_SW/'build-manifest.json'),
                    'baseline_rootfs': str(BASE_SW/'rootfs.cpio'),
                    'prepared_platform_cli_commit': a.platform_commit,
                    'new_kernel_release_suffix': RELEASE_SUFFIX, 'board_operations': False}
        base.save(path, manifest)
        (out/'inputs/bench').mkdir(parents=True); (out/'inputs/configs').mkdir()
        (out/'uapi/linux').mkdir(parents=True); (out/'bin').mkdir()
        for name in ['linux_defconfig', 'platform.dts']:
            shutil.copyfile(BASE_SW/'inputs/configs'/name, out/'inputs/configs'/name)
        for name in ['idma.h', 'idma_p0.h']:
            (out/'uapi/linux'/name).write_bytes(base.show(PLATFORM/'linux', M, 'include/uapi/linux/'+name))
        cli = out/'inputs/bench/idma_p0.c'
        cli.write_bytes(base.show(PLATFORM, a.platform_commit, 'bench/idma_p0.c'))
        # Preserve the exact three proven binaries from the actual baseline rootfs.
        module = base.load_module(PLATFORM/'scripts/rootfs.py', 'p0_preserved_rootfs')
        original = {n: b for n, _, b in module.read_newc((BASE_SW/'rootfs.cpio').read_bytes())}
        for name in ['idma_experiment', 'platform-timeout', 'platform-sd-update']:
            (out/'bin'/name).write_bytes(original['usr/bin/'+name])
        with (out/'build.log').open('xb') as log:
            base.run(['riscv64-linux-gnu-gcc', '-O2', '-static', '-Wall', '-Wextra', '-Werror',
                      '-I', out/'uapi', cli, '-o', out/'bin/idma_p0'], out, log, manifest, path)
        rootfs(base, out, manifest)
        manifest.update(status='prepared', prepared_utc=base.now(),
                        baseline_payload_bytes=(BASE_SW/'fw_payload.bin').stat().st_size,
                        baseline_payload_headroom_bytes=MAX_PAYLOAD-(BASE_SW/'fw_payload.bin').stat().st_size,
                        prepared_added_ELF_bytes=(out/'bin/idma_p0').stat().st_size,
                        note='Final payload size is checked after Image/lloader/OpenSBI; no temporary kernel or SD write.')
        base.save(path, manifest)
        print(json.dumps({'status': 'prepared', 'manifest': str(path), 'rootfs_bytes': manifest['rootfs_bytes']}))
        return
    assert a.linux_commit is not None, 'finalize requires frozen --linux-commit'
    manifest = json.loads(path.read_text())
    assert manifest['status'] == 'prepared', 'finalize only a prepared package; no retry/overwrite'
    base.git(PLATFORM/'linux', 'merge-base', '--is-ancestor', M, a.linux_commit)
    lock = json.loads(base.show(PLATFORM, a.platform_commit, 'sources.lock.json'))
    components = {x['path']: x['commit'] for x in lock['components']}
    assert components['linux'] == a.linux_commit and components['cva6'] == CVA6
    assert base.git(PLATFORM, 'rev-parse', a.platform_commit+':linux').decode().strip() == a.linux_commit
    assert base.git(PLATFORM, 'rev-parse', a.platform_commit+':cva6').decode().strip() == CVA6
    assert base.show(PLATFORM, a.platform_commit, 'bench/idma_p0.c') == (out/'inputs/bench/idma_p0.c').read_bytes()
    manifest.update(status='building', final_build_started=base.now(), selected_linux=a.linux_commit,
                    selected_platform=a.platform_commit, selected_cva6=CVA6, selected_opensbi=base.SBI,
                    finalized_builder_sha256=base.sha(__file__))
    base.save(path, manifest)
    try:
        with (out/'build.log').open('ab') as log:
            linux, sbi = out/'src/linux', out/'src/opensbi'
            base.archive(PLATFORM/'linux', a.linux_commit, linux)
            base.archive(PLATFORM/'opensbi', base.SBI, sbi)
            for name in ['idma.h', 'idma_p0.h']:
                assert (linux/'include/uapi/linux'/name).read_bytes() == (out/'uapi/linux'/name).read_bytes(), 'P0/legacy UAPI changed after preparation'
            for header in (linux/'include/uapi/linux').glob('idma*.h'):
                shutil.copyfile(header, out/'uapi/linux'/header.name)
            for extra in a.extra_program:
                name, source = extra.split('=', 1)
                assert re.fullmatch(r'[A-Za-z][A-Za-z0-9_-]*', name)
                assert name not in ['idma_experiment', 'idma_p0', 'platform-timeout', 'platform-sd-update']
                target = out/'inputs/bench'/(name+'.c')
                shutil.copyfile(source, target)
                manifest.setdefault('extra_program_sources', []).append(
                    {'name': name, 'source': str(Path(source).resolve()), 'sha256': base.sha(target)})
                base.run(['riscv64-linux-gnu-gcc', '-O2', '-static', '-Wall', '-Wextra', '-Werror',
                          '-I', out/'uapi', target, '-o', out/'bin'/name], out, log, manifest, path)
            rootfs(base, out, manifest)
            kernel = out/'linux-build'; kernel.mkdir()
            config = kernel/'.config'; shutil.copyfile(out/'inputs/configs/linux_defconfig', config)
            base.run([linux/'scripts/config', '--file', config, '--set-str', 'INITRAMFS_SOURCE', out/'rootfs.cpio',
                      '--set-str', 'LOCALVERSION', RELEASE_SUFFIX, '--disable', 'LOCALVERSION_AUTO'], out, log, manifest, path)
            common = ['make', '-C', linux, 'ARCH=riscv', 'O='+str(kernel), 'CROSS_COMPILE=riscv64-linux-gnu-']
            base.run(common+['olddefconfig'], out, log, manifest, path)
            critical = ['drivers/misc/idma.c', 'include/uapi/linux/idma.h', 'include/uapi/linux/idma_p0.h',
                        'drivers/iommu/riscv/iommu-hpm.c', 'include/linux/riscv_iommu_hpm.h',
                        'drivers/iommu/riscv/io_pgtable.c', 'drivers/iommu/riscv/iommu.c']
            critical += [str(h.relative_to(linux)) for h in (linux/'include/uapi/linux').glob('idma_multi*.h')]
            critical += ['include/uapi/linux/idma_pages.h', 'include/linux/riscv_iommu_leaf.h']
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
            assert size <= MAX_PAYLOAD, '32MiB SD payload budget exceeded; stop without changing configuration'
            release = (kernel/'include/config/kernel.release').read_text().strip()
            assert release.endswith(RELEASE_SUFFIX)
            # Match the actual final kernel exports/release, using unmodified SPI sources.
            modules = out/'spi-recovery'; modules.mkdir()
            for name in ['spi-bitbang.c', 'spi-xilinx.c']:
                shutil.copyfile(linux/'drivers/spi'/name, modules/name)
            (modules/'Makefile').write_text('obj-m += spi-bitbang.o\nobj-m += spi-xilinx.o\n')
            base.run(common+['KERNELRELEASE='+release, '-j'+str(a.jobs), 'modules_prepare'], out, log, manifest, path)
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
