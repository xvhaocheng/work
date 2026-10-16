#!/usr/bin/env python3
"""Build the pinned platform without writing into component source trees."""
import argparse
from datetime import datetime, timezone
import hashlib
import json
import os
from pathlib import Path
import re
import shutil
import subprocess
import sys
import tarfile

ROOT = Path(__file__).resolve().parents[1]
LOCAL = json.loads((ROOT / 'local.json').read_text()) if (ROOT / 'local.json').exists() else {}
ENV = dict(os.environ)
ENV.setdefault('RISCV', LOCAL.get('riscv_prefix', '/home/xvhaocheng/RISCV/riscv_tools'))
CC = LOCAL.get('cross_compile', 'riscv64-linux-gnu-')
BARE_CC = LOCAL.get('bare_cross_compile', 'riscv64-unknown-elf-')

def run(args, cwd=ROOT, **kwargs):
    print('+', ' '.join(map(str, args)), flush=True)
    subprocess.run(list(map(str, args)), cwd=cwd, env=ENV, check=True, **kwargs)

def capture(args, cwd=ROOT):
    return subprocess.check_output(list(map(str, args)), cwd=cwd, env=ENV, text=True).strip()

def sha(path):
    with Path(path).open('rb') as f:
        digest = hashlib.sha256()
        for chunk in iter(lambda: f.read(1024 * 1024), b''):
            digest.update(chunk)
        return digest.hexdigest()

def records():
    return json.loads((ROOT / 'sources.lock.json').read_text())['components']

def check():
    for item in records():
        p = ROOT / item['path']
        actual = capture(['git', 'rev-parse', 'HEAD'], p)
        if actual != item['commit']:
            raise RuntimeError(f"Version mismatch: {item['path']}")
        if capture(['git', '--no-optional-locks', 'status', '--porcelain', '--untracked-files=normal'], p):
            raise RuntimeError(f"Dirty component: {item['path']}; commit changes and update lock before building")
        alt = Path(capture(['git', 'rev-parse', '--git-path', 'objects/info/alternates'], p))
        if not alt.is_absolute():
            alt = p / alt
        if alt.exists():
            raise RuntimeError(f"External Git objects: {item['path']}")
    # Cross-check every parent gitlink rather than trusting two independent lists.
    for item in records():
        parents = [x for x in records() if item['path'].startswith(x['path'] + '/')]
        parent = max(parents, key=lambda x: len(x['path']))['path'] if parents else ''
        rel = str(Path(item['path']).relative_to(parent)) if parent else item['path']
        line = capture(['git', 'ls-tree', 'HEAD', '--', rel], ROOT / parent)
        if not line.startswith('160000 commit ' + item['commit'] + '\t'):
            raise RuntimeError(f"Gitlink mismatch: {item['path']}")
    print(f'PASS: {len(records())} pinned components; clean, gitlinks match, no alternates')

def doctor():
    status = {}
    for tool in [CC+'gcc', BARE_CC+'gcc', 'make', 'dtc', 'verilator', 'vivado']:
        executable = shutil.which(tool)
        status[tool] = executable
        print(tool, executable or 'MISSING')
    print('RISCV =', ENV['RISCV'])
    return status

def bench():
    dest = ROOT / 'out/bench'
    dest.mkdir(parents=True, exist_ok=True)
    for src in sorted((ROOT / 'bench').glob('*.c')):
        run([CC+'gcc', '-O2', '-static', '-Wall', '-Wextra', src, '-o', dest/src.stem])

def rootfs():
    bench()
    run([sys.executable, ROOT/'scripts/rootfs.py'])

def kernel(jobs):
    rootfs()
    dest = ROOT / 'out/linux'
    dest.mkdir(parents=True, exist_ok=True)
    config = dest / '.config'
    if not config.exists():
        shutil.copy2(ROOT/'configs/linux_defconfig', config)
    run([ROOT/'linux/scripts/config', '--file', config, '--set-str', 'INITRAMFS_SOURCE', ROOT/'out/rootfs.cpio'])
    run(['make', '-C', ROOT/'linux', 'ARCH=riscv', f'O={dest}', f'CROSS_COMPILE={CC}', 'olddefconfig'])
    run(['make', '-C', ROOT/'linux', 'ARCH=riscv', f'O={dest}', f'CROSS_COMPILE={CC}', f'-j{jobs}', 'Image'])
    run(['dtc', '-I', 'dts', '-O', 'dtb', '-o', dest/'platform.dtb', ROOT/'configs/platform.dts'])

def firmware(jobs):
    image = ROOT/'out/linux/arch/riscv/boot/Image'
    if not image.exists():
        kernel(jobs)
    loader = ROOT/'out/lloader'
    loader.mkdir(parents=True, exist_ok=True)
    # Copy only tracked loader sources, never a stale loader binary.
    paths = capture(['git', 'ls-files', 'tools/lloader'], ROOT/'linux').splitlines()
    for name in paths:
        source = ROOT/'linux'/name
        if source.is_file():
            target = loader / source.relative_to(ROOT/'linux/tools/lloader')
            target.parent.mkdir(parents=True, exist_ok=True)
            shutil.copy2(source, target)
    run(['make', f'CROSS_COMPILE={BARE_CC}', 'ARCH=rv64', f'IMAGE={image}',
         f'DTB={ROOT / "out/linux/platform.dtb"}', 'TARGET=linux-rv64-cva6'], cwd=loader)
    run(['make', '-C', ROOT/'opensbi', f'O={ROOT / "out/opensbi"}', f'CROSS_COMPILE={CC}',
         'PLATFORM=fpga/ariane', 'FW_PAYLOAD=y', f'FW_PAYLOAD_PATH={loader / "linux-rv64-cva6.bin"}', f'-j{jobs}'])

