#!/usr/bin/env python3
"""Regenerate exact archive-based patches and a navigable implementation lock."""
from pathlib import Path
import tarfile,difflib,hashlib,json,shutil,subprocess,datetime
root=Path(__file__).resolve().parent.parent
old=root.parent.parent/'a2-native-sva-binding-20261015/sources/date-raw'

def sha(p):
 h=hashlib.sha256()
 with p.open('rb') as f:
  for b in iter(lambda:f.read(1024*1024),b''):h.update(b)
 return h.hexdigest()
changes=[]
for label,archive,target,new_files in [
 ('hero-tools-47339e0-a2','hero-tools-carfield-archive.tgz','hero-tools',['sw/hero-driver/common/hero_platform_gate.c']),
 ('cva6-sdk-fb56489c-linux69-lock','sdk-ck-next-archive.tgz','cva6-sdk',[])]:
 pristine={}
 with tarfile.open(old/archive) as t:
  for m in t.getmembers():
   if m.isfile():pristine[str(Path(*Path(m.name).parts[1:]))]=t.extractfile(m).read()
 patch=[];verify=root/'build/patch-verify'/target
 for name in sorted(set(pristine)|set(new_files)):
  p=root/'source'/target/name
  if not p.is_file():continue
  before=pristine.get(name,b'');after=p.read_bytes()
  if before==after:continue
  patch.extend(difflib.unified_diff(before.decode().splitlines(True),after.decode().splitlines(True),
    fromfile='a/'+name if name in pristine else '/dev/null',tofile='b/'+name))
  dst=verify/name;dst.parent.mkdir(parents=True,exist_ok=True)
  if name in pristine:dst.write_bytes(before)
  changes.append({'repository':target,'file':name,'source_sha256':hashlib.sha256(before).hexdigest() if name in pristine else None,'patched_sha256':hashlib.sha256(after).hexdigest()})
 patch_file=root/'patches'/f'{label}.patch';patch_file.write_text(''.join(patch))
 r=subprocess.run(['patch','--batch','--dry-run','-p1','-d',str(verify),'-i',str(patch_file)],text=True,capture_output=True)
 (root/'logs'/f'{label}-patch-check.log').write_text(r.stdout+r.stderr)
 if r.returncode:raise RuntimeError(label)
(root/'evidence/changed-source-manifest.json').write_text(json.dumps(changes,indent=2)+'\n')
for name,dst in [('hero-tools-carfield-archive.tgz','hero-tools-47339e0.tar.gz'),('sdk-ck-next-archive.tgz','cva6-sdk-fb56489c.tar.gz')]:
 p=root/'source'/dst
 if not p.exists():shutil.copy2(old/name,p)
