#!/usr/bin/env python3
"""Prepare fixed G CLI/rootfs; finalize exactly one ABI6 Image/payload build.

No board operation. Reuse the passed full-E3 rootfs/config/DTS and original
kernel/lloader/OpenSBI/SPI flow; old binaries are retained byte-for-byte.
"""
import argparse
import hashlib
import importlib.util
import json
from pathlib import Path
import shutil
import stat

HERE = Path(__file__).resolve().parent
PLATFORM = Path('/home/xvhaocheng/work/iommu-platform')
PLATFORM_COMMIT = '6491dfd76fc92dc020d17166b327da93605f58b1'
LINUX = '69d8b9d4549c473477bcdc1df7c09ee1bb55528c'
CVA6 = 'cfb9a0a8bb70b32be6036bc112a5189d6e2aa0ab'
BASE_SW = Path('/home/xvhaocheng/work-a2-workloads/paper/ispass2027/exp/E3-workloads/artifacts/full-e3-software01')
BASE_MANIFEST_SHA = '7415dcc4d076fa434bc22294f4309582af84086964f1b47bb05c695555f0bcc4'
RELEASE_SUFFIX = '-ispass2027-g-v1'
MAX_PAYLOAD = 33554432


def library():
    spec = importlib.util.spec_from_file_location('g_baseline_helpers', HERE/'source/build_baseline.as-reused.py')
    base = importlib.util.module_from_spec(spec); spec.loader.exec_module(base)
    base.WORK = PLATFORM.parent; base.PLATFORM = PLATFORM
    archive = base.load_module(HERE/'source/platform.as-reused.py', 'g_platform_archive')
    archive.ROOT = PLATFORM
    base.archive = lambda repo, commit, dest: archive.stage_repo(
        {'path': str(repo.relative_to(PLATFORM)), 'commit': commit}, dest)
    return base


def frozen(base):
    lock = json.loads(base.show(PLATFORM, PLATFORM_COMMIT, 'sources.lock.json'))
    components = {c['path']: c['commit'] for c in lock['components']}
    for name, commit in [('linux', LINUX), ('cva6', CVA6), ('opensbi', base.SBI)]:
        assert components[name] == commit
        assert base.git(PLATFORM, 'rev-parse', PLATFORM_COMMIT+':'+name).decode().strip() == commit
    for source, copied in [('scripts/platform.py', 'source/platform.as-reused.py'),
                           ('scripts/rootfs.py', 'source/rootfs.as-reused.py')]:
        assert base.show(PLATFORM, PLATFORM_COMMIT, source) == (HERE/copied).read_bytes()
    assert base.sha(BASE_SW/'build-manifest.json') == BASE_MANIFEST_SHA
    common = json.loads((BASE_SW/'build-manifest.json').read_text())
    assert common['status'] == 'passed' and common['selected_linux'] == '65b427946152e3aca27babab0a437821729805d4'
    assert base.sha(BASE_SW/'rootfs.cpio') == common['artifacts']['rootfs.cpio']
    for name in ['linux_defconfig', 'platform.dts']:
        assert base.sha(BASE_SW/'inputs/configs'/name) == common['handwritten_inputs']['configs/'+name]
    return common


def rootfs(base, out, manifest):
    module = base.load_module(HERE/'source/rootfs.as-reused.py', 'g_rootfs')
    entries = list(module.read_newc((BASE_SW/'rootfs.cpio').read_bytes()))
    assert len({str(Path(n)) for n, _, _ in entries}) == len(entries)
    assert not any(Path(n).name == 'idma_g' for n, _, _ in entries)
    packed = bytearray()
    for name, fields, body in entries:
        module.write_entry(packed, name, fields, body)
    binary = (out/'bin/idma_g').read_bytes()
    module.write_entry(packed, 'usr/bin/idma_g',
        [max(f[0] for _, f, _ in entries)+1, stat.S_IFREG|0o755, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0], binary)
    module.write_entry(packed, 'TRAILER!!!', [0]*13, b'')
    packed.extend(b'\0'*((-len(packed)) % 512))
    reread = {n: (f, b) for n, f, b in module.read_newc(packed)}
    assert len(reread) == len(entries)+1
    for name, fields, body in entries:
        # write_entry deterministically zeros mtime; all original bodies/modes
        # including CLI, network scripts and device nodes remain as before.
        assert reread[name][1] == body and reread[name][0][1] == fields[1]
    assert reread['usr/bin/idma_g'][1] == binary
    (out/'rootfs.cpio').write_bytes(packed)
    manifest.update(rootfs_sha256=base.sha(out/'rootfs.cpio'), rootfs_bytes=len(packed),
        preserved_rootfs_entries=len(entries), preserved_executables={n: hashlib.sha256(b).hexdigest()
            for n, f, b in entries if stat.S_ISREG(f[1]) and f[1] & 0o111},
        rootfs_added_binary='usr/bin/idma_g', payload_limit_bytes=MAX_PAYLOAD)