def stage_repo(item, dest):
    dest.mkdir(parents=True, exist_ok=True)
    proc = subprocess.Popen(['git', '-C', str(ROOT/item['path']), 'archive', item['commit']], stdout=subprocess.PIPE)
    with tarfile.open(fileobj=proc.stdout, mode='r|') as archive:
        archive.extractall(dest, filter='data')
    if proc.wait():
        raise RuntimeError(f"Archive failed: {item['path']}")

def fpga(profile):
    cfg = json.loads((ROOT/'configs'/f'{profile}.json').read_text())
    if cfg['num_dma'] != 4 or cfg['device_ids'] != [10,11,12,13]:
        raise RuntimeError('Only the migrated four-DMA wiring/DTS is validated by this builder')
    stage = ROOT/'out'/profile/'fpga-src'
    if stage.exists():
        raise RuntimeError(f'{stage} exists. Use a fresh output directory/profile for a new hardware build.')
    for item in records():
        if item['path'] == 'cva6' or item['path'].startswith('cva6/'):
            stage_repo(item, stage/item['path'])
    cva6 = stage/'cva6'
    top = cva6/'corev_apu/fpga/src/ariane_xilinx.sv'
    data = top.read_text()
    for name, val in [('IOMMUDSFixedDelayInput', cfg['ds_delay_input']), ('IOMMUDSFixedDelayOutput', cfg['ds_delay_output'])]:
        data, n = re.subn(r'(localparam int unsigned '+name+r'\s*=\s*)\d+', lambda m:m[1]+str(val), data)
        if n != 1: raise RuntimeError(f'Missing unique hardware parameter: {name}')
    top.write_text(data)
    soc = cva6/'corev_apu/tb/ariane_soc_pkg.sv'
    data, n = re.subn(r'(NumIotlbEntries\s*:\s*32\x27d)\d+', lambda m:m[1]+str(cfg['iotlb_entries']), soc.read_text())
    if n != 1: raise RuntimeError('Missing IOTLB parameter')
    soc.write_text(data)
    run(['make', '-C', cva6, f'BOARD={cfg["board"]}', 'fpga'])

def hpm_test():
    dest=ROOT/'out/hpm-test'
    dest.mkdir(parents=True, exist_ok=True)
    iommu=ROOT/'cva6/corev_apu/riscv-iommu'
    run(['verilator', '--cc', '--exe', '--build', '-Wno-fatal', '--top-module', 'hpm_test_top',
         '--Mdir', dest, iommu/'packages/rv_iommu_pkg.sv', iommu/'packages/rv_iommu_cfg_pkg.sv',
         iommu/'packages/rv_iommu_reg_pkg.sv', iommu/'rtl/software_interface/rv_iommu_hpm.sv',
         ROOT/'tests/hpm_test_top.sv', ROOT/'tests/hpm_test.cpp'])
    run([dest/'Vhpm_test_top'])

def bundle():
    dest=ROOT/'out/source-bundles'
    dest.mkdir(parents=True, exist_ok=True)
    info=[]
    for item in [{'path':'.','commit':capture(['git','rev-parse','HEAD'])}]+records():
        name=item['path'].replace('/','__').replace('.','platform')+'.bundle'
        file=dest/name
        run(['git','-C',ROOT/item['path'],'-c','pack.threads=2','bundle','create',file,'HEAD'])
        info.append(dict(item, bundle=name, sha256=sha(file)))
    (dest/'bundles.json').write_text(json.dumps(info,indent=2)+'\n')
    shutil.copy2(ROOT/'scripts/restore.py',dest/'restore.py')

def manifest(action, profile, status):
    dest=ROOT/'out/manifests'
    dest.mkdir(parents=True, exist_ok=True)
    paths=['out/rootfs.cpio','out/linux/arch/riscv/boot/Image','out/linux/platform.dtb',
           'out/opensbi/platform/fpga/ariane/firmware/fw_payload.bin',
           f'out/{profile}/fpga-src/cva6/corev_apu/fpga/work-fpga/ariane_xilinx.bit']
    data={'timestamp':datetime.now(timezone.utc).isoformat(),'action':action,'status':status,
          'profile':json.loads((ROOT/'configs'/f'{profile}.json').read_text()),
          'platform_commit':capture(['git','rev-parse','HEAD']),
          'platform_dirty':bool(capture(['git','status','--porcelain'])),
          'sources_lock_sha256':sha(ROOT/'sources.lock.json'),
          'artifacts':{p:sha(ROOT/p) for p in paths if (ROOT/p).exists()}}
    (dest/f'{action}-{profile}.json').write_text(json.dumps(data,indent=2)+'\n')

def main():
    parser=argparse.ArgumentParser()
    parser.add_argument('action',choices=['doctor','check','bench','rootfs','kernel','firmware','software','hpm-test','fpga','bundle'])
    parser.add_argument('--profile',default='migration-4dma',choices=['migration-4dma','baseline-4dma'])
    parser.add_argument('--jobs',type=int,default=12)
    args=parser.parse_args()
    try:
        if args.action not in ['doctor','bundle']:
            check()
        if args.action=='doctor': doctor()
        elif args.action=='bench': bench()
        elif args.action=='rootfs': rootfs()
        elif args.action=='kernel': kernel(args.jobs)
        elif args.action=='firmware': firmware(args.jobs)
        elif args.action=='software': kernel(args.jobs); firmware(args.jobs)
        elif args.action=='hpm-test': hpm_test()
        elif args.action=='fpga': fpga(args.profile)
        elif args.action=='bundle': check(); bundle()
    except Exception:
        manifest(args.action,args.profile,'failed')
        raise
    manifest(args.action,args.profile,'passed')

if __name__=='__main__': main()