pins={
 'hero_tools':('https://github.com/pulp-platform/hero-tools','47339e040c43ab4d44d9a870bc307251084a1ba4'),
 'sdk':('https://github.com/pulp-platform/cva6-sdk','fb56489c97b41471b6b9d707f0a38b3bc789a76e'),
 'llvm':('https://github.com/pulp-platform/llvm-project','d8cffb8d7294483b45732283719e250116fbe6c0'),
 'linux':('https://github.com/CyrilKoe/linux','e4b0725a5089f6cc0fc5afec06f3091d98dffa68'),
 'carfield':('https://github.com/pulp-platform/carfield','dbb4ad3380e6e81fa7eac3955e2340752f29dab0'),
 'snitch':('https://github.com/pulp-platform/snitch','269830af872fa943d8b079470120d42d62e1cb69'),
 'buildroot':('https://git.buildroot.net/buildroot','aa433d1c5cfbd72b64ff3f92f2ffa2e02ea7089b'),
 'o1heap':('https://github.com/pavel-kirienko/o1heap','10e2754a3b721b77d1f3f92227f3a64ad546f956'),
 'newlib':('https://sourceware.org/git/newlib-cygwin.git','6d79e0a58866548f435527798fbd4a6849d05bc7'),
}
archives=[{'file':str(p.relative_to(root)),'bytes':p.stat().st_size,'sha256':sha(p)} for p in sorted((root/'source').glob('*.tar.*'))]
configs=[{'file':str(p.relative_to(root)),'sha256':sha(p)} for p in sorted((root/'config').glob('*')) if p.is_file() and p.name!='version-lock.json']
lock={'schema':'a2-hero-fixed-source-and-build-lock-v1','generated_utc':datetime.datetime.now(datetime.timezone.utc).isoformat(),
 'scope':'isolated production-unapplied repairs; no board/DMA/FPGA run',
 'paper_exact_experimental_snapshot_proven':False,
 'fixed_sources':{k:{'repository':url,'commit':ref} for k,(url,ref) in pins.items()},
 'archives':archives,'configs':configs,
 'additional_host_dependencies':{'libffi':{'version':'3.4.2','sdk_recipe':'evidence/buildroot-libffi.mk','expected_sha256':'540fb721619a6aba3bdeef7d940d8e9e0e6d2c193595bc243241b77ff9e93620'},'elfutils':{'version':'0.184','sdk_recipe':'evidence/buildroot-elfutils.mk','expected_sha512':'5920c354edd6b28bb7e713d674bf6f2221e8ee91b90d582de39b1f45446aea1e7e3d4b6671024c391675b5e7530fe952c1320d730bb625c02d4c647ceb938bc8'},'zlib':{'version':'1.2.11','sdk_recipe':'evidence/buildroot-libzlib.mk','expected_sha256':'4ff941449631ace0d4d203e3483be9dbc9da454084111f97ea0a2114e19bf066'}},
 'profile':{'host':'RV64 Linux/CVA6','target':'Carfield Snitch RV32IMAFD','kernel':'6.9.0 at exact e4b0725','workload':'FP32 AXPY n divisible by 512; do_map=1; noise=0; cold and hot independently restored and verified','abi':'native RV64 legacy ioctl arg structure; no 32-bit compat ABI'},
 'kernel_compile':{'script':'scripts/compile-kernel.sh','compiler':'riscv64-linux-gnu-gcc 11.4.0','resolved_config':'config/linux-6.9.compile-resolved.config','output':'evidence/carfield-linux-6.9.o','full_ko_modpost':False},
 'actual_host_compile_variant':'Ubuntu GCC11.4/glibc2.35 with isolated sysroot layout; not paper exact Buildroot execution',
 'host_linker_abi_compatibility':{'wrapper':'scripts/hero-host-ld-compat.py','scope':'fixed d8 RV64 .rela.eh_frame producer61 to standard57 in copies only','evidence':'evidence/final-elf-acceptance.json','default_link_attempt_rejected':'logs/hero-offload-final-link.log'},
 'native_toolchain_build':{'source_commit':pins['llvm'][1],'cmake':'3.28.3 isolated pip wheel','configuration':'config/llvm-host-cache.cmake','script':'scripts/build-llvm-minimal.sh','scope':'Release, RISCV backend only; exact custom Hero LLVM tools; OpenMP RV64 runtime and RV32 newlib/compiler-rt separately'},
 'hardware_implementation_gaps':['hero_platform_quiesce: stop every publisher in this domain and drain accepted DMA and final write responses','hero_platform_invalidate_complete: context/IOTLB invalidation and error/timeout-aware physical completion receipt; current generic iommu_unmap alone is not accepted','hero_platform_cache_sync: complete CPU/private-cache plus LLC clean/invalidate for mapped pages and physical bypass alias'],
 'default_hardware_gate':'actual implementation returns -EOPNOTSUPP before pin/map; no module-parameter bypass; no claim these hooks are implemented',
 'changed_files':changes}
(root/'config/version-lock.json').write_text(json.dumps(lock,indent=2)+'\n')
print(json.dumps({'changed_files':len(changes),'patches':[str(p.name) for p in (root/'patches').glob('*.patch')],'archive_count':len(archives)},indent=2))