def main():
    p = argparse.ArgumentParser(description=__doc__)
    p.add_argument('action', choices=['prepare', 'finalize'])
    p.add_argument('--out', required=True, type=Path)
    p.add_argument('--jobs', type=int, default=8)
    a = p.parse_args()
    assert a.jobs > 0
    base, out = library(), a.out.resolve()
    assert out.is_relative_to((HERE/'artifacts').resolve()), 'out must stay in this E6 worktree artifacts/'
    common = frozen(base)
    path = out/'build-manifest.json'
    if a.action == 'prepare':
        out.mkdir(parents=True, exist_ok=False)
        manifest = {'schema': 'ispass2027-g-software-v1', 'status': 'preparing', 'start': base.now(),
            'script_sha256': base.sha(__file__), 'commands': [], 'board_operations': False,
            'selected_platform': PLATFORM_COMMIT, 'selected_linux': LINUX, 'selected_cva6': CVA6,
            'selected_cva6_reference_only': True, 'selected_cva6_role': 'source_reference_not_loaded_bit',
            'selected_opensbi': base.SBI, 'new_kernel_release_suffix': RELEASE_SUFFIX,
            'common_software_manifest': str(BASE_SW/'build-manifest.json'),
            'common_software_manifest_sha256': BASE_MANIFEST_SHA,
            'common_rootfs_sha256': common['artifacts']['rootfs.cpio'],
            'helper_sha256': {str(p.relative_to(HERE)): base.sha(p) for p in [
                HERE/'source/build_baseline.as-reused.py', HERE/'source/platform.as-reused.py', HERE/'source/rootfs.as-reused.py']}}
        base.save(path, manifest)
        try:
            (out/'inputs/bench').mkdir(parents=True); (out/'inputs/configs').mkdir()
            (out/'uapi/linux').mkdir(parents=True); (out/'bin').mkdir()
            for name in ['linux_defconfig', 'platform.dts']:
                shutil.copyfile(BASE_SW/'inputs/configs'/name, out/'inputs/configs'/name)
            for name in ['idma.h', 'idma_g.h']:
                (out/'uapi/linux'/name).write_bytes(base.show(PLATFORM/'linux', LINUX, 'include/uapi/linux/'+name))
            cli = out/'inputs/bench/idma_g.c'
            cli.write_bytes(base.show(PLATFORM, PLATFORM_COMMIT, 'bench/idma_g.c'))
            with (out/'build.log').open('xb') as log:
                base.run(['riscv64-linux-gnu-gcc', '-O2', '-static', '-Wall', '-Wextra', '-Werror',
                          '-I', out/'uapi', cli, '-o', out/'bin/idma_g'], out, log, manifest, path)
            rootfs(base, out, manifest)
            manifest.update(status='prepared', prepared_utc=base.now(),
                prepared_inputs=base.tree_hashes(out/'inputs'), prepared_uapi=base.tree_hashes(out/'uapi'),
                g_cli_sha256=base.sha(out/'bin/idma_g'),
                note='CLI/rootfs only; Image not built. Formal finalize refuses retries or byte changes.')
        except BaseException as error:
            manifest.update(status='failed', error=repr(error)); raise
        finally:
            base.save(path, manifest)
        print(json.dumps({'status': manifest['status'], 'manifest': str(path)})); return
    manifest = json.loads(path.read_text())
    assert manifest['status'] == 'prepared', 'only a prepared package; no retry/overwrite'
    assert manifest['script_sha256'] == base.sha(__file__)
    assert manifest['selected_linux'] == LINUX and manifest['selected_platform'] == PLATFORM_COMMIT
    assert manifest['rootfs_sha256'] == base.sha(out/'rootfs.cpio')
    assert manifest['g_cli_sha256'] == base.sha(out/'bin/idma_g')
    assert manifest['prepared_inputs'] == base.tree_hashes(out/'inputs')
    assert manifest['prepared_uapi'] == base.tree_hashes(out/'uapi')
    assert all(base.sha(HERE/name) == digest for name, digest in manifest['helper_sha256'].items())
    manifest.update(status='building', final_build_started=base.now())
    base.save(path, manifest)
    try:
        with (out/'build.log').open('ab') as log:
            linux, sbi = out/'src/linux', out/'src/opensbi'
            base.archive(PLATFORM/'linux', LINUX, linux)
            base.archive(PLATFORM/'opensbi', base.SBI, sbi)
            for name in ['idma.h', 'idma_g.h']:
                assert (linux/'include/uapi/linux'/name).read_bytes() == (out/'uapi/linux'/name).read_bytes()
            kernel = out/'linux-build'; kernel.mkdir()
            config = kernel/'.config'; shutil.copyfile(out/'inputs/configs/linux_defconfig', config)
            base.run([linux/'scripts/config', '--file', config, '--set-str', 'INITRAMFS_SOURCE', out/'rootfs.cpio',
                      '--set-str', 'LOCALVERSION', RELEASE_SUFFIX, '--disable', 'LOCALVERSION_AUTO'], out, log, manifest, path)
            make = ['make', '-C', linux, 'ARCH=riscv', 'O='+str(kernel), 'CROSS_COMPILE=riscv64-linux-gnu-']
            base.run(make+['olddefconfig'], out, log, manifest, path)
            critical = ['drivers/misc/idma.c', 'drivers/misc/idma-g.h', 'drivers/misc/idma-workload.h',
                'drivers/iommu/riscv/iommu-hpm.c', 'include/linux/riscv_iommu_hpm.h',
                'drivers/iommu/riscv/io_pgtable.c', 'drivers/iommu/riscv/iommu.c',
                'include/linux/riscv_iommu_leaf.h', 'mm/maccess.c']
            critical += [str(h.relative_to(linux)) for h in sorted((linux/'include/uapi/linux').glob('idma*.h'))]
            manifest.update(kernel_config_sha256=base.sha(config), handwritten_inputs=base.tree_hashes(out/'inputs'),
                kernel_critical_sources={name: base.sha(linux/name) for name in critical})
            base.save(path, manifest)
            base.run(make+['-j'+str(a.jobs), 'Image'], out, log, manifest, path)
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
            assert size <= MAX_PAYLOAD, '32MiB payload budget exceeded; stop without configuration changes'
            release = (kernel/'include/config/kernel.release').read_text().strip()
            assert release == '6.5.0-rc2'+RELEASE_SUFFIX
            modules = out/'spi-recovery'; modules.mkdir()
            for name in ['spi-bitbang.c', 'spi-xilinx.c']:
                shutil.copyfile(linux/'drivers/spi'/name, modules/name)
            (modules/'Makefile').write_text('obj-m += spi-bitbang.o\nobj-m += spi-xilinx.o\n')
            base.run(make+['KERNELRELEASE='+release, '-j'+str(a.jobs), 'modules_prepare'], out, log, manifest, path)
            assert (kernel/'scripts/module.lds').is_file()
            shutil.copyfile(kernel/'vmlinux.symvers', kernel/'Module.symvers')
            base.run(make+['KERNELRELEASE='+release, '-j'+str(a.jobs), 'M='+str(modules), 'modules'], out, log, manifest, path)
            artifacts = [out/'fw_payload.bin', image, dtb, out/'rootfs.cpio', out/'bin/idma_g',
                         modules/'spi-bitbang.ko', modules/'spi-xilinx.ko']
            manifest.update(status='passed', kernel_release=release, payload_bytes=size,
                payload_headroom_bytes=MAX_PAYLOAD-size,
                artifacts={str(x.relative_to(out)): base.sha(x) for x in artifacts},
                recovery={'kernel_release': release, 'load_order': ['spi-bitbang.ko', 'spi-xilinx.ko'],
                          'policy': 'Software update only; not loaded during formal measurement'})
    except BaseException as error:
        manifest.update(status='failed', error=repr(error)); raise
    finally:
        manifest['end'] = base.now(); base.save(path, manifest)
    print(json.dumps({'status': manifest['status'], 'kernel_release': release,
                      'payload_bytes': size, 'manifest': str(path)}))


if __name__ == '__main__':
    main()
